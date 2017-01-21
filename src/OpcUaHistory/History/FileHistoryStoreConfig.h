/*
   Copyright 2015-2017 Kai Huebl (kai@huebl-sgh.de)

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

#ifndef __OpcUaHistory_HistoryConfig_h__
#define __OpcUaHistory_HistoryConfig_h__

#include "OpcUaStackCore/Base/ConfigXmlManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileHistoryStoreReadConfig
	{
	  public:
		FileHistoryStoreReadConfig(void);
		~FileHistoryStoreReadConfig(void);

		bool verboseLogging_;

		uint32_t maxConcurrentValues_;
		uint32_t ageCounter_;

		uint32_t maxContinousPoint_;
		uint32_t continousPointIdleTimeout_;
		uint32_t maxDeleteTimeoutEntries_;

		uint32_t maxNumResultValuesPerNode_;
	};

	class FileHistoryStoreWriteConfig
	{
	  public:
		FileHistoryStoreWriteConfig(void);
		~FileHistoryStoreWriteConfig(void);

		bool verboseLogging_;

		uint32_t maxDataFolderInValueFolder_;
		uint32_t maxDataFilesInDataFolder_;
		uint32_t maxEntriesInDataFile_;
		uint32_t maxConcurrentValues_;
		uint32_t ageCounter_;
	};

	class FileHistoryStoreConfig
	{
	  public:
		FileHistoryStoreConfig(void);
		~FileHistoryStoreConfig(void);

		void configFileName(const std::string& configFileName);
		void elementPrefix(const std::string& elementPrefix);
		bool activate(void);
		std::string& baseFolder(void);
		FileHistoryStoreReadConfig& historyStoreFileReadConfig(void);
		FileHistoryStoreWriteConfig& historyStoreFileWriteConfig(void);

		bool decode(Config& config);

	  private:
		bool decodeFileStoreBase(Config& config);
		bool decodeFileStoreRead(Config& config);
		bool decodeFileStoreWrite(Config& config);

		bool activate_;
		std::string configFileName_;
		std::string elementPrefix_;
		std::string baseFolder_;
		FileHistoryStoreReadConfig readConfig_;
		FileHistoryStoreWriteConfig writeConfig_;

	};

}

#endif
