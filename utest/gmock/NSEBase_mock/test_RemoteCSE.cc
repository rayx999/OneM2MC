/*
 * test_RemoteCSE.cc
 *
 *  Created on: 2015年10月23日
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
#include "RemoteCSE.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class RemoteCSETest : public NSEBaseMockTest {
protected:
	static const std::string create_request;
	static const std::string retrieve_request;
	static const std::string update_request;
	static const std::string csr_content, csr_exp;
	static const string exp_to_, exp_fr_;

	static pb::ResourceBase csr_pc_;
	static string csr_str_;

	static string ri_;
	ResourceBase res_;

public:
	RemoteCSETest() {}

    virtual void SetUp()
    {
    	 json2pb(csr_pc_, csr_exp.c_str(), csr_exp.length());
    }

    void setupRequestPrim(const string& reqp_json, const string& pc_json) {
    	pb::ResourceBase res;
    	json2pb(res, pc_json.c_str(), pc_json.length());
    	if (!res.csr().rr()) res.mutable_csr()->set_rr(false);
      	string res_pc_str;
        res.SerializeToString(&res_pc_str);

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

pb::ResourceBase RemoteCSETest::csr_pc_;
string RemoteCSETest::csr_str_;
string RemoteCSETest::ri_;

const string RemoteCSETest::create_request("{"
		"\"op\": 1, "
		"\"to\": \"//microwireless.com/IN-CSE-01\", "
		"\"rqi\": \"ab3f124a\", "
		"\"fr\": \"//microwireless.com/AN-CSE-01\" "
	"}");

const string RemoteCSETest::retrieve_request("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AN-CSE-01\""
		"}");

const string RemoteCSETest::update_request("{"
		"\"op\": 3, "
		"\"to\": \"//microwireless.com/IN-CSE-01\", "
		"\"rqi\": \"ab3f124a\", "
		"\"fr\": \"//microwireless.com/AN-CSE-01\" "
	"}");

const string RemoteCSETest::csr_content("{"
			"\"csr\"     : {"
				"\"cst\" 	: 3,"
				"\"csi\" 	: \"Z00016\","
				"\"rr\"		: false, "
				"\"cb\" 	: \"//microwireless.com/AN-CSE-01\" "
			"}"
		"}");

const string RemoteCSETest::csr_exp("{"
			"\"ty\" 	: 16,"
			"\"pi\"     : \"Z0005\","
			"\"rn\"		: \"AN-CSE-01\","
			"\"csr\"    : {"
				"\"cst\" 	: 3,"
				"\"csi\" 	: \"Z00016\","
				"\"rr\"		: false, "
				"\"cb\" 	: \"//microwireless.com/AN-CSE-01\" "
			"}"
		"}");

const string RemoteCSETest::exp_to_("//microwireless.com/AN-CSE-01");
const string RemoteCSETest::exp_fr_("//microwireless.com/IN-CSE-01");

TEST_F(RemoteCSETest, CreateFullURI) {
  string ret_pc_;
  setupRequestPrim(create_request, csr_content);
  p_reqp_->setTo("//microwireless.com/in-cse-01/AN-CSE-01");
  retrieveTestBody(ResponseStatusCode::CREATED, "ab3f124a", exp_to_, exp_fr_, ret_pc_);
  pb::ResourceBase ret_;
  ASSERT_TRUE(ret_.ParseFromString(ret_pc_));
  ri_ = ret_.ri();
  ASSERT_FALSE(ri_.empty());
  ASSERT_STREQ(ret_.rn().c_str(), "AN-CSE-01");
  cout << "Responded ri: " << ri_ << endl;
}

TEST_F(RemoteCSETest, Retrieve) {
  ASSERT_FALSE(last_test_bad_);

  NSEBaseMockTest::setupRequestPrim(retrieve_request);
  ASSERT_TRUE(p_reqp_->setTo(p_reqp_->getTo() + "/" + ri_));

  csr_pc_.set_ri(ri_);
  retrieveTestBody(ResponseStatusCode::OK, "ab3f124a", exp_to_, exp_fr_, csr_pc_);
}

TEST_F(RemoteCSETest, Retrieve1) {
  ASSERT_FALSE(last_test_bad_);

  NSEBaseMockTest::setupRequestPrim(retrieve_request);
  ASSERT_TRUE(p_reqp_->setTo("//microwireless.com/IN-CSE-01/AN-CSE-01"));

  csr_pc_.set_ri(ri_);
  retrieveTestBody(ResponseStatusCode::OK, "ab3f124a", exp_to_, exp_fr_, csr_pc_);
}

TEST_F(RemoteCSETest, UpdateLbl) {
  ASSERT_FALSE(last_test_bad_);

  const string csr_upd("{"
			"\"lbl\"     : [ \"new-lbl\" ], "
		    "\"csr\"	 : { } "
		"}");

  string ret_pc_;
  setupRequestPrim(update_request, csr_upd);
  ASSERT_TRUE(p_reqp_->setTo("//microwireless.com/in-cse-01/AN-CSE-01"));

  retrieveTestBody(ResponseStatusCode::CHANGED, "ab3f124a", exp_to_, exp_fr_, ret_pc_);
  pb::ResourceBase ret_;
  ASSERT_TRUE(ret_.ParseFromString(ret_pc_));
  ASSERT_EQ(ret_.lbl_size(), 1);
  ASSERT_STREQ(ret_.lbl(0).c_str(), "new-lbl");
}

TEST_F(RemoteCSETest, UpdateRR) {
  ASSERT_FALSE(last_test_bad_);

  const string csr_upd("{"
			"\"csr\"     : {"
				"\"rr\" 	: true "
			"}"
		"}");

  string ret_pc_;
  setupRequestPrim(update_request, csr_upd);
  ASSERT_TRUE(p_reqp_->setTo("//microwireless.com/in-cse-01/AN-CSE-01"));

  retrieveTestBody(ResponseStatusCode::CHANGED, "ab3f124a", exp_to_, exp_fr_, ret_pc_);
  pb::ResourceBase ret_;
  ASSERT_TRUE(ret_.ParseFromString(ret_pc_));
  ASSERT_TRUE(ret_.csr().rr());
}

TEST_F(RemoteCSETest, RetrieveUpdated) {
  ASSERT_FALSE(last_test_bad_);

  NSEBaseMockTest::setupRequestPrim(retrieve_request);
  ASSERT_TRUE(p_reqp_->setTo("//microwireless.com/in-cse-01/AN-CSE-01"));

  csr_pc_.set_ri(ri_);
  csr_pc_.mutable_csr()->set_rr(true);
  retrieveTestBody(ResponseStatusCode::OK, "ab3f124a", exp_to_, exp_fr_, csr_pc_);
}

TEST_F(RemoteCSETest, UpdateCst) {
  const string csr_upd("{"
			"\"csr\"     : {"
				"\"cst\" 	: 1 "
			"}"
		"}");

  setupRequestPrim(update_request, csr_upd);
  ASSERT_TRUE(p_reqp_->setTo("//microwireless.com/in-cse-01/AN-CSE-01"));

  retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, UpdateCb) {
  const string csr_upd("{"
			"\"csr\"     : {"
				"\"cb\" 	: \"//microwireless.com/IN-CSE-02\" "
			"}"
		"}");

  setupRequestPrim(update_request, csr_upd);
  ASSERT_TRUE(p_reqp_->setTo("//microwireless.com/in-cse-01/AN-CSE-01"));

  retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, UpdateCsi) {
  const string csr_upd("{"
			"\"csr\"     : {"
				"\"csi\" 	: \"AN-CSE-02\" "
			"}"
		"}");

  setupRequestPrim(update_request, csr_upd);
  ASSERT_TRUE(p_reqp_->setTo("//microwireless.com/in-cse-01/AN-CSE-01"));

  retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, CreateConflict) {
  setupRequestPrim(create_request, csr_content);
  p_reqp_->setTo("//microwireless.com/in-cse-01/AN-CSE-01");
  retrieveTestBody(ResponseStatusCode::CONFLICT, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, CreateConflict1) {
  setupRequestPrim(create_request, csr_content);
  p_reqp_->setName("AN-CSE-01");

  retrieveTestBody(ResponseStatusCode::CONFLICT, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, CreateConflict2) {
  setupRequestPrim(create_request, csr_content);
  p_reqp_->setTo("//in-cse-01.microwireless.com/AN-CSE-01");

  retrieveTestBody(ResponseStatusCode::CONFLICT, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, CreateNoContent) {
  NSEBaseMockTest::setupRequestPrim(create_request);
  retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, CreateInvalidContent) {
  setupRequestPrim(create_request, csr_content);
  p_reqp_->setContent("abcdefghijklmnopqrstuvwxyz1234567890");
  retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, CreateParentInvalid) {
  setupRequestPrim(create_request, csr_content);
  p_reqp_->setName("/IN-CSE-01/Bad/AN-CSE-01");

  retrieveTestBody(ResponseStatusCode::NOT_FOUND, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, CreateWithNPFields) {
  setupRequestPrim(create_request, csr_exp);
   retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, CreateNoMandaoryFields) {
	  const string csr_json("{"
					"\"csr\"     : {"
						"\"cb\" 	: \"FreshGo\""
					"}"
				"}");
   setupRequestPrim(create_request, csr_json);
   retrieveTestBody(ResponseStatusCode::BAD_REQUEST, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(RemoteCSETest, CreateNoRn) {
  string ret_pc_;
  setupRequestPrim(create_request, csr_content);
  retrieveTestBody(ResponseStatusCode::CREATED, "ab3f124a", exp_to_, exp_fr_, ret_pc_);
  pb::ResourceBase ret_;
  ASSERT_TRUE(ret_.ParseFromString(ret_pc_));
  ri_ = ret_.ri();
  ASSERT_FALSE(ri_.empty());
  ASSERT_STREQ(ret_.rn().c_str(), ri_.c_str()); // Resource name same as ri
  cout << "Responded ri: " << ri_ << endl;
}

