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
	};

	class ValueReadContinousPoint
	{
	  public:
		ValueReadContinousPoint(void);
		~ValueReadContinousPoint(void);

		typedef enum {
			NormalAccess,
			LastAccess,
			Delete
		} Command;

		std::string continousPoint_;
		Command command_;
		bool readComplete_;
		bool error_;
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

		void maxContinousPoint(uint32_t maxContinousPoints);
		void continousPointIdleTimeout(uint32_t continousPointIdleTimeout);
		std::string deletedContinousPoint(void);
		void maxDeleteTimeoutEntries(uint32_t maxDeleteTimeoutEntries);

		uint32_t timeoutHandler(void);

		bool readInitial(
			ValueReadContext& valueReadContext,
			ValueReadContinousPoint* continousPoint,
			OpcUaDateTime& from,
			OpcUaDateTime& to,
			OpcUaDataValue::Vec& dataValueVec,
			uint32_t maxResultEntries = 0
		);
		bool readNext(
			ValueReadContinousPoint& continousPoint,
			OpcUaDataValue::Vec& dataValueVec,
			uint32_t maxResultEntries = 0
		);

	  private:
		bool readInitial(
			ValueReadContext& valueReadContext,
			ValueReadContinousPoint* continousPoint,
			FileReadEntry::SPtr& fileReadEntry,
			OpcUaDateTime& from,
			OpcUaDateTime& to,
			OpcUaDataValue::Vec& dataValueVec,
			uint32_t maxResultEntries = 0
		);
		bool readNext(
			ValueReadContinousPoint& continousPoint,
			FileReadEntry::SPtr& fileReadEntry,
			OpcUaDataValue::Vec& dataValueVec,
			uint32_t maxResultEntries = 0
		);

		bool createFileReadEntry(const std::string& valueName);
		bool deleteFileReadEntry(FileReadEntry* fileReadEntry, bool aging=false);
		bool createContinousPoint(FileReadEntry::SPtr& fileReadEntry, ValueReadContinousPoint* continousPoint);
		bool deleteContinousPoint(FileReadEntry* fileReadEntry, bool timeout=false);

		bool verbose_;
		boost::filesystem::path baseFolder_;
		uint32_t maxConcurrentValues_;
		uint32_t ageCounter_;
		uint32_t continousPointId_;

		uint32_t maxContinousPoints_;
		uint32_t continousPointIdleTimeout_;
		uint32_t maxDeleteTimeoutEntries_;

		FileReadEntry::Map fileReadEntryMap_;
		DoublyLinkedList fileReadEntryList_;
		std::string deletedValueName_;

		FileReadEntry::Map continousPointMap_;
		DoublyLinkedList continousPointList_;
		std::string deletedContinousPoint_;
	};

}

#endif
