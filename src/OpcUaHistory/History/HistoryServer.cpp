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

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// HistoryServerItem
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	HistoryServerItem::HistoryServerItem(void)
	: contextRead_()
	, contextWrite_()
	{
	}

	HistoryServerItem::~HistoryServerItem(void)
	{
	}

	void
	HistoryServerItem::contextRead(Object::SPtr& contextRead)
	{
		contextRead_ = contextRead;
	}

	Object::SPtr&
	HistoryServerItem::contextRead(void)
	{
		return contextRead_;
	}

	void
	HistoryServerItem::contextWrite(Object::SPtr& contextWrite)
	{
		contextWrite_ = contextWrite;
	}

	Object::SPtr&
	HistoryServerItem::contextWrite(void)
	{
		return contextWrite_;
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// HistoryServer
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	HistoryServer::HistoryServer(void)
	: historyServerConfig_()
	, historyStoreIf_(nullptr)
	, applicationServiceIf_(nullptr)
	, namespaceMap_()
	, hReadCallback_(boost::bind(&HistoryServer::hReadValue, this, _1))
	, hWriteCallback_(boost::bind(&HistoryServer::hWriteValue, this, _1))
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
		ServiceTransactionNamespaceInfo::SPtr trx = constructSPtr<ServiceTransactionNamespaceInfo>();
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
	  	ServiceTransactionRegisterForward::SPtr trx = constructSPtr<ServiceTransactionRegisterForward>();
	  	RegisterForwardRequest::SPtr req = trx->request();
	  	RegisterForwardResponse::SPtr res = trx->response();
	  	req->forwardInfoSync()->readHService().setCallback(hReadCallback_);
	  	req->forwardInfoSync()->writeHService().setCallback(hWriteCallback_);
	  	req->nodesToRegister()->resize(historyServerConfig_.serverNodeConfigMap().size());

	  	uint32_t pos = 0;
	  	ServerNodeConfig::Map::iterator it;
	  	for (it = historyServerConfig_.serverNodeConfigMap().begin(); it != historyServerConfig_.serverNodeConfigMap().end(); it++) {
	  		ServerNodeConfig::SPtr snc = it->second;

	  		OpcUaNodeId::SPtr nodeId = constructSPtr<OpcUaNodeId>();
	  		*nodeId = snc->nodeId();

	  		NamespaceMap::iterator it;
	  		it = namespaceMap_.find(nodeId->namespaceIndex());
	  		if (it == namespaceMap_.end()) {
	  			Log(Error, "namespace index not exist in opc ua model")
	  				.parameter("NodeId", *nodeId);
	  			continue;
	  		}
	  		nodeId->namespaceIndex(it->second);

	  		req->nodesToRegister()->set(pos, nodeId);
	  		pos++;

	  		// add item to history server item map
	  		Object::SPtr contextRead;
	  		Object::SPtr contextWrite;
	  		historyStoreIf_->getHistoryStoreContext(snc->valueName(), contextRead, HistoryStoreIf::Read);
	  		historyStoreIf_->getHistoryStoreContext(snc->valueName(), contextWrite, HistoryStoreIf::Write);
	  		HistoryServerItem::SPtr historyServerItem = constructSPtr<HistoryServerItem>();
	  		historyServerItem->contextRead(contextRead);
	  		historyServerItem->contextWrite(contextWrite);
	  		historyServerItemMap_.insert(std::make_pair(*nodeId.get(), historyServerItem));
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
    	//
    	// use case:
    	//
    	// 1. First initial request
    	//	    Condition:
    	//		- Continous Point is empty
    	// 2. Continous Point Request
    	//	    Condition:
    	//		- Continous Point is not empty
    	//		- Release Continous Point is false
    	// 3. Delete Continous Point
    	//		Condition:
    	//		- Continous Point is not empty
    	//		- Release Continous Point is true
    	//

    	// find context
    	HistoryServerItem::Map::iterator it1;
    	it1 = historyServerItemMap_.find(applicationHReadContext->nodeId_);
    	if (it1 == historyServerItemMap_.end()) {
    		applicationHReadContext->statusCode_ = BadNotFound;
    		return;
    	}
    	HistoryServerItem::SPtr historyServerItem  = it1->second;

    	//
    	// 1. initial request
    	//
    	if (applicationHReadContext->continousPoint_.empty()) {
    		return hReadValueFirst(applicationHReadContext, historyServerItem);
    	}

    	//
    	// 2. continous point request
    	//
    	else if (!applicationHReadContext->releaseContinuationPoints_) {
    		return hReadValueNext(applicationHReadContext, historyServerItem);
    	}

    	//
    	// delete request
    	//
    	else if (applicationHReadContext->releaseContinuationPoints_) {
    		return hReadValueDelete(applicationHReadContext, historyServerItem);
    	}

    	applicationHReadContext->statusCode_ = BadInvalidArgument;
    	return;
    }

    void
    HistoryServer::hReadValueFirst(
    	ApplicationHReadContext* applicationHReadContext,
    	HistoryServerItem::SPtr& historyServerItem
    )
    {
    	//std::cout << std::endl;
    	//std::cout << "ReadFirst" << std::endl;
       	//std::cout << "ContinousPoint=" << applicationHReadContext->continousPoint_ << std::endl;
        //std::cout << "NumValuesPerNode=" << applicationHReadContext->numValuesPerNode_ << std::endl;

       	Object::SPtr context = historyServerItem->contextRead();

        // read data from value store
        OpcUaDataValue::Vec dataValueVec;
        std::string continousPoint = "";
        OpcUaDateTime startTime(applicationHReadContext->startTime_);
        OpcUaDateTime stopTime(applicationHReadContext->stopTime_);
        uint32_t maxNumResultValuesPerRequest = applicationHReadContext->numValuesPerNode_;

        bool success = historyStoreIf_->readInitial(
        	context,
        	continousPoint,
        	startTime,
        	stopTime,
        	dataValueVec,
        	applicationHReadContext->releaseContinuationPoints_,
        	applicationHReadContext->timestampsToReturn_,
        	maxNumResultValuesPerRequest
        );

        applicationHReadContext->dataValueArray_ = constructSPtr<OpcUaDataValueArray>();

        if (!success) {
        	applicationHReadContext->statusCode_ = BadInternalError;
        	return;
        }

        if (dataValueVec.size() == 0) {
        	applicationHReadContext->statusCode_ = Success;
        	return;
        }

        if (!continousPoint.empty()) {
        	applicationHReadContext->continousPoint_ = continousPoint;
        }

        // create result array
        applicationHReadContext->dataValueArray_->resize(dataValueVec.size());
        for (uint32_t idx = 0; idx < dataValueVec.size(); idx++) {
        	applicationHReadContext->dataValueArray_->set(idx, dataValueVec[idx]);
        }

        applicationHReadContext->statusCode_ = Success;
        return;
    }

    void
    HistoryServer::hReadValueNext(
    	ApplicationHReadContext* applicationHReadContext,
    	HistoryServerItem::SPtr& historyServerItem
    )
    {
       	//std::cout << std::endl;
        //std::cout << "ReadNext" << std::endl;
        //std::cout << "ContinousPoint=" << applicationHReadContext->continousPoint_ << std::endl;
        //std::cout << "NumValuesPerNode=" << applicationHReadContext->numValuesPerNode_ << std::endl;

        Object::SPtr context = historyServerItem->contextRead();

        // read data from value store
        OpcUaDataValue::Vec dataValueVec;
        OpcUaDateTime startTime(applicationHReadContext->startTime_);
        OpcUaDateTime stopTime(applicationHReadContext->stopTime_);
        uint32_t maxNumResultValuesPerRequest = applicationHReadContext->numValuesPerNode_;

        bool success = historyStoreIf_->readNext(
            applicationHReadContext->continousPoint_,
            dataValueVec,
            applicationHReadContext->releaseContinuationPoints_,
            applicationHReadContext->timestampsToReturn_,
           	maxNumResultValuesPerRequest
        );

        applicationHReadContext->dataValueArray_ = constructSPtr<OpcUaDataValueArray>();

        if (!success) {
        	applicationHReadContext->statusCode_ = BadInternalError;
            return;
        }

        if (dataValueVec.size() == 0) {
            applicationHReadContext->continousPoint_ = "";
            applicationHReadContext->statusCode_ = Success;
            return;
        }

        // create result array
        applicationHReadContext->dataValueArray_->resize(dataValueVec.size());
        for (uint32_t idx = 0; idx < dataValueVec.size(); idx++) {
            applicationHReadContext->dataValueArray_->set(idx, dataValueVec[idx]);
        }

        applicationHReadContext->statusCode_ = Success;
        return;
    }

    void
    HistoryServer::hReadValueDelete(
    	ApplicationHReadContext* applicationHReadContext,
    	HistoryServerItem::SPtr& historyServerItem
    )
    {
    	//std::cout << std::endl;
    	//std::cout << "ReadDelete" << std::endl;
       	//std::cout << "ContinousPoint=" << applicationHReadContext->continousPoint_ << std::endl;
        //std::cout << "NumValuesPerNode=" << applicationHReadContext->numValuesPerNode_ << std::endl;

        Object::SPtr context = historyServerItem->contextRead();

        // read data from value store
        OpcUaDataValue::Vec dataValueVec;
        OpcUaDateTime startTime(applicationHReadContext->startTime_);
        OpcUaDateTime stopTime(applicationHReadContext->stopTime_);
        uint32_t maxNumResultValuesPerRequest = applicationHReadContext->numValuesPerNode_;

        bool success = historyStoreIf_->readNext(
            applicationHReadContext->continousPoint_,
            dataValueVec,
            applicationHReadContext->releaseContinuationPoints_,
            applicationHReadContext->timestampsToReturn_,
           	maxNumResultValuesPerRequest
        );

        applicationHReadContext->dataValueArray_ = constructSPtr<OpcUaDataValueArray>();

        if (!success) {
        	applicationHReadContext->statusCode_ = BadInternalError;
            return;
        }

        applicationHReadContext->statusCode_ = Success;
        return;
    }

    void
    HistoryServer::hWriteValue(ApplicationHWriteContext* applicationHWriteContext)
    {
    	// find context
    	HistoryServerItem::Map::iterator it1;
    	it1 = historyServerItemMap_.find(applicationHWriteContext->nodeId_);
    	if (it1 == historyServerItemMap_.end()) {
    		applicationHWriteContext->statusCode_ = BadNotFound;
    		return;
    	}
    	HistoryServerItem::SPtr historyServerItem  = it1->second;

    	for (uint32_t idx=0; idx<applicationHWriteContext->dataValueArray_->size(); idx++) {
    		OpcUaDataValue::SPtr dataValue;
    		if (!applicationHWriteContext->dataValueArray_->get(idx, dataValue)) {
    			applicationHWriteContext->statusCode_ = BadInvalidArgument;
    			return;
    		}

    		if (!historyStoreIf_->write(historyServerItem->contextWrite(), *dataValue.get())) {
    			applicationHWriteContext->statusCode_ = BadInternalError;
    			return;
    		}
    	}
    	applicationHWriteContext->statusCode_ = Success;
    }

}
