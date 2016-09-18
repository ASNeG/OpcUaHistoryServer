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

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// aging
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileReadManager_readInitial_aging)
{
	FileReadManager fileReadManager;

	fileReadManager.verbose(true);
	fileReadManager.baseFolder("TestFolder");
	fileReadManager.maxConcurrentValues(3);
	fileReadManager.ageCounter(2);

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100059.000000000"));

	ValueReadContext valueReadContext[5];
	for (uint32_t idx=0; idx<5; idx++) {
		std::stringstream valueName;
		valueName << "MyValue" << idx;
		valueReadContext[idx].valueName_ = valueName.str();
	}

	OpcUaDataValue::Vec dataValueVec;

	// reading
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext[0], nullptr, from, to, dataValueVec) == true);
	BOOST_REQUIRE(dataValueVec.size() == 60);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext[1], nullptr, from, to, dataValueVec) == true);
	BOOST_REQUIRE(dataValueVec.size() == 60);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext[2], nullptr, from, to, dataValueVec) == true);
	BOOST_REQUIRE(dataValueVec.size() == 60);

	// aging
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext[0], nullptr, from, to, dataValueVec) == true);
	BOOST_REQUIRE(dataValueVec.size() == 60);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext[2], nullptr, from, to, dataValueVec) == true);
	BOOST_REQUIRE(dataValueVec.size() == 60);

	// delete oldest entry
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext[3], nullptr, from, to, dataValueVec) == true);
	BOOST_REQUIRE(dataValueVec.size() == 60);
	BOOST_REQUIRE(fileReadManager.deletedValueName() == "MyValue1");
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// readNext
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileReadManager_readNext_1)
{
	FileReadManager fileReadManager;

	fileReadManager.verbose(true);
	fileReadManager.baseFolder("TestFolder");
	fileReadManager.maxConcurrentValues(3);
	fileReadManager.ageCounter(2);

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T110000.000000000"));

	ValueReadContext valueReadContext;
	ValueReadContinousPoint continousPoint;
	OpcUaDataValue::Vec dataValueVec;

	valueReadContext.valueName_ = "MyValue0";

	// readInitial
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext, &continousPoint, from, to, dataValueVec, 1000) == true);
	BOOST_REQUIRE(dataValueVec.size() == 1000);
	BOOST_REQUIRE(continousPoint.readComplete_ == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint, dataValueVec, 1000) == true);
	BOOST_REQUIRE(dataValueVec.size() == 1000);
	BOOST_REQUIRE(continousPoint.readComplete_ == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint, dataValueVec, 1000) == true);
	BOOST_REQUIRE(dataValueVec.size() == 1000);
	BOOST_REQUIRE(continousPoint.readComplete_ == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint, dataValueVec, 1000) == true);
	BOOST_REQUIRE(dataValueVec.size() == 600);
	BOOST_REQUIRE(continousPoint.readComplete_ == true);
}

BOOST_AUTO_TEST_CASE(FileReadManager_readNext_2)
{
	FileReadManager fileReadManager;

	fileReadManager.verbose(true);
	fileReadManager.baseFolder("TestFolder");
	fileReadManager.maxConcurrentValues(3);
	fileReadManager.ageCounter(2);

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T110000.000000000"));

	ValueReadContext valueReadContext;
	ValueReadContinousPoint continousPoint;
	OpcUaDataValue::Vec dataValueVec;

	valueReadContext.valueName_ = "MyValue0";

	// readInitial
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext, &continousPoint, from, to, dataValueVec, 1000) == true);
	BOOST_REQUIRE(dataValueVec.size() == 1000);
	BOOST_REQUIRE(continousPoint.readComplete_ == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint, dataValueVec, 2600) == true);
	BOOST_REQUIRE(dataValueVec.size() == 2600);
	BOOST_REQUIRE(continousPoint.readComplete_ == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint, dataValueVec, 1000) == true);
	BOOST_REQUIRE(dataValueVec.size() == 0);
	BOOST_REQUIRE(continousPoint.readComplete_ == true);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// readNext - no free resource available
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileReadManager_readNext_no_free_resource_available)
{
	FileReadManager fileReadManager;

	fileReadManager.verbose(true);
	fileReadManager.baseFolder("TestFolder");
	fileReadManager.maxConcurrentValues(3);
	fileReadManager.ageCounter(2);
	fileReadManager.maxContinousPoint(2);

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T110059.000000000"));

	ValueReadContext valueReadContext1;
	ValueReadContext valueReadContext2;
	ValueReadContext valueReadContext3;
	ValueReadContinousPoint continousPoint1;
	ValueReadContinousPoint continousPoint2;
	ValueReadContinousPoint continousPoint3;
	OpcUaDataValue::Vec dataValueVec;

	valueReadContext1.valueName_ = "MyValue0";
	valueReadContext2.valueName_ = "MyValue0";
	valueReadContext3.valueName_ = "MyValue0";

	// readInitial
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext1, &continousPoint1, from, to, dataValueVec, 40) == true);
	BOOST_REQUIRE(dataValueVec.size() == 40);
	BOOST_REQUIRE(continousPoint1.readComplete_ == false);
	BOOST_REQUIRE(continousPoint1.error_ == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext2, &continousPoint2, from, to, dataValueVec, 40) == true);
	BOOST_REQUIRE(dataValueVec.size() == 40);
	BOOST_REQUIRE(continousPoint2.readComplete_ == false);
	BOOST_REQUIRE(continousPoint2.error_ == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext3, &continousPoint3, from, to, dataValueVec, 40) == true);
	BOOST_REQUIRE(dataValueVec.size() == 40);
	BOOST_REQUIRE(continousPoint3.readComplete_ == false);
	BOOST_REQUIRE(continousPoint3.error_ == true);

#if 0
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint, dataValueVec, 1000) == true);
	BOOST_REQUIRE(dataValueVec.size() == 1000);
	BOOST_REQUIRE(continousPoint.readComplete_ == false);
#endif

}

BOOST_AUTO_TEST_SUITE_END()
