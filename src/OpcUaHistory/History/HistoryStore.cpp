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
#include "OpcUaHistory/History/HistoryStore.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryStore::HistoryStore(void)
	: fileHistoryStore_()
	{
	}

	HistoryStore::~HistoryStore(void)
	{
	}

    bool
    HistoryStore::startup(std::string& configFile, ConfigXmlManager& configXmlManager)
    {
    	// startup file history store interface
    	if (!fileHistoryStore_.startup(configFile, configXmlManager)) {
    	    return false;
    	}

    	// get interface
    	if (fileHistoryStore_.activate()) {
    		// FIXME: todo get interface ....
    	}

    	return true;
    }

    bool
    HistoryStore::shutdown(void)
    {
    	// FIXME: todo
    	return true;
    }

}
