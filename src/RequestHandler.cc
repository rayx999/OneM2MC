/*
 * RequestHandler.cc
 *
 *  Created on: 2015年7月22日
 *      Author: weimi_000
 */


#include <string>

#include "ResourceBase.pb.h"
#include "ResourceBase.h"
#include "RequestHandler.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

// add modified/created resources to content pc
bool RequestHandler::composeContent(ResourceBase& res_pc, ResourceBase& res, string& pc) {
	// get all timestamps
	res_pc.CopyResourceTimeStamps(res);
	return res_pc.SerializeToString(&pc);
}

}	// OneM2M

}	// MicroWireless
