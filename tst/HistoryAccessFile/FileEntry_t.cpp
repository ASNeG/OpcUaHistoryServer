#include "unittest.h"
#include "OpcUaHistory/HistoryAccessFile/FileOutEntry.h"
#include "OpcUaHistory/HistoryAccessFile/FileInEntry.h"

using namespace OpcUaHistory;

BOOST_AUTO_TEST_SUITE(FileEntry_)

BOOST_AUTO_TEST_CASE(FileEntry_)
{
	std::cout << "FileEntry_t" << std::endl;
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// read single element
// - one data folder
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
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

BOOST_AUTO_TEST_CASE(FileEntry_read_first_file_first_element)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 1);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 0);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_first_file_second_element)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100001.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100001.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 1);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 1);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_first_file_center_element)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100100.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100100.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 1);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 60);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_first_file_last_element)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100459.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100459.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 1);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 299);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_second_file_first_element)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100500.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100500.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 1);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 300);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_second_file_second_element)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100501.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100501.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 1);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 301);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_last_file_first_element)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T105500.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T105500.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 1);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 3300);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_last_file_second_element)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T105501.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T105501.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 1);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 3301);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_last_file_last_element)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T105959.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T105959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 1);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 3599);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// read several elements
// - one data folder
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(FileEntry_read_first_file_begin)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100059.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 0);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_first_file_center)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100100.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100159.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 60);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_first_file_end)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100400.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100459.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 240);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_second_file_begin)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100500.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100559.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 300);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_last_file_end)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T105900.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T105959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 3540);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_first_and_second_file)
{
	FileInEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 600);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 0);
}

BOOST_AUTO_TEST_SUITE_END()
