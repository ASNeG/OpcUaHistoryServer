/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <boost/asio.hpp>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/Utility.h"
#include "OpcUaHistory/HistoryAccessFile/FileOut.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileOut::FileOut(void)
	: ofs_()
	, writeBytes_(0)
	{
	}

	FileOut::~FileOut(void)
	{
	}

	FileAccess::Result
	FileOut::open(const std::string& fileName)
	{
		fileName_ = fileName;

		ofs_.open(fileName_.c_str(), std::ios::out | std::ios::app | std::ios::binary);
		if (ofs_.fail()) {
			Log(Error, "file open failed")
				.parameter("FileName", fileName_);
			return FileAccess::FileNotOpen;
		}

		return FileAccess::Success;
	}

	FileAccess::Result
	FileOut::close(void)
	{
		ofs_.close();
		return FileAccess::Success;
	}

	FileAccess::Result
	FileOut::write(
		OpcUaDataValue& dataValue
	)
	{
		// write data value to stream buffer
		boost::asio::streambuf sb1;
		std::iostream ios1(&sb1);
		dataValue.sourceTimestamp().opcUaBinaryEncode(ios1);
		dataValue.serverTimestamp().opcUaBinaryEncode(ios1);
		OpcUaNumber::opcUaBinaryEncode(ios1, dataValue.statusCode());
		if (dataValue.statusCode() == Success) {
			dataValue.variant()->opcUaBinaryEncode(ios1);
		}

		// write data length to stream buffer
		boost::asio::streambuf sb2;
		std::iostream ios2(&sb2);
		writeBytes_ = OpcUaStackCore::count(sb1) + sizeof(uint32_t);
		OpcUaNumber::opcUaBinaryEncode(ios1, writeBytes_);

		// write stream buffers to file
		ofs_ << ios2 << ios1;
		if (ofs_.fail()) {
			Log(Error, "file write failed")
				.parameter("FileName", fileName_);
			return FileAccess::FileWriteFailed;
		}

		return FileAccess::Success;
	}

}
