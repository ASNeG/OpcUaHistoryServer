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
	{
	}

	FileOut::~FileOut(void)
	{
	}

	FileOut::Result
	FileOut::write(
		OpcUaDataValue& dataValue
	)
	{
		return Success;
	}

}
