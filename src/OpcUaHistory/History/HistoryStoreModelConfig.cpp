/*
   Copyright 2017 Kai Huebl (kai@huebl-sgh.de)

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

#include <boost/algorithm/string.hpp>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/HistoryStoreModelConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryStoreModelConfig::HistoryStoreModelConfig(void)
	: fileHistoryStoreConfig_()
	, historyStoreModelValuesConfig_()
	{
	}

	HistoryStoreModelConfig::~HistoryStoreModelConfig(void)
	{
	}

	HistoryStoreModelValuesConfig&
	HistoryStoreModelConfig::historyStoreModelValuesConfig(void)
	{
		return historyStoreModelValuesConfig_;
	}

	FileHistoryStoreConfig&
	HistoryStoreModelConfig::fileHistoryStoreConfig(void)
	{
		return fileHistoryStoreConfig_;
	}

	bool
	HistoryStoreModelConfig::decode(const std::string& configFileName, ConfigXmlManager& configXmlManager)
	{
		bool success;

		// read configuration file
        Log(Info, "read configuration file")
            .parameter("ConfigFileName", configFileName);
		Config::SPtr config;
		success = configXmlManager.registerConfiguration(configFileName, config);
		if (!success) {
			Log(Error, "read configuration file error")
			   .parameter("ConfigFile", configFileName)
			   .parameter("ErrorMessage", configXmlManager.errorMessage());
			   return false;
		}

		// get HistoryStoreModel configuration
		boost::optional<Config> child = config->getChild("HistoryStoreModel");
		if (!child) {
			Log(Error, "element missing in config file")
				.parameter("Element", "HistoryStoreModel")
				.parameter("ConfigFileName", configFileName);
			return false;
		}

		// get history store configuration
		fileHistoryStoreConfig_.configFileName(configFileName);
		fileHistoryStoreConfig_.elementPrefix("HistoryStoreModel");
		if (!fileHistoryStoreConfig_.decode(*child)) {
			return false;
		}

		// get values configuration
		historyStoreModelValuesConfig_.configFileName(configFileName);
		historyStoreModelValuesConfig_.elementPrefix("HistoryStoreModel");
		if (!historyStoreModelValuesConfig_.decode(*child)) {
			return false;
		}

		return true;
	}

}
