/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)

 */

#ifndef __OpcUaHistory_FileOutInfo_h__
#define __OpcUaHistory_FileOutInfo_h__

#include <boost/filesystem.hpp>

namespace OpcUaHistory
{

	class FileOutInfo
	{
	  public:
		FileOutInfo(void);
		virtual ~FileOutInfo(void);

		uint16_t countFolderInValueFolder(void);
		uint16_t countFilesInDataFolder(void);
		uint16_t countEntriesInFile(void);
		bool readFileInfo(const std::string& baseFolder, const std::string& name);

	  private:
		bool createValueFolder(boost::filesystem::path& valueFolder);
		bool getNewestDataFolder(boost::filesystem::path& valueFolder, std::string& newestDataFolder);
		bool createDataFolder(boost::filesystem::path& valueFolder, std::string& newestDataFolder);
		bool getNewestDataFile(boost::filesystem::path& dataFolder, std::string& newestDataFile);
		bool createDataFile(boost::filesystem::path& dataFolder, std::string& newestDataFile);
		bool readFileInfo(boost::filesystem::path& dataFile);

		// file information
		boost::filesystem::path baseFolder_;
		boost::filesystem::path valueFolder_;
		boost::filesystem::path dataFolder_;
		boost::filesystem::path dataFile_;
		uint16_t countFolderInValueFolder_;
		uint16_t countFilesInDataFolder_;
		uint16_t countEntriesInFile_;

	};

}

#endif
