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
    	if (!historyClientConfig_.decode(fileName, configXmlManager)) {
    		return false;
    	}

    	// configure client connection
    	clientConnection_.serverUri(historyClientConfig_.serverUri());
    	clientConnection_.reconnectTimeout(historyClientConfig_.reconnectTimeout());
    	clientConnection_.ioThread(ioThread);
    	clientConnection_.startup();

    	// create subscriptions
    	ClientSubscriptionConfig::Map::iterator it;
    	for (it = historyClientConfig_.clientSubscriptionMap().begin();
    		 it != historyClientConfig_.clientSubscriptionMap().end();
    		 it++) {

    		ClientSubscriptionConfig::SPtr csc = it->second;
    		ClientSubscription::SPtr cs = constructSPtr<ClientSubscription>();

    		cs->id(csc->id());
    		cs->publishingInterval(csc->publishingInterval());
    		cs->livetimeCount(csc->livetimeCount());
    		cs->maxKeepAliveCount(csc->maxKeepAliveCount());
    		cs->maxNotificationsPerPublish(csc->maxNotificationsPerPublish());

    		clientConnection_.clientSubscription(csc->id(), cs);
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

}
