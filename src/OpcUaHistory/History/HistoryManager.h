/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_HistoryManager_h__
#define __OpcUaHistory_HistoryManager_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Utility/IOThread.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryManager
	{
	  public:
		HistoryManager(void);
		~HistoryManager(void);

	    bool startup(Config* config, IOThread::SPtr ioThread);
	    bool shutdown(void);

	  private:
	};

}

#endif
