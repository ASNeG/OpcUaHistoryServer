#include "unittest.h"
#include "OpcUaHistory/HistoryAccessFile/FileOutEntry.h"

using namespace OpcUaHistory;

BOOST_AUTO_TEST_SUITE(FileEntry_)

BOOST_AUTO_TEST_CASE(FileEntry_)
{
	std::cout << "FileEntry_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(FileEntry_write_test_data)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileOutEntry fileOutEntry;

	fileOutEntry.maxDataFolderInValueFolder(100);
	fileOutEntry.maxDataFilesInDataFolder(100);
	fileOutEntry.maxEntriesInDataFile(300);
	fileOutEntry.valueName("TestVariable");
	fileOutEntry.baseFolder("./");

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T100000.000000000");

	for (uint32_t idx=0; idx<3600; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(1);
	}
}

BOOST_AUTO_TEST_SUITE_END()
