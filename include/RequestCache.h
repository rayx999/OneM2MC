/*
 * RequestCache.h
 *
 *  Created on: 2015年9月23日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REQUESTCACHE_H_
#define INCLUDE_REQUESTCACHE_H_

#include <iostream>
#include <string>
#include <map>
#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/function.hpp>

#include "RequestPrim.h"
#include "MapCache.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class ResponsePrim;

class RequestCache {
public:
	using PostRequestFunc = boost::function<void (const RequestPrim&,
			const std::string&, const ResponsePrim&)>;

	static RequestCache& getCache();

	bool addRequest(const RequestPrim&);
	bool addRequest(const RequestPrim&, const std::string&);
	bool addRequest(const RequestPrim&, PostRequestFunc);
	bool addRequest(const RequestPrim&, const std::string&, PostRequestFunc);

	bool getRequest(const std::string&, RequestPrim&);
	bool getRequest(const std::string&, RequestPrim&, std::string&);
	bool getRequest(const std::string&, RequestPrim&, PostRequestFunc&);
	bool getRequest(const std::string&, RequestPrim&, std::string&, PostRequestFunc&);

	bool isRequestIdInUse(const std::string&);

private:
	struct CacheItem {
		RequestPrim* p_reqp_;
		std::string pc_;
		PostRequestFunc f_;
	};

	RequestCache() : req_c_(MapCache<CacheItem>::getCache()) {}

	RequestCache(const RequestCache&) = delete;
	RequestCache& operator=(const RequestCache&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;

    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

	MapCache<CacheItem>& req_c_;
};


}	// OneM2M

}	// MicroWireless

#endif /* INCLUDE_REQUESTCACHE_H_ */
