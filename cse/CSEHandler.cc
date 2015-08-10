/*
 * CSEHandler.cc
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#include <string>

#include "Request.h"
#include "Response.h"
#include "RequestHandler.h"
#include "CSEHandler.h"
#include "NSEBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

void CSEHandler::handleRequest(Request& req) {
	string pc_;
	RequestHandler::handleRequest(req);
	ResponseStatusCode rsc_ = isForMe(req, *rdb_.cse());

	if (rsc_ == RSC_OK) {
		switch (req.getOperation()) {
		case OPERATION_CREATE:
			break;
		case OPERATION_RETRIEVE:
			if (rdb_.isResourceValid(req.getTo())) {
				if (!composeContent(req, pc_, rdb_)) {
					cerr << "Retrieve resource " << req.getTo() << " failed.\n";
					rsc_ = RSC_NOT_FOUND;
				}
			} else {
				cerr << "Retrieve resource " << req.getTo() << " is not valid.\n";
				rsc_ = RSC_NOT_FOUND;
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

	Response rsp_(&req, rsc_, req.getRequestId());	// FIX ME: remove rqi
	if (rsc_ == RSC_OK && !pc_.empty()) {
		rsp_.setContent(pc_);
	}
	nse_.send(rsp_);
}

}	// OneM2M

}	// MicroWireless
