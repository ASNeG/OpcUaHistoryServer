/*
   Copyright 2015-2017 Kai Huebl (kai@huebl-sgh.de)

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

#ifndef __OpcUaHistory_HistoryStoreIf_h__
#define __OpcUaHistory_HistoryStoreIf_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/Object.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"
#include "OpcUaStackCore/ServiceSet/TimestampsToReturn.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryStoreIf
	{
	  public:
		typedef enum {
			Read,
			Write
		} Access;

		HistoryStoreIf(void) {}
		virtual ~HistoryStoreIf(void) {}

		virtual bool write(
			Object::SPtr& context,
			OpcUaDataValue& dataValue
		) = 0;

		virtual bool getHistoryStoreContext(
			const std::string valueName,
			Object::SPtr& context,
			Access access
		) = 0;

		virtual bool readInitial(
			Object::SPtr& context,
			std::string& continousPoint,
			OpcUaDateTime& from,
			OpcUaDateTime& to,
			OpcUaDataValue::Vec& dataValueVec,
			OpcUaBoolean releaseContinuationPoint,
			TimestampsToReturn timestampsToReturn = TimestampsToReturn_Both,
			uint32_t maxResultEntries = 0
		) = 0;

		virtual bool readNext(
			std::string& continousPoint,
			OpcUaDataValue::Vec& dataValueVec,
			OpcUaBoolean releaseContinuationPoint,
			TimestampsToReturn timestampsToReturn = TimestampsToReturn_Both,
			uint32_t maxResultEntries = 0
		) = 0;
	};

}

#endif
