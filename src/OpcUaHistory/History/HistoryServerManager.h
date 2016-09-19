/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_HistoryServerManager_h__
#define __OpcUaHistory_HistoryServerManager_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Utility/IOThread.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryServerManager
	{
	  public:
		HistoryServerManager(void);
		~HistoryServerManager(void);

	    bool startup(Config* config, IOThread::SPtr ioThread);
	    bool shutdown(void);

	  private:
	};

}

#endif
