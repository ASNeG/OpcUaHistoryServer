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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/OpcUaClient/ClientConnection.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackClient;

namespace OpcUaHistory
{

	ClientConnection::ClientConnection(void)
	: serverUri_("opc.tcp://127.0.0.1:4841")
	, reconnectTimeout_(5000)

	, state_(S_Disconnected)
	, namespaceUris_()

	, ioThread_()
	, slotTimerElement_()
	, serviceSetManager_()
	, sessionService_()
	, attributeService_()

	, namespaceMap_()
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
	ClientConnection::ioThread(IOThread::SPtr& ioThread)
	{
		ioThread_ = ioThread;
	}

	IOThread::SPtr&
	ClientConnection::ioThread(void)
	{
		return ioThread_;
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

	bool
	ClientConnection::connect(void)
	{
		// init service sets
		serviceSetManager_.registerIOThread("GlobalIOThread", ioThread_);

		// create session service
		SessionServiceConfig sessionServiceConfig;
		sessionServiceConfig.ioThreadName("GlobalIOThread");
		sessionServiceConfig.sessionServiceIf_ = this;
		sessionServiceConfig.secureChannelClient_->endpointUrl(serverUri_);
		sessionServiceConfig.session_->sessionName("ASNeGHistoryServer");
		sessionServiceConfig.session_->reconnectTimeout(reconnectTimeout_);
		serviceSetManager_.sessionService(sessionServiceConfig);
		sessionService_ = serviceSetManager_.sessionService(sessionServiceConfig);

		// create attribute service
		AttributeServiceConfig attributeServiceConfig;
		attributeServiceConfig.ioThreadName("GlobalIOThread");
		attributeServiceConfig.attributeServiceIf_ = this;
		attributeService_ = serviceSetManager_.attributeService(sessionService_, attributeServiceConfig);

		// create session
		state_ = S_Connecting;
		sessionService_->asyncConnect();
		return true;
	}

	bool
	ClientConnection::disconnect(void)
	{
		// disconnect session
		state_ = S_Disconnected;
		sessionService_->asyncDisconnect(true);
		return true;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// SessionIf
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void
	ClientConnection::sessionStateUpdate(SessionBase& session, SessionState sessionState)
	{
		switch (sessionState)
		{
			case SS_Connect:
				Log(Debug, "session state changed to connect")
				    .parameter("ServerUri", serverUri_);
				state_ = S_Connected;
				readNamespaceArray();
				break;
			case SS_Disconnect:
				Log(Debug, "session state changed to disconnect/close")
					.parameter("ServerUri", serverUri_);
				state_ = S_Disconnected;
				//closeSubscriptions();
				break;
			case SS_Reactivate:
				Log(Debug, "session state changed to reactivate")
					.parameter("ServerUri", serverUri_);
				break;
		}
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// AttributeServiceIf
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void
	ClientConnection::attributeServiceReadResponse(ServiceTransactionRead::SPtr serviceTransactionRead)
	{
		handleNamespaceArray(serviceTransactionRead);
	}

	void
	ClientConnection::attributeServiceWriteResponse(ServiceTransactionWrite::SPtr serviceTransactionWrite)
	{
		// nothing to do
	}

	void
	ClientConnection::attributeServiceHistoryReadResponse(ServiceTransactionHistoryRead::SPtr serviceTransactionHistoryRead)
	{
		// nothing to do
	}

	void
	ClientConnection::attributeServiceHistoryUpdateResponse(ServiceTransactionHistoryUpdate::SPtr serviceTransactionHistoryUpdate)
	{
		// nothing to do
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// handle namespace array
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void
	ClientConnection::readNamespaceArray(void)
	{
		ServiceTransactionRead::SPtr readTrx = constructSPtr<ServiceTransactionRead>();
		ReadRequest::SPtr req = readTrx->request();
		req->maxAge(0);
		req->timestampsToReturn(2);

		ReadValueId::SPtr readValueId = ReadValueId::construct();
		readValueId->nodeId((OpcUaInt16)0, (OpcUaInt32)2255);
		readValueId->attributeId((OpcUaInt32) 13);
		readValueId->dataEncoding().namespaceIndex((OpcUaInt16) 0);
		req->readValueIdArray()->set(readValueId);

		attributeService_->asyncSend(readTrx);
	}

	void
	ClientConnection::handleNamespaceArray(ServiceTransactionRead::SPtr serviceTransactionRead)
	{
		ReadRequest::SPtr req = serviceTransactionRead->request();
		ReadResponse::SPtr res = serviceTransactionRead->response();

		// error handling
		OpcUaStatusCode statusCode = serviceTransactionRead->statusCode();
		if (statusCode != Success) {
			Log(Error, "read namespace array transaction error")
				.parameter("ServerUri", serverUri_)
				.parameter("StatusCode", OpcUaStatusCodeMap::shortString(statusCode));
			state_ = S_Disconnected;
			return;
		}
		if (res->dataValueArray()->size() != 1) {
			Log(Error, "read namespace array response error, because data array size error")
				.parameter("ServerUri", serverUri_);
			state_ = S_Error;
			return;
		}

		// get data value
		OpcUaDataValue::SPtr dataValue;
		res->dataValueArray()->get(0, dataValue);

		if (dataValue->statusCode() != Success) {
			Log(Error, "read namespace array data value error")
				.parameter("ServerUri", serverUri_)
				.parameter("StatusCode", OpcUaStatusCodeMap::shortString(statusCode));
			state_ = S_Error;
			return;
		}
		OpcUaVariant::SPtr variant = dataValue->variant();

		// get namespace array
		std::map<std::string, uint32_t> namespaceMap;
		std::map<std::string, uint32_t>::iterator it;
		Log(Debug, "read namespace array from server")
		    .parameter("ServerUri", serverUri_);
		for (int32_t idx=0; idx < variant->arrayLength(); idx++) {
			std::string uri = variant->variant()[idx].variantSPtr<OpcUaString>()->value();
			Log(Debug, "")
				.parameter("Uri", uri)
				.parameter("NamespaceIndex", idx);
			namespaceMap.insert(std::make_pair(uri, idx));
		}

		// create namespace mapping
		namespaceMap_.clear();
		for (uint32_t idx=0; idx<namespaceUris_.size(); idx++) {
			uint32_t namespaceIndex = idx+1;
			std::string namespaceName = namespaceUris_[idx];

			it = namespaceMap.find(namespaceName);
			if (it == namespaceMap.end()) {
				Log(Error, "namespace name not exist on server")
				    .parameter("ServerUri", serverUri_)
				    .parameter("NamespaceName", namespaceName);
				state_ = S_Error;
				return;
			}

			namespaceMap_.insert(std::make_pair(namespaceIndex, it->second));
		}

		state_ = S_Established;
	}

}
