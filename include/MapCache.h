/*
 * MapCache.h
 *
 *  Created on: 2015年11月1日
 *      Author: weimi_000
 */

#ifndef INCLUDE_MAPCACHE_H_
#define INCLUDE_MAPCACHE_H_

#include <iostream>
#include <string>
#include <map>
#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

template<typename CacheType>
class MapCache {
	boost::mutex m_;
	std::map<const std::string, CacheType> cache_;

	MapCache() { }

    MapCache(const MapCache&) = delete;
    MapCache& operator=(const MapCache&) = delete;

    void* operator new(std::size_t) = delete;
    void* operator new[](std::size_t) = delete;

    void operator delete(void*) = delete;
    void operator delete[](void*) = delete;

public:
	static MapCache& getCache() {
		static MapCache mapCache_;
		return mapCache_;
	}

	// Insert absolute
	bool insert_a(const std::string& id, CacheType& ca) {
		boost::lock_guard<boost::mutex> lock(m_);
		cache_.erase(id);
		return cache_.insert({id, ca}).second;
	}

	bool insert(const std::string& id, CacheType& ca) {
		boost::lock_guard<boost::mutex> lock(m_);
		if (cache_.find(id) != cache_.end()) {
			return false;
		}
		return cache_.insert({id, ca}).second;
	}

	bool get(const std::string& id, CacheType& ca) {
		boost::lock_guard<boost::mutex> lock(m_);
		auto f_ = cache_.find(id);
		if (f_ == cache_.end()) {
			return false;
		}
		ca = f_->second;
		return true;
	}

	bool remove(const std::string& id) {
		boost::lock_guard<boost::mutex> lock(m_);
		return cache_.erase(id);
	}

	bool isInCache(const std::string& id) {
		boost::lock_guard<boost::mutex> lock(m_);
		return cache_.find(id) != cache_.end();
	}
};

}	// OneM2M
}	// MicroWireless



#endif /* INCLUDE_MAPCACHE_H_ */
