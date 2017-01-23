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
	ClientMonitoredItemConfig::ClientMonitoredItemConfig(void)
	: id_("")
	, samplingInterval_(0)
	, queueSize_(0)
	, dataChangeFilter_(StatusValue)
	{
	}

	ClientMonitoredItemConfig::~ClientMonitoredItemConfig(void)
	{
	}

	std::string
	ClientMonitoredItemConfig::id(void)
	{
		return id_;
	}

	void
	ClientMonitoredItemConfig::id(const std::string& id)
	{
		id_ = id;
	}

	uint32_t
	ClientMonitoredItemConfig::samplingInterval(void)
	{
		return samplingInterval_;
	}

	void
	ClientMonitoredItemConfig::samplingInterval(uint32_t samplingInterval)
	{
		samplingInterval_ = samplingInterval;
	}

	uint32_t
	ClientMonitoredItemConfig::queueSize(void)
	{
		return queueSize_;
	}

	void
	ClientMonitoredItemConfig::queueSize(uint32_t queueSize)
	{
		queueSize_ = queueSize;
	}

	DataChangeFilter
	ClientMonitoredItemConfig::dataChangeFilter(void)
	{
		return dataChangeFilter_;
	}

	void
	ClientMonitoredItemConfig::dataChangeFilter(DataChangeFilter dataChangeFilter)
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

	ClientMonitoredItemConfig::Map&
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
	: id_("")
	, serverUri_("")
	, reconnectTimeout_(5000)
	, clientSubscriptionConfigMap_()
	{
	}

	HistoryClientConfig::~HistoryClientConfig(void)
	{
	}

	std::string
	HistoryClientConfig::id(void)
	{
		return id_;
	}

	void
	HistoryClientConfig::id(const std::string& id)
	{
		id_ = id;
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

		// client id
		if (!config->getConfigParameter("OpcUaClientModel.<xmlattr>.Id", id_)) {
			Log(Error, "attribute missing in config file")
				.parameter("Parameter", "OpcUaClientMode")
				.parameter("Attribute", "Id");
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
				.parameter("Parameter", "OpcUaClientModel.Endpoint.ServerUri");
			return false;
		}

		// sampling interval
		success = child->getConfigParameter("ReconnectTimeout", reconnectTimeout_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaClientModel.Endpoint.ReconnectTimeout");
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

		// monitored items
		std::vector<Config> childs;
		config.getChilds("MonitoredItem", childs);
		if (childs.size() == 0) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.NodeList");
			return false;
		}

		std::vector<Config>::iterator it;
		for (it = childs.begin(); it != childs.end(); it++) {
			if (!decodeMonitoredItems(*it, subscription->clientNodeConfigMap())) return false;
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
	HistoryClientConfig::decodeMonitoredItems(Config& config, ClientMonitoredItemConfig::Map& clientMonitoredItemConfigMap)
	{
		ClientMonitoredItemConfig::SPtr monitoredItem = constructSPtr<ClientMonitoredItemConfig>();

		// node list id
		std::string id;
		if (!config.getConfigParameter("<xmlattr>.Id", id)) {
			Log(Error, "attribute missing in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.MonitoredItem")
				.parameter("Attribute", "Id");
			return false;
		}
		monitoredItem->id(id);

		// sampling interval
		uint32_t samplingInterval;
		if (!config.getConfigParameter("SamplingInterval", samplingInterval)) {
			Log(Error, "parameter missing or invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.MonitoredItem.SamplingInterval")
				.parameter("MonitoredItemId", id);
			return false;
		}
		monitoredItem->samplingInterval(samplingInterval);

		// queue size
		uint32_t queueSize;
		if (!config.getConfigParameter("QueueSize", queueSize)) {
			Log(Error, "parameter missing or invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.MonitoredItem.QueueSize")
				.parameter("MonitoredItemId", id);
			return false;
		}
		monitoredItem->queueSize(queueSize);

		// data change filter
		std::string dataChangeFilter;
		DataChangeFilter dataChangeFilterType;
		if (!config.getConfigParameter("DataChangeFilter", dataChangeFilter)) {
			Log(Error, "parameter missing or invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.MonitoredItem.DataChangeFilter")
				.parameter("MonitoredItemId", id);
			return false;
		}
		if (dataChangeFilter == "status") {
			dataChangeFilterType = Status;
		}
		else if (dataChangeFilter == "status-value") {
			dataChangeFilterType = StatusValue;
		}
		else if (dataChangeFilter == "status-value-timestamp") {
			dataChangeFilterType = StatusValueTimestamp;
		}
		else {
			Log(Error, "parameter invalid in config file")
				.parameter("Parameter", "OpcUaClientMode.Subscription.MonitoredItem.DataChangeFilter")
				.parameter("MonitoredItemId", id)
				.parameter("DataChangeFilter", dataChangeFilter);
			return false;
		}
		monitoredItem->dataChangeFilter(dataChangeFilterType);

		clientMonitoredItemConfigMap.insert(std::make_pair(id, monitoredItem));
		return true;
	}

	std::string
	HistoryClientConfig::serverUri(void)
	{
		return serverUri_;
	}

	uint32_t
	HistoryClientConfig::reconnectTimeout(void)
	{
		return reconnectTimeout_;
	}

	ClientSubscriptionConfig::Map&
	HistoryClientConfig::clientSubscriptionMap(void)
	{
		return clientSubscriptionConfigMap_;
	}
}
