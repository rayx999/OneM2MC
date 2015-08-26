/*
 * ResourceBase.cc
 *
 *  Created on: 2015年8月5日
 *      Author: weimi_000
 */


#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string/predicate.hpp>
#include <json2pb.h>

#include "CommonTypes.pb.h"
#include "ResourceBase.pb.h"
#include "CommonUtils.h"
#include "ResourceBase.h"
#include "RequestPrim.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

//class pb::AE;
//class pb::CSEBase;
//class pb::Request;

const int ResourceBase::ResourceBaseOffset = 30000;

ResourceBase::ResourceBase() : base_() {
	if (!setCreateTimestamp()) {
		throw runtime_error("setCreateTimestamp failed.");
	}
}

ResourceBase::ResourceBase(const string& json, const string& id_str) : base_() {
	ResourceBase();
	if (!setResourceBase(json, id_str)) {
		throw runtime_error("setResourceBase(Json) failed.");
	}
}

bool ResourceBase::setResourceBase(const string &str, const string& id_str) {
	// keep original create time
	TimeStamp _ct;
	getCreateTimestamp(_ct);

	// parse to PB buffer, try pb in string first
	if (!base_.ParseFromString(str)) {
		try {
			json2pb(base_, str.c_str(), str.length());
		} catch (exception &e) {
			cerr << "try json2pb failed." << e.what() << endl;
			return false;
		}
	}
	// need to keep original create time if Json file
	// doesn't have create time set up
	if (!base_.has_ct()) {
		setCreateTimestamp(&_ct);
	}

	return checkResourceConsistency(id_str);
}

pb::CSEBase* ResourceBase::getCSEBase() {
	return base_.mutable_csb();
}

pb::AE* ResourceBase::getAE() {
	return base_.mutable_ae();
}

pb::Request* ResourceBase::getRequest() {
	return base_.mutable_req();
}

const string& ResourceBase::getDomain() {
	return domain_;
}

const string& ResourceBase::getIntCsi() {
	return csi_;
}

const string& ResourceBase::getIntRi() {
	return ri_;
}

SupportedResourceType ResourceBase::getResourceType() {
	return static_cast<SupportedResourceType>(base_.ty());
}

bool ResourceBase::setResourceType(SupportedResourceType ty) {
	base_.set_ty(static_cast<pb::CommonTypes_SupportedResourceType>(ty));
	return setLastModifiedTimestamp();
}

const string& ResourceBase::getResourceId() {
	return base_.ri();
}

bool ResourceBase::setResourceId(const string& ri) {
	base_.set_ri(ri);
	ri_ = ri;
	return setLastModifiedTimestamp();
}

const string& ResourceBase::getResourceName() {
	return base_.rn();
}

bool ResourceBase::setResourceName(const string& rn) {
	base_.set_rn(rn);
	return setLastModifiedTimestamp();
}

const string& ResourceBase::getParentId() {
	return base_.pi();
}

bool ResourceBase::setParentId(const string& pi) {
	base_.set_pi(pi);
	return setLastModifiedTimestamp();
}

bool ResourceBase::getCreateTimestamp(TimeStamp &ct) {
	if (base_.has_ct()) {
		const google::protobuf::Timestamp &_ct =
				base_.ct();
		ct.tv_sec = _ct.seconds();
		ct.tv_usec = _ct.nanos() / 1000;
		return true;
	}
	return false;
}

bool ResourceBase::getLastModifiedTimestamp(TimeStamp &lt) {
	if (base_.has_lt()) {
		const google::protobuf::Timestamp &_lt =
				base_.lt();
		lt.tv_sec = _lt.seconds();
		lt.tv_usec = _lt.nanos() / 1000;
		return true;
	}
	return false;
}

bool ResourceBase::setCreateTimestamp(TimeStamp* p_ts) {
	TimeStamp ts_;
	if (p_ts == NULL) {
		gettimeofday(&ts_, NULL);
	} else {
		ts_ = *p_ts;
	}
	google::protobuf::Timestamp * ct_ = base_.mutable_ct();

	if (ct_ != NULL) {
		ct_->set_seconds(ts_.tv_sec);
		ct_->set_nanos(ts_.tv_usec * 1000);
		return true;
	} else {
		cerr << "Can't get pb::Timestamp." << endl;
		return false;
	}
}

bool ResourceBase::setLastModifiedTimestamp(TimeStamp* p_ts) {
	TimeStamp ts_;
	if (p_ts == NULL) {
		gettimeofday(&ts_, NULL);
	} else {
		ts_ = *p_ts;
	}
	google::protobuf::Timestamp * lt_ = base_.mutable_lt();
	if (lt_ != NULL) {
		lt_->set_seconds(ts_.tv_sec);
		lt_->set_nanos(ts_.tv_usec * 1000);
		return true;
	} else {
		cerr << "Can't g pb::Timestamp." << endl;
		return false;
	}
}

SupportedResourceType ResourceBase::getResourceCase() {
	return static_cast<SupportedResourceType>(base_.resource_case() - ResourceBaseOffset);
}

bool ResourceBase::checkResourceConsistency(const string& id_str) {
	if (getResourceType() != getResourceCase())	{
		cerr << "checkResourceConsistency failed. ";
		cerr << " ResourceType: " << getResourceType();
		cerr << " ResourceCase: " << getResourceCase() << endl;
		return false;
	}
	if (!parseIds(id_str, CSERegex, domain_, csi_, ri_)) {
		cerr << "checkResourceConsistency: parseIds failed. res_path: " << id_str << endl;
		return false;
	}

	if (!(boost::iequals(getResourceId(), ri_) ||
		  boost::iequals(getResourceName(), ri_) ||
		  (getResourceId().empty() && getResourceName().empty()))) {
		cerr << "checkResourceConsistency failed. ";
		cerr << " ri/rn in resource: " << getResourceId();
		cerr << "  " << getResourceName();
		cerr << " vs. ri in store:" << ri_ << endl;
		return false;
	}

	if (!getResourceId().empty()) {
		ri_ = getResourceId();
	}
	return true;
}

bool ResourceBase::SerializeToString(string* pc) {
	return base_.SerializeToString(pc);
}

void ResourceBase::CopyResourceTimeStamps(ResourceBase& src) {
	*base_.mutable_ct() = src.base_.ct();
	*base_.mutable_lt() = src.base_.lt();
}

string ResourceBase::getJson() {
	return pb2json(base_);
}

}	// OneM2M

}	// MicroWireless
