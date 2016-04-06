/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_HistoryAccessIf_h__
#define __OpcUaHistory_HistoryAccessIf_h__

#include "OpcUaStackCore/BuildInTypes/OpcUaNodeId.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryAccessIf
	{
	  public:
		HistoryAccessIf(void);
		virtual ~HistoryAccessIf(void);

		void maxDataValuesToRead(uint32_t maxDataValuesToRead);
		uint32_t maxDataValuesToRead(void);

		virtual void write(
			OpcUaNodeId& nodeId,
			OpcUaDataValue& dataValue
		) = 0;
		virtual void read(
			OpcUaNodeId& nodeId,
			boost::posix_time::ptime fromTime,
			boost::posix_time::ptime toTime,
			OpcUaDataValueArray& dataValueArray,
			std::string& continuousPoint
		) = 0;

	  private:
		uint32_t maxDataValuesToRead_;
	};

}

#endif
