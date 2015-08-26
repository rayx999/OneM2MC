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
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/format.hpp>
#include <json2pb.h>

#include "CommonTypes.h"
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
	RequestHandler(NSEBase& nse) : nse_(nse) {
		gen_.seed(static_cast<long int>(time(0)));
	};

	virtual ~RequestHandler() {};

	bool composeContent(ResourceBase& res_pc, ResourceBase& res, string& pc);

	template <typename StoreType>
	bool checkRequest(RequestPrim& req, StoreType& rdb) {
		ResponseStatusCode rsc_ = isForMe(req, *rdb.cse());
		string pc_;

		if (rsc_ == RSC_OK) {
			ResponseType rt_ = req.getResponseType();
			switch (rt_) {
			case NON_BLOCKING_REQUEST_SYNC:
			case NON_BLOCKING_REQUEST_ASYNC:
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
			nse_.send(rsp_);
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

	template <typename StoreType>
	bool composeContent(RequestPrim& req, string& pc, ResourceStore<StoreType>& rdb) {
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

	template <typename StoreType>
	ResponseStatusCode setResourceToBeCreated(ResourceBase& res, RequestPrim& reqp,
			ResourceStore<StoreType>& rdb, ResourceBase& ret, ResourceBase& parent)
	{
		const string sp_id_ = rdb.getRoot()->getDomain() + rdb.getRoot()->getCSEId();
		const string pc_ = reqp.getContent();
		string full_path = sp_id_;

		if (pc_.empty()) {
			return RSC_BAD_REQUEST;
		} else if (!res.setResourceBase(pc_, sp_id_)) {
			return RSC_BAD_REQUEST;
		} else if (!reqp.getIntRn().empty()) {
			if (rdb.isResourceValid(reqp.getTo())) {
				return RSC_CONFLICT;
			} else {
				full_path += "/" + reqp.getIntRn();
			}
		} else if (!reqp.getName().empty()) {
			if (rdb.isResourceValid(sp_id_ + "/" + reqp.getName())) {
				return RSC_CONFLICT;
			} else {
				full_path += "/" + reqp.getName();
			}
		} else if (!res.getResourceName().empty()) {
			if (rdb.isResourceValid(sp_id_ + "/" + res.getResourceName())) {
				return RSC_CONFLICT;
			} else {
				full_path += "/" + res.getResourceName();
			}
		}

		// set parent id, default to CSEBase
		if (!rdb.getParentResource(full_path, parent)) {
			return RSC_NOT_FOUND;
		}
		string pi_ = parent.getResourceId();
		res.setParentId(pi_);
		ret.setParentId(pi_);

		// generate ri and rn
		string ri_;
		generateResourceId(res.getResourceType(), ri_, rdb);
		res.setResourceId(ri_);
		ret.setResourceId(ri_);

		string domain_, csi_, rn_;
		parseIds(full_path, CSERegex, domain_, csi_, rn_);
		// no resource name, set resource id as resource name
		if (rn_.empty()) {
			rn_ = ri_;
		}
		if (rn_ != res.getResourceName()) {
			res.setResourceName(rn_);
			ret.setResourceName(rn_);
		}
		return RSC_OK;
	}

protected:
	template <typename StoreType>
	bool createRequest(RequestPrim& reqp, string& pc, StoreType &rdb) {
		string ri_;
		generateResourceId(REQUEST, ri_, rdb);

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

	template <typename StoreType>
	void generateResourceId(SupportedResourceType ty, string& ri, StoreType& rdb) {
		boost::random::uniform_int_distribution<> dist(1, 99999);
		do {
			ri = boost::str(boost::format("%03d-%05d") % ty % dist(gen_));
		} while (rdb.isResourceValid(ri));

		ri = (ty == AE ? string("C") + ri : ri);
	}

protected:
	NSEBase& nse_;
	boost::random::mt19937 gen_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_REQUESTHANDLER_H_ */
