/*
 * NSEBase_mock.cc
 *
 *  Created on: 2015年7月24日
 *      Author: weimi_000
 */


#include <iostream>

//#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "RequestPrim.h"
#include "ResponsePrim.h"
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

CSEResourceStore * NSEBaseMockTest::rdb_;
NSEBaseMock * NSEBaseMockTest::nse_;
CSEHandler * NSEBaseMockTest::hdl_;
bool NSEBaseMockTest::last_test_bad_ = true;

void NSEBaseMockTest::SetUpTestCase()
{
    rdb_ = new CSEResourceStore("data/.store");
    nse_ = new NiceMock<NSEBaseMock>("127.0.0.1", "1234");
    hdl_ = new CSEHandler(*nse_, *rdb_);
    CSEServer::setCSEServer(*rdb_, *nse_, *hdl_);
}

void NSEBaseMockTest::TearDownTestCase()
{
    delete rdb_;
    delete nse_;
    delete hdl_;
    CSEServer::clrCSEServer();
}

void NSEBaseMockTest::TearDown() {
	last_test_bad_ = HasFailure();
	if (p_reqp_ != NULL) {
		delete p_reqp_;
		p_reqp_ = NULL;
	}
}

void NSEBaseMockTest::setupRequestPrim(const string& json) {
	p_reqp_ = new RequestPrim(json);
	ASSERT_TRUE(p_reqp_ != NULL);
}

void NSEBaseMockTest::handleRequest() {
	try {
     	hdl_->handleRequest(*p_reqp_);
	} catch (exception &e) {
		cerr << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

void NSEBaseMockTest::retrieveTestBody(ResponseStatusCode rsc, const string& rqi,
		const string& to, const string& fr, const pb::ResourceBase& exp)
{
	EXPECT_CALL(*nse_, run())
		.WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

	EXPECT_CALL(*nse_, send_response(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(rsc)),
				Property(&ResponsePrim::getTo, StrEq(to)),
				Property(&ResponsePrim::getFrom, StrEq(fr)),
				Property(&ResponsePrim::getRequestId, StrEq(rqi)),
				Property(&ResponsePrim::getContent, PbEq(&exp))), _, _))
		.Times(1);

	CSEServer::run();
}

void NSEBaseMockTest::retrieveTestBody(ResponseStatusCode rsc, const string& rqi,
		const string& to, const string& fr) {
	 EXPECT_CALL(*nse_, run())
		  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

	 EXPECT_CALL(*nse_, send_response(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(rsc)),
					Property(&ResponsePrim::getTo, StrEq(to)),
					Property(&ResponsePrim::getFrom, StrEq(fr)),
					Property(&ResponsePrim::getRequestId, StrEq(rqi))), _, _))
		  .Times(1);

	 CSEServer::run();
}

void NSEBaseMockTest::retrieveTestBody(ResponseStatusCode rsc, const string& rqi,
		const string& to, const string& fr, string& pc) {
	 EXPECT_CALL(*nse_, run())
		  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

	 EXPECT_CALL(*nse_, send_response(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(rsc)),
					Property(&ResponsePrim::getTo, StrEq(to)),
					Property(&ResponsePrim::getFrom, StrEq(fr)),
					Property(&ResponsePrim::getRequestId, StrEq(rqi)),
					Property(&ResponsePrim::getContent, StrSave(&pc))), _, _))
		  .Times(1);

	 CSEServer::run();
}

void NSEBaseMockTest::printResponse(ResponsePrim rsp) {
	cout << rsp.getJson() << endl;
}


