/*
 * CSEApi.h
 *
 *  Created on: 2015年10月26日
 *      Author: weimi_000
 */

#ifndef CSE_CSEAPI_H_
#define CSE_CSEAPI_H_

#include <string>
#include <boost/function.hpp>

#include "CommonTypes.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

enum class ApiStatus {
	ACCEPTED 	= 0,
	SUCCESS		= 1,
	FAILED		= 2,

	REMOTE_CSE_CREATE_REQ_SENT 	= 10,
	REMOTE_CSE_CREATE_RSP_RECV	= 11,
	REMOTE_CSE_RETRIEVE_REQ_SENT = 12,
	REMOTE_CSE_RETRIEVE_RSP_RECV = 13,
};

class CSEResourceStore;
class NSEBase;
class CSEHandler;
class IApiCallBack;

namespace CSEApi {

void setCSEApi(CSEResourceStore&, NSEBase&, CSEHandler&);
bool registerCSE(const std::string&, IApiCallBack*);

}	// CSEApi
}	// OneM2M
}	// MicroWireless

#endif /* CSE_CSEAPI_H_ */
