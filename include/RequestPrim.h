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

	RequestPrim();
	RequestPrim(const string &json);
	RequestPrim(Operation op, const string & to, const string & fr, const string & rqi);

	Operation getOperation() const;

	bool setResourceType(SupportedResourceType ty);
	SupportedResourceType getResourceType() const;

	const string & getTo() const;
	bool setTo(const string& to);

	const string & getFrom() const;
	void setFrom(const string& fr);

	const string & getRequestId() const;
	void setRequestId(const string& rqi);

	const string & getTargetResource() const;
	void setTargetResource(const string& target);

	void setName(const string &nm);
	const string& getName() const;

	void setContent(const string & pc);
	const string& getContent() const;

	bool setOriginatingTimestamp(TimeStamp &ot);
	bool getOriginatingTimestamp(TimeStamp &ot);

	bool setRequestExpireTimestamp(TimeStamp &rqet);
	bool getRequestExpireTimestamp(TimeStamp &rqet);

	bool setResultExpireTimestamp(TimeStamp &rset);
	bool getResultExpireTimestamp(TimeStamp &rset);

	bool setOperationExecutionTime(Duration &oet);
	bool getOperationExecutionTime(Duration &oet);

	void setResponseType(ResponseType rt);
	ResponseType getResponseType() const;

	bool setResultPersistence(Duration &rp);
	bool getResultPersistence(Duration &rp);

	bool setResultContent(ResultContent rcn);
	ResultContent getResultContent() const;

	bool setEventCat(EventCat ec);
	EventCat getEventCat() const;

	bool setDeliveryAggregation(bool da);
	bool getDeliveryAggregation();

	bool setGroupRequestId(string &gid);
	bool getGroupRequestId(string &gid);

	//bool setFilterCriteria(FilterCriteria & fc);
	//bool getFilterCriteria(FilterCriteria & fc);

	bool setDiscoveryResultType(DiscoveryResultType drt);
	DiscoveryResultType getDiscoveryResultType() const;

	void getIdInfo(string& domain, string& csi);
	const string& getIntRn() const;
	bool isValid(ValidateType vt = VALIDATE_COMMON);
	string getJson();
	void swap(RequestPrim&);

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
