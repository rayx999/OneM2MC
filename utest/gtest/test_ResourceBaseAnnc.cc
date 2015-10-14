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
#include "ResourceBase.pb.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class ResourceBaseAnncTest : public ::testing::Test {
protected:
	static const string valid_json;
	static const string valid_path;

	AnncAttr ma_, oa_, empty_;
	ResourceBase src_;
	ResourceBase annc_;

	ResourceBaseAnncTest() : src_(), annc_() {}
	virtual ~ResourceBaseAnncTest() {}

	virtual void SetUp() {
		ma_ = {"ri",  "rn",  "et",  "acpi", "lbl" };
		oa_ = { "st", "apn", "api", "aei", "poa",  "or", "nl" };
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
	pb::ResourceBase tgt_;
	ResourceBase annc_;

	try {
		ResourceBase src_(valid_json, valid_path);
		json2pb(tgt_, valid_json.c_str(), valid_json.length());
		tgt_.clear_ty();
		tgt_.clear_ae();

		annc_.copyAnncFields(src_, ma_, empty_);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
	ASSERT_TRUE(ma_.size() == 0);
	ASSERT_TRUE(annc_.compare(tgt_, true));
}

TEST_F(ResourceBaseAnncTest, CheckMAMissing) {
	const string json_no_rn("{"
			"\"ty\" 	: 2,"
			"\"ri\" 	: \"Z0002\","
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"lbl\"    : [ \"test\" ],"
			"\"acpi\"   : [ \"001-50294\", \"001-21850\" ],"
			"\"ae\"     : {"
				"\"apn\" 	: \"FreshGo\","
				"\"api\" 	: \"APP-01\","
				"\"aei\" 	: \"AE-01\" "
			"}"
	"}");

	ResourceBase annc_;

	try {
		ResourceBase src_(json_no_rn, "//microwireless.com/IN-CSE-01/Z0002");
		annc_.copyAnncFields(src_, ma_, empty_);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
	ASSERT_TRUE(ma_.size() == 1);
	ASSERT_FALSE(ma_.find("rn") == ma_.end());
}

TEST_F(ResourceBaseAnncTest, CheckMAMissing1) {
	const string json_no_acpi("{"
			"\"ty\" 	: 2,"
			"\"ri\" 	: \"Z0002\","
			"\"rn\" 	: \"AE-01\","
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"lbl\"    : [ \"test\" ],"
			"\"ae\"     : {"
				"\"apn\" 	: \"FreshGo\","
				"\"api\" 	: \"APP-01\","
				"\"aei\" 	: \"AE-01\" "
			"}"
	"}");

	ResourceBase annc_;

	try {
		ResourceBase src_(json_no_acpi, "//microwireless.com/IN-CSE-01/Z0002");
		annc_.copyAnncFields(src_, ma_, empty_);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
	ASSERT_TRUE(ma_.size() == 1);
	ASSERT_FALSE(ma_.find("acpi") == ma_.end());
}

TEST_F(ResourceBaseAnncTest, CheckOA) {
	const string json_st("{"
			"\"ty\" 	: 2,"
			"\"ri\" 	: \"Z0002\","
			"\"rn\" 	: \"AE-01\","
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"acpi\"   : [ \"001-50294\", \"001-21850\" ],"
			"\"lbl\"    : [ \"test\" ],"
			"\"st\"     : 2,"
			"\"ae\"     : {"
				"\"apn\" 	: \"FreshGo\","
				"\"api\" 	: \"APP-01\","
				"\"aei\" 	: \"AE-01\" "
			"}"
	"}");
	pb::ResourceBase tgt_;
	ResourceBase annc_;

	try {
		ResourceBase src_(json_st, valid_path);
		json2pb(tgt_, json_st.c_str(), json_st.length());
		tgt_.clear_ty();
		tgt_.clear_ae();

		annc_.copyAnncFields(src_, ma_, oa_);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
	ASSERT_TRUE(ma_.size() == 0);
	ASSERT_TRUE(oa_.size() == 6);
	ASSERT_TRUE(oa_.find("st") == oa_.end());
	ASSERT_TRUE(annc_.compare(tgt_, true));
}
