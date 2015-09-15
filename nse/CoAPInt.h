/*
 * CoAPInt.h - CoAP Interface
 *
 *  Created on: 2015年9月3日
 *      Author: weimi_000
 */

#ifndef NSE_COAPINT_H_
#define NSE_COAPINT_H_

#include "CoAPBinding.pb.h"

using namespace MicroWireless::OneM2M;

class CoAPInt {

public:
	CoAPInt(const std::string& ip, const std::string& port) :
		ip_(ip), port_(port) { }
	virtual void send(pb::CoAPBinding&) { }
	virtual void run() { }
	virtual ~CoAPInt() { }

private:
	const std::string ip_, port_;
};


#endif /* NSE_COAPINT_H_ */
