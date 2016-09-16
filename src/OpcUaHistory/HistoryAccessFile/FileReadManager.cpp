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
	// ValueReadContinousPoint
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	ValueReadContinousPoint::ValueReadContinousPoint(void)
	: continousPoint_("")
	, command_(NormalAccess)
	{
	}

	ValueReadContinousPoint::~ValueReadContinousPoint(void)
	{
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
	, fileReadEntryList_()
	, maxConcurrentValues_(0)
	, deletedValueName_("")
	, continousPointId_(rand() % 0xFFFFFFFF)
	, maxContinousPoints_(1000)
	, continousPointIdleTimeout_(60000)
	, continousPointMap_()
	, continousPointList_()
	, deletedContinousPoint_("")
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

	void
	FileReadManager::maxContinousPoint(uint32_t maxContinousPoints)
	{
		maxContinousPoints_ = maxContinousPoints;
	}

	void
	FileReadManager::continousPointIdleTimeout(uint32_t continousPointIdleTimeout)
	{
		continousPointIdleTimeout_ = continousPointIdleTimeout;
	}

	std::string
	FileReadManager::deletedValueName(void)
	{
		return deletedValueName_;
	}

	std::string
	FileReadManager::deletedContinousPoint(void)
	{
		return deletedContinousPoint_;
	}

	bool
	FileReadManager::readInitial(
		ValueReadContext& valueReadContext,
		ValueReadContinousPoint* continousPoint,
		FileReadEntry::SPtr& fileReadEntry,
		OpcUaDateTime& from,
		OpcUaDateTime& to,
		OpcUaDataValue::Vec& dataValueVec,
		uint32_t maxResultEntries
	)
	{
		fileReadEntry->dateTimeFrom(from);
		fileReadEntry->dateTimeTo(to);

		bool rc = fileReadEntry->readInitial(
			dataValueVec,
			maxResultEntries
		);
		if (!rc) return false;


		if (maxConcurrentValues_ != 0) {
			uint32_t ageCounter = fileReadEntry->ageCounter();

			if (ageCounter >= ageCounter_) {
				if (verbose_) {
					Log(Debug, "FileReadManager - aging")
					    .parameter("ValueName", fileReadEntry->valueName());
				}

				fileReadEntry->ageCounter(0);
				fileReadEntry->remove();
				fileReadEntryList_.pushAfter(*fileReadEntry);
			}
		}

		if (continousPoint != nullptr && fileReadEntry->maxResultEntriesReached()) {
			deleteFileReadEntry(fileReadEntry.get());
			valueReadContext.fileReadEntry_.reset();
			createContinousPoint(fileReadEntry.get(), continousPoint);
		}

		return true;
	}

	bool
	FileReadManager::readInitial(
		ValueReadContext& valueReadContext,
		ValueReadContinousPoint* continousPoint,
		OpcUaDateTime& from,
		OpcUaDateTime& to,
		OpcUaDataValue::Vec& dataValueVec,
		uint32_t maxResultEntries
	)
	{
		if (FileReadEntry::SPtr fileReadEntry = valueReadContext.fileReadEntry_.lock()) {
			return readInitial(
				valueReadContext,
				continousPoint,
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
				continousPoint,
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
			continousPoint,
			from,
			to,
			dataValueVec,
			maxResultEntries
		);
	}

	bool
	FileReadManager::readNext(
		ValueReadContinousPoint& continousPoint,
		FileReadEntry::SPtr& fileReadEntry,
		OpcUaDataValue::Vec& dataValueVec,
		uint32_t maxResultEntries
	)
	{
		// FIXME: todo
		return false;
	}

	bool
	FileReadManager::readNext(
		ValueReadContinousPoint& continousPoint,
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
			FileReadEntry* fileReadEntry = dynamic_cast<FileReadEntry*>(fileReadEntryList_.last());
			deleteFileReadEntry(fileReadEntry, true);
		}

		FileReadEntry::SPtr fileReadEntry = constructSPtr<FileReadEntry>();
		fileReadEntry->valueName(valueName);
		fileReadEntry->baseFolder(baseFolder_);
		fileReadEntryMap_.insert(std::make_pair(valueName, fileReadEntry));
		fileReadEntryList_.pushAfter(*fileReadEntry);

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

	void
	FileReadManager::createContinousPoint(FileReadEntry* fileReadEntry, ValueReadContinousPoint* continousPoint)
	{
		// create new continous point
		std::stringstream continousPointString;
		continousPointId_++;
		continousPointString << fileReadEntry->valueName() << std::hex << continousPointId_;
		continousPoint->continousPoint_ = continousPointString.str();

		// FIXME: todo
	}

	bool
	FileReadManager::deleteContinousPoint(FileReadEntry* fileReadEntry, bool timeout)
	{
		return false;
	}

}
