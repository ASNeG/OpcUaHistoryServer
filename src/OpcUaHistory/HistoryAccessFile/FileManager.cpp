/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/FileManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileManager::FileManager(void)
	{
	}

	FileManager::~FileManager(void)
	{
	}

	void
	FileManager::write(
		const std::string& name,
		OpcUaDataValue& dataValue
	)
	{
	}

	void
	FileManager::read(
		const std::string& name,
		boost::posix_time::ptime fromTime,
		boost::posix_time::ptime toTime,
		OpcUaDataValueArray& dataValueArray,
		std::string& continuousPoint
	)
	{
	}

}
