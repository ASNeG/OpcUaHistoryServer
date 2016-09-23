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

#ifndef __OpcUaHistory_HistoryStoreConfig_h__
#define __OpcUaHistory_HistoryStoreConfig_h__

#include "OpcUaStackCore/Base/ConfigXmlManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{


#if 0
<ReadFileAccess>
  	<VerboseLogging>off</VerboseLogging>

  	<MaxConcurrentValues>500</MaxConcurrentValues>
  	<AgeCounter>10</AgeCounter>

  	<MaxContinousPoint>100</MaxContinousPoint>
  	<ContinousPointIdleTime>60000</ContinousPointIdleTime>
  	<MaxDeleteTimeoutentries>20</MaxDeleteTimeoutentries>
</ReadFileAccess>

<WriteFileAccess>
	<VerboseLogging>off</VerboseLogging>

	<MaxDataFolderInValueFolder>1000</MaxDataFolderInValueFolder>
	<MaxDataFilesInDataFolder>1000</MaxDataFilesInDataFolder>
	<MaxEntriesInDataFile>1000</MaxEntriesInDataFile>
	<MaxConcurrentValues>500</MaxConcurrentValues>
	<AgeCounter>10</AgeCounter>
</WriteFileAccess>
#endif

	class HistoryStoreConfig
	{
	  public:
		HistoryStoreConfig(void);
		~HistoryStoreConfig(void);

		bool decode(const std::string& configFileName, ConfigXmlManager& configXmlManager);

	  private:

	};

}

#endif
