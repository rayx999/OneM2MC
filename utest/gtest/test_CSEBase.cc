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
	static const string id_str;
	static const string json2;
	static const string id_str2;

	CSEBase cse_base;
	ResourceStore<CSEBase> rdb;

	CSEBaseTest() : cse_base(), rdb("data/.store") {}

	virtual void SetUp() {
		if (setup) {
			rdb.setupRoot();
			cse_base.setCSEBase(cse_ri, rdb);
		}
	}

	// virtual void TearDown() {}
};

const string CSEBaseTest::cse_ri("Z0005");
const string CSEBaseTest::id_str("//microwireless.com/IN-CSE-01/Z0005");

TEST_F(CSEBaseTest, JsonValid) {
	static const string json("{"
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

	try {
		CSEBase cse_base_(json, id_str);
		//cse_base_.outToResourceStore(rdb);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(CSEBaseTest, JsonNoDomain) {
	static const string json("{"
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

	try {
		CSEBase cse_base_(json, "/IN-CSE-01/IN-CSE-01");
	} catch (exception &e) {
		cout << "Excepted exception: " << e.what() << endl;
		return;
	}
	cerr << "Excepted exception doesn't occur." << endl;
	ASSERT_TRUE(false);
}

TEST_F(CSEBaseTest, JsonNoCSEIdInRi) {
	static const string json("{"
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

	try {
		CSEBase cse_base_(json, id_str);
		ASSERT_STREQ(cse_base_.getCSEId().c_str(), "/IN-CSE-01");
	} catch (exception &e) {
		cout << "Unexcepted exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(CSEBaseTest, JsonBadDomain) {
	static const string json("{"
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

	try {
		CSEBase cse_base_(json, "//microwirless/IN-CSE-01/IN-CSE-01");
	} catch (exception &e) {
		cout << "Excepted exception: " << e.what() << endl;
		return;
	}
	cerr << "Excepted exception doesn't occur." << endl;
	ASSERT_TRUE(false);
}

TEST_F(CSEBaseTest, JsonConflictCSEId) {
	static const string json("{"
			"\"ty\" 	: 5,"
			"\"ri\" 	: \"Z0005\","
			"\"rn\" 	: \"IN-CSE-01\","
			"\"ct\" 	: { \"seconds\" : 1435434103 },"
			"\"csb\"    : {"
				"\"cst\" 	: 1,"
				"\"csi\" 	: \"/IN-CSE-10\","
				"\"srt\" 	: [ 2, 5, 16 ]"
			"}"
		"}");

	try {
		CSEBase cse_base_(json, id_str);
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
		ASSERT_EQ(p_root_->getResourceType(), CSE_BASE);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

const string CSEBaseTest::json2("{"
		"\"ty\" 	: 5,"
		"\"ri\" 	: \"X0005\","
		"\"rn\" 	: \"IN-CSE-02\","
		"\"ct\" 	: { \"seconds\" : 1435434103 },"
		"\"csb\"    : {"
			"\"cst\" 	: 1,"
			"\"csi\" 	: \"/IN-CSE-02\","
			"\"srt\" 	: [ 2, 5, 16 ]"
		"}"
	"}");

const string CSEBaseTest::id_str2("//microwireless.com/in-cse-02/X0005");

TEST_F(CSEBaseTest, OutResourceToStore) {
	try {
		CSEBase cse_base_(json2, id_str2);
		ASSERT_TRUE(cse_base_.outToResourceStore(rdb));
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

// check if 2 CSEBase the same
TEST_F(CSEBaseTest, CheckOutResource) {
	CSEBase cse_base_(json2, id_str2);
	CSEBase cse_base_new("X0005", rdb);

	ASSERT_STREQ(cse_base_.getCSEId().c_str(), cse_base_new.getCSEId().c_str());
	ASSERT_EQ(cse_base_.getCSEType(), cse_base_new.getCSEType());
	ASSERT_STREQ(cse_base_.getResourceId().c_str(), cse_base_new.getResourceId().c_str());
	ASSERT_STREQ(cse_base_.getResourceName().c_str(), cse_base_new.getResourceName().c_str());

	TimeStamp _ct1, _ct2;
	ASSERT_TRUE(cse_base_.getCreateTimestamp(_ct1));
	ASSERT_TRUE(cse_base_new.getCreateTimestamp(_ct2));
	ASSERT_TSEQ(_ct1, _ct2);

	// supported resource
}

TEST_F(CSEBaseTest, TurnOnFixture) {
	setup = true;
}

TEST_F(CSEBaseTest, GetAttributes) {
	ASSERT_STREQ(cse_base.getCSEId().c_str(), "/IN-CSE-01");
	ASSERT_EQ(cse_base.getCSEType(), IN_CSE);
	ASSERT_STREQ(cse_base.getResourceId().c_str(), "Z0005");
	ASSERT_STREQ(cse_base.getResourceName().c_str(), "IN-CSE-01");
	ASSERT_STREQ(cse_base.getDomain().c_str(), "//microwireless.com");
}

TEST_F(CSEBaseTest, CheckSupportedResourceType) {
	ASSERT_TRUE(cse_base.isResourceSupported(CSE_BASE));
	ASSERT_FALSE(cse_base.isResourceSupported(FAN_OUT_POINT));
}

TEST_F(CSEBaseTest, GetSupportedResource) {
	SupportedResourceType *_rt;
	int size = cse_base.getSupportedResource(_rt);
	ASSERT_EQ(size, 3);
	SupportedResourceType _srt[] = {AE,CSE_BASE,REMOTE_CSE};
	UTest::ASSERT_ARREQ(_rt, _srt, 3);
}

