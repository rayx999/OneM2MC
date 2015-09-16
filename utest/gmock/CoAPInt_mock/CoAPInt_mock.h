/*
 * CoAPInt_mock.h
 *
 *  Created on: 2015年7月24日
 *      Author: weimi_000
 */

#ifndef UTEST_GMOCK_COAPINT_MOCK_H_
#define UTEST_GMOCK_COAPINT_MOCK_H_

#include <iostream>
#include <string>

#include "gmock/gmock.h"

#include "CoAPBinding.pb.h"
#include "ResourceBase.pb.h"
#include "CommonTypes.h"
#include "NSECoAPBinding.h"
#include "CSEResourceStore.h"
#include "CSEHandler.h"
#include "CSEServer.h"
#include "CoAPInt.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class CoAPIntMock : public CoAPInt {
public:
  CoAPIntMock(const char* ip, const char* port) : CoAPInt(ip, port) {};
  MOCK_METHOD0(run, void());
  MOCK_METHOD1(send, void(pb::CoAPBinding&));
};

class CoAPIntMockTest : public ::testing::Test {
protected:
	static CSEResourceStore * rdb_;
	static CoAPIntMock * coap_int_;
	static NSE_CoAP * nse_;
	static CSEHandler * hdl_;
	static CSEServer * server_;
	static bool last_test_bad_;

	pb::CoAPBinding* p_coap_;
	RequestPrim* p_reqp_;

	boost::mutex mutex_;
	boost::condition_variable cond_var_;
	bool done_;

	void waitForSend();

public:
	CoAPIntMockTest() : p_coap_(), p_reqp_() {}

    static void SetUpTestCase();
    static void TearDownTestCase();

    virtual void SetUp();
    virtual void TearDown();

    void setupCoAPBinding(const string& json);

    void handleRequest();

    void retrieveTestBody(pb::CoAPTypes_MessageType type, pb::CoAPTypes_ResponseCode code,
    		const map<unsigned int, string>& opt, const pb::ResourceBase& exp);
    void retrieveTestBody(pb::CoAPTypes_MessageType type, pb::CoAPTypes_ResponseCode code,
    		const map<unsigned int, string>& opt);
    void retrieveTestBody(pb::CoAPTypes_MessageType type, pb::CoAPTypes_ResponseCode code,
    		const map<unsigned int, string>& opt, string& pc);
};

#endif /* UTEST_GMOCK_COAPINT_MOCK_H_ */
