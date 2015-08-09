/*
 * CSEHandler.cc
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#include <string>

#include "Request.h"
#include "Response.h"
//#include "RequestHandler.h"
#include "CSEHandler.h"
#include "NSEBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

void CSEHandler::handleRequest(Request& req) {
	const string* p_pc_ = NULL;
	RequestHandler::handleRequest(req);
	ResponseStatusCode rsc_ = isForMe(req, *rdb_.cse());

	if (rsc_ == RSC_OK) {
		switch (req.getOperation()) {
		case OPERATION_CREATE:
			break;
		case OPERATION_RETRIEVE:
			if (rdb_.isResourceValid(req.getTo())) {
				CSEBase cse_;
				if (cse_.setCSEBase(req.getTo(), rdb_)) {
					p_pc_ = composeContent(req, cse_);
				} else {
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
	if (p_pc_ != NULL && !(*p_pc_).empty()) {
		rsp_.setContent(*p_pc_);
	}
	nse_.send(rsp_);
}

}	// OneM2M

}	// MicroWireless
