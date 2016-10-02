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

#ifndef __OpcUaHistory_HistoryManager_h__
#define __OpcUaHistory_HistoryManager_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaHistory/History/FileHistoryStore.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryStore
	{
	  public:
		HistoryStore(void);
		~HistoryStore(void);

	    bool startup(std::string& configFile, ConfigXmlManager& configXmlManager);
	    bool shutdown(void);
	    HistoryStoreIf* historyStoreIf(void);

	  private:
	    FileHistoryStore fileHistoryStore_;
	};

}

#endif
