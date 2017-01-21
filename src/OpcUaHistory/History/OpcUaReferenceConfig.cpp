/*
   Copyright 2017 Kai Huebl (kai@huebl-sgh.de)

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
#include "OpcUaHistory/History/OpcUaReferenceConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	OpcUaReferenceConfig::OpcUaReferenceConfig(void)
	: configFileName_("")
	, elementPrefix_("")
	, nodeId_()
	, service_(None)
	{
	}

	OpcUaReferenceConfig::~OpcUaReferenceConfig(void)
	{
	}

	void
	OpcUaReferenceConfig::configFileName(const std::string& configFileName)
	{
		configFileName_ = configFileName;
	}

	void
	OpcUaReferenceConfig::elementPrefix(const std::string& elementPrefix)
	{
		elementPrefix_ = elementPrefix;
	}

	OpcUaNodeId&
	OpcUaReferenceConfig::nodeId(void)
	{
		return nodeId_;
	}

	OpcUaReferenceConfig::Service
	OpcUaReferenceConfig::service(void)
	{
		return service_;
	}

}
