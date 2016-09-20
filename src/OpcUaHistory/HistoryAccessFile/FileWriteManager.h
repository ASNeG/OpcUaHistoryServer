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

#ifndef __OpcUaHistory_FileWriteManager_h__
#define __OpcUaHistory_FileWriteManager_h__

#include <boost/filesystem.hpp>
#include "OpcUaStackCore/Container/DoublyLinkedList.h"
#include "OpcUaHistory/HistoryAccessFile/FileWriteEntry.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class ValueWriteContext
	{
	  public:
		ValueWriteContext(void);
		~ValueWriteContext(void);

		std::string valueName_;
		FileWriteEntry::WPtr fileWriteEntry_;
	};

	class FileWriteManager
	{
	  public:
		FileWriteManager(void);
		~FileWriteManager(void);

		void verbose(bool verbose);
		void maxDataFolderInValueFolder(uint16_t maxDataFolderInValueFolder);
		void maxDataFilesInDataFolder(uint16_t maxDataFilesInDataFolder);
		void maxEntriesInDataFile(uint16_t maxEntriesInDataFile);
		void baseFolder(const boost::filesystem::path& baseFolder);
		void maxConcurrentValues(uint32_t maxConcurrentValues);
		uint32_t actConcurrentValues(void);
		void ageCounter(uint32_t ageCounter);
		std::string deletedValueName(void);

		bool write(const std::string& valueName, OpcUaDataValue& dataValue);
		bool write(ValueWriteContext& valueWriteContext, OpcUaDataValue& dataValue);

	  private:
		bool createFileWriteEntry(const std::string& valueName);
		bool deleteFileWriteEntry(FileWriteEntry* fileWriteEntry, bool aging=false);
		bool write(FileWriteEntry::SPtr& fileWriteEntry, OpcUaDataValue& dataValue);

		// configuration parameters
		bool verbose_;
		uint16_t maxDataFolderInValueFolder_;
		uint16_t maxDataFilesInDataFolder_;
		uint16_t maxEntriesInDataFile_;
		boost::filesystem::path baseFolder_;
		uint32_t maxConcurrentValues_;
		uint32_t ageCounter_;

		FileWriteEntry::Map fileWriteEntryMap_;
		DoublyLinkedList fileWriteEntryList_;
		std::string deletedValueName_;
	};

}

#endif
