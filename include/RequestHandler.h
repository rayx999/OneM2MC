/*
 * RequestHandler.h
 *
 *  Created on: 2015年7月21日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REQUESTHANDLER_H_
#define INCLUDE_REQUESTHANDLER_H_

#include <string>
#include <boost/algorithm/string/predicate.hpp>
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
	void getResourceHAddress(RequestPrim& req, string& addr, Root& root) {
		addr =  root.getDomain() + root.getCSEId() + "/" + req.getIntRn();
	}

	template <typename Root>
	ResponseStatusCode isForMe(RequestPrim& req, Root& root) {
		if (!req.isValid()) {
			return RSC_BAD_REQUEST;
		}

		string domain_, csi_;
		req.getIdInfo(domain_, csi_);
		if (!domain_.empty() && !boost::iequals(domain_, root.getDomain())) {
			return RSC_ACCESS_DENIED;
		}

		if (!boost::iequals(csi_, root.getCSEId())) {
			return RSC_ACCESS_DENIED;
		}

		return RSC_OK;
	};

	template <typename StoreType>
	bool composeContent(RequestPrim& req, string& pc, StoreType& rdb) {
		bool ret_ = false;
		ResourceBase base_;
		if (!base_.setResourceBase(req.getTargetResource(), rdb)) {
			cerr << "setResourceBase(" << req.getTargetResource() <<") failed.\n";
			return false;
		}

		switch (req.getResultContent()) {
		case RESULT_CONTENT_ATTRIBUTES:
			//pc = pb2json(res);
			// return true;
			return base_.SerializeToString(&pc);
		default:
			cerr << "serializeContent: Unknown ResultContent:" << req.getResultContent() << endl;
			break;
		}

		return ret_;
	}

protected:
	NSEBase& nse_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_REQUESTHANDLER_H_ */
