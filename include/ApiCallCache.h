/*
 * ApiCallCache.h
 *
 *  Created on: 2015年10月26日
 *      Author: weimi_000
 */

#ifndef INCLUDE_APICALLCACHE_H_
#define INCLUDE_APICALLCACHE_H_

#include <iostream>
#include <string>
#include <ctime>
#include <locale>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

template<typename ApiCacheType>
class ApiCallCache {
	std::map<const std::string, ApiCacheType> cache_;

	ApiCallCache() { }

    ApiCallCache(const ApiCallCache&) = delete;
    ApiCallCache& operator=(const ApiCallCache&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;

    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

public:
	static ApiCallCache& getApiCallCache() {
		static ApiCallCache cache_;
		return cache_;
	}

	void getApiCallId(std::string& callid) {
		static unsigned int s_ = 0;
	    std::time_t t = std::time(NULL);
	    char mbstr[100];
	    std::strftime(mbstr, sizeof(mbstr), "%y%m%d", std::localtime(&t));
	    callid = mbstr + boost::str(boost::format("%05d") % s_++);
	}

	bool insert(const std::string& id, ApiCacheType& cb) {
		cache_.erase(id);
		return cache_.insert({id, cb}).second;
	}

	bool getCallback(const std::string& id, ApiCacheType& cb) {
		auto f_ = cache_.find(id);
		if (f_ == cache_.end()) {
			return false;
		}
		cb = f_->second;
		return true;
	}

	bool remove(const std::string& id) {
		return cache_.erase(id);
	}

};

}	// OneM2M
}	// MicroWireless


#endif /* INCLUDE_APICALLCACHE_H_ */
