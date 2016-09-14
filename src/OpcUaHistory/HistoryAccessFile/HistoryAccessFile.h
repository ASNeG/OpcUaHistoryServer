/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_HistoryAccessFile_h__
#define __OpcUaHistory_HistoryAccessFile_h__

#include "OpcUaHistory/HistoryAccess/HistoryAccessIf.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryAccessFile
	: public HistoryAccessIf
	{
	  public:
		HistoryAccessFile(void);
		virtual ~HistoryAccessFile(void);

		void rootDirectory(const std::string& rootDirectory);
		std::string rootDirectory(void);

		// HistoryAccessIf ----------------------------------------------------
		virtual void write(
			OpcUaNodeId& nodeId,
			OpcUaDataValue& dataValue
		);
		virtual void read(
			OpcUaNodeId& nodeId,
			boost::posix_time::ptime fromTime,
			boost::posix_time::ptime toTime,
			OpcUaDataValueArray& dataValueArray,
			std::string& continuousPoint
		);
		// HistoryAccessIf ----------------------------------------------------

	  private:
		std::string rootDirectory_;
	};

}

#endif
