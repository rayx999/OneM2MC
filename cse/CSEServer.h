/*
 * CSEServer.h
 *
 *  Created on: 2015年7月25日
 *      Author: weimi_000
 */

#ifndef CSE_CSESERVER_H_
#define CSE_CSESERVER_H_


namespace MicroWireless {
namespace OneM2M {

using namespace MicroWireless::OneM2M;

class CSEResourceStore;
class NSEBase;
class CSEHandler;

namespace CSEServer {
	void setCSEServer(CSEResourceStore& rdb, NSEBase& nse, CSEHandler& hdl);
	void clrCSEServer();
	CSEResourceStore& getCSEResourceStore();
	NSEBase& getNSEBase();
	CSEHandler& getCSEHandler();
	void run();
}	// CSEServer

}	//OneM2M

}	//MicroWireless

#endif /* CSE_CSESERVER_H_ */
