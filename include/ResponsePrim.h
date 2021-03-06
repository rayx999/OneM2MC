/*
 * ResponsePrim.h
 *
 *  Created on: 2015年7月18日
 *      Author: weimi_000
 */

#ifndef INCLUDE_RESPONSEPRIM_H_
#define INCLUDE_RESPONSEPRIM_H_

#include "ResponsePrim.pb.h"
#include "CommonTypes.h"
#include "RequestPrim.h"

using namespace std;
using namespace MicroWireless::OneM2M;

namespace MicroWireless {

namespace OneM2M {

class Request;

class ResponsePrim
{
public:
	ResponsePrim(RequestPrim * p_request, const string &json);
	ResponsePrim(RequestPrim * p_request, ResponseStatusCode rsc, const string& fr);

	const ResponseStatusCode getResponseStatusCode() const;
	const string & getRequestId() const;

	void setContent(const string & pc);
	const string & getContent() const;

	void setTo(const string & to);
	const string & getTo() const;

	void setFrom(const string & fr);
	const string & getFrom() const;

	bool setOriginatingTimestamp(TimeStamp &ot);
	bool getOriginatingTimestamp(TimeStamp &ot);

	bool setResultExpireTimestamp(TimeStamp &rset);
	bool getResultExpireTimestamp(TimeStamp &rset);

	bool setEventCat(EventCat ec);
	EventCat getEventCat();

	bool isValid(ValidateType vt = VALIDATE_COMMON);
	string getJson();

	const pb::ResponsePrim& getRspp() { return rspp_;	}
	~ResponsePrim();

private:
	RequestPrim * p_reqp_;		// matching request.
	pb::ResponsePrim rspp_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_RESPONSEPRIM_H_ */
