/*
 * test_CSEBase.cc for gtest
 *
 *  Created on: 2015年7月10日
 *      Author: weimi_000
 */
#include <fstream>
#include <sstream>
#include <json2pb.h>
#include "gtest/gtest.h"
#include "test_utils.h"
#include "CSEBase.h"

using namespace MicroWireless::OneM2M;

class CSEBaseTest : public ::testing::Test {
	protected:
	CSEBase cse_base;
	bool setup;

	CSEBaseTest() { setup = false; }

	virtual void SetUp() {
		if (std::ifstream(DEFAULT_CSEBASE_FN).good()) {
			if (cse_base.setCSEBase(DEFAULT_CSEBASE_FN)) {
				setup = true;
			}
		}
	}

	bool OutputResource(const char * fn) {
		if (setup) {
			return cse_base.outToFile(fn);
		}
		cerr << "Test fixture cse_base is not set up properly." << endl;
		return false;
	}

	const string& getCSEId() {
		return cse_base.getCSEId();
	}

	int getCSEType() {
		return (int)cse_base.getCSEType();
	}

	const string& getResourceId() {
		return cse_base.getResourceId();
	}

	const string& getResourceName() {
		return cse_base.getResourceName();
	}

	bool getCreateTime(TimeStamp &ct) {
		return cse_base.getCreateTimestamp(ct);
	}

	bool isResourceSupported(SupportedResourceType rt) {
		return cse_base.isResourceSupported(rt);
	}

	int getSupportedResource(SupportedResourceType *&rt) {
		return cse_base.getSupportedResource(rt);
	}
	// virtual void TearDown() {}
};

TEST_F(CSEBaseTest, checkCreateTime) {
	CSEBase cse_base;

	// record current time
	TimeStamp _create_time;
	gettimeofday(&_create_time, NULL);

	// retrieve CSEBase create time stamp
	TimeStamp _time_stamp;
	ASSERT_TRUE(cse_base.getCreateTimestamp(_time_stamp));
	EXPECT_TSEQ(_create_time, _time_stamp);
}

TEST_F(CSEBaseTest, JsonWithCreateTime) {
	CSEBase cse_base;

	fstream ins("data/in-cse.json", ios::in | ios::binary);
	ASSERT_TRUE(ins != NULL);

	// read json resource definition
	stringstream buf;
	buf << ins.rdbuf();
	//cout << buf.str() << endl;
	ASSERT_TRUE(cse_base.setCSEBase(buf.str()));
	//cout << cse_base.getJson() << endl;
}

TEST_F(CSEBaseTest, JsonWOCreateTime) {
	CSEBase cse_base;

	// keep ct from constructor
	TimeStamp _original_ct;
	ASSERT_TRUE(cse_base.getCreateTimestamp(_original_ct));

	fstream ins("data/in-cse-no-ct.json", ios::in | ios::binary);
	ASSERT_TRUE(ins != NULL);

	// read json resource definition
	stringstream buf;
	buf << ins.rdbuf();

	ASSERT_TRUE(cse_base.setCSEBase(buf.str()));

	// check original timestamp persist after loading json file w/o ct
	TimeStamp _check_ct;
	ASSERT_TRUE(cse_base.getCreateTimestamp(_check_ct));
	ASSERT_TSEQ(_original_ct, _check_ct);
}

TEST_F(CSEBaseTest, JsonConflictCSEId) {
	static const string json("{"
					"\"ty\" 	: 1,"
					"\"ri\" 	: \"//microwireless.com/IN-CSE-00/CSEBASE\","
					"\"rn\" 	: \"CSEBASE\","
					"\"ct\" 	: { \"seconds\" : 1435434103 },"
					"\"cst\" 	: 1,"
					"\"csi\" 	: \"/IN-CSE-01\","
					"\"srt\" 	: [ 2, 5, 16 ]"
				"}");

	try {
		CSEBase cse_base_(json);
	} catch (exception &e) {
		cout << "Excepted exception: " << e.what() << endl;
		return;
	}
	cerr << "Excepted exception doesn't occur." << endl;
	ASSERT_TRUE(false);
}

TEST_F(CSEBaseTest, JsonConflictResourceName) {
	static const string json("{"
					"\"ty\" 	: 1,"
					"\"ri\" 	: \"//microwireless.com/IN-CSE-00/CSE\","
					"\"rn\" 	: \"CSEBase\","
					"\"ct\" 	: { \"seconds\" : 1435434103 },"
					"\"cst\" 	: 1,"
					"\"csi\" 	: \"/IN-CSE-01\","
					"\"srt\" 	: [ 2, 5, 16 ]"
				"}");

	try {
		CSEBase cse_base_(json);
	} catch (exception &e) {
		cout << "Excepted exception: " << e.what() << endl;
		return;
	}
	cerr << "Excepted exception doesn't occur." << endl;
	ASSERT_TRUE(false);
}

TEST_F(CSEBaseTest, ResourceConstructor) {
	CSEBase cse_base;

	ASSERT_TRUE(UTest::CopyFile("data/CSEBase_Sample.res", DEFAULT_CSEBASE_FN));

	ASSERT_TRUE(cse_base.setCSEBase(DEFAULT_CSEBASE_FN));
}

TEST_F(CSEBaseTest, OutputResouceFile) {
	ASSERT_TRUE(OutputResource("data/CSEBase_new.res"));
}

// check if 2 CSEBase the same, except last_modified_time
TEST_F(CSEBaseTest, CheckOutputResouceFile) {
	CSEBase cse_base_new;

	ASSERT_TRUE(cse_base_new.setCSEBase("data/CSEBase_new.res"));
	ASSERT_STREQ(getCSEId().c_str(), cse_base_new.getCSEId().c_str());
	ASSERT_EQ(getCSEType(), cse_base_new.getCSEType());
	ASSERT_STREQ(getResourceId().c_str(), cse_base_new.getResourceId().c_str());
	ASSERT_STREQ(getResourceName().c_str(), cse_base_new.getResourceName().c_str());

	TimeStamp _ct1, _ct2;
	ASSERT_TRUE(getCreateTime(_ct1));
	ASSERT_TRUE(cse_base_new.getCreateTimestamp(_ct2));
	ASSERT_TSEQ(_ct1, _ct2);

	// supported resource
}

TEST_F(CSEBaseTest, GetAttributes) {
	ASSERT_STREQ(getCSEId().c_str(), "/IN-CSE-01");
	ASSERT_EQ(getCSEType(), IN_CSE);
	ASSERT_STREQ(getResourceId().c_str(), "//microwireless.com/IN-CSE-01/CSEBASE");
	ASSERT_STREQ(getResourceName().c_str(), "CSEBASE");
	ASSERT_STREQ(cse_base.getDomain().c_str(), "//microwireless.com");
}

TEST_F(CSEBaseTest, CheckLastModifiedTime) {
	TimeStamp _cur_time;
	gettimeofday(&_cur_time, NULL);

	ASSERT_TRUE(OutputResource("data/CSEBase_new.res"));

	// Reload
	CSEBase cse_base;
	TimeStamp _last_modified_time;
	ASSERT_TRUE(cse_base.setCSEBase("data/CSEBase_new.res"));
	ASSERT_TRUE(cse_base.getLastModifiedTimestamp(_last_modified_time));
	EXPECT_TSEQ(_last_modified_time, _cur_time);
}

TEST_F(CSEBaseTest, CheckSupportedResourceType) {
	ASSERT_TRUE(isResourceSupported(CSE_BASE));
	ASSERT_FALSE(isResourceSupported(FAN_OUT_POINT));
}

TEST_F(CSEBaseTest, GetSupportedResource) {
	SupportedResourceType *_rt;
	int size = getSupportedResource(_rt);
	ASSERT_EQ(size, 3);
	SupportedResourceType _srt[] = {AE,CSE_BASE,REMOTE_CSE};
	UTest::ASSERT_ARREQ(_rt, _srt, 3);
}

