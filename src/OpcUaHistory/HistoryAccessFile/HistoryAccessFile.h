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

#ifndef __OpcUaHistory_HistoryAccessFile_h__
#define __OpcUaHistory_HistoryAccessFile_h__

#include "OpcUaHistory/HistoryAccess/HistoryAccessIf.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryAccessFile
	: public HistoryAccessIf
	{
	  public:
		HistoryAccessFile(void);
		virtual ~HistoryAccessFile(void);

		void rootDirectory(const std::string& rootDirectory);
		std::string rootDirectory(void);

		// HistoryAccessIf ----------------------------------------------------
		virtual void write(
			OpcUaNodeId& nodeId,
			OpcUaDataValue& dataValue
		);
		virtual void read(
			OpcUaNodeId& nodeId,
			boost::posix_time::ptime fromTime,
			boost::posix_time::ptime toTime,
			OpcUaDataValueArray& dataValueArray,
			std::string& continuousPoint
		);
		// HistoryAccessIf ----------------------------------------------------

	  private:
		std::string rootDirectory_;
	};

}

#endif
