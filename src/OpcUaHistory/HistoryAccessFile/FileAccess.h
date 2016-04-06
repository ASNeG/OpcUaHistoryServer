/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_FileAccess_h__
#define __OpcUaHistory_FileAccess_h__

#include "OpcUaHistory/HistoryAccess/HistoryAccessIf.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileAccess
	{
	  public:
		typedef enum {
			Success
		} Result;

		FileAccess(void);
		virtual ~FileAccess(void);

		void fileName(const std::string& fileName);
		std::string& fileName(void);

		Result write(
			OpcUaDataValue& dataValue
		);
		Result read(
			boost::posix_time::ptime fromTime,
			boost::posix_time::ptime toTime,
			OpcUaDataValueArray& dataValueArray,
			std::string& continuousPoint
		);

	  private:
		std::string fileName_;
	};

}

#endif
