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
#include "OpcUaHistory/History/HistoryClientConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class ClientNodeConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ClientNodeConfig::ClientNodeConfig(void)
	: samplingInterval_(0)
	, queueSize_(0)
	, nodeId_()
	, valueName_("")
	, dataChangeFilter_(StatusValue)
	{
	}

	ClientNodeConfig::~ClientNodeConfig(void)
	{
	}

	uint32_t
	ClientNodeConfig::samplingInterval(void)
	{
		return samplingInterval_;
	}

	void
	ClientNodeConfig::samplingInterval(uint32_t samplingInterval)
	{
		samplingInterval_ = samplingInterval;
	}

	uint32_t
	ClientNodeConfig::queueSize(void)
	{
		return queueSize_;
	}

	void
	ClientNodeConfig::queueSize(uint32_t queueSize)
	{
		queueSize_ = queueSize;
	}

	OpcUaNodeId&
	ClientNodeConfig::nodeId(void)
	{
		return nodeId_;
	}

	void
	ClientNodeConfig::nodeId(OpcUaNodeId& nodeId)
	{
		nodeId_ = nodeId;
	}

	std::string&
	ClientNodeConfig::valueName(void)
	{
		return valueName_;
	}

	void
	ClientNodeConfig::valueName(const std::string& valueName)
	{
		valueName_ = valueName;
	}

	ClientNodeConfig::DataChangeFilter
	ClientNodeConfig::dataChangeFilter(void)
	{
		return dataChangeFilter_;
	}

	void
	ClientNodeConfig::dataChangeFilter(DataChangeFilter dataChangeFilter)
	{
		dataChangeFilter_ = dataChangeFilter;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class ClientSubscriptionConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ClientSubscriptionConfig::ClientSubscriptionConfig(void)
	: id_("")
	, publishingInterval_(0)
	, livetimeCount_(0)
	, maxKeepAliveCount_(0)
	, maxNotificationsPerPublish_(0)
	, clientNodeConfigMap_()
	{
	}

	ClientSubscriptionConfig::~ClientSubscriptionConfig(void)
	{
	}

	std::string
	ClientSubscriptionConfig::id(void)
	{
		return id_;
	}

	void
	ClientSubscriptionConfig::id(const std::string& id)
	{
		id_ = id;
	}

	uint32_t
	ClientSubscriptionConfig::publishingInterval(void)
	{
		return publishingInterval_;
	}

	void
	ClientSubscriptionConfig::publisingInterval(uint32_t publishingInterval)
	{
		publishingInterval_ =publishingInterval;
	}

	uint32_t
	ClientSubscriptionConfig::livetimeCount(void)
	{
		return livetimeCount_;
	}

	void
	ClientSubscriptionConfig::livetimeCount(uint32_t livetimeCount)
	{
		livetimeCount_ = livetimeCount;
	}

	uint32_t
	ClientSubscriptionConfig::maxKeepAliveCount(void)
	{
		return maxKeepAliveCount_;
	}

	void
	ClientSubscriptionConfig::maxKeepAliveCount(uint32_t maxKeepAliveCount)
	{
		maxKeepAliveCount_ = maxKeepAliveCount;
	}

	uint32_t
	ClientSubscriptionConfig::maxNotificationsPerPublish(void)
	{
		return maxNotificationsPerPublish_;
	}

	void
	ClientSubscriptionConfig::maxNotificationsPerPublish(uint32_t maxNotificationsPerPublish)
	{
		maxNotificationsPerPublish_ = maxNotificationsPerPublish;
	}

	ClientNodeConfig::Map&
	ClientSubscriptionConfig::clientNodeConfigMap(void)
	{
		return clientNodeConfigMap_;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class HistoryClientConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	HistoryClientConfig::HistoryClientConfig(void)
	: serverUri_("")
	, namespaceUris_()
	, clientSubscriptionConfigMap_()
	{
	}

	HistoryClientConfig::~HistoryClientConfig(void)
	{
	}

	bool
	HistoryClientConfig::decode(const std::string& configFileName, ConfigXmlManager& configXmlManager)
	{
		bool success;

		//
		// read configuration file
		//
        Log(Info, "read configuration file")
            .parameter("ConfigFileName", configFileName);
		Config::SPtr config;
		success = configXmlManager.registerConfiguration(configFileName, config);
		if (!success) {
			Log(Error, "read configuration file error")
			   .parameter("ConfigFile", configFileName);
			   return false;
		}

		//
		// decode Endpoint configuration
		//
		if (!decodeEndpoint(config)) {
			Log(Error, "read configuration file error")
				.parameter("ConfigFile", configFileName);
			return false;
		}

		//
		// decode NamespaceUris configuration
		//
		if (!decodeNamespaceUris(config)) {
			Log(Error, "read configuration file error")
				.parameter("ConfigFile", configFileName);
			return false;
		}

		//
		// decode Subscription configuration
		//
		if (!decodeSubscriptions(config)) {
			Log(Error, "read configuration file error")
				.parameter("ConfigFile", configFileName);
			return false;
		}

		return true;
	}

	bool
	HistoryClientConfig::decodeEndpoint(Config::SPtr& config)
	{
		bool success;

		// read Endpoint
		boost::optional<Config> child = config->getChild("OpcUaClientModel.Endpoint");
		if (!child) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Endpoint");
			return false;
		}

		// read ServerUri
		success = child->getConfigParameter("ServerUri", serverUri_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Endpoint.ServerUri");
			return false;
		}

		return true;
	}

	bool
	HistoryClientConfig::decodeNamespaceUris(Config::SPtr& config)
	{
		bool success;

		// read namespace uris
		config->getValues("OpcUaClientModel.NamespaceUris.Uri", namespaceUris_);
		if (namespaceUris_.size() == 0) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaClientModel.NamespaceUris.Uri");
			return false;
		}

		return true;
	}

	bool
	HistoryClientConfig::decodeSubscriptions(Config::SPtr& config)
	{
		// read list of subscriptions
		std::vector<Config> childs;
		config->getChilds("OpcUaClientModel.Subscription", childs);
		if (childs.size() == 0) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription");
			return false;
		}

		std::vector<Config>::iterator it;
		for (it = childs.begin(); it != childs.end(); it++) {
			if (!decodeSubscription(*it)) return false;
		}

		return true;
	}

	bool
	HistoryClientConfig::decodeSubscription(Config& config)
	{
		ClientSubscriptionConfig::SPtr subscription = constructSPtr<ClientSubscriptionConfig>();

		// subscription id
		std::string id;
		if (!config.getConfigParameter("<xmlattr>.Id", id)) {
			Log(Error, "attribute missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription")
				.parameter("Attribute", "Id");
			return false;
		}
		subscription->id(id);

		// publishing interval
		uint32_t publishingInterval;
		if (!config.getConfigParameter("PublishingInterval", publishingInterval)) {
			Log(Error, "parameter missing or invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.PublishingInterval")
				.parameter("SubscriptionId", id);
			return false;
		}
		subscription->publisingInterval(publishingInterval);

		// max keepalive count
		uint32_t maxKeepAliveCount;
		if (!config.getConfigParameter("MaxKeepAliveCount", maxKeepAliveCount)) {
			Log(Error, "parameter missing or invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.MaxKeepAliveCount")
				.parameter("SubscriptionId", id);
			return false;
		}
		subscription->maxKeepAliveCount(maxKeepAliveCount);

		// max notification per publish
		uint32_t maxNotificationsPerPublish;
		if (!config.getConfigParameter("MaxNotificationsPerPublish", maxNotificationsPerPublish)) {
			Log(Error, "parameter missing or invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.MaxNotificationsPerPublish")
				.parameter("SubscriptionId", id);
			return false;
		}

		// node lists
		std::vector<Config> childs;
		config.getChilds("NodeList", childs);
		if (childs.size() == 0) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList");
			return false;
		}

		std::vector<Config>::iterator it;
		for (it = childs.begin(); it != childs.end(); it++) {
			if (!decodeNodeList(*it, subscription->clientNodeConfigMap())) return false;
		}

		ClientSubscriptionConfig::Map::iterator it1;
		it1 = clientSubscriptionConfigMap_.find(id);
		if (it1 != clientSubscriptionConfigMap_.end()) {
			Log(Error, "dublicate node name in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.Id")
				.parameter("ValueName", id);
			return false;
		}
		clientSubscriptionConfigMap_.insert(std::make_pair(id, subscription));

		return true;
	}

	bool
	HistoryClientConfig::decodeNodeList(Config& config, ClientNodeConfig::Map& clientNodeConfigMap)
	{
		// node list id
		std::string id;
		if (!config.getConfigParameter("<xmlattr>.Id", id)) {
			Log(Error, "attribute missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList")
				.parameter("Attribute", "Id");
			return false;
		}

		// sampling interval
		uint32_t samplingInterval;
		if (!config.getConfigParameter("SamplingInterval", samplingInterval)) {
			Log(Error, "parameter missing or invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList.SamplingInterval")
				.parameter("NodeListId", id);
			return false;
		}

		// queue size
		uint32_t queueSize;
		if (!config.getConfigParameter("QueueSize", queueSize)) {
			Log(Error, "parameter missing or invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList.QueueSize")
				.parameter("NodeListId", id);
			return false;
		}

		// data change filter
		std::string dataChangeFilter;
		ClientNodeConfig::DataChangeFilter dataChangeFilterType;
		if (!config.getConfigParameter("DataChangeFilter", dataChangeFilter)) {
			Log(Error, "parameter missing or invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList.DataChangeFilter")
				.parameter("NodeListId", id);
			return false;
		}
		if (dataChangeFilter == "status") {
			dataChangeFilterType = ClientNodeConfig::Status;
		}
		else if (dataChangeFilter == "status-value") {
			dataChangeFilterType = ClientNodeConfig::StatusValue;
		}
		else if (dataChangeFilter == "status-value-timestamp") {
			dataChangeFilterType = ClientNodeConfig::StatusValueTimestamp;
		}
		else {
			Log(Error, "parameter invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList.DataChangeFilter")
				.parameter("NodeListId", id)
				.parameter("DataChangeFilter", dataChangeFilter);
			return false;
		}

		// node
		std::vector<Config> childs;
		config.getChilds("Node", childs);
		if (childs.size() == 0) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList.Node");
			return false;
		}

		std::vector<Config>::iterator it;
		for (it = childs.begin(); it != childs.end(); it++) {
			ClientNodeConfig::SPtr node = constructSPtr<ClientNodeConfig>();
			node->samplingInterval(samplingInterval);
			node->queueSize(queueSize);
			node->dataChangeFilter(dataChangeFilterType);

			if (!decodeNode(*it, *node.get())) return false;

			ClientNodeConfig::Map::iterator it;
			it = clientNodeConfigMap.find(node->valueName());
			if (it != clientNodeConfigMap.end()) {
				Log(Error, "dublicate node name in config file")
					.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList.Node.ValueName")
					.parameter("ValueName", node->valueName());
				return false;
			}
			clientNodeConfigMap.insert(std::make_pair(node->valueName(), node));
		}

		return true;
	}

	bool
	HistoryClientConfig::decodeNode(Config& config, ClientNodeConfig& clientNodeConfig)
	{
		// node id
		std::string nodeIdString;
		if (!config.getConfigParameter("<xmlattr>.NodeId", nodeIdString)) {
			Log(Error, "attribute missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList.Node")
				.parameter("Attribute", "NodeId");
			return false;
		}
		OpcUaNodeId nodeId;
		if (!nodeId.fromString(nodeIdString)) {
			Log(Error, "attribute invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList.Node")
				.parameter("Attribute", "NodeId")
				.parameter("NodeId", nodeIdString);
			return false;
		}
		clientNodeConfig.nodeId(nodeId);

		// value name
		std::string valueName;
		if (!config.getConfigParameter("<xmlattr>.ValueName", valueName)) {
			Log(Error, "attribute missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList.Node")
				.parameter("Attribute", "ValueName");
			return false;
		}
		clientNodeConfig.valueName(valueName);

		return true;
	}


#if 0
			  	<Node NodeId="ns=1;s=aaa" ValueName="Value1"/>
			  	<Node NodeId="ns=1;s=bbb" ValueName="Value2"/>
#endif

	std::string
	HistoryClientConfig::serverUri(void)
	{
		return serverUri_;
	}

	std::vector<std::string>&
	HistoryClientConfig::namespaceUris(void)
	{
		return namespaceUris_;
	}

	ClientSubscriptionConfig::Map&
	HistoryClientConfig::clientSubscriptionMap(void)
	{
		return clientSubscriptionConfigMap_;
	}
}
