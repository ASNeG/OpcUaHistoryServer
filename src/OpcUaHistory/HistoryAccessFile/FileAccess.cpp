/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/FileAccess.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileAccess::FileAccess(void)
	{
	}

	FileAccess::~FileAccess(void)
	{
	}

	void
	FileAccess::write(
		OpcUaNodeId& nodeId,
		OpcUaDataValue& dataValue
	)
	{
	}

	void
	FileAccess::read(
		OpcUaNodeId& nodeId,
		boost::posix_time::ptime fromTime,
		boost::posix_time::ptime toTime,
		OpcUaDataValueArray& dataValueArray,
		std::string& continuousPoint
	)
	{
	}

}
