/*
 * test_Request.cc
 *
 *  Created on: 2015年8月17日
 *      Author: weimi_000
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "test_utils.h"
#include "RequestPrim.h"
#include "Request.h"

using namespace MicroWireless::OneM2M;
using namespace std;

static int setup = 0;

class RequestTest : public ::testing::Test {
	protected:
	static const string request_json;
	static const string ol_json;
	static const string to_;
	static const string fr_;
	static const string rid_;
	static const string rn_;
	static const string ri_;

	RequestPrim * p_reqp_;
	Request * p_req_;

	RequestTest() {
		p_reqp_ = NULL;
		p_req_ = NULL;
	}

	virtual void SetUp() {
		if (setup >= 1) {
			p_reqp_ = new RequestPrim(request_json);
			p_req_ = new Request(*p_reqp_, ri_, p_reqp_->getIntRn());
			if (setup > 1) {
				pb::OperationResult* p_ol_ = new pb::OperationResult();
				json2pb(*p_ol_, ol_json.c_str(), ol_json.length());
				p_req_->setOperationResult(p_ol_);
			}
		}
	}

	virtual void TearDown() {
		if (p_reqp_ != NULL) {
			delete p_reqp_;
			p_reqp_ = NULL;
		}
		if (p_req_ != NULL) {
			delete p_req_;
			p_req_ = NULL;
		}
	}
};

const string RequestTest::request_json("{"
			"\"op\": 2, "
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/AE-01\", "
			"\"pc\": \"ct, lt\" "
		"}");

const string RequestTest::ol_json=("{"
		"\"rsc\": 2000, "
		"\"to\": \"//microwireless.com/AE-01\", "
		"\"rqi\": \"ab3f124a\", "
		"\"fr\": \"//microwireless.com/IN-CSE-01\", "
		"\"pc\": \"Content\", "
		"\"ot\" 	: { \"seconds\" : 1435434103 }"
	"}");

const string RequestTest::to_("//microwireless.com/IN-CSE-01/Z0005");
const string RequestTest::fr_("//microwireless.com/AE-01");
const string RequestTest::rid_("ab3f124a");
const string RequestTest::rn_("IN-CSE-01");
const string RequestTest::ri_("REQ24543");

TEST_F(RequestTest, FullCtor) {
	try {
		RequestPrim reqp_(Operation::RETRIEVE, to_, fr_, rid_);
		ASSERT_TRUE(reqp_.isValid());

		Request req_(reqp_, ri_, reqp_.getIntRn());
	} catch (exception &e) {
		cerr << "Unexpected exception:" << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(RequestTest, TurnOnFixture) {
	setup++;
}

TEST_F(RequestTest, CheckAttributes) {
	ASSERT_EQ(p_req_->getResourceType(), SupportedResourceType::REQUEST);
	ASSERT_STREQ(p_req_->getResourceId().c_str(), ri_.c_str());
	ASSERT_STREQ(p_req_->getParentId().c_str(), p_reqp_->getIntRn().c_str());

	ASSERT_EQ(p_req_->getOperation(), Operation::RETRIEVE);
	ASSERT_STREQ(p_req_->getTarget().c_str(), p_reqp_->getTo().c_str());
	ASSERT_STREQ(p_req_->getOriginator().c_str(), p_reqp_->getFrom().c_str());
	ASSERT_STREQ(p_req_->getRequestId().c_str(), p_reqp_->getRequestId().c_str());
	ASSERT_STREQ(p_req_->getContent().c_str(), p_reqp_->getContent().c_str());
	ASSERT_EQ(p_req_->getRequestStatus(), RequestStatus::PENDING);
}

TEST_F(RequestTest, SetRequestStatus) {
	p_req_->setRequestStatus(RequestStatus::FORWARDED);
	ASSERT_EQ(p_req_->getRequestStatus(), RequestStatus::FORWARDED);
}

TEST_F(RequestTest, NoOperationResult) {
	pb::OperationResult ol_;
	ASSERT_FALSE(p_req_->getOperationResult(ol_));
}

TEST_F(RequestTest, NoOperationResultRsc) {
	ResponseStatusCode rsc_;
	ASSERT_FALSE(p_req_->getOperationResult_rsc(rsc_));
}

TEST_F(RequestTest, NoOperationResultTo) {
	string str_;
	ASSERT_FALSE(p_req_->getOperationResult_to(str_));
}

TEST_F(RequestTest, NoOperationResultFr) {
	string str_;
	ASSERT_FALSE(p_req_->getOperationResult_fr(str_));
}

TEST_F(RequestTest, NoOperationResultRqi) {
	string str_;
	ASSERT_FALSE(p_req_->getOperationResult_rqi(str_));
}

TEST_F(RequestTest, NoOperationResultPc) {
	string str_;
	ASSERT_FALSE(p_req_->getOperationResult_pc(str_));
}

TEST_F(RequestTest, SetOperationResult) {
	static const string ol_json=("{"
			"\"rsc\": 2000, "
			"\"to\": \"//microwireless.com/AE-01\", "
			"\"rqi\": \"ab3f124a\", "
			"\"fr\": \"//microwireless.com/IN-CSE-01\", "
			"\"pc\": \"Content\", "
			"\"ot\" 	: { \"seconds\" : 1435434103 }"
		"}");

	try {
		pb::OperationResult* p_ol_ = new pb::OperationResult();

		ASSERT_TRUE(p_ol_ != NULL);
		json2pb(*p_ol_, ol_json.c_str(), ol_json.length());
		ASSERT_TRUE(p_req_->setOperationResult(p_ol_));

		ResponseStatusCode rsc_;
		ASSERT_TRUE(p_req_->getOperationResult_rsc(rsc_));
		ASSERT_EQ(rsc_, RSC_OK);

		string str_;
		ASSERT_TRUE(p_req_->getOperationResult_to(str_));
		ASSERT_STREQ(str_.c_str(), "//microwireless.com/AE-01");

		ASSERT_TRUE(p_req_->getOperationResult_fr(str_));
		ASSERT_STREQ(str_.c_str(), "//microwireless.com/IN-CSE-01");

		ASSERT_TRUE(p_req_->getOperationResult_rqi(str_));
		ASSERT_STREQ(str_.c_str(), "ab3f124a");

		ASSERT_TRUE(p_req_->getOperationResult_pc(str_));
		ASSERT_STREQ(str_.c_str(), "Content");

	} catch (exception &e) {
		cerr << "UnExpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(RequestTest, TurnOnOperationResult) {
	setup++;
}

TEST_F(RequestTest, SetOperationResultRsc) {
	ResponseStatusCode rsc_;
	ASSERT_TRUE(p_req_->setOperationResult_rsc(RSC_NOT_FOUND));
	ASSERT_TRUE(p_req_->getOperationResult_rsc(rsc_));
    ASSERT_EQ(rsc_, RSC_NOT_FOUND);
}

TEST_F(RequestTest, SetOperationResultTo) {
	string set_("//wonderful.one.m2m");
	string get_;
	ASSERT_TRUE(p_req_->setOperationResult_to(set_));
	ASSERT_TRUE(p_req_->getOperationResult_to(get_));
    ASSERT_STREQ(get_.c_str(), set_.c_str());
}

TEST_F(RequestTest, SetOperationResultFr) {
	string set_("//wonderful.one.m2m");
	string get_;
	ASSERT_TRUE(p_req_->setOperationResult_fr(set_));
	ASSERT_TRUE(p_req_->getOperationResult_fr(get_));
    ASSERT_STREQ(get_.c_str(), set_.c_str());
}

TEST_F(RequestTest, SetOperationResultPc) {
	string set_("//wonderful.one.m2m");
	string get_;
	ASSERT_TRUE(p_req_->setOperationResult_pc(set_));
	ASSERT_TRUE(p_req_->getOperationResult_pc(get_));
    ASSERT_STREQ(get_.c_str(), set_.c_str());
}
