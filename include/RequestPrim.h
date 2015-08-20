/*
 * RequestPrim.h
 *
 *  Created on: 2015年7月16日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REQUESTPRIM_H_
#define INCLUDE_REQUESTPRIM_H_

#include "RequestPrim.pb.h"
#include "CommonTypes.h"
#include "CommonUtils.h"

using namespace std;
using namespace MicroWireless::OneM2M;

namespace MicroWireless {

namespace OneM2M {

class Request;

class RequestPrim
{
public:
	friend class Request;

	RequestPrim(const string &json);
	RequestPrim(Operation op, const string & to, const string & fr, const string & rqi);

	Operation getOperation();

	bool setResourceType(SupportedResourceType ty);
	SupportedResourceType getResourceType();

	const string & getTo();
	const string & getFrom();
	const string & getRequestId();

	const string & getTargetResource();
	void setTargetResource(const string& target);

	bool setName(string &nm);
	bool getName(string &nm);

	void setContent(const string & pc);
	const string& getContent();

	bool setOriginatingTimestamp(TimeStamp &ot);
	bool getOriginatingTimestamp(TimeStamp &ot);

	bool setRequestExpireTimestamp(TimeStamp &rqet);
	bool getRequestExpireTimestamp(TimeStamp &rqet);

	bool setResultExpireTimestamp(TimeStamp &rset);
	bool getResultExpireTimestamp(TimeStamp &rset);

	bool setOperationExecutionTime(Duration &oet);
	bool getOperationExecutionTime(Duration &oet);

	void setResponseType(ResponseType rt);
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

	void getIdInfo(string& domain, string& csi);
	const string& getIntRn();
	bool isValid(ValidateType vt = VALIDATE_COMMON);
	string getJson();

private:
	void setDefaults();

private:
	pb::RequestPrim reqp_;
	// parsed fields from getTo()
	string domain_, csi_, rn_, target_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_REQUESTPRIM_H_ */
