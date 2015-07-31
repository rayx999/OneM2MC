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
	bool isResourceValid(const string& rn);
	Root* getRoot() { return p_root_; };

protected:
	const char * rdb_fn_;
	Root * p_root_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_RESOURCESTORE_H_ */
