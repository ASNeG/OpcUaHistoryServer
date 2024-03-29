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

#ifndef __OpcUaHistory_HistorySoreModelConfig_h__
#define __OpcUaHistory_HistorySoreModelConfig_h__

#include "OpcUaStackCore/Base/ConfigXmlManager.h"
#include "OpcUaHistory/History/FileHistoryStoreConfig.h"
#include "OpcUaHistory/History/HistoryStoreModelValuesConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryStoreModelConfig
	{
	  public:

		HistoryStoreModelConfig(void);
		~HistoryStoreModelConfig(void);

		FileHistoryStoreConfig& fileHistoryStoreConfig(void);
		HistoryStoreModelValuesConfig& historyStoreModelValuesConfig(void);

		bool decode(const std::string& configFileName, ConfigXmlManager& configXmlManager);

	  private:
		FileHistoryStoreConfig fileHistoryStoreConfig_;
		HistoryStoreModelValuesConfig historyStoreModelValuesConfig_;

	};

}

#endif
