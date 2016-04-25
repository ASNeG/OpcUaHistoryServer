/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#ifndef __OpcUaHistory_FileOutManager_h__
#define __OpcUaHistory_FileOutManager_h__

#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"
#include "OpcUaHistory/HistoryAccessFile/FileOut.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileOutManager
	{
	  public:
		FileOutManager(void);
		~FileOutManager(void);

		void basePath(const std::string& basePath);
		std::string& basePath(void);
		void maxNumberFilesPerDirectory(uint32_t maxNumberFilesPerDirectory);
		uint32_t maxNumberFilesPerDirectory(void);
		void maxFileSize(uint32_t);
		uint32_t maxFileSize(void);
		void maxOpenFiles(uint32_t maxOpenFiles);
		uint32_t maxOpenFiles(void);

		FileAccess::Result write(
			const std::string& name,
			OpcUaDataValue& dataValue
		);

	  private:
		FileOut* createFileOutEntry(const std::string& name);
		void removeFileOutEntry(FileOut* fileOut);
		void updateFileOutEntry(FileOut* fileOut);

		// configuration parameters
		std::string basePath_;
		uint32_t maxNumberFilesPerDirectory_;
		uint32_t maxFileSize_;
		uint32_t maxOpenFiles_;

		// management parameter
		FileOut::Map fileOutMap_;
		DoublyLinkedList fileOutList_;

	};

}

#endif
