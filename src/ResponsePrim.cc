/*
 * ResponsePrim.cc
 *
 *  Created on: 2015年7月18日
 *      Author: weimi_000
 */

#include "../include/ResponsePrim.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <json2pb.h>
#include <RequestPrim.h>

#include "CommonUtils.h"
#include "ResponsePrim.pb.h"

using namespace std;
using namespace MicroWireless::OneM2M;

namespace MicroWireless {

namespace OneM2M {

ResponsePrim::ResponsePrim(RequestPrim *p_request, const string &json) {
	// keep original request reference
	if (p_request == NULL || !p_request->isValid()) {
		throw runtime_error("Matching request is invalid!");
	}
	p_request_ = p_request;

	// parse to PB buffer
	json2pb(rspp_, json.c_str(), json.length());

	if (!isValid()) {
		throw runtime_error("ResponsePrim in JSON not valid!");
	}
}

ResponsePrim::ResponsePrim(RequestPrim *p_request, ResponseStatusCode rsc, const string & rqi){
	if (rsc != RSC_BAD_REQUEST) {
		if (rqi.empty()) {
			throw runtime_error("ResponsePrim mandatory field: requestId missing!");
		}
		// keep original request reference
		if (p_request == NULL || !p_request->isValid()) {
			throw runtime_error("Matching request is invalid!");
		}
		p_request_ = p_request;
	}
	// Mandatory fields
	rspp_.set_rsc(static_cast<pb::CommonTypes_ResponseStatusCode>(rsc));
	if (!setString(rqi, &pb::ResponsePrim::set_allocated_rqi, rspp_)) {
		throw runtime_error("ResponsePrim(status_code, req_id) failed.");
	}
}

const ResponseStatusCode ResponsePrim::getResponseStatusCode() const {
	return static_cast<ResponseStatusCode>(rspp_.rsc());
}

const string & ResponsePrim::getRequestId() const {
	return rspp_.rqi();
}

bool ResponsePrim::setContent(const string & pc) {
	return setString(pc, &pb::ResponsePrim::set_allocated_pc, rspp_);
}

const string & ResponsePrim::getContent() const {
	return rspp_.pc();
}

bool ResponsePrim::setTo(const string & to) {
	return setString(to, &pb::ResponsePrim::set_allocated_to, rspp_);
}

const string & ResponsePrim::getTo() const {
	return rspp_.to();
}

bool ResponsePrim::setFrom(const string & fr) {
	return setString(fr, &pb::ResponsePrim::set_allocated_fr, rspp_);
}

const string & ResponsePrim::getFrom() const {
	return rspp_.fr();
}

bool ResponsePrim::setOriginatingTimestamp(TimeStamp &ot){  }
bool ResponsePrim::getOriginatingTimestamp(TimeStamp &ot){  }

bool ResponsePrim::setResultExpireTimestamp(TimeStamp &rset){  }
bool ResponsePrim::getResultExpireTimestamp(TimeStamp &rset){  }

bool ResponsePrim::setEventCat(EventCat ec){  }
EventCat ResponsePrim::getEventCat(){  }

bool ResponsePrim::isValid(ValidateType vt) {

	if (getResponseStatusCode() == RSC_NONE ||
		rspp_.rqi().empty()) {
		cerr << "RequestPrim miss request id or response status code." << endl;
		return false;
	}

	if (vt == VALIDATE_COMMON) {
		return true;
	}

	switch (p_request_->getOperation()) {
	case OPERATION_CREATE:
		break;
	case OPERATION_RETRIEVE:
		if (rspp_.pc().empty()) {
			cerr << "RequestPrim miss content." << endl;
			return false;
		}
		break;
	case OPERATION_UPDATE:
		break;
	case OPERATION_DELETE:
		break;
	case OPERATION_NOTIFY:
		break;
	default:
		cerr << "Invalid response operation: " << p_request_->getOperation() << endl;
		return false;
	}

	return true;
}

string ResponsePrim::getJson() {
	return pb2json(rspp_);
}

ResponsePrim::~ResponsePrim() {
/*	if (p_request_ != NULL) {
		delete p_request_;
		p_request_ = NULL;
	} */
}

}	// OneM2M
}	// MicroWireless
