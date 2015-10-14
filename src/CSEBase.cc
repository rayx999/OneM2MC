/*
 * CSEBase.cc
 *
 *  Created on: 2015年7月15日
 *      Author: weimi_000
 */

#include <stdexcept>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>

#include "CommonTypes.pb.h"
#include "ResourceBase.pb.h"
#include "CSEBase.pb.h"
#include "CommonTypes.h"
#include "CSEBase.h"
#include "ResourceBase.h"

using namespace std;
using namespace MicroWireless::OneM2M::pb;

namespace MicroWireless {

namespace OneM2M {

CSEBase::CSEBase() : p_cse_(getCSEBase()) { }

CSEBase::CSEBase(const string& json, const string& id_str) : ResourceBase(json, id_str) {
	p_cse_ = getCSEBase();
	if (!checkIdConsistency()) {
		throw runtime_error("checkIdConsistency failed.");
	}
}

CSEBase::CSEBase(const string& ri, ResourceStore<CSEBase>& rdb)	: ResourceBase(ri, rdb) {
	p_cse_ = getCSEBase();
}

bool CSEBase::setCSEBase(const string& ri, ResourceStore<CSEBase>& rdb) {
	bool ret_ = false;
	if (setResourceBase(ri, rdb)) {
		p_cse_ = getCSEBase();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

bool CSEBase::setCSEBase(const string &json, const string& id_str) {
	bool ret_ = false;
	if (setResourceBase(json, id_str)) {
		p_cse_ = getCSEBase();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

CSEType CSEBase::getCSEType() {
	return static_cast<CSEType>(p_cse_->cst());
}

const string& CSEBase::getCSEId() {
	return p_cse_->csi();
}

int CSEBase::getSupportedResource(SupportedResourceType *&rt) {
	google::protobuf::RepeatedField<int> * _rt =
			p_cse_->mutable_srt();
    rt = (SupportedResourceType *)_rt->mutable_data();
	return p_cse_->srt_size();
}

bool CSEBase::isResourceSupported(SupportedResourceType rt) {
	google::protobuf::RepeatedField<int>::const_iterator _it;
	const google::protobuf::RepeatedField<int>& _srt =
			p_cse_->srt();
	for (_it = _srt.begin(); _it != _srt.end(); _it++) {
		if (*_it == static_cast<int>(rt)) {
			return true;
		}
	}
	return false;
}

bool CSEBase::checkIdConsistency() {
	if (getResourceType() != SupportedResourceType::CSE_BASE) {
		cerr << "checkIdConsistency: resource type mismatch:" <<
		"should be CSE_BASE but is: " << (int)base_.ty() << endl;
		return false;
	}
	if (domain_.empty()) {
		cerr << "checkIdConsistency: Missing domain.";
		return false;
	}
	if (!boost::iequals(getCSEId(), csi_)) {
		cerr << "checkIdConsistency: CSEId conflicts.";
		cerr << getCSEId() << " vs. " << csi_ << endl;
		return false;
	}
	return true;
}

CSEBase::~CSEBase() { }

}	// OneM2M

}	// MicroWireless



