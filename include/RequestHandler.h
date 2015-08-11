/*
 * RequestHandler.h
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REQUESTHANDLER_H_
#define INCLUDE_REQUESTHANDLER_H_

#include <string>
#include <json2pb.h>

#include "CommonTypes.h"
#include "RequestPrim.h"
#include "ResourceStore.h"
#include "ResourceBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class RequestPrim;
class CSEBase;
class NSEBase;

class RequestHandler {
public:
	RequestHandler(NSEBase& nse) : nse_(nse) { };

	virtual ~RequestHandler() {};

	virtual void handleRequest(RequestPrim& request) { };

	template <typename Root>
	ResponseStatusCode isForMe(RequestPrim& req, Root& root) {
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

	template <typename StoreType>
	bool composeContent(RequestPrim& req, string& pc, StoreType& rdb) {
		bool ret_ = false;
		ResourceBase base_;
		if (!base_.setResourceBase(req.getTo(), rdb)) {
			cerr << "setResourceBase(" << req.getTo() <<") failed.\n";
			return false;
		}
		SupportedResourceType srt_ = base_.getResourceBase();
		switch (srt_) {
		case CSE_BASE:
			ret_ = serializeContent(base_.getCSEBase(), pc, req.getResultContent());
			break;
		default:
			cerr << "composeContent: Unrecognozed resource: " << srt_ << endl;
			break;
		}

		return ret_;
		/*
		static const string json("{"
						"\"ty\" 	: 1,"
						"\"ri\" 	: \"//microwireless.com/IN-CSE-00/CSEBASE\","
						"\"rn\" 	: \"CSEBASE\","
						"\"ct\" 	: { \"seconds\" : 1435434103 },"
						"\"cst\" 	: 1,"
						"\"csi\" 	: \"/IN-CSE-01\","
						"\"srt\" 	: [ 2, 5, 16 ]"
					"}");

		return &json;
*/
	}

protected:
	template <typename ResourceType>
	bool serializeContent(ResourceType& res, string& pc, ResultContent rcn) {
		switch (rcn) {
		case RESULT_CONTENT_ATTRIBUTES:
			//pc = pb2json(res);
			// return true;
			return res.SerializeToString(&pc);
		default:
			cerr << "serializeContent: Unknown ResultContent:" << rcn << endl;
			break;
		}
		return false;
	}

protected:
	NSEBase& nse_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_REQUESTHANDLER_H_ */
