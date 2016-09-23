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
#include "OpcUaHistory/History/FileHistoryStoreConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class HistoryStoreReadConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FileHistoryStoreReadConfig::FileHistoryStoreReadConfig(void)
	: verboseLogging_(false)
	, maxConcurrentValues_(500)
	, ageCounter_(10)
	, maxContinousPoint_(100)
	, continousPointIdleTimeout_(60000)
	, maxDeleteTimeoutEntries_(20)
	{
	}

	FileHistoryStoreReadConfig::~FileHistoryStoreReadConfig(void)
	{
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class HistoryStoreWriteConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FileHistoryStoreWriteConfig::FileHistoryStoreWriteConfig(void)
	: verboseLogging_(false)
	, maxDataFilderInValueFolder_(1000)
	, maxDataFilesInDataFolder_(1000)
	, maxEntriesInDataFile_(1000)
	, maxConcurrentValues_(500)
	, ageCounter_(10)
	{
	}

	FileHistoryStoreWriteConfig::~FileHistoryStoreWriteConfig(void)
	{
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class FileHistoryStoreConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FileHistoryStoreConfig::FileHistoryStoreConfig(void)
	: baseFolder_("")
	, readConfig_()
	, writeConfig_()
	{
	}

	FileHistoryStoreConfig::~FileHistoryStoreConfig(void)
	{
	}

	std::string&
	FileHistoryStoreConfig::baseFolder(void)
	{
		return baseFolder_;
	}

	FileHistoryStoreReadConfig&
	FileHistoryStoreConfig::historyStoreFileReadConfig(void)
	{
		return readConfig_;
	}

	FileHistoryStoreWriteConfig&
	FileHistoryStoreConfig::historyStoreFileWriteConfig(void)
	{
		return writeConfig_;
	}

	bool
	FileHistoryStoreConfig::decode(const std::string& configFileName, ConfigXmlManager& configXmlManager)
	{
		return true;
	}

}
