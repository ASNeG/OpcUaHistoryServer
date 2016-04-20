/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/FileIn.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileIn::FileIn(void)
	{
	}

	FileIn::~FileIn(void)
	{
	}

	FileAccess::Result
	FileIn::read(
		boost::posix_time::ptime fromTime,
		boost::posix_time::ptime toTime,
		OpcUaDataValueArray& dataValueArray,
		std::string& continuousPoint
	)
	{
		return FileAccess::Success;
	}

}
