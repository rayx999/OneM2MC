/*
 * IApiCallBack.h
 *
 *  Created on: 2015年10月30日
 *      Author: weimi_000
 */

#ifndef INCLUDE_IAPICALLBACK_H_
#define INCLUDE_IAPICALLBACK_H_

#include "CSEApi.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class IApiCallBack {
public:
	IApiCallBack() : cookie_() {}
	IApiCallBack(const std::string& cookie) : cookie_(cookie) {}
	virtual ~IApiCallBack() {}

	const std::string& getCookie() const {
		return cookie_;
	}

	virtual void registerCSECb(const std::string&, const std::string&, ApiStatus, unsigned int) {}

private:
	std::string cookie_;
};

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_IAPICALLBACK_H_ */
