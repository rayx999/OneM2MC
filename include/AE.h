/*
 * AEClass.h
 *
 *  Created on: 2015年8月21日
 *      Author: weimi_000
 */

#ifndef INCLUDE_AE_H_
#define INCLUDE_AE_H_

#include <iostream>
#include <string>

#include "CommonTypes.pb.h"
#include "AE.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

class CSEBase;

class AEClass : public ResourceBase {

public:
	AEClass();
	~AEClass();

	AEClass(const string& json, const string& id_str);
	AEClass(const string& ri, ResourceStore<CSEBase>& rdb);

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
