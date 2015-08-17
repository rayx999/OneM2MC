/*
 * test_ResourceBase.cc
 *
 *  Created on: 2015年8月12日
 *      Author: weimi_000
 */


#include <fstream>
#include <sstream>
#include <json2pb.h>
#include "gtest/gtest.h"
#include "test_utils.h"
#include "ResourceBase.h"

using namespace MicroWireless::OneM2M;

static bool setup = false;

class ResourceBaseTest : public ::testing::Test {
protected:
	static const string valid_json;
	static const string valid_path;

	ResourceBase res_base_;
//	ResourceStore<CSEBase> rdb;

	ResourceBaseTest() : res_base_()/*, rdb("data/.store")*/ {}

	virtual void SetUp() {
		if (setup) {
			//rdb.setupRoot();
			res_base_.setResourceBase(valid_json, valid_path);
		}
	}

	// virtual void TearDown() {}
};

//const string ResourceBaseTest::cse_ri("//microwireless.com/IN-CSE-01/CSEBASE");

const string ResourceBaseTest::valid_json("{"
				"\"ty\" 	: 5,"
				"\"ri\" 	: \"Z0005\","
				"\"rn\" 	: \"IN-CSE-01\","
				"\"ct\" 	: { \"seconds\" : 1435434103 },"
				"\"csb\"    : {"
					"\"cst\" 	: 1,"
					"\"csi\" 	: \"/IN-CSE-01\","
					"\"srt\" 	: [ 2, 5, 16 ]"
				"}"
			"}");

const string ResourceBaseTest::valid_path("//microwireless.com/IN-CSE-01/IN-CSE-01");

TEST_F(ResourceBaseTest, JsonValid) {
	try {
		ResourceBase res_base_(valid_json, valid_path);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(ResourceBaseTest, JsonWrongResourceType) {
	static const string json("{"
					"\"ty\" 	: 2,"
					"\"ri\" 	: \"Z0005\","
					"\"rn\" 	: \"IN-CSE-01\","
					"\"ct\" 	: { \"seconds\" : 1435434103 },"
					"\"csb\"    : {"
						"\"cst\" 	: 1,"
						"\"csi\" 	: \"/IN-CSE-01\","
						"\"srt\" 	: [ 2, 5, 16 ]"
					"}"
				"}");

	try {
		ResourceBase res_base_(json, valid_path);
	} catch (exception &e) {
		cout << "Expected exception: " << e.what() << endl;
		ASSERT_TRUE(true);
	}
}

TEST_F(ResourceBaseTest, JsonWrongElement) {
	static const string json("{"
					"\"ty\" 	: 5,"
					"\"ri\" 	: \"Z0005\","
					"\"rn\" 	: \"IN-CSE-01\","
					"\"ct\" 	: { \"seconds\" : 1435434103 },"
					"\"ae\"    : {"  // wrong!!
						"\"apn\" 	: \"APP-01\","
						"\"api\" 	: \"APP-01\","
						"\"aei\" 	: \"AE-01\" "
					"}"
				"}");

	try {
		ResourceBase res_base_(json, valid_path);
	} catch (exception &e) {
		cout << "Expected exception: " << e.what() << endl;
		ASSERT_TRUE(true);
	}
}


TEST_F(ResourceBaseTest, checkCreateTime) {
	ResourceBase res_base_;

	// record current time
	TimeStamp _create_time;
	gettimeofday(&_create_time, NULL);

	// retrieve CSEBase create time stamp
	TimeStamp _time_stamp;
	ASSERT_TRUE(res_base_.getCreateTimestamp(_time_stamp));
	EXPECT_TSEQ(_create_time, _time_stamp);
}

TEST_F(ResourceBaseTest, JsonWOCreateTime) {
	static const string json("{"
				"\"ty\" 	: 5,"
				"\"ri\" 	: \"Z0005\","
				"\"rn\" 	: \"IN-CSE-01\","
				"\"csb\"    : {"
					"\"cst\" 	: 1,"
					"\"csi\" 	: \"/IN-CSE-01\","
					"\"srt\" 	: [ 2, 5, 16 ]"
				"}"
			"}");

	try {
		ResourceBase res_base_;

		// keep ct from constructor
		TimeStamp _original_ct;
		ASSERT_TRUE(res_base_.getCreateTimestamp(_original_ct));

		ASSERT_TRUE(res_base_.setResourceBase(json, valid_path));

		// check original timestamp persist after loading json file w/o ct
		TimeStamp _check_ct;
		ASSERT_TRUE(res_base_.getCreateTimestamp(_check_ct));
		ASSERT_TSEQ(_original_ct, _check_ct);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(ResourceBaseTest, TurnOnFixture) {
	setup = true;
}


TEST_F(ResourceBaseTest, CheckLastModifyTime) {
	//get current time
	TimeStamp  cur_time_;
	gettimeofday(&cur_time_, NULL);

	// modify resource
	ASSERT_TRUE(res_base_.setParentId("in-cse-01"));
	TimeStamp ts_;
	ASSERT_TRUE(res_base_.getLastModifiedTimestamp(ts_));
	EXPECT_TSEQ(ts_, cur_time_);
	ASSERT_STREQ(res_base_.getParentId().c_str(), "in-cse-01");
}

TEST_F(ResourceBaseTest, CheckDomainCsiRi) {
	ASSERT_STREQ(res_base_.getDomain().c_str(), "//microwireless.com");
	ASSERT_STREQ(res_base_.getIntCsi().c_str(), "/IN-CSE-01");
	ASSERT_STREQ(res_base_.getIntRi().c_str(), "IN-CSE-01");
	ASSERT_STREQ(res_base_.getResourceId().c_str(), "Z0005");
	ASSERT_STREQ(res_base_.getResourceName().c_str(), "IN-CSE-01");
}
