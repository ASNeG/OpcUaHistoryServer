/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_FileReadManager_h__
#define __OpcUaHistory_FileReadManager_h__

#include "OpcUaHistory/HistoryAccessFile/FileReadEntry.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{
	class ValueReadContext
	{
	  public:
		ValueReadContext(void);
		~ValueReadContext(void);

		std::string valueName_;
		FileReadEntry::WPtr fileReadEntry_;
		std::string continousPoint_;
	};


	class FileReadManager
	{
	  public:
		FileReadManager(void);
		~FileReadManager(void);

		void verbose(bool verbose);
		void baseFolder(const boost::filesystem::path& baseFolder);
		void maxConcurrentValues(uint32_t maxConcurrentValues);
		uint32_t actConcurrentValues(void);
		void ageCounter(uint32_t ageCounter);
		uint32_t ageCounter(void);
		std::string deletedValueName(void);

		bool readInitial(
			ValueReadContext& valueReadContext,
			OpcUaDateTime& from,
			OpcUaDateTime& to,
			OpcUaDataValue::Vec& dataValueVec,
			uint32_t maxResultEntries = 0
		);
		bool readNext(
			ValueReadContext& valueReadConext,
			OpcUaDataValue::Vec& dataValueVec,
			uint32_t maxResultEntries = 0
		);

	  private:
		bool readInitial(
			ValueReadContext& valueReadContext,
			FileReadEntry::SPtr& fileReadEntry,
			OpcUaDateTime& from,
			OpcUaDateTime& to,
			OpcUaDataValue::Vec& dataValueVec,
			uint32_t maxResultEntries = 0
		);

		bool createFileReadEntry(const std::string& valueName);
		bool deleteFileReadEntry(FileReadEntry* fileReadEntry, bool aging=false);

		bool verbose_;
		boost::filesystem::path baseFolder_;
		uint32_t maxConcurrentValues_;
		uint32_t ageCounter_;

		FileReadEntry::Map fileReadEntryMap_;
		std::string deletedValueName_;
	};

}

#endif
