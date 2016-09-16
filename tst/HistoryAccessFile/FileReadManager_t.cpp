#include "unittest.h"
#include "OpcUaHistory/HistoryAccessFile/FileReadManager.h"
#include "OpcUaHistory/HistoryAccessFile/FileWriteManager.h"

using namespace OpcUaHistory;

BOOST_AUTO_TEST_SUITE(FileReadManager_)

BOOST_AUTO_TEST_CASE(FileReadManager_)
{
	std::cout << "FileReadManager_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(FileReadManager_init)
{
	FileReadManager fileWriteManager;
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// readinitial
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileReadManager_testdata)
{
	boost::filesystem::path path("TestFolder");
	boost::filesystem::remove_all(path);
	boost::filesystem::create_directory("TestFolder");

	FileWriteManager fileWriteManager;

	fileWriteManager.maxDataFolderInValueFolder(10);
	fileWriteManager.maxDataFilesInDataFolder(10);
	fileWriteManager.maxEntriesInDataFile(60);
	fileWriteManager.baseFolder("TestFolder");
	fileWriteManager.verbose(true);

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T100000.000000000");

	for (uint32_t idx=0; idx<3600; idx++) {

		for (uint32_t val=0; val<10; val++) {
			std::stringstream valueName;
			valueName << "MyValue" << val;

			OpcUaDataValue dataValue;
			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant((uint32_t)idx);
			BOOST_REQUIRE(fileWriteManager.write(valueName.str(), dataValue) == true);
		}

		time += boost::posix_time::seconds(1);
	}
}

BOOST_AUTO_TEST_CASE(FileReadManager_readInitial)
{
	FileReadManager fileReadManager;

	fileReadManager.verbose(true);
	fileReadManager.baseFolder("TestFolder");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T110000.000000000"));

	ValueReadContext valueReadContext;
	valueReadContext.valueName_ = "MyValue0";

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext, nullptr, from, to, dataValueVec) == true);
	BOOST_REQUIRE(dataValueVec.size() == 3600);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext, nullptr, from, to, dataValueVec) == true);
	BOOST_REQUIRE(dataValueVec.size() == 3600);
}

BOOST_AUTO_TEST_SUITE_END()
