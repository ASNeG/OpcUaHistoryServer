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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/ConfigXmlManager.h"
#include "OpcUaHistory/OpcUaClient/HistoryClientManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryClientManager::HistoryClientManager(void)
	: clientConfigIf_(nullptr)
	, historyClientSet_()
	, ioThread_()
	, historyStoreIf_(nullptr)
	, mainConfig_(nullptr)
	{
	}

	HistoryClientManager::~HistoryClientManager(void)
	{
	}

	void
	HistoryClientManager::clientConfigIf(ApplicationClientConfigIf* clientConfigIf)
	{
		clientConfigIf_ = clientConfigIf;
	}

	void
	HistoryClientManager::ioThread(IOThread::SPtr& ioThread)
	{
		ioThread_ = ioThread;
	}

	void
	HistoryClientManager::historyStoreIf(HistoryStoreIf* historyStoreIf)
	{
		historyStoreIf_ = historyStoreIf;
	}

	void
	HistoryClientManager::mainConfig(Config* config)
	{
		mainConfig_ = config;
	}

    bool
    HistoryClientManager::startup(
    	std::vector<std::string>& configFiles,
    	ConfigXmlManager& configXmlManager
    )
    {
    	std::vector<std::string>::iterator it;
    	for (it = configFiles.begin(); it != configFiles.end(); it++) {
    		HistoryClient::SPtr historyClient = constructSPtr<HistoryClient>();
    		historyClientSet_.insert(historyClient);

    		historyClient->clientConfigIf(clientConfigIf_);
    		historyClient->ioThread(ioThread_);
    		historyClient->historyStoreIf(historyStoreIf_);
    		historyClient->mainConfig(mainConfig_);
    		if (!historyClient->startup(*it, configXmlManager)) {
    			return false;
    		}
    	}

    	return true;
    }

    bool
    HistoryClientManager::shutdown(void)
    {
    	HistoryClient::Set::iterator it;
    	for (it = historyClientSet_.begin(); it !=  historyClientSet_.end(); it++) {
    		HistoryClient::SPtr historyClient = *it;
    		historyClient->shutdown();
    	}
    	return true;
    }

}
