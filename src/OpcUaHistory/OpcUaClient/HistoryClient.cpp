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
#include "OpcUaHistory/OpcUaClient/HistoryClient.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryClient::HistoryClient(void)
	: clientHandle_(0)
	, variableElementVec_()
	, clientConfigIf_(nullptr)
	, clientConfig_()
	, ioThread_()
	, historyStoreIf_(nullptr)
	, discoveryClient_()
	, mainConfig_(nullptr)
	, discoveryEnabled_(false)
	{
	}

	HistoryClient::~HistoryClient(void)
	{
	}

	void
	HistoryClient::clientConfigIf(ApplicationClientConfigIf* clientConfigIf)
	{
		clientConfigIf_ = clientConfigIf;
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

	void
	HistoryClient::mainConfig(Config* config)
	{
		mainConfig_ = config;
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

		// get client namespaces
		NamespaceElement::Vec namespaceElementVec;
		clientConfigIf_->clientNamespaces(namespaceElementVec);

		// get client references
		clientConfigIf_->clientVariables(variableElementVec_);

		// startup discovery service
		if (!startupDiscovery()) {
			return false;
		}

    	// configure client connection
    	clientConnection_.serverUri(clientConfig_.clientEndpointConfig().serverUri());
    	clientConnection_.reconnectTimeout(clientConfig_.clientEndpointConfig().reconnectTimeout());
    	clientConnection_.sessionName("OpcUaHistoryServer");
    	clientConnection_.ioThread(ioThread_);
    	clientConnection_.namespaceElementVec(namespaceElementVec);

    	if (!createSubscriptions()) {
    		return false;
    	}

    	// open connection to server
    	clientConnection_.connect();
    	return true;
    }

    bool
    HistoryClient::createSubscriptions(void)
    {
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

    		if (!createMonitoredItems(csc, cs)) {
    			return false;
    		}
    	}

    	return true;
    }

    bool
    HistoryClient::createMonitoredItems(
    	ClientSubscriptionConfig::SPtr& csc, ClientSubscription::SPtr& cs
    )
    {
    	uint32_t clientHandle = 0;
    	ClientMonitoredItemConfig::Map::iterator it;
    	for (it = csc->clientNodeConfigMap().begin();
    		it != csc->clientNodeConfigMap().end();
    		it++)
    	{
    		ClientMonitoredItemConfig::SPtr cmic = it->second;

    		if (!createMonitoredItems(csc, cs, cmic)) {
    			return false;
    		}

    	}

    	return true;
    }

    bool
    HistoryClient::createMonitoredItems(
     	ClientSubscriptionConfig::SPtr& csc,
     	ClientSubscription::SPtr& cs,
     	ClientMonitoredItemConfig::SPtr& cmic
    )
    {
    	std::string monitredItem = clientConfig_.id() + "." + csc->id() + "." + cmic->id();

    	VariableElement::Vec::iterator it1;
    	for (it1=variableElementVec_.begin(); it1!=variableElementVec_.end(); it1++) {

    		OpcUaReferenceConfig::Vec::iterator it2;
    		for (it2=it1->references_.begin(); it2!=it1->references_.end(); it2++) {

    			OpcUaReferenceConfig::SPtr ref = *it2;

    			if (ref->service() != OpcUaReferenceConfig::Mon) continue;

    			OpcUaMonReferenceConfig::SPtr refExt = boost::static_pointer_cast<OpcUaMonReferenceConfig>(ref->extension());
    			if (monitredItem != refExt->handle()) continue;

    			Object::SPtr context;
    			historyStoreIf_->getHistoryStoreContext(it1->name_, context, HistoryStoreIf::Write);

    			// create new monitored item
    			ClientMonitoredItem::SPtr cmi = constructSPtr<ClientMonitoredItem>();

       			clientHandle_++;
        		cmi->clientHandle(clientHandle_);
        		cmi->samplingInterval(cmi->samplingInterval());
        		cmi->queueSize(cmi->queueSize());
        		cmi->dataChangeFilter(cmi->dataChangeFilter());
        		cmi->nodeId(ref->nodeId());
        		cmi->context(context);

    			cs->addMonitoredItem(cmi);
    		}

    	}

    	return true;

    }

    bool
    HistoryClient::shutdown(void)
    {
    	// shutdown discovery client
    	shutdownDiscovery();

    	// close client connection
    	clientConnection_.syncDisconnect();

    	return true;
    }

    void
    HistoryClient::dataChangeNotification(ClientMonitoredItem::SPtr& clientMonitoredItem, OpcUaDataValue& dataValue)
    {
    	historyStoreIf_->write(clientMonitoredItem->context(), dataValue);
    }

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    //
    // discovery process
    //
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    bool
    HistoryClient::startupDiscovery(void)
    {
    	// read discover uri from configuration file
    	std::string discoveryUri = "";
    	mainConfig_->getConfigParameter("OpcUaServer.DiscoveryServer.DiscoveryUrl", discoveryUri, "");

    	// read server urn
    	std::string serverUrn = clientConfig_.clientEndpointConfig().serverUrn();

    	// check if discovery process is enabled
    	if (discoveryUri.empty() || serverUrn.empty()) {
    		return true;
    	}

    	// startup discovery client
    	discoveryEnabled_ = true;
    	discoveryClient_.ioThread(ioThread_);
    	discoveryClient_.discoveryUri(discoveryUri);
    	return discoveryClient_.startup();
    }

    bool
    HistoryClient::shutdownDiscovery(void)
    {
    	if (discoveryEnabled_) {
    		discoveryClient_.shutdown();
    	}
    	return true;
    }
}
