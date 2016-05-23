/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)

 */

#ifndef __OpcUaHistory_FileInEntry_h__
#define __OpcUaHistory_FileInEntry_h__

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "OpcUaStackCore/Container/DoublyLinkedList.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileInEntry
	: public DoublyLinkedList
	{
	  public:
		FileInEntry(void);
		virtual ~FileInEntry(void);
	};

}

#endif
