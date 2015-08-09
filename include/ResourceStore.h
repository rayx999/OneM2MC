/*
 * ResourceStore.h
 *
 *  Created on: 2015年7月27日
 *      Author: weimi_000
 */

#ifndef INCLUDE_RESOURCESTORE_H_
#define INCLUDE_RESOURCESTORE_H_

#include <iostream>


namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;
using namespace std;

template <typename Root>
class ResourceStore {

public:
	ResourceStore(const char * rdb_fn) :
		rdb_fn_(rdb_fn), p_root_(NULL) {};
	~ResourceStore() {
		if (p_root_ != NULL) delete p_root_;
		p_root_ = NULL;
	};

	bool setupRoot();
	bool isResourceValid(const string& ri);
	bool getResource(const string& ri, string& res_str);
	bool putResource(const string& ri, const string& res_str);
	Root* getRoot() { return p_root_; }

protected:
	const string normalizeRi(const string& ri);

protected:
	const char * rdb_fn_;
	Root * p_root_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_RESOURCESTORE_H_ */
