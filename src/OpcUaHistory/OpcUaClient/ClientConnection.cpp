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

#include "OpcUaHistory/OpcUaClient/ClientConnection.h"

namespace OpcUaHistory
{

	ClientConnection::ClientConnection(void)
	{
	}

	ClientConnection::~ClientConnection(void)
	{
	}

	void
	ClientConnection::serverUri(const std::string& serverUri)
	{
		serverUri_ = serverUri;
	}

	std::string
	ClientConnection::serverUri(void)
	{
		return serverUri_;
	}

	void
	ClientConnection::reconnectTimeout(uint32_t reconnectTimeout)
	{
		reconnectTimeout_ = reconnectTimeout;
	}

	uint32_t
	ClientConnection::reconnectTimeout(void)
	{
		return reconnectTimeout_;
	}

	void
	ClientConnection::state(State state)
	{
		state_ = state;
	}

	ClientConnection::State
	ClientConnection::state(void)
	{
		return state_;
	}

	ClientConnection::NamespaceUris&
	ClientConnection::namespaceUris(void)
	{
		return namespaceUris_;
	}

}
