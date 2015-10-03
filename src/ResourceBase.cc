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

const int ResourceBase::ResourceBaseOffset = 30000;

map<const string, map<Operation, ResourceBase::attrOption>>
	ResourceBase::allowAttr = {
   {AttrName::TY(),   { {Operation::CREATE, NOTPRESENT}, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::RI(),   { {Operation::CREATE, NOTPRESENT}, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::RN(),   { {Operation::CREATE, NOTPRESENT}, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::PI(),   { {Operation::CREATE, NOTPRESENT}, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::CT(),   { {Operation::CREATE, NOTPRESENT}, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::LT(),   { {Operation::CREATE, NOTPRESENT}, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::ET(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::ACPI(), { {Operation::CREATE, NOTPRESENT}, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::LBL(),  { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::AA(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::AT(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::ST(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } }
};

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

	try {
		json2pb(base_, json.c_str(), json.length());
	} catch (exception &e) {
		cerr << "try json2pb failed." << e.what() << endl;
		return false;
	}

	// need to keep original create time if Json file
	// doesn't have create time set up
	if (!base_.has_ct()) {
		setCreateTimestamp(&_ct);
	}

	return checkResourceConsistency(id_str);
}

bool ResourceBase::setResourceBase(const string &pc, const string& id_str, Operation op) {
	// keep original create time
	TimeStamp _ct;
	getCreateTimestamp(_ct);

	// parse to PB buffer, try pb in string first
	if (!base_.ParseFromString(pc)) {
		cerr << "setResourceBase: ParseFromString failed.\n";
		return false;
	}

	if (!checkResourceAttributes(op)) {
		cerr << "setResourceBase: checkResourceAttributes failed.\n";
		return false;
	}
	// need to keep original create time if Json file
	// doesn't have create time set up
	if (!base_.has_ct()) {
		setCreateTimestamp(&_ct);
	}

	return checkResourceConsistency(id_str);
}

bool ResourceBase::setNewResourceBaseAttr(const string& ri, const string& rn,
		const string& pi, ResourceBase& ret) {
	if (!setResourceId(ri) || !ret.setResourceId(ri) ) {
		return false;
	}
	if (!setParentId(pi) || !ret.setParentId(pi) ) {
		return false;
	}
	if (rn != getResourceName()) {
		return setResourceName(rn) && ret.setResourceName(rn);
	}
	return true;
}

const pb::ResourceBase& ResourceBase::getResourceBase() const {
	return base_;
}

template <>
const pb::AE& ResourceBase::getElement<pb::AE>() const {
	return base_.ae();
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

const string& ResourceBase::getDomain() const {
	return domain_;
}

const string& ResourceBase::getIntCsi() const {
	return csi_;
}

const string& ResourceBase::getIntRi() const {
	return ri_;
}

SupportedResourceType ResourceBase::getResourceType() const {
	return static_cast<SupportedResourceType>(base_.ty());
}

bool ResourceBase::setResourceType(SupportedResourceType ty) {
	base_.set_ty(static_cast<pb::CommonTypes_SupportedResourceType>(ty));
	return setLastModifiedTimestamp();
}

const string& ResourceBase::getResourceId() const {
	return base_.ri();
}

bool ResourceBase::setResourceId(const string& ri) {
	base_.set_ri(ri);
	ri_ = ri;
	return setLastModifiedTimestamp();
}

const string& ResourceBase::getResourceName() const {
	return base_.rn();
}

bool ResourceBase::setResourceName(const string& rn) {
	base_.set_rn(rn);
	return setLastModifiedTimestamp();
}

int ResourceBase::getAccessControlPolicyNum() {
	return base_.acpi_size();
}

const std::string& ResourceBase::getAccessControlPolicy(int i) const {
	return base_.acpi(i);
}

bool ResourceBase::setAccessControlPolicy(const std::string& acpi) {
	base_.add_acpi(acpi);
	return true;
}

const string& ResourceBase::getParentId() const {
	return base_.pi();
}

bool ResourceBase::setParentId(const string& pi) {
	base_.set_pi(pi);
	return setLastModifiedTimestamp();
}

const string& ResourceBase::getAnncTo() const {
	return base_.at();
}

bool ResourceBase::setAnncTo(const string& at) {
	base_.set_at(at);
	return setLastModifiedTimestamp();
}

const string& ResourceBase::getAnncAttr() const {
	return base_.aa();
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

bool ResourceBase::checkResourceAttributes(Operation op) {
	if (checkResourceAttributes(op, base_, allowAttr)) {
		base_.set_ty(static_cast<pb::CommonTypes_SupportedResourceType>(getResourceCase()));
		return true;
	}
	// check mandatory attributes

	return false;
}

bool ResourceBase::checkResourceConsistency(const string& id_str) {
	if (getResourceType() != getResourceCase())	{
		cerr << "checkResourceConsistency failed. ";
		cerr << " ResourceType: " << static_cast<int>(getResourceType())
		     << " ResourceCase: " << static_cast<int>(getResourceCase()) << endl;
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

ResourceBase::~ResourceBase() { }

}	// OneM2M

}	// MicroWireless
