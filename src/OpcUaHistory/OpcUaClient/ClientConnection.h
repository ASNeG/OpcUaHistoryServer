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
#include <map>
#include <set>

namespace OpcUaHistory
{

	class ClientConnection
	{
	  public:
		typedef boost::shared_ptr<ClientConnection> SPtr;
		typedef std::map<std::string, ClientConnection::SPtr> Map;
		typedef std::set<ClientConnection::SPtr> Set;

		typedef enum {
			S_Connecting,
			S_Connected,
			S_Established,
			S_Disconnecting,
			S_Disconnected
		} State;

		ClientConnection(void);
		~ClientConnection(void);

		void state(State state);
		State state(void);

	  private:
		State state_;
	};

}

#endif
