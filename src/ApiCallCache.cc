/*
 * ApiCallCache.cc
 *
 *  Created on: 2015年11月1日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include <ctime>
#include <locale>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include "MapCache.h"
#include "ApiCallCache.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

void ApiCallCache::getApiCallId(std::string& callid) {
	static unsigned int s_ = 0;
    std::time_t t = std::time(NULL);
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), "%y%m%d", std::localtime(&t));
    callid = mbstr + boost::str(boost::format("%05d") % s_++);
}

ApiCallCache& ApiCallCache::getCache() {
	static ApiCallCache c_;
	return c_;
}

bool ApiCallCache::insert(const std::string& callid, IApiCallBack*& cb) {
	return cache_.insert_a(callid, cb);
}

bool ApiCallCache::get(const std::string& callid, IApiCallBack*& cb) {
	return cache_.get(callid, cb);
}

bool ApiCallCache::remove(const std::string& callid) {
	return cache_.remove(callid);
}

}	// OneM2M
}	// MicroWireless
