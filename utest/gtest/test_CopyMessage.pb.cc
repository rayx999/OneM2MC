/*
 * test_CopyMessage.cc
 *
 *  Created on: 2015年9月20日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include <json2pb.h>

#include "gtest/gtest.h"
#include "test_utils.h"
#include "ResourceBase.pb.h"
#include "RequestPrim.pb.h"
#include "CopyMessage.pb.h"

using namespace std;
using namespace MicroWireless::OneM2M;

using google::protobuf::CopyMessage;
using google::protobuf::compareMessage;

class CopyMessageTest : public ::testing::Test {
protected:
	static const string json;
	pb::ResourceBase src_, dst_;

	virtual void SetUp() {}
	// virtual void TearDown() {}
};

const string CopyMessageTest::json("{"
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

TEST_F(CopyMessageTest, ComparePositive) {
	json2pb(src_, json.c_str(), json.length());
	json2pb(dst_, json.c_str(), json.length());

	ASSERT_TRUE(compareMessage(src_, dst_));
}

TEST_F(CopyMessageTest, CompareNegative) {
	json2pb(src_, json.c_str(), json.length());
	json2pb(dst_, json.c_str(), json.length());
	dst_.set_ri("XXXXX");

	ASSERT_FALSE(compareMessage(src_, dst_));
}

TEST_F(CopyMessageTest, CompareNegative1) {
	json2pb(src_, json.c_str(), json.length());
	json2pb(dst_, json.c_str(), json.length());
	dst_.clear_ri();

	ASSERT_FALSE(compareMessage(src_, dst_));
}

TEST_F(CopyMessageTest, CompareNegative2) {
	const string json1("{"
			"\"op\": 2, "
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");
	pb::RequestPrim reqp_;
	json2pb(src_, json.c_str(), json.length());
	json2pb(reqp_, json1.c_str(), json1.length());

	ASSERT_FALSE(compareMessage(src_, reqp_));
}

TEST_F(CopyMessageTest, FullCopy) {
	json2pb(src_, json.c_str(), json.length());
	google::protobuf::CopyMessage cp_(src_, dst_);
	cp_.copyFields();

	ASSERT_TRUE(compareMessage(src_, dst_));
}

TEST_F(CopyMessageTest, FullCopy1) {
	std::set<std::string> ma_ = {"ty", "ri", "rn", "ct", "csb"};

	json2pb(src_, json.c_str(), json.length());
	google::protobuf::CopyMessage cp_(src_, dst_);
	cp_.copyFields(ma_);

	ASSERT_TRUE(ma_.size() == 0);
	ASSERT_TRUE(compareMessage(src_, dst_));
}

TEST_F(CopyMessageTest, PartialCopy) {
	std::set<std::string> ma_ = {"ty", "ri", "rn", "ct"};
	pb::ResourceBase cmp_;

	json2pb(src_, json.c_str(), json.length());
	json2pb(cmp_, json.c_str(), json.length());
	cmp_.clear_csb();

	google::protobuf::CopyMessage cp_(src_, dst_);
	cp_.copyFields(ma_);

	ASSERT_TRUE(ma_.size() == 0);
	ASSERT_TRUE(compareMessage(dst_, cmp_));
}

TEST_F(CopyMessageTest, CopyInvalidField) {
	std::set<std::string> ma_ = {"ty", "ri", "rn", "blah", "ct", "csb"};

	json2pb(src_, json.c_str(), json.length());
	google::protobuf::CopyMessage cp_(src_, dst_);
	cp_.copyFields(ma_);

	ASSERT_TRUE(ma_.size() == 1);
	ASSERT_FALSE(ma_.find("blah") == ma_.end());
	ASSERT_TRUE(compareMessage(src_, dst_));
}
