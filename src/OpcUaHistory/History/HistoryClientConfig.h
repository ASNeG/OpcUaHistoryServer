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

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class ClientSubscriptionConfig
	{
	  public:
		typedef boost::shared_ptr<ClientSubscriptionConfig> SPtr;
		typedef std::map<std::string, ClientSubscriptionConfig> Map;

		ClientSubscriptionConfig(void);
		~ClientSubscriptionConfig(void);

		std::string id(void);
		void id(const std::string& id);

		uint32_t publishingInterval(void);
		void publisingInterval(uint32_t publishingInterval);
		uint32_t livetimeCount(void);
		void livetimeCount(uint32_t livetimeCount);
		uint32_t maxKeepaliveCount(void);
		void maxKeepaliveCount(uint32_t maxKeepaliveCount);
		uint32_t maxNotificationPerPublish(void);
		void maxNotificationPerPublish(uint32_t maxNotificationPerPublish);

	  private:
		std::string id_;

		uint32_t publishingInterval_;
		uint32_t livetimeCount_;
		uint32_t maxKeepaliveCount_;
		uint32_t maxNotificationPerPublish_;
	};

	class HistoryClientConfig
	{
	  public:
		HistoryClientConfig(void);
		~HistoryClientConfig(void);

		bool decode(const std::string& configFileName, ConfigXmlManager& configXmlManager);
		bool decodeEndpoint(Config::SPtr& config);
		bool decodeNamespaceUris(Config::SPtr& config);
		bool decodeSubscription(Config::SPtr& subscription);

		std::string serverUri(void);

	  private:
		std::string serverUri_;
		std::vector<std::string> namespaceUris_;
	};

}

#endif
