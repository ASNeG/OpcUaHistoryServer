/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_FileAccess_h__
#define __OpcUaHistory_FileAccess_h__

#include "OpcUaHistory/HistoryAccess/HistoryAccessIf.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileAccess
	{
	  public:
		FileAccess(void);
		virtual ~FileAccess(void);

		void write(
			OpcUaNodeId& nodeId,
			OpcUaDataValue& dataValue
		);
		void read(
			OpcUaNodeId& nodeId,
			boost::posix_time::ptime fromTime,
			boost::posix_time::ptime toTime,
			OpcUaDataValueArray& dataValueArray,
			std::string& continuousPoint
		);

	  private:
	};

}

#endif
