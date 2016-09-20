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

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/FileWriteManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// ValueWriteContext
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ValueWriteContext::ValueWriteContext(void)
	: valueName_("")
	, fileWriteEntry_()
	{
	}

	ValueWriteContext::~ValueWriteContext(void)
	{
		fileWriteEntry_.reset();
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// FileWriteManager
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FileWriteManager::FileWriteManager(void)
	: verbose_(false)
	, maxDataFolderInValueFolder_(1000)
	, maxDataFilesInDataFolder_(1000)
	, maxEntriesInDataFile_(300)
	, baseFolder_(".")
	, maxConcurrentValues_(0)
	, fileWriteEntryMap_()
	, fileWriteEntryList_()
	, ageCounter_(30)
	, deletedValueName_("")
	{
	}

	FileWriteManager::~FileWriteManager(void)
	{
		while (fileWriteEntryMap_.begin() != fileWriteEntryMap_.end()) {
			deleteFileWriteEntry(fileWriteEntryMap_.begin()->second.get());
		}
	}

	void
	FileWriteManager::verbose(bool verbose)
	{
		verbose_ = verbose;
	}

	void
	FileWriteManager::maxDataFolderInValueFolder(uint16_t maxDataFolderInValueFolder)
	{
		maxDataFolderInValueFolder_ = maxDataFolderInValueFolder;
	}

	void
	FileWriteManager::maxDataFilesInDataFolder(uint16_t maxDataFilesInDataFolder)
	{
		maxDataFilesInDataFolder_ = maxDataFilesInDataFolder;
	}

	void
	FileWriteManager::maxEntriesInDataFile(uint16_t maxEntriesInDataFile)
	{
		maxEntriesInDataFile_ = maxEntriesInDataFile;
	}

	void
	FileWriteManager::baseFolder(const boost::filesystem::path& baseFolder)
	{
		baseFolder_ = baseFolder;
	}

	void
	FileWriteManager::maxConcurrentValues(uint32_t maxConcurrentValues)
	{
		maxConcurrentValues_ = maxConcurrentValues;
	}

	uint32_t
	FileWriteManager::actConcurrentValues(void)
	{
		return fileWriteEntryMap_.size();
	}

	void
	FileWriteManager::ageCounter(uint32_t ageCounter)
	{
		ageCounter_ = ageCounter;
		if (ageCounter_ == 0) ageCounter_ = 1;
	}

	std::string
	FileWriteManager::deletedValueName(void)
	{
		return deletedValueName_;
	}

	bool
	FileWriteManager::write(const std::string& valueName, OpcUaDataValue& dataValue)
	{
		// check if write access entry exists
		FileWriteEntry::Map::iterator it;
		it = fileWriteEntryMap_.find(valueName);

		// create a new entry if it does not exist
		if (it == fileWriteEntryMap_.end()) {
			if (!createFileWriteEntry(valueName)) {
				return false;
			}
			return write(valueName, dataValue);
		}

		// write data to historical value store
		FileWriteEntry::SPtr fileWriteEntry = it->second;
		return write(fileWriteEntry, dataValue);
	}

	bool
	FileWriteManager::write(ValueWriteContext& valueWriteContext, OpcUaDataValue& dataValue)
	{
		if (FileWriteEntry::SPtr fileEntryWrite = valueWriteContext.fileWriteEntry_.lock()) {
			return write(fileEntryWrite, dataValue);
		}

		// check if write access entry exists
		FileWriteEntry::Map::iterator it;
		it = fileWriteEntryMap_.find(valueWriteContext.valueName_);
		if (it != fileWriteEntryMap_.end()) {
			valueWriteContext.fileWriteEntry_ = it->second;
			return write(valueWriteContext, dataValue);
		}

		// create a new entry
		if (!createFileWriteEntry(valueWriteContext.valueName_)) {
			return false;
		}

		return write(valueWriteContext, dataValue);
	}

	bool
	FileWriteManager::write(FileWriteEntry::SPtr& fileWriteEntry, OpcUaDataValue& dataValue)
	{
		if (!fileWriteEntry->write(dataValue)) {
			return false;
		}

		if (maxConcurrentValues_ != 0) {
			uint32_t ageCounter = fileWriteEntry->ageCounter();

			if (ageCounter >= ageCounter_) {
				if (verbose_) {
					Log(Debug, "FileWriteManager - aging")
					    .parameter("ValueName", fileWriteEntry->valueName());
				}

				fileWriteEntry->ageCounter(0);
				fileWriteEntry->remove();
				fileWriteEntryList_.pushAfter(*fileWriteEntry);
			}
		}
		return true;
	}

	bool
	FileWriteManager::createFileWriteEntry(const std::string& valueName)
	{
		if (verbose_) {
			Log(Debug, "FileWriteManager - create entry")
			    .parameter("ValueName", valueName);
		}

		if (maxConcurrentValues_ != 0 && maxConcurrentValues_ <= fileWriteEntryMap_.size()) {
			FileWriteEntry* fileWriteEntry = dynamic_cast<FileWriteEntry*>(fileWriteEntryList_.last());
			deleteFileWriteEntry(fileWriteEntry, true);
		}

		FileWriteEntry::SPtr fileWriteEntry = constructSPtr<FileWriteEntry>();
		fileWriteEntry->maxDataFolderInValueFolder(maxDataFolderInValueFolder_);
		fileWriteEntry->maxDataFilesInDataFolder(maxDataFilesInDataFolder_);
		fileWriteEntry->maxEntriesInDataFile(maxEntriesInDataFile_);
		fileWriteEntry->valueName(valueName);
		fileWriteEntry->baseFolder(baseFolder_);
		fileWriteEntryMap_.insert(std::make_pair(valueName, fileWriteEntry));
		fileWriteEntryList_.pushAfter(*fileWriteEntry);
		return true;
	}

	bool
	FileWriteManager::deleteFileWriteEntry(FileWriteEntry* fileWriteEntry, bool aging)
	{
		if (verbose_) {
			Log(Debug, "FileWriteManager - delete entry")
			    .parameter("ValueName", fileWriteEntry->valueName())
			    .parameter("Aging", aging);
		}

		deletedValueName_ = fileWriteEntry->valueName();

		FileWriteEntry::Map::iterator it;
		it = fileWriteEntryMap_.find(fileWriteEntry->valueName());
		if (it == fileWriteEntryMap_.end()) return false;
		it->second->remove();
		fileWriteEntryMap_.erase(it);
		return true;
	}

}
