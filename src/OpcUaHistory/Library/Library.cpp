/*
   Copyright 2015 Kai Huebl (kai@huebl-sgh.de)

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

#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackServer/ServiceSetApplication/ApplicationService.h"
#include "OpcUaStackServer/ServiceSetApplication/NodeReferenceApplication.h"
#include "OpcUaHistory/Library/Library.h"
#include "OpcUaHistory/HistoryAccessFile/FileWriteEntry.h"
#include <iostream>

namespace OpcUaHistory
{

	Library::Library(void)
	: ApplicationIf()
	{
		Log(Debug, "Library::Library");
	}

	Library::~Library(void)
	{
		Log(Debug, "Library::~Library");
	}

	bool
	Library::startup(void)
	{
		Log(Debug, "Library::startup");

		FileWriteEntry fileOutEntry;
		fileOutEntry.baseFolder(boost::filesystem::path("./"));
		fileOutEntry.valueName("MyValue");
		fileOutEntry.maxDataFolderInValueFolder(5);
		fileOutEntry.maxDataFilesInDataFolder(5);
		fileOutEntry.maxEntriesInDataFile(5);

		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		for (uint32_t idx=0; idx<20; idx++) {
			OpcUaDataValue dataValue;
			boost::posix_time::ptime time = now + boost::posix_time::seconds(idx);

			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant(idx);

			fileOutEntry.write(dataValue);
		}

		return true;
	}

	bool
	Library::shutdown(void)
	{
		Log(Debug, "Library::shutdown");
		return true;
	}

}

extern "C" DLLEXPORT void  init(ApplicationIf** applicationIf) {
    *applicationIf = new OpcUaHistory::Library();
}

