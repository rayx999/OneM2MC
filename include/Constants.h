/*
 * Constants.h
 *
 *  Created on: 2015年9月29日
 *      Author: weimi_000
 */

#ifndef INCLUDE_CONSTANTS_H_
#define INCLUDE_CONSTANTS_H_

#include <iostream>
#include <string>

namespace MicroWireless {

namespace OneM2M {

using namespace MicroWireless::OneM2M;

class AttrName {
public:
	// ResourceBase
	static const std::string& TY();
	static const std::string& RI();
	static const std::string& RN();
	static const std::string& PI();
	static const std::string& CT();
	static const std::string& LT();
	static const std::string& ET();
	static const std::string& ACPI();
	static const std::string& LBL();
	static const std::string& AA();
	static const std::string& AT();
	static const std::string& ST();

	// Common
	static const std::string& POA();
	static const std::string& NL();
	static const std::string& RR();

	// CSE

	// AE
	static const std::string& APN();
	static const std::string& API();
	static const std::string& AEI();
	static const std::string& OR();


	// RemoteCSE
	static const std::string& CST();
	static const std::string& CB();
	static const std::string& CSI();
	static const std::string& MEI();
	static const std::string& TRI();
};

}	// OneM2M

}	// MicroWireless


#endif /* INCLUDE_CONSTANTS_H_ */
