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
   {AttrName::API(),   { {Operation::CREATE, MANDATORY }, {Operation::UPDATE, MANDATORY } } },
   {AttrName::AEI(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
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

bool AEClass::setAE(const string& ri, ResourceStore<CSEBase>& rdb) {
	bool ret_ = false;
	if (ResourceBase::setResourceBase(ri, rdb)) {
		p_ae_ = getAE();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

bool AEClass::setAE(const string &json, const string& id_str) {
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

const string& AEClass::getAppName() {
	return p_ae_->apn();
}

const string& AEClass::getAppId() {
	return p_ae_->api();
}

const string& AEClass::getAEId() {
	return p_ae_->aei();
}

bool AEClass::setAEId(const string& aei) {
	p_ae_->set_aei(aei);
	return setLastModifiedTimestamp();
}

bool AEClass::checkIdConsistency() {
	if (getResourceType() != SupportedResourceType::AE) {
		cerr << "checkIdConsistency: resource type mismatch:" <<
		"should be AE but is: " << (int)base_.ty() << endl;
		return false;
	}
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
	return true;
}

bool AEClass::checkAttributes(Operation op) {
	return checkResourceAttributes(op, *p_ae_, allowAttr);
}

AEClass::~AEClass() { }

}	// OneM2M

}	// MicroWireless


