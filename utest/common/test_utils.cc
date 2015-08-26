/*
 * test_utils.cc
 *
 *  Created on: 2015年7月15日
 *      Author: weimi_000
 */

#include <fstream>
#include <iostream>
#include <string>
#include <json2pb.h>

#include "test_utils.h"
#include "ResourceBase.pb.h"

using namespace std;

namespace MicroWireless {

namespace OneM2M {

namespace UTest {

bool CopyFile(const char * srcFn, const char * dstFn) {

	try {
		ifstream src(srcFn, ios::binary);
		ofstream dst(dstFn, ios::binary);

		dst << src.rdbuf();
	} catch (exception &e) {
		cerr << "Copy File error:" << e.what() << endl;
		return false;
	}
    return true;
}

bool Resource2String(const string& json, string& str) {
	pb::ResourceBase res_pc;
	json2pb(res_pc, json.c_str(), json.length());
	return res_pc.SerializeToString(&str);
}

}	// UTest

}	// OneM2M

}	// MicroWireless




