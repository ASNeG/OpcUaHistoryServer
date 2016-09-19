/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/HistoryServerManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryServerManager::HistoryServerManager(void)
	{
	}

	HistoryServerManager::~HistoryServerManager(void)
	{
	}

    bool
    HistoryServerManager::startup(Config* config, IOThread::SPtr ioThread)
    {
    	// FIXME: todo
    	return true;
    }

    bool
    HistoryServerManager::shutdown(void)
    {
    	// FIXME: todo
    	return true;
    }

}
