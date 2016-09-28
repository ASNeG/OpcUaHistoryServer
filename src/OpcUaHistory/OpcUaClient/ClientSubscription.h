/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)

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

#ifndef __OpcUaHistory_ClientSubscription_h__
#define __OpcUaHistory_ClientSubscription_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackClient/ServiceSet/ServiceSetManager.h"
#include <map>
#include <set>
#include <vector>
#include <stdint.h>

using namespace OpcUaStackCore;
using namespace OpcUaStackClient;

namespace OpcUaHistory
{

	class ClientSubscription
	: public SubscriptionServiceIf
	{
	  public:
		typedef boost::shared_ptr<ClientSubscription> SPtr;
		typedef std::map<std::string, ClientSubscription::SPtr> Map;
		typedef std::set<ClientSubscription::SPtr> Set;

		typedef enum {
			S_Error,
			S_Close,
			S_Opening,
			S_Open,
			S_Closing,
		} State;

		ClientSubscription(void);
		~ClientSubscription(void);

		void ioThread(IOThread::SPtr& ioThread);
		IOThread::SPtr& ioThread(void);
		std::string id(void);
		void id(const std::string& id);
		uint32_t publishingInterval(void);
		void publishingInterval(uint32_t publishingInterval);
		uint32_t livetimeCount(void);
		void livetimeCount(uint32_t livetimeCount);
		uint32_t maxKeepAliveCount(void);
		void maxKeepAliveCount(uint32_t maxKeepAliveCount);
		uint32_t maxNotificationsPerPublish(void);
		void maxNotificationsPerPublish(uint32_t maxNotificationsPerPublish);


		void state(State state);
		State state(void);

		void startup(ServiceSetManager& serviceSetManager, SessionService::SPtr& sessionService);
		void open(void);
		void close(void);

		//- SubscriptionServiceIf ---------------------------------------------
	    virtual void subscriptionServiceCreateSubscriptionResponse(ServiceTransactionCreateSubscription::SPtr serviceTransactionCreateSubscription);
	    virtual void subscriptionServiceModifySubscriptionResponse(ServiceTransactionModifySubscription::SPtr serviceTransactionModifySubscription);
	    virtual void subscriptionServiceTransferSubscriptionsResponse(ServiceTransactionTransferSubscriptions::SPtr serviceTransactionTransferSubscriptions);
	    virtual void subscriptionServiceDeleteSubscriptionsResponse(ServiceTransactionDeleteSubscriptions::SPtr serviceTransactionDeleteSubscriptions);

		virtual void dataChangeNotification(const MonitoredItemNotification::SPtr& monitoredItem);
		virtual void subscriptionStateUpdate(SubscriptionState subscriptionState, uint32_t subscriptionId);
		//- SubscriptionServiceIf ---------------------------------------------


	  private:

		// configuration parameters
		IOThread::SPtr ioThread_;
		std::string id_;
		uint32_t publishingInterval_;
		uint32_t livetimeCount_;
		uint32_t maxKeepAliveCount_;
		uint32_t maxNotificationsPerPublish_;

		// runtime parameters
		bool active_;
		State state_;
		uint32_t subscriptionId_;

		SubscriptionService::SPtr subscriptionService_;
	};

}

#endif
