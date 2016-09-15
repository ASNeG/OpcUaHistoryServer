#include "unittest.h"
#include "OpcUaHistory/HistoryAccessFile/FileWriteManager.h"

using namespace OpcUaHistory;

BOOST_AUTO_TEST_SUITE(FileWriteManager_)

BOOST_AUTO_TEST_CASE(FileWriteManager_)
{
	std::cout << "FileWriteManager_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(FileWriteManager_init)
{
	FileWriteManager fileWriteManager;
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// write data via value name
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileWriteManager_write_1_value_with_value_name)
{
	boost::filesystem::path path("TestFolder");
	boost::filesystem::remove_all(path);
	boost::filesystem::create_directory("TestFolder");

	FileWriteManager fileWriteManager;

	fileWriteManager.maxDataFolderInValueFolder(5);
	fileWriteManager.maxDataFilesInDataFolder(5);
	fileWriteManager.maxEntriesInDataFile(5);
	fileWriteManager.baseFolder("TestFolder");
	fileWriteManager.verbose(true);

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");
	OpcUaDataValue dataValue;
	dataValue.sourceTimestamp(time);
	dataValue.serverTimestamp(time);
	dataValue.statusCode(Success);
	dataValue.variant()->variant((uint32_t)123);
	BOOST_REQUIRE(fileWriteManager.write("MyValue", dataValue) == true);

	time += boost::posix_time::seconds(1);
	dataValue.sourceTimestamp(time);
	dataValue.serverTimestamp(time);
	dataValue.statusCode(Success);
	dataValue.variant()->variant((uint32_t)123);
	BOOST_REQUIRE(fileWriteManager.write("MyValue", dataValue) == true);

	BOOST_REQUIRE(fileWriteManager.actConcurrentValues() == 1);
}

BOOST_AUTO_TEST_CASE(FileWriteManager_write_10_value_with_value_name)
{
	boost::filesystem::path path("TestFolder");
	boost::filesystem::remove_all(path);
	boost::filesystem::create_directory("TestFolder");

	FileWriteManager fileWriteManager;

	fileWriteManager.maxDataFolderInValueFolder(5);
	fileWriteManager.maxDataFilesInDataFolder(5);
	fileWriteManager.maxEntriesInDataFile(5);
	fileWriteManager.baseFolder("TestFolder");
	fileWriteManager.verbose(true);

	for (uint32_t idx=0; idx<10; idx++) {
		std::stringstream valueName;
		valueName << "MyValue" << idx;

		boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)123);
		BOOST_REQUIRE(fileWriteManager.write(valueName.str(), dataValue) == true);

		time += boost::posix_time::seconds(1);
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)123);
		BOOST_REQUIRE(fileWriteManager.write(valueName.str(), dataValue) == true);
	}

	BOOST_REQUIRE(fileWriteManager.actConcurrentValues() == 10);
}

BOOST_AUTO_TEST_CASE(FileWriteManager_write_10_value_with_value_name_10_times)
{
	boost::filesystem::path path("TestFolder");
	boost::filesystem::remove_all(path);
	boost::filesystem::create_directory("TestFolder");

	FileWriteManager fileWriteManager;

	fileWriteManager.maxDataFolderInValueFolder(5);
	fileWriteManager.maxDataFilesInDataFolder(5);
	fileWriteManager.maxEntriesInDataFile(5);
	fileWriteManager.baseFolder("TestFolder");
	fileWriteManager.verbose(true);

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");
	for (uint32_t idx=0; idx<10; idx++) {

		time += boost::posix_time::seconds(1);

		for (uint32_t j=0; j<10; j++) {

			std::stringstream valueName;
			valueName << "MyValue" << j;

			OpcUaDataValue dataValue;
			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant((uint32_t)123);
			BOOST_REQUIRE(fileWriteManager.write(valueName.str(), dataValue) == true);
		}
	}

	BOOST_REQUIRE(fileWriteManager.actConcurrentValues() == 10);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// write data via context
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileWriteManager_write_1_value_with_context)
{
	boost::filesystem::path path("TestFolder");
	boost::filesystem::remove_all(path);
	boost::filesystem::create_directory("TestFolder");

	FileWriteManager fileWriteManager;
	ValueWriteContext valueWriteContext;

	fileWriteManager.maxDataFolderInValueFolder(5);
	fileWriteManager.maxDataFilesInDataFolder(5);
	fileWriteManager.maxEntriesInDataFile(5);
	fileWriteManager.baseFolder("TestFolder");
	fileWriteManager.verbose(true);

	valueWriteContext.valueName_ = "MyValue";

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");
	OpcUaDataValue dataValue;
	dataValue.sourceTimestamp(time);
	dataValue.serverTimestamp(time);
	dataValue.statusCode(Success);
	dataValue.variant()->variant((uint32_t)123);
	BOOST_REQUIRE(fileWriteManager.write(valueWriteContext, dataValue) == true);

	time += boost::posix_time::seconds(1);
	dataValue.sourceTimestamp(time);
	dataValue.serverTimestamp(time);
	dataValue.statusCode(Success);
	dataValue.variant()->variant((uint32_t)123);
	BOOST_REQUIRE(fileWriteManager.write(valueWriteContext, dataValue) == true);

	BOOST_REQUIRE(fileWriteManager.actConcurrentValues() == 1);
}

BOOST_AUTO_TEST_CASE(FileWriteManager_write_10_value_with_context)
{
	boost::filesystem::path path("TestFolder");
	boost::filesystem::remove_all(path);
	boost::filesystem::create_directory("TestFolder");

	FileWriteManager fileWriteManager;
	ValueWriteContext valueWriteContext[10];

	fileWriteManager.maxDataFolderInValueFolder(5);
	fileWriteManager.maxDataFilesInDataFolder(5);
	fileWriteManager.maxEntriesInDataFile(5);
	fileWriteManager.baseFolder("TestFolder");
	fileWriteManager.verbose(true);

	for (uint32_t idx=0; idx<10; idx++) {
		std::stringstream valueName;
		valueName << "MyValue" << idx;
		valueWriteContext[idx].valueName_ = valueName.str();

		boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)123);
		BOOST_REQUIRE(fileWriteManager.write(valueWriteContext[idx], dataValue) == true);

		time += boost::posix_time::seconds(1);
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)123);
		BOOST_REQUIRE(fileWriteManager.write(valueWriteContext[idx], dataValue) == true);
	}

	BOOST_REQUIRE(fileWriteManager.actConcurrentValues() == 10);
}

BOOST_AUTO_TEST_CASE(FileWriteManager_write_10_value_with_context_10_times)
{
	boost::filesystem::path path("TestFolder");
	boost::filesystem::remove_all(path);
	boost::filesystem::create_directory("TestFolder");

	FileWriteManager fileWriteManager;
	ValueWriteContext valueWriteContext[10];

	fileWriteManager.maxDataFolderInValueFolder(5);
	fileWriteManager.maxDataFilesInDataFolder(5);
	fileWriteManager.maxEntriesInDataFile(5);
	fileWriteManager.baseFolder("TestFolder");
	fileWriteManager.verbose(true);

	for (uint32_t idx=0; idx<10; idx++) {
		std::stringstream valueName;
		valueName << "MyValue" << idx;
		valueWriteContext[idx].valueName_ = valueName.str();
	}

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");
	for (uint32_t idx=0; idx<10; idx++) {

		time += boost::posix_time::seconds(1);

		for (uint32_t j=0; j<10; j++) {
			OpcUaDataValue dataValue;
			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant((uint32_t)123);
			BOOST_REQUIRE(fileWriteManager.write(valueWriteContext[j], dataValue) == true);
		}
	}

	BOOST_REQUIRE(fileWriteManager.actConcurrentValues() == 10);
}

BOOST_AUTO_TEST_SUITE_END()
