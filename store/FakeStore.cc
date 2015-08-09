/*
 * FakeStore.cc
 *
 *  Created on: 2015年7月28日
 *      Author: weimi_000
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
	  cout << "Resource store: " << store_path.string() << " doesn't exist." << endl;
	  return false;
	}

	store_path += "/root";
	if (!exists(store_path)) {
	  cout << "Root: " << store_path.string() << " doesn't exist." << endl;
	  return false;
	}

	// initialize root element resource
	try {
	  p_root_ = new Root("root", *this);
	} catch (exception &e) {
		cout << e.what() << endl;
		return false;
	}
	return true;
}

template <typename Root>
bool ResourceStore<Root>::isResourceValid(const string& ri) {
	const string norm_ri = normalizeRi(ri);
	return exists(norm_ri) && is_regular_file(norm_ri);
}

template <typename Root>
bool ResourceStore<Root>::getResource(const string& ri, string& res_str) {
    fstream ins(normalizeRi(ri), ios::in | ios::binary);

    if (!ins) {
        cerr << "Can't open resource file " << normalizeRi(ri) << endl;
        return false;
    }

	stringstream buf;
	buf << ins.rdbuf();
	string b_str = buf.str();
	swap(res_str, b_str);

	return true;
}

template <typename Root>
bool ResourceStore<Root>::putResource(const string& ri, const string& res_str) {
	const string norm_ri_(normalizeRi(ri));

	fstream ous(norm_ri_, ios::out | ios::trunc | ios::binary);
	ous << res_str;
	return true;
}

template <typename Root>
const string ResourceStore<Root>::normalizeRi(const string& ri) {
	path res_path(rdb_fn_);

	if (ri.c_str()[0] != '/') {
		res_path += "/" + ri;
	} else if (ri.c_str()[1] == '/') {
		res_path += ri.substr(1);
	} else {
		res_path += ri;
	}

	return res_path.string();
}

// ghost func to get around template implementation in cc file problem.
void TemporaryFunction ()
{
	string tmp;
    ResourceStore<CSEBase> TempObj("");
    TempObj.setupRoot();
    TempObj.isResourceValid("");
    TempObj.getResource("", tmp);
    TempObj.putResource("", "");
}

}	// OneM2M
}	// MicroWireless
