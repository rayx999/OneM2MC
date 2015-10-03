/*
 * NSE_CoAP.cc
 *
 *  Created on: 2015年8月31日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

#include "CoAPBinding.pb.h"
#include "NSECoAPBinding.h"
#include "CommonUtils.h"
#include "RequestPrim.h"
#include "ResponsePrim.h"
#include "CoAPInt.h"

namespace MicroWireless {
namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

using google::protobuf::Reflection;
using google::protobuf::FieldDescriptor;
using google::protobuf::RepeatedPtrField;

// CoAP response code > 1000, need to add ONEM2M_RSC option
map<int, int> NSE_CoAP::rsc2coap_ = {
		{ 1000,    0 },    // ACCEPTED
		{ 2000,  205 },    // OK
		{ 2001,  201 },    // CREATED                                      Created
		{ 2002,  202 },    // DELETED                                      Deleted
		{ 2004,  204 },    // CHANGED                                      Changed
		{ 4000, 1400 },    // BAD_REQUEST                                  Bad Request
		{ 4004, 1404 },    // NOT_FOUND                                    Not Found
		{ 4005,  405 },    // Operation::NOT_ALLOWED                        Method Not Allowed
		{ 4008, 1404 },    // REQUEST_TIMEOUT                              Not Found
		{ 4101, 1403 },    // SUBSCRIPTION_CREATOR_HAS_NO_PRIVILEGE–       Forbidden
		{ 4102, 1400 },    // CONTENTS_UNACCEPTABLE                        Bad Request
		{ 4103, 1403 },    // ACCESS_DENIED                                Forbidden
		{ 4104, 1400 },    // GROUP_REQUEST_IDENTIFIER_EXISTS              Bad Request
		{ 4105, 1403 },    // CONFLICT                                     Forbidden
		{ 5000, 1500 },    // INTERNAL_SERVER_ERROR                        Internal Server Error
		{ 5001,  501 },    // NOT_IMPLEMENTED                              Not Implemented
		{ 5103, 1404 },    // TARGET_NOT_REACHABLE                         Not Found
		{ 5105, 1403 },    // NO_PRIVILEGE                                 Forbidden
		{ 5106, 1400 },    // ALREADY_EXISTS                               Bad Request
		{ 5203, 1403 },    // TARGET_NOT_SUBSCRIBABLE                      Forbidden
		{ 5204, 1500 },    // SUBSCRIPTION_VERIFICATION_INITIATION_FAILED  Internal Server Error
		{ 5205, 1403 },    // SUBSCRIPTION_HOST_HAS_NO_PRIVILEGE           Forbidden
		{ 5206, 1500 },    // NON_BLOCKING_REQUEST_NOT_SUPPORTED           Internal Server Error
		{ 6003, 1404 },    // EXTENAL_OBJECT_NOT_REACHABLE                 Not Found
		{ 6005, 1404 },    // EXTENAL_OBJECT_NOT_FOUND                     Not Found
		{ 6010, 1400 },    // MAX_NUMBERF_OF_MEMBER_EXCEEDED               Bad Request
		{ 6011, 1400 },    // MEMBER_TYPE_INCONSISTENT                     Bad Request
		{ 6020, 1500 },    // MGMT_SESSION_CANNOT_BE_ESTABLISHED           Internal Server Error
		{ 6021, 1500 },    // MGMT_SESSION_ESTABLISHMENT _TIMEOUT          Internal Server Error
		{ 6022, 1400 },    // INVALID _CMDTYPE                             Bad Request
		{ 6023, 1400 },    // INVALID_ARGUMENTS                            Bad Request
		{ 6024, 1400 },    // INSUFFICIENT_ARGUMENTS                       Bad Request
		{ 6025, 1500 },    // MGMT_CONVERSION_ERROR                        Internal Server Error
		{ 6026, 1500 },    // MGMT_CANCELATION_FAILURE                     Internal Server Error
		{ 6028, 1400 },    // ALREADY_COMPLETE                             Bad Request
		{ 6029, 1400 }     // COMMAND_NOT_CANCELLABLE                      Bad Request
};

void NSE_CoAP::addOpt(pb::CoAPBinding& coap, pb::CoAPTypes_OptionType type, const string& value) {
	coap.add_opt();
	pb::CoAPOption* p_opt_ = coap.mutable_opt(coap.opt_size() - 1);
	p_opt_->set_num(type);
	p_opt_->set_value(value);
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

void NSE_CoAP::convertUriQuery(const string& query, RequestPrim& reqp) {
	map<string, string> queries_;
	getPairs(queries_, query, "&", "=");

	for (auto q : queries_) {
		if (q.first == "rt") {
			unsigned int rt_ = boost::lexical_cast<unsigned int>(q.second);
			reqp.setResponseType(static_cast<ResponseType>(rt_));
		} else {
			cerr << "NSE_CoAP::convertUriQuery: unknown query "
					<< q.first << "=" << q.second << endl;
		}
	}
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
	switch (coap.method()) {
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
	string to_, fr_, rqi_, nm_, uri_query_;
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
			uri_query_ = i->value();
			break;
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
			nm_ = i->value();
			break;
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

	if (!uri_query_.empty()) {
		convertUriQuery(uri_query_, *p_reqp);
	}
	if (!nm_.empty()) {
		p_reqp->setName(nm_);
	}
	if (!coap.payload().empty()) {
		p_reqp->setContent(coap.payload());
	}
	return true;
}

void NSE_CoAP::send_request(RequestPrim& reqp, const string& addr, uint port) {

}

void NSE_CoAP::send_response(ResponsePrim& rsp, const string& addr, uint port) {

	//cout << "NSE_CoAP::send： Response rsc:" << (int)rsp.getResponseStatusCode() << endl;

	pb::CoAPBinding coap_;
	coap_.set_ver(1);
	coap_.set_type(pb::CoAPTypes_MessageType_CoAP_ACK);

	int rsc_ = static_cast<int>(rsp.getResponseStatusCode());
	if (rsc2coap_.find(rsc_) == rsc2coap_.end()) {
		cerr << "Send: RSC:" << rsc_ << " Not found.\n";
		return;
	}
	// ACCEPTED match an empty ACK COAP response
	if (rsc_ != (int)ResponseStatusCode::ACCEPTED) {
		int code_ = rsc2coap_[rsc_];
		if (code_ > 1000) {
			code_ -= 1000;
			// add ONEM2M_RSC option
			addOpt(coap_, pb::CoAPTypes_OptionType_ONEM2M_RSC,
					boost::lexical_cast<string>(rsc_));
		}
		coap_.set_code(static_cast<pb::CoAPTypes_ResponseCode>(code_));
	}
	// Add CoAP Option CoAP_Uri_Host
	addOpt(coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Host, addr);
	// Add CoAP Option CoAP_Uri_Port 16 bit
	addOpt(coap_, pb::CoAPTypes_OptionType_CoAP_Uri_Port, boost::lexical_cast<string>(port));
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

	// set content if any
	if (!rsp.getContent().empty()) {
		coap_.set_payload(rsp.getContent());
	}

	coap_int_.send(coap_);
}

void NSE_CoAP::run() {
	NSEBase::run();
	coap_int_.run();
}

}	// OneM2M

}	// MicroWireless



