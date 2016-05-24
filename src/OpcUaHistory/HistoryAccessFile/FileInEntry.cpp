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
	, valueName_("")
	, baseFolder_()
	, valueFolder_()
	{
	}

	FileInEntry::~FileInEntry(void)
	{
	}

	void
	FileInEntry::valueName(const std::string& valueName)
	{
		valueName_ = valueName;
		valueFolder_ = baseFolder_ / boost::filesystem::path(valueName_);
	}

	void
	FileInEntry::baseFolder(const boost::filesystem::path& baseFolder)
	{
		baseFolder_ = baseFolder;
		valueFolder_ = baseFolder_ / boost::filesystem::path(valueName_);
	}

	bool
	FileInEntry::read(OpcUaDateTime& from, OpcUaDateTime& to)
	{
		// check if base folder exists
		if (!boost::filesystem::exists(baseFolder_)) {
			Log(Error, "base folder not exist")
				.parameter("BaseFolder", baseFolder_.string());
			return false;
		}

		// check if value folder exists
		if (!boost::filesystem::exists(valueFolder_)) {
			Log(Error, "value folder not exist")
				.parameter("ValueFolder", valueFolder_.string());
			return false;
		}

		return true;
	}

}
