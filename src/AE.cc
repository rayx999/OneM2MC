/*
 * AEClass.cc
 *
 *  Created on: 2015年8月21日
 *      Author: weimi_000
 */

#include <stdexcept>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>

#include "CommonTypes.pb.h"
#include "ResourceBase.pb.h"
#include "AE.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"
#include "AE.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

map<const string, map<Operation, AEClass::attrOption>> AEClass::allowAttr = {
   {AttrName::APN(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::API(),   { {Operation::CREATE, MANDATORY }, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::AEI(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::POA(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::OR() ,   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::NL() ,   { {Operation::CREATE, NOTPRESENT}, {Operation::UPDATE, NOTPRESENT} } }
};

AEClass::AEClass() : p_ae_(getAE()) { }

AEClass::AEClass(const string& json, const string& id_str) : ResourceBase(json, id_str) {
	p_ae_ = getAE();
	if (!checkIdConsistency()) {
		throw runtime_error("AEClass: checkIdConsistency failed.");
	}
}

AEClass::AEClass(const string& ri, ResourceStore<CSEBase>& rdb) : ResourceBase(ri, rdb) {
	p_ae_ = getAE();
	if (!checkIdConsistency()) {
		throw runtime_error("AEClass: checkIdConsistency failed.");
	}
}

bool AEClass::setResourceBase(const string& ri, ResourceStore<CSEBase>& rdb) {
	bool ret_ = false;
	if (ResourceBase::setResourceBase(ri, rdb)) {
		p_ae_ = getAE();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

bool AEClass::setResourceBase(const string &json, const string& id_str) {
	bool ret_ = false;
	if (ResourceBase::setResourceBase(json, id_str)) {
		p_ae_ = getAE();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

bool AEClass::setResourceBase(const string& pc, const string& id_str, Operation op) {
	bool ret_ = false;
	if (ResourceBase::setResourceBase(pc, id_str, op)) {
		p_ae_ = getAE();
		ret_ = checkIdConsistency() && checkAttributes(op);
	}
	return ret_;
}

bool AEClass::setNewResourceAttr(const string& ri, const string& rn, const string& pi, AEClass& ret) {
	if (!ResourceBase::setNewResourceBaseAttr(ri, rn, pi, ret)) {
		return false;
	}
	// set AE-ID to FQ ri
	string aei_ = getDomain() + getIntCsi() + '/' + ri;
	return setAEId(aei_) && ret.setAEId(aei_);
}

bool AEClass::updateResource(const AEClass& upd) {
	if (!ResourceBase::updateResource(upd)) {
		return false;
	}

	const Reflection* r_ = upd.base_.ae().GetReflection();
	vector<const FieldDescriptor*> fs_;
	r_->ListFields(upd.base_.ae(), &fs_);
	for (unsigned int i = 0; i < fs_.size(); i++) {
		const string def_str_;
		if (fs_[i]->name() == AttrName::APN()) {
			if (upd.getAppName().empty() || upd.getAppName() == def_str_) {
				p_ae_->clear_apn();
			} else {
				p_ae_->set_apn(upd.getAppName());
			}
		} else if (fs_[i]->name() == AttrName::API()) {
			if (upd.getAppId().empty() || upd.getAppId() == def_str_) {
				p_ae_->clear_api();
			} else {
				p_ae_->set_api(upd.getAppId());
			}
		} else if (fs_[i]->name() == AttrName::AEI()) {
			if (upd.getAEId().empty() || upd.getAEId() == def_str_) {
				p_ae_->clear_aei();
			} else {
				p_ae_->set_aei(upd.getAEId());
			}
		} else if (fs_[i]->name() == AttrName::POA()) {
			if (upd.base_.ae().poa_size() == 0) {
				p_ae_->clear_poa();
			} else {
				p_ae_->mutable_poa()->CopyFrom(upd.base_.ae().poa());
			}
		} else if (fs_[i]->name() == AttrName::OR()) {
			if (upd.getOr().empty() || upd.getOr() == def_str_) {
				p_ae_->clear_or_();
			} else {
				p_ae_->set_or_(upd.getOr());
			}
		}
	}
	return setLastModifiedTimestamp();
}

const string& AEClass::getAppName() const {
	return p_ae_->apn();
}

bool AEClass::setAppName(const string& apn) {
	p_ae_->set_apn(apn);
	return setLastModifiedTimestamp();
}

const string& AEClass::getAppId() const {
	return p_ae_->api();
}

bool AEClass::setAppId(const string& api) {
	p_ae_->set_api(api);
	return setLastModifiedTimestamp();
}

const string& AEClass::getAEId() const {
	return p_ae_->aei();
}

bool AEClass::setAEId(const string& aei) {
	p_ae_->set_aei(aei);
	return setLastModifiedTimestamp();
}

const string& AEClass::getOr() const {
	return p_ae_->or_();
}

bool AEClass::setOr(const string& or_) {
	p_ae_->set_or_(or_);
	return setLastModifiedTimestamp();
}

bool AEClass::checkIdConsistency() {
	if (getResourceType() != SupportedResourceType::AE) {
		cerr << "checkIdConsistency: resource type mismatch:" <<
		"should be AE but is: " << (int)base_.ty() << endl;
		return false;
	}
/*
	string aei_ = base_.ae().aei();
	size_t aei_pos_ = aei_.rfind('/');
	if (aei_pos_ != string::npos) {
		aei_ = aei_.substr(aei_pos_ + 1);
	}
	if (!getResourceName().empty() && !aei_.empty() &&
		!boost::iequals(aei_, getResourceName())) {
		cerr << "checkIdConsistency: AEId conflicts.";
		cerr << aei_ << " vs. " << getResourceName() << endl;
		return false;
	}
*/
	return true;
}

bool AEClass::checkAttributes(Operation op) {
	return checkResourceAttributes(op, *p_ae_, allowAttr);
}

AEClass::~AEClass() { }

}	// OneM2M

}	// MicroWireless


