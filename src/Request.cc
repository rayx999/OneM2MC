/*
 * Request.cc
 *
 *  Created on: 2015年7月16日
 *      Author: weimi_000
 */

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <json2pb.h>

#include "Request.pb.h"
#include "CommonUtils.h"
#include "Request.h"

using namespace std;
using namespace MicroWireless::OneM2M;

namespace MicroWireless {

namespace OneM2M {

Request::Request(const string & json) {
	// parse to PB buffer
	json2pb(request_pb_, json.c_str(), json.length());

	if (!isValid()) {
		throw runtime_error("Request in JSON not valid!");
	}

	parseIdInfo();
}

Request::Request(Operation op, const string & to, const string & fr, const string & rqi) {
	if (to.empty() || fr.empty() || rqi.empty()) {
		throw runtime_error("Request mandatory fields to, from, or requestId missing!");
	}
	// Mandatory fields
	request_pb_.set_op(static_cast<pb::CommonTypes_Operation>(op));
	setString(to, &pb::Request::set_allocated_to, request_pb_);
	setString(fr, &pb::Request::set_allocated_fr, request_pb_);
	setString(rqi, &pb::Request::set_allocated_rqi, request_pb_);

	if (!isValid()) {
		throw runtime_error("Request constructor failed!");
	}

	parseIdInfo();
}

const string & Request::getTo() {
	return request_pb_.to();
}

const string & Request::getFrom() {
	return request_pb_.fr();
}

const string & Request::getRequestId() {
	return request_pb_.rqi();
}

const string & Request::getTargetResource() {
	return rn_;
}

Operation Request::getOperation() {
	return static_cast<Operation>(request_pb_.op());
}

bool Request::setResourceType(ResourceType ty) {
	request_pb_.set_ty(static_cast<pb::CommonTypes_ResourceType>(ty));
	return true;
}

ResourceType Request::getResourceType() {
	return static_cast<ResourceType>(request_pb_.ty());
}

#if 0
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

bool setResultContent(ResultContent rcn);
ResultContent getResultContent();

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

#endif

void Request::parseIdInfo() {
	// to = //domain/cseid/resource, case insensitive
	static boost::regex pattern_("(^//[\\.\\w-]+)(/[\\w-]+)(?:/([\\w-]+))?");
	boost::smatch sm;

	boost::regex_match(getTo(), sm, pattern_);
	switch (sm.size()) {
	case 4:
	case 3: rn_ = sm[3];
	case 2: csi_ = sm[2];
	case 1: domain_ = sm[1];
			break;
	default:
		cerr << "regex match error on " << getTo() << endl;
	}
}

void Request::getIdInfo(string& domain, string& csi) {
	domain = domain_;
	csi = csi_;
}

bool Request::isValid(ValidateType vt) {
	if (request_pb_.to().empty() ||
		request_pb_.fr().empty() ||
		request_pb_.rqi().empty() ) {
		cerr << "Request miss to, from or request id." << endl;
		return false;
	}

	if (vt == VALIDATE_COMMON) {
		return true;
	}

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

string Request::getJson() {
	return pb2json(request_pb_);
}

}	// OneM2M
}	// MicroWireless
