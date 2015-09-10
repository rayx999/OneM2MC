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

ResponsePrim::ResponsePrim(RequestPrim *p_reqp, const string &json) {
	// keep original request reference
	if (p_reqp == NULL || !p_reqp->isValid()) {
		throw runtime_error("Matching request is invalid!");
	}
	p_reqp_ = p_reqp;

	// parse to PB buffer
	json2pb(rspp_, json.c_str(), json.length());

	if (!isValid()) {
		throw runtime_error("ResponsePrim in JSON not valid!");
	}
}

ResponsePrim::ResponsePrim(RequestPrim *p_reqp, ResponseStatusCode rsc, const string& fr){
	if (fr.empty()) {
		throw runtime_error("ResponsePrim::ResponsePrim: From field is empty!");
	}
	if (rsc != ResponseStatusCode::BAD_REQUEST) {
		// keep original request reference
		if (p_reqp == NULL || !p_reqp->isValid()) {
			throw runtime_error("ResponsePrim::ResponsePrim: Matching request is invalid!");
		}
		p_reqp_ = p_reqp;
	}
	// Mandatory fields
	rspp_.set_rsc(static_cast<pb::CommonTypes_ResponseStatusCode>(rsc));
	rspp_.set_rqi(p_reqp->getRequestId());
	setTo(p_reqp->getFrom());
	setFrom(fr);
}

const ResponseStatusCode ResponsePrim::getResponseStatusCode() const {
	return static_cast<ResponseStatusCode>(rspp_.rsc());
}

const string & ResponsePrim::getRequestId() const {
	return rspp_.rqi();
}

void ResponsePrim::setContent(const string & pc) {
	rspp_.set_pc(pc);
}

const string & ResponsePrim::getContent() const {
	return rspp_.pc();
}

void ResponsePrim::setTo(const string & to) {
	return rspp_.set_to(to);
}

const string & ResponsePrim::getTo() const {
	return rspp_.to();
}

void ResponsePrim::setFrom(const string & fr) {
	return rspp_.set_fr(fr);
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

	if (getResponseStatusCode() == ResponseStatusCode::NONE ||
		rspp_.rqi().empty()) {
		cerr << "RequestPrim miss request id or response status code." << endl;
		return false;
	}

	if (vt == VALIDATE_COMMON) {
		return true;
	}

	switch (p_reqp_->getOperation()) {
	case Operation::CREATE:
		break;
	case Operation::RETRIEVE:
		if (rspp_.pc().empty()) {
			cerr << "RequestPrim miss content." << endl;
			return false;
		}
		break;
	case Operation::UPDATE:
		break;
	case Operation::DDELETE:
		break;
	case Operation::NOTIFY:
		break;
	default:
		cerr << "Invalid response operation: " <<
		static_cast<int>(p_reqp_->getOperation()) << endl;
		return false;
	}

	return true;
}

string ResponsePrim::getJson() {
	return pb2json(rspp_);
}

ResponsePrim::~ResponsePrim() {
/*	if (p_reqp_ != NULL) {
		delete p_reqp_;
		p_reqp_ = NULL;
	} */
}

}	// OneM2M
}	// MicroWireless
