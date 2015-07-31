/*
 * CSEServer.cc
 *
 *  Created on: 2015年7月25日
 *      Author: weimi_000
 */

#include "CSEResourceStore.h"
#include "CommonTypes.h"
#include "NSEBase.h"
#include "CSEHandler.h"
#include "CSEServer.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

CSEServer::CSEServer(CSEResourceStore& rdb, NSEBase& nse, CSEHandler& hdl) :
	rdb_(rdb),
	nse_(nse),
	hdl_(hdl)
{
	nse_.setRequestHandler(hdl_);
}

void CSEServer::run() {
	nse_.run();
}


}	//OneM2M
}	//MicroWireless
