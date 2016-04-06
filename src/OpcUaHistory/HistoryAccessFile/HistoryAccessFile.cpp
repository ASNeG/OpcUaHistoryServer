/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/HistoryAccessFile.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryAccessFile::HistoryAccessFile(void)
	: HistoryAccessIf()
	{
	}

	HistoryAccessFile::~HistoryAccessFile(void)
	{
	}

	void
	HistoryAccessFile::write(
		OpcUaNodeId& nodeId,
		OpcUaDataValue& dataValue
	)
	{
	}

	void
	HistoryAccessFile::read(
		OpcUaNodeId& nodeId,
		boost::posix_time::ptime fromTime,
		boost::posix_time::ptime toTime,
		OpcUaDataValueArray& dataValueArray,
		std::string& continuousPoint
	)
	{
	}

}
