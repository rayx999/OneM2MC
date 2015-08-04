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
#include "Request.h"

using namespace MicroWireless::OneM2M;

class RequestTest : public ::testing::Test {
	protected:
	static const string request_json;
	static const string to_;
	static const string fr_;
	static const string rqi_;
	static const string domain_;
	static const string csi_;
	static const string rn_;
	Operation op_;
	Request * p_request_;

	RequestTest() {
		op_ = OPERATION_RETRIEVE;
		p_request_ = NULL;
	}

	virtual void SetUp() {
		p_request_ = new Request(request_json);
	}

	Operation getOperation() {
		return p_request_->getOperation();
	}

	const string & getTo() {
		return p_request_->getTo();
	}

	const string & getFrom() {
		return p_request_->getFrom();
	}

	const string & getRequestId() {
		return p_request_->getRequestId();
	}

	bool setResourceType(ResourceType ty) {
		return p_request_->setResourceType(ty);
	}

	ResourceType getResourceType() {
		return p_request_->getResourceType();
	}

	virtual void TearDown() {
		if (p_request_ == NULL) {
			delete p_request_;
			p_request_ = NULL;
		}
	}
};

const string RequestTest::request_json("{\"op\": 2, \"to\": \"//microwireless.com/IN-CSE-01/CSEBase\", \"rqi\": \"ab3f124a\", \"fr\": \"//microwireless.com/AE-01\"}");
const string RequestTest::to_("//microwireless.com/IN-CSE-01/CSEBase");
const string RequestTest::fr_("//microwireless.com/AE-01");
const string RequestTest::rqi_("ab3f124a");
const string RequestTest::domain_("//microwireless.com");
const string RequestTest::csi_("/IN-CSE-01");
const string RequestTest::rn_("CSEBase");

TEST_F(RequestTest, FullCtor) {
	Request request_(op_, to_, fr_, rqi_);
	ASSERT_TRUE(request_.isValid());
}

TEST_F(RequestTest, CtorNoTo) {
	string to_none;

	Request request_(op_, to_none, fr_, rqi_);
	ASSERT_FALSE(request_.isValid());
}

TEST_F(RequestTest, CtorNoRqi) {
	string rqi_none;

	Request request_(op_, to_, fr_, rqi_none);
	ASSERT_FALSE(request_.isValid());
}

TEST_F(RequestTest, CtorNoFr) {
	string fr_none;

	Request request_(op_, to_, fr_none, rqi_);
	ASSERT_FALSE(request_.isValid());
}

TEST_F(RequestTest, JsonNormal) {
	Request request_(request_json);
	ASSERT_TRUE(request_.isValid());
}

TEST_F(RequestTest, JsonInvalidOp) {
	string json("{\"op\": 10, \"to\": \"//microwireless.com/IN-CSE-01\", \"rqi\": \"ab3f124a\", \"fr\": \"//microwireless.com/AE-01\"}");

	Request request_(json);
	ASSERT_FALSE(request_.isValid());
}

TEST_F(RequestTest, JsonNoTo) {
	string json("{\"op\": 2, \"rqi\": \"ab3f124a\", \"fr\": \"//microwireless.com/AE-01\"}");

	Request request_(json);
	ASSERT_FALSE(request_.isValid());
}

TEST_F(RequestTest, JsonNoFr) {
	string json("{\"op\": 2, \"to\": \"//microwireless.com/IN-CSE-01\", \"rqi\": \"ab3f124a\"}");

	Request request_(json);
	ASSERT_FALSE(request_.isValid());
}

TEST_F(RequestTest, JsonNoRqi) {
	string json("{\"op\": 2, \"to\": \"//microwireless.com/IN-CSE-01\", \"fr\": \"//microwireless.com/AE-01\"}");

	Request request_(json);
	ASSERT_FALSE(request_.isValid());
}

TEST_F(RequestTest, RetrieveWithResourceType) {
	string json("{\"op\": 2, \"to\": \"//microwireless.com/IN-CSE-01\", \"rqi\": \"ab3f124a\", \"fr\": \"//microwireless.com/AE-01\", \"ty\": 1}");

	Request request_(json);
	ASSERT_FALSE(request_.isValid(VALIDATE_ALL));
}

TEST_F(RequestTest, RetrieveWithName) {
	string json("{\"op\": 2, \"to\": \"//microwireless.com/IN-CSE-01\", \"rqi\": \"ab3f124a\", \"fr\": \"//microwireless.com/AE-01\", \"nm\": \"Name\" }");

	Request request_(json);
	ASSERT_FALSE(request_.isValid(VALIDATE_ALL));
}

TEST_F(RequestTest, SetGetResourceType) {
	ASSERT_TRUE(setResourceType(NORMAL));
	ASSERT_EQ(getResourceType(), NORMAL);
}

TEST_F(RequestTest, GetAttributes) {
	ASSERT_EQ(getOperation(), OPERATION_RETRIEVE);
	ASSERT_STREQ(getTo().c_str(), to_.c_str());
	ASSERT_STREQ(getFrom().c_str(), fr_.c_str());
	ASSERT_STREQ(getRequestId().c_str(), rqi_.c_str());
}

TEST_F(RequestTest, GetIdInfoAndTargetResource) {
	string domainC_, csiC_;

	p_request_->getIdInfo(domainC_, csiC_);

	ASSERT_STREQ(domainC_.c_str(), domain_.c_str());
	ASSERT_STREQ(csiC_.c_str(), csi_.c_str());
	ASSERT_STREQ(p_request_->getTargetResource().c_str(), rn_.c_str());
}
