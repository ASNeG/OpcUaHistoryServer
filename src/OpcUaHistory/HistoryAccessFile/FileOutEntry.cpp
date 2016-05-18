/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <boost/asio.hpp>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/Utility.h"
#include "OpcUaHistory/HistoryAccessFile/FileOutEntry.h"

namespace OpcUaHistory
{

	FileOutEntry::FileOutEntry(void)
	: DoublyLinkedList()
	, valueName_("")
	, baseFolder_(".")
	, valueFolder_()
	, dataFolder_()
	, dataFile_()
	, maxDataFolderInValueFolder_(1000)
	, maxDataFilesInDataFolder_(1000)
	, maxEntriesInDataFile_(300)
	, countDataFolderInValueFolder_(0)
	, countDataFilesInDataFolder_(0)
	, countEntriesInDataFile_(0)
	, ofs_()
	{
	}

	FileOutEntry::~FileOutEntry(void)
	{
	}

	void
	FileOutEntry::maxDataFolderInValueFolder(uint16_t maxDataFolderInValueFolder)
	{
		maxDataFolderInValueFolder_ = maxDataFolderInValueFolder;
	}

	void
	FileOutEntry::maxDataFilesInDataFolder(uint16_t maxDataFilesInDataFolder)
	{
		maxDataFilesInDataFolder_ = maxDataFilesInDataFolder;
	}

	void
	FileOutEntry::maxEntriesInDataFile(uint16_t maxEntriesInDataFile)
	{
		maxEntriesInDataFile_ = maxEntriesInDataFile;
	}

	void
	FileOutEntry::valueName(const std::string& valueName)
	{
		valueName_ = valueName;
		valueFolder_ = baseFolder_ / boost::filesystem::path(valueName_);
	}

	void
	FileOutEntry::baseFolder(const boost::filesystem::path& baseFolder)
	{
		baseFolder_ = baseFolder;
		valueFolder_ = baseFolder_ / boost::filesystem::path(valueName_);
	}

	bool
	FileOutEntry::write(OpcUaDataValue& dataValue)
	{
		// check if base folder exists
		if (!boost::filesystem::exists(baseFolder_)) {
			Log(Error, "base folder not exist")
				.parameter("BaseFolder", baseFolder_.string());
			return false;
		}

		// check if value folder exists - if not create new value folder
		if (!boost::filesystem::exists(valueFolder_)) {
			if (!createValueFolder()) {
				return false;
			}
		}

		// get or create data folder
		if (countDataFolderInValueFolder_ == 0) {
			getNewestDataFolder();
		}
		if (countDataFolderInValueFolder_ == 0) {
			return createDataFolder(dataValue);
		}

		// get or create data files
		if (countDataFilesInDataFolder_ == 0) {
			getNewestDataFile();
		}
		if (countDataFilesInDataFolder_ == 0) {
			return createDataFile(dataValue);
		}

		// open file
		if (!ofs_.is_open()) {
			return openDataFile(dataValue);
		}

		return writeData(dataValue);
	}

	bool
	FileOutEntry::createValueFolder(void)
	{
		if (!boost::filesystem::create_directory(valueFolder_)) {
			Log(Error, "create value folder error")
			    .parameter("ValueFolder", valueFolder_.string());
			return false;
		}

		return true;
	}

	void
	FileOutEntry::getNewestDataFolder(void)
	{
		countDataFolderInValueFolder_ = 0;
		boost::filesystem::directory_iterator it(valueFolder_);
		boost::filesystem::directory_iterator itend;

		std::string newestDataFolder = "";
		for(; it != itend; it++) {
			if (!boost::filesystem::is_directory(*it)) {
				continue;
			}

			countDataFolderInValueFolder_++;
			std::string dataFolder = (*it).path().leaf().string();

			if (newestDataFolder.empty()) {
				newestDataFolder = dataFolder;
				continue;
			}

			if (dataFolder > newestDataFolder) {
				newestDataFolder = dataFolder;
				continue;
			}
		}

		if (countDataFolderInValueFolder_ > 0) {
			dataFolder_ = valueFolder_ / newestDataFolder;
		}
	}

	bool
	FileOutEntry::createDataFolder(OpcUaDataValue& dataValue)
	{
		if (countDataFolderInValueFolder_ >= maxDataFolderInValueFolder_) {
			Log(Error, "to many folders available")
			    .parameter("ValueFolder", dataFolder_.string());
			return false;
		}
		countDataFolderInValueFolder_++;
		countDataFilesInDataFolder_ = 0;

		// create new data folder
		dataFolder_ = valueFolder_ / dataValue.sourceTimestamp().toISOString();
		if (!boost::filesystem::create_directory(dataFolder_)) {
			Log(Error, "create data folder error")
			    .parameter("DataFolder", dataFolder_.string());
			return false;
		}

		return createDataFile(dataValue);
	}

	void
	FileOutEntry::getNewestDataFile(void)
	{
		countDataFilesInDataFolder_ = 0;
		boost::filesystem::directory_iterator it(dataFolder_);
		boost::filesystem::directory_iterator itend;

		std::string newestDataFile;
		for(; it != itend; it++) {
			if (!boost::filesystem::is_regular_file(*it)) {
				continue;
			}

			countDataFilesInDataFolder_++;
			std::string dataFile = (*it).path().leaf().string();

			if (newestDataFile.empty()) {
				newestDataFile = dataFile;
				continue;
			}

			if (dataFile > newestDataFile) {
				newestDataFile = dataFile;
				continue;
			}
		}

		if (countDataFilesInDataFolder_ > 0) {
			dataFile_ = dataFolder_ / newestDataFile;
		}
	}

	bool
	FileOutEntry::createDataFile(OpcUaDataValue& dataValue)
	{
		if (countDataFilesInDataFolder_ >= maxDataFilesInDataFolder_) {
			return createDataFolder(dataValue);
		}
		countDataFilesInDataFolder_++;
		countEntriesInDataFile_ = 0;

		// create new data file
		dataFile_ = dataFolder_ / dataValue.sourceTimestamp().toISOString();
		ofs_.open(dataFile_.string(), std::ios::out | std::ios::app | std::ios::binary);
		if (ofs_.fail()) {
			Log(Error, "file open failed")
				.parameter("FileName", dataFile_.string());
			return false;
		}
		ofs_.close();

		Log(Debug, "file create")
		    .parameter("FileName", dataFile_.string());
		return openDataFile(dataValue);
	}

	bool
	FileOutEntry::openDataFile(OpcUaDataValue& dataValue)
	{
		// get file size
		uint32_t fileSize = boost::filesystem::file_size(dataFile_.string());
		if (fileSize == 0) {
			countEntriesInDataFile_ = 0;

			ofs_.open(dataFile_.string(), std::ios::out | std::ios::app | std::ios::binary);
			if (ofs_.fail()) {
				Log(Error, "file create failed")
					.parameter("FileName", dataFile_.string());
				return false;
			}

			Log(Debug, "file open")
			    .parameter("FileName", dataFile_.string());

			return writeData(dataValue);
		}

		// read number of entries in file
		boost::filesystem::ifstream ifs;
		ifs.open(dataFile_.string(), std::ios::in | std::ios::app | std::ios::binary);
		if (ifs.fail()) {
			Log(Error, "file open failed")
				.parameter("FileName", dataFile_.string());
			return false;
		}
		ifs.seekg(fileSize-2);

		char buf[2];
		boost::asio::streambuf sb;
		std::iostream ios(&sb);

	    ifs.read(buf, 2);
	    ios.write(buf, 2);
		OpcUaNumber::opcUaBinaryDecode(ios, countEntriesInDataFile_);
		ifs.close();

		// open data file
		ofs_.open(dataFile_.string(), std::ios::out | std::ios::app | std::ios::binary);
		if (ofs_.fail()) {
			Log(Error, "file open failed")
				.parameter("FileName", dataFile_.string());
			return false;
		}

		Log(Debug, "file open")
		    .parameter("FileName", dataFile_.string())
		    .parameter("FileSize", fileSize)
		    .parameter("CountEntriesInDataFile",countEntriesInDataFile_);

		return writeData(dataValue);
	}

	bool
	FileOutEntry::writeData(OpcUaDataValue& dataValue)
	{
		if (countEntriesInDataFile_ >= maxEntriesInDataFile_) {
			if (ofs_.is_open()) {
				ofs_.close();
			}
			return createDataFile(dataValue);
		}
		countEntriesInDataFile_++;

		// write data value to stream buffer
		boost::asio::streambuf sb1;
		std::iostream ios1(&sb1);
		dataValue.sourceTimestamp().opcUaBinaryEncode(ios1);
		dataValue.serverTimestamp().opcUaBinaryEncode(ios1);
		OpcUaNumber::opcUaBinaryEncode(ios1, dataValue.statusCode());
		if (dataValue.statusCode() == Success) {
			dataValue.variant()->opcUaBinaryEncode(ios1);
		}
		OpcUaNumber::opcUaBinaryEncode(ios1, countEntriesInDataFile_);

		// write data length to stream buffer
		boost::asio::streambuf sb2;
		std::iostream ios2(&sb2);
		uint32_t writeBytes_ = OpcUaStackCore::count(sb1) + sizeof(uint32_t);
		OpcUaNumber::opcUaBinaryEncode(ios2, writeBytes_);

		// write stream buffers to file
		ofs_.write(boost::asio::buffer_cast<const char*>(sb2.data()), sb2.size());
		ofs_.write(boost::asio::buffer_cast<const char*>(sb1.data()), sb1.size());
		if (ofs_.fail()) {
			Log(Error, "file write failed")
				.parameter("FileName", dataFile_.string());
			return false;
		}
		return true;
	}

}
