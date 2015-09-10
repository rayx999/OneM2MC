/*
 * NSECoAPBinding.h
 *
 *  Created on: 2015年8月31日
 *      Author: weimi_000
 */

#ifndef NSE_NSECOAPBINDING_H_
#define NSE_NSECOAPBINDING_H_

#include <string>

#include "CoAPBinding.pb.h"
#include "NSEBase.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class ResponsePrim;

class NSE_CoAP : public NSEBase {
public:
	NSE_CoAP(const std::string& ip, const std::string& port) : NSEBase(ip, port) { }
	virtual ~NSE_CoAP() { }

	virtual void send(ResponsePrim& rsp, const std::string& addr, uint port);
	virtual void run();

	bool convertCoAPRequest(const pb::CoAPBinding&, RequestPrim*& p_reqp);

private:
	void addOpt(pb::CoAPBinding&, pb::CoAPTypes_OptionType, const std::string&);
	void addOpt(pb::CoAPBinding&, pb::CoAPTypes_OptionType, uint, int);
	bool getOpt(const pb::CoAPBinding&, pb::CoAPTypes_OptionType, const pb::CoAPOption*&);
	static std::map<int, int> rsc2coap_;
};

}	// OneM2M

}	// MicroWireless

#endif /* NSE_NSECOAPBINDING_H_ */