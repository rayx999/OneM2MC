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

#include "ResourceBase.pb.h"
#include "CommonTypes.h"
#include "CommonUtils.h"
#include "CSEBase.h"
#include "ResourceStore.h"
#include "NSEBase_mock.h"
#include "CSEResourceStore.h"
#include "CSEHandler.h"
#include "CSEServer.h"

using namespace std;
using namespace MicroWireless::OneM2M;

using ::testing::NiceMock;
using ::testing::AllOf;
using ::testing::StrEq;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Property;
using ::testing::Eq;
using ::testing::_;

// Matcher for pb::ResourceBase
MATCHER_P(PbCseEq, exp_cse, "") {
	pb::ResourceBase act_cse;
	if (!act_cse.ParseFromString(arg)) {
		return false;
	}

	return  act_cse.ty() == exp_cse.ty() &&
			act_cse.ri() == exp_cse.ri() &&
			act_cse.rn() == exp_cse.rn() &&
			act_cse.ct() == exp_cse.ct() &&
			act_cse.has_csb() &&
			act_cse.csb().cst() == exp_cse.csb().cst() &&
			act_cse.csb().csi() == exp_cse.csb().csi();
}

class NSEBaseMockTest : public ::testing::Test {
protected:
	static const std::string retrieve_json;
	static const std::string cse_content;

	const string* json_;

	ResponsePrim * rsp_;
	CSEResourceStore * rdb_;
	NSEBaseMock * nse_;
	CSEHandler * hdl_;
	CSEServer * server_;
	pb::ResourceBase exp_pc_;

public:
	NSEBaseMockTest() : json_() {}

    virtual void SetUp()
    {
        rdb_ = new CSEResourceStore("data/.store");
        nse_ = new NiceMock<NSEBaseMock>("127.0.0.1", "1234");
        hdl_ = new CSEHandler(*nse_, *rdb_);
        server_ = new CSEServer(*rdb_, *nse_, *hdl_);

        json2pb(exp_pc_, cse_content.c_str(), cse_content.length());
    }

    virtual void TearDown()
    {
        delete rdb_;
        delete nse_;
        delete hdl_;
        delete server_;
    }

    void setJson(const string* json) {
    	json_ = json;
    }

    void handleRequest() {
    	try {
    		RequestPrim req_(*json_);
         	hdl_->handleRequest(req_);
    	} catch (exception &e) {
    		cerr << "Unexpected exception: " << e.what() << endl;
    		ASSERT_TRUE(false);
    	}
    }

    void retrieveTestBody() {
		EXPECT_CALL(*nse_, run())
				.WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

		EXPECT_CALL(*nse_, send(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(RSC_OK)),
								Property(&ResponsePrim::getRequestId, StrEq("ab3f124a")),
								Property(&ResponsePrim::getContent, PbCseEq(exp_pc_)))))
			  .Times(1);

		server_->run();
    }

    void printResponse(ResponsePrim rsp) {
    	cout << rsp.getJson() << endl;
    }
};

const string NSEBaseMockTest::retrieve_json("{"
		"\"op\": 2,"
		"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\","
		"\"rqi\": \"ab3f124a\","
		"\"fr\": \"//microwireless.com/AE-01\""
	"}");

const string NSEBaseMockTest::cse_content("{"
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

TEST_F(NSEBaseMockTest, RetrieveCSE) {
  setJson(&retrieve_json);
  retrieveTestBody();
}

TEST_F(NSEBaseMockTest, RetrieveCSE2) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01/IN-CSE-01\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");
  setJson(&json);
  retrieveTestBody();
}

TEST_F(NSEBaseMockTest, RetrieveCSE3) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"/IN-CSE-01/Z0005\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");
  setJson(&json);
  retrieveTestBody();
}

TEST_F(NSEBaseMockTest, NotExistResource) {
    static const string not_exist_json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01/blah\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");

  setJson(&not_exist_json);
  EXPECT_CALL(*nse_, run())
	  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

  EXPECT_CALL(*nse_, send(Property(&ResponsePrim::getResponseStatusCode, Eq(RSC_NOT_FOUND))))
	  .Times(1);

  server_->run();
}

