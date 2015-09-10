/*
 * CoAPInt.h
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

	virtual void coap_send(pb::CoAPBinding&) { }

	virtual void run() { }

};


#endif /* NSE_COAPINT_H_ */
