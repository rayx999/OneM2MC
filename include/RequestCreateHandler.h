/*
 * ReuestCreateHandler.h
 *
 *  Created on: 2015年8月28日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REQUESTCREATEHANDLER_H_
#define INCLUDE_REQUESTCREATEHANDLER_H_

#include <string>

#include "CommonTypes.h"
#include "AE.h"
#include "RequestPrim.h"
#include "ResourceStore.h"
#include "ResourceBase.h"
//#include "ResourcePtr.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

template <typename StoreType>
class RequestCreateHandler {

public:
	RequestCreateHandler(RequestPrim& reqp, ResourceStore<StoreType>& rdb) :
		reqp_(reqp), rdb_(rdb), p_res_(NULL), p_ret_(NULL), p_par_(NULL), pc_(reqp.getContent()) {
		sp_id_ = rdb.getRoot()->getDomain() + rdb.getRoot()->getCSEId();
	}

	~RequestCreateHandler() {
		if (p_res_ != NULL) {
			delete p_res_;
			p_res_ = NULL;
		}
		if (p_ret_ != NULL) {
			delete p_ret_;
			p_ret_ = NULL;
		}
		if (rdb_.getRoot() != (ResourceBase*)p_par_) {
			delete p_par_;
			p_par_ = NULL;
		}
	}

	ResponseStatusCode setResourceToBeCreated() {
		SupportedResourceType ty_;

		if (pc_.empty()) {
			return RSC_BAD_REQUEST;
		} else if (!checkResourceType(ty_)) {
			return RSC_BAD_REQUEST;
		}

		switch (ty_) {
		case AE:
			p_res_ = new AEClass();
			p_ret_ = new AEClass();
			break;
		default:
			return RSC_BAD_REQUEST;
		}
		if (p_res_ == NULL || p_ret_ == NULL) {
			return RSC_BAD_REQUEST;
		} else if (!p_res_->setResourceBase(pc_, sp_id_, OPERATION_CREATE)) {
			return RSC_BAD_REQUEST;
		}

		string full_path;
		ResponseStatusCode rsc_ = getFullPathToBeCreated(full_path);
		if (rsc_ != RSC_OK) {
			return rsc_;
		} else if (!rdb_.getParentResource(full_path, p_par_)) {
			return RSC_NOT_FOUND;
		}
		string pi_ = p_par_->getResourceId();

		string ri_;
		rdb_.generateResourceId(ty_, ri_);

		string domain_, csi_, rn_;
		parseIds(full_path, CSERegex, domain_, csi_, rn_);
		// no resource name, set resource id as resource name
		if (rn_.empty()) {
			rn_ = ri_;
		}

		bool result_ = false;
		switch (ty_) {
		case AE:
			result_ = ((AEClass*)p_res_)->setNewResourceAttr(ri_, rn_, pi_, *(AEClass*)p_ret_);
			break;
		default:
			return RSC_INTERNAL_SERVER_ERROR;
		}
		if (!result_) {
			return RSC_INTERNAL_SERVER_ERROR;
		}
		return RSC_OK;
	}

	bool outToResourceStore() {
		return p_res_->outToResourceStore(rdb_);
	}

	bool composeContent(string& pc) {
		// keep all timestamps
		p_ret_->CopyResourceTimeStamps(*p_res_);
		return p_ret_->SerializeToString(&pc);
	}

	bool saveParentLastModifiedTime() {
		return
			p_par_->setLastModifiedTimestamp() &&
			p_par_->outToResourceStore(rdb_, true);
	}

private:

	bool checkResourceType(SupportedResourceType& ty) {
		pb::ResourceBase pb_res_;
		if (pb_res_.ParseFromString(pc_)) {
			ty = static_cast<SupportedResourceType>(
				pb_res_.resource_case() - ResourceBase::ResourceBaseOffset);
			return true;
		}
		return false;
	}

	ResponseStatusCode getFullPathToBeCreated(string& full_path) {
		full_path = sp_id_;
		if (!reqp_.getIntRn().empty()) {
			if (rdb_.isResourceValid(reqp_.getTo())) {
				return RSC_CONFLICT;
			} else {
				full_path += "/" + reqp_.getIntRn();
			}
		} else if (!reqp_.getName().empty()) {
			if (rdb_.isResourceValid(sp_id_ + "/" + reqp_.getName())) {
				return RSC_CONFLICT;
			} else {
				full_path += "/" + reqp_.getName();
			}
		}
		return RSC_OK;
	}

private:
	RequestPrim& reqp_;
	ResourceStore<StoreType>& rdb_;
	ResourceBase* p_res_;
	ResourceBase* p_ret_;
	ResourceBase* p_par_;
	const string& pc_;
	string sp_id_;
};

}	// OneM2M
}	// MicroWireless


#endif /* INCLUDE_REQUESTCREATEHANDLER_H_ */