/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_FileManager_h__
#define __OpcUaHistory_FileManager_h__

#include "OpcUaHistory/HistoryAccess/HistoryAccessIf.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileManager
	{
	  public:
		FileManager(void);
		~FileManager(void);

		void write(
			const std::string& name,
			OpcUaDataValue& dataValue
		);
		void read(
			const std::string& name,
			boost::posix_time::ptime fromTime,
			boost::posix_time::ptime toTime,
			OpcUaDataValueArray& dataValueArray,
			std::string& continuousPoint
		);

	  private:
	};

}

#endif
