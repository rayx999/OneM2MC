/*
 * test_AE.cc
 *
 *  Created on: 2015年8月23日
 *      Author: weimi_000
 */


#include <fstream>
#include <string>
#include <json2pb.h>
#include "gtest/gtest.h"
#include "test_utils.h"
#include "CSEBase.h"
#include "AE.h"

using namespace MicroWireless::OneM2M;

static bool setup = false;

class AETest : public ::testing::Test {
protected:
	static const string ae_json;
	static const string ae_ri;
	static const string id_str;

	AEClass ae_;
	ResourceStore<CSEBase> rdb;

	AETest() : ae_(), rdb("data/.store") {}

	virtual void SetUp() {
		if (setup) {
			rdb.setupRoot();
			ae_.setResourceBase(ae_json, id_str);
		}
	}

	// virtual void TearDown() {}
};

const string AETest::ae_json("{"
		"\"ty\" 	: 2,"
		"\"ri\" 	: \"Z0002\","
		"\"rn\" 	: \"AE-01\","
		"\"ae\"     : {"
			"\"apn\" 	: \"FreshGo\","
			"\"api\" 	: \"APP-01\","
			"\"aei\" 	: \"//microwireless.com/IN-CSE-01/AE-01\" "
		"}"
	"}");

const string AETest::ae_ri("Z0002");
const string AETest::id_str("//microwireless.com/IN-CSE-01/Z0002");

TEST_F(AETest, JsonValid) {
	try {
		AEClass ae_(ae_json, id_str);
		//ae_.outToResourceStore(rdb);
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(AETest, WrongResourceType) {
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
		AEClass ae_(json, "//microwireless.com/IN-CSE-01/Z0005");
	} catch (exception &e) {
		cout << "Excepted exception: " << e.what() << endl;
		return;
	}
	cerr << "Excepted exception doesn't occur." << endl;
	ASSERT_TRUE(false);
}

TEST_F(AETest, JsonNoDomain) {
	try {
		AEClass ae_(ae_json, "/IN-CSE-01/AE-01");
	} catch (exception &e) {
		cerr << "UnExpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
		return;
	}
}

TEST_F(AETest, JsonNoCSEIdInRi) {
	try {
		AEClass ae_(ae_json, id_str);
		ASSERT_STREQ(ae_.getAEId().c_str(), "//microwireless.com/IN-CSE-01/AE-01");
	} catch (exception &e) {
		cout << "Unexcepted exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(AETest, JsonBadDomain) {
	try {
		AEClass ae_(ae_json, "//microwirless/IN-CSE-01/AE-01");
	} catch (exception &e) {
		cout << "Excepted exception: " << e.what() << endl;
		return;
	}
	cerr << "Excepted exception doesn't occur." << endl;
	ASSERT_TRUE(false);
}
/*
TEST_F(AETest, JsonConflictAEId) {
	static const string json("{"
			"\"ty\" 	: 2,"
			"\"ri\" 	: \"Z0002\","
			"\"rn\" 	: \"AE01\","
			"\"ae\"     : {"
				"\"apn\" 	: \"FreshGo\","
				"\"api\" 	: \"APP-01\","
				"\"aei\" 	: \"//microwireless.com/IN-CSE-01/AE-01\" "
			"}"
		"}");

	try {
		AEClass ae_(json, id_str);
	} catch (exception &e) {
		cout << "Excepted exception: " << e.what() << endl;
		return;
	}
	cerr << "Excepted exception doesn't occur." << endl;
	ASSERT_TRUE(false);
}
*/
TEST_F(AETest, TurnOnFixture) {
	setup = true;
}

TEST_F(AETest, OutResourceToStore) {
	try {
		AEClass ae_(ae_json, id_str);
		ASSERT_TRUE(ae_.outToResourceStore(rdb));
	} catch (exception &e) {
		cout << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

// check if 2 AEClass the same
TEST_F(AETest, CheckOutResource) {
	AEClass ae_new("Z0002", rdb);

	ASSERT_STREQ(ae_.getAEId().c_str(), ae_new.getAEId().c_str());
	ASSERT_STREQ(ae_.getAppId().c_str(), ae_new.getAppId().c_str());
	ASSERT_STREQ(ae_.getAppName().c_str(), ae_new.getAppName().c_str());
	ASSERT_STREQ(ae_.getResourceId().c_str(), ae_new.getResourceId().c_str());
	ASSERT_STREQ(ae_.getResourceName().c_str(), ae_new.getResourceName().c_str());
/*
	TimeStamp _ct1, _ct2;
	ASSERT_TRUE(ae_.getCreateTimestamp(_ct1));
	ASSERT_TRUE(ae_new.getCreateTimestamp(_ct2));
	ASSERT_TSEQ(_ct1, _ct2);
*/
	// supported resource
}

TEST_F(AETest, GetAttributes) {
	ASSERT_STREQ(ae_.getAEId().c_str(), "//microwireless.com/IN-CSE-01/AE-01");
	ASSERT_STREQ(ae_.getResourceId().c_str(), "Z0002");
	ASSERT_STREQ(ae_.getResourceName().c_str(), "AE-01");
}

