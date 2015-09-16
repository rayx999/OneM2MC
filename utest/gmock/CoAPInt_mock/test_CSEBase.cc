/*
 * test_CSEBase.cc
 *
 *  Created on: 2015年9月11日
 *      Author: weimi_000
 */

#include <iostream>

//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "CoAPInt_mock.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"
#include "ResourceBase.pb.h"
#include "CommonTypes.h"
#include "CommonUtils.h"
#include "CSEBase.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class CSEBaseTest : public CoAPIntMockTest {
protected:
	static const std::string retrieve_json;
	static const std::string cse_content;
	static const std::map<unsigned int, std::string> exp_opt_;

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
		"\"ver\": 1,"
		"\"type\": 0,"			// Confirmable
		"\"method\": 1,"		// Get
		"\"opt\": [ "
			"{ \"num\":  11, \"value\": \"//microwireless.com/IN-CSE-01/Z0005\" },"	// Uri_Path
			"{ \"num\": 256, \"value\": \"//microwireless.com/AE-01\" },"			// FR
			"{ \"num\": 257, \"value\": \"ab3f124a\" } "							// RQI
		"]"
	"}");

const map<unsigned int, string> CSEBaseTest::exp_opt_ = {
		{   3, "localhost" },						// Uri_Host
		{   7, "5555" },                            // Uri_Port
		{  11, "//microwireless.com/AE-01" },		// Uri_path
		{ 256, "//microwireless.com/IN-CSE-01" },	// FR
		{ 257, "ab3f124a" }							// RQI
};

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
  setupCoAPBinding(retrieve_json);
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Content,
		  exp_opt_, exp_pc_);
}

TEST_F(CSEBaseTest, RetrieveCSE2) {
  static const string json("{"
			"\"ver\": 1,"
			"\"type\": 0,"			// Confirmable
			"\"method\": 1,"		// Get
			"\"opt\": [ "
				"{ \"num\":  11, \"value\": \"//microwireless.com/IN-CSE-01/IN-CSE-01\" },"	// Uri_Path
				"{ \"num\": 256, \"value\": \"//microwireless.com/AE-01\" },"			// FR
				"{ \"num\": 257, \"value\": \"ab3f124a\" } "							// RQI
			"]"
		"}");
  setupCoAPBinding(json);
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Content,
		  exp_opt_, exp_pc_);
}

TEST_F(CSEBaseTest, RetrieveCSE3) {
  static const string json("{"
			"\"ver\": 1,"
			"\"type\": 0,"			// Confirmable
			"\"method\": 1,"		// Get
			"\"opt\": [ "
				"{ \"num\":  11, \"value\": \"/IN-CSE-01/Z0005\" },"					// Uri_Path
				"{ \"num\": 256, \"value\": \"//microwireless.com/AE-01\" },"			// FR
				"{ \"num\": 257, \"value\": \"ab3f124a\" } "							// RQI
			"]"
		"}");
  setupCoAPBinding(json);
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Content,
		  exp_opt_, exp_pc_);
}

TEST_F(CSEBaseTest, RetrieveCSE4) {
  static const string json("{"
			"\"ver\": 1,"
			"\"type\": 0,"			// Confirmable
			"\"method\": 1,"		// Get
			"\"opt\": [ "
		  	  	"{ \"num\":  11, \"value\": \"/IN-CSE-01/Z0005\" },"					// Uri_Path
				"{ \"num\":  15, \"value\": \"rt=3\" },"								// Uri_Query
				"{ \"num\": 256, \"value\": \"//microwireless.com/AE-01\" },"			// FR
				"{ \"num\": 257, \"value\": \"ab3f124a\" } "							// RQI
			"]"
		"}");
  setupCoAPBinding(json);
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Content,
		  exp_opt_, exp_pc_);
}

TEST_F(CSEBaseTest, NotExistResource) {
    static const string not_exist_json("{"
			"\"ver\": 1,"
			"\"type\": 0,"			// Confirmable
			"\"method\": 1,"		// Get
			"\"opt\": [ "
				"{ \"num\":  11, \"value\": \"/IN-CSE-01/blah\" },"					// Uri_Path
				"{ \"num\": 256, \"value\": \"//microwireless.com/AE-01\" },"			// FR
				"{ \"num\": 257, \"value\": \"ab3f124a\" } "							// RQI
			"]"
 		"}");

  setupCoAPBinding(not_exist_json);
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Not_Found,
		  exp_opt_);
}

TEST_F(CSEBaseTest, RetrieveCSENonBlockSync) {
  static const string json("{"
			"\"ver\": 1,"
			"\"type\": 0,"			// Confirmable
			"\"method\": 1,"		// Get
			"\"opt\": [ "
		  	  	"{ \"num\":  11, \"value\": \"/IN-CSE-01/Z0005\" },"					// Uri_Path
				"{ \"num\":  15, \"value\": \"rt=1\" },"								// Uri_Query NonBlockingSync
				"{ \"num\": 256, \"value\": \"//microwireless.com/AE-01\" },"			// FR
				"{ \"num\": 257, \"value\": \"ab3f124a\" } "							// RQI
			"]"
		"}");
  string pc_str_;
  pb::ResourceBase pc_;

  setupCoAPBinding(json);
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Code_None,
		  exp_opt_, pc_str_);
  ASSERT_TRUE(pc_.ParseFromString(pc_str_));
  req_ri_ = pc_.ri();
  cout << "Accepted Request RI = " << req_ri_ << endl;
  ASSERT_FALSE(req_ri_.empty());
}

TEST_F(CSEBaseTest, RetrieveNonBlockRequest) {
  // last test must be good
  ASSERT_FALSE(last_test_bad_);

  static const string json = string("{"
			"\"ver\": 1,"
			"\"type\": 0,"			// Confirmable
			"\"method\": 1,"		// Get
			"\"opt\": [ "
		  	  	"{ \"num\":  11, \"value\": \"/IN-CSE-01/" + req_ri_ + "\" },"			// Uri_Path
				"{ \"num\": 256, \"value\": \"//microwireless.com/AE-01\" },"			// FR
				"{ \"num\": 257, \"value\": \"ab3f124a\" } "							// RQI
			"]"
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

  setupCoAPBinding(json);
  retrieveTestBody(pb::CoAPTypes_MessageType_CoAP_ACK,
		  pb::CoAPTypes_ResponseCode_CoAP_Content,
		  exp_opt_, exp_req_);
}

