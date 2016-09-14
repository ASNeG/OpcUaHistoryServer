#include "unittest.h"
#include "OpcUaHistory/HistoryAccessFile/FileWriteEntry.h"
#include "OpcUaHistory/HistoryAccessFile/FileReadEntry.h"

using namespace OpcUaHistory;

BOOST_AUTO_TEST_SUITE(FileOutEntry_)

BOOST_AUTO_TEST_CASE(FileOutEntry_)
{
	std::cout << "FileOutEntry_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(FileOutEntry_init)
{
	FileWriteEntry fileWriteEntry;
}

BOOST_AUTO_TEST_CASE(FileOutEntry_out_one_value)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileWriteEntry fileWriteEntry;

	fileWriteEntry.maxDataFolderInValueFolder(5);
	fileWriteEntry.maxDataFilesInDataFolder(5);
	fileWriteEntry.maxEntriesInDataFile(5);
	fileWriteEntry.valueName("TestVariable");
	fileWriteEntry.baseFolder("./");

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");
	OpcUaDataValue dataValue;
	dataValue.sourceTimestamp(time);
	dataValue.serverTimestamp(time);
	dataValue.statusCode(Success);
	dataValue.variant()->variant((uint32_t)123);
	BOOST_REQUIRE(fileWriteEntry.write(dataValue) == true);
}

BOOST_AUTO_TEST_CASE(FileOutEntry_out_10)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileWriteEntry fileWriteEntry;

	fileWriteEntry.maxDataFolderInValueFolder(5);
	fileWriteEntry.maxDataFilesInDataFolder(5);
	fileWriteEntry.maxEntriesInDataFile(5);
	fileWriteEntry.valueName("TestVariable");
	fileWriteEntry.baseFolder("./");

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");

	for (uint32_t idx=0; idx<10; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileWriteEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(1);
	}
}

BOOST_AUTO_TEST_CASE(FileOutEntry_out_100)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileWriteEntry fileWriteEntry;

	fileWriteEntry.maxDataFolderInValueFolder(5);
	fileWriteEntry.maxDataFilesInDataFolder(5);
	fileWriteEntry.maxEntriesInDataFile(5);
	fileWriteEntry.valueName("TestVariable");
	fileWriteEntry.baseFolder("./");

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T102030.000000000");

	for (uint32_t idx=0; idx<100; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileWriteEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(1);
	}
}

BOOST_AUTO_TEST_CASE(FileEntry_several_write_objects1)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T100000.000000000");

	for (uint32_t idx=0; idx<2; idx++) {
		FileWriteEntry fileWriteEntry;

		fileWriteEntry.maxDataFolderInValueFolder(100);
		fileWriteEntry.maxDataFilesInDataFolder(10);
		fileWriteEntry.maxEntriesInDataFile(6);
		fileWriteEntry.valueName("TestVariable");
		fileWriteEntry.baseFolder("./");

		for (uint32_t idx=0; idx<5; idx++) {
			OpcUaDataValue dataValue;
			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant((uint32_t)idx);
			BOOST_REQUIRE(fileWriteEntry.write(dataValue) == true);
			time += boost::posix_time::seconds(10);
		}
	}

	FileReadEntry fileReadEntry;

	fileReadEntry.verbose(true);
	fileReadEntry.valueName("TestVariable");
	fileReadEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
	fileReadEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
	fileReadEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileReadEntry.readInitial(dataValueVec) == true);
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

	FileWriteEntry fileWriteEntry;

	fileWriteEntry.maxDataFolderInValueFolder(100);
	fileWriteEntry.maxDataFilesInDataFolder(10);
	fileWriteEntry.maxEntriesInDataFile(6);
	fileWriteEntry.valueName("TestVariable");
	fileWriteEntry.baseFolder("./");

	for (uint32_t idx=0; idx<5; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileWriteEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(10);
	}

	{
		FileReadEntry fileReadEntry;

		fileReadEntry.verbose(true);
		fileReadEntry.valueName("TestVariable");
		fileReadEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileReadEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileReadEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileReadEntry.readInitial(dataValueVec) == true);
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
		BOOST_REQUIRE(fileWriteEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(10);
	}

	{
		FileReadEntry fileReadEntry;

		fileReadEntry.verbose(true);
		fileReadEntry.valueName("TestVariable");
		fileReadEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileReadEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileReadEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileReadEntry.readInitial(dataValueVec) == true);
		std::cout << "Size=" << dataValueVec.size() << std::endl;
		BOOST_REQUIRE(dataValueVec.size() == 10);

		uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
		BOOST_REQUIRE(number == 0);
	}

	FileWriteEntry fileWriteEntry1;

	fileWriteEntry1.maxDataFolderInValueFolder(100);
	fileWriteEntry1.maxDataFilesInDataFolder(10);
	fileWriteEntry1.maxEntriesInDataFile(6);
	fileWriteEntry1.valueName("TestVariable");
	fileWriteEntry1.baseFolder("./");

	for (uint32_t idx=0; idx<5; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileWriteEntry1.write(dataValue) == true);
		time += boost::posix_time::seconds(10);
	}

	{
		FileReadEntry fileReadEntry;

		fileReadEntry.verbose(true);
		fileReadEntry.valueName("TestVariable");
		fileReadEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileReadEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileReadEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileReadEntry.readInitial(dataValueVec) == true);
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
		FileWriteEntry fileWriteEntry;

		fileWriteEntry.maxDataFolderInValueFolder(100);
		fileWriteEntry.maxDataFilesInDataFolder(10);
		fileWriteEntry.maxEntriesInDataFile(6);
		fileWriteEntry.valueName("TestVariable");
		fileWriteEntry.baseFolder("./");

		for (uint32_t idx=0; idx<5; idx++) {
			OpcUaDataValue dataValue;
			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant((uint32_t)idx);
			BOOST_REQUIRE(fileWriteEntry.write(dataValue) == true);
			time += boost::posix_time::seconds(10);
		}
	}

	{
		FileReadEntry fileReadEntry;

		fileReadEntry.verbose(true);
		fileReadEntry.valueName("TestVariable");
		fileReadEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileReadEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileReadEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileReadEntry.readInitial(dataValueVec) == true);
		std::cout << "Size=" << dataValueVec.size() << std::endl;
		BOOST_REQUIRE(dataValueVec.size() == 5);

		uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
		BOOST_REQUIRE(number == 0);
	}

	{
		FileWriteEntry fileWriteEntry;

		fileWriteEntry.maxDataFolderInValueFolder(100);
		fileWriteEntry.maxDataFilesInDataFolder(10);
		fileWriteEntry.maxEntriesInDataFile(6);
		fileWriteEntry.valueName("TestVariable");
		fileWriteEntry.baseFolder("./");

		for (uint32_t idx=0; idx<5; idx++) {
			OpcUaDataValue dataValue;
			dataValue.sourceTimestamp(time);
			dataValue.serverTimestamp(time);
			dataValue.statusCode(Success);
			dataValue.variant()->variant((uint32_t)idx);
			BOOST_REQUIRE(fileWriteEntry.write(dataValue) == true);
			time += boost::posix_time::seconds(10);
		}
	}

	{
		FileReadEntry fileReadEntry;

		fileReadEntry.verbose(true);
		fileReadEntry.valueName("TestVariable");
		fileReadEntry.baseFolder("./");

		OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T000000.000000000"));
		fileReadEntry.dateTimeFrom(from);
		OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T235959.000000000"));
		fileReadEntry.dateTimeTo(to);

		OpcUaDataValue::Vec dataValueVec;
		BOOST_REQUIRE(fileReadEntry.readInitial(dataValueVec) == true);
		std::cout << "Size=" << dataValueVec.size() << std::endl;
		BOOST_REQUIRE(dataValueVec.size() == 10);

		uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
		BOOST_REQUIRE(number == 0);
	}
}

BOOST_AUTO_TEST_SUITE_END()
