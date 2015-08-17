/*
 * test_Request.cc
 *
 *  Created on: 2015年7月17日
 *      Author: weimi_000
 */

#include <iostream>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include "test_utils.h"
#include "RequestPrim.h"

using namespace MicroWireless::OneM2M;

static bool setup = false;

class RequestPrimTest : public ::testing::Test {
	protected:
	static const string request_json;
	static const string to_;
	static const string fr_;
	static const string rqi_;
	static const string domain_;
	static const string csi_;
	static const string rn_;
	Operation op_;
	RequestPrim * p_req_;

	RequestPrimTest() {
		op_ = OPERATION_RETRIEVE;
		p_req_ = NULL;
	}

	virtual void SetUp() {
		if (setup) {
			p_req_ = new RequestPrim(request_json);
		}
	}

	void try_false(const string& json) {
		try {
			RequestPrim request_(json);
		} catch (exception &e) {
			cout << "Expected exception: " << e.what() << endl;
			return;
		}
		cout << "Expected exception dowsn't occur.\n";
		ASSERT_TRUE(false);
	}

	void try_false(Operation op, const string& to, const string& fr, const string& rqi) {
		try {
			RequestPrim request_(op, to, fr, rqi);
		} catch (exception &e) {
			cout << "Expected exception: " << e.what() << endl;
			return;
		}
		cout << "Expected exception dowsn't occur.\n";
		ASSERT_TRUE(false);
	}

	virtual void TearDown() {
		if (p_req_ != NULL) {
			delete p_req_;
			p_req_ = NULL;
		}
	}
};

const string RequestPrimTest::request_json("{"
			"\"op\": 2, "
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");

const string RequestPrimTest::to_("//microwireless.com/IN-CSE-01/Z0005");
const string RequestPrimTest::fr_("//microwireless.com/AE-01");
const string RequestPrimTest::rqi_("ab3f124a");
const string RequestPrimTest::domain_("//microwireless.com");
const string RequestPrimTest::csi_("/IN-CSE-01");
const string RequestPrimTest::rn_("IN-CSE-01");

TEST_F(RequestPrimTest, FullCtor) {
	try {
		RequestPrim request_(op_, to_, fr_, rqi_);
		ASSERT_TRUE(request_.isValid());
	} catch (exception &e) {
		cerr << "Unexpected exception:" << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(RequestPrimTest, CtorNoTo) {
	string to_none;
	try_false(op_, to_none, fr_, rqi_);
}

TEST_F(RequestPrimTest, CtorNoRqi) {
	string rqi_none;
	try_false(op_, to_, fr_, rqi_none);
}

TEST_F(RequestPrimTest, CtorNoFr) {
	string fr_none;
	try_false(op_, to_, fr_none, rqi_);
}

TEST_F(RequestPrimTest, JsonNormal) {
	try {
		RequestPrim request_(request_json);
		ASSERT_TRUE(request_.isValid());
	} catch (exception &e) {
		cerr << "Unexpected exception:" << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(RequestPrimTest, JsonInvalidOp) {
	string json("{"
			"\"op\": 10, "
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");

	try_false(json);
}

TEST_F(RequestPrimTest, JsonNoTo) {
	string json("{"
			"\"op\": 2, "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");

	try_false(json);
}

TEST_F(RequestPrimTest, JsonNoFr) {
	string json("{"
			"\"op\": 2, "
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
			"\"rqi\": \"ab3f124a\" "
		"}");

	try_false(json);
}

TEST_F(RequestPrimTest, JsonNoRqi) {
	string json("{"
			"\"op\": 2, "
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");

	try_false(json);
}

TEST_F(RequestPrimTest, RetrieveWithResourceType) {
	string json("{"
			"\"op\": 2, "
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\", "
			"\"ty\": 1 "
		"}");

	try_false(json);
}

TEST_F(RequestPrimTest, RetrieveWithName) {
	string json("{"
			"\"op\": 2, "
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\", "
			"\"nm\": \"Name\" "
		"}");

	try_false(json);
}

TEST_F(RequestPrimTest, TurnOnFixture) {
	setup = true;
}

TEST_F(RequestPrimTest, SetGetResourceType) {
	ASSERT_TRUE(p_req_->setResourceType(NORMAL));
	ASSERT_EQ(p_req_->getResourceType(), NORMAL);
}

TEST_F(RequestPrimTest, GetAttributes) {
	ASSERT_EQ(p_req_->getOperation(), OPERATION_RETRIEVE);
	ASSERT_STREQ(p_req_->getTo().c_str(), to_.c_str());
	ASSERT_STREQ(p_req_->getFrom().c_str(), fr_.c_str());
	ASSERT_STREQ(p_req_->getRequestId().c_str(), rqi_.c_str());
}

TEST_F(RequestPrimTest, GetIdInfoAndTargetResource) {
	string domainC_, csiC_;

	p_req_->getIdInfo(domainC_, csiC_);

	ASSERT_STREQ(domainC_.c_str(), domain_.c_str());
	ASSERT_STREQ(csiC_.c_str(), csi_.c_str());
	//ASSERT_STREQ(p_req_->getTargetResource().c_str(), rn_.c_str());
}
