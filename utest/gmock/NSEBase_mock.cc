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

// Matcher for keep test arg to val
MATCHER_P(StrGood, val, "") {
	if (arg.empty()) {
		return false;
	} else {
		*val = arg;
		return true;
	}
}

bool matcher_cse(const pb::CSEBase& act, const pb::CSEBase& exp) {
	return  act.cst() == exp.cst() &&
			act.csi() == exp.csi();
}

bool matcher_req(const pb::Request& act, const pb::Request& exp) {
	string act_str_;
	string exp_str_;
	if (act.SerializeToString(&act_str_) &&
		exp.SerializeToString(&exp_str_)) {
		return  act_str_ == exp_str_;
	}
	cerr << "macher_req: can't serialize to string\n";
	return false;
}

MATCHER_P(PbEq, exp_res, "") {
	pb::ResourceBase act_res_;
	if (!act_res_.ParseFromString(arg)) {
		return false;
	}

	if  (act_res_.ty() == exp_res.ty() &&
		 act_res_.ri() == exp_res.ri() &&
		 act_res_.rn() == exp_res.rn() )
	{
		if (act_res_.has_csb()) {
			return matcher_cse(act_res_.csb(), exp_res.csb());
		} if (act_res_.has_req()) {
			return matcher_req(act_res_.req(), exp_res.req());
		} else {
			return true;
		}
	}
	return false;
}


class NSEBaseMockTest : public ::testing::Test {
protected:
	static const std::string retrieve_json;
	static const std::string cse_content;
	static const string exp_to_, exp_fr_;

	static CSEResourceStore * rdb_;
	static NSEBaseMock * nse_;
	static CSEHandler * hdl_;
	static CSEServer * server_;
	static pb::ResourceBase exp_pc_;
	static bool last_test_bad_;
	static string req_ri_;

	const string* json_;

public:
	NSEBaseMockTest() : json_() {}

    //virtual void SetUp()
    static void SetUpTestCase()
    {
        rdb_ = new CSEResourceStore("data/.store");
        nse_ = new NiceMock<NSEBaseMock>("127.0.0.1", "1234");
        hdl_ = new CSEHandler(*nse_, *rdb_);
        server_ = new CSEServer(*rdb_, *nse_, *hdl_);

        json2pb(exp_pc_, cse_content.c_str(), cse_content.length());
    }

    //virtual void TearDown()
    static void TearDownTestCase()
    {
        delete rdb_;
        delete nse_;
        delete hdl_;
        delete server_;
    }

    virtual void TearDown() {
    	last_test_bad_ = HasFailure();
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

    void retrieveTestBody(ResponseStatusCode rsc, const string& rqi, const pb::ResourceBase& exp)
    {
		EXPECT_CALL(*nse_, run())
				.WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

		EXPECT_CALL(*nse_, send(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(rsc)),
								Property(&ResponsePrim::getTo, StrEq(exp_to_)),
								Property(&ResponsePrim::getFrom, StrEq(exp_fr_)),
								Property(&ResponsePrim::getRequestId, StrEq(rqi)),
								Property(&ResponsePrim::getContent, PbEq(exp)))))
			  .Times(1);

		server_->run();
    }

    void retrieveTestBody(ResponseStatusCode rsc, const string& rqi) {
    	 EXPECT_CALL(*nse_, run())
    		  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

 		EXPECT_CALL(*nse_, send(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(rsc)),
 								Property(&ResponsePrim::getTo, StrEq(exp_to_)),
 								Property(&ResponsePrim::getFrom, StrEq(exp_fr_)),
 								Property(&ResponsePrim::getRequestId, StrEq(rqi)))))
    		  .Times(1);

    	  server_->run();
    }

    void retrieveTestBody(ResponseStatusCode rsc, const string& rqi, string& pc) {
    	 EXPECT_CALL(*nse_, run())
    		  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

 		EXPECT_CALL(*nse_, send(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(rsc)),
 								Property(&ResponsePrim::getTo, StrEq(exp_to_)),
 								Property(&ResponsePrim::getFrom, StrEq(exp_fr_)),
 								Property(&ResponsePrim::getRequestId, StrEq(rqi)),
								Property(&ResponsePrim::getContent, StrGood(&pc)))))
    		  .Times(1);

    	  server_->run();
    }

    void printResponse(ResponsePrim rsp) {
    	cout << rsp.getJson() << endl;
    }
};

CSEResourceStore * NSEBaseMockTest::rdb_;
NSEBaseMock * NSEBaseMockTest::nse_;
CSEHandler * NSEBaseMockTest::hdl_;
CSEServer * NSEBaseMockTest::server_;
pb::ResourceBase NSEBaseMockTest::exp_pc_;
bool NSEBaseMockTest::last_test_bad_ = true;
string NSEBaseMockTest::req_ri_;

const string NSEBaseMockTest::retrieve_json("{"
		"\"op\": 2,"
		"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\","
		"\"rqi\": \"ab3f124a\","
		"\"fr\": \"//microwireless.com/AE-01\""
	"}");

const string NSEBaseMockTest::exp_to_("//microwireless.com/AE-01");
const string NSEBaseMockTest::exp_fr_("//microwireless.com/IN-CSE-01");

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
  retrieveTestBody(RSC_OK, "ab3f124a", exp_pc_);
}

TEST_F(NSEBaseMockTest, RetrieveCSE2) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01/IN-CSE-01\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");
  setJson(&json);
  retrieveTestBody(RSC_OK, "ab3f124a", exp_pc_);
}

TEST_F(NSEBaseMockTest, RetrieveCSE3) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"/IN-CSE-01/Z0005\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");
  setJson(&json);
  retrieveTestBody(RSC_OK, "ab3f124a", exp_pc_);
}

TEST_F(NSEBaseMockTest, RetrieveCSE4) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\", "
		    "\"rt\": 3"
		  "}");
  setJson(&json);
  retrieveTestBody(RSC_OK, "ab3f124a", exp_pc_);
}

TEST_F(NSEBaseMockTest, NotExistResource) {
    static const string not_exist_json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01/blah\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");

  setJson(&not_exist_json);
  retrieveTestBody(RSC_NOT_FOUND, "ab3f124a");
}

TEST_F(NSEBaseMockTest, RetrieveCSENonBlockSync) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"/IN-CSE-01/Z0005\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\", "
		    "\"rt\": 1"
		"}");
  string pc_str_;
  pb::ResourceBase pc_;

  setJson(&json);
  retrieveTestBody(RSC_ACCEPTED, "ab3f124a", pc_str_);
  ASSERT_TRUE(pc_.ParseFromString(pc_str_));
  req_ri_ = pc_.ri();
  cout << "Accepted Request RI = " << req_ri_ << endl;
}

TEST_F(NSEBaseMockTest, RetrieveNonBlockRequest) {
  // last test must be good
  ASSERT_FALSE(last_test_bad_);

  static const string json = string("{"
			"\"op\": 2,"
			"\"to\": \"/IN-CSE-01/") + req_ri_ +
   string("\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\" "
		"}");

  static const string exp_json("{"
		  "\"ty\": 17, "
		  "\"req\": {\"op\": 2, "
		            "\"tg\": \"/IN-CSE-01/Z0005\", "
		            "\"rs\": 3, "
		            "\"mi\": {\"rcn\": 1, "
		                     "\"rt\": 1}, "
		            "\"og\": \"//microwireless.com/AE-01\", "
		            "\"rid\": \"ab3f124a\"}, "
		  "\"ri\": \"" + req_ri_ + "\", "
		  "\"rn\": \"Request/ab3f124a\", "
		  "\"pi\": \"Z0005\" "
		  "}");

  pb::ResourceBase exp_req_;
  json2pb(exp_req_, exp_json.c_str(), exp_json.length());

  setJson(&json);
  retrieveTestBody(RSC_OK, "ab3f124a", exp_req_);
//  pb::ResourceBase res_;
//  ASSERT_TRUE(res_.ParseFromString(str_));
//  cout << pb2json(res_) << endl;
}

