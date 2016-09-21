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
#include "OpcUaHistory/History/FileHistory.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileHistory::FileHistory(void)
	: fileHistoryConfig_()
	{
	}

	FileHistory::~FileHistory(void)
	{
	}

    bool
    FileHistory::startup(const std::string& fileName, ConfigXmlManager& configXmlManager)
    {
    	if (!fileHistoryConfig_.decode(fileName, configXmlManager)) {
    		return false;
    	}

    	return true;
    }

    bool
    FileHistory::shutdown(void)
    {
    	// FIXME: todo
    	return true;
    }

}