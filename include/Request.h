/*
 * Request.h
 *
 *  Created on: 2015年7月16日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REQUEST_H_
#define INCLUDE_REQUEST_H_

#include "Request.pb.h"
#include "CommonTypes.h"
#include "CommonUtils.h"

using namespace std;
using namespace MicroWireless::OneM2M;

namespace MicroWireless {

namespace OneM2M {

class Request
{
public:
	Request(const string &json);
	Request(Operation op, const string & to, const string & fr, const string & rqi);

	Operation getOperation();

	bool setResourceType(ResourceType ty);
	ResourceType getResourceType();

	const string & getTo();
	const string & getFrom();
	const string & getRequestId();
	const string & getTargetResource();

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

	void getIdInfo(string& domain, string& csi);
	bool isValid(ValidateType vt = VALIDATE_COMMON);
	string getJson();

private:
	void parseIdInfo();

private:
	pb::Request request_pb_;
	// parsed fields from getTo()
	string domain_, csi_, rn_;
};
/*
class Retrieve : public Request
{

};
*/

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_REQUEST_H_ */
