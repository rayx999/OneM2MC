/*
 * ResourceStore.h
 *
 *  Created on: 2015年7月27日
 *      Author: weimi_000
 */

#ifndef INCLUDE_RESOURCESTORE_H_
#define INCLUDE_RESOURCESTORE_H_

#include <iostream>
#include <string>
#include <boost/random/mersenne_twister.hpp>

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;
using namespace std;

class ResourceBase;

template <typename Root>
class ResourceStore {

public:
	static const string RootStr;

	ResourceStore(const char * rdb_fn) :
		rdb_fn_(rdb_fn), p_root_(NULL) {
		gen_.seed(static_cast<long int>(time(0)));
	}

	~ResourceStore() {
		if (p_root_ != NULL) delete p_root_;
		p_root_ = NULL;
	};

	bool setupRoot();
	bool isResourceValid(const string& ri);
	bool getResource(const string& ri, string& res_str);
	bool putResource(const string& ri, const string& lnk, const string& res_str);
	Root* getRoot() { return p_root_; }
	const string getResourcePath(const string& ri);
	bool getParentResource(const string& path, ResourceBase*& p_parent);
	void generateResourceId(SupportedResourceType ty, string& ri);

protected:
	const string normalizeRi(const string& ri, const string& ext = "res", int level = 999);

protected:
	const char * rdb_fn_;
	Root * p_root_;
	boost::random::mt19937 gen_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_RESOURCESTORE_H_ */
