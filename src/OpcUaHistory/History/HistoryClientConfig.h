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

#ifndef __OpcUaHistory_HistoryClientConfig_h__
#define __OpcUaHistory_HistoryClientConfig_h__

#include "OpcUaStackCore/Base/ConfigXmlManager.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaNodeId.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class ClientNodeConfig
	{
	  public:
		typedef boost::shared_ptr<ClientNodeConfig> SPtr;
		typedef std::map<std::string, ClientNodeConfig::SPtr> Map;

		typedef enum
		{
			Status,
			StatusValue,
			StatusValueTimestamp
		} DataChangeFilter;

		ClientNodeConfig(void);
		~ClientNodeConfig(void);

		uint32_t samplingInterval(void);
		void samplingInterval(uint32_t samplingInterval);
		uint32_t queueSize(void);
		void queueSize(uint32_t queueSize);
		DataChangeFilter dataChangeFilter(void);
		void dataChangeFilter(DataChangeFilter dataChangeFilter);
		OpcUaNodeId& nodeId(void);
		void nodeId(OpcUaNodeId& nodeId);
		std::string& valueName(void);
		void valueName(const std::string& valueName);

	  private:
		uint32_t samplingInterval_;
		uint32_t queueSize_;
		OpcUaNodeId nodeId_;
		std::string valueName_;
		DataChangeFilter dataChangeFilter_;
	};

	class ClientSubscriptionConfig
	{
	  public:
		typedef boost::shared_ptr<ClientSubscriptionConfig> SPtr;
		typedef std::map<std::string, ClientSubscriptionConfig::SPtr> Map;

		ClientSubscriptionConfig(void);
		~ClientSubscriptionConfig(void);

		std::string id(void);
		void id(const std::string& id);
		uint32_t publishingInterval(void);
		void publisingInterval(uint32_t publishingInterval);
		uint32_t livetimeCount(void);
		void livetimeCount(uint32_t livetimeCount);
		uint32_t maxKeepAliveCount(void);
		void maxKeepAliveCount(uint32_t maxKeepAliveCount);
		uint32_t maxNotificationsPerPublish(void);
		void maxNotificationsPerPublish(uint32_t maxNotificationsPerPublish);
		ClientNodeConfig::Map& clientNodeConfigMap(void);

	  private:
		std::string id_;

		uint32_t publishingInterval_;
		uint32_t livetimeCount_;
		uint32_t maxKeepAliveCount_;
		uint32_t maxNotificationsPerPublish_;

		ClientNodeConfig::Map clientNodeConfigMap_;
	};

	class HistoryClientConfig
	{
	  public:
		HistoryClientConfig(void);
		~HistoryClientConfig(void);

		bool decode(const std::string& configFileName, ConfigXmlManager& configXmlManager);

		std::string serverUri(void);
		uint32_t reconnectTimeout(void);
		std::vector<std::string>& namespaceUris(void);
		ClientSubscriptionConfig::Map& clientSubscriptionMap(void);

	  private:
		bool decodeEndpoint(Config::SPtr& config);
		bool decodeNamespaceUris(Config::SPtr& config);
		bool decodeSubscriptions(Config::SPtr& config);
		bool decodeSubscription(Config& config);
		bool decodeNodeList(Config& config, ClientNodeConfig::Map& clientNodeConfigMap);
		bool decodeNode(Config& config, ClientNodeConfig& clientNodeConfig);

		std::string serverUri_;
		uint32_t reconnectTimeout_;
		std::vector<std::string> namespaceUris_;
		ClientSubscriptionConfig::Map clientSubscriptionConfigMap_;
	};

}

#endif
