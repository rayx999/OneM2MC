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
#include "CommonTypes.h"
#include "ResourceStore.h"
#include "RequestPrim.h"

namespace MicroWireless {

namespace OneM2M {

using namespace MicroWireless::OneM2M;

class ResponsePrim;

using google::protobuf::Reflection;
using google::protobuf::FieldDescriptor;

class ResourceBase {
public:
	static const int ResourceBaseOffset;

	enum attrOption {
		MANDATORY  = 1,
		OOPTIONAL  = 2,		// prefix 'O' to avoid wired conflict using boost
		NOTPRESENT = 3
	};

	ResourceBase();
	ResourceBase(const std::string& json, const std::string& id_str);
	virtual ~ResourceBase();

	template <typename StoreType>
	ResourceBase(const std::string& ri, ResourceStore<StoreType>& rdb) : base_() {
		ResourceBase();
		if (!setResourceBase(ri, rdb)) {
			throw runtime_error("setResourceBase failed.");
		}
	}

	// set from json
	bool setResourceBase(const std::string &json, const std::string& id_str);
	// set from PB::ResourceBase in std::string
	bool setResourceBase(const std::string&, const std::string&, Operation);
	bool setResourceBase(const ResponsePrim&);

	bool setNewAttr(const std::string& ri, const std::string& rn, const std::string& pi);
	bool setNewAttr(const std::string& ri, const std::string& rn, const std::string& pi, ResourceBase* p_ret);
	bool updateResource(const ResourceBase&);

	template <typename Root>
	bool setResourceBase(RequestPrim& reqp, Root& root) {
		const std::string& pc_str_ = reqp.getContent();
		if (pc_str_.empty()) {
			cerr << "setResourceBase: No content.\n";
			return false;
		}
		if (!base_.ParseFromString(pc_str_)) {
			cerr << "setResourceBase: ParseFromString failed.\n";
			return false;
		}
		std::string res_path = root.getDomain() + root.getCSEId();
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
	bool setResourceBase(const std::string& ri, ResourceStore<StoreType>& rdb) {
		try {
			std::string res_str_;
			if (rdb.getResource(ri, res_str_) && base_.ParseFromString(res_str_)) {
				return checkResourceConsistency(rdb.getResourcePath(ri));
			}
		} catch (exception &e) {
			cerr << "setResourceBase exception: " << e.what() << endl;
		}
		return false;
	}

	const pb::ResourceBase& getResourceBase() const;

	template <typename PbElementType>
	const PbElementType& getElement() const;

	pb::CSEBase* getCSEBase();
	pb::AE* getAE();
	pb::AE* getAEAnnc();
	pb::RemoteCSE* getRemoteCSE();
	pb::Request* getRequest();

	const std::string& getDomain() const;
	// get CSE id parsed from ResourceId
	const std::string& getIntCsi() const;
	// get resource id parsed internally
	const std::string& getIntRi() const;

	SupportedResourceType getResourceType() const;
	bool setResourceType(SupportedResourceType ty);

	const std::string& getResourceId() const;
	bool setResourceId(const std::string& ri);

	const std::string& getResourceName() const;
	bool setResourceName(const std::string& rn);

	int getAccessControlPolicyNum();
	const std::string& getAccessControlPolicy(int i) const;
	bool setAccessControlPolicy(const std::string& acpi);

	const std::string& getParentId() const;
	bool setParentId(const std::string& pi);

	bool getCreateTimestamp(TimeStamp&);
	bool getLastModifiedTimestamp(TimeStamp&);

	bool getExpirationTimestamp(TimeStamp&);
	bool setExpirationTimestamp(TimeStamp*);

	const std::string& getAnncTo() const;
	bool setAnncTo(const std::string&);

	const std::string& getAnncAttr() const;
	bool setAnncAttr(const std::string&);

	template <typename StoreType>
	bool outToResourceStore(ResourceStore<StoreType>& rdb, bool nolink = false) {
		if (!isMatching(ri_, "[A-Z]?\\d{5}-\\d{5}") && !isMatching(ri_, "[A-Z]\\d{4}")) {
			cerr << "outToResourceStore:Invalid ri format:" << ri_ << endl;
			return false;
		}
		std::string res_str;
		if (base_.SerializeToString(&res_str)) {
			const std::string res_path = rdb.getRoot()->getDomain()
					+ rdb.getRoot()->getCSEId()
					+ "/" + getResourceName();

			if (nolink) {
				return rdb.putResource(res_path, std::string(), res_str);
			} else {
				return rdb.putResource(res_path, ri_, res_str);
			}
		} else {
			cerr << "outToResourceStore: Serialization failed.\n";
			return false;
		}
	}

	void clrNotPresentAttrs();
	bool serializeToString(std::string* pc);
	void copyAnncFields(const ResourceBase&, AnncAttr&, AnncAttr&);
	bool compare(pb::ResourceBase&, bool noct = false, bool nolt = false);
	void copyTimeStamps(ResourceBase&);
	std::string getJson();

private:
	bool checkResourceAttributes(Operation op);
	bool filterAnncAttr(google::protobuf::Message*,	std::map<const std::string, attrOption>);

	template <bool (pb::ResourceBase::*has)() const,
			const google::protobuf::Timestamp& (pb::ResourceBase::*gt)() const>
	bool getTimestamp(TimeStamp &ts) {
		if ((base_.*has)()) {
			const google::protobuf::Timestamp &ts_ = (base_.*gt)();
			ts.tv_sec = ts_.seconds();
			ts.tv_usec = ts_.nanos() / 1000;
			return true;
		}
		return false;
	}

	template <google::protobuf::Timestamp* (pb::ResourceBase::*mut)()>
	bool setTimestamp(TimeStamp* p_ts) {
		TimeStamp ts_;
		if (p_ts == NULL) {
			gettimeofday(&ts_, NULL);
		} else {
			ts_ = *p_ts;
		}
		google::protobuf::Timestamp * pb_ts_ = (base_.*mut)();
		if (pb_ts_ != NULL) {
			pb_ts_->set_seconds(ts_.tv_sec);
			pb_ts_->set_nanos(ts_.tv_usec * 1000);
			return true;
		} else {
			cerr << "Can't g pb::Timestamp." << endl;
			return false;
		}
	}

protected:
	template <typename StoreType>
	friend class RequestCreateHandler;

	bool setCreateTimestamp(TimeStamp* p_ts = NULL);
	bool setLastModifiedTimestamp(TimeStamp* p_ts = NULL);

	SupportedResourceType getResourceCase();
	bool checkResourceConsistency(const std::string& id_str);

	template <typename Resource, typename Map>
	bool checkResourceAttributes(Operation op, Resource& res, Map& m) {
		bool ret_ = true;
		const Reflection* reflection = res.GetReflection();
		vector<const FieldDescriptor*> fields;
		reflection->ListFields(res, &fields);
		for (unsigned int i = 0; i < fields.size(); i++) {
			auto f_ = m.find(fields[i]->name());
			if (f_ != m.end()) {
				auto f_op_ = f_->second.find(op);
				if (f_op_ != f_->second.end() && f_op_->second == NOTPRESENT) {
					cerr << "checkResourceAttributes: Attribute [" << fields[i]->name();
					cerr << "] field shouldn't present.\n";
					ret_ = false;
				}
			}
		}

		// check mandatory attributes
		for (auto i = m.begin(); i != m.end(); ++i) {
			auto f_ = i->second.find(op);
			if (f_ == i->second.end()) continue;
			if (f_->second != MANDATORY) continue;
			auto found = find_if(fields.begin(), fields.end(), [&i](const FieldDescriptor* f) {
				return f->name() == i->first;
			});
			if (found == fields.end()){
				cerr << "checkResourceAttributes: Attribute [" << i->first;
				cerr << "] NOT present.\n";
				ret_ = false;
			}
		}
		return ret_;
	}

	template <typename ResourceType, ResourceType* (ResourceBase::*getAnnc)()>
	bool filterAnncAttr(std::map<const std::string, attrOption>& attr) {
		return filterAnncAttr((this->*getAnnc)(), attr)
			&& filterAnncAttr(&base_, anncAttr);
	}

protected:
	pb::ResourceBase base_;
	std::string domain_, csi_, ri_;

private:
	static map<const std::string, map<Operation, attrOption>> allowAttr;
	static map<const std::string, attrOption> anncAttr;
};

}	// OneM2M

}	// MicroWireless


#endif /* INCLUDE_RESOURCEBASE_H_ */
