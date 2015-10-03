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

using namespace MicroWireless::OneM2M;

class CSEBase;

class AEClass : public ResourceBase {

public:
	AEClass();
	virtual ~AEClass();

	AEClass(const std::string& json, const std::string& id_str);
	AEClass(const std::string& ri, ResourceStore<CSEBase>& rdb);

	bool setAE(const std::string& ri, ResourceStore<CSEBase>& rdb);
	bool setAE(const std::string &json, const std::string& id_str);
	virtual bool setResourceBase(const std::string& pc, const std::string& id_str, Operation op);
	virtual bool setNewResourceAttr(const std::string& ri, const std::string& rn,
			const std::string& pi, AEClass& ret);

	const std::string& getAppName();
	const std::string& getAppId();
	const std::string& getAEId();
	bool setAEId(const std::string& aei);

protected:

private:
	bool checkIdConsistency();
	bool checkAttributes(Operation op);

private:
	pb::AE* p_ae_;

	static map<const std::string, map<Operation, attrOption>> allowAttr;
};

}	// OneM2M

}	// MicroWireless



#endif /* INCLUDE_AE_H_ */
