/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)

 */

#ifndef __OpcUaHistory_FileReadEntry_h__
#define __OpcUaHistory_FileReadEntry_h__

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/weak_ptr.hpp>
#include <list>
#include "OpcUaStackCore/Container/DoublyLinkedList.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileReadEntry
	: public DoublyLinkedList
	{
	  public:
		typedef boost::shared_ptr<FileReadEntry> SPtr;
		typedef std::map<std::string, FileReadEntry::SPtr> Map;
		typedef boost::weak_ptr<FileReadEntry> WPtr;

		FileReadEntry(void);
		virtual ~FileReadEntry(void);

		void verbose(bool verbose);
		bool verbose(void);

		void valueName(const std::string& valueName);
		std::string valueName(void);
		void baseFolder(const boost::filesystem::path& baseFolder);
		void ageCounter(uint32_t ageCounter);
		uint32_t ageCounter(void);

		void dateTimeFrom(OpcUaDateTime& from);
		void dateTimeTo(OpcUaDateTime& to);
		bool readInitial(OpcUaDataValue::Vec& dataValueVec, uint32_t maxResultEntries = 0);
		bool readNext(OpcUaDataValue::Vec& dataValueVec, uint32_t maxResultEntries = 0);

		bool maxResultEntriesReached(void);

	  private:
		bool getDataFolderList(void);
		bool getDataFileList(void);
		bool skipEntry(uint32_t recordSize);

		// configuration parameters
		bool verbose_;
		std::string valueName_;
		boost::filesystem::path baseFolder_;
		boost::filesystem::path valueFolder_;
		boost::filesystem::path dataFolder_;
		boost::filesystem::path dataFile_;
		std::list<std::string> dataFolderList_;
		std::list<std::string> dataFileList_;

		uint32_t ageCounter_;
		boost::filesystem::ifstream ifs_;

		bool maxResultEntriesReached_;
		OpcUaDateTime from_;
		OpcUaDateTime to_;
	};

}

#endif
