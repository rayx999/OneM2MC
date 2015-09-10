/*
 * NSEBase_mock.h
 *
 *  Created on: 2015年7月24日
 *      Author: weimi_000
 */

#ifndef UTEST_GMOCK_NSEBASE_MOCK_H_
#define UTEST_GMOCK_NSEBASE_MOCK_H_

#include <iostream>
#include <string>

#include "gmock/gmock.h"

#include "ResourceBase.pb.h"
#include "CommonTypes.h"
#include "NSEBase.h"
#include "CSEResourceStore.h"
#include "CSEHandler.h"
#include "CSEServer.h"

using namespace std;
using namespace MicroWireless::OneM2M;

class NSEBaseMock : public NSEBase {
public:
  NSEBaseMock(const char* ip, const char* port) : NSEBase(ip, port) {};
  MOCK_METHOD0(run, void());
  MOCK_METHOD3(send, void(ResponsePrim&, const std::string&, uint));
};

class NSEBaseMockTest : public ::testing::Test {
protected:
	static CSEResourceStore * rdb_;
	static NSEBaseMock * nse_;
	static CSEHandler * hdl_;
	static CSEServer * server_;
	static bool last_test_bad_;

	RequestPrim* p_reqp_;

public:
	NSEBaseMockTest() : p_reqp_() {}

    static void SetUpTestCase();
    static void TearDownTestCase();

    virtual void SetUp() {}
    virtual void TearDown();

    void setupRequestPrim(const string& json);

    void handleRequest();

    void retrieveTestBody(ResponseStatusCode rsc, const string& rqi,
    		const string& to, const string& fr, const pb::ResourceBase& exp);
    void retrieveTestBody(ResponseStatusCode rsc, const string& rqi,
    		const string& to, const string& fr);
    void retrieveTestBody(ResponseStatusCode rsc, const string& rqi,
    		const string& to, const string& fr, string& pc);

    void printResponse(ResponsePrim rsp);
};

#endif /* UTEST_GMOCK_NSEBASE_MOCK_H_ */
