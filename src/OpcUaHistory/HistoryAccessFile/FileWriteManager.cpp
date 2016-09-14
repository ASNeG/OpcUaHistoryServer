/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/FileWriteManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileWriteManager::FileWriteManager(void)
	: maxDataFolderInValueFolder_(1000)
	, maxDataFilesInDataFolder_(1000)
	, maxEntriesInDataFile_(300)
	, baseFolder_(".")
	, maxConcurrentValues_(0)
	, fileWriteEntryMap_()
	, fileWriteEntryList_()
	{
	}

	FileWriteManager::~FileWriteManager(void)
	{
	}

	void
	FileWriteManager::maxDataFolderInValueFolder(uint16_t maxDataFolderInValueFolder)
	{
		maxDataFolderInValueFolder_ = maxDataFolderInValueFolder;
	}

	void
	FileWriteManager::maxDataFilesInDataFolder(uint16_t maxDataFilesInDataFolder)
	{
		maxDataFilesInDataFolder_ = maxDataFilesInDataFolder;
	}

	void
	FileWriteManager::maxEntriesInDataFile(uint16_t maxEntriesInDataFile)
	{
		maxEntriesInDataFile_ = maxEntriesInDataFile;
	}

	void
	FileWriteManager::baseFolder(const boost::filesystem::path& baseFolder)
	{
		baseFolder_ = baseFolder;
	}

	void
	FileWriteManager::maxConcurrentValues(uint32_t maxConcurrentValues)
	{
		maxConcurrentValues_ = maxConcurrentValues;
	}

	bool
	FileWriteManager::write(const std::string& valueName, OpcUaDataValue& dataValue)
	{
		// check if write access entry exists
		FileWriteEntry::Map::iterator it;
		it = fileWriteEntryMap_.find(valueName);

		// create a new entry if it does not exist
		if (it == fileWriteEntryMap_.end()) {
			if (!createFileWriteEntry(valueName)) {
				return false;
			}
			return write(valueName, dataValue);
		}

		// write data to historical value store
		FileWriteEntry::SPtr fileWriteEntry = it->second;
		return write(fileWriteEntry, dataValue);
	}

	bool
	FileWriteManager::write(FileWriteEntry::SPtr& fileWriteEntry, OpcUaDataValue& dataValue)
	{
		return fileWriteEntry->write(dataValue);
	}

	bool
	FileWriteManager::createFileWriteEntry(const std::string& valueName)
	{
		FileWriteEntry::SPtr fileWriteEntry = constructSPtr<FileWriteEntry>();
		fileWriteEntry->maxDataFolderInValueFolder(maxDataFolderInValueFolder_);
		fileWriteEntry->maxDataFilesInDataFolder(maxDataFilesInDataFolder_);
		fileWriteEntry->maxEntriesInDataFile(maxEntriesInDataFile_);
		fileWriteEntry->valueName(valueName);
		fileWriteEntry->baseFolder(baseFolder_);
		fileWriteEntryMap_.insert(std::make_pair(valueName, fileWriteEntry));
		return true;
	}

}
