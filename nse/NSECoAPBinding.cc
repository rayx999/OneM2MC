/*
 * NSE_CoAP.cc
 *
 *  Created on: 2015年8月31日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include <bitset>

#include "CoAPBinding.pb.h"
#include "NSECoAPBinding.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

using google::protobuf::Reflection;
using google::protobuf::FieldDescriptor;
using google::protobuf::RepeatedPtrField;

map<int, int> NSE_CoAP::rsc2coap_ = {
		{ 1000,   0 },    // ACCEPTED
		{ 2000, 205 },    // OK
		{ 2001, 201 },    // CREATED                                      Created
		{ 2002, 202 },    // DELETED                                      Deleted
		{ 2004, 204 },    // CHANGED                                      Changed
		{ 4000, 400 },    // BAD_REQUEST                                  Bad Request
		{ 4004, 404 },    // NOT_FOUND                                    Not Found
		{ 4005, 405 },    // Operation::NOT_ALLOWED                        Method Not Allowed
		{ 4008, 404 },    // REQUEST_TIMEOUT                              Not Found
		{ 4101, 403 },    // SUBSCRIPTION_CREATOR_HAS_NO_PRIVILEGE–       Forbidden
		{ 4102, 400 },    // CONTENTS_UNACCEPTABLE                        Bad Request
		{ 4103, 403 },    // ACCESS_DENIED                                Forbidden
		{ 4104, 400 },    // GROUP_REQUEST_IDENTIFIER_EXISTS              Bad Request
		{ 4105, 403 },    // CONFLICT                                     Forbidden
		{ 5000, 500 },    // INTERNAL_SERVER_ERROR                        Internal Server Error
		{ 5001, 501 },    // NOT_IMPLEMENTED                              Not Implemented
		{ 5103, 404 },    // TARGET_NOT_REACHABLE                         Not Found
		{ 5105, 403 },    // NO_PRIVILEGE                                 Forbidden
		{ 5106, 400 },    // ALREADY_EXISTS                               Bad Request
		{ 5203, 403 },    // TARGET_NOT_SUBSCRIBABLE                      Forbidden
		{ 5204, 500 },    // SUBSCRIPTION_VERIFICATION_INITIATION_FAILED  Internal Server Error
		{ 5205, 403 },    // SUBSCRIPTION_HOST_HAS_NO_PRIVILEGE           Forbidden
		{ 5206, 500 },    // NON_BLOCKING_REQUEST_NOT_SUPPORTED           Internal Server Error
		{ 6003, 404 },    // EXTENAL_OBJECT_NOT_REACHABLE                 Not Found
		{ 6005, 404 },    // EXTENAL_OBJECT_NOT_FOUND                     Not Found
		{ 6010, 400 },    // MAX_NUMBERF_OF_MEMBER_EXCEEDED               Bad Request
		{ 6011, 400 },    // MEMBER_TYPE_INCONSISTENT                     Bad Request
		{ 6020, 500 },    // MGMT_SESSION_CANNOT_BE_ESTABLISHED           Internal Server Error
		{ 6021, 500 },    // MGMT_SESSION_ESTABLISHMENT _TIMEOUT          Internal Server Error
		{ 6022, 400 },    // INVALID _CMDTYPE                             Bad Request
		{ 6023, 400 },    // INVALID_ARGUMENTS                            Bad Request
		{ 6024, 400 },    // INSUFFICIENT_ARGUMENTS                       Bad Request
		{ 6025, 500 },    // MGMT_CONVERSION_ERROR                        Internal Server Error
		{ 6026, 500 },    // MGMT_CANCELATION_FAILURE                     Internal Server Error
		{ 6028, 400 },    // ALREADY_COMPLETE                             Bad Request
		{ 6029, 400 }     // COMMAND_NOT_CANCELLABLE                      Bad Request
};

void NSE_CoAP::addOpt(pb::CoAPBinding& coap, pb::CoAPTypes_OptionType type, const string& value) {
	coap.add_opt();
	pb::CoAPOption* p_opt_ = coap.mutable_opt(coap.opt_size() - 1);
	p_opt_->set_num(type);
	p_opt_->set_value(value);
}

void NSE_CoAP::addOpt(pb::CoAPBinding& coap, pb::CoAPTypes_OptionType type, uint value, int base) {
	coap.add_opt();
	pb::CoAPOption* p_opt_ = coap.mutable_opt(coap.opt_size() - 1);
	p_opt_->set_num(type);
	switch (base) {
	case 16:
		p_opt_->set_value(bitset<16>(value).to_string());
		break;
	case 32:
		p_opt_->set_value(bitset<32>(value).to_string());
		break;
	case 64:
		p_opt_->set_value(bitset<64>(value).to_string());
		break;
	default:
		cerr << "addOpt: Unknown base:" << base << endl;
	}
}

bool NSE_CoAP::getOpt(const pb::CoAPBinding& coap, pb::CoAPTypes_OptionType type,
		const pb::CoAPOption*& p_opt) {
	const RepeatedPtrField<pb::CoAPOption>& opts_ = coap.opt();
	for (auto i = opts_.begin(); i != opts_.end(); i++) {
		if (i->num() == type) {
			p_opt = &(*i);
			return true;
		}
	}
	return false;
}

bool NSE_CoAP::convertCoAPRequest(const pb::CoAPBinding& coap, RequestPrim*& p_reqp) {

	switch (coap.type()) {
	case pb::CoAPTypes_MessageType_CoAP_CON:
	case pb::CoAPTypes_MessageType_CoAP_NON:
			break;
	default:
		cerr << "convertCoAPRequest:Invalid message type:" << coap.type() << endl;
		return false;
	}

	Operation op_;
	string to_, fr_, rqi_;

	switch (static_cast<pb::CoAPTypes_MethodType>(coap.code())) {
	case pb::CoAPTypes_MethodType_CoAP_POST:
		op_ = Operation::CREATE;  // Operation::NOTIFY ??
		break;
	case pb::CoAPTypes_MethodType_CoAP_GET:
		op_ = Operation::RETRIEVE;
		break;
	case pb::CoAPTypes_MethodType_CoAP_PUT:
		op_ = Operation::UPDATE;
		break;
	case pb::CoAPTypes_MethodType_CoAP_DELETE:
		op_ = Operation::DDELETE;
		break;
	default:
		cerr << "convertCoAPRequest:Invalid Method type:" << coap.code() << endl;
		return false;
	}

	// parse all options
	const RepeatedPtrField<pb::CoAPOption>& opts_ = coap.opt();
	for (auto i = opts_.begin(); i != opts_.end(); i++) {
		switch (i->num()) {
		case pb::CoAPTypes_OptionType_CoAP_If_Match:
		case pb::CoAPTypes_OptionType_CoAP_Uri_Host:
		case pb::CoAPTypes_OptionType_CoAP_ETag:
		case pb::CoAPTypes_OptionType_CoAP_If_None_Match:
		case pb::CoAPTypes_OptionType_CoAP_Uri_Port:
		case pb::CoAPTypes_OptionType_CoAP_Location_Path:
			break;
		case pb::CoAPTypes_OptionType_CoAP_Uri_Path:
			to_ = i->value();
			break;
		case pb::CoAPTypes_OptionType_CoAP_Content_Format:
		case pb::CoAPTypes_OptionType_CoAP_Max_Age:
		case pb::CoAPTypes_OptionType_CoAP_Uri_Query:
		case pb::CoAPTypes_OptionType_CoAP_Accept:
		case pb::CoAPTypes_OptionType_CoAP_Location_Query:
		case pb::CoAPTypes_OptionType_CoAP_Proxy_Uri:
		case pb::CoAPTypes_OptionType_CoAP_Proxy_Scheme:
		case pb::CoAPTypes_OptionType_CoAP_Size1:
			break;
		case pb::CoAPTypes_OptionType_ONEM2M_FR:
			fr_ = i->value();
			break;
		case pb::CoAPTypes_OptionType_ONEM2M_RQI:
			rqi_ = i->value();
			break;
		case pb::CoAPTypes_OptionType_ONEM2M_NM:
		case pb::CoAPTypes_OptionType_ONEM2M_OT:
		case pb::CoAPTypes_OptionType_ONEM2M_RQET:
		case pb::CoAPTypes_OptionType_ONEM2M_RSET:
		case pb::CoAPTypes_OptionType_ONEM2M_OET:
		case pb::CoAPTypes_OptionType_ONEM2M_RTURI:
		case pb::CoAPTypes_OptionType_ONEM2M_EC:
		case pb::CoAPTypes_OptionType_ONEM2M_RSC:
		case pb::CoAPTypes_OptionType_ONEM2M_GID:
			break;
		default:
			cerr << "convertCoAPRequest: Unknown option:" << i->num() << endl;
			break;
		}
	}

	if (to_.empty() || fr_.empty() || rqi_.empty()) {
		cerr << "convertCoAPRequest: mandatory fields missing:\n";
		cerr << "  to: " << to_ << endl;
		cerr << "  fr: " << fr_ << endl;
		cerr << "  rqi:" << rqi_ << endl;
		return false;
	}
	try {
		p_reqp = new RequestPrim(op_, to_, fr_, rqi_);
	} catch (exception &e) {
		cerr << "convertCoAPRequest: RequestPrim Ctor faild:";
		cerr << e.what() << endl;
		return false;
	}

	if (!coap.payload().empty()) {
		p_reqp->setContent(coap.payload());
	}
	return true;
}

void NSE_CoAP::send(ResponsePrim& rsp, const string& addr, uint port) {
	pb::CoAPBinding coap_;
	coap_.set_ver(1);
	coap_.set_type(pb::CoAPTypes_MessageType_CoAP_ACK);

	int rsc_ = static_cast<int>(rsp.getResponseStatusCode());
	if (rsc2coap_.find(rsc_) == rsc2coap_.end()) {
		cerr << "Send: RSC:" << rsc_ << " Not found.\n";
		return;
	}
	coap_.set_code(static_cast<pb::CoAPTypes_ResponseCode>(rsc2coap_[rsc_]));

	// Add CoAP Option CoAP_Uri_Host
	addOpt(coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Host, addr);
	// Add CoAP Option CoAP_Uri_Port 16 bit
	addOpt(coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Port, port, 16);
	// Add CoAP Option CoAP_Uri_Path
	addOpt(coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Path, rsp.getTo());
	// Add CoAP Option CoAP_Location_Path
	//if (!rsp.getContent().rn().empty()) {
	//	addOpt(coap_, pb::CoAPTypes_OptionType_CoAP_Location_Path, rsp.getFrom());
	//}

	// Add CoAP Option ONEM2M_FR
	addOpt(coap_, pb::CoAPTypes_OptionType_ONEM2M_FR, rsp.getFrom());
	// Add CoAP Option ONEM2M_RQI
	addOpt(coap_, pb::CoAPTypes_OptionType_ONEM2M_RQI, rsp.getRequestId());

	//coap_send(coap_);
}

void NSE_CoAP::run() {
	NSEBase::run();
}

}	// OneM2M

}	// MicroWireless



