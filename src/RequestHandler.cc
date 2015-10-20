/*
 * RequestHandler.cc
 *
 *  Created on: 2015年7月22日
 *      Author: weimi_000
 */


#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/format.hpp>

#include "ResourceBase.pb.h"
#include "NSEBase.h"
#include "ResourceBase.h"
#include "RequestHandler.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

void RequestHandler::sendResponse(RequestPrim& reqp, ResponseStatusCode rsc,
		const string& fr, const string& pc) {
	//cout << "CSEHandler::handleRequest: compose fr:" << fr_ << endl;
	ResponsePrim rsp_(&reqp, rsc, fr);
	if (!pc.empty()) {
		rsp_.setContent(pc);
	}
	nse_.send_response(rsp_, "localhost", 5555);
}

void RequestHandler::generateRequestId(string& rqi) {
	boost::random::uniform_int_distribution<> dist(1, 99999);
	do {
		rqi = boost::str(boost::format("RQI-%05d%05d") % dist(gen_) % dist(gen_));
	} while (reqc_.isRequestIdInUse(rqi));
}

}	// OneM2M

}	// MicroWireless
