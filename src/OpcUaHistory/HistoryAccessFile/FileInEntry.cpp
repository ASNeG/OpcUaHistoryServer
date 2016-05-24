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
	, dataFolderList_()
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
			if (!getDataFolder(from, to)) {
				return false;
			}
		}

		return true;
	}

	bool
	FileInEntry::getDataFolder(OpcUaDateTime& from, OpcUaDateTime& to)
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

}
