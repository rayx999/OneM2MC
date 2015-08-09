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

static bool setup = false;

class CSEBaseTest : public ::testing::Test {
protected:
	static const string cse_ri;
	CSEBase cse_base;
	ResourceStore<CSEBase> rdb;

	CSEBaseTest() : cse_base(), rdb("data/.store") {}

	virtual void SetUp() {
		if (setup) {
			rdb.setupRoot();
			cse_base.setCSEBase(cse_ri, rdb);
		}
	}

	bool OutputResource(const string& res_path)  {
		return cse_base.outToResourceStore(res_path, rdb);
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

const string CSEBaseTest::cse_ri("//microwireless.com/IN-CSE-01/CSEBASE");

TEST_F(CSEBaseTest, JsonValid) {
	static const string json("{"
					"\"ty\" 	: 1,"
					"\"ri\" 	: \"//microwireless.com/IN-CSE-01/CSEBASE\","
					"\"rn\" 	: \"CSEBASE\","
					"\"ct\" 	: { \"seconds\" : 1435434103 },"
					"\"cst\" 	: 1,"
					"\"csi\" 	: \"/IN-CSE-01\","
					"\"srt\" 	: [ 2, 5, 16 ]"
				"}");

	try {
		CSEBase cse_base_(json);
		cse_base_.outToResourceStore(string("CSEBase_resourcebase"), rdb);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(CSEBaseTest, checkCreateTime) {
	CSEBase cse_base_;

	// record current time
	TimeStamp _create_time;
	gettimeofday(&_create_time, NULL);

	// retrieve CSEBase create time stamp
	TimeStamp _time_stamp;
	ASSERT_TRUE(cse_base_.getCreateTimestamp(_time_stamp));
	EXPECT_TSEQ(_create_time, _time_stamp);
}

TEST_F(CSEBaseTest, JsonWithCreateTime) {
	static const string json("{"
					"\"ty\" 	: 1,"
					"\"ri\" 	: \"//microwireless.com/IN-CSE-01/CSEBASE\","
					"\"rn\" 	: \"CSEBASE\","
					"\"ct\" 	: { \"seconds\" : 1435434103 },"
					"\"cst\" 	: 1,"
					"\"csi\" 	: \"/IN-CSE-01\","
					"\"srt\" 	: [ 2, 5, 16 ]"
				"}");

	try {
		CSEBase cse_base_(json);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(CSEBaseTest, JsonWOCreateTime) {
	static const string json("{"
					"\"ty\" 	: 1,"
					"\"ri\" 	: \"//microwireless.com/IN-CSE-01/CSEBASE\","
					"\"rn\" 	: \"CSEBASE\","
					"\"cst\" 	: 1,"
					"\"csi\" 	: \"/IN-CSE-01\","
					"\"srt\" 	: [ 2, 5, 16 ]"
				"}");

	try {
		CSEBase cse_base;

		// keep ct from constructor
		TimeStamp _original_ct;
		ASSERT_TRUE(cse_base.getCreateTimestamp(_original_ct));

		ASSERT_TRUE(cse_base.setCSEBase(json));

		// check original timestamp persist after loading json file w/o ct
		TimeStamp _check_ct;
		ASSERT_TRUE(cse_base.getCreateTimestamp(_check_ct));
		ASSERT_TSEQ(_original_ct, _check_ct);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
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

TEST_F(CSEBaseTest, SetupRoot) {

	try {
		ResourceStore<CSEBase> rdb("data/.store");
		ASSERT_TRUE(rdb.setupRoot());
		CSEBase * p_root_ = rdb.getRoot();
		ASSERT_TRUE(p_root_ != NULL);
		ASSERT_EQ(p_root_->getResourceBase(), CSE_BASE);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(CSEBaseTest, TurnOnFixture) {
	setup = true;
}

TEST_F(CSEBaseTest, OutResourceToStore) {
	ASSERT_TRUE(OutputResource("CSEBase_new"));
}

// check if 2 CSEBase the same, except last_modified_time
TEST_F(CSEBaseTest, CheckOutResource) {
	CSEBase cse_base_new("CSEBase_new", rdb);

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

	ASSERT_TRUE(OutputResource("CSEBase_new"));

	// Reload
	CSEBase cse_base;
	TimeStamp _last_modified_time;
	ASSERT_TRUE(cse_base.setCSEBase("CSEBase_new", rdb));
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

