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
class AEAnnc;

class AEClass : public ResourceBase {

public:
	AEClass();
	virtual ~AEClass();

	AEClass(const std::string& json, const std::string& id_str);
	AEClass(const std::string& ri, ResourceStore<CSEBase>& rdb);

	bool setResourceBase(const std::string& ri, ResourceStore<CSEBase>& rdb);
	bool setResourceBase(const std::string &json, const std::string& id_str);
	bool setResourceBase(const std::string& pc, const std::string& id_str, Operation op);
	bool setNewAttr(const std::string&, const std::string&, const std::string&, AEClass*);
	bool updateResource(const AEClass&);

	const std::string& getAppName() const;
	const std::string& getAppId() const;
	const std::string& getAEId() const;
	const std::string& getOr() const;
	bool setAppName(const std::string&);
	bool setAppId(const std::string&);
	bool setAEId(const std::string&);
	bool setOr(const std::string&);

protected:

private:
	bool checkIdConsistency();
	bool checkAttributes(Operation op);

private:
	pb::AE* p_ae_;

	friend AEAnnc;
	static map<const std::string, map<Operation, attrOption>> allowAttr;
};

}	// OneM2M

}	// MicroWireless



#endif /* INCLUDE_AE_H_ */
