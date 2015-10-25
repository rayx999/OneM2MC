/*
 * gmock_common.h
 *
 *  Created on: 2015年10月3日
 *      Author: weimi_000
 */

#ifndef UTEST_GMOCK_GMOCK_COMMON_H_
#define UTEST_GMOCK_GMOCK_COMMON_H_

#include <iostream>
#include <string>

#include "gmock/gmock.h"

#include "CopyMessage.pb.h"
#include "ResourceBase.pb.h"
#include "CSEBase.pb.h"
#include "Request.pb.h"
#include "AE.pb.h"
#include "CommonTypes.h"
#include "NSEBase.h"
#include "CSEResourceStore.h"
#include "CSEHandler.h"
#include "CSEServer.h"

using namespace std;
using namespace MicroWireless::OneM2M;

using google::protobuf::Message;

// Matcher for keep test arg to val
MATCHER_P(StrSave, val, "") {
	if (arg.empty()) {
		return false;
	} else {
		*val = arg;
		return true;
	}
}

inline bool matcher_cse(const pb::CSEBase& act, const pb::CSEBase& exp) {
	return  act.cst() == exp.cst() &&
			act.csi() == exp.csi();
}

inline bool matcher_req(const pb::Request& act, const pb::Request& exp) {
	string act_str_;
	string exp_str_;
	if (act.SerializeToString(&act_str_) &&
		exp.SerializeToString(&exp_str_)) {
		return  act_str_ == exp_str_;
	}
	cerr << "macher_req: can't serialize to string\n";
	return false;
}

inline bool matcher_ae(const pb::AE& act, const pb::AE& exp) {
	return act.apn() == exp.apn() &&
			act.api() == exp.api() &&
			act.aei() == exp.aei();
}

MATCHER_P(PbEq, exp_res, "") {
	pb::ResourceBase act_res_;
	if (!act_res_.ParseFromString(arg)) {
		return false;
	}

	if  (act_res_.ty() == exp_res->ty() &&
		 act_res_.ri() == exp_res->ri() &&
		 act_res_.rn() == exp_res->rn() &&
		 act_res_.pi() == exp_res->pi() )
	{
		if (act_res_.has_csb()) {
			return matcher_cse(act_res_.csb(), exp_res->csb());
		} else if (act_res_.has_req()) {
			return CompareMessage(act_res_.req(), exp_res->req());
		} else if (act_res_.has_ae()) {
			return CompareMessage(act_res_.ae(), exp_res->ae());
		} else if (act_res_.has_csr()) {
			return CompareMessage(act_res_.csr(), exp_res->csr());
		} else {
			cerr << "PbEq: No sub-resource.\n";
		}
	}
	return false;
}

#endif /* UTEST_GMOCK_GMOCK_COMMON_H_ */
