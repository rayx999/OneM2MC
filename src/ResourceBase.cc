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
#include <boost/algorithm/string.hpp>
#include <json2pb.h>

#include "CommonTypes.pb.h"
#include "CopyMessage.pb.h"
#include "ResourceBase.pb.h"
#include "CommonUtils.h"
#include "ResourceBase.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"

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

map<const string, ResourceBase::attrOption>	ResourceBase::anncAttr = {
   {AttrName::TY(),   NOTPRESENT},
   {AttrName::RI(),   MANDATORY },
   {AttrName::RN(),   MANDATORY },
   {AttrName::PI(),   NOTPRESENT},
   {AttrName::CT(),   NOTPRESENT},
   {AttrName::LT(),   NOTPRESENT},
   {AttrName::ET(),   MANDATORY },
   {AttrName::ACPI(), MANDATORY },
   {AttrName::LBL(),  MANDATORY },
   {AttrName::AA(),   NOTPRESENT},
   {AttrName::AT(),   NOTPRESENT},
   {AttrName::ST(),   NOTPRESENT}
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

bool ResourceBase::setResourceBase(const ResponsePrim& rspp) {
	// parse to PB buffer, try pb in string first
	if (!base_.ParseFromString(rspp.getContent())) {
		cerr << "setResourceBase: ParseFromString failed.\n";
		return false;
	}
	if (!parseIds(rspp.getFrom(), CSERegex, domain_, csi_, ri_)) {
		cerr << "setResourceBase: parseIds failed. res_path: " << rspp.getFrom() << endl;
		return false;
	}
	if (ri_.empty()) {
		ri_ = getResourceId();
	}
	return true;
}

bool ResourceBase::setNewAttr(const string& ri, const string& rn, const string& pi) {
	return setNewAttr(ri, rn, pi, NULL);
}

bool ResourceBase::setNewAttr(const string& ri, const string& rn, const string& pi, ResourceBase* p_ret) {
	if (!setResourceId(ri) || (p_ret != NULL && !p_ret->setResourceId(ri))) {
		return false;
	}
	if (!setParentId(pi) || (p_ret != NULL && !p_ret->setParentId(pi))) {
		return false;
	}
	if (rn != getResourceName()) {
		if (!setResourceName(rn) || (p_ret != NULL && !p_ret->setResourceName(rn))) {
			return false;
		}
	}
	return true;
}

bool ResourceBase::updateResource(const ResourceBase& upd) {
	const Reflection* r_ = upd.base_.GetReflection();
	vector<const FieldDescriptor*> fs_;
	r_->ListFields(upd.base_, &fs_);
	for (unsigned int i = 0; i < fs_.size(); i++) {
		if (fs_[i]->name() == AttrName::ET()) {
			Timestamp default_ts;
			if (upd.base_.et() == default_ts) {
				base_.clear_et();
			} else {
				base_.mutable_et()->CopyFrom(upd.base_.et());
			}
		} else if (fs_[i]->name() == AttrName::LBL()) {
			if (upd.base_.lbl_size() == 0) {
				base_.clear_lbl();
			} else {
				base_.mutable_lbl()->CopyFrom(upd.base_.lbl());
			}
		} else if (fs_[i]->name() == AttrName::AT()) {
			if (upd.base_.at().empty() || upd.base_.at() == "") {
				base_.clear_at();
			} else {
				base_.set_at(upd.base_.at());
			}
		} else if (fs_[i]->name() == AttrName::AA()) {
			if (upd.base_.aa().empty() || upd.base_.aa() == "") {
				base_.clear_aa();
			} else {
				base_.set_aa(upd.base_.aa());
			}
		}
	}
	return setLastModifiedTimestamp();
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

pb::AE* ResourceBase::getAEAnnc() {
	return base_.mutable_aea();
}

pb::RemoteCSE* ResourceBase::getRemoteCSE() {
	return base_.mutable_csr();
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

bool ResourceBase::setAnncAttr(const string& aa) {
	base_.set_aa(aa);
	return setLastModifiedTimestamp();
}

bool ResourceBase::getCreateTimestamp(TimeStamp &ts) {
	return getTimestamp<&pb::ResourceBase::has_ct, &pb::ResourceBase::ct>(ts);
}

bool ResourceBase::getLastModifiedTimestamp(TimeStamp &ts) {
	return getTimestamp<&pb::ResourceBase::has_lt, &pb::ResourceBase::lt>(ts);
}

bool ResourceBase::getExpirationTimestamp(TimeStamp &ts) {
	return getTimestamp<&pb::ResourceBase::has_et, &pb::ResourceBase::et>(ts);
}

bool ResourceBase::setCreateTimestamp(TimeStamp* p_ts) {
	return setTimestamp<&pb::ResourceBase::mutable_ct>(p_ts);
}

bool ResourceBase::setLastModifiedTimestamp(TimeStamp* p_ts) {
	return setTimestamp<&pb::ResourceBase::mutable_lt>(p_ts);
}

bool ResourceBase::setExpirationTimestamp(TimeStamp* p_ts) {
	return setTimestamp<&pb::ResourceBase::mutable_et>(p_ts);
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

bool ResourceBase::serializeToString(string* pc) {
	return base_.SerializeToString(pc);
}

void ResourceBase::copyAnncFields(const ResourceBase& src, AnncAttr& ma, AnncAttr& oa) {
	google::protobuf::CopyMessage cf_(src.getResourceBase(), base_);
	cf_.copyFields(ma);
	cf_.copyFields(oa);
}

void ResourceBase::copyTimeStamps(ResourceBase& src) {
	*base_.mutable_ct() = src.base_.ct();
	*base_.mutable_lt() = src.base_.lt();
}

void ResourceBase::clrNotPresentAttrs() {
	base_.clear_ty();
	base_.clear_rn();
	base_.clear_ri();
	base_.clear_pi();
	base_.clear_ct();
	base_.clear_lt();
}

bool ResourceBase::compare(pb::ResourceBase& tgt, bool noct, bool nolt) {
	pb::ResourceBase src_ = base_;
	pb::ResourceBase tgt_ = tgt;

	if (noct) {
		src_.clear_ct();
		tgt_.clear_ct();
	}
	if (nolt) {
		src_.clear_lt();
		tgt_.clear_lt();
	}
	return CompareMessage(src_, tgt_);
}

string ResourceBase::getJson() {
	return pb2json(base_);
}

ResourceBase::~ResourceBase() { }

using google::protobuf::Message;
using google::protobuf::Descriptor;
using google::protobuf::Reflection;

bool ResourceBase::filterAnncAttr(Message* msg, map<const string, attrOption> attr) {

	vector<string> aas_;
	boost::split(aas_, getAnncAttr(), boost::is_any_of(" ,"));
	for (auto a : attr) {
		if (a.first.empty()) continue;
		if (std::find(aas_.begin(), aas_.end(), a.first) == aas_.end()) {
			// mandatory attr always present
			if (attr.at(a.first) == MANDATORY) continue;
			// OA attr will not present
			attr.at(a.first) = NOTPRESENT;
		}
	}

	const Descriptor* d_ = msg->GetDescriptor();
	const Reflection* r_ = msg->GetReflection();
	bool ret_ = true;
	for (auto a : attr) {
		switch (a.second) {
		case NOTPRESENT:
			if (d_->FindFieldByName(a.first) != NULL) {
				r_->ClearField(msg, d_->FindFieldByName(a.first));
			}
			break;
		case MANDATORY:
			if (d_->FindFieldByName(a.first) == NULL) {
				cerr << "ResourceBase::filterAnncAttr: mandatory annc field missing: "
						<< a.first << endl;
				ret_ = false;
			}
			break;
		default:
			break;
		}
	}
	return ret_;
}

}	// OneM2M

}	// MicroWireless
