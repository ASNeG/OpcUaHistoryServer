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

#ifndef __OpcUaHistory_ClientMonitoredItem_h__
#define __OpcUaHistory_ClientMonitoredItem_h__

#include <boost/shared_ptr.hpp>
#include <map>
#include <set>
#include <vector>
#include <stdint.h>
#include "OpcUaStackCore/BuildInTypes/OpcUaNodeId.h"
#include "OpcUaHistory/OpcUaClient/DataChangeFilter.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class ClientMonitoredItem
	{
	  public:
		typedef boost::shared_ptr<ClientMonitoredItem> SPtr;
		typedef std::map<std::string, ClientMonitoredItem::SPtr> Map;
		typedef std::set<ClientMonitoredItem::SPtr> Set;
		typedef std::vector<std::string> NamespaceUris;
		typedef std::map<uint32_t, uint32_t> NamespaceMap;

		typedef enum {
			S_Error,
		} State;

		ClientMonitoredItem(void);
		~ClientMonitoredItem(void);

		uint32_t samplingInterval(void);
		void samplingInterval(uint32_t samplingInterval);
		uint32_t queueSize(void);
		void queueSize(uint32_t queueSize);
		DataChangeFilter dataChangeFilter(void);
		void dataChangeFilter(DataChangeFilter dataChangeFilter);
		OpcUaNodeId& nodeId(void);
		void nodeId(OpcUaNodeId& nodeId);

	  private:
		uint32_t samplingInterval_;
		uint32_t queueSize_;
		OpcUaNodeId nodeId_;
		std::string valueName_;
		DataChangeFilter dataChangeFilter_;
	};

}

#endif
