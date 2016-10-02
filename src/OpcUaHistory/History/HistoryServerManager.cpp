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
#include "OpcUaHistory/History/HistoryServerManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryServerManager::HistoryServerManager(void)
	: historyServerSet_()
	, historyStoreIf_(nullptr)
	, applicationServiceIf_(nullptr)
	{
	}

	HistoryServerManager::~HistoryServerManager(void)
	{
	}

	void
	HistoryServerManager::historyStoreIf(HistoryStoreIf* historyStoreIf)
	{
		historyStoreIf_ = historyStoreIf;
	}

	void
	HistoryServerManager::applicationServiceIf(ApplicationServiceIf* applicationServiceIf)
	{
		applicationServiceIf_ = applicationServiceIf;
	}

    bool
    HistoryServerManager::startup(std::vector<std::string>& configFiles, ConfigXmlManager& configXmlManager)
    {
    	std::vector<std::string>::iterator it;
    	for (it = configFiles.begin(); it != configFiles.end(); it++) {
    		HistoryServer::SPtr historyServer = constructSPtr<HistoryServer>();
    		historyServerSet_.insert(historyServer);

    		historyServer->historyStoreIf(historyStoreIf_);
    		historyServer->applicationServiceIf(applicationServiceIf_);
    		if (!historyServer->startup(*it, configXmlManager)) {
    			return false;
    		}
    	}
    	return true;
    }

    bool
    HistoryServerManager::shutdown(void)
    {
    	HistoryServer::Set::iterator it;
    	for (it = historyServerSet_.begin(); it != historyServerSet_.end(); it++) {
    		HistoryServer::SPtr historyServer = *it;
    		historyServer->shutdown();
    	}
    	return true;
    }

}
