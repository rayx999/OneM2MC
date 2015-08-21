/*
 * AE.cc
 *
 *  Created on: 2015年8月21日
 *      Author: weimi_000
 */

#include <stdexcept>
#include <iostream>

#include "CommonTypes.pb.h"
#include "ResourceBase.pb.h"
#include "AE.pb.h"
#include "CommonTypes.h"
#include "AE.h"
#include "ResourceBase.h"

using namespace std;
using namespace MicroWireless::OneM2M::pb;

namespace MicroWireless {

namespace OneM2M {

AE::AE() : p_ae_(getAE()) { }

AE::AE(const string& json, const string& id_str) : ResourceBase(json, id_str) {
	p_ae_ = getAE();
	if (!checkIdConsistency()) {
		throw runtime_error("AE: checkIdConsistency failed.");
	}
}

AE::AE(const string& ri, ResourceStore<CSEBase>& rdb) : ResourceBase(ri, rdb) {
	p_ae_ = getAE();
}

bool AE::setAE(const string& ri, ResourceStore<CSEBase>& rdb) {
	bool ret_ = false;
	if (setResourceBase(ri, rdb)) {
		p_ae_ = getAE();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

bool AE::setAE(const string &json, const string& id_str) {
	bool ret_ = false;
	if (setResourceBase(json, id_str)) {
		p_ae_ = getAE();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

const string& AE::getAppName() {
	return p_ae_->apn();
}

const string& AE::getAppId() {
	return p_ae_->api();
}

const string& AE::getAEId() {
	return p_ae_->aei();
}

bool AE::checkIdConsistency() {
	return true;
}

AE::~AE() { }

}	// OneM2M

}	// MicroWireless


