/*
 * NSEBase_mock.cc
 *
 *  Created on: 2015年7月24日
 *      Author: weimi_000
 */


#include <RequestPrim.h>
#include <ResponsePrim.h>
#include <iostream>

//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "CSEBase.pb.h"
#include "CommonTypes.h"
#include "CommonUtils.h"
#include "CSEBase.h"
#include "ResourceStore.h"
#include "NSEBase_mock.h"
#include "CSEResourceStore.h"
#include "CSEHandler.h"
#include "CSEServer.h"

using namespace MicroWireless::OneM2M;

using ::testing::NiceMock;
using ::testing::AllOf;
using ::testing::StrEq;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Property;
using ::testing::Eq;
using ::testing::_;

// Matcher for pb::CSEBase
MATCHER_P(PbCseEq, exp_cse, "") {
	pb::CSEBase act_cse;
	if (!act_cse.ParseFromString(arg)) {
		return false;
	}

	return  act_cse.csi() == exp_cse.csi() &&
			act_cse.ty() == exp_cse.ty() &&
			act_cse.ri() == exp_cse.ri() &&
			act_cse.rn() == exp_cse.rn() &&
			act_cse.ct() == exp_cse.ct();
}

class NSEBaseMockTest : public ::testing::Test {
protected:
	static const std::string retrieve_json;
	static const std::string bad_request_json;
	static const std::string not_exist_json;
	static const std::string cse_content;

	string& json_;

	RequestPrim * req_;
	ResponsePrim * rsp_;
	CSEResourceStore * rdb_;
	NSEBaseMock * nse_;
	CSEHandler * hdl_;
	CSEServer * server_;

public:
	NSEBaseMockTest() : json_(const_cast<string&>(retrieve_json)) {}

    virtual void SetUp()
    {
        rdb_ = new CSEResourceStore("data/.store");
        nse_ = new NiceMock<NSEBaseMock>("127.0.0.1", "1234");
        hdl_ = new CSEHandler(*nse_, *rdb_);
        server_ = new CSEServer(*rdb_, *nse_, *hdl_);
        req_ = NULL;
    }

    virtual void TearDown()
    {
        delete rdb_;
        delete nse_;
        delete hdl_;
        delete server_;
        delete req_;
    }

    void setJson(string& json) {
    	json_ = json;
    }

    void handleRequest() {
    	req_ = new RequestPrim(json_);
    	hdl_->handleRequest(*req_);
    }

    void printResponse(ResponsePrim rsp) {
    	cout << rsp.getJson() << endl;
    }
};

const string NSEBaseMockTest::retrieve_json("{"
		"\"op\": 2,"
		"\"to\": \"//microwireless.com/IN-CSE-01/CSEBase\","
		"\"rqi\": \"ab3f124a\","
		"\"fr\": \"//microwireless.com/AE-01\""
	"}");

const string NSEBaseMockTest::cse_content("{"
				"\"ty\" 	: 1,"
				"\"ri\" 	: \"//microwireless.com/IN-CSE-01/CSEBASE\","
				"\"rn\" 	: \"CSEBASE\","
				"\"ct\" 	: { \"seconds\" : 1435434103 },"
				"\"cst\" 	: 1,"
				"\"csi\" 	: \"/IN-CSE-01\","
				"\"srt\" 	: [ 2, 5, 16 ]"
			"}");

TEST_F(NSEBaseMockTest, RetrieveCSE) {
  pb::CSEBase exp_pc_;
  json2pb(exp_pc_, cse_content.c_str(), cse_content.length());

  setJson(const_cast<string&>(retrieve_json));
  EXPECT_CALL(*nse_, run())
	  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

  EXPECT_CALL(*nse_, send(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(RSC_OK)),
		  	  	  	  	  	  	Property(&ResponsePrim::getRequestId, StrEq("ab3f124a")),
								Property(&ResponsePrim::getContent, PbCseEq(exp_pc_)))))
  	  .Times(1);

  server_->run();

}

const string NSEBaseMockTest::bad_request_json("{"
		"\"op\": 2,"
		"\"to\": \"//microwireless.com/IN-CSE-01/CSEBase\","
		"\"fr\": \"//microwireless.com/AE-01\""
	"}");

TEST_F(NSEBaseMockTest, BadRequest) {

  setJson(const_cast<string&>(bad_request_json));
  EXPECT_CALL(*nse_, run())
	  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

  EXPECT_CALL(*nse_, send(Property(&ResponsePrim::getResponseStatusCode, Eq(RSC_BAD_REQUEST))))
	  .Times(1);

  server_->run();

}

const string NSEBaseMockTest::not_exist_json("{"
		"\"op\": 2,"
		"\"to\": \"//microwireless.com/IN-CSE-01\","
		"\"rqi\": \"ab3f124a\","
		"\"fr\": \"//microwireless.com/AE-01\""
	"}");

TEST_F(NSEBaseMockTest, NotExistResource) {

  setJson(const_cast<string&>(not_exist_json));
  EXPECT_CALL(*nse_, run())
	  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

  EXPECT_CALL(*nse_, send(Property(&ResponsePrim::getResponseStatusCode, Eq(RSC_NOT_FOUND))))
	  .Times(1);

  server_->run();

}

