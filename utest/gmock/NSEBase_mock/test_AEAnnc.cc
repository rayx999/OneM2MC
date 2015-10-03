/*
 * test_AEAnnc.cc
 *
 *  Created on: 2015年9月29日
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
#include "AEAnnc.h"

using namespace std;
using namespace MicroWireless::OneM2M;

using ::testing::NiceMock;
using ::testing::AllOf;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Property;
using ::testing::Eq;
using ::testing::_;

class AEAnncTest : public NSEBaseMockTest {
public:
	static const std::string create_request;
	static const std::string ae_content, ae_exp, aeA_exp;
	static const string exp_to_, exp_fr_, exp_an_, an_rsp;

	ResourceBase res_;

public:
	AEAnncTest() {}

    virtual void SetUp()
    {
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

const string AEAnncTest::create_request("{"
		"\"op\": 1, "
		"\"to\": \"//microwireless.com/IN-CSE-01\", "
		"\"rqi\": \"ab3f124a\", "
		"\"fr\": \"//microwireless.com/AE-01\" "
	"}");

const string AEAnncTest::ae_content("{"
		"\"et\"     : { \"seconds\" : 1435434103 },"
		"\"lbl\"    : [ \"test\" ],"
		"\"at\"		: \"//microwireless.com/IN-CSE-02\","
		"\"ae\"     : {"
				"\"apn\" 	: \"FreshGoAnnc\","
				"\"api\" 	: \"APP-02\" "
			"}"
		"}");

const string AEAnncTest::aeA_exp("{"
		    "\"rn\"     : \"FreshGoAnnc\","
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"acpi\"	: [ \"001-25423\" ],"
			"\"lbl\"    : [ \"test\" ],"
			"\"aeA\"    : {"
				"\"apn\" 	: \"FreshGoAnnc\","
				"\"api\" 	: \"APP-02\" "
			"}"
		"}");

const string AEAnncTest::ae_exp("{"
		"\"rn\"     : \"FreshGoAnnc\","
		"\"pi\"     : \"Z0005\","
		"\"acpi\"	: [ \"001-25423\" ],"
		"\"ae\"     : {"
			"}"
		"}");

const string AEAnncTest::exp_to_("//microwireless.com/AE-01");
const string AEAnncTest::exp_fr_("//microwireless.com/IN-CSE-01");
const string AEAnncTest::exp_an_("//microwireless.com/IN-CSE-02");

ACTION_P4(handleResponse, hdl, rsc, fr, pc) {
	RequestPrim& reqp_ = arg0;
	ResponsePrim rspp(&reqp_, rsc, fr);
	rspp.setContent(pc);
	hdl->handleResponse(rspp);
}

ACTION_P2(setExpAE, exp_ae, ri) {
	exp_ae->set_ri(*ri);
	exp_ae->mutable_ae()->set_aei("//microwireless.com/IN-CSE-01/" + *ri);
}

MATCHER_P2(AnncEq, exp_res, ri, "") {
	pb::ResourceBase act_res_;
	pb::ResourceBase exp_res_(exp_res);
	if (!act_res_.ParseFromString(arg)) {
		return false;
	}

	*ri = act_res_.ri();

	if  (act_res_.et() == exp_res_.et() &&
		 act_res_.rn() == exp_res_.rn() &&
		 act_res_.acpi(0) == exp_res_.acpi(0) )
	{
		if (act_res_.has_aea()) {
			string aei_ = "//microwireless.com/IN-CSE-01/" + act_res_.ri();
			exp_res_.mutable_aea()->set_aei(aei_);
			return matcher_ae(act_res_.aea(), exp_res_.aea());
		} else {
			cerr << "AnncEq: No sub-resource.\n";
			return false;
		}
		cout << "AnncEq succeeds.\n";
	}
	return false;
}

TEST_F(AEAnncTest, CreateAEAnnc) {
  string rqi_, ri_, ret_pc_;
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setTo("//microwireless.com/in-cse-01/FreshGoAnnc");

  // expected annc resource
  pb::ResourceBase exp_aea_, exp_ae_;
  string aeA_pc_;
  json2pb(exp_aea_, aeA_exp.c_str(), aeA_exp.length());
  exp_aea_.SerializeToString(&aeA_pc_);
  json2pb(exp_ae_, ae_exp.c_str(), ae_exp.length());

  // send create AE with at attribute
  EXPECT_CALL(*nse_, run())
		.WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

  // expect create annc request back
  EXPECT_CALL(*nse_, send_request(AllOf(Property(&RequestPrim::getOperation, Eq(Operation::CREATE)),
				Property(&RequestPrim::getTo, StrEq(exp_an_)),
				Property(&RequestPrim::getFrom, StrEq(exp_fr_)),
				Property(&RequestPrim::getRequestId, StrSave(&rqi_)),
				Property(&RequestPrim::getContent, AnncEq(exp_aea_, &ri_))), _, _))
		.WillOnce(DoAll(setExpAE(&exp_ae_, &ri_),
				handleResponse(hdl_, ResponseStatusCode::CREATED, exp_an_, aeA_pc_)));

  // expect response to original request
  EXPECT_CALL(*nse_, send_response(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(ResponseStatusCode::CREATED)),
				Property(&ResponsePrim::getTo, StrEq(exp_to_)),
				Property(&ResponsePrim::getFrom, StrEq(exp_fr_)),
				Property(&ResponsePrim::getRequestId, StrEq("ab3f124a")),
				Property(&ResponsePrim::getContent, PbEq(&exp_ae_))), _, _))
		.Times(1);

  server_->run();
}

/*
TEST_F(AEAnncTest, RetrieveAE) {
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

TEST_F(AEAnncTest, RetrieveAE1) {
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

TEST_F(AEAnncTest, CreateAEConflict) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setTo("//microwireless.com/in-cse-01/AE-01");
  retrieveTestBody(ResponseStatusCode::CONFLICT, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AEAnncTest, CreateAEConflict1) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setName("AE-01");

  retrieveTestBody(ResponseStatusCode::CONFLICT, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AEAnncTest, CreateAEConflict2) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setTo("//in-cse-01.microwireless.com/AE-01");

  retrieveTestBody(ResponseStatusCode::CONFLICT, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AEAnncTest, CreateAENoContent) {
  NSEBaseMockTest::setupRequestPrim(create_request);
  retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AEAnncTest, CreateAEInvalidContent) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setContent("abcdefghijklmnopqrstuvwxyz1234567890");
  retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AEAnncTest, CreateAEParentInvalid) {
  setupRequestPrim(create_request, ae_content);
  p_reqp_->setName("AE-01/Fun/Wrong");

  retrieveTestBody(ResponseStatusCode::NOT_FOUND, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AEAnncTest, CreateAEWithNPFields) {
  setupRequestPrim(create_request, ae_exp);
   retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AEAnncTest, CreateAENoMandaoryFields) {
	  const string ae_json("{"
					"\"ae\"     : {"
						"\"apn\" 	: \"FreshGo\""
					"}"
				"}");
   setupRequestPrim(create_request, ae_json);
   retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(AEAnncTest, CreateAENoRn) {
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
*/
