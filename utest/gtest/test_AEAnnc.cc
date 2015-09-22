/*
 * test_AEAnnc.cc
 *
 *  Created on: 2015年9月21日
 *      Author: weimi_000
 */


#include <iostream>
#include <string>
#include <json2pb.h>
#include "gtest/gtest.h"
#include "test_utils.h"
#include "CommonTypes.h"
#include "AE.h"
#include "ResourceBase.pb.h"
#include "AEAnnc.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class AEAnncTest : public ::testing::Test {
protected:
	static const string valid_json;
	static const string valid_path;

	AEClass src_;

	AEAnncTest() : src_() {}
	virtual ~AEAnncTest() {}

	virtual void SetUp() {
	}

	virtual void TearDown() {}
};

const string AEAnncTest::valid_json("{"
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

const string AEAnncTest::valid_path("//microwireless.com/IN-CSE-01/AE-01");

TEST_F(AEAnncTest, CheckAll) {
	const string tgt_json("{"
			"\"ri\" 	: \"Z0002\","
			"\"rn\" 	: \"AE-01\","
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"acpi\"   : [ \"001-50294\", \"001-21850\" ],"
			"\"lbl\"    : [ \"test\" ],"
			"\"aeA\"    : {"
				"\"apn\" 	: \"FreshGo\","
				"\"api\" 	: \"APP-01\","
				"\"aei\" 	: \"AE-01\" "
			"}"
	"}");

	try {
		AEClass src_(valid_json, valid_path);
		AEAnnc annc_(src_);
		pb::ResourceBase tgt_;

		json2pb(tgt_, tgt_json.c_str(), tgt_json.length());
		ASSERT_TRUE(annc_.compare(tgt_));
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(AEAnncTest, CheckMAMissingMA) {
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

	try {
		AEClass src_(json_no_acpi, valid_path);
		AEAnnc annc_(src_);
	} catch (exception &e) {
		cout << "Expected exception: " << e.what() << endl;
		return;
	}
	cerr << "Expected exception didn't happen.\n";
	ASSERT_TRUE(false);
}

TEST_F(AEAnncTest, CheckMAMissingMA1) {
	const string json_no_rn("{"
			"\"ty\" 	: 2,"
			"\"ri\" 	: \"Z0002\","
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"acpi\"   : [ \"001-50294\", \"001-21850\" ],"
			"\"lbl\"    : [ \"test\" ],"
			"\"ae\"     : {"
				"\"apn\" 	: \"FreshGo\","
				"\"api\" 	: \"APP-01\","
				"\"aei\" 	: \"AE-01\" "
			"}"
	"}");

	try {
		AEClass src_(json_no_rn, "//microwireless.com/IN-CSE-01/Z0002");
		AEAnnc annc_(src_);
	} catch (exception &e) {
		cout << "Expected exception: " << e.what() << endl;
		return;
	}
	cerr << "Expected exception didn't happen.\n";
	ASSERT_TRUE(false);
}

TEST_F(AEAnncTest, CheckMAMissingOA) {
	const string json_no_api("{"
			"\"ty\" 	: 2,"
			"\"ri\" 	: \"Z0002\","
			"\"rn\" 	: \"AE-01\","
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"acpi\"   : [ \"001-50294\", \"001-21850\" ],"
			"\"lbl\"    : [ \"test\" ],"
			"\"ae\"     : {"
				"\"apn\" 	: \"FreshGo\","
				"\"aei\" 	: \"AE-01\" "
			"}"
	"}");

	const string tgt_json("{"
			"\"ri\" 	: \"Z0002\","
			"\"rn\" 	: \"AE-01\","
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"acpi\"   : [ \"001-50294\", \"001-21850\" ],"
			"\"lbl\"    : [ \"test\" ],"
			"\"aeA\"    : {"
				"\"apn\" 	: \"FreshGo\","
				"\"aei\" 	: \"AE-01\" "
			"}"
	"}");

	try {
		AEClass src_(json_no_api, valid_path);
		pb::ResourceBase tgt_;
		json2pb(tgt_, tgt_json.c_str(), tgt_json.length());
		AEAnnc annc_(src_);
		ASSERT_TRUE(annc_.compare(tgt_));
	} catch (exception &e) {
		cerr << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}
