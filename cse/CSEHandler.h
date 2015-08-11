/*
 * CSEHandler.h
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#ifndef CSE_CSEHANDLER_H_
#define CSE_CSEHANDLER_H_

#include "CommonTypes.h"
#include "CSEBase.h"
#include "CSEResourceStore.h"
#include "RequestHandler.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class NSEBase;
//class ResourceStore<CSEBase>;
class RequestPrim;
class ResponsePrim;

class CSEHandler : public RequestHandler {

public:
	CSEHandler(NSEBase& nse, CSEResourceStore& rdb)
		: RequestHandler(nse), rdb_(rdb) {};

	void handleRequest(RequestPrim& request);

private:
	CSEResourceStore& rdb_;
};

}	// OneM2M

}	// MicroWireless

#endif /* CSE_CSEHANDLER_H_ */
