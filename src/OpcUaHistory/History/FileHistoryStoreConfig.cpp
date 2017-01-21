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

#include <boost/algorithm/string.hpp>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/History/FileHistoryStoreConfig.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class HistoryStoreReadConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FileHistoryStoreReadConfig::FileHistoryStoreReadConfig(void)
	: verboseLogging_(false)
	, maxConcurrentValues_(500)
	, ageCounter_(10)
	, maxContinousPoint_(100)
	, continousPointIdleTimeout_(60000)
	, maxDeleteTimeoutEntries_(20)
	, maxNumResultValuesPerNode_(2000)
	{
	}

	FileHistoryStoreReadConfig::~FileHistoryStoreReadConfig(void)
	{
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class HistoryStoreWriteConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FileHistoryStoreWriteConfig::FileHistoryStoreWriteConfig(void)
	: verboseLogging_(false)
	, maxDataFolderInValueFolder_(1000)
	, maxDataFilesInDataFolder_(1000)
	, maxEntriesInDataFile_(1000)
	, maxConcurrentValues_(500)
	, ageCounter_(10)
	{
	}

	FileHistoryStoreWriteConfig::~FileHistoryStoreWriteConfig(void)
	{
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// class FileHistoryStoreConfig
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FileHistoryStoreConfig::FileHistoryStoreConfig(void)
	: activate_(false)
	, configFileName_("")
	, baseFolder_("")
	, readConfig_()
	, writeConfig_()
	{
	}

	FileHistoryStoreConfig::~FileHistoryStoreConfig(void)
	{
	}

	void
	FileHistoryStoreConfig::configFileName(const std::string& configFileName)
	{
		configFileName_ = configFileName;
	}

	void
	FileHistoryStoreConfig::elementPrefix(const std::string& elementPrefix)
	{
		elementPrefix_ = elementPrefix;
	}

	bool
	FileHistoryStoreConfig::activate(void)
	{
		return activate_;
	}

	std::string&
	FileHistoryStoreConfig::baseFolder(void)
	{
		return baseFolder_;
	}

	FileHistoryStoreReadConfig&
	FileHistoryStoreConfig::historyStoreFileReadConfig(void)
	{
		return readConfig_;
	}

	FileHistoryStoreWriteConfig&
	FileHistoryStoreConfig::historyStoreFileWriteConfig(void)
	{
		return writeConfig_;
	}

	bool
	FileHistoryStoreConfig::decode(Config& config)
	{
		// read history model information
		std::string historyStoreModel = "";
		if (!config.getConfigParameter("HistoryStoreModel", historyStoreModel)) {
			Log(Error, "element missing config file")
				.parameter("Element", "HistoryStoreModel.HistoryStore.HistoryStoreModel")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}
		if (historyStoreModel != "FileHistoryStore") {
			activate_ = false;
			return true;
		}
		activate_ = true;

		// get file history store
		boost::optional<Config> child = config.getChild("FileHistoryStore");
		if (!child) {
			Log(Error, "element missing in config file")
				.parameter("Element", "FileHistoryStore")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		// get base configuration
		if (!decodeFileStoreBase(*child)) {
			Log(Error, "read configuration file error")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}

		// parse read configuration
		boost::optional<Config> childRead = child->getChild("ReadFileAccess");
		if (!child) {
			Log(Error, "element missing in config file")
				.parameter("Element", "FileHistoryStore.ReadFileAccess")
				.parameter("ConfigFileName", configFileName_);
			return false;
		}
		if (!decodeFileStoreRead(*childRead)) {
			Log(Error, "read configuration file error")
				.parameter("ConfigFile", configFileName_);
			return false;
		}

		// parse write configuration
		boost::optional<Config> childWrite = child->getChild("WriteFileAccess");
		if (!child) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.WriteFileAccess")
				.parameter("ConfigFile", configFileName_);
			return false;
		}
		if (!decodeFileStoreWrite(*childWrite)) {
			Log(Error, "read configuration file error")
				.parameter("ConfigFile", configFileName_);
			return false;
		}

		return true;
	}

	bool
	FileHistoryStoreConfig::decodeFileStoreBase(Config& config)
	{
		bool success;

		// get base folder
		success = config.getConfigParameter("BaseFolder", baseFolder_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.BaseFolder");
			return false;
		}

		return true;
	}

	bool
	FileHistoryStoreConfig::decodeFileStoreRead(Config& config)
	{
		bool success;

		// get verbose logging
		std::string verboseLogging;
		success = config.getConfigParameter("VerboseLogging", verboseLogging);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.ReadFileAccess.VerboseLogging");
			return false;
		}
		boost::to_upper(verboseLogging);
		if (verboseLogging == "ON") {
			readConfig_.verboseLogging_ = true;
		}
		else if (verboseLogging == "OFF") {
			readConfig_.verboseLogging_ = false;
		}
		else {
			Log(Error, "parameter invalid in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.ReadFileAccess.VerboseLogging")
				.parameter("VerboseLogging", verboseLogging);
			return false;
		}


		// get max concurrent values
		success = config.getConfigParameter("MaxConcurrentValues", readConfig_.maxConcurrentValues_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.ReadFileAccess.MaxConcurrentValues");
			return false;
		}

		// get age counter
		success = config.getConfigParameter("AgeCounter", readConfig_.ageCounter_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.ReadFileAccess.AgeCounter");
			return false;
		}

		// get max continous point
		success = config.getConfigParameter("MaxContinousPoint", readConfig_.maxContinousPoint_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.ReadFileAccess.MaxContinousPoint");
			return false;
		}

		// get continous point idle timeout
		success = config.getConfigParameter("ContinousPointIdleTimeout", readConfig_.continousPointIdleTimeout_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.ReadFileAccess.ContinousPointIdleTimeout");
			return false;
		}

		// get max delete timeout entries
		success = config.getConfigParameter("MaxDeleteTimeoutEntries", readConfig_.maxDeleteTimeoutEntries_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.ReadFileAccess.MaxDeleteTimeoutEntries");
			return false;
		}

		// get max num result values per request
		success = config.getConfigParameter("MaxNumResultValuesPerNode", readConfig_.maxNumResultValuesPerNode_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.ReadFileAccess.MaxNumResultValuesPerNode");
			return false;
		}


		return true;
	}

	bool
	FileHistoryStoreConfig::decodeFileStoreWrite(Config& config)
	{
		bool success;

		// get verbose logging
		std::string verboseLogging;
		success = config.getConfigParameter("VerboseLogging", verboseLogging);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.WriteFileAccess.VerboseLogging");
			return false;
		}
		boost::to_upper(verboseLogging);
		if (verboseLogging == "ON") {
			writeConfig_.verboseLogging_ = true;
		}
		else if (verboseLogging == "OFF") {
			writeConfig_.verboseLogging_ = false;
		}
		else {
			Log(Error, "parameter invalid in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.WriteFileAccess.VerboseLogging")
				.parameter("VerboseLogging", verboseLogging);
			return false;
		}

		// get MaxDataFolderInValueFolder
		success = config.getConfigParameter("MaxDataFolderInValueFolder", writeConfig_.maxDataFolderInValueFolder_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.WriteFileAccess.MaxDataFolderInValueFolder");
			return false;
		}

		// get MaxDataFilesInDataFolder
		success = config.getConfigParameter("MaxDataFilesInDataFolder", writeConfig_.maxDataFilesInDataFolder_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.WriteFileAccess.MaxDataFilesInDataFolder");
			return false;
		}

		// get MaxEntriesInDataFile
		success = config.getConfigParameter("MaxEntriesInDataFile", writeConfig_.maxEntriesInDataFile_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.WriteFileAccess.MaxEntriesInDataFile");
			return false;
		}

		// get MaxConcurrentValues
		success = config.getConfigParameter("MaxConcurrentValues", writeConfig_.maxConcurrentValues_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.WriteFileAccess.MaxConcurrentValues");
			return false;
		}

		// get AgeCounter
		success = config.getConfigParameter("AgeCounter", writeConfig_.ageCounter_);
		if (!success) {
			Log(Error, "parameter missing in config file")
				.parameter("Parameter", "HistoryStore.FileHistoryStore.WriteFileAccess.AgeCounter");
			return false;
		}

		return true;
	}

}
