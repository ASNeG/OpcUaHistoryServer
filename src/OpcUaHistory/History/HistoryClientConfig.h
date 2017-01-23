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

#ifndef __OpcUaHistory_HistoryClientConfig_h__
#define __OpcUaHistory_HistoryClientConfig_h__

#include "OpcUaStackCore/Base/ConfigXmlManager.h"
#include "OpcUaHistory/History/ConfigBase.h"
#include "OpcUaHistory/OpcUaClient/DataChangeFilter.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class ClientMonitoredItemConfig
	{
	  public:
		typedef boost::shared_ptr<ClientMonitoredItemConfig> SPtr;
		typedef std::map<std::string, ClientMonitoredItemConfig::SPtr> Map;

		ClientMonitoredItemConfig(void);
		~ClientMonitoredItemConfig(void);

		std::string& id(void);
		void id(const std::string& id);
		uint32_t samplingInterval(void);
		void samplingInterval(uint32_t samplingInterval);
		uint32_t queueSize(void);
		void queueSize(uint32_t queueSize);
		DataChangeFilter dataChangeFilter(void);
		void dataChangeFilter(DataChangeFilter dataChangeFilter);

		bool decode(Config& config, ConfigBase& configBase);

	  private:
		std::string id_;
		uint32_t samplingInterval_;
		uint32_t queueSize_;
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
		ClientMonitoredItemConfig::Map& clientNodeConfigMap(void);

		bool decode(Config& config, ConfigBase& configBase);

	  private:
		std::string id_;

		uint32_t publishingInterval_;
		uint32_t livetimeCount_;
		uint32_t maxKeepAliveCount_;
		uint32_t maxNotificationsPerPublish_;

		ClientMonitoredItemConfig::Map clientMonitoredItemConfigMap_;
	};


	class ClientEndpointConfig
	{
	  public:
		ClientEndpointConfig(void);
		~ClientEndpointConfig(void);

		void serverUri(const std::string& serverUri);
		std::string& serverUri(void);
		void reconnectTimeout(uint32_t reconnectTimeout);
		uint32_t reconnectTimeout(void);

		bool decode(Config& config, ConfigBase& configBase);

	  private:
		std::string serverUri_;
		uint32_t reconnectTimeout_;
	};


	class ClientConfig
	{
	  public:
		ClientConfig(void);
		~ClientConfig(void);

		bool decode(const std::string& configFileName, ConfigXmlManager& configXmlManager);

		std::string id(void);
		void id(const std::string& id);
		std::string serverUri(void);
		uint32_t reconnectTimeout(void);
		ClientSubscriptionConfig::Map& clientSubscriptionMap(void);

	  private:
		bool decodeEndpoint(Config::SPtr& config);
		bool decodeSubscriptions(Config::SPtr& config);
		bool decodeSubscription(Config& config);
		bool decodeMonitoredItems(Config& config, ClientMonitoredItemConfig::Map& clientMonitoredItemConfigMap);

		std::string id_;
		std::string serverUri_;
		uint32_t reconnectTimeout_;
		ClientSubscriptionConfig::Map clientSubscriptionConfigMap_;
	};

}

#endif
