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
#include "OpcUaHistory/HistoryAccessFile/HistoryAccessFile.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	HistoryAccessFile::HistoryAccessFile(void)
	: HistoryAccessIf()
	{
	}

	HistoryAccessFile::~HistoryAccessFile(void)
	{
	}

	void
	HistoryAccessFile::rootDirectory(const std::string& rootDirectory)
	{
		rootDirectory_ = rootDirectory;
	}

	std::string
	HistoryAccessFile::rootDirectory(void)
	{
		return rootDirectory_;
	}

	void
	HistoryAccessFile::write(
		OpcUaNodeId& nodeId,
		OpcUaDataValue& dataValue
	)
	{
	}

	void
	HistoryAccessFile::read(
		OpcUaNodeId& nodeId,
		boost::posix_time::ptime fromTime,
		boost::posix_time::ptime toTime,
		OpcUaDataValueArray& dataValueArray,
		std::string& continuousPoint
	)
	{
	}

}
