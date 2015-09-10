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
#include "AE.h"
#include "Request.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"
#include "ResourceStore.h"
#include "ResourceBase.h"
#include "NSEBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

class RequestHandler {
public:
	RequestHandler(NSEBase& nse) : nse_(nse) {}

	virtual ~RequestHandler() {}

	bool composeContent(ResourceBase& res_pc, ResourceBase& res, string& pc);

	template <typename StoreType>
	bool checkRequest(RequestPrim& req, StoreType& rdb) {
		ResponseStatusCode rsc_ = isForMe(req, *rdb.cse());
		string pc_;

		if (rsc_ == RSC_OK) {
			ResponseType rt_ = req.getResponseType();
			switch (rt_) {
			case ResponseType::NON_BLOCKING_REQUEST_SYNC:
			case ResponseType::NON_BLOCKING_REQUEST_ASYNC:
				if (createRequest(req, pc_, rdb)) {
					rsc_ = RSC_ACCEPTED;
				} else {
					cerr << "handleRequest: createRequest failed.\n";
					rsc_ = RSC_INTERNAL_SERVER_ERROR;
				}
				break;
			default:
				break;
			}
		}

		if (rsc_ != RSC_OK) {
			string fr_ = rdb.getRoot()->getDomain() + rdb.getRoot()->getCSEId();
			ResponsePrim rsp_(&req, rsc_, fr_);
			if (!pc_.empty()) {
				rsp_.setContent(pc_);
			}
			nse_.send(rsp_, "127.0.1", 5555);
		}

		return (rsc_ == RSC_OK);
}

	template <typename Root>
	void getResourceHAddress(RequestPrim& req, string& addr, Root& root) {
		addr = root.getDomain() + root.getCSEId() + "/" + req.getIntRn();
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

	template <typename Root>
	bool checkResourceType(const string& pc, SupportedResourceType& ty, Root& root) {
		pb::ResourceBase res_;

		if (res_.ParseFromString(pc)) {
			return false;
		}

		ty = static_cast<SupportedResourceType>(res_.resource_case() - ResourceBase::ResourceBaseOffset);
		return root.isResourceSupported(ty) && ty != SupportedResourceType::REQUEST;
	}

	template <typename StoreType>
	bool composeContent(RequestPrim& req, string& pc, ResourceStore<StoreType>& rdb) {
		bool ret_ = false;
		ResourceBase base_;
		if (!base_.setResourceBase(req.getTargetResource(), rdb)) {
			cerr << "setResourceBase(" << req.getTargetResource() <<") failed.\n";
			return false;
		}

		switch (req.getResultContent()) {
		case ResultContent::ATTRIBUTES:
			//pc = pb2json(res);
			// return true;
			return base_.SerializeToString(&pc);
		default:
			cerr << "serializeContent: Unknown ResultContent:"
			<< static_cast<int>(req.getResultContent()) << endl;
			break;
		}

		return ret_;
	}

protected:
	template <typename StoreType>
	bool createRequest(RequestPrim& reqp, string& pc, StoreType &rdb) {
		string ri_;
		rdb.generateResourceId(SupportedResourceType::REQUEST, ri_);

		// set a resource to hold ri as response content
		ResourceBase res_;
		res_.setResourceId(ri_);
		res_.SerializeToString(&pc);

		Request req_(reqp, ri_, rdb.getRoot()->getResourceId());
		if (req_.getResourceName().empty()) {
			req_.setResourceName(ri_);
		}
		if (!req_.outToResourceStore(rdb)) {
			cerr << "createRequest: outToResourceStore failed.\n";
			return false;
		}
		return true;
	}

protected:
	NSEBase& nse_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_REQUESTHANDLER_H_ */
