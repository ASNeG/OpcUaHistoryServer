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
	FileReadManager::ageCounter(uint32_t ageCounter)
	{
		ageCounter_ = ageCounter;
	}

	uint32_t
	FileReadManager::ageCounter(void)
	{
		return ageCounter_;
	}

	bool
	FileReadManager::readInitial(
		const std::string& valueName,
		OpcUaDateTime& from,
		OpcUaDateTime& to,
		OpcUaDataValue::Vec& dataValueVec,
		uint32_t maxResultEntries
	)
	{
		// FIXME: todo
		return false;
	}

	bool
	FileReadManager::readInitial(
		FileReadEntry::SPtr& fileReadEntry,
		OpcUaDateTime& from,
		OpcUaDateTime& to,
		OpcUaDataValue::Vec& dataValueVec,
		uint32_t maxResultEntries
	)
	{
		// FIXME: todo
		return false;
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
				fileReadEntry,
				from,
				to,
				dataValueVec,
				maxResultEntries
			);
		}

#if 0
		// check if read access entry exists
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
#endif
		return false;
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
		// FIXME: todo
		return false;
	}

	bool
	FileReadManager::deleteFileReadEntry(FileReadEntry* fileReadEntry)
	{
		// FIXME: todo
		return false;
	}

}
