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

#include "MapCache.h"
#include "RequestCache.h"
#include "RequestPrim.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

RequestCache& RequestCache::getCache() {
	static RequestCache c_;
	return c_;
}

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
	CacheItem ci_({ new RequestPrim(reqp), rsp_pc, f });
	return req_c_.insert(reqp.getRequestId(), ci_);
}

bool RequestCache::getRequest(const std::string& rqi, RequestPrim& reqp) {
	CacheItem ci_;
	if (req_c_.get(rqi, ci_)) {
		reqp.swap(*ci_.p_reqp_);
		delete ci_.p_reqp_;
		req_c_.remove(rqi);
		return true;
	}
	return false;
}

bool RequestCache::getRequest(const std::string& rqi, RequestPrim& reqp, std::string& pc) {
	CacheItem ci_;
	if (req_c_.get(rqi, ci_)) {
		reqp.swap(*ci_.p_reqp_);
		pc.swap(ci_.pc_);
		delete ci_.p_reqp_;
		req_c_.remove(rqi);
		return true;
	}
	return false;
}

bool RequestCache::getRequest(const std::string& rqi, RequestPrim& reqp, PostRequestFunc& f) {
	CacheItem ci_;
	if (req_c_.get(rqi, ci_)) {
		reqp.swap(*ci_.p_reqp_);
		f = ci_.f_;
		delete ci_.p_reqp_;
		req_c_.remove(rqi);
		return true;
	}
	return false;
}

bool RequestCache::getRequest(const string& rqi, RequestPrim& reqp, string& rsp_pc,
		PostRequestFunc& f) {
	CacheItem ci_;
	if (req_c_.get(rqi, ci_)) {
		reqp.swap(*ci_.p_reqp_);
		rsp_pc.swap(ci_.pc_);
		f = ci_.f_;
		delete ci_.p_reqp_;
		req_c_.remove(rqi);
		return true;
	}
	return false;
}

bool RequestCache::isRequestIdInUse(const std::string& rqi) {
	return req_c_.isInCache(rqi);
}

}	// OneM2M

}	// MicroWireless
