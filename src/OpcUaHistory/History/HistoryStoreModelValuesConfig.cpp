/*
   Copyright 2017 Kai Huebl (kai@huebl-sgh.de)

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

#include <boost/algorithm/string.hpp>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/ObjectPool.h"
#include "OpcUaHistory/History/HistoryStoreModelValuesConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// HistoryStoreModelValueConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	HistoryStoreModelValueConfig::HistoryStoreModelValueConfig(void)
	: configFileName_("")
	, elementPrefix_("")
	, name_("")
	, serverVec_()
	, clientVec_()
	{
	}

	void
	HistoryStoreModelValueConfig::configFileName(const std::string& configFileName)
	{
		configFileName_ = configFileName;
	}

	void
	HistoryStoreModelValueConfig::elementPrefix(const std::string& elementPrefix)
	{
		elementPrefix_ = elementPrefix;
	}

	std::string&
	HistoryStoreModelValueConfig::name(void)
	{
		return name_;
	}

	OpcUaReferenceConfig::Vec&
	HistoryStoreModelValueConfig::serverVec(void)
	{
		return serverVec_;
	}

	OpcUaReferenceConfig::Vec&
	HistoryStoreModelValueConfig::clientVec(void)
	{
		return clientVec_;
	}

	HistoryStoreModelValueConfig::~HistoryStoreModelValueConfig(void)
	{
	}

	bool
	HistoryStoreModelValueConfig::decode(Config& config)
	{
		std::vector<Config>::iterator it;

		// get name element
		if (!config.getConfigParameter("<xmlattr>.name", name_)) {
			Log(Error, "attribute missing in config file")
				.parameter("Element", "HistoryStoreModel.Values.Value")
				.parameter("Attribute", "Name")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		// get client elements
		std::vector<Config> clients;
		config.getChilds("Client", clients);
		if (clients.size() == 0) {
			Log(Error, "element missing in config file")
				.parameter("Element", "HistoryStoreModel.Values.Value.Client")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		for (it = clients.begin(); it != clients.end(); it++) {

			OpcUaReferenceConfig::SPtr client = constructSPtr<OpcUaReferenceConfig>();
			client->configFileName(configFileName_);
			client->elementPrefix("HistoryStoreModel.Values.Value.Client");

			if (!client->decode(*it))

			clientVec_.push_back(client);
		}

		// get server elements
		std::vector<Config> servers;
		config.getChilds("Server", servers);
		if (servers.size() == 0) {
			Log(Error, "element missing in config file")
				.parameter("Element", "HistoryStoreModel.Values.Value.Server")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		for (it = servers.begin(); it != servers.end(); it++) {

			OpcUaReferenceConfig::SPtr server = constructSPtr<OpcUaReferenceConfig>();
			server->configFileName(configFileName_);
			server->elementPrefix("HistoryStoreModel.Values.Value.Client");

			if (!server->decode(*it))

			serverVec_.push_back(server);
		}

		return true;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// HistoryStoreModelValuesConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	HistoryStoreModelValuesConfig::HistoryStoreModelValuesConfig(void)
	: configFileName_("")
	, elementPrefix_("")
	, namespaceUris_()
	, namespaceTypes_()
	, valueVec_()
	{
	}

	HistoryStoreModelValuesConfig::~HistoryStoreModelValuesConfig(void)
	{
	}

	void
	HistoryStoreModelValuesConfig::configFileName(const std::string& configFileName)
	{
		configFileName_ = configFileName;
	}

	void
	HistoryStoreModelValuesConfig::elementPrefix(const std::string& elementPrefix)
	{
		elementPrefix_ = elementPrefix;
	}

	HistoryStoreModelValuesConfig::NamespaceUris&
	HistoryStoreModelValuesConfig::namespaceUris(void)
	{
		return namespaceUris_;
	}

	HistoryStoreModelValuesConfig::NamespaceTypes&
	HistoryStoreModelValuesConfig::namespaceTypes(void)
	{
		return namespaceTypes_;
	}

	HistoryStoreModelValueConfig::Vec&
	HistoryStoreModelValuesConfig::valueVec(void)
	{
		return valueVec_;
	}

	bool
	HistoryStoreModelValuesConfig::decode(Config& config)
	{
		// get Values element
		boost::optional<Config> valuesConfig = config.getChild("Values");
		if (!valuesConfig) {
			Log(Error, "element missing in config file")
				.parameter("Element", "HistoryStoreModel.Values")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		// decode NamespaceUris element
		if (!decodeNamespaceUris(*valuesConfig)) {
			return false;
		}

		// decode Values element
		if (!decodeValues(*valuesConfig)) {
			return false;
		}

		return true;
	}

	bool
	HistoryStoreModelValuesConfig::decodeNamespaceUris(Config& config)
	{
		// get NamespaceUris element
		boost::optional<Config> namespaceUris = config.getChild("NamespaceUris");
		if (!namespaceUris) {
			Log(Error, "element missing in config file")
				.parameter("Element", "HistoryStoreModel.Values.NamespaceUris")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		// get Uri elements
		config.getValues("Uri", namespaceUris_);
		if (namespaceUris_.size() == 0) {
			Log(Error, "element missing in config file")
				.parameter("Element", "HistoryStoreModel.Values.NamespaceUris.Uri")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		// create namespace types
		for (uint32_t idx=0; idx<namespaceUris_.size(); idx++) {
			namespaceTypes_.push_back(None);
		}

		return true;
	}

	bool
	HistoryStoreModelValuesConfig::decodeValues(Config& config)
	{
		// get Value elements
		std::vector<Config> childs;
		config.getChilds("Value", childs);
		if (childs.size() == 0) {
			Log(Error, "element missing in config file")
				.parameter("Element", "HistoryStoreModel.Values.Value");
			return false;
		}

		std::vector<Config>::iterator it;
		for (it = childs.begin(); it != childs.end(); it++) {

			// get value element
			HistoryStoreModelValueConfig::SPtr value = constructSPtr<HistoryStoreModelValueConfig>();
			value->configFileName(configFileName_);
			value->configFileName(elementPrefix_ + ".Value");
			if (!value->decode(*it)) {
				return false;
			}
			valueVec_.push_back(value);

			// check namespace index from NodeId element
			if (!namespaceCheck(value)) {
				return false;
			}
		}

		return true;
	}

	bool
	HistoryStoreModelValuesConfig::namespaceCheck(HistoryStoreModelValueConfig::SPtr& value)
	{
		OpcUaReferenceConfig::Vec::iterator it;

		// check server namespace index
		for (it = value->serverVec().begin(); it != value->serverVec().end(); it++) {
			OpcUaReferenceConfig::SPtr ref = *it;
			uint32_t namespaceIndex = ref->nodeId().namespaceIndex();

			if (namespaceIndex > namespaceUris_.size()) {
				Log(Error, "invalid namespace index in config file")
					.parameter("Element", "HistoryStoreModel.Values.Value.NodeId")
					.parameter("NodeId", ref->nodeId().toString())
					.parameter("ConfigFileName", configFileName_);
				return false;
			}

			if (namespaceTypes_[namespaceIndex-1] == None) {
				namespaceTypes_[namespaceIndex-1] = Server;
			}
			else if (namespaceTypes_[namespaceIndex-1] == Client) {
				namespaceTypes_[namespaceIndex-1] = ClientServer;
			}
		}

		// check client namespace index
		for (it = value->clientVec().begin(); it != value->clientVec().end(); it++) {
			OpcUaReferenceConfig::SPtr ref = *it;
			uint32_t namespaceIndex = ref->nodeId().namespaceIndex();

			if (namespaceIndex > namespaceUris_.size()) {
				Log(Error, "invalid namespace index in config file")
					.parameter("Element", "HistoryStoreModel.Values.Value.NodeId")
					.parameter("NodeId", ref->nodeId().toString())
					.parameter("ConfigFileName", configFileName_);
				return false;
			}

			if (namespaceTypes_[namespaceIndex-1] == None) {
				namespaceTypes_[namespaceIndex-1] = Client;
			}
			else if (namespaceTypes_[namespaceIndex-1] == Server) {
				namespaceTypes_[namespaceIndex-1] = ClientServer;
			}
		}

		return true;
	}

}
