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
	, ioThread_()
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

    bool
    HistoryClient::startup(
    	const std::string& fileName,
    	ConfigXmlManager& configXmlManager
    )
    {
    	if (!historyClientConfig_.decode(fileName, configXmlManager)) {
    		return false;
    	}

    	// configure client connection
    	clientConnection_.serverUri(historyClientConfig_.serverUri());
    	clientConnection_.reconnectTimeout(historyClientConfig_.reconnectTimeout());
    	clientConnection_.ioThread(ioThread_);
    	clientConnection_.namespaceUris(historyClientConfig_.namespaceUris());

    	// create subscriptions
    	ClientSubscriptionConfig::Map::iterator it1;
    	for (it1 = historyClientConfig_.clientSubscriptionMap().begin();
    		 it1 != historyClientConfig_.clientSubscriptionMap().end();
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
    		ClientNodeConfig::Map::iterator it2;
    		for (it2 = csc->clientNodeConfigMap().begin();
    			 it2 != csc->clientNodeConfigMap().end();
    			 it2++) {

    			//
    			// create monitored item
    			//
    			ClientNodeConfig::SPtr cnc = it2->second;
    			ClientMonitoredItem::SPtr cmi = constructSPtr<ClientMonitoredItem>();

    			clientHandle++;
    			cmi->clientHandle(clientHandle);
    			cmi->samplingInterval(cnc->samplingInterval());
    			cmi->queueSize(cnc->queueSize());
    			cmi->dataChangeFilter(cnc->dataChangeFilter());
    			cmi->nodeId(cnc->nodeId());

    			cs->addMonitoredItem(cmi);
    		}
    	}

    	// open connection to server
    	return clientConnection_.connect();
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
    	std::cout << "data change notification ...";
    	dataValue.out(std::cout);
    	std::cout << std::endl;

    }


}
