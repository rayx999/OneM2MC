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
#include "RequestPrim.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

using google::protobuf::Reflection;
using google::protobuf::FieldDescriptor;

class ResourceBase {
public:
	static const int ResourceBaseOffset;

	ResourceBase();
	ResourceBase(const string& json, const string& id_str);
	virtual ~ResourceBase();

	template <typename StoreType>
	ResourceBase(const string& ri, ResourceStore<StoreType>& rdb) : base_() {
		ResourceBase();
		if (!setResourceBase(ri, rdb)) {
			throw runtime_error("setResourceBase failed.");
		}
	}

	// set from json
	bool setResourceBase(const string &json, const string& id_str);
	// set from PB::ResourceBase in string
	virtual bool setResourceBase(const string &pc, const string& id_str, Operation op);
	bool setNewResourceBaseAttr(const string& ri, const string& rn,	const string& pi, ResourceBase& ret);

	template <typename Root>
	bool setResourceBase(RequestPrim& reqp, Root& root) {
		const string& pc_str_ = reqp.getContent();
		if (pc_str_.empty()) {
			cerr << "setResourceBase: No content.\n";
			return false;
		}
		if (!base_.ParseFromString(pc_str_)) {
			cerr << "setResourceBase: ParseFromString failed.\n";
			return false;
		}
		string res_path = root.getDomain() + root.getCSEId();
		if (!reqp.getIntRn().empty()) {
			res_path += "/" + reqp.getIntRn();
		} else if (!reqp.getName().empty()) {
			res_path += "/" + reqp.getName();
		} else if (!getResourceName().empty()) {
			res_path += "/" + getResourceName();
		}
		return checkResourceConsistency(res_path);
	}

	template <typename StoreType>
	bool setResourceBase(const string& ri, ResourceStore<StoreType>& rdb) {
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
	bool outToResourceStore(ResourceStore<StoreType>& rdb, bool nolink = false) {
		if (!isMatching(ri_, "[A-Z]?\\d{3}-\\d{5}") && !isMatching(ri_, "[A-Z]\\d{4}")) {
			cerr << "Invalid ri format:" << ri_ << endl;
			return false;
		}
		string res_str;
		if (base_.SerializeToString(&res_str)) {
			const string res_path = rdb.getRoot()->getDomain()
					+ rdb.getRoot()->getCSEId()
					+ "/" + getResourceName();

			if (nolink) {
				return rdb.putResource(res_path, string(), res_str);
			} else {
				return rdb.putResource(res_path, ri_, res_str);
			}
		} else {
			cerr << "Serialization failed.\n";
			return false;
		}
	}

	bool SerializeToString(string* pc);
	void CopyResourceTimeStamps(ResourceBase& src);
	string getJson();

private:
	bool checkResourceAttributes(Operation op);

protected:
	template <typename StoreType>
	friend class RequestCreateHandler;

	bool setCreateTimestamp(TimeStamp* p_ts = NULL);
	bool setLastModifiedTimestamp(TimeStamp* p_ts = NULL);

	SupportedResourceType getResourceCase();
	bool checkResourceConsistency(const string& id_str);

	template <typename Resource, typename Map>
	bool checkResourceAttributes(Operation op, Resource& res, Map& m) {
		bool ret_ = true;
		const Reflection* reflection = res.GetReflection();
		vector<const FieldDescriptor*> fields;
		reflection->ListFields(res, &fields);
		for (unsigned int i = 0; i < fields.size(); i++) {
			if (m[fields[i]->number()][op] == NOTPRESENT) {
				cerr << "checkResourceAttributes: Tag " << fields[i]->number();
				cerr << " field shouldn't present.\n";
				ret_ = false;
			}
		}

		// check mandatory attributes
		for (auto i = m.begin(); i != m.end(); ++i) {
			if (i->second[op] != MANDATORY) continue;
			auto found = find_if(fields.begin(), fields.end(), [&i](const FieldDescriptor* f) {
				return f->number() == i->first;
			});
			if (found == fields.end()){
				cerr << "checkResourceAttributes: Tag " << i->first;
				cerr << " field NOT present.\n";
				ret_ = false;
			}
		}
		return ret_;
	}


protected:
	pb::ResourceBase base_;
	string domain_, csi_, ri_;

	enum attrOption {
		MANDATORY  = 1,
		OPTIONAL   = 2,
		NOTPRESENT = 3
	};

private:
	enum commonAttrTag {
		TAG_TY  = 1,
		TAG_RI  = 2,
		TAG_RN  = 3,
		TAG_PI  = 4,
		TAG_CT  = 5,
		TAG_LT  = 6,
		TAG_ET  = 7,
		TAG_ACPI = 8,
		TAG_LBL = 9,
		TAG_AA  = 10,
		TAG_AT  = 11,
		TAG_ST  = 12
	};
	static map<int, map<Operation, attrOption>> allowAttr;
};

}	// OneM2M

}	// MicroWireless


#endif /* INCLUDE_RESOURCEBASE_H_ */
