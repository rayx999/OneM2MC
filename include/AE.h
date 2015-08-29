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
	virtual ~AEClass();

	AEClass(const string& json, const string& id_str);
	AEClass(const string& ri, ResourceStore<CSEBase>& rdb);

	bool setAE(const string& ri, ResourceStore<CSEBase>& rdb);
	bool setAE(const string &json, const string& id_str);
	virtual bool setResourceBase(const string& pc, const string& id_str, Operation op);
	virtual bool setNewResourceAttr(const string& ri, const string& rn, const string& pi,
			AEClass& ret);

	const string& getAppName();
	const string& getAppId();
	const string& getAEId();
	bool setAEId(const string& aei);

protected:

private:
	bool checkIdConsistency();
	bool checkAttributes(Operation op);

private:
	pb::AE* p_ae_;

	enum attrTag {
		TAG_APN  = 100,
		TAG_API  = 101,
		TAG_AEI  = 102,
		TAG_POA  = 103,
		TAG_OR   = 104,
		TAG_NL   = 105
	};

	static map<int, map<Operation, attrOption>> allowAttr;
};

}	// OneM2M

}	// MicroWireless



#endif /* INCLUDE_AE_H_ */
