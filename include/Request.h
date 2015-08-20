/*
 * Request.h
 *
 *  Created on: 2015年8月16日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REQUEST_H_
#define INCLUDE_REQUEST_H_

#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Request.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"
#include "RequestPrim.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;

class Request : public ResourceBase {

public:
	Request(RequestPrim& reqp, const string& ri, const string& pi);

	Operation getOperation();
	const string& getTarget();
	const string& getOriginator();
	const string& getRequestId();
	const string& getContent();
	RequestStatus getRequestStatus();
	bool setRequestStatus(RequestStatus rs);

	bool getOperationResult(pb::OperationResult& ol);
	bool setOperationResult(pb::OperationResult* ol);
	bool getOperationResult_rsc(ResponseStatusCode& rsc);
	bool setOperationResult_rsc(ResponseStatusCode  rsc);
	bool getOperationResult_to(string& to);
	bool setOperationResult_to(string& to);
	bool getOperationResult_fr(string& fr);
	bool setOperationResult_fr(string& fr);
	bool getOperationResult_rqi(string& rqi);
	bool setOperationResult_rqi(string& rqi);
	bool getOperationResult_pc(string& pc);
	bool setOperationResult_pc(string& pc);
	bool getOperationResult_ot(TimeStamp& ot);
	bool setOperationResult_ot(TimeStamp& ot);
	bool getOperationResult_rset(TimeStamp& rset);
	bool setOperationResult_rset(TimeStamp& rset);
	bool getOperationResult_ec(EventCat& ec);
	bool setOperationResult_ec(EventCat& ec);

protected:

private:

private:
	pb::Request* p_req_;
	pb::MetaInfo* p_mi_;
	pb::OperationResult* p_ol_;
};

}	// OneM2M

}	// MicroWireless



#endif /* INCLUDE_REQUEST_H_ */
