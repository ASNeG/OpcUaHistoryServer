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

#ifndef __OpcUaHistory_HistoryClient_h__
#define __OpcUaHistory_HistoryClient_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackClient/ServiceSet/ServiceSetManager.h"
#include "OpcUaHistory/History/HistoryClientConfig.h"
#include "OpcUaHistory/OpcUaClient/ClientConnection.h"

namespace OpcUaHistory
{

	class HistoryClient
	: public ClientSubscriptionIf
	{
	  public:
		typedef boost::shared_ptr<HistoryClient> SPtr;
		typedef std::map<std::string, HistoryClient::SPtr> Map;
		typedef std::set<HistoryClient::SPtr> Set;

		typedef enum {
			S_Open,
			S_Close
		} State;

		HistoryClient(void);
		~HistoryClient(void);

        bool startup(const std::string& fileName, ConfigXmlManager& configXmlManager, IOThread::SPtr ioThread);
        bool shutdown(void);

        // -- ClientSubscriptionIf --------------------------------------------
        virtual void dataChangeNotification(ClientMonitoredItem::SPtr& clientMonitoredItem, OpcUaDataValue& dataValue);
        // -- ClientSubscriptionIf --------------------------------------------

	  private:
        HistoryClientConfig historyClientConfig_;
        ClientConnection clientConnection_;
	};

}

#endif
