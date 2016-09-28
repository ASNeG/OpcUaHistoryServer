/*
   Copyright 2015-2016 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/FileHistoryStore.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileHistoryStore::FileHistoryStore(void)
	: fileHistoryConfig_()
	, fileReadManager_()
	, fileWriteManager_()
	{
	}

	FileHistoryStore::~FileHistoryStore(void)
	{
	}

    bool
    FileHistoryStore::startup(const std::string& fileName, ConfigXmlManager& configXmlManager)
    {
    	if (!fileHistoryConfig_.decode(fileName, configXmlManager)) {
    		return false;
    	}

    	if (!fileHistoryConfig_.activate()) {
    		return true;
    	}

    	// startup reader and writer
    	return startupFileStore();
    }

    bool
    FileHistoryStore::shutdown(void)
    {
    	return true;
    }

    bool
    FileHistoryStore::activate(void)
    {
    	return fileHistoryConfig_.activate();
    }

    bool
    FileHistoryStore::startupFileStore(void)
    {
    	// init file read manager
    	fileReadManager_.baseFolder(fileHistoryConfig_.baseFolder());
    	fileReadManager_.verbose(fileHistoryConfig_.historyStoreFileReadConfig().verboseLogging_);

    	fileReadManager_.maxConcurrentValues(fileHistoryConfig_.historyStoreFileReadConfig().maxConcurrentValues_);
    	fileReadManager_.ageCounter(fileHistoryConfig_.historyStoreFileReadConfig().ageCounter_);

    	fileReadManager_.maxContinousPoint(fileHistoryConfig_.historyStoreFileReadConfig().maxContinousPoint_);
    	fileReadManager_.continousPointIdleTimeout(fileHistoryConfig_.historyStoreFileReadConfig().continousPointIdleTimeout_);
    	fileReadManager_.maxDeleteTimeoutEntries(fileHistoryConfig_.historyStoreFileReadConfig().maxDeleteTimeoutEntries_);


    	// init file write manager
    	fileWriteManager_.baseFolder(fileHistoryConfig_.baseFolder());
    	fileWriteManager_.verbose(fileHistoryConfig_.historyStoreFileWriteConfig().verboseLogging_);

    	fileWriteManager_.maxDataFolderInValueFolder(fileHistoryConfig_.historyStoreFileWriteConfig().maxDataFolderInValueFolder_);
    	fileWriteManager_.maxDataFilesInDataFolder(fileHistoryConfig_.historyStoreFileWriteConfig().maxDataFilesInDataFolder_);
    	fileWriteManager_.maxEntriesInDataFile(fileHistoryConfig_.historyStoreFileWriteConfig().maxEntriesInDataFile_);
    	fileWriteManager_.maxConcurrentValues(fileHistoryConfig_.historyStoreFileWriteConfig().maxConcurrentValues_);
    	fileWriteManager_.ageCounter(fileHistoryConfig_.historyStoreFileWriteConfig().ageCounter_);

    	return true;
    }

}