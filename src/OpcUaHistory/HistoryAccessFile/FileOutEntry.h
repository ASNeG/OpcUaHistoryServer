/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)

 */

#ifndef __OpcUaHistory_FileOutEntry_h__
#define __OpcUaHistory_FileOutEntry_h__

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "OpcUaStackCore/Container/DoublyLinkedList.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileOutEntry
	: public DoublyLinkedList
	{
	  public:
		FileOutEntry(void);
		virtual ~FileOutEntry(void);

		void maxDataFolderInValueFolder(uint16_t maxDataFolderInValueFolder);
		void maxDataFilesInDataFolder(uint16_t maxDataFilesInDataFolder);
		void maxEntriesInDataFile(uint16_t maxEntriesInDataFile);
		void valueName(const std::string& valueName);
		void baseFolder(const boost::filesystem::path& baseFolder);

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

		uint16_t countDataFolderInValueFolder_;
		uint16_t countDataFilesInDataFolder_;
		uint16_t countEntriesInDataFile_;

		boost::filesystem::ofstream ofs_;
	};

}

#endif
