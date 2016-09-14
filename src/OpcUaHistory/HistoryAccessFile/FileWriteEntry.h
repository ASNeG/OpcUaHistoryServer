/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)

 */

#ifndef __OpcUaHistory_FileWriteEntry_h__
#define __OpcUaHistory_FileWriteEntry_h__

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "OpcUaStackCore/Container/DoublyLinkedList.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileWriteEntry
	: public DoublyLinkedList
	{
	  public:
		FileWriteEntry(void);
		virtual ~FileWriteEntry(void);

		void maxDataFolderInValueFolder(uint16_t maxDataFolderInValueFolder);
		void maxDataFilesInDataFolder(uint16_t maxDataFilesInDataFolder);
		void maxEntriesInDataFile(uint16_t maxEntriesInDataFile);
		void valueName(const std::string& valueName);
		void baseFolder(const boost::filesystem::path& baseFolder);
		void usedCounter(uint32_t usedCounter);
		uint32_t usedCounter(void);

		bool write(OpcUaDataValue& dataValue);

	  private:
		bool createValueFolder(void);
		void getNewestDataFolder(void);
		bool createDataFolder(OpcUaDataValue& dataValue);
		void getNewestDataFile(void);
		bool createDataFile(OpcUaDataValue& dataValue);
		bool openDataFile(OpcUaDataValue& dataValue);
		bool writeData(OpcUaDataValue& dataValue);

		// configuration parameters
		std::string valueName_;
		boost::filesystem::path baseFolder_;
		boost::filesystem::path valueFolder_;
		boost::filesystem::path dataFolder_;
		boost::filesystem::path dataFile_;
		uint16_t maxDataFolderInValueFolder_;
		uint16_t maxDataFilesInDataFolder_;
		uint16_t maxEntriesInDataFile_;

		uint32_t usedCounter_;
		uint16_t countDataFolderInValueFolder_;
		uint16_t countDataFilesInDataFolder_;
		uint16_t countEntriesInDataFile_;

		boost::filesystem::ofstream ofs_;
	};

}

#endif
