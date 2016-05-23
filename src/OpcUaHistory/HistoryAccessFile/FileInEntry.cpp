/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <boost/asio.hpp>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/Utility.h"
#include "OpcUaHistory/HistoryAccessFile/FileInEntry.h"

namespace OpcUaHistory
{

	FileInEntry::FileInEntry(void)
	: DoublyLinkedList()
	{
	}

	FileInEntry::~FileInEntry(void)
	{
	}

}
