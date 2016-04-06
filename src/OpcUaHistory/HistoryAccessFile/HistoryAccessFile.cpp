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
	HistoryAccessFile::rootDirectory(const std::string& rootDirectory)
	{
		rootDirectory_ = rootDirectory;
	}

	std::string
	HistoryAccessFile::rootDirectory(void)
	{
		return rootDirectory_;
	}

	void
	HistoryAccessFile::maxFileSize(uint32_t maxFileSize)
	{
		maxFileSize_ = maxFileSize;
	}

	uint32_t
	HistoryAccessFile::maxFileSize(void)
	{
		return maxFileSize_;
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
