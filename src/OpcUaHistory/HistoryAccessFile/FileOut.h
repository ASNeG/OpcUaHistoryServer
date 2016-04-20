/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)


   File := Record*
   Record := Length SourceTimestamp ServerTimestamp StatusCode Variant

 */

#ifndef __OpcUaHistory_FileOut_h__
#define __OpcUaHistory_FileOut_h__

#include "OpcUaHistory/HistoryAccessFile/FileAccess.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileOut
	{
	  public:
		FileOut(void);
		virtual ~FileOut(void);

		FileAccess::Result write(
			OpcUaDataValue& dataValue
		);

	  private:
	};

}

#endif
