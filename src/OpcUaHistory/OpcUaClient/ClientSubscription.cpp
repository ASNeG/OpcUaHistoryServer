/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)

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
#include "OpcUaHistory/OpcUaClient/ClientSubscription.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	ClientSubscription::ClientSubscription(void)
	: init_(false)
	, state_(S_Close)
	, ioThread_()
	, id_("")
	, subscriptionId_(0)
	, publishingInterval_(0)
	, livetimeCount_(0)
	, maxKeepAliveCount_(0)
	, maxNotificationsPerPublish_(0)
	, subscriptionService_()
	, monitoredItemService_()
	, serviceSetManager_(nullptr)
	, sessionService_()
	, clientMonitoredItemSet_()
	{
	}

	ClientSubscription::~ClientSubscription(void)
	{
	}

	void
	ClientSubscription::ioThread(IOThread::SPtr& ioThread)
	{
		ioThread_ = ioThread;
	}

	IOThread::SPtr&
	ClientSubscription::ioThread(void)
	{
		return ioThread_;
	}

	std::string
	ClientSubscription::id(void)
	{
		return id_;
	}

	void
	ClientSubscription::id(const std::string& id)
	{
		id_ = id;
	}

	uint32_t
	ClientSubscription::publishingInterval(void)
	{
		return publishingInterval_;
	}

	void
	ClientSubscription::publishingInterval(uint32_t publishingInterval)
	{
		publishingInterval_ =publishingInterval;
	}

	uint32_t
	ClientSubscription::livetimeCount(void)
	{
		return livetimeCount_;
	}

	void
	ClientSubscription::livetimeCount(uint32_t livetimeCount)
	{
		livetimeCount_ = livetimeCount;
	}

	uint32_t
	ClientSubscription::maxKeepAliveCount(void)
	{
		return maxKeepAliveCount_;
	}

	void
	ClientSubscription::maxKeepAliveCount(uint32_t maxKeepAliveCount)
	{
		maxKeepAliveCount_ = maxKeepAliveCount;
	}

	uint32_t
	ClientSubscription::maxNotificationsPerPublish(void)
	{
		return maxNotificationsPerPublish_;
	}

	void
	ClientSubscription::maxNotificationsPerPublish(uint32_t maxNotificationsPerPublish)
	{
		maxNotificationsPerPublish_ = maxNotificationsPerPublish;
	}

	void
	ClientSubscription::serviceSetManager(ServiceSetManager* serviceSetManager)
	{
		serviceSetManager_ = serviceSetManager;
	}

	void
	ClientSubscription::sessionService(SessionService::SPtr& sessionService)
	{
		sessionService_ = sessionService;
	}

	void
	ClientSubscription::state(State state)
	{
		state_ = state;
	}

	ClientSubscription::State
	ClientSubscription::state(void)
	{
		return state_;
	}

	void
	ClientSubscription::init(void)
	{
		// initialization of the subscription component only once
		if (init_) return;
		init_ = true;

		// create subscriptions service
		SubscriptionServiceConfig subscriptionServiceConfig;
		subscriptionServiceConfig.ioThreadName("GlobalIOThread");
		subscriptionServiceConfig.subscriptionServiceIf_ = this;
		subscriptionService_ = serviceSetManager_->subscriptionService(sessionService_, subscriptionServiceConfig);

		// create monitored item service
		MonitoredItemServiceConfig monitoredItemServiceConfig;
		monitoredItemServiceConfig.ioThreadName("GlobalIOThread");
		monitoredItemServiceConfig.monitoredItemServiceIf_ = this;
		monitoredItemService_ = serviceSetManager_->monitoredItemService(sessionService_, monitoredItemServiceConfig);
	}

	void
	ClientSubscription::open(void)
	{
		state_ = S_Opening;
		ServiceTransactionCreateSubscription::SPtr trx = ServiceTransactionCreateSubscription::construct();
		subscriptionService_->asyncSend(trx);
	}

	void
	ClientSubscription::close(void)
	{
		state_ = S_Closing;
		ServiceTransactionDeleteSubscriptions::SPtr trx = constructSPtr<ServiceTransactionDeleteSubscriptions>();
		DeleteSubscriptionsRequest::SPtr req = trx->request();
		req->subscriptionIds()->resize(1);
		req->subscriptionIds()->set(0, subscriptionId_);

		subscriptionService_->asyncSend(trx);
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// functions to handle monitored items
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void
	ClientSubscription::addMonitoredItem(ClientMonitoredItem::SPtr& clientMonitoredItem)
	{
		init();

		// add monitored item to set
		clientMonitoredItemSet_.insert(clientMonitoredItem);
	}

	void
	ClientSubscription::openMonitoredItems(void)
	{
		// open all monitored items
		ClientMonitoredItem::Set::iterator it;
		for (it = clientMonitoredItemSet_.begin(); it != clientMonitoredItemSet_.end(); it++) {
			;
		}

	}

	void
	ClientSubscription::cleanUpMonitoredItems(void)
	{
		// clean up all monitored items
		ClientMonitoredItem::Set::iterator it;
		for (it = clientMonitoredItemSet_.begin(); it != clientMonitoredItemSet_.end(); it++) {
			;
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
    ClientSubscription::subscriptionServiceCreateSubscriptionResponse(ServiceTransactionCreateSubscription::SPtr trx)
    {
		CreateSubscriptionResponse::SPtr res = trx->response();
		if (trx->responseHeader()->serviceResult() != Success) {
			Log(Error, "create subscription response error")
				.parameter("Id", id_)
				.parameter("StatusCode", OpcUaStatusCodeMap::shortString(trx->responseHeader()->serviceResult()));
			state_ = S_Error;
			return;
		}

		subscriptionId_ = res->subscriptionId();
		state_ = S_Open;
		Log(Debug, "open subscription")
			.parameter("Id", id_)
		    .parameter("SubscriptionId", subscriptionId_);

		// open all monitored items
		openMonitoredItems();
    }

    void
    ClientSubscription::subscriptionServiceModifySubscriptionResponse(ServiceTransactionModifySubscription::SPtr serviceTransactionModifySubscription)
    {
    	// nothing to do
    }

    void
    ClientSubscription::subscriptionServiceTransferSubscriptionsResponse(ServiceTransactionTransferSubscriptions::SPtr serviceTransactionTransferSubscriptions)
    {
    	// nothing to do
    }

    void
    ClientSubscription::subscriptionServiceDeleteSubscriptionsResponse(ServiceTransactionDeleteSubscriptions::SPtr serviceTransactionDeleteSubscriptions)
    {
		Log(Debug, "close subscription")
			.parameter("Id", id_)
		    .parameter("SubscriptionId", subscriptionId_);

    	subscriptionId_ = 0;
    	state_ = S_Close;

    	// clean up all monitored items
    	cleanUpMonitoredItems();
    }

    void
    ClientSubscription::dataChangeNotification(const MonitoredItemNotification::SPtr& monitoredItem)
    {
    }

	void
	ClientSubscription::subscriptionStateUpdate(SubscriptionState subscriptionState, uint32_t subscriptionId)
	{
		Log(Debug, "state update subscription")
			.parameter("Id", id_)
		    .parameter("SubscriptionId", subscriptionId_)
		    .parameter("SubscriptionState", (uint32_t)subscriptionState);

		// FIXME: todo
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// MonitoredItemService
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
    void
    ClientSubscription::monitoredItemServiceCreateMonitoredItemsResponse(ServiceTransactionCreateMonitoredItems::SPtr serviceTransactionCreateMonitoredItems)
    {
    	;
    }

    void
    ClientSubscription::monitoredItemServiceDeleteMonitoredItemsResponse(ServiceTransactionDeleteMonitoredItems::SPtr serviceTransactionDeleteMonitoredItems)
    {
    	// nothing to do
    }

    void
    ClientSubscription::monitoredItemServiceModifyMonitoredItemsResponse(ServiceTransactionModifyMonitoredItems::SPtr serviceTransactionModifyMonitoredItems)
    {
    	// nothing to do
    }

    void
    ClientSubscription::monitoredItemServiceSetMonitoringModeResponse(ServiceTransactionSetMonitoringMode::SPtr serviceTransactionSetMonitoringMode)
    {
    	// nothing to do
    }

    void
    ClientSubscription::monitoredItemServiceSetTriggeringResponse(ServiceTransactionSetTriggering::SPtr serviceTransactionSetTriggering)
    {
    	// nothing to do
    }

}
