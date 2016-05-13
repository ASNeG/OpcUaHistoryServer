/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/FileOutEntry.h"

namespace OpcUaHistory
{

	FileOutEntry::FileOutEntry(void)
	: DoublyLinkedList()
	, valueName_("")
	, baseFolder_()
	, valueFolder_()
	, maxDataFolderInValueFolder_(1000)
	, maxDataFilesInDataFolder_(1000)
	, maxEntriesInDataFile_(300)
	, countDataFolderInValueFolder_(0)
	, countDataFilesInDataFolder_(0)
	, countEntriesInDataFile_(0)
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

		return true;
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

}
