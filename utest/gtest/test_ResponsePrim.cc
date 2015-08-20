/*
 * test_Response.cc
 *
 *  Created on: 2015年7月18日
 *      Author: weimi_000
 */
#include <ResponsePrim.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include "test_utils.h"

using namespace MicroWireless::OneM2M;

class ResponsePrimTest : public ::testing::Test {
	protected:
	static const string request_json;
	static const string response_json;
	static const string rqi_;
	static const string to_;
	static const string fr_;
	string pc_;
	ResponseStatusCode rsc_ok_;
	ResponsePrim * p_rsp_;
	RequestPrim * p_request_;

	ResponsePrimTest() {
		rsc_ok_ = RSC_OK;
		pc_ = request_json;
		p_rsp_ = NULL;
		p_request_ = NULL;
	}

	virtual void SetUp() {
		p_request_ = new RequestPrim(request_json);
		p_rsp_ = new ResponsePrim(new RequestPrim(request_json), response_json);
	}

	virtual void TearDown() {
		DeleteRequestPtr();
	}

	inline void DeleteRequestPtr() {
		if (p_request_ != NULL) {
			delete p_request_;
			p_request_ = NULL;
		}
	}
};

const string ResponsePrimTest::request_json("{"
		"\"op\": 2, "
		"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\", "
		"\"rqi\": \"ab3f124a\", "
		"\"fr\": \"//microwireless.com/AE-01\""
	"}");

const string ResponsePrimTest::response_json("{\"rsc\": 2000, \"rqi\": \"ab3f124a\"}");
const string ResponsePrimTest::rqi_("ab3f124a");
const string ResponsePrimTest::to_("//microwireless.com/IN-CSE-01/Z0005");
const string ResponsePrimTest::fr_("//microwireless.com/AE-01");

TEST_F(ResponsePrimTest, FullCtor) {
	try {
		ResponsePrim response_(p_request_, rsc_ok_, rqi_);
	} catch (const exception &e) {
		cout << "Exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(ResponsePrimTest, CtorNoFr) {
	string fr_none;

	try {
		ResponsePrim response_(p_request_, rsc_ok_, fr_none);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_TRUE(false);
}

TEST_F(ResponsePrimTest, JsonNormal) {
	try {
		ResponsePrim response_(p_request_, response_json);
	} catch (const exception &e) {
		cout << "exception:" << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(ResponsePrimTest, JsonInvalidRsc) {
	string invalid_json("{\"rsc\": 10, \"rqi\": \"ab3f124a\"}");

	try {
		ResponsePrim response_(p_request_, invalid_json);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_TRUE(false);
}

TEST_F(ResponsePrimTest, JsonNoRsc) {
	string invalid_json("{\"rqi\": \"ab3f124a\"}");

	try {
		ResponsePrim response_(p_request_, invalid_json);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_TRUE(false);
}

TEST_F(ResponsePrimTest, JsonNoRqi) {
	string norsi_json("{\"rsc\": 2000");

	try {
		ResponsePrim response_(p_request_, norsi_json);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_TRUE(false);
}

TEST_F(ResponsePrimTest, GetAttributes) {
	ASSERT_EQ(p_rsp_->getResponseStatusCode(), rsc_ok_);
	ASSERT_STREQ(p_rsp_->getRequestId().c_str(), rqi_.c_str());
}

TEST_F(ResponsePrimTest, SetGetContent) {
	p_rsp_->setContent(request_json);
	ASSERT_STREQ(p_rsp_->getContent().c_str(), request_json.c_str());
}

TEST_F(ResponsePrimTest, SetGetTo) {
	p_rsp_->setTo(to_);
	ASSERT_STREQ(p_rsp_->getTo().c_str(), to_.c_str());
}

TEST_F(ResponsePrimTest, SetGetFr) {
	p_rsp_->setFrom(fr_);
	ASSERT_STREQ(p_rsp_->getFrom().c_str(), fr_.c_str());
}
