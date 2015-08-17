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

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

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

bool ResourceBase::setResourceBase(const string &json, const string& id_str) {
	// keep original create time
	TimeStamp _ct;
	getCreateTimestamp(_ct);

	// parse to PB buffer
	try {
		json2pb(base_, json.c_str(), json.length());
	} catch (exception &e) {
		cerr << "json2pb failed." << e.what() << endl;
		return false;
	}
	// need to keep original create time if Json file
	// doesn't have create time set up
	if (!base_.has_ct()) {
		setCreateTimestamp();
	}

	return checkResourceConsistency(id_str);
}

pb::CSEBase* ResourceBase::getCSEBase() {
	return base_.mutable_csb();
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

const string& ResourceBase::getResourceId() {
	return base_.ri();
}

const string& ResourceBase::getResourceName() {
	return base_.rn();
}

const string& ResourceBase::getParentId() {
	return base_.pi();
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

bool ResourceBase::setParentId(const string& pi) {
	base_.set_pi(pi);
	return setLastModifiedTimestamp();
}

bool ResourceBase::setCreateTimestamp() {
	TimeStamp ts_;
	gettimeofday(&ts_, NULL);
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

bool ResourceBase::setLastModifiedTimestamp() {
	TimeStamp ts_;
	gettimeofday(&ts_, NULL);
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
	if (!boost::iequals(getResourceId(), ri_) && !boost::iequals(getResourceName(), ri_)) {
		cerr << "checkResourceConsistency failed. ";
		cerr << " ri/rn in resource: " << getResourceId();
		cerr << "  " << getResourceName();
		cerr << " vs. ri in store:" << ri_ << endl;
		return false;
	}
	return true;
}

bool ResourceBase::SerializeToString(string* pc) {
	return base_.SerializeToString(pc);
}

string ResourceBase::getJson() {
	return pb2json(base_);
}

const string ResourceBase::composeResourceStorePath() {
	return domain_ + csi_ + "/" + getResourceName();
}

}	// OneM2M

}	// MicroWireless
