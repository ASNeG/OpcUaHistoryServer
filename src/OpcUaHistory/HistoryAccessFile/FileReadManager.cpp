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
	, readComplete_(false)
	, error_(false)
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
	, maxDeleteTimeoutEntries_(10)
	{
	}

	FileReadManager::~FileReadManager(void)
	{
		while (fileReadEntryMap_.begin() != fileReadEntryMap_.end()) {
			deleteFileReadEntry(fileReadEntryMap_.begin()->second.get());
		}

		while (continousPointMap_.begin() != continousPointMap_.end()) {
			deleteContinousPoint(continousPointMap_.begin()->second.get());
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


	void
	FileReadManager::maxDeleteTimeoutEntries(uint32_t maxDeleteTimeoutEntries)
	{
		maxDeleteTimeoutEntries_ = maxDeleteTimeoutEntries;
	}

	bool
	FileReadManager::readInitial(
		ValueReadContext& valueReadContext,
		ValueReadContinousPoint* continousPoint,
		FileReadEntry::SPtr& fileReadEntry,
		OpcUaDateTime& from,
		OpcUaDateTime& to,
		OpcUaDataValue::Vec& dataValueVec,
		TimestampsToReturn timestampsToReturn,
		uint32_t maxResultEntries
	)
	{
		fileReadEntry->dateTimeFrom(from);
		fileReadEntry->dateTimeTo(to);

		bool rc = fileReadEntry->readInitial(
			dataValueVec,
			timestampsToReturn,
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

			createContinousPoint(fileReadEntry, continousPoint);
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
		TimestampsToReturn timestampsToReturn,
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
				timestampsToReturn,
				maxResultEntries
			);
		}

		// check if read access entry exists
		FileReadEntry::Map::iterator it;
		it = fileReadEntryMap_.find(valueReadContext.valueName_);
		if (it != fileReadEntryMap_.end()) {
			FileReadEntry::SPtr fileReadEntry = it->second;
			valueReadContext.fileReadEntry_ = fileReadEntry;
			return readInitial(
				valueReadContext,
				continousPoint,
				fileReadEntry,
				from,
				to,
				dataValueVec,
				timestampsToReturn,
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
			timestampsToReturn,
			maxResultEntries
		);
	}

	bool
	FileReadManager::readNext(
		ValueReadContinousPoint& continousPoint,
		FileReadEntry::SPtr& fileReadEntry,
		OpcUaDataValue::Vec& dataValueVec,
		TimestampsToReturn timestampsToReturn,
		uint32_t maxResultEntries
	)
	{
		bool rc = fileReadEntry->readNext(
			dataValueVec,
			timestampsToReturn,
			maxResultEntries
		);
		if (!rc) return false;

		if (maxConcurrentValues_ != 0) {
			uint32_t ageCounter = fileReadEntry->ageCounter();

			if (ageCounter >= ageCounter_) {
				if (verbose_) {
					Log(Debug, "FileReadManager - aging")
					    .parameter("ContinousPoint", fileReadEntry->continousPoint());
				}

				fileReadEntry->lastAccessTime(boost::posix_time::microsec_clock::local_time());
				fileReadEntry->ageCounter(0);
				fileReadEntry->remove();
				continousPointList_.pushAfter(*fileReadEntry.get());
			}
		}

		if (!fileReadEntry->maxResultEntriesReached() || dataValueVec.size() == 0) {
			continousPoint.readComplete_ = true;
			deleteContinousPoint(fileReadEntry.get());
		}

		// delete continous point
		else if (continousPoint.command_ == ValueReadContinousPoint::LastAccess) {
			continousPoint.readComplete_ = true;
			deleteContinousPoint(fileReadEntry.get());
		}

		return true;
	}

	bool
	FileReadManager::readNext(
		ValueReadContinousPoint& continousPoint,
		OpcUaDataValue::Vec& dataValueVec,
		TimestampsToReturn timestampsToReturn,
		uint32_t maxResultEntries
	)
	{
		continousPoint.readComplete_ = false;
		continousPoint.error_ = false;

		// check if read continout point exists
		FileReadEntry::Map::iterator it;
		it = continousPointMap_.find(continousPoint.continousPoint_);
		if (it == continousPointMap_.end()) {
			Log(Error, "continous point do not exist")
				.parameter("ContinousPoint", continousPoint.continousPoint_);
			return false;
		}

		if (continousPoint.command_ == ValueReadContinousPoint::Delete) {
			FileReadEntry::SPtr fileReadEntry = it->second;
			deleteContinousPoint(fileReadEntry.get());
			continousPoint.readComplete_ = true;
			return true;
		}

		return readNext(
			continousPoint,
			it->second,
			dataValueVec,
			timestampsToReturn,
			maxResultEntries
		);
	}

	void
	FileReadManager::logFileReadEntryList(uint32_t maxEntries, bool inverse)
	{
		uint32_t counter = 0;
		Log(Debug, "file entry list")
		    .parameter("Inverse", inverse);

		if (inverse) {
			DoublyLinkedList* listEntry = fileReadEntryList_.last();
			while (listEntry != (&fileReadEntryList_)) {
				FileReadEntry* fileReadEntry = dynamic_cast<FileReadEntry*>(listEntry);
				Log(Debug, "--")
				    .parameter("ValueName", fileReadEntry->valueName());
				listEntry = listEntry->last();

				counter++;
				if (maxEntries != 0 && counter > maxEntries) {
					Log(Debug, "continous point list skip entries ...");
					return;
				}
			}
		}
		else {
			DoublyLinkedList* listEntry = fileReadEntryList_.next();
			while (listEntry != (&fileReadEntryList_)) {
				FileReadEntry* fileReadEntry = dynamic_cast<FileReadEntry*>(listEntry);
				Log(Debug, "--")
				    .parameter("ValueName", fileReadEntry->valueName());
				listEntry = listEntry->next();

				counter++;
				if (maxEntries != 0 && counter > maxEntries) {
					Log(Debug, "continous point list skip entries ...");
					return;
				}
			}
		}
	}

	void
	FileReadManager::logContinousPointList(uint32_t maxEntries, bool inverse)
	{
		uint32_t counter = 0;
		Log(Debug, "continous point list")
			.parameter("Inverse", inverse);

		if (inverse) {
			DoublyLinkedList* listEntry = continousPointList_.last();
			while (listEntry != (&continousPointList_)) {
				FileReadEntry* fileReadEntry = dynamic_cast<FileReadEntry*>(listEntry);
				Log(Debug, "--")
				    .parameter("ValueName", fileReadEntry->valueName());
				listEntry = listEntry->last();

				counter++;
				if (maxEntries != 0 && counter > maxEntries) {
					Log(Debug, "continous point list skip entries ...");
					return;
				}
			}
		}
		else {
			DoublyLinkedList* listEntry = continousPointList_.next();
			while (listEntry != (&continousPointList_)) {
				FileReadEntry* fileReadEntry = dynamic_cast<FileReadEntry*>(listEntry);
				Log(Debug, "--")
				    .parameter("ValueName", fileReadEntry->valueName());
				listEntry = listEntry->next();

				counter++;
				if (maxEntries != 0 && counter > maxEntries) {
					Log(Debug, "continous point list skip entries ...");
					return;
				}
			}
		}
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


	uint32_t
	FileReadManager::timeoutHandler(void)
	{
		uint32_t deletedEntries = 0;
		boost::posix_time::ptime timeout = boost::posix_time::microsec_clock::local_time() - boost::posix_time::milliseconds(continousPointIdleTimeout_);

		for (uint32_t idx=0; idx<maxDeleteTimeoutEntries_; idx++) {
			if (continousPointMap_.size() == 0) return deletedEntries;

			FileReadEntry* fileReadEntry = dynamic_cast<FileReadEntry*>(continousPointList_.last());
			if (fileReadEntry->lastAccessTime() < timeout) {
				deleteContinousPoint(fileReadEntry, true);
				deletedEntries++;
			}
			else {
				return deletedEntries;
			}
		}

		return deletedEntries;
	}

	bool
	FileReadManager::createContinousPoint(FileReadEntry::SPtr& fileReadEntry, ValueReadContinousPoint* continousPoint)
	{
		// check whether there is enough free resources available
		if (maxContinousPoints_ != 0 && continousPointMap_.size() >= maxContinousPoints_ ) {
			if (timeoutHandler() == 0) {
				Log(Warning, "no free continous points available")
					.parameter("ContinousPoint", fileReadEntry->valueName());
				continousPoint->error_ = true;
				return false;
			}
		}

		// create new continous point
		std::stringstream continousPointString;
		continousPointId_++;
		continousPointString << fileReadEntry->valueName() << "_" << std::hex << continousPointId_;
		continousPoint->continousPoint_ = continousPointString.str();
		continousPoint->readComplete_ = false;

		fileReadEntry->continousPoint(continousPoint->continousPoint_);
		fileReadEntry->ageCounter(0);
		fileReadEntry->lastAccessTime(boost::posix_time::microsec_clock::local_time());
		continousPointMap_.insert(std::make_pair(continousPoint->continousPoint_, fileReadEntry));
		continousPointList_.pushAfter(*fileReadEntry);

		if (verbose_) {
			Log(Debug, "FileReadManager - create continous point")
			    .parameter("ContinousPoint", continousPoint->continousPoint_);
		}

		return true;
	}

	bool
	FileReadManager::deleteContinousPoint(FileReadEntry* fileReadEntry, bool timeout)
	{
		if (verbose_) {
			Log(Debug, "FileReadManager - delete continous point")
			    .parameter("ContinoutPoint", fileReadEntry->continousPoint())
			    .parameter("Timeout", timeout);
		}

		deletedContinousPoint_ = fileReadEntry->continousPoint();
		deletedValueName_ = fileReadEntry->valueName();

		FileReadEntry::Map::iterator it;
		it = continousPointMap_.find(fileReadEntry->continousPoint());
		if (it == continousPointMap_.end()) return false;
		it->second->remove();
		continousPointMap_.erase(it);
		return true;
	}

}
