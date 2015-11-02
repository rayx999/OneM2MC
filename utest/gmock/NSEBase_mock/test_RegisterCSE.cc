/*
 * test_RegisterCSE.cc
 *
 *  Created on: 2015年10月29日
 *      Author: weimi_000
 */


#include <iostream>
#include <string>
#include <boost/bind.hpp>
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
#include "CSEApi.h"

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
using ::testing::InSequence;
using ::testing::Eq;
using ::testing::_;

class RegisterCSETest : public NSEBaseMockTest {
protected:
	static const std::string csr_, csr_rsp_, csb_;
	static const string in_cse_01, in_cse_02;
	string csr_rsp_str_, csb_str_;
	pb::ResourceBase csr_pb_;
	IApiCallBackMock* p_cb_;

public:
	RegisterCSETest() {}

    virtual void SetUp() {
      	json2pb(csr_pb_, csr_.c_str(), csr_.length());
      	pb::ResourceBase csr_rsp_pb_;
      	json2pb(csr_rsp_pb_, csr_rsp_.c_str(), csr_rsp_.length());
      	csr_rsp_pb_.SerializeToString(&csr_rsp_str_);
      	pb::ResourceBase csb_pb_;
      	json2pb(csb_pb_, csb_.c_str(), csb_.length());
      	csb_pb_.SerializeToString(&csb_str_);
      	p_cb_ = new IApiCallBackMock("API-00001");
    }

    virtual void TearDown() {
    	delete p_cb_;
    }

    void registerCSE() {
    	CSEApi::registerCSE(in_cse_02, p_cb_);
    }
};

const string RegisterCSETest::csr_("{"
			"\"csr\"     : {"
				"\"cst\" 	: 1,"
				"\"csi\" 	: \"/IN-CSE-01\","
				"\"rr\"		: true, "
				"\"cb\" 	: \"//microwireless.com/IN-CSE-01\" "
			"}"
		"}");

const string RegisterCSETest::csr_rsp_("{"
			"\"pi\"     : \"Z0005B\" "
		"}");

const string RegisterCSETest::csb_("{"
			"\"ty\" 	: 5,"
			"\"ri\" 	: \"Z0005B\","
			"\"rn\" 	: \"IN-CSE-02\","
			"\"ct\" 	: { \"seconds\" : 1435434103 },"
			"\"csb\"    : {"
				"\"cst\" 	: 1,"
				"\"csi\" 	: \"/IN-CSE-02\","
				"\"srt\" 	: [ 2, 5, 16 ]"
			"}"
		"}");

const string RegisterCSETest::in_cse_01("//microwireless.com/IN-CSE-01");
const string RegisterCSETest::in_cse_02("//microwireless.com/IN-CSE-02");

TEST_F(RegisterCSETest, RegisterCSE) {
	 EXPECT_CALL(*nse_, run())
		  .WillOnce(Invoke(this, &RegisterCSETest::registerCSE));
	 {
		// InSequence s;  Callback will not be in sequence due to gmock_nse call stack in one thread.
		EXPECT_CALL(*p_cb_, registerCSECb(_, _, ApiStatus::ACCEPTED, _));
		EXPECT_CALL(*p_cb_, registerCSECb(_, _, ApiStatus::REMOTE_CSE_CREATE_REQ_SENT, _));
		EXPECT_CALL(*p_cb_, registerCSECb(_, _, ApiStatus::REMOTE_CSE_CREATE_RSP_RECV, _));
		EXPECT_CALL(*p_cb_, registerCSECb(_, _, ApiStatus::REMOTE_CSE_RETRIEVE_REQ_SENT, _));
		EXPECT_CALL(*p_cb_, registerCSECb(_, _, ApiStatus::REMOTE_CSE_RETRIEVE_RSP_RECV, _));
		EXPECT_CALL(*p_cb_, registerCSECb(_, _, ApiStatus::SUCCESS, _));
	 }
	 {
		 InSequence s;
		 EXPECT_CALL(*nse_, send_request(AllOf(Property(&RequestPrim::getOperation, Eq(Operation::CREATE)),
					Property(&RequestPrim::getTo, StrEq(in_cse_02)),
					Property(&RequestPrim::getFrom, StrEq(in_cse_01)),
					Property(&RequestPrim::getRequestId, StrPrn()),
					Property(&RequestPrim::getContent, PbEq(&csr_pb_))), _, _))
		 	 .WillOnce(handleResponse(hdl_, ResponseStatusCode::CREATED, in_cse_02, csr_rsp_str_));

		 EXPECT_CALL(*nse_, send_request(AllOf(Property(&RequestPrim::getOperation, Eq(Operation::RETRIEVE)),
					Property(&RequestPrim::getTo, StrEq(in_cse_02 + "/Z0005B")),
					Property(&RequestPrim::getFrom, StrEq(in_cse_01)),
					Property(&RequestPrim::getRequestId, StrPrn())), _, _))
		 	 .WillOnce(handleResponse(hdl_, ResponseStatusCode::OK, in_cse_02, csb_str_));
	 }
	 CSEServer::run();
}
