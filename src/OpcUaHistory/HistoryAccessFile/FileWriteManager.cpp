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
		return true;
	}
}
