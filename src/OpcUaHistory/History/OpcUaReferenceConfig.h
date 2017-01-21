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

#ifndef __OpcUaHistory_OpcUaReferenceConfig_h__
#define __OpcUaHistory_OpcUaReferenceConfig_h__

#include "OpcUaStackCore/BuildInTypes/OpcUaNodeId.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class OpcUaReferenceConfig
	{
	  public:
		typedef boost::shared_ptr<OpcUaReferenceConfig> SPtr;
		typedef std::vector<OpcUaReferenceConfig::SPtr> Vec;
		typedef enum
		{
			None,
			Read,
			Write,
			Mon,
			HRead,
			HWrite
		} Service;

		OpcUaReferenceConfig(void);
		~OpcUaReferenceConfig(void);

		void configFileName(const std::string& configFileName);
		void elementPrefix(const std::string& elementPrefix);
		OpcUaNodeId& nodeId(void);
		Service service(void);

	  private:
		std::string configFileName_;
		std::string elementPrefix_;
		OpcUaNodeId nodeId_;
		Service service_;
	};

}

#endif
