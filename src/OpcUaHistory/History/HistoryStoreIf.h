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

#ifndef __OpcUaHistory_HistoryStoreIf_h__
#define __OpcUaHistory_HistoryStoreIf_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryStoreContextBase
	{
	  public:
		typedef boost::shared_ptr<HistoryStoreContextBase> SPtr;

		HistoryStoreContextBase(void) {}
		virtual ~HistoryStoreContextBase(void) {}
	};

	class HistoryStoreIf
	{
	  public:
		HistoryStoreIf(void) {}
		virtual ~HistoryStoreIf(void) {}

		virtual bool write(HistoryStoreContextBase::SPtr valueStoreWriteContextBase, OpcUaDataValue& dataValue) = 0;
		virtual bool getHistoryStoreContext(const std::string valueName, HistoryStoreContextBase::SPtr& historyStoreContextBase) = 0;
	};

}

#endif
