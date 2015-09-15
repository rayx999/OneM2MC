/*
 * CSEHandler.cc
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>

#include "RequestPrim.h"
#include "ResponsePrim.h"
#include "ResourceBase.h"
#include "RequestHandler.h"
#include "RequestCreateHandler.h"
#include "CSEHandler.h"
#include "NSEBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

void CSEHandler::handleRequest(RequestPrim& req) {
	//cout << "CSEHandler::handleRequest: start...\n";

	if (!checkRequest(req, rdb_)) {
		return;
	}

	string pc_;
	ResponseStatusCode rsc_ = ResponseStatusCode::OK;

	switch (req.getOperation()) {
	case Operation::CREATE:
	{
		RequestCreateHandler<CSEBase> rch_(req, rdb_);
		rsc_ = rch_.setResourceToBeCreated();
		if (rsc_ == ResponseStatusCode::OK) {
			if (!rch_.outToResourceStore()) {
				rsc_ = ResponseStatusCode::INTERNAL_SERVER_ERROR;
			} else if (!rch_.composeContent(pc_)) {
				rsc_ = ResponseStatusCode::INTERNAL_SERVER_ERROR;
			} else {
				rsc_ = ResponseStatusCode::CREATED;
				rch_.saveParentLastModifiedTime();
			}
		}
		break;
	}
	case Operation::RETRIEVE:
	{
		string target_;
		getResourceHAddress(req, target_, *rdb_.cse());

		if (rdb_.isResourceValid(req.getTo())) {
			req.setTargetResource(req.getTo());
		} else if (rdb_.isResourceValid(req.getIntRn())) {
			req.setTargetResource(req.getIntRn());
		} else {
			cerr << "handleRequest: resource not found. ri:";
			cerr << req.getIntRn() << " to: " << req.getTo() << endl;
			rsc_ = ResponseStatusCode::NOT_FOUND;
			break;
		}

		//cout << "CSEHandler::handleRequest: Prepare content..." << endl;

		if (!composeContent(req, pc_, rdb_)) {
			cerr << "handleRequest: Retrieve resource " << req.getTargetResource();
			cerr << " failed.\n";
			rsc_ = ResponseStatusCode::INTERNAL_SERVER_ERROR;
		}
		break;
	}
	case Operation::UPDATE:
		break;
	case Operation::DDELETE:
		break;
	case Operation::NOTIFY:
		break;
	default:
		rsc_ = ResponseStatusCode::BAD_REQUEST;
		break;
	}

	string fr_ = rdb_.getRoot()->getDomain() + rdb_.getRoot()->getCSEId();
	//cout << "CSEHandler::handleRequest: compose fr:" << fr_ << endl;
	ResponsePrim rsp_(&req, rsc_, fr_);
	if ((rsc_ == ResponseStatusCode::OK || rsc_ == ResponseStatusCode::CREATED) && !pc_.empty()) {
		rsp_.setContent(pc_);
	}
	nse_.send(rsp_, "localhost", 5555);
}

}	// OneM2M

}	// MicroWireless
