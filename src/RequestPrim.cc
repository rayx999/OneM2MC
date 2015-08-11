/*
 * RequestPrim.cc
 *
 *  Created on: 2015年7月16日
 *      Author: weimi_000
 */

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
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
		return;
	}

	setDefaults();

	if (isValid()) {
		parseIdInfo();
	} else {
		cerr << "RequestPrim in JSON not valid!\n";
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
		return;
	}

	setDefaults();

	if (isValid()) {
		parseIdInfo();
	} else {
		cerr << "RequestPrim validation failed!\n";
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
	return rn_;
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

void RequestPrim::parseIdInfo() {
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

void RequestPrim::getIdInfo(string& domain, string& csi) {
	domain = domain_;
	csi = csi_;
}

bool RequestPrim::isValid(ValidateType vt) {
	if (request_pb_.to().empty() ||
		request_pb_.fr().empty() ||
		request_pb_.rqi().empty() ) {
		cerr << "RequestPrim miss to, from or request id." << endl;
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

string RequestPrim::getJson() {
	return pb2json(request_pb_);
}

}	// OneM2M
}	// MicroWireless
