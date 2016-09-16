/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaHistory/HistoryAccessFile/FileReadManager.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// ValueReadContext
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ValueReadContext::ValueReadContext(void)
	: valueName_("")
	, continousPoint_("")
	, fileReadEntry_()
	{
	}

	ValueReadContext::~ValueReadContext(void)
	{
		fileReadEntry_.reset();
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// FileReadManager
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	FileReadManager::FileReadManager(void)
	: verbose_(false)
	, baseFolder_(".")
	, ageCounter_(30)
	, fileReadEntryMap_()
	, maxConcurrentValues_(0)
	, deletedValueName_("")
	{
	}

	FileReadManager::~FileReadManager(void)
	{
		while (fileReadEntryMap_.begin() != fileReadEntryMap_.end()) {
			deleteFileReadEntry(fileReadEntryMap_.begin()->second.get());
		}
	}

	void
	FileReadManager::verbose(bool verbose)
	{
		verbose_ = verbose;
	}

	void
	FileReadManager::baseFolder(const boost::filesystem::path& baseFolder)
	{
		baseFolder_ = baseFolder;
	}

	void
	FileReadManager::maxConcurrentValues(uint32_t maxConcurrentValues)
	{
		maxConcurrentValues_ = maxConcurrentValues;
	}

	uint32_t
	FileReadManager::actConcurrentValues(void)
	{
		return fileReadEntryMap_.size();
	}

	void
	FileReadManager::ageCounter(uint32_t ageCounter)
	{
		ageCounter_ = ageCounter;
	}

	uint32_t
	FileReadManager::ageCounter(void)
	{
		return ageCounter_;
	}

	std::string
	FileReadManager::deletedValueName(void)
	{
		return deletedValueName_;
	}

	bool
	FileReadManager::readInitial(
		ValueReadContext& valueReadContext,
		FileReadEntry::SPtr& fileReadEntry,
		OpcUaDateTime& from,
		OpcUaDateTime& to,
		OpcUaDataValue::Vec& dataValueVec,
		uint32_t maxResultEntries
	)
	{
		valueReadContext.continousPoint_ = "";

		fileReadEntry->dateTimeFrom(from);
		fileReadEntry->dateTimeTo(to);

		bool rc = fileReadEntry->readInitial(
			dataValueVec,
			maxResultEntries
		);
		if (!rc) return false;


		if (fileReadEntry->maxResultEntriesReached()) {
			// FIXME: todo
		}

		if (maxConcurrentValues_ != 0) {
#if 0
			// FIXME: todo
			uint32_t ageCounter = fileReadEntry->ageCounter();

			if (ageCounter >= ageCounter_) {
				if (verbose_) {
					Log(Debug, "FileReadManager - aging")
					    .parameter("ValueName", fileWriteEntry->valueName());
				}

				fileWriteEntry->ageCounter(0);
				fileWriteEntry->remove();
				fileWriteEntryList_.pushAfter(*fileWriteEntry);
			}
#endif
		}

		return true;
	}

	bool
	FileReadManager::readInitial(
		ValueReadContext& valueReadContext,
		OpcUaDateTime& from,
		OpcUaDateTime& to,
		OpcUaDataValue::Vec& dataValueVec,
		uint32_t maxResultEntries
	)
	{
		if (FileReadEntry::SPtr fileReadEntry = valueReadContext.fileReadEntry_.lock()) {
			return readInitial(
				valueReadContext,
				fileReadEntry,
				from,
				to,
				dataValueVec,
				maxResultEntries
			);
		}

		// check if read access entry exists
		FileReadEntry::Map::iterator it;
		it = fileReadEntryMap_.find(valueReadContext.valueName_);
		if (it != fileReadEntryMap_.end()) {
			valueReadContext.fileReadEntry_ = it->second;
			return readInitial(
				valueReadContext,
				it->second,
				from,
				to,
				dataValueVec,
				maxResultEntries
			);
		}

		// create a new entry
		if (!createFileReadEntry(valueReadContext.valueName_)) {
			return false;
		}

		return readInitial(
			valueReadContext,
			from,
			to,
			dataValueVec,
			maxResultEntries
		);
	}

	bool
	FileReadManager::readNext(
		ValueReadContext& valueReadConext,
		OpcUaDataValue::Vec& dataValueVec,
		uint32_t maxResultEntries
	)
	{
		// FIXME: todo
		return false;
	}

	bool
	FileReadManager::createFileReadEntry(const std::string& valueName)
	{
		if (verbose_) {
			Log(Debug, "FileReadManager - create entry")
			    .parameter("ValueName", valueName);
		}

		if (maxConcurrentValues_ != 0 && maxConcurrentValues_ <= fileReadEntryMap_.size()) {
#if 0
			// FIXME: todo
			FileReadEntry* fileReadEntry = dynamic_cast<FileReadEntry*>(fileReadEntryList_.last());
			deleteFileReadEntry(fileReadEntry, true);
#endif
		}

		FileReadEntry::SPtr fileReadEntry = constructSPtr<FileReadEntry>();
		fileReadEntry->valueName(valueName);
		fileReadEntry->baseFolder(baseFolder_);
		fileReadEntryMap_.insert(std::make_pair(valueName, fileReadEntry));
#if 0
		// FIXME: todo
		fileReadEntryList_.pushAfter(*fileReadEntry);
#endif
		return true;
	}

	bool
	FileReadManager::deleteFileReadEntry(FileReadEntry* fileReadEntry, bool aging)
	{
		if (verbose_) {
			Log(Debug, "FileReadManager - delete entry")
			    .parameter("ValueName", fileReadEntry->valueName())
			    .parameter("Aging", aging);
		}

		deletedValueName_ = fileReadEntry->valueName();

		FileReadEntry::Map::iterator it;
		it = fileReadEntryMap_.find(fileReadEntry->valueName());
		if (it == fileReadEntryMap_.end()) return false;
		it->second->remove();
		fileReadEntryMap_.erase(it);
		return true;
	}

}
