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

#ifndef __OpcUaHistory_ApplicationConfigIf_h__
#define __OpcUaHistory_ApplicationConfigIf_h__

#include "OpcUaStackCore/ApplicationUtility/OpcUaReferenceConfig.h"
#include <vector>

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class NamespaceElement
	{
	  public:
		typedef std::vector<NamespaceElement> Vec;

		uint32_t namespaceIndex_;
		std::string namespaceName_;
	};

	class VariableElement
	{
	  public:
		typedef std::vector<VariableElement> Vec;

		std::string name_;
		OpcUaReferenceConfig::Vec references_;
	};

}

#endif
