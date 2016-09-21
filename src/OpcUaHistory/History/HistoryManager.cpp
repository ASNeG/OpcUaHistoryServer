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
#include "OpcUaHistory/History/HistoryManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryManager::HistoryManager(void)
	: history_()
	{
	}

	HistoryManager::~HistoryManager(void)
	{
	}

    bool
    HistoryManager::startup(std::string& configFile, ConfigXmlManager& configXmlManager)
    {
    	if (!history_.startup(configFile, configXmlManager)) {
    	    return false;
    	}
    	return true;
    }

    bool
    HistoryManager::shutdown(void)
    {
    	// FIXME: todo
    	return true;
    }

}
