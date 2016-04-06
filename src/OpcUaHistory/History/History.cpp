/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <iostream>

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/History.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	History::History(void)
	{
	}

	History::~History(void)
	{
	}

	bool
	History::startup(void)
	{
		return true;
	}

	bool
	History::shutdown(void)
	{
		return true;
	}

}
