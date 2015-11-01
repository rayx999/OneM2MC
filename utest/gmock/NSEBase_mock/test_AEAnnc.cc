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
	static const std::string ae_content, aeA_content, ae_exp, aeA_exp;
	static const string exp_to_, exp_fr_, exp_an_, an_rsp;

	static string ri_, aei_, aeA_str_;

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

const string AEAnncTest::aeA_content("{"
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"lbl\"    : [ \"test\" ],"
			"\"aa\"		: \"apn api nl\","
			"\"aeA\"    : {"
				"\"apn\" 	: \"FreshGoAnnc1\","
				"\"api\" 	: \"APP-02\","
			    "\"nl\" 	: \"//microwireless.com/in-cse-02/10002-94770\" "
			"}"
		"}");

const string AEAnncTest::aeA_exp("{"
		    "\"rn\"     : \"FreshGoAnnc\","
			"\"et\"     : { \"seconds\" : 1435434103 },"
			"\"acpi\"	: [ \"00001-25423\" ],"
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

string AEAnncTest::ri_;
string AEAnncTest::aei_;
string AEAnncTest::aeA_str_;

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

TEST_F(AEAnncTest, CreateAEWithAt) {
  string rqi_, ret_pc_;
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

  CSEServer::run();
}

TEST_F(AEAnncTest, CreateAEAnncAsCSE) {
  setupRequestPrim(create_request, aeA_content);
  p_reqp_->setTo("//microwireless.com/in-cse-01/FreshGoAnnc1");
  p_reqp_->setFrom(exp_an_);

  retrieveTestBody(ResponseStatusCode::CREATED, "ab3f124a", exp_an_, exp_fr_, aeA_str_);

  pb::ResourceBase aeA_pc_;
  aeA_pc_.ParseFromString(aeA_str_);
  ASSERT_STREQ(aeA_pc_.rn().c_str(), "FreshGoAnnc1");
  ASSERT_STREQ(aeA_pc_.pi().c_str(), "Z0005");
  ri_ = aeA_pc_.ri();
  aei_ = "//microwireless.com/IN-CSE-01/" + ri_;
  ASSERT_STREQ(aeA_pc_.aea().aei().c_str(), aei_.c_str());
}

TEST_F(AEAnncTest, RetrieveAEAnncAsCSE) {
  ASSERT_FALSE(last_test_bad_);

  static const string json("{"
 		"\"op\": 2,"
 		"\"to\": \"//microwireless.com/IN-CSE-01\","
 		"\"rqi\": \"ab3f124a\","
 		"\"fr\": \"//microwireless.com/IN-CSE-02\""
 	"}");
  NSEBaseMockTest::setupRequestPrim(json);
  ASSERT_TRUE(p_reqp_->setTo(p_reqp_->getTo() + "/" + ri_));

  retrieveTestBody(ResponseStatusCode::OK, "ab3f124a", exp_an_, exp_fr_, aeA_str_);

  pb::ResourceBase aeA_pc_;
  aeA_pc_.ParseFromString(aeA_str_);
  ASSERT_FALSE(aeA_pc_.ri().empty());
  ASSERT_STREQ(aeA_pc_.rn().c_str(), "FreshGoAnnc1");
  ASSERT_STREQ(aeA_pc_.acpi(0).c_str(), "00001-25423");
  ASSERT_STREQ(aeA_pc_.lbl(0).c_str(), "test");
  ASSERT_TRUE(aeA_pc_.has_aea());
  ASSERT_STREQ(aeA_pc_.aea().apn().c_str(), "FreshGoAnnc1");
  ASSERT_STREQ(aeA_pc_.aea().api().c_str(), "APP-02");
  ASSERT_TRUE(aeA_pc_.aea().aei().empty());
  ASSERT_STREQ(aeA_pc_.aea().nl().c_str(), "//microwireless.com/in-cse-02/10002-94770");
}
