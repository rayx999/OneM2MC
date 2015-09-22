/*
 * test_ResourceBaseAnnc.cc
 *
 *  Created on: 2015年9月19日
 *      Author: weimi_000
 */


#include <iostream>
#include <string>
#include <json2pb.h>
#include "gtest/gtest.h"
#include "test_utils.h"
#include "CommonTypes.h"
#include "ResourceBase.h"
#include "ResourceBaseAnnc.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class ResourceBaseAnncTest : public ::testing::Test {
protected:
	static const string valid_json;
	static const string valid_path;

	AnncAttr ma_, oa_, empty_;
	ResourceBase src_;
	ResourceBaseAnnc annc_;

	ResourceBaseAnncTest() : src_(), annc_() {}
	virtual ~ResourceBaseAnncTest() {}
/*
	void testRequest2Resource(const string& res_json, const string& reqp_json, const string &rn) {
		string res_pc_str;
		ASSERT_TRUE(UTest::Resource2String(res_json, res_pc_str));

		try {
			RequestPrim reqp(reqp_json);
			reqp.setContent(res_pc_str);
			ResourceBase res_;
			ASSERT_TRUE(res_.setResourceBase(reqp, *rdb.cse()));
			if (rn.empty()) {
				ASSERT_TRUE(res_.getIntRi().empty());
			} else {
				ASSERT_STREQ(res_.getIntRi().c_str(), rn.c_str());
			}
		} catch (exception &e) {
			cout << "Unexpected exception: " << e.what() << endl;
			ASSERT_TRUE(false);
		}
	}
*/
	virtual void SetUp() {
		ma_ = {"ri",  "rn",  "et",  "acpi", "lbl" };
		oa_ = { "apn", "api", "aei", "poa",  "or", "nl" };
	}

	virtual void TearDown() {}
};

const string ResourceBaseAnncTest::valid_json("{"
		"\"ty\" 	: 2,"
		"\"ri\" 	: \"Z0002\","
		"\"rn\" 	: \"AE-01\","
		"\"et\"     : { \"seconds\" : 1435434103 },"
		"\"acpi\"   : [ \"001-50294\", \"001-21850\" ],"
		"\"lbl\"    : [ \"test\" ],"
		"\"ae\"     : {"
			"\"apn\" 	: \"FreshGo\","
			"\"api\" 	: \"APP-01\","
			"\"aei\" 	: \"AE-01\" "
		"}"
"}");

const string ResourceBaseAnncTest::valid_path("//microwireless.com/IN-CSE-01/AE-01");

TEST_F(ResourceBaseAnncTest, CheckMA) {
	try {
		ResourceBase src_(valid_json, valid_path);
		ResourceBaseAnnc annc_;
		annc_.copyAnncFields(src_, ma_, empty_);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
	ASSERT_TRUE(ma_.size() == 0);
}
/*
TEST_F(ResourceBaseAnncTest, JsonWrongResourceType) {
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

TEST_F(ResourceBaseAnncTest, JsonWrongElement) {
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

TEST_F(ResourceBaseAnncTest, RequestWithName) {
	const string reqp_pc("{"
					"\"ty\" 	: 2,"
					"\"ae\"     : {"
						"\"apn\" 	: \"APP-01\","
						"\"api\" 	: \"APP-01\","
						"\"aei\" 	: \"AE-01\" "
					             "}"
					"}");

	const string reqp_json("{"
			"\"op\": 1, "
			"\"to\": \"//microwireless.com/IN-CSE-01\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\", "
			"\"nm\": \"AE-01\" "
		"}");

	testRequest2Resource(reqp_pc, reqp_json, "AE-01");
}

TEST_F(ResourceBaseAnncTest, RequestWithRn) {
	const string reqp_pc("{"
					"\"ty\" 	: 2,"
					"\"rn\"     :\"AE-01\", "
					"\"ae\"     : {"
						"\"apn\" 	: \"APP-01\","
						"\"api\" 	: \"APP-01\","
						"\"aei\" 	: \"AE-01\" "
					             "}"
					"}");

	const string reqp_json("{"
			"\"op\": 1, "
			"\"to\": \"//microwireless.com/IN-CSE-01\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\" "
		"}");

	testRequest2Resource(reqp_pc, reqp_json, "AE-01");
}

TEST_F(ResourceBaseAnncTest, RequestWithTo) {
	const string reqp_pc("{"
					"\"ty\" 	: 2,"
					"\"ae\"     : {"
						"\"apn\" 	: \"APP-01\","
						"\"api\" 	: \"APP-01\","
						"\"aei\" 	: \"AE-01\" "
					             "}"
					"}");

	const string reqp_json("{"
			"\"op\": 1, "
			"\"to\": \"//microwireless.com/IN-CSE-01/AE-01\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\" "
		"}");

	testRequest2Resource(reqp_pc, reqp_json, "AE-01");
}

TEST_F(ResourceBaseAnncTest, RequestNoRn) {
	const string reqp_pc("{"
					"\"ty\" 	: 2,"
					"\"ae\"     : {"
						"\"apn\" 	: \"APP-01\","
						"\"api\" 	: \"APP-01\","
						"\"aei\" 	: \"AE-01\" "
					             "}"
					"}");

	const string reqp_json("{"
			"\"op\": 1, "
			"\"to\": \"//microwireless.com/IN-CSE-01\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\" "
		"}");

	testRequest2Resource(reqp_pc, reqp_json, string());
}

TEST_F(ResourceBaseAnncTest, checkCreateTime) {
	ResourceBase res_base_;

	// record current time
	TimeStamp _create_time;
	gettimeofday(&_create_time, NULL);

	// retrieve CSEBase create time stamp
	TimeStamp _time_stamp;
	ASSERT_TRUE(res_base_.getCreateTimestamp(_time_stamp));
	EXPECT_TSEQ(_create_time, _time_stamp);
}

TEST_F(ResourceBaseAnncTest, JsonWOCreateTime) {
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

TEST_F(ResourceBaseAnncTest, TurnOnFixture) {
	setup = true;
}


TEST_F(ResourceBaseAnncTest, CheckLastModifyTime) {
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

TEST_F(ResourceBaseAnncTest, CheckDomainCsiRi) {
	ASSERT_STREQ(res_base_.getDomain().c_str(), "//microwireless.com");
	ASSERT_STREQ(res_base_.getIntCsi().c_str(), "/IN-CSE-01");
	ASSERT_STREQ(res_base_.getIntRi().c_str(), "Z0005");
	ASSERT_STREQ(res_base_.getResourceId().c_str(), "Z0005");
	ASSERT_STREQ(res_base_.getResourceName().c_str(), "IN-CSE-01");
}
*/
