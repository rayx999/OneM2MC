/*
 * CoAPInt_mock.cc
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
#include "CSEResourceStore.h"
#include "CSEHandler.h"
#include "CSEServer.h"
#include "NSEBase.h"
#include "NSECoAPBinding.h"
#include "CoAPInt_mock.h"

using namespace std;
using namespace MicroWireless::OneM2M;

using ::testing::NiceMock;
using ::testing::AllOf;
using ::testing::StrEq;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Property;
using ::testing::Eq;
using ::testing::IsNull;
using ::testing::_;

CSEResourceStore * CoAPIntMockTest::rdb_;
CoAPIntMock * CoAPIntMockTest::coap_int_;
NSE_CoAP * CoAPIntMockTest::nse_;
CSEHandler * CoAPIntMockTest::hdl_;
CSEServer * CoAPIntMockTest::server_;
bool CoAPIntMockTest::last_test_bad_ = true;

MATCHER_P(OptEq, exp_opt, "") {
	for (auto i = arg.begin(); i != arg.end(); i++) {
		if (exp_opt.find(i->num()) == exp_opt.end() ||
			exp_opt.at(i->num()) != i->value()) {
			return false;
		}
	}
	return true;
}

ACTION_P3(sendInvokedNotify, p_m, p_cv, p_done) {
    boost::mutex::scoped_lock lock(*p_m);
    *p_done = true;
    p_cv->notify_one();
}

void CoAPIntMockTest::SetUpTestCase()
{
    rdb_ = new CSEResourceStore("data/.store");
    coap_int_ = new NiceMock<CoAPIntMock>("127.0.0.1", "1234");
    nse_ = new NSE_CoAP("127.0.0.1", "1234", *coap_int_);
    hdl_ = new CSEHandler(*nse_, *rdb_);
    server_ = new CSEServer(*rdb_, *nse_, *hdl_);
}

void CoAPIntMockTest::TearDownTestCase()
{
    delete rdb_;
    delete coap_int_;
    delete nse_;
    delete hdl_;
    delete server_;
}

void CoAPIntMockTest::SetUp() {
	p_coap_ = NULL;
	p_reqp_ = NULL;
	done_ = false;
}

void CoAPIntMockTest::TearDown() {
	last_test_bad_ = HasFailure();
	if (p_coap_ != NULL) {
		delete p_coap_;
		p_coap_ = NULL;
	}
	if (p_reqp_ != NULL) {
		delete p_reqp_;
		p_reqp_ = NULL;
	}
}

void CoAPIntMockTest::setupCoAPBinding(const string& json) {
	p_coap_ = new pb::CoAPBinding();
	ASSERT_TRUE(p_coap_ != NULL);
	try {
		json2pb(*p_coap_, json.c_str(), json.length());
	} catch (exception &e) {
		cerr << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

void CoAPIntMockTest::handleRequest() {
	try {
		ASSERT_TRUE(nse_->convertCoAPRequest(*p_coap_, p_reqp_));
     	nse_->post(*p_reqp_, hdl_);
	} catch (exception &e) {
		cerr << "Unexpected exception: " << e.what() << endl;
		ASSERT_TRUE(false);
	}
}

void CoAPIntMockTest::waitForSend() {
    boost::mutex::scoped_lock lock(mutex_);
    cond_var_.timed_wait(lock, boost::posix_time::seconds(1));
}

void CoAPIntMockTest::retrieveTestBody(pb::CoAPTypes_MessageType type, pb::CoAPTypes_ResponseCode code,
		const ExpOption& opt, const pb::ResourceBase& exp)
{
	EXPECT_CALL(*coap_int_, run())
		.WillOnce(Invoke(this, &CoAPIntMockTest::handleRequest));

	EXPECT_CALL(*coap_int_, send(AllOf(Property(&pb::CoAPBinding::ver, Eq((unsigned int)1)),
				Property(&pb::CoAPBinding::type, Eq(type)),
				Property(&pb::CoAPBinding::code, Eq(code)),
				Property(&pb::CoAPBinding::opt_size, Eq((int)opt.size())),
				Property(&pb::CoAPBinding::opt, OptEq(opt)),
				Property(&pb::CoAPBinding::payload, PbEq(&exp)))))
        .WillOnce(sendInvokedNotify(&mutex_, &cond_var_, &done_));

	server_->run();

	waitForSend();
    EXPECT_TRUE(done_);
}

void CoAPIntMockTest::retrieveTestBody(pb::CoAPTypes_MessageType type, pb::CoAPTypes_ResponseCode code,
		const ExpOption& opt) {

	EXPECT_CALL(*coap_int_, run())
		.WillOnce(Invoke(this, &CoAPIntMockTest::handleRequest));

	EXPECT_CALL(*coap_int_, send(AllOf(Property(&pb::CoAPBinding::ver, Eq((unsigned int)1)),
				Property(&pb::CoAPBinding::type, Eq(type)),
				Property(&pb::CoAPBinding::code, Eq(code)),
				Property(&pb::CoAPBinding::opt_size, Eq((int)opt.size())),
				Property(&pb::CoAPBinding::opt, OptEq(opt)))))
        .WillOnce(sendInvokedNotify(&mutex_, &cond_var_, &done_));

	server_->run();

	waitForSend();
    EXPECT_TRUE(done_);
}

void CoAPIntMockTest::retrieveTestBody(pb::CoAPTypes_MessageType type, pb::CoAPTypes_ResponseCode code,
		const ExpOption& opt, string& pc) {

	EXPECT_CALL(*coap_int_, run())
		 .WillOnce(Invoke(this, &CoAPIntMockTest::handleRequest));

	EXPECT_CALL(*coap_int_, send(AllOf(Property(&pb::CoAPBinding::ver, Eq((unsigned int)1)),
				Property(&pb::CoAPBinding::type, Eq(type)),
				Property(&pb::CoAPBinding::code, Eq(code)),
				Property(&pb::CoAPBinding::opt_size, Eq((int)opt.size())),
				Property(&pb::CoAPBinding::opt, OptEq(opt)),
				Property(&pb::CoAPBinding::payload, StrSave(&pc)))))
	    .WillOnce(sendInvokedNotify(&mutex_, &cond_var_, &done_));

	server_->run();

	waitForSend();
	EXPECT_TRUE(done_);
}



