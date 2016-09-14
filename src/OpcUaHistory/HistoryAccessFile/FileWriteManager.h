/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_FileWriteManager_h__
#define __OpcUaHistory_FileWriteManager_h__

#include <boost/filesystem.hpp>

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileWriteManager
	{
	  public:
		FileWriteManager(void);
		~FileWriteManager(void);

		void maxDataFolderInValueFolder(uint16_t maxDataFolderInValueFolder);
		void maxDataFilesInDataFolder(uint16_t maxDataFilesInDataFolder);
		void maxEntriesInDataFile(uint16_t maxEntriesInDataFile);
		void baseFolder(const boost::filesystem::path& baseFolder);
		void maxConcurrentValues(uint32_t maxConcurrentValues);

		bool write(const std::string& valueName, OpcUaDataValue& dataValue);

	  private:
		// configuration parameters

		uint16_t maxDataFolderInValueFolder_;
		uint16_t maxDataFilesInDataFolder_;
		uint16_t maxEntriesInDataFile_;
		boost::filesystem::path baseFolder_;
		uint32_t maxConcurrentValues_;
	};

}

#endif
