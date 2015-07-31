/*
 * RequestHandler.h
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REQUESTHANDLER_H_
#define INCLUDE_REQUESTHANDLER_H_

#include <string>

#include "CommonTypes.h"
#include "Request.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class Request;
class CSEBase;
class NSEBase;

class RequestHandler {
public:
	RequestHandler(NSEBase& nse) : nse_(nse) { };

	virtual ~RequestHandler() {};

	virtual void handleRequest(Request& request) { };

	template <typename Root>
	ResponseStatusCode isForMe(Request& req, Root& root) {
		ResponseStatusCode rsc_ = RSC_OK;

		if (req.isValid()) {
			string domain_, csi_;
			req.getIdInfo(domain_, csi_);
			if (domain_.compare(root.getDomain()) != 0 ||
				csi_.compare(root.getCSEId()) != 0) {
				rsc_ = RSC_ACCESS_DENIED;
			}
		} else {
			rsc_ = RSC_BAD_REQUEST;
		}

		return rsc_;
	};

protected:
	NSEBase& nse_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_REQUESTHANDLER_H_ */
