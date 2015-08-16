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
	request_pb_.set_rcn(static_cast<pb::CommonTypes_ResultContent>(RESULT_CONTENT_ATTRIBUTES));
}

RequestPrim::RequestPrim(const string & json) {
	// parse to PB buffer
	try {
		json2pb(request_pb_, json.c_str(), json.length());
	} catch (exception &e) {
		cerr << "Json2pb exception: " << e.what() << endl;
		throw runtime_error("RequestPrim(json) Ctor failed.");
	}

	setDefaults();

	if (!isValid()) {
		cerr << "RequestPrim validation failed!\n";
		throw runtime_error("RequestPrim(json) Ctor failed.");
	}

	if (!parseIds(request_pb_.to(), CSERegex, domain_, csi_, rn_)) {
		cerr << "RequestPrim parseIds failed. req.to: ";
		cerr << request_pb_.to() << endl;
		throw runtime_error("RequestPrim(json) Ctor failed.");
	}
}

RequestPrim::RequestPrim(Operation op, const string & to, const string & fr, const string & rqi) {

	// Mandatory fields
	try {
		request_pb_.set_op(static_cast<pb::CommonTypes_Operation>(op));
		setString(to, &pb::RequestPrim::set_allocated_to, request_pb_);
		setString(fr, &pb::RequestPrim::set_allocated_fr, request_pb_);
		setString(rqi, &pb::RequestPrim::set_allocated_rqi, request_pb_);
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

const string & RequestPrim::getTo() {
	return request_pb_.to();
}

const string & RequestPrim::getFrom() {
	return request_pb_.fr();
}

const string & RequestPrim::getRequestId() {
	return request_pb_.rqi();
}

const string & RequestPrim::getTargetResource() {
	return target_;
}

void RequestPrim::setTargetResource(const string& target) {
	target_ = target;
}

Operation RequestPrim::getOperation() {
	return static_cast<Operation>(request_pb_.op());
}

bool RequestPrim::setResourceType(ResourceType ty) {
	request_pb_.set_ty(static_cast<pb::CommonTypes_ResourceType>(ty));
	return true;
}

ResourceType RequestPrim::getResourceType() {
	return static_cast<ResourceType>(request_pb_.ty());
}

/*
bool setName(string &nm);
bool getName(string &nm);

bool setContent(string & pc);
bool getContent(string & pc);

bool setOriginatingTimestamp(TimeStamp &ot);
bool getOriginatingTimestamp(TimeStamp &ot);

bool setRequestExpireTimestamp(TimeStamp &rqet);
bool getRequestExpireTimestamp(TimeStamp &rqet);

bool setResultExpireTimestamp(TimeStamp &rset);
bool getResultExpireTimestamp(TimeStamp &rset);

bool setOperationExecutionTime(Duration &oet);
bool getOperationExecutionTime(Duration &oet);

bool setResponseType(ResponseType rt);
ResponseType getResponseType();

bool setResultPersistence(Duration &rp);
bool getResultPersistence(Duration &rp);
*/
bool RequestPrim::setResultContent(ResultContent rcn) {
	request_pb_.set_rcn(static_cast<pb::CommonTypes_ResultContent>(rcn));
	return true;
}

ResultContent RequestPrim::getResultContent() {
	return static_cast<ResultContent>(request_pb_.rcn());
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

const string& RequestPrim::getIntRn() {
	return rn_;
}

bool RequestPrim::isValid(ValidateType vt) {
	if (request_pb_.to().empty() ||
		request_pb_.fr().empty() ||
		request_pb_.rqi().empty() ) {
		cerr << "RequestPrim miss to, from or request id." << endl;
		return false;
	}

/*	if (vt == VALIDATE_COMMON) {
		return true;
	}
*/
	switch (static_cast<Operation>(request_pb_.op())) {
	case OPERATION_CREATE:
		break;
	case OPERATION_RETRIEVE:
		if (request_pb_.ty() != pb::CommonTypes_ResourceType_RESOURCE_TYPE_NONE ||
			!request_pb_.nm().empty()) {
			cerr << "Retrieve request has resource type or name fields." << endl;
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
		cerr << "Invalid request operation: " << request_pb_.op() << endl;
		return false;
	}

	return true;
}

string RequestPrim::getJson() {
	return pb2json(request_pb_);
}

}	// OneM2M
}	// MicroWireless
