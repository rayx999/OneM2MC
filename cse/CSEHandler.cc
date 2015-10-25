/*
 * CSEHandler.cc
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

//#include "AE.h"
//#include "AEAnnc.h"
#include "RequestCache.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"
#include "ResourceBase.h"
#include "RequestHandler.h"
#include "RequestCreateHandler.h"
#include "RequestUpdateHandler.h"
#include "CSEHandler.h"
#include "NSEBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

map<SupportedResourceType, CSEHandler::anncCreator> CSEHandler::annc_create_;

CSEHandler::CSEHandler(NSEBase& nse, CSEResourceStore& rdb)
	: RequestHandler(nse), rdb_(rdb) {
	annc_create_.clear();
	annc_create_.insert( { SupportedResourceType::AE,
			boost::bind(&CSEHandler::createAnnc<AEClass, AEAnnc>, this, _1, _2, _3, _4) } );
}

void CSEHandler::handleRequest(RequestPrim& reqp) {
	//cout << "CSEHandler::handleRequest: start...\n";

	if (!checkRequest(reqp, rdb_)) {
		return;
	}

	string pc_;
	ResponseStatusCode rsc_ = ResponseStatusCode::OK;

	switch (reqp.getOperation()) {
	case Operation::CREATE:
	{
		RequestCreateHandler<CSEBase> rch_(reqp, rdb_);
		rsc_ = rch_.setResourceToBeCreated();
		if (rsc_ == ResponseStatusCode::OK) {
			if (!rch_.outToResourceStore()) {
				rsc_ = ResponseStatusCode::INTERNAL_SERVER_ERROR;
			} else  if (!rch_.composeContent(pc_)) {
				rsc_ = ResponseStatusCode::INTERNAL_SERVER_ERROR;
			} else if (!rch_.getNewResource().getAnncTo().empty()) {
				const ResourceBase& res_ = rch_.getNewResource();
				// keep current request pending
				reqc_.addRequest(reqp, pc_);
				if (initiateAnnc(reqp, res_, rsc_)) {
					return;
				}
			} else {
				rsc_ = ResponseStatusCode::CREATED;
				rch_.saveParentLastModifiedTime();
			}
		}
		break;
	}
	case Operation::RETRIEVE:
	case Operation::UPDATE:
	{
		string target_;
		getResourceHAddress(reqp, target_, *rdb_.cse());

		if (rdb_.isResourceValid(reqp.getTo())) {
			reqp.setTargetResource(reqp.getTo());
		} else if (rdb_.isResourceValid(reqp.getIntRn())) {
			reqp.setTargetResource(reqp.getIntRn());
		} else {
			cerr << "handleRequest: resource not found. ri:";
			cerr << reqp.getIntRn() << " to: " << reqp.getTo() << endl;
			rsc_ = ResponseStatusCode::NOT_FOUND;
			break;
		}

		//cout << "CSEHandler::handleRequest: Prepare content..." << endl;
		if (reqp.getOperation() == Operation::RETRIEVE) {
			if (!composeContent(reqp, pc_, rdb_)) {
				cerr << "handleRequest: Retrieve resource " << reqp.getTargetResource();
				cerr << " failed.\n";
				rsc_ = ResponseStatusCode::INTERNAL_SERVER_ERROR;
			}
		} else if (reqp.getOperation() == Operation::UPDATE) {
			RequestUpdateHandler<CSEBase> ruh_(reqp, rdb_);
			rsc_ = ruh_.setResourceToBeUpdated();
			if (rsc_ == ResponseStatusCode::OK) {
				if (!ruh_.outToResourceStore()) {
					rsc_ = ResponseStatusCode::INTERNAL_SERVER_ERROR;
				} else  if (!ruh_.composeContent(pc_)) {
					rsc_ = ResponseStatusCode::INTERNAL_SERVER_ERROR;
				} else {
					rsc_ = ResponseStatusCode::CHANGED;
				}
			}
		}
		break;
	}
	case Operation::DDELETE:
		break;
	case Operation::NOTIFY:
		break;
	default:
		rsc_ = ResponseStatusCode::BAD_REQUEST;
		break;
	}

	string rsp_pc_;
	if (rsc_ == ResponseStatusCode::OK ||
		rsc_ == ResponseStatusCode::CREATED ||
		rsc_ == ResponseStatusCode::CHANGED) {
		rsp_pc_ = pc_;
	}
	sendResponse(reqp, rsc_, rdb_.getRoot()->getDomain() + rdb_.getRoot()->getCSEId(), rsp_pc_);
}

void CSEHandler::handleResponse(ResponsePrim& rspp) {
	RequestPrim reqp_;
	string rsp_pc_;
	RequestCache::PostRequestFunc f;
	if (!reqc_.getRequest(rspp.getRequestId(), reqp_, rsp_pc_, f)) {
		cerr << "CSEHandler::handleResponse: Request Id:" << rspp.getRequestId()
				<< " Not found.\n";
		return;
	}

	switch (reqp_.getOperation()) {
	case Operation::CREATE:
		break;
	case Operation::UPDATE:
	case Operation::RETRIEVE:
	case Operation::DDELETE:
	case Operation::NOTIFY:
		break;
	default:
		cerr << "CSEHandler::handleResponse: Invalid Operation: " <<
			(int)reqp_.getOperation() << endl;
		return;
	}

	if (f != NULL) {
		f(reqp_, rsp_pc_, rspp);
	}
}

bool CSEHandler::initiateAnnc(const RequestPrim& reqp, const ResourceBase& res,
		ResponseStatusCode& rsc) {
	vector<string> aa_, at_;
	AnncAttr oa_;
	string s_ = res.getAnncAttr();
	boost::trim(s_);
	if (!s_.empty()) {
		boost::split(aa_, s_, boost::is_any_of(","));
		oa_.insert(aa_.begin(), aa_.end());
	}
	s_ = res.getAnncTo();
	boost::trim(s_);
	boost::split(at_, s_, boost::is_any_of(","));

	if (at_.size() == 0) {
		cerr << "CSEHandler::initiateAnnc: Invalid anncTo: " <<
				res.getAnncTo() << endl;
		rsc = ResponseStatusCode::BAD_REQUEST;
		return false;
	}

	auto found_ = annc_create_.find(res.getResourceType());
	if (found_ == annc_create_.end()) {
		cerr << "CSEHandler::initiateAnnc: Invalid annc resource type: " <<
				(int)res.getResourceType() << endl;
		rsc = ResponseStatusCode::BAD_REQUEST;
		return false;
	}

	try {
		found_->second(res, at_, oa_, reqp.getRequestId());
	} catch (exception &e) {
		cerr << "initiateAnnc: exception:" << e.what() << endl;
		return false;
	}
	return true;
}

void CSEHandler::postAnnc(const RequestPrim& reqp, const string& rqi, const ResponsePrim& rspp) {
	RequestPrim ori_reqp_;
	string ori_pc_;
	if (!reqc_.getRequest(rqi, ori_reqp_, ori_pc_)) {
		cerr << "CSEHandler::postAnnc: can not get original request [" << rqi << "]\n";
		return;
	}

	string fr_ = rdb_.getRoot()->getDomain() + rdb_.getRoot()->getCSEId();
	if (rspp.getResponseStatusCode() == ResponseStatusCode::CREATED) {
		string& rsp_pc_ = ori_pc_;
		ResponseStatusCode rsp_rsc_;
		switch (reqp.getOperation()) {
		case Operation::CREATE:
			rsp_rsc_ = ResponseStatusCode::CREATED;
			break;
		case Operation::UPDATE:
			rsp_rsc_ = ResponseStatusCode::OK;
			// return at attributes

			break;
		default:
			cerr << "CSEHandler::postAnnc: Invalid operation: " << (int)reqp.getOperation();
			return;
		}
		sendResponse(ori_reqp_, rsp_rsc_, fr_, rsp_pc_);
	}
}

}	// OneM2M

}	// MicroWireless
