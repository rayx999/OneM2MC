/*
 * test_RequestHandler.cc
 *
 *  Created on: 2015年7月27日
 *      Author: weimi_000
 */


#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "test_utils.h"
#include "Request.h"
#include "CSEBase.h"
#include "NSEBase.h"
#include "RequestHandler.h"

using namespace MicroWireless::OneM2M;

class RequestHandlerTest : public ::testing::Test {

protected:
	static const string cse_json;
	static const string req_json, req1_json;

//	CSEBase *p_cse_;
	NSEBase *p_nse_;
	RequestHandler *p_hdl_;

	virtual void SetUp() {
//		p_cse_ = new CSEBase(cse_json);
		p_nse_ = new NSEBase("127.0.0.1", "1234");
		p_hdl_ = new RequestHandler(*p_nse_);
	}

	virtual void TearDown() {
//		delete p_cse_;
		delete p_nse_;
		delete p_hdl_;
	}
};

const string RequestHandlerTest::cse_json("{"
		"\"ty\" 	: 1,"
		"\"ri\" 	: \"//microwireless.com/IN-CSE-01/CSEBASE\","
		"\"rn\" 	: \"CSEBASE\","
		"\"ct\" 	: { \"seconds\" : 1435434103 },"
		"\"cst\" 	: 1,"
		"\"csi\" 	: \"/IN-CSE-01\","
		"\"srt\" 	: [ 2, 5, 16 ]"
	"}");
const string RequestHandlerTest::req_json("{"
		"\"op\"		: 2,"
		"\"to\"		: \"//microwireless.com/IN-CSE-01/CSEBase\","
		"\"rqi\"	: \"ab3f124a\","
		"\"fr\"		: \"//microwireless.com/AE-01\""
	"}");

const string RequestHandlerTest::req1_json("{"
		"\"op\"		: 2,"
		"\"to\"		: \"//blarblarblar.com/IN-CSE-01/CSEBase\","
		"\"rqi\"	: \"ab3f124a\","
		"\"fr\"		: \"//microwireless.com/AE-01\""
	"}");

TEST_F(RequestHandlerTest, isForMe) {
	CSEBase cse_(cse_json);
	Request req_(req_json);
	ASSERT_EQ(p_hdl_->isForMe<CSEBase>(req_, cse_), RSC_OK);
}

TEST_F(RequestHandlerTest, isNOTForMe) {
	CSEBase cse_(cse_json);
	Request req_(req1_json);
	ASSERT_EQ(p_hdl_->isForMe<CSEBase>(req_, cse_), RSC_ACCESS_DENIED);
}
