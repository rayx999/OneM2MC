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

#include "CoAPBinding.pb.h"
#include "test_utils.h"
#include "CoAPInt_mock.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"
#include "ResourceBase.pb.h"
#include "CoAPBinding.pb.h"
#include "CommonTypes.h"
#include "CommonUtils.h"
#include "AE.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class AETest : public CoAPIntMockTest {
protected:
	static const std::string request_json;
	static const std::string ae_content, ae_exp;

	static ExpOption exp_opt_;
	static pb::ResourceBase ae_pc_;
	static string ae_str_;

	static string ri_, aei_;
	ResourceBase res_;

public:
	AETest() {}

    virtual void SetUp()
    {
    	 json2pb(ae_pc_, ae_exp.c_str(), ae_exp.length());

    	 exp_opt_ = {
    	 	{ pb::CoAPTypes_OptionType_CoAP_Uri_Host, "localhost" },
    	 	{ pb::CoAPTypes_OptionType_CoAP_Uri_Port, "5555" },
    	 	{ pb::CoAPTypes_OptionType_CoAP_Uri_Path, "//microwireless.com/AE-01" },
    	 	{ pb::CoAPTypes_OptionType_ONEM2M_FR,     "//microwireless.com/IN-CSE-01" },
    	 	{ pb::CoAPTypes_OptionType_ONEM2M_RQI,    "ab3f124a" }
    	 };
    }

    void setupCoAPBinding(const string& reqp_json, const string& pc_json) {
    	string res_pc_str;
        UTest::Resource2String(pc_json, res_pc_str);

        CoAPIntMockTest::setupCoAPBinding(reqp_json);
        p_coap_->set_payload(res_pc_str);
    }
};

ExpOption AETest::exp_opt_;
pb::ResourceBase AETest::ae_pc_;
string AETest::ae_str_;
string AETest::ri_;
string AETest::aei_;

const string AETest::request_json("{"
		"\"ver\": 1,"
		"\"type\": 0,"			// Confirmable
		"\"method\": 2,"		// Post
		"\"opt\": [ "
			"{ \"num\": 256, \"value\": \"//microwireless.com/AE-01\" },"			// FR
			"{ \"num\": 257, \"value\": \"ab3f124a\" } "							// RQI
		"]"
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
				"\"api\" 	: \"APP-01\" "
			"}"
		"}");

TEST_F(AETest, CreateAEFullURI) {
  string ret_pc_;
  setupCoAPBinding(request_json, ae_content);
  // add ONEM2M to_
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
		  "//microwireless.com/IN-CSE-01/AE-01");
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Created,
		  exp_opt_, ret_pc_);

  pb::ResourceBase ret_;
  ASSERT_TRUE(ret_.ParseFromString(ret_pc_));
  ri_ = ret_.ri();
  aei_ = "//microwireless.com/IN-CSE-01/" + ri_;
  ASSERT_FALSE(ri_.empty());
  ASSERT_STREQ(ret_.rn().c_str(), "AE-01"); // ret_ rn overwrites origianl in ae_content
  ASSERT_STREQ(ret_.ae().aei().c_str(), aei_.c_str()); // AEId set
  cout << "Responded ri: " << ri_ << endl;
}

TEST_F(AETest, RetrieveAE) {
  ASSERT_FALSE(last_test_bad_);

  CoAPIntMockTest::setupCoAPBinding(request_json);
  p_coap_->set_method(pb::CoAPTypes_MethodType_CoAP_GET);
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
		  "//microwireless.com/IN-CSE-01/" + ri_);
  ae_pc_.set_ri(ri_);
  ae_pc_.mutable_ae()->set_aei("//microwireless.com/IN-CSE-01/" + ri_);
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Content,
		  exp_opt_, ae_pc_);
}

TEST_F(AETest, RetrieveAE1) {
  ASSERT_FALSE(last_test_bad_);

  CoAPIntMockTest::setupCoAPBinding(request_json);
  p_coap_->set_method(pb::CoAPTypes_MethodType_CoAP_GET);
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
		  "//microwireless.com/IN-CSE-01/AE-01");
  ae_pc_.set_ri(ri_);
  ae_pc_.mutable_ae()->set_aei("//microwireless.com/IN-CSE-01/" + ri_);
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Content,
		  exp_opt_, ae_pc_);
}

TEST_F(AETest, CreateAEConflict) {
  setupCoAPBinding(request_json, ae_content);
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
			  "//microwireless.com/IN-CSE-01/AE-01");
  // add expected option ONEM2M_RSC Conflict
  exp_opt_[pb::CoAPTypes_OptionType_ONEM2M_RSC] = "4105";
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Forbidden,
		  exp_opt_);
}

TEST_F(AETest, CreateAEConflict1) {
  setupCoAPBinding(request_json, ae_content);
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
				  "//microwireless.com/IN-CSE-01");
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_ONEM2M_NM, "AE-01");
  // add expected option ONEM2M_RSC Conflict
  exp_opt_[pb::CoAPTypes_OptionType_ONEM2M_RSC] = "4105";
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Forbidden,
		  exp_opt_);
}

TEST_F(AETest, CreateAEConflict2) {
  setupCoAPBinding(request_json, ae_content);
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
		  "//in-cse-01.microwireless.com/ae-01");
  // add expected option ONEM2M_RSC Conflict
  exp_opt_[pb::CoAPTypes_OptionType_ONEM2M_RSC] = "4105";
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Forbidden,
		  exp_opt_);
}

TEST_F(AETest, CreateAENoContent) {
  CoAPIntMockTest::setupCoAPBinding(request_json);
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
		  "//microwireless.com/in-cse-01");
  // add expected option ONEM2M_RSC BAD_REQUEST
  exp_opt_[pb::CoAPTypes_OptionType_ONEM2M_RSC] = "4000";
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Bad_Request,
		  exp_opt_);
}

TEST_F(AETest, CreateAEInvalidContent) {
  setupCoAPBinding(request_json, ae_content);
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
		  "//microwireless.com/in-cse-01");
  p_coap_->set_payload("abcdefghijklmnopqrstuvwxyz1234567890");
  // add expected option ONEM2M_RSC BAD_REQUEST
  exp_opt_[pb::CoAPTypes_OptionType_ONEM2M_RSC] = "4000";
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Bad_Request,
		  exp_opt_);
 }

TEST_F(AETest, CreateAEParentInvalid) {
  setupCoAPBinding(request_json, ae_content);
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
			  "//microwireless.com/in-cse-01");
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_ONEM2M_NM,
			  "AE-01/Fun/Wrong");
  // add expected option ONEM2M_RSC NOT_FOUND
  exp_opt_[pb::CoAPTypes_OptionType_ONEM2M_RSC] = "4004";
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Not_Found,
		  exp_opt_);
}

TEST_F(AETest, CreateAEWithNPFields) {
   setupCoAPBinding(request_json, ae_exp);
   nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
 			  "//microwireless.com/in-cse-01");
   // add expected option ONEM2M_RSC BAD_REQUEST
   exp_opt_[pb::CoAPTypes_OptionType_ONEM2M_RSC] = "4000";
   retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
 		  pb::CoAPTypes_ResponseCode_CoAP_Bad_Request,
 		  exp_opt_);
}

TEST_F(AETest, CreateAENoMandaoryFields) {
   const string ae_json("{"
					"\"ae\"     : {"
						"\"apn\" 	: \"FreshGo\""
					"}"
				"}");
   setupCoAPBinding(request_json, ae_json);
   nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
 			  "//microwireless.com/in-cse-01");
   // add expected option ONEM2M_RSC BAD_REQUEST
   exp_opt_[pb::CoAPTypes_OptionType_ONEM2M_RSC] = "4000";
   retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
 		  pb::CoAPTypes_ResponseCode_CoAP_Bad_Request,
 		  exp_opt_);
}

TEST_F(AETest, CreateAENoRn) {
	  const string ae_json("{"
					"\"ae\"     : {"
						"\"apn\" 	: \"FreshGo\","
						"\"api\" 	: \"APP-01\" "
					"}"
				"}");
  string ret_pc_;
  setupCoAPBinding(request_json, ae_json);
  nse_->addOpt(*p_coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path,
			  "//microwireless.com/in-cse-01");
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Created,
		  exp_opt_, ret_pc_);

  pb::ResourceBase ret_;
  ASSERT_TRUE(ret_.ParseFromString(ret_pc_));
  ri_ = ret_.ri();
  aei_ = "//microwireless.com/IN-CSE-01/" + ri_;
  ASSERT_FALSE(ri_.empty());
  ASSERT_STREQ(ret_.rn().c_str(), ri_.c_str()); // Resource name same as ri
  ASSERT_STREQ(ret_.ae().aei().c_str(), aei_.c_str()); // AE-id set to ri
  cout << "Responded ri: " << ri_ << endl;
}

