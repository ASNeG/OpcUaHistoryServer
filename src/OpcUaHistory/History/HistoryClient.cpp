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
	, attributeService_()
	, subscriptionService_()
	, monitoredItemService_()

	, state_(S_Close)
	, namespaceMap_()

	, subscriptionId_(0)

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

		// create attribute service
		AttributeServiceConfig attributeServiceConfig;
		attributeServiceConfig.ioThreadName("GlobalIOThread");
		attributeServiceConfig.attributeServiceIf_ = this;
		attributeService_ = serviceSetManager_.attributeService(sessionService_, attributeServiceConfig);

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
				readNamespaceArray();
				break;
			case SS_Disconnect:
				Log(Debug, "session state changed to disconnect/close");
				state_ = S_Close;
				closeSubscriptions();
				break;
			case SS_Reactivate:
				Log(Debug, "session state changed to reactivate");
				break;
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// AttributeServiceIf
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void
	HistoryClient::attributeServiceReadResponse(ServiceTransactionRead::SPtr serviceTransactionRead)
	{
		handleNamespaceArray(serviceTransactionRead);
	}

	void
	HistoryClient::attributeServiceWriteResponse(ServiceTransactionWrite::SPtr serviceTransactionWrite)
	{
		// nothing to do
	}

	void
	HistoryClient::attributeServiceHistoryReadResponse(ServiceTransactionHistoryRead::SPtr serviceTransactionHistoryRead)
	{
		// nothing to do
	}

	void
	HistoryClient::attributeServiceHistoryUpdateResponse(ServiceTransactionHistoryUpdate::SPtr serviceTransactionHistoryUpdate)
	{
		// nothing to do
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// SubscriptionServiceIf
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
    void
    HistoryClient::openSubscriptions(void)
    {
    	// FIXME: actual only one subscription is allowed
		ServiceTransactionCreateSubscription::SPtr trx = ServiceTransactionCreateSubscription::construct();
		subscriptionService_->asyncSend(trx);
    }

    void
    HistoryClient::closeSubscriptions(void)
    {
		ServiceTransactionDeleteSubscriptions::SPtr trx = constructSPtr<ServiceTransactionDeleteSubscriptions>();
		DeleteSubscriptionsRequest::SPtr req = trx->request();
		req->subscriptionIds()->resize(1);
		req->subscriptionIds()->set(0, subscriptionId_);

		subscriptionService_->asyncSend(trx);
    }

    void
    HistoryClient::subscriptionServiceCreateSubscriptionResponse(ServiceTransactionCreateSubscription::SPtr trx)
    {
		CreateSubscriptionResponse::SPtr res = trx->response();
		if (trx->responseHeader()->serviceResult() != Success) {
			Log(Error, "create subscription response error")
				.parameter("StatusCode", OpcUaStatusCodeMap::shortString(trx->responseHeader()->serviceResult()));
			return;
		}

		subscriptionId_ = res->subscriptionId();
		Log(Debug, "open subscription")
		    .parameter("SubscriptionId", subscriptionId_);

		createMonitoredItems();
    }

    void
    HistoryClient::subscriptionServiceModifySubscriptionResponse(ServiceTransactionModifySubscription::SPtr serviceTransactionModifySubscription)
    {
    	// nothing to do
    }

    void
    HistoryClient::subscriptionServiceTransferSubscriptionsResponse(ServiceTransactionTransferSubscriptions::SPtr serviceTransactionTransferSubscriptions)
    {
    	// nothing to do
    }

    void
    HistoryClient::subscriptionServiceDeleteSubscriptionsResponse(ServiceTransactionDeleteSubscriptions::SPtr serviceTransactionDeleteSubscriptions)
    {
		Log(Debug, "close subscription")
		    .parameter("SubscriptionId", subscriptionId_);

    	subscriptionId_ = 0;
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
    HistoryClient::createMonitoredItems(void)
    {
    }

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

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// handle namespace array
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void
	HistoryClient::readNamespaceArray(void)
	{
		ServiceTransactionRead::SPtr readTrx = constructSPtr<ServiceTransactionRead>();
		ReadRequest::SPtr req = readTrx->request();
		req->maxAge(0);
		req->timestampsToReturn(2);

		ReadValueId::SPtr readValueId = ReadValueId::construct();
		readValueId->nodeId((OpcUaInt16)0, (OpcUaInt32)2255);
		readValueId->attributeId((OpcUaInt32) 13);
		readValueId->dataEncoding().namespaceIndex((OpcUaInt16) 0);
		req->readValueIdArray()->set(readValueId);

		attributeService_->asyncSend(readTrx);
	}

	void
	HistoryClient::handleNamespaceArray(ServiceTransactionRead::SPtr serviceTransactionRead)
	{
		ReadRequest::SPtr req = serviceTransactionRead->request();
		ReadResponse::SPtr res = serviceTransactionRead->response();

		// error handling
		OpcUaStatusCode statusCode = serviceTransactionRead->statusCode();
		if (statusCode != Success) {
			Log(Error, "read namespace array transaction error")
				.parameter("StatusCode", OpcUaStatusCodeMap::shortString(statusCode));
			return;
		}
		if (res->dataValueArray()->size() != 1) {
			Log(Error, "read namespace array response error, because data array size error");
			return;
		}

		// get data value
		OpcUaDataValue::SPtr dataValue;
		res->dataValueArray()->get(0, dataValue);

		if (dataValue->statusCode() != Success) {
			Log(Error, "read namespace array data value error")
				.parameter("StatusCode", OpcUaStatusCodeMap::shortString(statusCode));
			return;
		}
		OpcUaVariant::SPtr variant = dataValue->variant();

		// get namespace array
		std::map<std::string, uint32_t> namespaceMap;
		std::map<std::string, uint32_t>::iterator it;
		Log(Debug, "read namespace array from server")
		    .parameter("ServerUri", historyClientConfig_.serverUri());
		for (int32_t idx=0; idx < variant->arrayLength(); idx++) {
			std::string uri = variant->variant()[idx].variantSPtr<OpcUaString>()->value();
			Log(Debug, "")
				.parameter("Uri", uri)
				.parameter("NamespaceIndex", idx);
			namespaceMap.insert(std::make_pair(uri, idx));
		}

		// create namespace mapping
		namespaceMap_.clear();
		for (uint32_t idx=0; idx<historyClientConfig_.namespaceUris().size(); idx++) {
			uint32_t namespaceIndex = idx+1;
			std::string namespaceName = historyClientConfig_.namespaceUris()[idx];

			it = namespaceMap.find(namespaceName);
			if (it == namespaceMap.end()) {
				Log(Error, "namespace name not exist on server")
				    .parameter("ServerUri", historyClientConfig_.serverUri())
				    .parameter("NamespaceName", namespaceName);
				return;
			}

			namespaceMap_.insert(std::make_pair(namespaceIndex, it->second));
		}

		Log(Debug, "session state changed to open");
		state_ = S_Open;

		openSubscriptions();
	}

}
