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

map<int, map<Operation, AEClass::attrOption>> AEClass::allowAttr = {
   {TAG_APN,   { {OPERATION_CREATE, OPTIONAL  }, {OPERATION_UPDATE, OPTIONAL  } } },
   {TAG_API,   { {OPERATION_CREATE, MANDATORY }, {OPERATION_UPDATE, MANDATORY } } },
   {TAG_AEI,   { {OPERATION_CREATE, NOTPRESENT}, {OPERATION_UPDATE, NOTPRESENT} } },
   {TAG_POA,   { {OPERATION_CREATE, OPTIONAL  }, {OPERATION_UPDATE, OPTIONAL  } } },
   {TAG_OR ,   { {OPERATION_CREATE, OPTIONAL  }, {OPERATION_UPDATE, OPTIONAL  } } },
   {TAG_NL ,   { {OPERATION_CREATE, NOTPRESENT}, {OPERATION_UPDATE, NOTPRESENT} } }
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

	return setAEId(rn) && ret.setAEId(rn);
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
	if (!getResourceName().empty() && !getAEId().empty() &&
		!boost::iequals(getAEId(), getResourceName())) {
		cerr << "checkIdConsistency: AEId conflicts.";
		cerr << getAEId() << " vs. " << getResourceName() << endl;
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


