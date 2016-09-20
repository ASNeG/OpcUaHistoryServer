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
#include "OpcUaStackCore/Base/ConfigXmlManager.h"
#include "OpcUaHistory/History/HistoryClientManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryClientManager::HistoryClientManager(void)
	: historyClientSet_()
	{
	}

	HistoryClientManager::~HistoryClientManager(void)
	{
	}

    bool
    HistoryClientManager::startup(std::vector<std::string>& configFiles, ConfigXmlManager& configXmlManager)
    {
    	std::vector<std::string>::iterator it;
    	for (it = configFiles.begin(); it != configFiles.end(); it++) {
    		HistoryClient::SPtr historyClient = constructSPtr<HistoryClient>();
    		historyClientSet_.insert(historyClient);

    		if (!historyClient->startup(*it, configXmlManager)) {
    			return false;
    		}
    	}

    	return true;
    }

    bool
    HistoryClientManager::shutdown(void)
    {
    	// FIXME: todo
    	return true;
    }

}
