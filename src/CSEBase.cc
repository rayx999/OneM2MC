/*
 * CSEBase.cc
 *
 *  Created on: 2015年7月15日
 *      Author: weimi_000
 */

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>
#include <json2pb.h>

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

CSEBase::CSEBase() : p_cse_(new(pb::CSEBase)) {
	// set create time
	TimeStamp ct_;
	gettimeofday(&ct_, NULL);
	if (!setCreateTimestamp(ct_)) {
		throw runtime_error("setCreateTimestamp failed.");
	}
	cse_orphan_ = true;
}

CSEBase::CSEBase(const string& json) : p_cse_(new(pb::CSEBase)) {
	CSEBase();
	if (!setCSEBase(json)) {
		throw runtime_error("Set CSE Json failed.");
	}
}

CSEBase::CSEBase(const string& ri, ResourceStore<CSEBase>& rdb) : p_cse_(new(pb::CSEBase)) {
	CSEBase();
	if (!setCSEBase(ri, rdb)) {
		throw runtime_error("Set CSE resource file failed.");
	}
}

bool CSEBase::setCSEBase(const string& ri, ResourceStore<CSEBase>& rdb) {
	bool ret_ = false;

	if (!setResourceBase(ri, rdb)) {
		cerr << "SetResourceBase failed.\n";
	} else {
		// release orphan cse, keep the new one
		delete p_cse_;
		p_cse_ = base_.mutable_cse();
		cse_orphan_ = false;
		ret_ = checkIdConsistency();
	}
	return ret_;
}

bool CSEBase::setCSEBase(const string &json) {
	// keep original create time
	TimeStamp _ct;
	getCreateTimestamp(_ct);

	// parse to PB buffer
	json2pb(*p_cse_, json.c_str(), json.length());

	// need to keep original create time if Json file
	// doesn't have create time set up
	if (!p_cse_->has_ct()) {
		setCreateTimestamp(_ct);
	}

	// set CSEBase to ResourceBase
	base_.set_allocated_cse(p_cse_);
	cse_orphan_ = false;

	return checkIdConsistency();
}

const string& CSEBase::getDomain() {
	return domain_;
}

const string& CSEBase::getCSEId() {
	return p_cse_->csi();
}

CSEType CSEBase::getCSEType() {
	return (CSEType)p_cse_->ty();
}

const string& CSEBase::getResourceId() {
	return p_cse_->ri();
}

const string& CSEBase::getResourceName() {
	return p_cse_->rn();
}

bool CSEBase::getCreateTimestamp(TimeStamp &ct) {
	if (p_cse_->has_ct()) {
		const google::protobuf::Timestamp &_ct =
				p_cse_->ct();
		ct.tv_sec = _ct.seconds();
		ct.tv_usec = _ct.nanos() / 1000;
		return true;
	}
	return false;
}

bool CSEBase::getLastModifiedTimestamp(TimeStamp &lt) {
	if (p_cse_->has_lt()) {
		const google::protobuf::Timestamp &_lt =
				p_cse_->lt();
		lt.tv_sec = _lt.seconds();
		lt.tv_usec = _lt.nanos() / 1000;
		return true;
	}
	return false;
}

bool CSEBase::setCreateTimestamp(TimeStamp &ct) {
	google::protobuf::Timestamp * _ts = p_cse_->mutable_ct();

	if (_ts != NULL) {
		_ts->set_seconds(ct.tv_sec);
		_ts->set_nanos(ct.tv_usec * 1000);
		return true;
	} else {
		cerr << "Can't allocate pb::Timestamp." << endl;
		return false;
	}
}

bool CSEBase::setLastModifiedTimestamp(TimeStamp &lt) {
	google::protobuf::Timestamp * _ts = p_cse_->mutable_lt();
	if (_ts != NULL) {
		_ts->set_seconds(lt.tv_sec);
		_ts->set_nanos(lt.tv_usec * 1000);
		return true;
	} else {
		cerr << "Can't allocate pb::Timestamp." << endl;
		return false;
	}
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
		if (*_it == rt) {
			return true;
		}
	}
	return false;
}

bool CSEBase::outToResourceStore(const string& res_path, ResourceStore<CSEBase>& rdb) {
	// update last_modified_time
	TimeStamp lt_;
	gettimeofday(&lt_, NULL);
	if (!setLastModifiedTimestamp(lt_)) {
		cerr << "Can't set last modified time." << endl;
		return false;
	}

	if (!ResourceBase::outToResourceStore(res_path, rdb)) {
       return false;
    }
	return true;
}

bool CSEBase::checkIdConsistency() {
	static boost::regex pattern_("(^//[\\.\\w-]+)(/[\\w-]+)(?:/([\\w-]+))?");
	boost::smatch sm;
	bool ret = false;

	if (getResourceBase() != CSE_BASE) {
		cerr << "CSE resource file with wrong resource base!\n";
		cerr << "Current resource: " << getResourceBase() << endl;
	}

	const string& id_ = getResourceId();
	boost::regex_match(id_, sm, pattern_);
	switch (sm.size()) {
	case 4:
	case 3: if (getResourceName().compare(sm[3]) != 0) {
				cerr << "Resource name conflicts." << getResourceName();
				cerr << " vs. " << sm[3] << endl;
				break;
			}
	case 2: if (getCSEId().compare(sm[2]) != 0) {
				cerr << "CSEId conflicts." << getCSEId();
				cerr << " vs. " << sm[2] << endl;
				break;
			}
	case 1: domain_ = sm[1];
			ret = true;
			break;
	default:
		cerr << "regex match error on " << getResourceId() << endl;
	}

	return ret;
}

CSEBase::~CSEBase() {
	// delete orphan cse, otherwise it's ResourceBase to release
	if (cse_orphan_) {
		delete p_cse_;
	}
}

}	// OneM2M

}	// MicroWireless



