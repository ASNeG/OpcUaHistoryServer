/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <boost/asio.hpp>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/Utility.h"
#include "OpcUaHistory/HistoryAccessFile/FileReadEntry.h"

#define MAX_RECORD_SIZE 1000000

namespace OpcUaHistory
{

	FileReadEntry::FileReadEntry(void)
	: DoublyLinkedList()
	, verbose_(false)
	, valueName_("")
	, baseFolder_()
	, valueFolder_()
	, dataFile_()
	, dataFolderList_()
	, dataFileList_()
	, dataFolder_()
	, ifs_()
	, from_()
	, to_()
	, maxResultEntriesReached_(false)
	{
	}

	FileReadEntry::~FileReadEntry(void)
	{
		if (ifs_.is_open()) {
			ifs_.close();
		}
	}

	void
	FileReadEntry::verbose(bool verbose)
	{
		verbose_ = verbose;
	}

	bool
	FileReadEntry::verbose(void)
	{
		return verbose_;
	}

	void
	FileReadEntry::valueName(const std::string& valueName)
	{
		valueName_ = valueName;
		valueFolder_ = baseFolder_ / boost::filesystem::path(valueName_);
	}

	void
	FileReadEntry::baseFolder(const boost::filesystem::path& baseFolder)
	{
		baseFolder_ = baseFolder;
		valueFolder_ = baseFolder_ / boost::filesystem::path(valueName_);
	}

	void
	FileReadEntry::dateTimeFrom(OpcUaDateTime& from)
	{
		from_ = from;
	}

	void
	FileReadEntry::dateTimeTo(OpcUaDateTime& to)
	{
		to_ = to;
	}

	bool
	FileReadEntry::maxResultEntriesReached(void)
	{
		return maxResultEntriesReached_;
	}

	bool
	FileReadEntry::readInitial(OpcUaDataValue::Vec& dataValueVec, uint32_t maxResultEntries)
	{
		maxResultEntriesReached_ = false;

		// check if base folder exists
		if (!boost::filesystem::exists(baseFolder_)) {
			Log(Error, "base folder not exist")
				.parameter("BaseFolder", baseFolder_.string());
			return false;
		}

		// check if value folder exists
		if (!boost::filesystem::exists(valueFolder_)) {
			Log(Error, "value folder not exist")
				.parameter("ValueFolder", valueFolder_.string());
			return false;
		}

		// get data folder
		if (dataFolderList_.empty()) {
			if (!getDataFolderList()) {
				return false;
			}
		}
		if (dataFolderList_.empty()) {
			return true;
		}

		// read entries
		return readNext(dataValueVec, maxResultEntries);
	}

	bool
	FileReadEntry::readNext(OpcUaDataValue::Vec& dataValueVec, uint32_t maxResultEntries)
	{
		maxResultEntriesReached_ = false;
		uint32_t numberResultEntries = 0;

		while (true)
		{
			// check if file is open. If not open a new file
			if (!ifs_.is_open()) {

				// when file list is empty read a new file list
				if (dataFileList_.empty()) {
					if (!getDataFileList()) {
						return false;
					}
				}

				// check file list an open new file
				if (dataFileList_.empty()) return true;
				dataFile_ = dataFolder_ / dataFileList_.front();
				dataFileList_.pop_front();
				ifs_.open(dataFile_.string(), std::ios::in | std::ios::app | std::ios::binary);
				if (ifs_.fail()) {
					Log(Error, "file open error")
						.parameter("FileName", dataFile_.string());
					return false;
				}
			}

			// read size and source timestamp of entry
			char header[12];
			boost::asio::streambuf sbHeader;
			std::iostream iosHeader(&sbHeader);

			uint32_t recordSize;
			OpcUaDateTime sourceTimestamp;

			ifs_.read(header, 12);

#if 0
			std::cout << "RECORD SIZE";
			for (uint32_t idx=0; idx<12; idx++) {
				std::cout << " " << (uint32_t)(header[idx] & 0xFF);
			}
			std::cout << std::endl;
#endif

			if (ifs_.eof()) {
				ifs_.close();
				continue;
			}

			if (ifs_.fail()) {
				ifs_.close();
				Log(Error, "file read error")
					.parameter("FileName", dataFile_.string());
				return false;
			}

			iosHeader.write(header, 12);
			OpcUaNumber::opcUaBinaryDecode(iosHeader, recordSize);
			sourceTimestamp.opcUaBinaryDecode(iosHeader);

			//std::cout << "RecordSize=" << recordSize << std::endl;

			// ignore record dated before from.
			// skip record
			if (sourceTimestamp < from_) {
				if (!skipEntry(recordSize-12)) {
					ifs_.close();
					return false;
				}
				continue;
			}

			//std::cout << "ST=" << sourceTimestamp.toISOString() << std::endl;

			// ignore record dated after to
			// we are ready
			if (sourceTimestamp > to_) {
				ifs_.close();
				return true;
			}

			// check max record size
			if (recordSize > MAX_RECORD_SIZE) {
				Log(Error, "record larger than max record size - skip record")
					.parameter("FileName", dataFile_.string())
					.parameter("SourceTimestamp", sourceTimestamp.toISOString());

				if (!skipEntry(recordSize-12)) {
					ifs_.close();
					return false;
				}

				continue;
			}

			// read record
			char record[MAX_RECORD_SIZE];
			boost::asio::streambuf sbRecord;
			std::iostream iosRecord(&sbRecord);

			ifs_.read(record, recordSize-12);

			if (ifs_.eof()) {
				ifs_.close();

				Log(Error, "read eof in record - skip record")
					.parameter("FileName", dataFile_.string())
					.parameter("SourceTimestamp", sourceTimestamp.toISOString());
				return false;
			}

			if (ifs_.fail()) {
				ifs_.close();

				Log(Error, "read error in record - skip record")
					.parameter("FileName", dataFile_.string())
					.parameter("SourceTimestamp", sourceTimestamp.toISOString());
				return false;
			}

			uint32_t statusCode;
			uint16_t count;
			iosRecord.write(record, recordSize-12);

			OpcUaDataValue::SPtr dataValue = constructSPtr<OpcUaDataValue>();
			dataValue->serverTimestamp().opcUaBinaryDecode(iosRecord);
			OpcUaNumber::opcUaBinaryDecode(iosRecord, statusCode);
			dataValue->statusCode((OpcUaStatusCode)statusCode);
			if (statusCode == Success) {
				dataValue->variant()->opcUaBinaryDecode(iosRecord);
			}
			OpcUaNumber::opcUaBinaryDecode(iosRecord, count);

			dataValueVec.push_back(dataValue);

			if (maxResultEntries != 0) {
				numberResultEntries++;
				if (numberResultEntries >= maxResultEntries) {
					maxResultEntriesReached_ = true;
					return true;
				}
			}
		}

		return true;
	}


	bool
	FileReadEntry::getDataFolderList(void)
	{
		dataFolderList_.clear();
		boost::filesystem::directory_iterator it(valueFolder_);
		boost::filesystem::directory_iterator itend;

		// read data folders
		std::list<std::string> dataFolderList;
		for(; it != itend; it++) {
			if (!boost::filesystem::is_directory(*it)) {
				continue;
			}

			dataFolderList.push_back((*it).path().leaf().string());
		}
		dataFolderList.sort();

		// find range
		std::string fromString = from_.toISOString();
		std::string toString = to_.toISOString();
		std::string lastDataFolder = "";
		std::string actDataFolder = "";

		// find first element in range
		while (!dataFolderList.empty()) {
			actDataFolder = dataFolderList.front();
			dataFolderList.pop_front();

			if (actDataFolder >= fromString) break;
			lastDataFolder = actDataFolder;
		}
		if (!lastDataFolder.empty() && lastDataFolder != actDataFolder && fromString < actDataFolder) {
			dataFolderList_.push_back(lastDataFolder);
		}
		if (!actDataFolder.empty() && toString >= actDataFolder) {
			dataFolderList_.push_back(actDataFolder);
		}

		// find last element in range
		while (!dataFolderList.empty()) {
			actDataFolder = dataFolderList.front();
			dataFolderList.pop_front();

			if (actDataFolder > toString) break;
			dataFolderList_.push_back(actDataFolder);
		}

		if (verbose_) {
			std::list<std::string>::iterator it;

			Log(Debug, "FileInEntry - Get DataFolderList")
			    .parameter("FromString", fromString)
			    .parameter("ToString", toString);
			for (it = dataFolderList_.begin(); it != dataFolderList_.end(); it++) {
				Log(Debug, "  ")
					.parameter("DataFolder", *it);
			}
		}

		return true;
	}

	bool
	FileReadEntry::getDataFileList(void)
	{
		dataFileList_.clear();
		if (dataFolderList_.empty()) return true;

		dataFolder_ = valueFolder_ / dataFolderList_.front();
		dataFolderList_.pop_front();

		boost::filesystem::directory_iterator it(dataFolder_);
		boost::filesystem::directory_iterator itend;

		// read data files
		std::list<std::string> dataFileList;
		for(; it != itend; it++) {
			if (boost::filesystem::is_directory(*it)) {
				continue;
			}

			dataFileList.push_back((*it).path().leaf().string());
		}
		dataFileList.sort();

		// find range
		std::string fromString = from_.toISOString();
		std::string toString = to_.toISOString();
		std::string lastDataFile = "";
		std::string actDataFile = "";

#if 0
		if (verbose_) {
			std::list<std::string>::iterator it;

			Log(Debug, "FileInEntry - Get DataFileList")
		        .parameter("FromString", fromString)
		        .parameter("ToString", toString);
			for (it = dataFileList.begin(); it != dataFileList.end(); it++) {
				Log(Debug, "  ")
					.parameter("DataFile", *it);
			}
		}
#endif

		// find first element in range
		while (!dataFileList.empty()) {
			actDataFile = dataFileList.front();
			dataFileList.pop_front();

			if (actDataFile >= fromString) break;
			lastDataFile = actDataFile;
		}
		if (!lastDataFile.empty() && lastDataFile != actDataFile && fromString < actDataFile) {
			dataFileList_.push_back(lastDataFile);
		}
		if (!actDataFile.empty() && toString >= actDataFile) {
			dataFileList_.push_back(actDataFile);
		}

		// find last element in range
		while (!dataFileList.empty()) {
			actDataFile = dataFileList.front();
			dataFileList.pop_front();

			if (actDataFile > toString) break;
			dataFileList_.push_back(actDataFile);
		}

		if (verbose_) {
			std::list<std::string>::iterator it;

			Log(Debug, "FileInEntry - Get DataFileList")
		        .parameter("FromString", fromString)
		        .parameter("ToString", toString);
			for (it = dataFileList_.begin(); it != dataFileList_.end(); it++) {
				Log(Debug, "  ")
					.parameter("DataFile", *it);
			}
		}

		return true;
	}

	bool
	FileReadEntry::skipEntry(uint32_t recordSize)
	{
		ifs_.seekg(recordSize, ifs_.cur /*std::ios_base::cur*/);
		return true;
	}

}
