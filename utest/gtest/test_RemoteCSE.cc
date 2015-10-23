/*
 * test_RemoteCSE.cc
 *
 *  Created on: 2015年10月23日
 *      Author: weimi_000
 */


#include <fstream>
#include <string>
#include <json2pb.h>
#include "gtest/gtest.h"
#include "CopyMessage.pb.h"
#include "test_utils.h"
#include "CSEBase.h"
#include "RemoteCSE.h"

using namespace MicroWireless::OneM2M;

static bool setup = false;

class RemoteCSETest : public ::testing::Test {
protected:
	static const string csr_json;
	static const string csr_ri;
	static const string id_str;

	RemoteCSE csr_;
	ResourceStore<CSEBase> rdb;

	RemoteCSETest() : csr_(), rdb("data/.store") {}

	virtual void SetUp() {
		if (setup) {
			rdb.setupRoot();
			csr_.setResourceBase(csr_json, id_str);
		}
	}

	// virtual void TearDown() {}
};

const string RemoteCSETest::csr_json("{"
		"\"ty\" 	: 16,"
		"\"ri\" 	: \"00016-23637\","
		"\"rn\" 	: \"AN-CSE-01\","
		"\"csr\"     : {"
			"\"cst\" 	: 3,"
			"\"csi\" 	: \"Z00016\","
			"\"rr\"		: false, "
			"\"cb\" 	: \"//microwireless.com/AN-CSE-01\" "
		"}"
	"}");

const string RemoteCSETest::csr_ri("00016-23637");
const string RemoteCSETest::id_str("//microwireless.com/IN-CSE-01/00016-23637");

TEST_F(RemoteCSETest, JsonValid) {
	try {
		RemoteCSE csr_(csr_json, id_str);
		//csr_.outToResourceStore(rdb);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(RemoteCSETest, WrongResourceType) {
	static const string json("{"
			"\"ty\" 	: 2,"
			"\"ri\" 	: \"00016-23637\","
			"\"rn\" 	: \"AN-CSE-01\","
			"\"csr\"     : {"
				"\"cst\" 	: 3,"
				"\"csi\" 	: \"Z00016\","
				"\"rr\"		: false, "
				"\"cb\" 	: \"//microwireless.com/AN-CSE-01\" "
			"}"
		"}");

	try {
		RemoteCSE csr_(json, id_str);
	} catch (exception &e) {
		cout << "Excepted exception: " << e.what() << endl;
		return;
	}
	cerr << "Excepted exception doesn't occur." << endl;
	ASSERT_TRUE(false);
}

TEST_F(RemoteCSETest, JsonNoDomain) {
	try {
		RemoteCSE csr_(csr_json, "/IN-CSE-01/00016-23637");
	} catch (exception &e) {
		cerr << "UnExpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
		return;
	}
}

TEST_F(RemoteCSETest, JsonBadDomain) {
	try {
		RemoteCSE csr_(csr_json, "//microwirless/IN-CSE-01/00016-23637");
	} catch (exception &e) {
		cout << "Excepted exception: " << e.what() << endl;
		return;
	}
	cerr << "Excepted exception doesn't occur." << endl;
	ASSERT_TRUE(false);
}

TEST_F(RemoteCSETest, TurnOnFixture) {
	setup = true;
}

TEST_F(RemoteCSETest, OutResourceToStore) {
	try {
		RemoteCSE csr_(csr_json, id_str);
		ASSERT_TRUE(csr_.outToResourceStore(rdb));
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

// check if 2 RemoteCSE the same
TEST_F(RemoteCSETest, CheckOutResource) {
	RemoteCSE csr_new("00016-23637", rdb);

	ASSERT_TRUE(csr_.compare(const_cast<pb::ResourceBase&>(csr_new.getResourceBase()), true, true));
}

TEST_F(RemoteCSETest, GetAttributes) {
	ASSERT_EQ(csr_.getCSEType(), CSEType::ASN_CSE);
	ASSERT_STREQ(csr_.getCSEBase().c_str(), "//microwireless.com/AN-CSE-01");
	ASSERT_STREQ(csr_.getCSEId().c_str(), "Z00016");
	ASSERT_STREQ(csr_.getResourceId().c_str(), "00016-23637");
	ASSERT_STREQ(csr_.getResourceName().c_str(), "AN-CSE-01");
	ASSERT_FALSE(csr_.getRequestReachability());
}

