/*
 * RequestPrim.cc
 *
 *  Created on: 2015年7月16日
 *      Author: weimi_000
 */

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <json2pb.h>

#include "RequestPrim.pb.h"
#include "CommonUtils.h"
#include "RequestPrim.h"

using namespace std;
using namespace MicroWireless::OneM2M;

namespace MicroWireless {

namespace OneM2M {

void RequestPrim::setDefaults() {
	reqp_.set_rcn(static_cast<pb::CommonTypes_ResultContent>(ResultContent::ATTRIBUTES));
}

RequestPrim::RequestPrim() { }

RequestPrim::RequestPrim(const string & json) {
	// parse to PB buffer
	try {
		json2pb(reqp_, json.c_str(), json.length());
	} catch (exception &e) {
		cerr << "Json2pb exception: " << e.what() << endl;
		throw runtime_error("RequestPrim(json) Ctor failed.");
	}

	setDefaults();

	if (!isValid()) {
		cerr << "RequestPrim validation failed!\n";
		throw runtime_error("RequestPrim(json) Ctor failed.");
	}

	if (!parseIds(reqp_.to(), CSERegex, domain_, csi_, rn_)) {
		cerr << "RequestPrim parseIds failed. req.to: ";
		cerr << reqp_.to() << endl;
		throw runtime_error("RequestPrim(json) Ctor failed.");
	}
}

RequestPrim::RequestPrim(Operation op, const string & to, const string & fr, const string & rqi) {

	// Mandatory fields
	try {
		reqp_.set_op(static_cast<pb::CommonTypes_Operation>(op));
		setString(to, &pb::RequestPrim::set_allocated_to, reqp_);
		setString(fr, &pb::RequestPrim::set_allocated_fr, reqp_);
		setString(rqi, &pb::RequestPrim::set_allocated_rqi, reqp_);
	} catch (exception &e) {
		cerr << "PB exception: " << e.what() << endl;
		throw runtime_error("RequestPrim() Ctor failed.");
	}

	setDefaults();

	if (!isValid()) {
		cerr << "RequestPrim validation failed!\n";
		throw runtime_error("RequestPrim() Ctor failed.");;
	}

	if (!parseIds(to, CSERegex, domain_, csi_, rn_)) {
		cerr << "RequestPrim parseIds failed. req.to: ";
		cerr << to << endl;
		throw runtime_error("RequestPrim() Ctor failed.");;
	}
}

const string & RequestPrim::getTo() const {
	return reqp_.to();
}

bool  RequestPrim::setTo(const string& to) {
	string domain_, csi_, rn_;
	if (parseIds(to, CSERegex, domain_, csi_, rn_)) {
		reqp_.set_to(to);
		RequestPrim::domain_ = domain_;
		RequestPrim::csi_ = csi_;
		RequestPrim::rn_ = rn_;
		return true;
	} else {
		cerr << "setTo failed.\n";
		return false;
	}
}

const string & RequestPrim::getFrom() const {
	return reqp_.fr();
}

void  RequestPrim::setFrom(const string& fr) {
	reqp_.set_fr(fr);
}

const string & RequestPrim::getRequestId() const {
	return reqp_.rqi();
}

void  RequestPrim::setRequestId(const string& rqi) {
	reqp_.set_rqi(rqi);
}

const string & RequestPrim::getTargetResource() const {
	return target_;
}

void RequestPrim::setTargetResource(const string& target) {
	target_ = target;
}

Operation RequestPrim::getOperation() const {
	return static_cast<Operation>(reqp_.op());
}

bool RequestPrim::setResourceType(SupportedResourceType ty) {
	reqp_.set_ty(static_cast<pb::CommonTypes_SupportedResourceType>(ty));
	return true;
}

SupportedResourceType RequestPrim::getResourceType() const {
	return static_cast<SupportedResourceType>(reqp_.ty());
}


void  RequestPrim::setName(const string& nm) {
	reqp_.set_nm(nm);
}

const string&  RequestPrim::getName() const {
	return reqp_.nm();
}

void RequestPrim::setContent(const string & pc) {
	reqp_.set_pc(pc);
}

const string& RequestPrim::getContent() const {
	return reqp_.pc();
}

/*
bool setOriginatingTimestamp(TimeStamp &ot);
bool getOriginatingTimestamp(TimeStamp &ot);

bool setRequestExpireTimestamp(TimeStamp &rqet);
bool getRequestExpireTimestamp(TimeStamp &rqet);

bool setResultExpireTimestamp(TimeStamp &rset);
bool getResultExpireTimestamp(TimeStamp &rset);

bool setOperationExecutionTime(Duration &oet);
bool getOperationExecutionTime(Duration &oet);
*/
void RequestPrim::setResponseType(ResponseType rt) {
	reqp_.set_rt(static_cast<pb::CommonTypes_ResponseType>(rt));
}

ResponseType RequestPrim::getResponseType() const {
	return static_cast<ResponseType>(reqp_.rt());
}

/*
bool setResultPersistence(Duration &rp);
bool getResultPersistence(Duration &rp);
*/
bool RequestPrim::setResultContent(ResultContent rcn) {
	reqp_.set_rcn(static_cast<pb::CommonTypes_ResultContent>(rcn));
	return true;
}

ResultContent RequestPrim::getResultContent() const {
	return static_cast<ResultContent>(reqp_.rcn());
}

/*
bool setEventCat(EventCat ec);
EventCat getEventCat();

bool setDeliveryAggregation(bool da);
bool getDeliveryAggregation();

bool setGroupRequestId(string &gid);
bool getGroupRequestId(string &gid);

//bool setFilterCriteria(FilterCriteria & fc);
//bool getFilterCriteria(FilterCriteria & fc);

bool setDiscoveryResultType(DiscoveryResultType drt);
DiscoveryResultType getDiscoveryResultType();

*/

void RequestPrim::getIdInfo(string& domain, string& csi) {
	domain = domain_;
	csi = csi_;
}

const string& RequestPrim::getIntRn() const {
	return rn_;
}

bool RequestPrim::isValid(ValidateType vt) {
	if (reqp_.to().empty() ||
		reqp_.fr().empty() ||
		reqp_.rqi().empty() ) {
		cerr << "RequestPrim miss to, from or request id." << endl;
		return false;
	}

/*	if (vt == VALIDATE_COMMON) {
		return true;
	}
*/
	switch (static_cast<Operation>(reqp_.op())) {
	case Operation::CREATE:
		break;
	case Operation::RETRIEVE:
		if (reqp_.ty() != pb::CommonTypes_SupportedResourceType_SUPPORTED_RESOURCE_TYPE_NONE ||
			!reqp_.nm().empty()) {
			cerr << "Retrieve request has resource type or name fields." << endl;
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
		cerr << "Invalid request operation: " << reqp_.op() << endl;
		return false;
	}

	return true;
}

string RequestPrim::getJson() {
	return pb2json(reqp_);
}

void RequestPrim::swap(RequestPrim& reqp) {
	reqp_.Swap(&reqp.reqp_);
	domain_.swap(reqp.domain_);
	csi_.swap(reqp.csi_);
	rn_.swap(reqp.rn_);
	target_.swap(reqp.target_);
}

}	// OneM2M
}	// MicroWireless
