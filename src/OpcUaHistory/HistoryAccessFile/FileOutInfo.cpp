/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <fstream>
#include <iostream>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDateTime.h"
#include "OpcUaHistory/HistoryAccessFile/FileOutInfo.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	FileOutInfo::FileOutInfo(void)
	: newFile_(false)
	, baseFolder_()
	, valueFolder_()
	, dataFolder_()
	, dataFile_()
	, countEntriesInFile_(0)
	, countFilesInDataFolder_(0)
	{
	}

	FileOutInfo::~FileOutInfo(void)
	{
	}

	bool
	FileOutInfo::readFileInfo(const std::string& baseFolder, const std::string& name)
	{
		baseFolder_ = baseFolder;
		valueFolder_ = baseFolder_ / name;

		// check if base folder exists
		if (!boost::filesystem::exists(baseFolder_)) {
			Log(Error, "history base folder not exist")
				.parameter("BaseFolder", baseFolder_.string());
			return false;
		}

		// check if value folder exists - if not create new value folder
		if (!boost::filesystem::exists(valueFolder_)) {
			if (!createValueFolder(valueFolder_)) {
				return false;
			}
		}

		// get newest data folder
		std::string newestDataFolder = "";
		if (!getNewestDataFolder(valueFolder_, newestDataFolder)) {
			if (!createDataFolder(valueFolder_, newestDataFolder)) {
				Log(Error, "create data folder error")
					.parameter("ValueFolder", valueFolder_.string())
					.parameter("DataFolder", newestDataFolder);
				return false;
			}
		}
		dataFolder_ = valueFolder_ / newestDataFolder;

		// get newest data file
		std::string newestDataFile = "";
		if (!getNewestDataFile(dataFolder_, newestDataFile)) {
			if (!createDataFile(dataFolder_, newestDataFile)) {
				Log(Error, "create data file error")
					.parameter("DataFolder", dataFolder_.string())
					.parameter("DataFile", newestDataFile);
				return false;
			}
		}
		dataFile_ = dataFolder_ / newestDataFile;

		// get file information
		if (!readFileInfo(dataFile_)) {
			Log(Error, "read data file info error")
				.parameter("DataFile", dataFile_.string());
			return false;
		}

		Log(Error, "xxxx").parameter("xxx", dataFile_.string());

		return true;
	}

	bool
	FileOutInfo::createValueFolder(boost::filesystem::path& valueFolder)
	{
		if (!boost::filesystem::create_directory(valueFolder)) {
			Log(Error, "create history value folder error")
			    .parameter("ValueFolder", valueFolder.string());
			return false;
		}

		return true;
	}

	bool
	FileOutInfo::getNewestDataFolder(boost::filesystem::path& valueFolder, std::string& newestDataFolder)
	{
		newestDataFolder = "";
		boost::filesystem::directory_iterator it(valueFolder);
		boost::filesystem::directory_iterator itend;
		for(; it != itend; it++) {
			if (!boost::filesystem::is_directory(*it)) {
				continue;
			}

			std::string dataFolder = (*it).path().leaf().string();

			if (newestDataFolder.empty()) {
				newestDataFolder = dataFolder;
				continue;
			}

			if (dataFolder > newestDataFolder) {
				newestDataFolder = dataFolder;
				continue;
			}
		}

		if (newestDataFolder.empty()) {
			return false;
		}

		return true;
	}

	bool
	FileOutInfo::createDataFolder(boost::filesystem::path& valueFolder, std::string& newestDataFolder)
	{
		OpcUaDateTime dateTime(boost::posix_time::microsec_clock::local_time());
		newestDataFolder = dateTime.toISOString();

		// create new data folder
		boost::filesystem::path dataFolder = valueFolder / newestDataFolder;
		if (!boost::filesystem::create_directory(dataFolder)) {
			Log(Error, "create history data folder error")
			    .parameter("DataFolder", dataFolder.string());
			return false;
		}

		return true;
	}

	bool
	FileOutInfo::getNewestDataFile(boost::filesystem::path& dataFolder, std::string& newestDataFile)
	{
		countFilesInDataFolder_ = 0;
		newestDataFile = "";
		boost::filesystem::directory_iterator it(dataFolder);
		boost::filesystem::directory_iterator itend;
		for(; it != itend; it++) {
			if (!boost::filesystem::is_regular_file(*it)) {
				continue;
			}

			countFilesInDataFolder_++;
			std::string dataFile = (*it).path().leaf().string();

			if (newestDataFile.empty()) {
				newestDataFile = dataFile;
				continue;
			}

			if (*it > newestDataFile) {
				newestDataFile = dataFile;
				continue;
			}
		}

		if (newestDataFile.empty()) {
			return false;
		}

		return true;
	}

	bool
	FileOutInfo::createDataFile(boost::filesystem::path& dataFolder, std::string& newestDataFile)
	{
		OpcUaDateTime dateTime(boost::posix_time::microsec_clock::local_time());
		newestDataFile = dateTime.toISOString();

		// create new data file
		boost::filesystem::path dataFile = dataFolder / newestDataFile;
		std::ofstream o(dataFile.string().c_str());
		countEntriesInFile_++;
		newFile_ = true;

		return true;
	}

	bool
	FileOutInfo::readFileInfo(boost::filesystem::path& dataFile)
	{
		// check if file new
		if (newFile_) {
			countEntriesInFile_ = 0;
			newFile_ = false;
			return true;
		}

		// get file size
		uint32_t fileSize = boost::filesystem::file_size(dataFile);
		if (fileSize == 0) {
			countEntriesInFile_ = 0;
			newFile_ = false;
			return true;
		}

		// read number of entries in file
		// FIXME: todo

		return true;
	}

}
