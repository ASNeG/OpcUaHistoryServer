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
#include "OpcUaHistory/History/HistoryServerConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class ServerNodeConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ServerNodeConfig::ServerNodeConfig(void)
	: nodeId_()
	, valueName_("")
	{
	}

	ServerNodeConfig::~ServerNodeConfig(void)
	{
	}

	OpcUaNodeId&
	ServerNodeConfig::nodeId(void)
	{
		return nodeId_;
	}

	void
	ServerNodeConfig::nodeId(OpcUaNodeId& nodeId)
	{
		nodeId_ = nodeId;
	}

	std::string&
	ServerNodeConfig::valueName(void)
	{
		return valueName_;
	}

	void
	ServerNodeConfig::valueName(const std::string& valueName)
	{
		valueName_ = valueName;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class HistoryServerConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	HistoryServerConfig::HistoryServerConfig(void)
	: namespaceUris_()
	, serverNodeConfigMap_()
	{
	}

	HistoryServerConfig::~HistoryServerConfig(void)
	{
	}

	bool
	HistoryServerConfig::decode(const std::string& configFileName, ConfigXmlManager& configXmlManager)
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
		// decode NamespaceUris configuration
		//
		if (!decodeNamespaceUris(config)) {
			Log(Error, "read configuration file error")
				.parameter("ConfigFile", configFileName);
			return false;
		}

		//
		// decode nodes
		//
		std::vector<Config> childs;
		config->getChilds("OpcUaServerModel.Node", childs);
		if (childs.size() == 0) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaServerModel.Node");
			return false;
		}

		std::vector<Config>::iterator it;
		for (it = childs.begin(); it != childs.end(); it++) {
			ServerNodeConfig::SPtr node = constructSPtr<ServerNodeConfig>();

			if (!decodeNode(*it, *node.get())) return false;

			ServerNodeConfig::Map::iterator it;
			it = serverNodeConfigMap_.find(node->valueName());
			if (it != serverNodeConfigMap_.end()) {
				Log(Error, "dublicate node name in config file")
					.parameter("Parameter", "OpcUaServerModel.Node.ValueName")
					.parameter("ValueName", node->valueName());
				return false;
			}
			serverNodeConfigMap_.insert(std::make_pair(node->valueName(), node));
		}

		return true;
	}

	bool
	HistoryServerConfig::decodeNamespaceUris(Config::SPtr& config)
	{
		bool success;

		// read namespace uris
		config->getValues("OpcUaServerModel.NamespaceUris.Uri", namespaceUris_);
		if (namespaceUris_.size() == 0) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "OpcUaServerModel.NamespaceUris.Uri");
			return false;
		}

		return true;
	}

	bool
	HistoryServerConfig::decodeNode(Config& config, ServerNodeConfig& serverNodeConfig)
	{
		// node id
		std::string nodeIdString;
		if (!config.getConfigParameter("<xmlattr>.NodeId", nodeIdString)) {
			Log(Error, "attribute missing in config file")
				.parameter("Parameter", "OpcUaServerModel.Node")
				.parameter("Attribute", "NodeId");
			return false;
		}
		OpcUaNodeId nodeId;
		if (!nodeId.fromString(nodeIdString)) {
			Log(Error, "attribute invalid in config file")
				.parameter("Parameter", "OpcUaServerModel.Node")
				.parameter("Attribute", "NodeId")
				.parameter("NodeId", nodeIdString);
			return false;
		}
		serverNodeConfig.nodeId(nodeId);

		// value name
		std::string valueName;
		if (!config.getConfigParameter("<xmlattr>.ValueName", valueName)) {
			Log(Error, "attribute missing in config file")
				.parameter("Parameter", "OpcUaServerMode..Node")
				.parameter("Attribute", "ValueName");
			return false;
		}
		serverNodeConfig.valueName(valueName);

		return true;
	}


	std::vector<std::string>&
	HistoryServerConfig::namespaceUris(void)
	{
		return namespaceUris_;
	}

	ServerNodeConfig::Map&
	HistoryServerConfig::serverNodeConfigMap(void)
	{
		return serverNodeConfigMap_;
	}

}
