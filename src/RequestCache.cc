/*
 * RequestCache.cc
 *
 *  Created on: 2015年9月25日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>

#include "RequestCache.h"
#include "RequestPrim.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

bool RequestCache::addRequest(const RequestPrim& reqp) {
	return addRequest(reqp, string(), NULL);
}

bool RequestCache::addRequest(const RequestPrim& reqp, const std::string& pc) {
	return addRequest(reqp, pc, NULL);
}

bool RequestCache::addRequest(const RequestPrim& reqp, PostRequestFunc f) {
	return addRequest(reqp, string(), f);
}

bool RequestCache::addRequest(const RequestPrim& reqp, const string& rsp_pc, PostRequestFunc f) {
	boost::lock_guard<boost::mutex> lock(m_);
	if (req_c_.find(reqp.getRequestId()) != req_c_.end()) {
		cerr << "RequestCache::addRequest: request id [" <<
				reqp.getRequestId() << "] already exists.\n";
		return false;
	}
	req_c_.insert({reqp.getRequestId(), { new RequestPrim(reqp), rsp_pc, f }});
	return true;
}

bool RequestCache::getRequest(const std::string& rqi, RequestPrim& reqp) {
	boost::lock_guard<boost::mutex> lock(m_);
	if (req_c_.find(rqi) == req_c_.end()) {
		cerr << "RequestCache::getRequest: request id [" <<
				reqp.getRequestId() << "] doesn't exist.\n";
		return false;
	}
	reqp.swap(*req_c_.at(rqi).p_reqp_);

	delete req_c_.at(rqi).p_reqp_;
	req_c_.erase(rqi);
	return true;
}

bool RequestCache::getRequest(const std::string& rqi, RequestPrim& reqp, std::string& pc) {
	boost::lock_guard<boost::mutex> lock(m_);
	if (req_c_.find(rqi) == req_c_.end()) {
		cerr << "RequestCache::getRequest: request id [" <<
				rqi << "] doesn't exist.\n";
		return false;
	}
	reqp.swap(*req_c_.at(rqi).p_reqp_);
	pc = req_c_.at(rqi).pc_;

	req_c_.erase(rqi);
	return true;
}

bool RequestCache::getRequest(const std::string& rqi, RequestPrim& reqp, PostRequestFunc& f) {
	boost::lock_guard<boost::mutex> lock(m_);
	if (req_c_.find(rqi) == req_c_.end()) {
		cerr << "RequestCache::getRequest: request id [" <<
				reqp.getRequestId() << "] doesn't exist.\n";
		return false;
	}
	reqp.swap(*req_c_.at(rqi).p_reqp_);
	f = req_c_.at(rqi).f_;

	req_c_.erase(rqi);
	return true;
}

bool RequestCache::getRequest(const string& rqi, RequestPrim& reqp, string& rsp_pc,
		PostRequestFunc& f) {
	boost::lock_guard<boost::mutex> lock(m_);
	if (req_c_.find(rqi) == req_c_.end()) {
		cerr << "RequestCache::getRequest: request id [" <<
				reqp.getRequestId() << "] doesn't exist.\n";
		return false;
	}
	reqp.swap(*req_c_.at(rqi).p_reqp_);
	rsp_pc = req_c_.at(rqi).pc_;
	f = req_c_.at(rqi).f_;

	req_c_.erase(rqi);
	return true;
}

bool RequestCache::isRequestIdInUse(const std::string& rqi) {
	boost::lock_guard<boost::mutex> lock(m_);
	return req_c_.find(rqi) != req_c_.end();
}

}	// OneM2M

}	// MicroWireless
