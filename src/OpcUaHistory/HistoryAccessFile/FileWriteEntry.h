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

#ifndef __OpcUaHistory_FileWriteEntry_h__
#define __OpcUaHistory_FileWriteEntry_h__

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/weak_ptr.hpp>
#include "OpcUaStackCore/Container/DoublyLinkedList.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDataValue.h"

using namespace OpcUaStackCore;

namespace OpcUaHistory
{

	class FileWriteEntry
	: public DoublyLinkedList
	{
	  public:
		typedef boost::shared_ptr<FileWriteEntry> SPtr;
		typedef boost::weak_ptr<FileWriteEntry> WPtr;
		typedef std::map<std::string, FileWriteEntry::SPtr> Map;

		FileWriteEntry(void);
		virtual ~FileWriteEntry(void);

		void maxDataFolderInValueFolder(uint16_t maxDataFolderInValueFolder);
		void maxDataFilesInDataFolder(uint16_t maxDataFilesInDataFolder);
		void maxEntriesInDataFile(uint16_t maxEntriesInDataFile);
		void valueName(const std::string& valueName);
		std::string valueName(void);
		void baseFolder(const boost::filesystem::path& baseFolder);
		void ageCounter(uint32_t ageCounter);
		uint32_t ageCounter(void);

		bool write(OpcUaDataValue& dataValue);

	  private:
		bool createValueFolder(void);
		void getNewestDataFolder(void);
		bool createDataFolder(OpcUaDataValue& dataValue);
		void getNewestDataFile(void);
		bool createDataFile(OpcUaDataValue& dataValue);
		bool openDataFile(OpcUaDataValue& dataValue);
		bool writeData(OpcUaDataValue& dataValue);

		// configuration parameters
		std::string valueName_;
		boost::filesystem::path baseFolder_;
		boost::filesystem::path valueFolder_;
		boost::filesystem::path dataFolder_;
		boost::filesystem::path dataFile_;
		uint16_t maxDataFolderInValueFolder_;
		uint16_t maxDataFilesInDataFolder_;
		uint16_t maxEntriesInDataFile_;

		uint32_t ageCounter_;
		uint16_t countDataFolderInValueFolder_;
		uint16_t countDataFilesInDataFolder_;
		uint16_t countEntriesInDataFile_;

		boost::filesystem::ofstream ofs_;
	};

}

#endif
