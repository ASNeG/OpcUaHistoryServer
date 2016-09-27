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
#include "OpcUaHistory/History/HistoryClient.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryClient::HistoryClient(void)
	: historyClientConfig_()

	, serviceSetManager_()
	, sessionService_()
	, subscriptionService_()
	, monitoredItemService_()
	{
	}

	HistoryClient::~HistoryClient(void)
	{
	}

    bool
    HistoryClient::startup(
    	const std::string& fileName,
    	ConfigXmlManager& configXmlManager,
    	IOThread::SPtr ioThread
    )
    {
    	ioThread_ = ioThread;

    	if (!historyClientConfig_.decode(fileName, configXmlManager)) {
    		return false;
    	}

		// init service sets
		serviceSetManager_.registerIOThread("GlobalIOThread", ioThread);

		// create session service
		SessionServiceConfig sessionServiceConfig;
		sessionServiceConfig.ioThreadName("GlobalIOThread");
		sessionServiceConfig.sessionServiceIf_ = this;
		sessionServiceConfig.secureChannelClient_->endpointUrl(historyClientConfig_.serverUri());
		sessionServiceConfig.session_->sessionName("ASNeGHistoryServer");
		sessionServiceConfig.session_->reconnectTimeout(historyClientConfig_.reconnectTimeout());
		serviceSetManager_.sessionService(sessionServiceConfig);
		sessionService_ = serviceSetManager_.sessionService(sessionServiceConfig);

		// create subscriptions service
		SubscriptionServiceConfig subscriptionServiceConfig;
		subscriptionServiceConfig.ioThreadName("GlobalIOThread");
		subscriptionServiceConfig.subscriptionServiceIf_ = this;
		subscriptionService_ = serviceSetManager_.subscriptionService(sessionService_, subscriptionServiceConfig);

		// create monitored item service
		MonitoredItemServiceConfig monitoredItemServiceConfig;
		monitoredItemServiceConfig.ioThreadName("GlobalIOThread");
		monitoredItemServiceConfig.monitoredItemServiceIf_ = this;
		monitoredItemService_ = serviceSetManager_.monitoredItemService(sessionService_, monitoredItemServiceConfig);

		// connect to server
		sessionService_->asyncConnect();

#if 0
		slotTimerElement_ = constructSPtr<SlotTimerElement>();
		slotTimerElement_->callback().reset(boost::bind(&OpcUaClient::timerLoop, this));
		slotTimerElement_->expireTime(boost::posix_time::microsec_clock::local_time(), 900);
		ioThread_->slotTimer()->start(slotTimerElement_);

#endif

    	return true;
    }

    bool
    HistoryClient::shutdown(void)
    {
    	return true;
    }

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// SessionIf
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void
	HistoryClient::sessionStateUpdate(SessionBase& session, SessionState sessionState)
	{
		switch (sessionState)
		{
			case SS_Connect:
				Log(Debug, "session state changed to connect");
				//readNamespaceArray();
				break;
			case SS_Disconnect:
				Log(Debug, "session state changed to disconnect/close");
#if 0
				state_ = S_Close;
				if (!polling_) {
					deleteSubscription();
				}
#endif
				break;
			case SS_Reactivate:
				Log(Debug, "session state changed to reactivate");
				break;
		}
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// SubscriptionServiceIf
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
    void
    HistoryClient::subscriptionServiceCreateSubscriptionResponse(ServiceTransactionCreateSubscription::SPtr serviceTransactionCreateSubscription)
    {
    }

    void
    HistoryClient::subscriptionServiceModifySubscriptionResponse(ServiceTransactionModifySubscription::SPtr serviceTransactionModifySubscription)
    {
    }

    void
    HistoryClient::subscriptionServiceTransferSubscriptionsResponse(ServiceTransactionTransferSubscriptions::SPtr serviceTransactionTransferSubscriptions)
    {
    }

    void
    HistoryClient::subscriptionServiceDeleteSubscriptionsResponse(ServiceTransactionDeleteSubscriptions::SPtr serviceTransactionDeleteSubscriptions)
    {
    }

    void
	HistoryClient::dataChangeNotification(const MonitoredItemNotification::SPtr& monitoredItem)
    {
    }

    void
	HistoryClient::subscriptionStateUpdate(SubscriptionState subscriptionState, uint32_t subscriptionId)
    {
    }


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// MonitoredItemServiceIf
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
    void
    HistoryClient::monitoredItemServiceCreateMonitoredItemsResponse(ServiceTransactionCreateMonitoredItems::SPtr serviceTransactionCreateMonitoredItems)
    {
    }

    void
    HistoryClient::monitoredItemServiceDeleteMonitoredItemsResponse(ServiceTransactionDeleteMonitoredItems::SPtr serviceTransactionDeleteMonitoredItems)
    {
    }

    void
    HistoryClient::monitoredItemServiceModifyMonitoredItemsResponse(ServiceTransactionModifyMonitoredItems::SPtr serviceTransactionModifyMonitoredItems)
    {
    }

    void
    HistoryClient::monitoredItemServiceSetMonitoringModeResponse(ServiceTransactionSetMonitoringMode::SPtr serviceTransactionSetMonitoringMode)
    {
    }

    void
    HistoryClient::monitoredItemServiceSetTriggeringResponse(ServiceTransactionSetTriggering::SPtr serviceTransactionSetTriggering)
    {
    }

}
