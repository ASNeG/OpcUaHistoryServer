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

#ifndef __OpcUaHistory_HistoryServerConfig_h__
#define __OpcUaHistory_HistoryServerConfig_h__

#include "OpcUaStackCore/Base/ConfigXmlManager.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaNodeId.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class ServerNodeConfig
	{
	  public:
		typedef boost::shared_ptr<ServerNodeConfig> SPtr;
		typedef std::map<std::string, ServerNodeConfig::SPtr> Map;

		ServerNodeConfig(void);
		~ServerNodeConfig(void);

		OpcUaNodeId& nodeId(void);
		void nodeId(OpcUaNodeId& nodeId);
		std::string& valueName(void);
		void valueName(const std::string& valueName);

	  private:
		OpcUaNodeId nodeId_;
		std::string valueName_;
	};

	class HistoryServerConfig
	{
	  public:
		HistoryServerConfig(void);
		~HistoryServerConfig(void);

		bool decode(const std::string& configFileName, ConfigXmlManager& configXmlManager);

		std::vector<std::string>& namespaceUris(void);
		ServerNodeConfig::Map& serverNodeConfigMap(void);

	  private:
		bool decodeNamespaceUris(Config::SPtr& config);
		bool decodeNode(Config& config, ServerNodeConfig& clientNodeConfig);

		std::vector<std::string> namespaceUris_;
		ServerNodeConfig::Map serverNodeConfigMap_;

	};

}

#endif
