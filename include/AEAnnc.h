/*
 * AEAnnc.h
 *
 *  Created on: 2015年9月18日
 *      Author: weimi_000
 */

#ifndef INCLUDE_AEANNC_H_
#define INCLUDE_AEANNC_H_

#include <iostream>
#include <string>

#include "AE.pb.h"
#include "CommonTypes.h"
#include "AE.h"
#include "ResourceBase.h"

namespace MicroWireless {

namespace OneM2M {

using namespace MicroWireless::OneM2M;

class AEAnnc : public ResourceBase {

public:
	AEAnnc();
	AEAnnc(const AEClass& ae);
	virtual ~AEAnnc();

	virtual bool setResourceBase(const std::string& pc, const std::string& id_str, Operation op);
	virtual bool setNewAttr(const std::string&, const std::string&,	const std::string&, AEAnnc*);

	const std::string& getAppName();
	const std::string& getAppId();
	const std::string& getAEId();
	bool setAEId(const std::string& aei);

	bool filterAnncAttr();

	bool copyAnncFields();
	bool copyAnncFields(const AnncAttr& oa);

private:
	bool copyAnncFields(AnncAttr ma, AnncAttr oa);
	bool checkIdConsistency();
	bool checkAttributes(Operation op);

private:
	const AEClass* p_ae_;
	pb::AE* p_aea_;
	static const AnncAttr ma_, oa_;
	static map<const string, map<Operation, AEClass::attrOption>> allowAttr;
	static std::map<const std::string, AEAnnc::attrOption> anncAttr;
};

}	// OneM2M

}	// MicroWireless



#endif /* INCLUDE_AE_H_ */
