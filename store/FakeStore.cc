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
#include <boost/algorithm/string.hpp>


#include "CommonUtils.h"
#include "ResourceBase.h"
#include "CSEBase.h"
#include "ResourceStore.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace boost::filesystem;
using namespace boost::algorithm;
using namespace MicroWireless::OneM2M;

template <typename Root>
const string ResourceStore<Root>::RootStr("root");

template <typename Root>
const string ResourceStore<Root>::normalizeRi(const string& ri, const string& ext, int level) {
	const string norm_regex = string("^") + rdb_fn_ + string(".+\\.") + ext + string("$");

	if (isMatching(ri, norm_regex)) {
		return ri;
	}

	string ri_ = ri;
	while (level--) {
		path res_path(rdb_fn_);
		if (ri_.c_str()[0] != '/') {
			res_path += "/" + ri_;
		} else if (ri_.c_str()[1] == '/') {
			res_path += ri_.substr(1);
		} else {
			res_path += ri_;
		}
		path lnk_path(res_path);
		lnk_path += ".lnk";
		if (exists(lnk_path) && is_regular_file(lnk_path)) {
			ifstream ifs(lnk_path.string());
			stringstream sstr;
			sstr << ifs.rdbuf();
			ri_ = sstr.str();
			trim(ri_);
			if (level) continue;
			return ri_;
		}
		return res_path.string() + "." + ext;
	}
}

template <typename Root>
bool ResourceStore<Root>::setupRoot() {
	string ri_ = normalizeRi(RootStr);
	if (!exists(ri_)) {
	  cout << "Root: " << ri_ << " doesn't exist." << endl;
	  return false;
	}
	// initialize root element resource
	try {
	  p_root_ = new Root(RootStr, *this);
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
bool ResourceStore<Root>::putResource(const string& ri, const string& lnk, const string& res_str) {
	const string norm_ri_(normalizeRi(ri));
	path norm_dir_(norm_ri_);
	norm_dir_ = norm_dir_.parent_path();
	if (!exists(norm_dir_) || !is_directory(norm_dir_)) {
		if (!create_directories(norm_dir_)) {
			cerr << "putResource: create_dir failed. path: ";
			cerr << norm_dir_.parent_path() << endl;
			return false;
		}
	}
	// create resource
	fstream ous(norm_ri_, ios::out | ios::trunc | ios::binary);
	ous << res_str;
	// create link resource
	if (!lnk.empty()) {
		fstream oul(normalizeRi(lnk, "lnk"), ios::out | ios::trunc | ios::binary);
		oul << ri;
	}
	return true;
}

template <typename Root>
const string ResourceStore<Root>::getResourcePath(const string& ri) {
	string norm_ri_(normalizeRi(ri));
	int rdb_fn_len_ = strlen(rdb_fn_);
	int start_ = rdb_fn_len_ + 1;
	int len_ = norm_ri_.length() - start_ - strlen(".res");
	norm_ri_ = "//" + norm_ri_.substr(start_, len_);
	return norm_ri_;
}

// get parent resource, default to CSEBase
template <typename Root>
bool ResourceStore<Root>::getParentResource(const string& path, ResourceBase& parent) {
	size_t pos = path.find_last_of("/\\");
	string parent_path_ = path.substr(0, pos);
	string sp_id_ = p_root_->getDomain() + p_root_->getCSEId();
	if (boost::iequals(path, sp_id_) || boost::iequals(parent_path_, sp_id_)) {
		// parent is root
		parent = *p_root_;
		return true;
	} else if (isResourceValid(parent_path_)) {
		string parent_str_;
		if (getResource(parent_path_, parent_str_) &&
			parent.setResourceBase(parent_str_, sp_id_)) {
			return true;
		}
	}
	return false;
}

// ghost func to get around template implementation in cc file problem.
void TemporaryFunction ()
{
	string tmp;
    ResourceStore<CSEBase> TempObj("");
    ResourceBase res;
    TempObj.setupRoot();
    TempObj.isResourceValid("");
    TempObj.getResource("", tmp);
    TempObj.putResource("", "", "");
    TempObj.getResourcePath("");
    TempObj.getParentResource("", res);
}

}	// OneM2M
}	// MicroWireless
