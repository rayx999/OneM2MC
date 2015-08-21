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
//#include "CSEBase.pb.h"
#include "CommonTypes.h"
#include "ResourceStore.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;

//class pb::CSEBase;
//class pb::AE;
//class pb::Request;

class ResourceBase {
public:
	static const int ResourceBaseOffset;

	ResourceBase();
	ResourceBase(const string& json, const string& id_str);

	template <typename StoreType>
	ResourceBase(const string& ri, StoreType& rdb) : base_() {
		ResourceBase();
		if (!setResourceBase(ri, rdb)) {
			throw runtime_error("setResourceBase failed.");
		}
	}

	bool setResourceBase(const string &json, const string& id_str);

	template <typename StoreType>
	bool setResourceBase(const string& ri, StoreType& rdb) {
		try {
			string res_str_;
			if (rdb.getResource(ri, res_str_) && base_.ParseFromString(res_str_)) {
				return checkResourceConsistency(rdb.getResourcePath(ri));
			}
		} catch (exception &e) {
			cerr << "setResourceBase exception: " << e.what() << endl;
		}
		return false;
	}

	pb::CSEBase* getCSEBase();
	pb::AE* getAE();
	pb::Request* getRequest();

	const string& getDomain();
	// get CSE id parsed from ResourceId
	const string& getIntCsi();
	// get resource id parsed internally
	const string& getIntRi();

	SupportedResourceType getResourceType();
	bool setResourceType(SupportedResourceType ty);

	const string& getResourceId();
	bool setResourceId(const string& ri);

	const string& getResourceName();
	bool setResourceName(const string& rn);

	const string& getParentId();
	bool setParentId(const string& pi);

	bool getCreateTimestamp(TimeStamp &create_time);
	bool getLastModifiedTimestamp(TimeStamp &create_time);

	template <typename StoreType>
	bool outToResourceStore(StoreType& rdb) {
		string res_str;
		if (base_.SerializeToString(&res_str)) {
			const string res_path = rdb.getRoot()->getDomain()
					+ rdb.getRoot()->getCSEId()
					+ "/" + getResourceName();

			return rdb.putResource(res_path, ri_, res_str);
		} else {
			cerr << "Serialization failed.\n";
			return false;
		}
	}

	bool SerializeToString(string* pc);
	string getJson();

private:

protected:
	bool setCreateTimestamp();
	bool setLastModifiedTimestamp();
	SupportedResourceType getResourceCase();
	bool checkResourceConsistency(const string& id_str);

protected:
	pb::ResourceBase base_;
	string domain_, csi_, ri_;
};

}	// OneM2M

}	// MicroWireless


#endif /* INCLUDE_RESOURCEBASE_H_ */
