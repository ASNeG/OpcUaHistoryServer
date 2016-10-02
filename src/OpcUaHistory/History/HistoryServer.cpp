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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/HistoryServer.h"
#include "OpcUaStackServer/ServiceSetApplication/ApplicationService.h"

using namespace OpcUaStackCore;
using namespace OpcUaStackServer;

namespace OpcUaHistory
{

	HistoryServer::HistoryServer(void)
	: historyServerConfig_()
	, historyStoreIf_(nullptr)
	, applicationServiceIf_(nullptr)
	, namespaceMap_()
	, hReadCallback_(boost::bind(&HistoryServer::hReadValue, this, _1))
	{
	}

	HistoryServer::~HistoryServer(void)
	{
	}

	void
	HistoryServer::historyStoreIf(HistoryStoreIf* historyStoreIf)
	{
		historyStoreIf_ = historyStoreIf;
	}

	void
	HistoryServer::applicationServiceIf(ApplicationServiceIf* applicationServiceIf)
	{
		applicationServiceIf_ = applicationServiceIf;
	}

    bool
    HistoryServer::startup(std::string& fileName, ConfigXmlManager& configXmlManager)
    {
    	// parse server configuration file
    	if (!historyServerConfig_.decode(fileName, configXmlManager)) {
    		return false;
    	}

		// read namespace info from server service
		if (!getNamespaceInfo()) {
			return false;
		}

		// register history read callback functions
    	if (!registerCallbacks()) {
    		return false;
    	}

    	return true;
    }

    bool
    HistoryServer::shutdown(void)
    {
    	return true;
    }

	bool
	HistoryServer::getNamespaceInfo(void)
	{
		// read namespace array
		ServiceTransactionNamespaceInfo::SPtr trx = ServiceTransactionNamespaceInfo::construct();
		NamespaceInfoRequest::SPtr req = trx->request();
		NamespaceInfoResponse::SPtr res = trx->response();

		applicationServiceIf_->sendSync(trx);
		if (trx->statusCode() != Success) {
			std::cout << "NamespaceInfoResponse error" << std::endl;
			return false;
		}

		// create namespace mapping table // historyServerConfig_
		namespaceMap_.clear();
		for (uint32_t idx=0; idx<historyServerConfig_.namespaceUris().size(); idx++) {
			uint32_t namespaceIndex = idx+1;
			std::string namespaceName = historyServerConfig_.namespaceUris()[idx];

			bool found = false;
			NamespaceInfoResponse::Index2NamespaceMap::iterator it;
			for (it = res->index2NamespaceMap().begin();
				 it != res->index2NamespaceMap().end();
				 it++)
			{
				if (it->second == namespaceName) {
					found = true;
					namespaceMap_.insert(std::make_pair(namespaceIndex, it->first));
					break;
				}
			}

			if (!found) {
				Log(Error, "namespace name not exist on own server")
				    .parameter("NamespaceName", namespaceName);
				return false;
			}

		}

		return true;
	}

    bool
    HistoryServer::registerCallbacks(void)
    {
	  	ServiceTransactionRegisterForward::SPtr trx = ServiceTransactionRegisterForward::construct();
	  	RegisterForwardRequest::SPtr req = trx->request();
	  	RegisterForwardResponse::SPtr res = trx->response();

	  	req->forwardInfoSync()->setReadHCallback(hReadCallback_);
	  	req->nodesToRegister()->resize(historyServerConfig_.serverNodeConfigMap().size());

	  	uint32_t pos = 0;
	  	ServerNodeConfig::Map::iterator it;
	  	for (it = historyServerConfig_.serverNodeConfigMap().begin(); it != historyServerConfig_.serverNodeConfigMap().end(); it++) {
	  		ServerNodeConfig::SPtr snc = it->second;

	  		OpcUaNodeId::SPtr nodeId = constructSPtr<OpcUaNodeId>();
	  		*nodeId = snc->nodeId();

	  		req->nodesToRegister()->set(pos, nodeId);
	  		pos++;
	  	}

	  	applicationServiceIf_->sendSync(trx);
	  	if (trx->statusCode() != Success) {
	  		Log(Error, "register forward response error")
	  		    .parameter("StatusCode", OpcUaStatusCodeMap::shortString(trx->statusCode()));
	  		return false;
	  	}

	  	for (pos = 0; pos < res->statusCodeArray()->size(); pos++) {
	  		OpcUaStatusCode statusCode;
	  		res->statusCodeArray()->get(pos, statusCode);
	  		if (statusCode != Success) {
		  		Log(Error, "register forward value error")
		  			.parameter("Idx", pos)
		  		    .parameter("StatusCode", OpcUaStatusCodeMap::shortString(statusCode));
	  			return false;
	  		}
	  	}

    	return true;
    }

    void
    HistoryServer::hReadValue(ApplicationHReadContext* applicationHReadContext)
    {
    	std::cout << "H_READ_VALUE" << std::endl;
    	// FIXME: todo

    	applicationHReadContext->statusCode_ = BadInternalError;
    }

}
