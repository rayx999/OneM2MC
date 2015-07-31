/*
 * NSEBase.h
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#ifndef INCLUDE_NSEBASE_H_
#define INCLUDE_NSEBASE_H_

#include <string>

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class Response;
class RequestHandler;

class NSEBase {
public:
	NSEBase(const std::string& ip, const std::string& port)
		: ip_(ip), port_(port) { } ;

	void setRequestHandler(RequestHandler& req_hdl) {
		request_handler_ = &req_hdl;
	};

	virtual void send(Response& rsp) { };
	virtual void run() { };

	virtual ~NSEBase() { };

private:
	RequestHandler * request_handler_;
	const std::string ip_, port_;
};

}	// OneM2M

}	// MicroWireless

#endif /* INCLUDE_NSEBASE_H_ */
