/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/HistoryClient.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryClient::HistoryClient(void)
	{
	}

	HistoryClient::~HistoryClient(void)
	{
	}

    bool
    HistoryClient::startup(Config* config, IOThread::SPtr ioThread)
    {
    	// FIXME: todo
    	return true;
    }

    bool
    HistoryClient::shutdown(void)
    {
    	// FIXME: todo
    	return true;
    }

}
