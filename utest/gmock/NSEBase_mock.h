/*
 * NSEBase_mock.h
 *
 *  Created on: 2015年7月24日
 *      Author: weimi_000
 */

#ifndef UTEST_GMOCK_NSEBASE_MOCK_H_
#define UTEST_GMOCK_NSEBASE_MOCK_H_

#include "../../include/RequestPrim.h"
#include "../../include/ResponsePrim.h"
#include "gmock/gmock.h"

#include "CommonTypes.h"
#include "NSEBase.h"


using namespace MicroWireless::OneM2M;

class NSEBaseMock : public NSEBase {
public:
  NSEBaseMock(const char* ip, const char* port) : NSEBase(ip, port) {};
  MOCK_METHOD0(run, void());
  MOCK_METHOD1(send, void(ResponsePrim& rsp));
};

#endif /* UTEST_GMOCK_NSEBASE_MOCK_H_ */
