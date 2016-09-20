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
	// class ClientSubscriptionConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ClientSubscriptionConfig::ClientSubscriptionConfig(void)
	{
	}

	ClientSubscriptionConfig::~ClientSubscriptionConfig(void)
	{
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
		if (!decodeSubscription(config)) {
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
	HistoryClientConfig::decodeSubscription(Config::SPtr& subscription)
	{
		std::cout <<  "FIXME: todo" << std::endl;
		return true;
	}


#if 0
		<Subscription id="subscription1">
			<PublishingInterval>0</PublishingInterval>
			<LifetimeCount>0</LifetimeCount>
			<MaxKeepAliveCount>0</MaxKeepAliveCount>
			<MaxNotificationsPerPublish>0</MaxNotificationsPerPublish>

			<NodeList id="nodelist1">
				<SamplingInterval>0</SamplingInterval>
			  	<QueueSize>1</QueueSize>
			  	<DataChangeFilter>status-value-timestamp</DataChangeFilter>

			  	<Node NodeId="ns=1;s=aaa" ValueName="Value1"/>
			  	<Node NodeId="ns=1;s=bbb" ValueName="Value2"/>

			</NodeList>
#endif

	std::string
	HistoryClientConfig::serverUri(void)
	{
		return serverUri_;
	}
}
