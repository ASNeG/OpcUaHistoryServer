/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)

 */

#ifndef __OpcUaHistory_FileOutEntry_h__
#define __OpcUaHistory_FileOutEntry_h__

#include <boost/filesystem.hpp>
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
		void basePath(const boost::filesystem::path& basePath);

		bool write(OpcUaDataValue& dataValue);

	  private:

		// configuration parameters
		std::string valueName_;
		boost::filesystem::path basePath_;
		uint16_t maxDataFolderInValueFolder_;
		uint16_t maxDataFilesInDataFolder_;
		uint16_t maxEntriesInDataFile_;

		uint16_t countDataFolderInValueFolder_;
		uint16_t countDataFilesInDataFolder_;
		uint16_t countEntriesInDataFile_;
	};

}

#endif
