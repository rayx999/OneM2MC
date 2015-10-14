/*
 * AEAnnc.cc
 *
 *  Created on: 2015年9月18日
 *      Author: weimi_000
 */


#include <stdexcept>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>

#include "CopyMessage.pb.h"
#include "AE.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"
#include "AE.h"
#include "AEAnnc.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

using google::protobuf::CopyMessage;

const AnncAttr AEAnnc::ma_ = {
		AttrName::RI(),
		AttrName::RN(),
		AttrName::ET(),
		AttrName::ACPI(),
		AttrName::LBL()
};

const AnncAttr AEAnnc::oa_ = {
		AttrName::APN(),
		AttrName::API(),
		AttrName::AEI(),
		AttrName::POA(),
		AttrName::OR(),
		AttrName::NL()
};

map<const string, map<Operation, AEClass::attrOption>> AEAnnc::allowAttr = {
   {AttrName::APN(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::API(),   { {Operation::CREATE, MANDATORY }, {Operation::UPDATE, MANDATORY } } },
   {AttrName::AEI(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::POA(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::OR() ,   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::NL() ,   { {Operation::CREATE, MANDATORY }, {Operation::UPDATE, OOPTIONAL } } }
};

map<const string, AEAnnc::attrOption> AEAnnc::anncAttr = {
   {AttrName::APN(),   OOPTIONAL},
   {AttrName::API(),   OOPTIONAL},
   {AttrName::AEI(),   OOPTIONAL},
   {AttrName::POA(),   OOPTIONAL},
   {AttrName::OR() ,   OOPTIONAL},
   {AttrName::NL() ,   OOPTIONAL}
};

AEAnnc::AEAnnc() : p_ae_(), p_aea_(getAEAnnc()) { }

AEAnnc::AEAnnc(const AEClass& ae) : p_ae_(&ae), p_aea_(getAEAnnc()) { }

bool AEAnnc::setResourceBase(const string& pc, const string& id_str, Operation op) {
	bool ret_ = false;
	if (ResourceBase::setResourceBase(pc, id_str, op)) {
		p_aea_ = getAEAnnc();
		ret_ = checkIdConsistency() && checkAttributes(op);
	}
	return ret_;
}

bool AEAnnc::setNewResourceAttr(const string& ri, const string& rn, const string& pi, AEAnnc& ret) {
	if (!ResourceBase::setNewResourceBaseAttr(ri, rn, pi, ret)) {
		return false;
	}
	// set AE-ID to FQ ri
	string aei_ = getDomain() + getIntCsi() + '/' + ri;
	return setAEId(aei_) && ret.setAEId(aei_);
}

const string& AEAnnc::getAppName() {
	return p_aea_->apn();
}

const string& AEAnnc::getAppId() {
	return p_aea_->api();
}

const string& AEAnnc::getAEId() {
	return p_aea_->aei();
}

bool AEAnnc::setAEId(const string& aei) {
	p_aea_->set_aei(aei);
	return setLastModifiedTimestamp();
}

bool AEAnnc::filterAnncAttr() {
	return ResourceBase::filterAnncAttr<pb::AE, &ResourceBase::getAEAnnc>(anncAttr);
}

bool AEAnnc::copyAnncFields() {
	return copyAnncFields(ma_, oa_);
}

bool AEAnnc::copyAnncFields(const AnncAttr& oa) {
	return copyAnncFields(ma_, oa);
}

bool AEAnnc::copyAnncFields(AnncAttr ma, AnncAttr oa) {
	ResourceBase::copyAnncFields(*p_ae_, ma, oa);

	const pb::AE& ae_pb_ = p_ae_->getElement<pb::AE>();
	CopyMessage cf_(ae_pb_, *p_aea_);
	cf_.copyFields(ma);
	cf_.copyFields(oa);
	if (ma.size() > 0) {
		cerr << "AEAnnc::copyAnncFields: MA fields missing in source:\n";
		std::copy(ma_.begin(), ma_.end(), ostream_iterator<string>(cout));
		return false;
	}
	return true;
}

bool AEAnnc::checkIdConsistency() {
	if (getResourceType() != SupportedResourceType::AE_ANNC) {
		cerr << "checkIdConsistency: resource type mismatch:" <<
		"should be AE_ANNC but is: " << (int)base_.ty() << endl;
		return false;
	}
	string aei_ = base_.aea().aei();
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

bool AEAnnc::checkAttributes(Operation op) {
	// check aa attributes

	// reuse AEClass allowAttr
	return checkResourceAttributes(op, *p_aea_, allowAttr);
}

AEAnnc::~AEAnnc() { }

}	// OneM2M

}	// MicroWireless


