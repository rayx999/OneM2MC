/*
 * FakeStore.cc
 *
 *  Created on: 2015年7月28日
 *      Author: weimi_000
 */

#include <iostream>
#include <boost/filesystem.hpp>

#include "CSEBase.h"
#include "ResourceStore.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace boost::filesystem;
using namespace MicroWireless::OneM2M;

//template ResourceStore<CSEBase>;

template <typename Root>
bool ResourceStore<Root>::setupRoot() {
	path store_path(rdb_fn_);

	if (!exists(store_path) || !is_directory(store_path)) {
	  cerr << "Resource store: " << store_path.string() << " doesn't exist." << endl;
	  return false;
	}

	store_path += "/root";
	if (!exists(store_path)) {
	  cerr << "Root: " << store_path.string() << " doesn't exist." << endl;
	  return false;
	}

	// initialize root element resource
	try {
	  p_root_ = new Root(store_path.string().c_str());
	} catch (exception &e) {
		cerr << e.what() << endl;
		return false;
	}
	return true;
}

template <typename Root>
bool ResourceStore<Root>::isResourceValid(const string& ri) {
	path res_path(rdb_fn_);
	res_path += "/" + ri + ".res";

	return exists(res_path);
}

// ghost func to get around template implementation in cc file problem.
void TemporaryFunction ()
{
    ResourceStore<CSEBase> TempObj("");
    TempObj.setupRoot();
    TempObj.isResourceValid("");
}

}	// OneM2M
}	// MicroWireless
