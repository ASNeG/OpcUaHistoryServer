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

#ifndef __OpcUaHistory_HistoryServer_h__
#define __OpcUaHistory_HistoryServer_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Base/ConfigXmlManager.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackCore/Application/ApplicationHReadContext.h"
#include "OpcUaStackCore/Application/ApplicationHWriteContext.h"
#include "OpcUaStackServer/Application/ApplicationIf.h"
#include "OpcUaHistory/Interface/HistoryStoreIf.h"
#include "OpcUaHistory/Interface/ApplicationServerConfigIf.h"
#include "OpcUaHistory/History/HistoryStoreModelConfig.h"


using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaHistory
{

	class HistoryServerItem
	{
	  public:
		typedef boost::shared_ptr<HistoryServerItem> SPtr;
		typedef std::map<OpcUaNodeId, HistoryServerItem::SPtr> Map;

		HistoryServerItem(void);
		~HistoryServerItem(void);

		void contextRead(Object::SPtr& context);
		Object::SPtr& contextRead(void);
		void contextWrite(Object::SPtr& context);
		Object::SPtr& contextWrite(void);

	  private:
		Object::SPtr contextRead_;
		Object::SPtr contextWrite_;
	};


	class HistoryServer
	{
	  public:
		typedef boost::shared_ptr<HistoryServer> SPtr;
		typedef std::map<std::string, HistoryServer::SPtr> Map;
		typedef std::set<HistoryServer::SPtr> Set;
		typedef std::map<uint32_t, uint32_t> NamespaceMap;

		HistoryServer(void);
		~HistoryServer(void);

		void serverConfigIf(ApplicationServerConfigIf* serverConfigIf);
		void historyStoreIf(HistoryStoreIf* historyStoreIf);
		void applicationServiceIf(ApplicationServiceIf* applicationServiceIf);

	    bool startup(void);
	    bool shutdown(void);

	  private:
	    void hReadValue(ApplicationHReadContext* applicationHReadContext);
	    void hWriteValue(ApplicationHWriteContext* applicationHWriteContext);
	    void hReadValueFirst(
	    	ApplicationHReadContext* applicationHReadContext,
	    	HistoryServerItem::SPtr& historyServerItem
	    );
	    void hReadValueNext(
	    	ApplicationHReadContext* applicationHReadContext,
	    	HistoryServerItem::SPtr& historyServerItem
	    );
	    void hReadValueDelete(
	    	ApplicationHReadContext* applicationHReadContext,
	    	HistoryServerItem::SPtr& historyServerItem
	    );
	    bool getNamespaceInfo(void);
	    bool registerServerCallbacks(void);
	    bool registerServerCallbacks(VariableElement& variableElement);
	    bool registerServerCallbacks(VariableElement& variableElement, OpcUaReferenceConfig::SPtr& ref);


	    ApplicationServerConfigIf* serverConfigIf_;
	    HistoryStoreIf* historyStoreIf_;
	    ApplicationServiceIf* applicationServiceIf_;

	    NamespaceMap namespaceMap_;
	    Callback hReadCallback_;
	    Callback hWriteCallback_;
	    HistoryServerItem::Map historyServerItemMap_;
	};

}

#endif
