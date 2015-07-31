/*
 * Response.h
 *
 *  Created on: 2015年7月18日
 *      Author: weimi_000
 */

#ifndef INCLUDE_RESPONSE_H_
#define INCLUDE_RESPONSE_H_

#include "Response.pb.h"
#include "CommonTypes.h"
#include "Request.h"

using namespace std;
using namespace MicroWireless::OneM2M;

namespace MicroWireless {

namespace OneM2M {

class Response
{
public:
	Response(Request * p_request, const string &json);
	Response(Request * p_request, ResponseStatusCode rsc, const string & rqi);

	const ResponseStatusCode getResponseStatusCode() const;
	const string & getRequestId();

	bool setContent(const string & pc);
	const string & getContent();

	bool setTo(const string & to);
	const string & getTo();

	bool setFrom(const string & fr);
	const string & getFrom();

	bool setOriginatingTimestamp(TimeStamp &ot);
	bool getOriginatingTimestamp(TimeStamp &ot);

	bool setResultExpireTimestamp(TimeStamp &rset);
	bool getResultExpireTimestamp(TimeStamp &rset);

	bool setEventCat(EventCat ec);
	EventCat getEventCat();

	bool isValid(ValidateType vt = VALIDATE_COMMON);
	string getJson();

	~Response();

private:
	Request * p_request_;		// matching request.
	pb::Response response_pb_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_RESPONSE_H_ */
