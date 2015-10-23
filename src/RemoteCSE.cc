/*
 * RemoteCSE.cc
 *
 *  Created on: 2015年10月23日
 *      Author: weimi_000
 */


#include <stdexcept>
#include <iostream>

#include "CommonTypes.pb.h"
#include "ResourceBase.pb.h"
#include "RemoteCSE.pb.h"
#include "CommonTypes.h"
#include "RemoteCSE.h"
#include "ResourceBase.h"

using namespace MicroWireless::OneM2M::pb;

namespace MicroWireless {

namespace OneM2M {

std::map<const std::string, std::map<Operation, ResourceBase::attrOption>> RemoteCSE::allowAttr = {
   {AttrName::CST(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::POA(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::CB(),    { {Operation::CREATE, MANDATORY }, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::CSI(),   { {Operation::CREATE, MANDATORY }, {Operation::UPDATE, NOTPRESENT} } },
   {AttrName::MEI(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::TRI(),   { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::RR(),    { {Operation::CREATE, MANDATORY }, {Operation::UPDATE, OOPTIONAL } } },
   {AttrName::NL(),    { {Operation::CREATE, OOPTIONAL }, {Operation::UPDATE, OOPTIONAL } } }
};

RemoteCSE::RemoteCSE() : p_csr_(getRemoteCSE()) { }

RemoteCSE::RemoteCSE(const std::string& json, const std::string& id_str) : ResourceBase(json, id_str) {
	p_csr_ = getRemoteCSE();
	if (!checkIdConsistency()) {
		throw runtime_error("RemoteCSE::checkIdConsistency failed.");
	}
}

RemoteCSE::RemoteCSE(const std::string& ri, ResourceStore<CSEBase>& rdb) : ResourceBase(ri, rdb) {
	p_csr_ = getRemoteCSE();
}

bool RemoteCSE::setResourceBase(const std::string& ri, ResourceStore<CSEBase>& rdb) {
	bool ret_ = false;
	if (ResourceBase::setResourceBase(ri, rdb)) {
		p_csr_ = getRemoteCSE();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

bool RemoteCSE::setResourceBase(const std::string &json, const std::string& id_str) {
	bool ret_ = false;
	if (ResourceBase::setResourceBase(json, id_str)) {
		p_csr_ = getRemoteCSE();
		ret_ = checkIdConsistency();
	}
	return ret_;
}

CSEType RemoteCSE::getCSEType() {
	return static_cast<CSEType>(p_csr_->cst());
}

const std::string& RemoteCSE::getCSEBase() const {
	return p_csr_->cb();
}

const std::string& RemoteCSE::getCSEId() const {
	return p_csr_->csi();
}

const std::string& RemoteCSE::getM2MExtId() const {
	return p_csr_->mei();
}

bool RemoteCSE::setM2MExtId(const std::string& mei) {
	p_csr_->set_mei(mei);
	return setLastModifiedTimestamp();
}

const std::string& RemoteCSE::getTriggerRecipientId() const {
	return p_csr_->tri();
}

bool RemoteCSE::setTriggerRecipientId(const std::string& tri){
	p_csr_->set_tri(tri);
	return setLastModifiedTimestamp();
}

bool RemoteCSE::getRequestReachability() {
	return p_csr_->rr();
}

bool RemoteCSE::setRequestReachability(bool rr){
	p_csr_->set_rr(rr);
	return setLastModifiedTimestamp();
}

bool RemoteCSE::checkIdConsistency() {
	if (getResourceType() != SupportedResourceType::REMOTE_CSE) {
		std::cerr << "RemoteCSE::checkIdConsistency: resource type mismatch:" <<
		"should be REMOTE_CSE but is: " << (int)base_.ty() << std::endl;
		return false;
	}
	return true;
}

bool RemoteCSE::checkAttributes(Operation op) {
	return checkResourceAttributes(op, *p_csr_, allowAttr);
}

RemoteCSE::~RemoteCSE() { }

}	// OneM2M

}	// MicroWireless

