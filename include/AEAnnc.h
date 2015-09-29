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
#include "ResourceBaseAnnc.h"
#include "AE.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

class AEAnnc : public ResourceBaseAnnc {

public:
	AEAnnc(const AEClass& ae);
	virtual ~AEAnnc();
	bool copyAnncFields();
	bool copyAnncFields(const AnncAttr& oa);

private:
	bool copyAnncFields(AnncAttr ma, AnncAttr oa);

private:
	const AEClass& ae_;
	pb::AE* p_aea_;
	static const AnncAttr ma_, oa_;
};

}	// OneM2M

}	// MicroWireless



#endif /* INCLUDE_AE_H_ */
