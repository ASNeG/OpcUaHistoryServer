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

#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/ObjectPool.h"
#include "OpcUaStackServer/ServiceSetApplication/ApplicationService.h"
#include "OpcUaStackServer/ServiceSetApplication/NodeReferenceApplication.h"
#include "OpcUaHistory/Library/Library.h"
#include <iostream>

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	Library::Library(void)
	: ApplicationIf()
	, configXmlManager_()
	, historyStore_()
	, historyClientManager_()
	, historyServerManager_()
	{
		Log(Debug, "Library::Library");
	}

	Library::~Library(void)
	{
		Log(Debug, "Library::~Library");
	}

	bool
	Library::startup(void)
	{
		bool success;
		std::vector<Config>::iterator it1;
		std::vector<std::string>::iterator it2;
		Log(Debug, "Library::startup");

		//
		// create own thread
		//
		ioThread_ = constructSPtr<IOThread>();
		if (!ioThread_->startup()) return false;


		//
        // read history model configuration file
		//
		Config::SPtr config;
        if (!configXmlManager_.registerConfiguration(applicationInfo()->configFileName(), config)) {
        	return false;
        }
        Log(Info, "read configuration file")
            .parameter("ConfigFileName", applicationInfo()->configFileName());


        //
        // read OpcUaClient configuration file
        //
        std::vector<Config> configVecClient;
        std::vector<std::string> configClients;
        config->getChilds("HistoryModel.OpcUaClients", configVecClient);

        for (it1 = configVecClient.begin(); it1 != configVecClient.end(); it1++) {
        	std::string configFileName;
        	success = it1->getConfigParameter("OpcUaClient", configFileName);
            if (!success) {
             	Log(Error, "history manager configuration entry missing")
             		.parameter("ConfigFile", applicationInfo()->configFileName())
             		.parameter("Parameter", "HistoryModel.OpcUaClients.OpcUaClient");
             	return false;
             }
            configClients.push_back(configFileName);
        }


        //
        // read OpcUaServer configuration file
        //
        std::vector<Config> configVecServer;
        std::vector<std::string> configServers;
        config->getChilds("HistoryModel.OpcUaServers", configVecServer);

        for (it1 = configVecServer.begin(); it1 != configVecServer.end(); it1++) {
        	std::string configFileName;
        	success = it1->getConfigParameter("OpcUaServer", configFileName);
            if (!success) {
             	Log(Error, "history manager configuration entry missing")
             		.parameter("ConfigFile", applicationInfo()->configFileName())
             		.parameter("Parameter", "HistoryModel.OpcUaServer.OpcUaServer");
             	return false;
            }
            configServers.push_back(configFileName);
        }


        // read history manager configuration
        std::string configHistory;
        success = config->getConfigParameter("HistoryModel.HistoryStore", configHistory);
        if (!success) {
        	Log(Error, "history store configuration entry missing")
        		.parameter("ConfigFile", applicationInfo()->configFileName())
        		.parameter("Parameter", "HistoryModel.HistoryStore");
        	return false;
        }

        ioThread_ = constructSPtr<IOThread>();
        if (!ioThread_->startup()) return false;

        // start history client and history server
        if (!historyStore_.startup(configHistory, configXmlManager_)) return false;
        if (!historyClientManager_.startup(configClients, configXmlManager_, ioThread_)) return false;
        if (!historyServerManager_.startup(configServers, configXmlManager_)) return false;

		return true;
	}

	bool
	Library::shutdown(void)
	{
		Log(Debug, "Library::shutdown");

        if (!ioThread_->shutdown()) return false;
        ioThread_.reset();

        // shutdown history client and history server
        if (!historyServerManager_.shutdown()) return false;
        if (!historyClientManager_.shutdown()) return false;
        if (!historyStore_.shutdown()) return false;

		return true;
	}

}

extern "C" DLLEXPORT void  init(ApplicationIf** applicationIf) {
    *applicationIf = new OpcUaHistory::Library();
}

