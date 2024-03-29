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

#ifndef __OpcUaHistory_HistoryClient_h__
#define __OpcUaHistory_HistoryClient_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackClient/ServiceSet/ServiceSetManager.h"
#include "OpcUaStackClient/ApplicationUtility/ClientConfig.h"
#include "OpcUaStackClient/ApplicationUtility/ApplicationClientConfigIf.h"
#include "OpcUaStackClient/ApplicationUtility/ClientConnection.h"
#include "OpcUaStackClient/ApplicationUtility/DiscoveryClientFindServers.h"
#include "OpcUaHistory/Interface/HistoryStoreIf.h"

using namespace OpcUaStackClient;

namespace OpcUaHistory
{

	class HistoryClient
	: public ClientSubscriptionIf
	{
	  public:
		typedef boost::shared_ptr<HistoryClient> SPtr;
		typedef std::map<std::string, HistoryClient::SPtr> Map;
		typedef std::set<HistoryClient::SPtr> Set;

		typedef enum {
			S_Open,
			S_Close
		} State;

		HistoryClient(void);
		~HistoryClient(void);

		void clientConfigIf(ApplicationClientConfigIf* clientConfigIf);
		void ioThread(IOThread::SPtr& ioThread);
		void historyStoreIf(HistoryStoreIf* historyStoreIf);
		void mainConfig(Config* config);

        bool startup(const std::string& fileName, ConfigXmlManager& configXmlManager);
        bool shutdown(void);

        // -- ClientSubscriptionIf --------------------------------------------
        virtual void dataChangeNotification(ClientMonitoredItem::SPtr& clientMonitoredItem, OpcUaDataValue& dataValue);
        // -- ClientSubscriptionIf --------------------------------------------

	  private:
        bool createSubscriptions(void);
        bool createMonitoredItems(
        	ClientSubscriptionConfig::SPtr& csc,
        	ClientSubscription::SPtr& cs
        );
        bool createMonitoredItems(
        	ClientSubscriptionConfig::SPtr& csc,
        	ClientSubscription::SPtr& cs,
        	ClientMonitoredItemConfig::SPtr& cmic
        );

        uint32_t clientHandle_;
        VariableElement::Vec variableElementVec_;
        ApplicationClientConfigIf* clientConfigIf_;
        ClientConfig clientConfig_;
        ClientConnection clientConnection_;
        IOThread::SPtr ioThread_;
        HistoryStoreIf* historyStoreIf_;

        // discovery parameters
        Config* mainConfig_;
        bool startupDiscovery(void);
        bool shutdownDiscovery(void);
        bool discoveryEnabled_;
        DiscoveryClientFindServers discoveryClient_;
	};

}

#endif
