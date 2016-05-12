/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaHistory/HistoryAccessFile/FileOutEntry.h"

namespace OpcUaHistory
{

	FileOutEntry::FileOutEntry(void)
	: DoublyLinkedList()
	, valueName_("")
	, basePath_()
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
	}

	void
	FileOutEntry::basePath(const boost::filesystem::path& basePath)
	{
		basePath_ = basePath;
	}

	bool
	FileOutEntry::write(OpcUaDataValue& dataValue)
	{
		// FIXME:
		return true;
	}

}
