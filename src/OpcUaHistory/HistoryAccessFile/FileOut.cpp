/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/FileOut.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileOut::FileOut(void)
	: ofs_()
	{
	}

	FileOut::~FileOut(void)
	{
	}

	FileAccess::Result
	FileOut::open(const std::string& fileName)
	{
		ofs_.open(fileName.c_str(), std::ios::out | std::ios::app | std::ios::binary);
		if (ofs_.fail()) {
			Log(Error, "file open failed")
				.parameter("FileName", fileName);
			return FileAccess::FileNotOpen;
		}

		return FileAccess::Success;
	}

	FileAccess::Result
	FileOut::close(void)
	{
		ofs_.close();
		return FileAccess::Success;
	}

	FileAccess::Result
	FileOut::write(
		OpcUaDataValue& dataValue
	)
	{
		return FileAccess::Success;
	}

}
