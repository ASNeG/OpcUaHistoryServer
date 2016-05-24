/*
   Copyright 2016 Kai Huebl (kai@huebl-sgh.de)
   Alle Rechte vorbehalten.

   Autor: Kai Huebl (kai@huebl-sgh.de)

 */

#ifndef __OpcUaHistory_FileInEntry_h__
#define __OpcUaHistory_FileInEntry_h__

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <list>
#include "OpcUaStackCore/Container/DoublyLinkedList.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileInEntry
	: public DoublyLinkedList
	{
	  public:
		FileInEntry(void);
		virtual ~FileInEntry(void);

		void valueName(const std::string& valueName);
		void baseFolder(const boost::filesystem::path& baseFolder);

		bool read(OpcUaDateTime& from, OpcUaDateTime& to);

	  private:
		bool getDataFolderList(OpcUaDateTime& from, OpcUaDateTime& to);
		bool getDataFileList(OpcUaDateTime& from, OpcUaDateTime& to);

		// configuration parameters
		std::string valueName_;
		boost::filesystem::path baseFolder_;
		boost::filesystem::path valueFolder_;
		boost::filesystem::path dataFolder_;
		std::list<std::string> dataFolderList_;
		std::list<std::string> dataFileList_;
	};

}

#endif
