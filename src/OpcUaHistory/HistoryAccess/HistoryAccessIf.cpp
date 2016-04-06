/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccess/HistoryAccessIf.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryAccessIf::HistoryAccessIf(void)
	{
	}

	HistoryAccessIf::~HistoryAccessIf(void)
	{
	}

	void
	HistoryAccessIf::maxDataValuesToRead(uint32_t maxDataValuesToRead)
	{
		maxDataValuesToRead_ = maxDataValuesToRead;
	}

	uint32_t
	HistoryAccessIf::maxDataValuesToRead(void)
	{
		return maxDataValuesToRead_;
	}

}
