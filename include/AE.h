/*
 * AE.h
 *
 *  Created on: 2015年8月21日
 *      Author: weimi_000
 */

#ifndef INCLUDE_AE_H_
#define INCLUDE_AE_H_

#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "CommonTypes.pb.h"
#include "AE.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;

class CSEBase;

class AE : public ResourceBase {

public:
	AE();
	~AE();

	AE(const string& json, const string& id_str);
	AE(const string& ri, ResourceStore<CSEBase>& rdb);

	bool setAE(const string& ri, ResourceStore<CSEBase>& rdb);
	bool setAE(const string &json, const string& id_str);

	const string& getAppName();
	const string& getAppId();
	const string& getAEId();

protected:

private:
	bool checkIdConsistency();

private:
	pb::AE* p_ae_;
};

}	// OneM2M

}	// MicroWireless



#endif /* INCLUDE_AE_H_ */
