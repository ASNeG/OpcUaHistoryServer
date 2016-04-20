/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)


   File := Record*
   Record := Length SourceTimestamp ServerTimestamp StatusCode Variant

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
			Success,
			FileNotOpen,
			FileWriteFailed
		} Result;

		FileAccess(void);
		virtual ~FileAccess(void);

		bool open(const std::string& fileName);
		bool close(void);

	  private:
		std::string fileName_;
	};

}

#endif
