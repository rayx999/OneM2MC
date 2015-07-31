/*
 * CSEServer.h
 *
 *  Created on: 2015年7月25日
 *      Author: weimi_000
 */

#ifndef CSE_CSESERVER_H_
#define CSE_CSESERVER_H_


#include "CommonTypes.h"
#include "NSEBase.h"
#include "CSEHandler.h"
#include "CSEResourceStore.h"

namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class CSEServer
{
public:
	CSEServer(CSEResourceStore& rdb, NSEBase& nse, CSEHandler& hdl);

	void run();

private:
	CSEResourceStore& rdb_;
	NSEBase& nse_;
	CSEHandler& hdl_;
};

}	//OneM2M
}	//MicroWireless

#endif /* CSE_CSESERVER_H_ */
