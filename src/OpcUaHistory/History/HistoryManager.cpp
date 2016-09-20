/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/HistoryManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryManager::HistoryManager(void)
	{
	}

	HistoryManager::~HistoryManager(void)
	{
	}

    bool
    HistoryManager::startup(Config* config, IOThread::SPtr ioThread)
    {
    	// FIXME: todo
    	return true;
    }

    bool
    HistoryManager::shutdown(void)
    {
    	// FIXME: todo
    	return true;
    }

}
