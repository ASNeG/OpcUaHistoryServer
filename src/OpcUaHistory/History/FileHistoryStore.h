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

#ifndef __OpcUaHistory_FileHistoryStore_h__
#define __OpcUaHistory_FileHistoryStore_h__

#include "OpcUaStackCore/Base/Config.h"
#include "OpcUaStackCore/Utility/IOThread.h"
#include "OpcUaHistory/Interface/HistoryStoreIf.h"
#include "OpcUaHistory/History/FileHistoryStoreConfig.h"
#include "OpcUaHistory/HistoryAccessFile/FileReadManager.h"
#include "OpcUaHistory/HistoryAccessFile/FileWriteManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class HistoryStoreContextWrite
	: public Object
	{
	  public:
		typedef boost::shared_ptr<HistoryStoreContextWrite> SPtr;

		HistoryStoreContextWrite(void);
		~HistoryStoreContextWrite(void);

		ValueWriteContext valueWriteContext_;
	};


	class HistoryStoreContextRead
	: public Object
	{
	  public:
		typedef boost::shared_ptr<HistoryStoreContextRead> SPtr;

		HistoryStoreContextRead(void);
		~HistoryStoreContextRead(void);

		ValueReadContext valueReadContext_;
	};


	class FileHistoryStore
	: public HistoryStoreIf
	{
	  public:
		typedef boost::shared_ptr<FileHistoryStore> SPtr;
		typedef std::map<std::string, FileHistoryStore::SPtr> Map;
		typedef std::set<FileHistoryStore::SPtr> Set;

		FileHistoryStore(void);
		~FileHistoryStore(void);

        bool startup(FileHistoryStoreConfig* fileHistoryConfig);
        bool shutdown(void);

	  private:

	    // -- HistoryStoreIf --------------------------------------------------
	    bool write(
	    	Object::SPtr& context,
	    	OpcUaDataValue& dataValue
	    );
	    bool getHistoryStoreContext(
	    	const std::string valueName,
	    	Object::SPtr& context,
	    	HistoryStoreIf::Access access
	    );
		bool readInitial(
			Object::SPtr& context,
			std::string& continousPoint,
			OpcUaDateTime& from,
			OpcUaDateTime& to,
			OpcUaDataValue::Vec& dataValueVec,
			OpcUaBoolean releaseContinuationPoint,
			TimestampsToReturn timestampsToReturn = TimestampsToReturn_Both,
			uint32_t maxResultEntries = 0
		);
		bool readNext(
			std::string& continousPoint,
			OpcUaDataValue::Vec& dataValueVec,
			OpcUaBoolean releaseContinuationPoint,
			TimestampsToReturn timestampsToReturn = TimestampsToReturn_Both,
			uint32_t maxResultEntries = 0
		);
	    // -- HistoryStoreIf --------------------------------------------------

        FileHistoryStoreConfig* fileHistoryConfig_;

        FileReadManager fileReadManager_;
        FileWriteManager fileWriteManager_;
	};

}

#endif
