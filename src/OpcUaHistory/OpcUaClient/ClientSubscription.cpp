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
	, publishingInterval_(100)
	, livetimeCount_(100)
	, maxKeepAliveCount_(100)
	, maxNotificationsPerPublish_(0)
	, namespaceMap_(nullptr)
	, subscriptionService_()
	, monitoredItemService_()
	, serviceSetManager_(nullptr)
	, sessionService_()
	, clientMonitoredItemMap_()
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
	ClientSubscription::open(NamespaceMap& namespaceMap)
	{
		namespaceMap_ = &namespaceMap;
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

		// add monitored item to map
		clientMonitoredItemMap_.insert(
			std::make_pair(clientMonitoredItem->clientHandle(), clientMonitoredItem)
		);
	}

	void
	ClientSubscription::openMonitoredItems(void)
	{
		if (state_ != S_Open) return;

		ClientMonitoredItem::Vec cmiv;
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();


		// find all monitored items to be opened
		ClientMonitoredItem::IdMap::iterator it1;
		for (it1 = clientMonitoredItemMap_.begin(); it1 != clientMonitoredItemMap_.end(); it1++) {
			ClientMonitoredItem::SPtr cmi = it1->second;

			if (cmi->state() != ClientMonitoredItem::S_Close) {
				// the monitored item is already opened
				continue;
			}

			if (cmi->reconnectTime() > now) {
				// reconnect time has not been reached
				continue;
			}

			cmiv.push_back(cmi);
		}
		if (cmiv.empty()) return;

		// create monitored item transaction
		ServiceTransactionCreateMonitoredItems::SPtr trx = ServiceTransactionCreateMonitoredItems::construct();
		CreateMonitoredItemsRequest::SPtr req = trx->request();
		req->subscriptionId(subscriptionId_);
		req->itemsToCreate()->resize(cmiv.size());


		// open the found monitored items
		ClientMonitoredItem::Vec::iterator it2;
		for (it2 = cmiv.begin(); it2 != cmiv.end(); it2++) {
			ClientMonitoredItem::SPtr cmi = *it2;

			OpcUaNodeId nodeId = cmi->nodeId();

			// determine namespace index
			NamespaceMap::iterator it3;
			it3 = namespaceMap_->find(cmi->nodeId().namespaceIndex());
			if (it3 == namespaceMap_->end()) {
				Log(Error, "cannot create monitored items, because namespace index not exist in namespace map")
					.parameter("Id", id_)
				    .parameter("SubscriptionId", subscriptionId_)
					.parameter("NodeId", nodeId.toString())
					.parameter("ClientHandle", cmi->clientHandle());
				return;
			}
			nodeId.namespaceIndex(it3->second);

			Log(Debug, "Try to create monitored item")
				.parameter("Id", id_)
				.parameter("SubscriptionId", subscriptionId_)
			    .parameter("NodeId", nodeId.toString())
			    .parameter("ClientHandle", cmi->clientHandle());

			MonitoredItemCreateRequest::SPtr monitoredItemCreateRequest = MonitoredItemCreateRequest::construct();
			monitoredItemCreateRequest->itemToMonitor().nodeId()->copyFrom(nodeId);
			monitoredItemCreateRequest->requestedParameters().clientHandle(cmi->clientHandle());
			req->itemsToCreate()->push_back(monitoredItemCreateRequest);
		}

		monitoredItemService_->asyncSend(trx);
	}

	void
	ClientSubscription::cleanUpMonitoredItems(void)
	{
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

		// clean up all monitored items
		ClientMonitoredItem::IdMap::iterator it;
		for (it = clientMonitoredItemMap_.begin(); it != clientMonitoredItemMap_.end(); it++) {
			ClientMonitoredItem::SPtr cmi = it->second;
			cmi->reconnectTime(now);
			cmi->state(ClientMonitoredItem::S_Close);
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
    	// FIXME: todo
    	std::cout << "data change notification..." << std::endl;
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
		OpcUaStatusCode statusCode = serviceTransactionCreateMonitoredItems->statusCode();
		if (statusCode != Success) {
			Log(Error, "create monitor item request error")
				.parameter("Id", id_)
				.parameter("SubscriptionId", subscriptionId_)
				.parameter("StatusCode", OpcUaStatusCodeMap::shortString(statusCode));
			return;
		}

		CreateMonitoredItemsRequest::SPtr req = serviceTransactionCreateMonitoredItems->request();
		CreateMonitoredItemsResponse::SPtr res = serviceTransactionCreateMonitoredItems->response();

		if (req->itemsToCreate()->size() != res->results()->size()) {
			Log(Error, "create monitored item response error (size)")
				.parameter("Id", id_)
				.parameter("SubscriptionId", subscriptionId_)
				.parameter("ReqSize", req->itemsToCreate()->size())
				.parameter("ResSize", res->results()->size());
			return;
		}

		for (uint32_t idx=0; idx < req->itemsToCreate()->size(); idx++) {
			MonitoredItemCreateRequest::SPtr monitoredItemCreateRequest;
			req->itemsToCreate()->get(idx, monitoredItemCreateRequest);
			if (monitoredItemCreateRequest.get() == nullptr) {
				Log(Error, "create monitored item response error, because request content not found")
					.parameter("Id", id_)
					.parameter("SubscriptionId", subscriptionId_)
					.parameter("Idx", idx);
				continue;
			}

			uint32_t clientHandle = monitoredItemCreateRequest->requestedParameters().clientHandle();
			ClientMonitoredItem::IdMap::iterator it;
			it = clientMonitoredItemMap_.find(clientHandle);
			if (it == clientMonitoredItemMap_.end()) {
				Log(Error, "create monitored item response error, because monitor item not found")
					.parameter("Id", id_)
					.parameter("SubscriptionId", subscriptionId_)
					.parameter("Idx", idx)
					.parameter("ClientHandle", clientHandle);
				continue;
			}
			ClientMonitoredItem::SPtr cmi = it->second;

			MonitoredItemCreateResult::SPtr monitoredItemCreateResult;
			res->results()->get(idx, monitoredItemCreateResult);
			if (monitoredItemCreateResult.get() == nullptr) {
				Log(Error, "create monitored item response error, because response content not found")
					.parameter("Id", id_)
					.parameter("SubscriptionId", subscriptionId_)
					.parameter("Idx", idx)
				    .parameter("NodeId", cmi->nodeId().toString())
				    .parameter("ClientHandle", clientHandle);
				continue;
			}
			if (monitoredItemCreateResult->statusCode() != Success) {
				Log(Error, "create monitored item response error, because server response error")
					.parameter("Id", id_)
					.parameter("SubscriptionId", subscriptionId_)
					.parameter("Idx", idx)
				    .parameter("NodeId", cmi->nodeId().toString())
				    .parameter("ClientHandle", clientHandle)
				    .parameter("StatusCode", OpcUaStatusCodeMap::shortString(monitoredItemCreateResult->statusCode()));
				continue;
			}

			Log(Error, "create monitored item")
				.parameter("Id", id_)
				.parameter("SubscriptionId", subscriptionId_)
				.parameter("NodeId", cmi->nodeId().toString())
				.parameter("ClientHandle", clientHandle)
				.parameter("MonitoredItemId", monitoredItemCreateResult->monitoredItemId());

			cmi->monitoredItemId(monitoredItemCreateResult->monitoredItemId());
			cmi->state(ClientMonitoredItem::S_Open);
		}
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
