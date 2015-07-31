/*
 * test_Response.cc
 *
 *  Created on: 2015年7月18日
 *      Author: weimi_000
 */
#include <iostream>
#include <fstream>
#include <sstream>

#include "gtest/gtest.h"
#include "test_utils.h"
#include "Response.h"

using namespace MicroWireless::OneM2M;

class ResponseTest : public ::testing::Test {
	protected:
	static const string request_json;
	static const string response_json;
	static const string rqi_;
	static const string to_;
	static const string fr_;
	string pc_;
	ResponseStatusCode rsc_ok_;
	Response * p_response_;
	Request * p_request_;

	ResponseTest() {
		rsc_ok_ = RSC_OK;
		pc_ = request_json;
		p_response_ = NULL;
		p_request_ = NULL;
	}

	virtual void SetUp() {
		p_request_ = new Request(request_json);
		p_response_ = new Response(new Request(request_json), response_json);
	}

	ResponseStatusCode getResponseStatusCode() {
		return static_cast<const ResponseTest*>(this)->p_response_->getResponseStatusCode();
		//return c->p_response_->getResponseStatusCode();
	}

	const string & getRequestId() {
		return p_response_->getRequestId();
	}

	bool setContent(const string & pc) {
		return p_response_->setContent(pc);
	}

	const string & getContent() {
		return p_response_->getContent();
	}

	bool setTo(const string & to) {
		return p_response_->setTo(to);
	}

	const string & getTo() {
		return p_response_->getTo();
	}

	bool setFrom(const string & fr) {
		return p_response_->setFrom(fr);
	}

	const string & getFrom() {
		return p_response_->getFrom();
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

const string ResponseTest::request_json("{\"op\": 2, \"to\": \"//microwireless.com/IN-CSE-01\", \"rqi\": \"ab3f124a\", \"fr\": \"//microwireless.com/AE-01\"}");
const string ResponseTest::response_json("{\"rsc\": 2000, \"rqi\": \"ab3f124a\"}");
const string ResponseTest::rqi_("ab3f124a");
const string ResponseTest::to_("//microwireless.com/IN-CSE-01");
const string ResponseTest::fr_("//microwireless.com/AE-01");

TEST_F(ResponseTest, FullCtor) {
	try {
		Response response_(p_request_, rsc_ok_, rqi_);
	} catch (const exception &e) {
		cout << "Exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(ResponseTest, CtorNoRqi) {
	string rqi_none;

	try {
		Response response_(p_request_, rsc_ok_, rqi_none);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_TRUE(false);
}

TEST_F(ResponseTest, JsonNormal) {
	try {
		Response response_(p_request_, response_json);
	} catch (const exception &e) {
		cout << "exception:" << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

TEST_F(ResponseTest, JsonInvalidRsc) {
	string invalid_json("{\"rsc\": 10, \"rqi\": \"ab3f124a\"}");

	try {
		Response response_(p_request_, invalid_json);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_TRUE(false);
}

TEST_F(ResponseTest, JsonNoRsc) {
	string invalid_json("{\"rqi\": \"ab3f124a\"}");

	try {
		Response response_(p_request_, invalid_json);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_TRUE(false);
}

TEST_F(ResponseTest, JsonNoRqi) {
	string norsi_json("{\"rsc\": 2000");

	try {
		Response response_(p_request_, norsi_json);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_TRUE(false);
}
/*
TEST_F(ResponseTest, RetrieveWithResourceType) {
	string json("{\"op\": 2, \"to\": \"//microwireless.com/IN-CSE-01\", \"rqi\": \"ab3f124a\", \"fr\": \"//microwireless.com/AE-01\", \"ty\": 1}");
	Response * pReq_ = NULL;
	try {
		pReq_ = new Response(json);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_FALSE(pReq_->isValid());
}

TEST_F(ResponseTest, RetrieveWithName) {
	string json("{\"op\": 2, \"to\": \"//microwireless.com/IN-CSE-01\", \"rqi\": \"ab3f124a\", \"fr\": \"//microwireless.com/AE-01\", \"nm\": \"Name\" }");
	Response * pReq_ = NULL;
	try {
		pReq_ = new Response(json);
	} catch (const exception &e) {
		cout << "Expected exception:" << e.what() << endl;
		return;
	}
	ASSERT_FALSE(pReq_->isValid());
}
*/

TEST_F(ResponseTest, GetAttributes) {
	ASSERT_EQ(getResponseStatusCode(), rsc_ok_);
	ASSERT_STREQ(getRequestId().c_str(), rqi_.c_str());
}

TEST_F(ResponseTest, SetGetContent) {
	ASSERT_TRUE(setContent(request_json));
	ASSERT_STREQ(getContent().c_str(), request_json.c_str());
}

TEST_F(ResponseTest, SetGetTo) {
	ASSERT_TRUE(setTo(to_));
	ASSERT_STREQ(getTo().c_str(), to_.c_str());
}

TEST_F(ResponseTest, SetGetFr) {
	ASSERT_TRUE(setFrom(fr_));
	ASSERT_STREQ(getFrom().c_str(), fr_.c_str());
}
