#include "unittest.h"
#include "OpcUaHistory/HistoryAccessFile/FileOutEntry.h"

using namespace OpcUaHistory;

BOOST_AUTO_TEST_SUITE(FileOutEntry_)

BOOST_AUTO_TEST_CASE(FileOutEntry_)
{
	std::cout << "FileOutEntry_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(FileOutEntry_init)
{
	FileOutEntry fileOutEntry;
}

#if 0
BOOST_AUTO_TEST_CASE(FileOutEntry_out_one_value)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileOutEntry fileOutEntry;

	fileOutEntry.maxDataFolderInValueFolder(5);
	fileOutEntry.maxDataFilesInDataFolder(5);
	fileOutEntry.maxEntriesInDataFile(5);
	fileOutEntry.valueName("TestVariable");
	fileOutEntry.baseFolder("./");

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");
	OpcUaDataValue dataValue;
	dataValue.sourceTimestamp(time);
	dataValue.serverTimestamp(time);
	dataValue.statusCode(Success);
	dataValue.variant()->variant((uint32_t)123);
	BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
}

BOOST_AUTO_TEST_CASE(FileOutEntry_out_10)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileOutEntry fileOutEntry;

	fileOutEntry.maxDataFolderInValueFolder(5);
	fileOutEntry.maxDataFilesInDataFolder(5);
	fileOutEntry.maxEntriesInDataFile(5);
	fileOutEntry.valueName("TestVariable");
	fileOutEntry.baseFolder("./");

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");

	for (uint32_t idx=0; idx<10; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(1);
	}
}

BOOST_AUTO_TEST_CASE(FileOutEntry_out_100)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileOutEntry fileOutEntry;

	fileOutEntry.maxDataFolderInValueFolder(5);
	fileOutEntry.maxDataFilesInDataFolder(5);
	fileOutEntry.maxEntriesInDataFile(5);
	fileOutEntry.valueName("TestVariable");
	fileOutEntry.baseFolder("./");

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");

	for (uint32_t idx=0; idx<100; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(1);
	}
}
#endif

BOOST_AUTO_TEST_SUITE_END()
