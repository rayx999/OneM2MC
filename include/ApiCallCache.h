/*
 * ApiCallCache.h
 *
 *  Created on: 2015年10月26日
 *      Author: weimi_000
 */

#ifndef INCLUDE_APICALLCACHE_H_
#define INCLUDE_APICALLCACHE_H_

#include "MapCache.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class IApiCallBack;

class ApiCallCache {
	MapCache<IApiCallBack*>& cache_;

	ApiCallCache() : cache_(MapCache<IApiCallBack*>::getCache()) {}

    ApiCallCache(const ApiCallCache&) = delete;
    ApiCallCache& operator=(const ApiCallCache&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;

    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

public:
	static void getApiCallId(std::string& callid);
	static ApiCallCache& getCache();

	bool insert(const std::string& callid, IApiCallBack*& cb);
	bool get(const std::string& callid, IApiCallBack*& cb);
	bool remove(const std::string& callid);
};

}	// OneM2M
}	// MicroWireless


#endif /* INCLUDE_APICALLCACHE_H_ */
