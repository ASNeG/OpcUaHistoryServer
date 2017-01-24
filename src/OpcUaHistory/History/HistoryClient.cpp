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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/HistoryClient.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryClient::HistoryClient(void)
	: clientConfig_()
	, ioThread_()
	, historyStoreIf_(nullptr)
	{
	}

	HistoryClient::~HistoryClient(void)
	{
	}

	void
	HistoryClient::ioThread(IOThread::SPtr& ioThread)
	{
		ioThread_ = ioThread;
	}

	void
	HistoryClient::historyStoreIf(HistoryStoreIf* historyStoreIf)
	{
		historyStoreIf_ = historyStoreIf;
	}

    bool
    HistoryClient::startup(
    	const std::string& fileName,
    	ConfigXmlManager& configXmlManager
    )
    {
    	if (!clientConfig_.decode(fileName, configXmlManager)) {
    		return false;
    	}

    	// configure client connection
    	clientConnection_.serverUri(clientConfig_.clientEndpointConfig().serverUri());
    	clientConnection_.reconnectTimeout(clientConfig_.clientEndpointConfig().reconnectTimeout());
    	clientConnection_.ioThread(ioThread_);
    	//clientConnection_.namespaceUris(clientConfig_.namespaceUris());

#if 0

    	// create subscriptions
    	ClientSubscriptionConfig::Map::iterator it1;
    	for (it1 = clientConfig_.clientSubscriptionMap().begin();
    		 it1 != clientConfig_.clientSubscriptionMap().end();
    		 it1++) {

    		//
    		// create subscription
    		//
    		ClientSubscriptionConfig::SPtr csc = it1->second;
    		ClientSubscription::SPtr cs = constructSPtr<ClientSubscription>();

    		cs->id(csc->id());
    		cs->publishingInterval(csc->publishingInterval());
    		cs->livetimeCount(csc->livetimeCount());
    		cs->maxKeepAliveCount(csc->maxKeepAliveCount());
    		cs->maxNotificationsPerPublish(csc->maxNotificationsPerPublish());
    		cs->clientSubscriptionIf(this);

    		clientConnection_.addClientSubscription(csc->id(), cs);


    		uint32_t clientHandle = 0;
    		ClientMonitoredItemConfig::Map::iterator it2;
    		for (it2 = csc->clientNodeConfigMap().begin();
    			 it2 != csc->clientNodeConfigMap().end();
    			 it2++) {

    			//
    			// create monitored item
    			//
    			ClientMonitoredItemConfig::SPtr cnc = it2->second;
    			ClientMonitoredItem::SPtr cmi = constructSPtr<ClientMonitoredItem>();

    			Object::SPtr context;
    			historyStoreIf_->getHistoryStoreContext(cnc->valueName(), context, HistoryStoreIf::Write);

    			clientHandle++;
    			cmi->clientHandle(clientHandle);
    			cmi->samplingInterval(cnc->samplingInterval());
    			cmi->queueSize(cnc->queueSize());
    			cmi->dataChangeFilter(cnc->dataChangeFilter());
    			cmi->nodeId(cnc->nodeId());
    			cmi->context(context);

    			cs->addMonitoredItem(cmi);
    		}
    	}

    	// open connection to server
    	return clientConnection_.connect();
#endif
    	return true;
    }

    bool
    HistoryClient::shutdown(void)
    {
    	clientConnection_.disconnect();
    	return true;
    }

    void
    HistoryClient::dataChangeNotification(ClientMonitoredItem::SPtr& clientMonitoredItem, OpcUaDataValue& dataValue)
    {
    	historyStoreIf_->write(clientMonitoredItem->context(), dataValue);
    }


}
