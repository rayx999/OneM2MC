/*
 * test_AE.cc
 *
 *  Created on: 2015年8月21日
 *      Author: weimi_000
 */


#include <iostream>
#include <string>
#include <json2pb.h>

//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "test_utils.h"
#include "NSEBase_mock.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"
#include "ResourceBase.pb.h"
#include "CommonTypes.h"
#include "CommonUtils.h"
#include "AE.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class AETest : public NSEBaseMockTest {
protected:
	static const std::string create_request;
	static const std::string ae_content, ae_exp;
	static const string exp_to_, exp_fr_;

	static pb::ResourceBase ae_pc_;
	static string ae_str_;

	static string ri_;
	ResourceBase res_;

public:
	AETest() {}

    virtual void SetUp()
    {
    	 json2pb(ae_pc_, ae_exp.c_str(), ae_exp.length());
    }

    void setupRequestPrim(const string& reqp_json, const string& pc_json) {
    	string res_pc_str;
        UTest::Resource2String(pc_json, res_pc_str);

        try {
        	p_reqp_ = new RequestPrim(reqp_json);
        	ASSERT_TRUE(p_reqp_ != NULL);
        	p_reqp_->setContent(res_pc_str);
        } catch (exception &e) {
        	cerr << "setupRequestPrim: exception:" << e.what() << endl;
        	ASSERT_TRUE(false);
        }
    }

};

pb::ResourceBase AETest::ae_pc_;
string AETest::ae_str_;
string AETest::ri_;

const string AETest::create_request("{"
		"\"op\": 1, "
		"\"to\": \"//microwireless.com/IN-CSE-01\", "
		"\"rqi\": \"ab3f124a\", "
		"\"fr\": \"//microwireless.com/AE-01\" "
	"}");

const string AETest::ae_content("{"
			"\"ae\"     : {"
				"\"apn\" 	: \"FreshGo\","
				"\"api\" 	: \"APP-01\" "
			"}"
		"}");

const string AETest::ae_exp("{"
			"\"ty\" 	: 2,"
		    "\"rn\"     : \"AE-01\","
	    	"\"pi\"     : \"Z0005\","
			"\"ae\"     : {"
				"\"apn\" 	: \"FreshGo\","
				"\"api\" 	: \"APP-01\","
				"\"aei\" 	: \"AE-01\" "
			"}"
		"}");

const string AETest::exp_to_("//microwireless.com/AE-01");
const string AETest::exp_fr_("//microwireless.com/IN-CSE-01");

TEST_F(AETest, CreateAEFullURI) {
  string ret_pc_;
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setTo("//microwireless.com/in-cse-01/AE-01");
  retrieveTestBody(ResponseStatusCode::CREATED, "ab3f124a", exp_to_, exp_fr_, ret_pc_);
  pb::ResourceBase ret_;
  ASSERT_TRUE(ret_.ParseFromString(ret_pc_));
  ri_ = ret_.ri();
  ASSERT_FALSE(ri_.empty());
  ASSERT_STREQ(ret_.rn().c_str(), "AE-01"); // ret_ rn overwrites origianl in ae_content
  ASSERT_STREQ(ret_.ae().aei().c_str(), "AE-01"); // AEId set
  cout << "Responded ri: " << ri_ << endl;
}

TEST_F(AETest, RetrieveAE) {
  ASSERT_FALSE(last_test_bad_);

  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");
  NSEBaseMockTest::setupRequestPrim(json);
  ASSERT_TRUE(p_reqp_->setTo(p_reqp_->getTo() + "/" + ri_));

  ae_pc_.set_ri(ri_);
  retrieveTestBody(ResponseStatusCode::OK, "ab3f124a", exp_to_, exp_fr_, ae_pc_);
}

TEST_F(AETest, RetrieveAE1) {
  ASSERT_FALSE(last_test_bad_);

  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01/AE-01\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");
  NSEBaseMockTest::setupRequestPrim(json);

  ae_pc_.set_ri(ri_);
  retrieveTestBody(ResponseStatusCode::OK, "ab3f124a", exp_to_, exp_fr_, ae_pc_);
}

TEST_F(AETest, CreateAEConflict) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setTo("//microwireless.com/in-cse-01/AE-01");
  retrieveTestBody(ResponseStatusCode::CONFLICT, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AETest, CreateAEConflict1) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setName("AE-01");

  retrieveTestBody(ResponseStatusCode::CONFLICT, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AETest, CreateAEConflict2) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setTo("//in-cse-01.microwireless.com/AE-01");

  retrieveTestBody(ResponseStatusCode::CONFLICT, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AETest, CreateAENoContent) {
  NSEBaseMockTest::setupRequestPrim(create_request);
  retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AETest, CreateAEInvalidContent) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setContent("abcdefghijklmnopqrstuvwxyz1234567890");
  retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AETest, CreateAEParentInvalid) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setName("AE-01/Fun/Wrong");

  retrieveTestBody(ResponseStatusCode::NOT_FOUND, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AETest, CreateAEWithNPFields) {
  setupRequestPrim(create_request, ae_exp);
   retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AETest, CreateAENoMandaoryFields) {
	  const string ae_json("{"
					"\"ae\"     : {"
						"\"apn\" 	: \"FreshGo\""
					"}"
				"}");
   setupRequestPrim(create_request, ae_json);
   retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AETest, CreateAENoRn) {
	  const string ae_json("{"
					"\"ae\"     : {"
						"\"apn\" 	: \"FreshGo\","
						"\"api\" 	: \"APP-01\" "
					"}"
				"}");
  string ret_pc_;
  setupRequestPrim(create_request, ae_json);
  retrieveTestBody(ResponseStatusCode::CREATED, "ab3f124a", exp_to_, exp_fr_, ret_pc_);
  pb::ResourceBase ret_;
  ASSERT_TRUE(ret_.ParseFromString(ret_pc_));
  ri_ = ret_.ri();
  ASSERT_FALSE(ri_.empty());
  ASSERT_STREQ(ret_.rn().c_str(), ri_.c_str()); // Resource name same as ri
  ASSERT_STREQ(ret_.ae().aei().c_str(), ri_.c_str()); // AE-id set to ri
  cout << "Responded ri: " << ri_ << endl;
}
