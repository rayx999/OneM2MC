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
CSEServer * NSEBaseMockTest::server_;
bool NSEBaseMockTest::last_test_bad_ = true;

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


void NSEBaseMockTest::SetUpTestCase()
{
    rdb_ = new CSEResourceStore("data/.store");
    nse_ = new NiceMock<NSEBaseMock>("127.0.0.1", "1234");
    hdl_ = new CSEHandler(*nse_, *rdb_);
    server_ = new CSEServer(*rdb_, *nse_, *hdl_);
}

void NSEBaseMockTest::TearDownTestCase()
{
    delete rdb_;
    delete nse_;
    delete hdl_;
    delete server_;
}

void NSEBaseMockTest::TearDown() {
	last_test_bad_ = HasFailure();
}

void NSEBaseMockTest::setJson(const string* json) {
	json_ = json;
}

void NSEBaseMockTest::handleRequest() {
	try {
		RequestPrim req_(*json_);
     	hdl_->handleRequest(req_);
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

	EXPECT_CALL(*nse_, send(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(rsc)),
				Property(&ResponsePrim::getTo, StrEq(to)),
				Property(&ResponsePrim::getFrom, StrEq(fr)),
				Property(&ResponsePrim::getRequestId, StrEq(rqi)),
				Property(&ResponsePrim::getContent, PbEq(exp)))))
		.Times(1);

	server_->run();
}

void NSEBaseMockTest::retrieveTestBody(ResponseStatusCode rsc, const string& rqi,
		const string& to, const string& fr) {
	 EXPECT_CALL(*nse_, run())
		  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

	 EXPECT_CALL(*nse_, send(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(rsc)),
					Property(&ResponsePrim::getTo, StrEq(to)),
					Property(&ResponsePrim::getFrom, StrEq(fr)),
					Property(&ResponsePrim::getRequestId, StrEq(rqi)))))
		  .Times(1);

	 server_->run();
}

void NSEBaseMockTest::retrieveTestBody(ResponseStatusCode rsc, const string& rqi,
		const string& to, const string& fr, string& pc) {
	 EXPECT_CALL(*nse_, run())
		  .WillOnce(Invoke(this, &NSEBaseMockTest::handleRequest));

	 EXPECT_CALL(*nse_, send(AllOf(Property(&ResponsePrim::getResponseStatusCode, Eq(rsc)),
					Property(&ResponsePrim::getTo, StrEq(to)),
					Property(&ResponsePrim::getFrom, StrEq(fr)),
					Property(&ResponsePrim::getRequestId, StrEq(rqi)),
					Property(&ResponsePrim::getContent, StrGood(&pc)))))
		  .Times(1);

	 server_->run();
}

void NSEBaseMockTest::printResponse(ResponsePrim rsp) {
	cout << rsp.getJson() << endl;
}


