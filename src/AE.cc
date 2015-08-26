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

AEClass::AEClass() : p_ae_(getAE()) { }

AEClass::AEClass(const string& json, const string& id_str) : ResourceBase(json, id_str) {
	p_ae_ = getAE();
	if (!checkIdConsistency()) {
		throw runtime_error("AEClass: checkIdConsistency failed.");
	}
}

AEClass::AEClass(const string& ri, ResourceStore<CSEBase>& rdb) : ResourceBase(ri, rdb) {
	p_ae_ = getAE();
}

bool AEClass::setAE(const string& ri, ResourceStore<CSEBase>& rdb) {
	bool ret_ = false;
	if (setResourceBase(ri, rdb)) {
		p_ae_ = getAE();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

bool AEClass::setAE(const string &json, const string& id_str) {
	bool ret_ = false;
	if (setResourceBase(json, id_str)) {
		p_ae_ = getAE();
		ret_ = checkIdConsistency();
	}
	return ret_;
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

bool AEClass::checkIdConsistency() {
	if (!getResourceName().empty() &&
		!boost::iequals(getAEId(), getResourceName())) {
		cerr << "checkIdConsistency: AEId conflicts.";
		cerr << getAEId() << " vs. " << getResourceName() << endl;
		return false;
	}
	return true;
}

AEClass::~AEClass() { }

}	// OneM2M

}	// MicroWireless


