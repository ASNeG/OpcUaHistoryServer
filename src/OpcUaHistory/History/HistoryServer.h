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

#ifndef __OpcUaHistory_HistoryServer_h__
#define __OpcUaHistory_HistoryServer_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackCore/Application/ApplicationHReadContext.h"
#include "OpcUaHistory/History/HistoryStoreIf.h"
#include "OpcUaHistory/History/HistoryServerConfig.h"
#include "OpcUaStackServer/Application/ApplicationIf.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaHistory
{

	class HistoryServer
	{
	  public:
		typedef boost::shared_ptr<HistoryServer> SPtr;
		typedef std::map<std::string, HistoryServer::SPtr> Map;
		typedef std::set<HistoryServer::SPtr> Set;
		typedef std::map<uint32_t, uint32_t> NamespaceMap;

		HistoryServer(void);
		~HistoryServer(void);

		void historyStoreIf(HistoryStoreIf* historyStoreIf);
		void applicationServiceIf(ApplicationServiceIf* applicationServiceIf);

	    bool startup(std::string& fileName, ConfigXmlManager& configXmlManager);
	    bool shutdown(void);

	  private:
	    void hReadValue(ApplicationHReadContext* applicationHReadContext);
	    bool getNamespaceInfo(void);
	    bool registerCallbacks(void);

	    HistoryServerConfig historyServerConfig_;
	    HistoryStoreIf* historyStoreIf_;
	    ApplicationServiceIf* applicationServiceIf_;
	    NamespaceMap namespaceMap_;
	    Callback hReadCallback_;
	};

}

#endif
