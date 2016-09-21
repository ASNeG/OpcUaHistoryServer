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
#include "OpcUaHistory/History/HistoryServer.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryServer::HistoryServer(void)
	: historyServerConfig_()
	{
	}

	HistoryServer::~HistoryServer(void)
	{
	}

    bool
    HistoryServer::startup(std::string& fileName, ConfigXmlManager& configXmlManager)
    {
    	if (!historyServerConfig_.decode(fileName, configXmlManager)) {
    		return false;
    	}

    	return true;
    }

    bool
    HistoryServer::shutdown(void)
    {
    	// FIXME: todo
    	return true;
    }

}
