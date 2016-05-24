/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <boost/asio.hpp>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/Utility.h"
#include "OpcUaHistory/HistoryAccessFile/FileInEntry.h"

namespace OpcUaHistory
{

	FileInEntry::FileInEntry(void)
	: DoublyLinkedList()
	, valueName_("")
	, baseFolder_()
	, valueFolder_()
	, dataFile_()
	, dataFolderList_()
	, dataFileList_()
	, dataFolder_()
	, ifs_()
	{
	}

	FileInEntry::~FileInEntry(void)
	{
	}

	void
	FileInEntry::valueName(const std::string& valueName)
	{
		valueName_ = valueName;
		valueFolder_ = baseFolder_ / boost::filesystem::path(valueName_);
	}

	void
	FileInEntry::baseFolder(const boost::filesystem::path& baseFolder)
	{
		baseFolder_ = baseFolder;
		valueFolder_ = baseFolder_ / boost::filesystem::path(valueName_);
	}

	bool
	FileInEntry::read(OpcUaDateTime& from, OpcUaDateTime& to)
	{
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
			if (!getDataFolderList(from, to)) {
				return false;
			}
		}
		if (dataFolderList_.empty()) return true;

		// get data files
		if (dataFileList_.empty()) {
			if (!getDataFileList(from, to)) {
				return false;
			}
		}
		if (dataFileList_.empty()) return true;

		// find first element in data file
		if (!readEntryFirst(from, to)) {
			return false;
		}

		return true;
	}

	bool
	FileInEntry::getDataFolderList(OpcUaDateTime& from, OpcUaDateTime& to)
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
		std::string fromString = from.toISOString();
		std::string toString = to.toISOString();
		std::string lastDataFolder = "";
		std::string actDataFolder = "";

		// find first element in range
		while (!dataFolderList.empty()) {
			actDataFolder = dataFolderList.front();
			dataFolderList.pop_front();

			if (actDataFolder >= fromString) break;
			lastDataFolder = actDataFolder;
		}
		if (!lastDataFolder.empty()) dataFolderList_.push_back(lastDataFolder);
		if (!actDataFolder.empty()) dataFolderList_.push_back(actDataFolder);

		// find last element in range
		while (!dataFolderList.empty()) {
			actDataFolder = dataFolderList.front();
			dataFolderList.pop_front();

			if (actDataFolder > toString) break;
			dataFolderList_.push_back(actDataFolder);
		}

		return true;
	}

	bool
	FileInEntry::getDataFileList(OpcUaDateTime& from, OpcUaDateTime& to)
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
			if (!boost::filesystem::is_directory(*it)) {
				continue;
			}

			dataFileList.push_back((*it).path().leaf().string());
		}
		dataFileList.sort();

		// find range
		std::string fromString = from.toISOString();
		std::string toString = to.toISOString();
		std::string lastDataFile = "";
		std::string actDataFile = "";

		// find first element in range
		while (!dataFileList.empty()) {
			actDataFile = dataFileList.front();
			dataFileList.pop_front();

			if (actDataFile >= fromString) break;
			lastDataFile = actDataFile;
		}
		if (!lastDataFile.empty()) dataFileList_.push_back(lastDataFile);
		if (!actDataFile.empty()) dataFileList_.push_back(actDataFile);

		// find last element in range
		while (!dataFileList.empty()) {
			actDataFile = dataFileList.front();
			dataFileList.pop_front();

			if (actDataFile > toString) break;
			dataFileList_.push_back(actDataFile);
		}

		return true;
	}

	bool
	FileInEntry::readEntryFirst(OpcUaDateTime& from, OpcUaDateTime& to)
	{
		if (dataFileList_.empty()) return true;
		dataFile_ = dataFolder_ / dataFileList_.front();
		dataFileList_.pop_front();

		// open data file
		ifs_.open(dataFile_.string(), std::ios::in | std::ios::app | std::ios::binary);
		if (ifs_.fail()) {
			Log(Error, "file open error")
				.parameter("FileName", dataFile_.string());
			return false;
		}

		return true;
	}

	bool
	FileInEntry::readEntryNext(OpcUaDateTime& from, OpcUaDateTime& to)
	{
		return true;
	}

}
