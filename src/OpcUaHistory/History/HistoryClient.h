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

#ifndef __OpcUaHistory_HistoryClient_h__
#define __OpcUaHistory_HistoryClient_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackClient/ServiceSet/ServiceSetManager.h"
#include "OpcUaHistory/History/HistoryClientConfig.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackClient;

namespace OpcUaHistory
{

	class HistoryClient
	: public SessionServiceIf
	, public AttributeServiceIf
	, public SubscriptionServiceIf
	, public MonitoredItemServiceIf
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

        bool startup(const std::string& fileName, ConfigXmlManager& configXmlManager, IOThread::SPtr ioThread);
        bool shutdown(void);

		//- SessionServiceIf --------------------------------------------------
		virtual void sessionStateUpdate(SessionBase& session, SessionState sessionState);
		//- SessionServiceIf --------------------------------------------------

		//- AttributeServiceIf ------------------------------------------------
		virtual void attributeServiceReadResponse(ServiceTransactionRead::SPtr serviceTransactionRead);
		virtual void attributeServiceWriteResponse(ServiceTransactionWrite::SPtr serviceTransactionWrite);
		virtual void attributeServiceHistoryReadResponse(ServiceTransactionHistoryRead::SPtr serviceTransactionHistoryRead);
		virtual void attributeServiceHistoryUpdateResponse(ServiceTransactionHistoryUpdate::SPtr serviceTransactionHistoryUpdate);
		//- AttributeServuceIf ------------------------------------------------

		//- SubscriptionServiceIf ---------------------------------------------
	    virtual void subscriptionServiceCreateSubscriptionResponse(ServiceTransactionCreateSubscription::SPtr serviceTransactionCreateSubscription);
	    virtual void subscriptionServiceModifySubscriptionResponse(ServiceTransactionModifySubscription::SPtr serviceTransactionModifySubscription);
	    virtual void subscriptionServiceTransferSubscriptionsResponse(ServiceTransactionTransferSubscriptions::SPtr serviceTransactionTransferSubscriptions);
	    virtual void subscriptionServiceDeleteSubscriptionsResponse(ServiceTransactionDeleteSubscriptions::SPtr serviceTransactionDeleteSubscriptions);

		virtual void dataChangeNotification(const MonitoredItemNotification::SPtr& monitoredItem);
		virtual void subscriptionStateUpdate(SubscriptionState subscriptionState, uint32_t subscriptionId);
		//- SubscriptionServiceIf ---------------------------------------------

		//- MonitoredItemServiceIf --------------------------------------------
	    virtual void monitoredItemServiceCreateMonitoredItemsResponse(ServiceTransactionCreateMonitoredItems::SPtr serviceTransactionCreateMonitoredItems);
	    virtual void monitoredItemServiceDeleteMonitoredItemsResponse(ServiceTransactionDeleteMonitoredItems::SPtr serviceTransactionDeleteMonitoredItems);
	    virtual void monitoredItemServiceModifyMonitoredItemsResponse(ServiceTransactionModifyMonitoredItems::SPtr serviceTransactionModifyMonitoredItems);
	    virtual void monitoredItemServiceSetMonitoringModeResponse(ServiceTransactionSetMonitoringMode::SPtr serviceTransactionSetMonitoringMode);
	    virtual void monitoredItemServiceSetTriggeringResponse(ServiceTransactionSetTriggering::SPtr serviceTransactionSetTriggering);
		//- MonitoredItemServiceIf --------------------------------------------

	  private:
	    void readNamespaceArray(void);
	    void handleNamespaceArray(ServiceTransactionRead::SPtr serviceTransactionRead);

        HistoryClientConfig historyClientConfig_;

        IOThread::SPtr ioThread_;
        ServiceSetManager serviceSetManager_;
        SessionService::SPtr sessionService_;
        AttributeService::SPtr attributeService_;
		SubscriptionService::SPtr subscriptionService_;
		MonitoredItemService::SPtr monitoredItemService_;

		State state_;
		typedef std::map<uint32_t, uint32_t> NamespaceMap;
		NamespaceMap namespaceMap_;
	};

}

#endif
