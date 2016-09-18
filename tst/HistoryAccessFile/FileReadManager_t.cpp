#include "unittest.h"
#include <boost/thread/thread.hpp>
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
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100059.000000000"));

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

	// readNext
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint1, dataValueVec, 40) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint1.readComplete_ == true);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint2, dataValueVec, 40) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint2.readComplete_ == true);

}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// readNext - timeout handling - continous point
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileReadManager_readNext_timeout)
{
	FileReadManager fileReadManager;

	fileReadManager.verbose(true);
	fileReadManager.baseFolder("TestFolder");
	fileReadManager.maxConcurrentValues(3);
	fileReadManager.ageCounter(1);
	fileReadManager.maxContinousPoint(3);
	fileReadManager.continousPointIdleTimeout(1);
	fileReadManager.maxDeleteTimeoutEntries(10);

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100059.000000000"));

	ValueReadContext valueReadContext1;
	ValueReadContext valueReadContext2;
	ValueReadContext valueReadContext3;
	ValueReadContext valueReadContext4;
	ValueReadContinousPoint continousPoint1;
	ValueReadContinousPoint continousPoint2;
	ValueReadContinousPoint continousPoint3;
	ValueReadContinousPoint continousPoint4;
	OpcUaDataValue::Vec dataValueVec;

	valueReadContext1.valueName_ = "MyValue0";
	valueReadContext2.valueName_ = "MyValue1";
	valueReadContext3.valueName_ = "MyValue2";
	valueReadContext4.valueName_ = "MyValue3";

	// readInitial
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext1, &continousPoint1, from, to, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint1.readComplete_ == false);
	BOOST_REQUIRE(continousPoint1.error_ == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext2, &continousPoint2, from, to, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint2.readComplete_ == false);
	BOOST_REQUIRE(continousPoint2.error_ == false);


	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext3, &continousPoint3, from, to, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint3.readComplete_ == false);
	BOOST_REQUIRE(continousPoint3.error_ == false);

	// wait 20 milliseconds
	boost::this_thread::sleep(boost::posix_time::milliseconds(20));

	// readNext
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint1, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint1.readComplete_ == false);

	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint3, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint3.readComplete_ == false);

	// readInitial
	// timeout MyValue2
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext4, &continousPoint4, from, to, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint4.readComplete_ == false);
	BOOST_REQUIRE(continousPoint4.error_ == false);
	BOOST_REQUIRE(fileReadManager.deletedContinousPoint() == continousPoint2.continousPoint_);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// readNext - delete continous point
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileReadManager_readNext_delete_continous_point)
{
	FileReadManager fileReadManager;

	fileReadManager.verbose(true);
	fileReadManager.baseFolder("TestFolder");
	fileReadManager.maxConcurrentValues(3);
	fileReadManager.ageCounter(1);
	fileReadManager.maxContinousPoint(3);
	fileReadManager.continousPointIdleTimeout(1);
	fileReadManager.maxDeleteTimeoutEntries(10);

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100059.000000000"));

	ValueReadContext valueReadContext1;
	ValueReadContinousPoint continousPoint1;

	OpcUaDataValue::Vec dataValueVec;
	valueReadContext1.valueName_ = "MyValue0";


	// readInitial
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext1, &continousPoint1, from, to, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint1.readComplete_ == false);
	BOOST_REQUIRE(continousPoint1.error_ == false);

	// readNext
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint1, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint1.readComplete_ == false);

	// readNext - delete continous point
	dataValueVec.clear();
	continousPoint1.command_ = ValueReadContinousPoint::Delete;
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint1, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 0);
	BOOST_REQUIRE(continousPoint1.readComplete_ == true);
	BOOST_REQUIRE(fileReadManager.deletedContinousPoint() == continousPoint1.continousPoint_);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
// readNext - last continous point access
//
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE(FileReadManager_readNext_last_continous_point_access)
{
	FileReadManager fileReadManager;

	fileReadManager.verbose(true);
	fileReadManager.baseFolder("TestFolder");
	fileReadManager.maxConcurrentValues(3);
	fileReadManager.ageCounter(1);
	fileReadManager.maxContinousPoint(3);
	fileReadManager.continousPointIdleTimeout(1);
	fileReadManager.maxDeleteTimeoutEntries(10);

	OpcUaDateTime from(boost::posix_time::from_iso_string("20150101T100000.000000000"));
	OpcUaDateTime to(boost::posix_time::from_iso_string("20150101T100059.000000000"));

	ValueReadContext valueReadContext1;
	ValueReadContinousPoint continousPoint1;

	OpcUaDataValue::Vec dataValueVec;
	valueReadContext1.valueName_ = "MyValue0";


	// readInitial
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readInitial(valueReadContext1, &continousPoint1, from, to, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint1.readComplete_ == false);
	BOOST_REQUIRE(continousPoint1.error_ == false);

	// readNext
	dataValueVec.clear();
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint1, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint1.readComplete_ == false);

	// readNext - delete continous point
	dataValueVec.clear();
	continousPoint1.command_ = ValueReadContinousPoint::LastAccess;
	BOOST_REQUIRE(fileReadManager.readNext(continousPoint1, dataValueVec, 20) == true);
	BOOST_REQUIRE(dataValueVec.size() == 20);
	BOOST_REQUIRE(continousPoint1.readComplete_ == true);
	BOOST_REQUIRE(fileReadManager.deletedContinousPoint() == continousPoint1.continousPoint_);

}

BOOST_AUTO_TEST_SUITE_END()
