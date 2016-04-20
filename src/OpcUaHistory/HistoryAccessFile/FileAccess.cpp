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
	: fileName_()
	{
	}

	FileAccess::~FileAccess(void)
	{
	}

	bool
	FileAccess::open(const std::string& fileName)
	{
		fileName_ = fileName;
		return true;
	}

	bool
	FileAccess::close(void)
	{
		return true;
	}

	FileAccess::Result
	FileAccess::write(
		OpcUaDataValue& dataValue
	)
	{
		return Success;
	}

	FileAccess::Result
	FileAccess::read(
		boost::posix_time::ptime fromTime,
		boost::posix_time::ptime toTime,
		OpcUaDataValueArray& dataValueArray,
		std::string& continuousPoint
	)
	{
		return Success;
	}

}
