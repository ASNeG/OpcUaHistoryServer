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
BOOST_AUTO_TEST_CASE(FileEntry_write_test_data_1)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileWriteEntry fileOutEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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
	FileReadEntry fileInEntry;

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

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// use several data folder
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileEntry_several_data_folder)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileWriteEntry fileOutEntry;

	fileOutEntry.maxDataFolderInValueFolder(100);
	fileOutEntry.maxDataFilesInDataFolder(10);
	fileOutEntry.maxEntriesInDataFile(10);
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

BOOST_AUTO_TEST_CASE(FileEntry_read_from_several_data_folder)
{
	FileReadEntry fileInEntry;

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

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// tests with empty result set
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileEntry_write_test_data_2)
{
	boost::filesystem::path path("TestVariable");
	boost::filesystem::remove_all(path);

	FileWriteEntry fileOutEntry;

	fileOutEntry.maxDataFolderInValueFolder(100);
	fileOutEntry.maxDataFilesInDataFolder(10);
	fileOutEntry.maxEntriesInDataFile(6);
	fileOutEntry.valueName("TestVariable");
	fileOutEntry.baseFolder("./");

	boost::posix_time::ptime time = boost::posix_time::from_iso_string("20150101T100000.000000000");

	for (uint32_t idx=0; idx<360; idx++) {
		OpcUaDataValue dataValue;
		dataValue.sourceTimestamp(time);
		dataValue.serverTimestamp(time);
		dataValue.statusCode(Success);
		dataValue.variant()->variant((uint32_t)idx);
		BOOST_REQUIRE(fileOutEntry.write(dataValue) == true);
		time += boost::posix_time::seconds(10);
	}
}

BOOST_AUTO_TEST_CASE(FileEntry_empty_result_first_file_befor)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T090000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T093040.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 0);
}

BOOST_AUTO_TEST_CASE(FileEntry_empty_result_first_file_center)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100105.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100109.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 0);
}

BOOST_AUTO_TEST_CASE(FileEntry_empty_result_second_file_before)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100905.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100909.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 0);
}

BOOST_AUTO_TEST_CASE(FileEntry_empty_result_second_file_center)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T101105.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T101109.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 0);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// tests with several data folders
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileEntry_read_first_folder_begin)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100459.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 30);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 0);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_first_folder_center)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100300.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100759.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 30);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 18);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_first_folder_end)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100500.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 30);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 30);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_second_folder_begin)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T101000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T101459.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 30);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 60);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_several_directory_begin)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T101959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 120);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 0);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_several_directory_center)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T102000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T103459.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 90);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 120);
}

BOOST_AUTO_TEST_CASE(FileEntry_read_several_directory_end)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T102000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T110000.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 240);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 120);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// several reads with same intstance
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileEntry_read_instance_second)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	// first read access
	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);

	uint32_t number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 0);

	// second read access
	from = boost::posix_time::from_iso_string("20150101T100100.000000000");
	fileInEntry.dateTimeFrom(from);
	to = boost::posix_time::from_iso_string("20150101T101059.000000000");
	fileInEntry.dateTimeTo(to);

	dataValueVec.clear();
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);

	number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 6);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// max result entries
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileEntry_max_result_entries_limit1)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	// first read access
	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T101000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T101959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec, 61) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == false);
}

BOOST_AUTO_TEST_CASE(FileEntry_max_result_entries_limit2)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	// first read access
	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T101000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T101959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec, 60) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == true);
}

BOOST_AUTO_TEST_CASE(FileEntry_max_result_entries_limit3)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	// first read access
	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T101000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T101959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec, 50) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 50);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == true);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// readNext
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileEntry_readNext_empty)
{
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	// first read access
	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T101000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T101959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec, 70) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 60);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileInEntry.readNext(dataValueVec, 70) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 0);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == false);
}

BOOST_AUTO_TEST_CASE(FileEntry_readNext_not_empty_1)
{
	uint32_t number;
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	// first read access
	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T101000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T101959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec, 25) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 25);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == true);
	number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 60);

	dataValueVec.clear();
	BOOST_REQUIRE(fileInEntry.readNext(dataValueVec, 25) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 25);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == true);
	number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 85);

	dataValueVec.clear();
	BOOST_REQUIRE(fileInEntry.readNext(dataValueVec, 25) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 10);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == false);
	number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 110);
}

BOOST_AUTO_TEST_CASE(FileEntry_readNext_not_empty_2)
{
	uint32_t number;
	FileReadEntry fileInEntry;

	fileInEntry.verbose(true);
	fileInEntry.valueName("TestVariable");
	fileInEntry.baseFolder("./");

	// first read access
	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T101000.000000000"));
	fileInEntry.dateTimeFrom(from);
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T101959.000000000"));
	fileInEntry.dateTimeTo(to);

	OpcUaDataValue::Vec dataValueVec;
	BOOST_REQUIRE(fileInEntry.readInitial(dataValueVec, 30) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 30);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == true);
	number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 60);

	dataValueVec.clear();
	BOOST_REQUIRE(fileInEntry.readNext(dataValueVec, 30) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 30);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == true);
	number = dataValueVec[0]->variant()->get<OpcUaUInt32>();
	BOOST_REQUIRE(number == 90);

	dataValueVec.clear();
	BOOST_REQUIRE(fileInEntry.readNext(dataValueVec, 30) == true);
	std::cout << "Size=" << dataValueVec.size() << std::endl;
	BOOST_REQUIRE(dataValueVec.size() == 0);
	BOOST_REQUIRE(fileInEntry.maxResultEntriesReached() == false);
}

BOOST_AUTO_TEST_SUITE_END()
