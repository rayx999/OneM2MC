/*
 * NSEBase_mock.cc
 *
 *  Created on: 2015年7月24日
 *      Author: weimi_000
 */


#include <iostream>

//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "CommonTypes.h"
#include "CSEBase.h"
#include "Request.h"
#include "Response.h"
#include "ResourceStore.h"
#include "NSEBase_mock.h"
#include "CSEResourceStore.h"
#include "CSEHandler.h"
#include "CSEServer.h"

using namespace MicroWireless::OneM2M;

using ::testing::NiceMock;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Property;
using ::testing::Eq;

class NSEBaseMockTest : public ::testing::Test {
protected:
	static const std::string retrieve_json;
	Request * req_;
	Response * rsp_;
	CSEResourceStore * rdb_;
	NSEBaseMock * nse_;
	CSEHandler * hdl_;
	CSEServer * server_;

public:
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

    void handleRequest() {
    	req_ = new Request(retrieve_json);
    	hdl_->handleRequest(*req_);
    }

};

const string NSEBaseMockTest::retrieve_json("{\"op\": 2, \"to\": \"//microwireless.com/IN-CSE-01\", \"rqi\": \"ab3f124a\", \"fr\": \"//microwireless.com/AE-01\"}");

TEST_F(NSEBaseMockTest, RetrieveCSE) {

	 std::cout << "EXPECT_CALL() starts.." << std::endl;

  EXPECT_CALL(*nse_, run()).Times(1);
//	  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

  std::cout << "EXPECT_CALL() done" << std::endl;

  ON_CALL(*nse_, send(Property(&Response::getResponseStatusCode, Eq(RSC_OK))));

  std::cout << "ON_CALL() done" << std::endl;


  server_->run();

}
