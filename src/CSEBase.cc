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
#include "CSEBase.pb.h"
#include "CSEBase.h"

using namespace std;
using namespace MicroWireless::OneM2M::pb;

namespace MicroWireless {

namespace OneM2M {

CSEBase::CSEBase() {
	// set create time
	TimeStamp _ct;
	gettimeofday(&_ct, NULL);

	setCreateTimestamp(_ct);
}

CSEBase::CSEBase(const char * fn) {
	CSEBase();
	if (!setCSEBase(fn)) {
		throw runtime_error("Set CSE resource file failed.");
	}
}

CSEBase::CSEBase(const string& json) {
	CSEBase();
	if (!setCSEBase(json)) {
		throw runtime_error("Set CSE Json failed.");
	}
}

bool CSEBase::setCSEBase(const char * fn) {
	bool _ret = false;
    fstream ins(fn, ios::in | ios::binary);

    if (!ins) {
        cout << "Can't open resource file " << fn << endl;
    } else if (!cse_base_.ParseFromIstream(&ins)) {
        cerr << "Failed to parse resource file " << fn << endl;
    } else {
    	_ret = checkIdConsistency();
    }

    return _ret;
}

bool CSEBase::setCSEBase(const string &json) {
	pb::CSEBase _cse_base;

	// parse to PB buffer
	json2pb(_cse_base, json.c_str(), json.length());

	// need to keep original create time if Json file
	// doesn't have create time set up
	TimeStamp _ct;
	bool _ct_set = true;
	if (!_cse_base.has_ct()) {
		getCreateTimestamp(_ct);
		_ct_set = false;
	}
	cse_base_ = _cse_base;
	if (!_ct_set) {
		setCreateTimestamp(_ct);
	}

	return checkIdConsistency();
}

const string& CSEBase::getDomain() {
	return domain_;
}

const string& CSEBase::getCSEId() {
	return cse_base_.csi();
}

CSEType CSEBase::getCSEType() {
	return (CSEType)cse_base_.ty();
}

const string& CSEBase::getResourceId() {
	return cse_base_.ri();
}

const string& CSEBase::getResourceName() {
	return cse_base_.rn();
}

bool CSEBase::getCreateTimestamp(TimeStamp &ct) {
	if (cse_base_.has_ct()) {
		const google::protobuf::Timestamp &_ct =
				cse_base_.ct();
		ct.tv_sec = _ct.seconds();
		ct.tv_usec = _ct.nanos() / 1000;
		return true;
	}
	return false;
}

bool CSEBase::getLastModifiedTimestamp(TimeStamp &lt) {
	if (cse_base_.has_lt()) {
		const google::protobuf::Timestamp &_lt =
				cse_base_.lt();
		lt.tv_sec = _lt.seconds();
		lt.tv_usec = _lt.nanos() / 1000;
		return true;
	}
	return false;
}

bool CSEBase::setCreateTimestamp(TimeStamp &ct) {
	google::protobuf::Timestamp * _ts =
			new(google::protobuf::Timestamp);

	if (_ts != NULL) {
		_ts->set_seconds(ct.tv_sec);
		_ts->set_nanos(ct.tv_usec * 1000);
		cse_base_.set_allocated_ct(_ts);
		return true;
	} else {
		cerr << "Can't allocate pb::Timestamp." << endl;
		return false;
	}
}

bool CSEBase::setLastModifiedTimestamp(TimeStamp &lt) {
	google::protobuf::Timestamp * _ts =
			new(google::protobuf::Timestamp);

	if (_ts != NULL) {
		_ts->set_seconds(lt.tv_sec);
		_ts->set_nanos(lt.tv_usec * 1000);
		cse_base_.set_allocated_lt(_ts);
		return true;
	} else {
		cerr << "Can't allocate pb::Timestamp." << endl;
		return false;
	}
}

int CSEBase::getSupportedResource(SupportedResourceType *&rt) {
	google::protobuf::RepeatedField<int> * _rt =
			cse_base_.mutable_srt();
    rt = (SupportedResourceType *)_rt->mutable_data();
	return cse_base_.srt_size();
}

bool CSEBase::isResourceSupported(SupportedResourceType rt) {
	google::protobuf::RepeatedField<int>::const_iterator _it;
	const google::protobuf::RepeatedField<int>& _srt =
			cse_base_.srt();
	for (_it = _srt.begin(); _it != _srt.end(); _it++) {
		if (*_it == rt) {
			return true;
		}
	}
	return false;
}

bool CSEBase::outToFile(const char * fn) {
	// update last_modified_time
	TimeStamp _lt;
	gettimeofday(&_lt, NULL);
	if (!setLastModifiedTimestamp(_lt)) {
		cerr << "Can't set last modified time." << endl;
		return false;
	}

    fstream ous(fn, ios::out | ios::trunc | ios::binary);
    if (!cse_base_.SerializeToOstream(&ous)) {
      cerr << "Failed to write " << fn << endl;
      return false;
    }
	return true;
}

string CSEBase::getJson() {
  	return pb2json(cse_base_);
}

bool CSEBase::checkIdConsistency() {
	static boost::regex pattern_("(^//[\\.\\w-]+)(/[\\w-]+)(?:/([\\w-]+))?");
	boost::smatch sm;
	bool ret = false;

	boost::regex_match(getResourceId(), sm, pattern_);
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

}	// OneM2M

}	// MicroWireless



