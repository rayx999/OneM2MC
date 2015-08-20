/*
 * Request.cc
 *
 *  Created on: 2015年8月16日
 *      Author: weimi_000
 */


#include <stdexcept>
#include <iostream>
#include "google/protobuf/timestamp.pb.h"
#include "google/protobuf/duration.pb.h"
#include "CommonTypes.pb.h"
#include "ResourceBase.pb.h"
#include "Request.pb.h"
#include "CommonTypes.h"
#include "CommonUtils.h"
#include "Request.h"
#include "RequestPrim.h"
#include "ResourceBase.h"

using namespace std;
using namespace MicroWireless::OneM2M::pb;
using ::google::protobuf::Timestamp;
//using ::google::protobuf::Duration;

namespace MicroWireless {

namespace OneM2M {

Request::Request(RequestPrim& reqp, const string& ri, const string& pi) {
	// construct common fields
	base_.set_ty(pb::CommonTypes_SupportedResourceType_REQUEST);
	base_.set_ri(ri);
	// Request resource name: Request/rqi
	base_.set_rn("Request/" + reqp.getRequestId());
	base_.set_pi(pi);
	setCreateTimestamp();
	setLastModifiedTimestamp();
	//Timestamp et_(max(reqp.reqp_.rqet(), reqp.reqp_.rset()));
	*base_.mutable_et() = max(reqp.reqp_.rqet(), reqp.reqp_.rset());
	// acpi
	// lbl
	// st

	// construct request specific fields
	p_req_ = new pb::Request();
	p_req_->set_op(reqp.reqp_.op());
	p_req_->set_tg(reqp.reqp_.to());
	p_req_->set_og(reqp.reqp_.fr());
	p_req_->set_rid(reqp.reqp_.rqi());

	// set metainfo
	p_mi_ = new pb::MetaInfo();
	p_mi_->set_ty(reqp.reqp_.ty());
	p_mi_->set_nm(reqp.reqp_.nm());
	if (reqp.reqp_.has_ot())
		*p_mi_->mutable_ot() = reqp.reqp_.ot();
	if (reqp.reqp_.has_rqet())
		*p_mi_->mutable_rqet() = reqp.reqp_.rqet();
	if (reqp.reqp_.has_rset())
		*p_mi_->mutable_rset() = reqp.reqp_.rset();
	if (reqp.reqp_.has_oet())
		*p_mi_->mutable_oet() = reqp.reqp_.oet();
	p_mi_->set_rt(reqp.reqp_.rt());
	if (reqp.reqp_.has_rp())
		*p_mi_->mutable_rp() = reqp.reqp_.rp();
	p_mi_->set_rcn(reqp.reqp_.rcn());
	p_mi_->set_ec(reqp.reqp_.ec());
	p_mi_->set_da(reqp.reqp_.da());
	p_mi_->set_gid(reqp.reqp_.gid());
	if (reqp.reqp_.has_fc())
		*p_mi_->mutable_fc() = reqp.reqp_.fc();
	p_mi_->set_drt(reqp.reqp_.drt());

	p_req_->set_allocated_mi(p_mi_);

	p_req_->set_pc(reqp.reqp_.pc());
	p_req_->set_rs(pb::CommonTypes_RequestStatus_REQUEST_STATUS_PENDING);
	//p_req_->set_allocated_ol();

	base_.set_allocated_req(p_req_);
	// Operation Result empty
	p_ol_ = NULL;

	if (!checkResourceConsistency(ri)) {
		throw runtime_error("Request Ctor failed.");
	}

}

Operation Request::getOperation() {
	return static_cast<Operation>(p_req_->op());
}

const string & Request::getTarget() {
	return p_req_->tg();
}

const string & Request::getOriginator() {
	return p_req_->og();
}

const string & Request::getRequestId() {
	return p_req_->rid();
}

const string & Request::getContent() {
	return p_req_->pc();
}

RequestStatus Request::getRequestStatus() {
	return static_cast<RequestStatus>(p_req_->rs());
}

bool Request::setRequestStatus(RequestStatus rs) {
	p_req_->set_rs(static_cast<pb::CommonTypes_RequestStatus>(rs));
	return setLastModifiedTimestamp();
}

bool Request::getOperationResult(pb::OperationResult& ol) {
	if (p_req_->has_ol() && p_ol_ != NULL) {
		ol = *p_ol_;
		return true;
	}
	return false;
}

bool Request::setOperationResult(pb::OperationResult* ol) {
	if (ol == NULL) {
		cerr << "Request::setOperationResult failed. ol is NULL\n";
		return false;
	}
	p_req_->set_allocated_ol(ol);
	p_ol_ = ol;
	return true;
}

bool Request::getOperationResult_rsc(ResponseStatusCode& rsc) {
	if (p_ol_ != NULL) {
		rsc = static_cast<ResponseStatusCode>(p_ol_->rsc());
		return true;
	}
	return false;
}

bool Request::setOperationResult_rsc(ResponseStatusCode rsc) {
	if (p_ol_ != NULL) {
		p_ol_->set_rsc(static_cast<pb::CommonTypes_ResponseStatusCode>(rsc));
		return setLastModifiedTimestamp();
	}
	return false;
}

bool Request::getOperationResult_to(string& to) {
	if (p_ol_ != NULL) {
		to = p_ol_->to();
		return true;
	}
	return false;
}

bool Request::setOperationResult_to(string& to) {
	if (p_ol_ != NULL) {
		p_ol_->set_to(to);
		return setLastModifiedTimestamp();
	}
	return false;
}

bool Request::getOperationResult_fr(string& fr) {
	if (p_ol_ != NULL) {
		fr = p_ol_->fr();
		return true;
	}
	return false;
}

bool Request::setOperationResult_fr(string& fr) {
	if (p_ol_ != NULL) {
		p_ol_->set_fr(fr);
		return setLastModifiedTimestamp();
	}
	return false;
}

bool Request::getOperationResult_rqi(string& rqi) {
	if (p_ol_ != NULL) {
		rqi = p_ol_->rqi();
		return true;
	}
	return false;
}

bool Request::setOperationResult_rqi(string& rqi) {
	if (p_ol_ != NULL) {
		p_ol_->set_rqi(rqi);
		return setLastModifiedTimestamp();
	}
	return false;
}

bool Request::getOperationResult_pc(string& pc) {
	if (p_ol_ != NULL) {
		pc = p_ol_->pc();
		return true;
	}
	return false;
}

bool Request::setOperationResult_pc(string& pc) {
	if (p_ol_ != NULL) {
		p_ol_->set_pc(pc);
		return setLastModifiedTimestamp();
	}
	return false;
}

}	// OneM2M

}	// MicroWireless
