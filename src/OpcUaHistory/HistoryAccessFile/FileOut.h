/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)


   File := Record*
   Record := Length SourceTimestamp ServerTimestamp StatusCode Variant

 */

#ifndef __OpcUaHistory_FileOut_h__
#define __OpcUaHistory_FileOut_h__

#include "OpcUaHistory/HistoryAccess/HistoryAccessIf.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileOut
	{
	  public:
		typedef enum {
			Success
		} Result;

		FileOut(void);
		virtual ~FileOut(void);


		Result write(
			OpcUaDataValue& dataValue
		);

	  private:
	};

}

#endif
