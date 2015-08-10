/*
 * ResourceBase.h
 *
 *  Created on: 2015年8月5日
 *      Author: weimi_000
 */

#ifndef INCLUDE_RESOURCEBASE_H_
#define INCLUDE_RESOURCEBASE_H_

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <json2pb.h>

#include "CommonTypes.pb.h"
#include "ResourceBase.pb.h"
#include "CSEBase.pb.h"
#include "CommonTypes.h"
#include "ResourceStore.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;

class ResourceBase {
public:
	ResourceBase() : base_() {}
	SupportedResourceType getResourceBase();
	pb::CSEBase& getCSEBase();

	template <typename StoreType>
	bool setResourceBase(const string& ri, StoreType& rdb) {
		try {
			string res_str_;
			if (rdb.getResource(ri, res_str_) && base_.ParseFromString(res_str_)) {
				return true;
			}
		} catch (exception &e) {
			cerr << "setResourceBase exception: " << e.what() << endl;
		}
		return false;
	}

	template <typename StoreType>
	bool outToResourceStore(const string& res_path, StoreType& rdb) {
		string res_str;
		if (base_.SerializeToString(&res_str)) {
			return rdb.putResource(res_path, res_str);
		} else {
			cerr << "Serialization failed.\n";
			return false;
		}
	}

	string getJson();
	~ResourceBase() { }

protected:
	pb::ResourceBase base_;
};

}	// OneM2M

}	// MicroWireless


#endif /* INCLUDE_RESOURCEBASE_H_ */
