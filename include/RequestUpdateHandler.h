/*
 * ReuestUpdateHandler.h
 *
 *  Created on: 2015年10月14日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REQUESTUPDATEHANDLER_H_
#define INCLUDE_REQUESTUPDATEHANDLER_H_

#include <string>

#include "CommonTypes.h"
#include "AE.h"
#include "RemoteCSE.h"
#include "RequestPrim.h"
#include "ResourceStore.h"
#include "ResourceBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

template <typename StoreType>
class RequestUpdateHandler {

public:
	RequestUpdateHandler(RequestPrim& reqp, ResourceStore<StoreType>& rdb) :
		reqp_(reqp), rdb_(rdb), p_res_(NULL), p_ret_(NULL), pc_(reqp.getContent()) {
		sp_id_ = rdb.getRoot()->getDomain() + rdb.getRoot()->getCSEId();
	}

	~RequestUpdateHandler() {
		if (p_res_ != NULL) {
			delete p_res_;
			p_res_ = NULL;
		}
		if (p_ret_ != NULL) {
			delete p_ret_;
			p_ret_ = NULL;
		}
	}

	ResponseStatusCode setResourceToBeUpdated() {
		SupportedResourceType ty_;

		if (pc_.empty()) {
			return ResponseStatusCode::BAD_REQUEST;
		} else if (!checkResourceType(ty_)) {
			return ResponseStatusCode::BAD_REQUEST;
		}

		bool result_ = false;
		switch (ty_) {
		case SupportedResourceType::AE:
			result_ = createUpdateResource<AEClass>();
			break;
		case SupportedResourceType::REMOTE_CSE:
			result_ = createUpdateResource<RemoteCSE>();
			break;
		default:
			return ResponseStatusCode::BAD_REQUEST;
		}
		return result_ ? ResponseStatusCode::OK : ResponseStatusCode::BAD_REQUEST;
	}

	bool outToResourceStore() {
		return p_res_->outToResourceStore(rdb_, true);
	}

	bool composeContent(string& pc) {
		// keep all timestamps
		return p_ret_->serializeToString(&pc);
	}

private:
	template <typename ResourceType>
	bool createUpdateResource() {
		ResourceType* p_res = new ResourceType();
		ResourceType* p_ret = new ResourceType();
		p_res_ = (ResourceBase*)p_res;
		p_ret_ = (ResourceBase*)p_ret;
		if (p_ret == NULL || p_res == NULL) {
			return false;
		} else if (!p_res->setResourceBase(reqp_.getTargetResource(), rdb_)) {
			return false;
		} else if (!p_ret->setResourceBase(pc_, sp_id_, Operation::UPDATE)) {
			return false;
		}
		return p_res->updateResource(*p_ret);
	}

	bool checkResourceType(SupportedResourceType& ty) {
		pb::ResourceBase pb_res_;
		if (pb_res_.ParseFromString(pc_)) {
			ty = static_cast<SupportedResourceType>(
				pb_res_.resource_case() - ResourceBase::ResourceBaseOffset);
			return true;
		}
		return false;
	}

private:
	RequestPrim& reqp_;
	ResourceStore<StoreType>& rdb_;
	ResourceBase* p_res_;
	ResourceBase* p_ret_;
	const string& pc_;
	string sp_id_;
};

}	// OneM2M
}	// MicroWireless


#endif /* INCLUDE_REQUESTUPDATEHANDLER_H_ */
