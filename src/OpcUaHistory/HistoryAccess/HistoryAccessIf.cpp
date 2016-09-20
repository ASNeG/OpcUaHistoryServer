/*
   Copyright 2015-2016 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

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
