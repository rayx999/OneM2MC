/*
 * CSEApi.cc
 *
 *  Created on: 2015年10月26日
 *      Author: weimi_000
 */
#include <iostream>
#include <string>
#include <boost/bind.hpp>

#include "CSEServer.h"
#include "CSEResourceStore.h"
#include "CSEHandler.h"
#include "NSEBase.h"
#include "IApiCallBack.h"
#include "ApiCallCache.h"
#include "CommonTypes.h"
#include "CSEApi.h"
#include "CSEBase.h"
#include "RemoteCSE.h"
#include "RequestPrim.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

namespace CSEApi {

namespace {

CSEResourceStore* p_rdb_ = NULL;
CSEHandler* p_hdl_ = NULL;
NSEBase* p_nse_ = NULL;
ApiCallCache<IApiCallBack*>* p_cache_ = NULL;
std::string self_;

void postRegisterCSE2(const RequestPrim& reqp, const std::string& call_id,
		const ResponsePrim& rspp)	{
	IApiCallBack* p_cb_ = NULL;
	if (!p_cache_->getCallback(call_id, p_cb_) || p_cb_ == NULL) {
		std::cerr << "PostRegisterCSE2: getCallback failed on call_id:" <<
			call_id << std::endl;
		return;
	}

	if (rspp.getResponseStatusCode() != ResponseStatusCode::OK) {
		std::cerr << "PostRegisterCSE2: TRIEVE RemoteCSE failed on call_id:" <<
					call_id << std::endl;
		std::cerr << " Response Code:" <<
					(int) rspp.getResponseStatusCode() << std::endl;
		p_cb_->registerCSECb(p_cb_->getCookie(),call_id, ApiStatus::FAILED, (unsigned int)rspp.getResponseStatusCode());
		return;
	}

	p_cb_->registerCSECb(p_cb_->getCookie(),call_id, ApiStatus::REMOTE_CSE_RETRIEVE_RSP_RECV, 0);

	bool ret_ = false;
	do {
		// parse content in response, this is CSEBase from remote
		CSEBase csb_;
		if (!csb_.setResourceBase(rspp)) {
			std::cerr << "PostRegisterCSE1: setResourceBase failed\n";
			break;
		}
		RemoteCSE csr_(csb_);

		std::string ri_;
		p_rdb_->generateResourceId(csr_.getResourceType(), ri_);
		std::string pi_ = p_rdb_->getRoot()->getResourceId();
		std::string rn_ = csr_.getResourceName();
		if (rn_.empty() || p_rdb_->isResourceValid(self_ + "/" + rn_)) {
			rn_ = ri_;
		}
		if (!csr_.setNewAttr(ri_, rn_, pi_)) {
			std::cerr << "PostRegisterCSE1: setNewAttr failed\n";
			break;
		}
		ret_ = csr_.outToResourceStore(*p_rdb_);
	} while (0);

	p_cb_->registerCSECb(p_cb_->getCookie(),call_id, ret_ ? ApiStatus::SUCCESS : ApiStatus::FAILED, 0);
}

void postRegisterCSE1(const RequestPrim& reqp, const std::string& call_id,
		const ResponsePrim& rspp)	{
	IApiCallBack* p_cb_ = NULL;
	if (!p_cache_->getCallback(call_id, p_cb_) || p_cb_ == NULL) {
		std::cerr << "PostRegisterCSE1: getCallback failed on call_id:" <<
			call_id << std::endl;
		return;
	}

	if (rspp.getResponseStatusCode() != ResponseStatusCode::CREATED) {
		std::cerr << "PostRegisterCSE1: CREATE RemoteCSE failed on call_id:" <<
					call_id << std::endl;
		std::cerr <<" Response Code:" <<
					(int) rspp.getResponseStatusCode() << std::endl;
		p_cb_->registerCSECb(p_cb_->getCookie(),call_id, ApiStatus::FAILED, (unsigned int)rspp.getResponseStatusCode());
		return;
	}

	p_cb_->registerCSECb(p_cb_->getCookie(),call_id, ApiStatus::REMOTE_CSE_CREATE_RSP_RECV, 0);

	// retrieve full RemoteCSE
	bool ret_ = false;
	std::string call_id_;

	do {
		// parse content in response
		pb::ResourceBase csr_;
		if (!csr_.ParseFromString(rspp.getContent())) {
			std::cerr << "PostRegisterCSE1: ParseFromString failed\n";
			break;
		}
		if (csr_.pi().empty()) {
			std::cerr << "PostRegisterCSE1: Response missing pi\n";
			break;
		}
		std::string to_ = rspp.getFrom() + "/" + csr_.pi(); // this is remote CSEBase

		p_cache_->getApiCallId(call_id_);
		call_id_ = p_rdb_->getRoot()->getCSEId() + call_id_;
		if (!p_cache_->insert(call_id_, p_cb_)) {
			std::cerr << "PostRegisterCSE1: ApiCache.insert failed\n";
			break;
		}

		// build and send csr RETRIEVE message
		ret_ = p_hdl_->sendRequest(Operation::RETRIEVE,
				to_, self_, call_id_, "",
				boost::bind(&postRegisterCSE2, _1, _2, _3));
		if (ret_) {
			p_cb_->registerCSECb(p_cb_->getCookie(),call_id_, ApiStatus::REMOTE_CSE_RETRIEVE_REQ_SENT, 0);
		}
	} while (0);
	if (!ret_) {
		p_cb_->registerCSECb(p_cb_->getCookie(),call_id_, ApiStatus::FAILED, 0);
		p_cache_->remove(call_id_);
	}
}

}

void setCSEApi(CSEResourceStore& rdb, NSEBase& nse, CSEHandler& hdl) {
	p_rdb_ = &rdb;
	p_nse_ = &nse;
	p_hdl_ = &hdl;
	p_cache_ = &ApiCallCache<IApiCallBack*>::getApiCallCache();

	self_ = p_rdb_->getRoot()->getDomain() + p_rdb_->getRoot()->getCSEId();
}

bool registerCSE(const std::string& to, IApiCallBack* p_cb) {
	bool ret_ = false;
	std::string call_id_;

	do {
		if (p_cb == NULL) {
			std::cerr << "registerCSE: invalid IApiCallBack\n";
			break;
		}

		p_cache_->getApiCallId(call_id_);
		call_id_ = p_rdb_->getRoot()->getCSEId() + call_id_;
		if (!p_cache_->insert(call_id_, p_cb)) {
			std::cerr << "RegisterCSE: ApiCache.insert failed\n";
			break;
		}

		p_cb->registerCSECb(p_cb->getCookie(), call_id_, ApiStatus::ACCEPTED, 0);

		// build csr content
		RemoteCSE csr_(*p_rdb_->getRoot());
		csr_.clrNotPresentAttrs();

		std::string csr_pc_;
		if (!csr_.serializeToString(&csr_pc_)) {
			std::cerr << "RegisterCSE: csr_.SerializeToString failed\n";
			break;
		}
		// build and send csr CREATE message
		ret_ = p_hdl_->sendRequest(Operation::CREATE, to, self_, call_id_,
				csr_pc_, boost::bind(&postRegisterCSE1, _1, _2, _3));
		if (ret_) {
			p_cb->registerCSECb(p_cb->getCookie(), call_id_, ApiStatus::REMOTE_CSE_CREATE_REQ_SENT, 0);
		}
	} while (0);

	if (!ret_) {
		p_cb->registerCSECb(p_cb->getCookie(), call_id_, ApiStatus::FAILED, 0);
		p_cache_->remove(call_id_);
	}
	return ret_;
}

}	// CSEApi
}	// OneM2M
}	// MicroWireless
