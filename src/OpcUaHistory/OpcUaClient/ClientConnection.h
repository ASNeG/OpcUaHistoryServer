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

#ifndef __OpcUaHistory_ClientConnection_h__
#define __OpcUaHistory_ClientConnection_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackClient/ServiceSet/ServiceSetManager.h"
#include "OpcUaHistory/OpcUaClient/ClientSubscription.h"
#include <map>
#include <set>
#include <vector>
#include <stdint.h>

using namespace OpcUaStackCore;
using namespace OpcUaStackClient;

namespace OpcUaHistory
{

	class ClientConnection
	: public SessionServiceIf
	, public AttributeServiceIf
	{
	  public:
		typedef boost::shared_ptr<ClientConnection> SPtr;
		typedef std::map<std::string, ClientConnection::SPtr> Map;
		typedef std::set<ClientConnection::SPtr> Set;
		typedef std::vector<std::string> NamespaceUris;
		typedef std::map<uint32_t, uint32_t> NamespaceMap;

		typedef enum {
			S_Error,
			S_Connecting,
			S_Connected,
			S_Established,
			S_Disconnecting,
			S_Disconnected
		} State;

		ClientConnection(void);
		~ClientConnection(void);

		void serverUri(const std::string& serverUri);
		std::string serverUri(void);
		void reconnectTimeout(uint32_t reconnectTimeout);
		uint32_t reconnectTimeout(void);
		bool clientSubscription(const std::string& id, ClientSubscription::SPtr& clientSubscription);
		void ioThread(IOThread::SPtr& ioThread);
		IOThread::SPtr& ioThread(void);

		void state(State state);
		State state(void);
		NamespaceUris& namespaceUris(void);

		void startup(void);
		bool connect(void);
		bool disconnect(void);

		//- SessionServiceIf --------------------------------------------------
		virtual void sessionStateUpdate(SessionBase& session, SessionState sessionState);
		//- SessionServiceIf --------------------------------------------------

		//- AttributeServiceIf ------------------------------------------------
		virtual void attributeServiceReadResponse(ServiceTransactionRead::SPtr serviceTransactionRead);
		virtual void attributeServiceWriteResponse(ServiceTransactionWrite::SPtr serviceTransactionWrite);
		virtual void attributeServiceHistoryReadResponse(ServiceTransactionHistoryRead::SPtr serviceTransactionHistoryRead);
		virtual void attributeServiceHistoryUpdateResponse(ServiceTransactionHistoryUpdate::SPtr serviceTransactionHistoryUpdate);
		//- AttributeServuceIf ------------------------------------------------


	  private:
		// configuration parameters
		std::string serverUri_;
		uint32_t reconnectTimeout_;
		IOThread::SPtr ioThread_;

		// runtime parameters
		State state_;
		NamespaceUris namespaceUris_;

		SlotTimerElement::SPtr slotTimerElement_;
		ServiceSetManager serviceSetManager_;
		SessionService::SPtr sessionService_;
		AttributeService::SPtr attributeService_;

		NamespaceMap namespaceMap_;
		void timerLoop(void);
	    void readNamespaceArray(void);
	    void handleNamespaceArray(ServiceTransactionRead::SPtr serviceTransactionRead);
	    void handleConnect(void);
	    void handleDisconnect(void);

	    ClientSubscription::Map clientSubscriptionMap_;
	};

}

#endif
