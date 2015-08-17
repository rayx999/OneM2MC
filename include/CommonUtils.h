/*
 * CommonUtils.h
 *
 *  Created on: 2015年7月19日
 *      Author: weimi_000
 */

#ifndef INCLUDE_COMMONUTILS_H_
#define INCLUDE_COMMONUTILS_H_

#include <iostream>
#include <stdlib.h>
#include "google/protobuf/timestamp.pb.h"

namespace MicroWireless {

namespace OneM2M {

using namespace MicroWireless::OneM2M;

static const std::string CSERegex("IN-CSE-\\d\\d");

enum ValidateType {
	VALIDATE_COMMON,
	VALIDATE_ALL
};

template <typename T>
bool setString(const std::string & s, void (T::*setter)(std::string *), T & t) {
	std::string * _p = new std::string(s);

	if (_p == NULL) {
		std::cerr << "Can't allocate. new(" << s << ") failed." << std::endl;
		return false;
	} else {
		(t.*setter)(_p);
		return true;
	}
}

bool isMatching(const std::string& id, const std::string& regex);

// Parse Request/Response Primitive To fields into domain, CSE_id,
// and Rsource_Id (Structured/Unstructured).
// id_str: max 4 subdomains in FQDN, including host. First one can be CSE_id.
// Legitimate strings:
//     FQDN with CSE_id:		//in-cse-01.o2o.microwireless.com/95847
//     FQDN with CSE_id:		//in-cse-10.microwireless.com/95847
//     Explicit CSE_id:			//o2o.microwireless.com/in-cse-01/95847
//     Full path:				//microwireless.com/in-cse-10/fresh/potato
//	   CSE id only:				/in-cse-01/95847
//  Invalid string:
//	   Duplicate CSE_id:		//in-cse-10.microwireless.com/in-cse-01/95847
// 	   No CSE_id:				//o2o.microwireless.com/95847
//     No resource id:			//microwireless.com/in-cse-10
//	   							/in-cse-01
//  Output:
//     domain:					//o2o.microwireless.com, //mcrowireless.com
//     CSE_id:					/in-cse-01, /in-cse-10
//	   Resource_id:				95847, fresh/potato
bool parseIds(const std::string& id_str, const std::string& csi_regex,
		std::string& domain, std::string& csi, std::string& ri);


}	// OneM2M
}	// MicroWireless

// Protobuf 3 Timestamp operator overloading
using google::protobuf::Timestamp;

bool operator == (const Timestamp& ts1, const Timestamp& ts2);

bool operator < (const Timestamp& ts1,	const Timestamp& ts2);

const Timestamp& max(const Timestamp& ts1,	const Timestamp& ts2);

const Timestamp& min(const Timestamp& ts1,	const Timestamp& ts2);

#endif /* INCLUDE_COMMONUTILS_H_ */
