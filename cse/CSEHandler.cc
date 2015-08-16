/*
 * CSEHandler.cc
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#include <RequestPrim.h>
#include <ResponsePrim.h>
#include <string>

#include "RequestHandler.h"
#include "CSEHandler.h"
#include "NSEBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

void CSEHandler::handleRequest(RequestPrim& req) {
	string pc_;
	RequestHandler::handleRequest(req);
	ResponseStatusCode rsc_ = isForMe(req, *rdb_.cse());

	if (rsc_ == RSC_OK) {
		string target_;
		switch (req.getOperation()) {
		case OPERATION_CREATE:
			break;
		case OPERATION_RETRIEVE:
			getResourceHAddress(req, target_, *rdb_.cse());
			if (rdb_.isResourceValid(target_)) {
				req.setTargetResource(target_);
			} else if (rdb_.isResourceValid(req.getIntRn())) {
				req.setTargetResource(req.getIntRn());
			} else {
				cerr << "handleRequest: resource not found. ri:";
				cerr << req.getIntRn() << " to:" << target_ << endl;
				rsc_ = RSC_NOT_FOUND;
				break;
			}
			if (!composeContent(req, pc_, rdb_)) {
				cerr << "handleRequest: Retrieve resource " << req.getTargetResource();
				cerr << " failed.\n";
				rsc_ = RSC_INTERNAL_SERVER_ERROR;
			}
			break;
		case OPERATION_UPDATE:
			break;
		case OPERATION_DELETE:
			break;
		case OPERATION_NOTIFY:
			break;
		default:
			rsc_ = RSC_BAD_REQUEST;
			break;
		}
	}

	ResponsePrim rsp_(&req, rsc_, req.getRequestId());	// FIX ME: remove rqi
	if (rsc_ == RSC_OK && !pc_.empty()) {
		rsp_.setContent(pc_);
	}
	nse_.send(rsp_);
}

}	// OneM2M

}	// MicroWireless
