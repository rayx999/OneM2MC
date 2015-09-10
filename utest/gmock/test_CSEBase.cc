/*
 * test_CSEBase.cc
 *
 *  Created on: 2015年8月21日
 *      Author: weimi_000
 */

#include <iostream>

//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "NSEBase_mock.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"
#include "ResourceBase.pb.h"
#include "CommonTypes.h"
#include "CommonUtils.h"
#include "CSEBase.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class CSEBaseTest : public NSEBaseMockTest {
protected:
	static const std::string retrieve_json;
	static const std::string cse_content;
	static const string exp_to_, exp_fr_;

	static pb::ResourceBase exp_pc_;
	static string req_ri_;

public:
	CSEBaseTest() {}

    virtual void SetUp()
    {
         json2pb(exp_pc_, cse_content.c_str(), cse_content.length());
    }
};

pb::ResourceBase CSEBaseTest::exp_pc_;
string CSEBaseTest::req_ri_;

const string CSEBaseTest::retrieve_json("{"
		"\"op\": 2,"
		"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\","
		"\"rqi\": \"ab3f124a\","
		"\"fr\": \"//microwireless.com/AE-01\""
	"}");

const string CSEBaseTest::exp_to_("//microwireless.com/AE-01");
const string CSEBaseTest::exp_fr_("//microwireless.com/IN-CSE-01");

const string CSEBaseTest::cse_content("{"
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

TEST_F(CSEBaseTest, RetrieveCSE) {
  setupRequestPrim(retrieve_json);
  retrieveTestBody(RSC_OK, "ab3f124a", exp_to_, exp_fr_, exp_pc_);
}

TEST_F(CSEBaseTest, RetrieveCSE2) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01/IN-CSE-01\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");
  setupRequestPrim(json);
  retrieveTestBody(RSC_OK, "ab3f124a", exp_to_, exp_fr_, exp_pc_);
}

TEST_F(CSEBaseTest, RetrieveCSE3) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"/IN-CSE-01/Z0005\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");
  setupRequestPrim(json);
  retrieveTestBody(RSC_OK, "ab3f124a", exp_to_, exp_fr_, exp_pc_);
}

TEST_F(CSEBaseTest, RetrieveCSE4) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01/Z0005\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\", "
		    "\"rt\": 3"
		  "}");
  setupRequestPrim(json);
  retrieveTestBody(RSC_OK, "ab3f124a", exp_to_, exp_fr_, exp_pc_);
}

TEST_F(CSEBaseTest, NotExistResource) {
    static const string not_exist_json("{"
			"\"op\": 2,"
			"\"to\": \"//microwireless.com/IN-CSE-01/blah\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\""
		"}");

  setupRequestPrim(not_exist_json);
  retrieveTestBody(RSC_NOT_FOUND, "ab3f124a", exp_to_, exp_fr_);
}

TEST_F(CSEBaseTest, RetrieveCSENonBlockSync) {
  static const string json("{"
			"\"op\": 2,"
			"\"to\": \"/IN-CSE-01/Z0005\","
			"\"rqi\": \"ab3f124a\","
			"\"fr\": \"//microwireless.com/AE-01\", "
		    "\"rt\": 1"
		"}");
  string pc_str_;
  pb::ResourceBase pc_;

  setupRequestPrim(json);
  retrieveTestBody(RSC_ACCEPTED, "ab3f124a", exp_to_, exp_fr_, pc_str_);
  ASSERT_TRUE(pc_.ParseFromString(pc_str_));
  req_ri_ = pc_.ri();
  cout << "Accepted Request RI = " << req_ri_ << endl;
  ASSERT_FALSE(req_ri_.empty());
}

TEST_F(CSEBaseTest, RetrieveNonBlockRequest) {
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

  setupRequestPrim(json);
  retrieveTestBody(RSC_OK, "ab3f124a", exp_to_, exp_fr_, exp_req_);
//  pb::ResourceBase res_;
//  ASSERT_TRUE(res_.ParseFromString(str_));
//  cout << pb2json(res_) << endl;
}

