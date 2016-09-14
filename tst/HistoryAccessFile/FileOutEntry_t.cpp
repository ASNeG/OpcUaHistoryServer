#include "unittest.h"
#include "OpcUaHistory/HistoryAccessFile/FileOutEntry.h"
#include "OpcUaHistory/HistoryAccessFile/FileInEntry.h"

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

BOOST_AUTO_TEST_CASE(FileEntry_several_write_objects1)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T100000.000000000");

	for (uint32_t idx=0; idx<2; idx++) {
		FileOutEntry fileOutEntry;

		fileOutEntry.maxDataFolderInValueFolder(100);
		fileOutEntry.maxDataFilesInDataFolder(10);
		fileOutEntry.maxEntriesInDataFile(6);
		fileOutEntry.valueName("TestVariable");
		fileOutEntry.baseFolder("./");

		for (uint32_t idx=0; idx<5; idx++) {
			OpcUaDataValue dataValue;
			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant((uint32_t)idx);
			BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
			time += boost::posix_time::seconds(10);
		}
	}

	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 10);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 0);
}

BOOST_AUTO_TEST_CASE(FileEntry_several_write_objects2)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T100000.000000000");

	FileOutEntry fileOutEntry;

	fileOutEntry.maxDataFolderInValueFolder(100);
	fileOutEntry.maxDataFilesInDataFolder(10);
	fileOutEntry.maxEntriesInDataFile(6);
	fileOutEntry.valueName("TestVariable");
	fileOutEntry.baseFolder("./");

	for (uint32_t idx=0; idx<5; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(10);
	}

	{
		FileInEntry fileInEntry;

		fileInEntry.verbose(true);
		fileInEntry.valueName("TestVariable");
		fileInEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileInEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileInEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
		std::cout << "Size=" << dataValueVec.size() << std::endl;
		BOOST_REQUIRE(dataValueVec.size() == 5);

		uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
		BOOST_REQUIRE(number == 0);
	}

	for (uint32_t idx=0; idx<5; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(10);
	}

	{
		FileInEntry fileInEntry;

		fileInEntry.verbose(true);
		fileInEntry.valueName("TestVariable");
		fileInEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileInEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileInEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
		std::cout << "Size=" << dataValueVec.size() << std::endl;
		BOOST_REQUIRE(dataValueVec.size() == 10);

		uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
		BOOST_REQUIRE(number == 0);
	}

	FileOutEntry fileOutEntry1;

	fileOutEntry1.maxDataFolderInValueFolder(100);
	fileOutEntry1.maxDataFilesInDataFolder(10);
	fileOutEntry1.maxEntriesInDataFile(6);
	fileOutEntry1.valueName("TestVariable");
	fileOutEntry1.baseFolder("./");

	for (uint32_t idx=0; idx<5; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileOutEntry1.write(dataValue) == true);
		time += boost::posix_time::seconds(10);
	}

	{
		FileInEntry fileInEntry;

		fileInEntry.verbose(true);
		fileInEntry.valueName("TestVariable");
		fileInEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileInEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileInEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
		std::cout << "Size=" << dataValueVec.size() << std::endl;
		BOOST_REQUIRE(dataValueVec.size() == 15);

		uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
		BOOST_REQUIRE(number == 0);
	}
}

BOOST_AUTO_TEST_CASE(FileEntry_several_write_objects3)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T100000.000000000");

	{
		FileOutEntry fileOutEntry;

		fileOutEntry.maxDataFolderInValueFolder(100);
		fileOutEntry.maxDataFilesInDataFolder(10);
		fileOutEntry.maxEntriesInDataFile(6);
		fileOutEntry.valueName("TestVariable");
		fileOutEntry.baseFolder("./");

		for (uint32_t idx=0; idx<5; idx++) {
			OpcUaDataValue dataValue;
			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant((uint32_t)idx);
			BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
			time += boost::posix_time::seconds(10);
		}
	}

	{
		FileInEntry fileInEntry;

		fileInEntry.verbose(true);
		fileInEntry.valueName("TestVariable");
		fileInEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileInEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileInEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
		std::cout << "Size=" << dataValueVec.size() << std::endl;
		BOOST_REQUIRE(dataValueVec.size() == 5);

		uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
		BOOST_REQUIRE(number == 0);
	}

	{
		FileOutEntry fileOutEntry;

		fileOutEntry.maxDataFolderInValueFolder(100);
		fileOutEntry.maxDataFilesInDataFolder(10);
		fileOutEntry.maxEntriesInDataFile(6);
		fileOutEntry.valueName("TestVariable");
		fileOutEntry.baseFolder("./");

		for (uint32_t idx=0; idx<5; idx++) {
			OpcUaDataValue dataValue;
			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant((uint32_t)idx);
			BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
			time += boost::posix_time::seconds(10);
		}
	}

	{
		FileInEntry fileInEntry;

		fileInEntry.verbose(true);
		fileInEntry.valueName("TestVariable");
		fileInEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileInEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileInEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
		std::cout << "Size=" << dataValueVec.size() << std::endl;
		BOOST_REQUIRE(dataValueVec.size() == 10);

		uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
		BOOST_REQUIRE(number == 0);
	}

}

BOOST_AUTO_TEST_SUITE_END()
