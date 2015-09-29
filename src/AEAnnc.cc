/*
 * AEAnnc.cc
 *
 *  Created on: 2015年9月18日
 *      Author: weimi_000
 */


#include <stdexcept>
#include <iostream>

#include "CopyMessage.pb.h"
#include "AE.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"
#include "ResourceBaseAnnc.h"
#include "AE.h"
#include "AEAnnc.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

using google::protobuf::CopyMessage;

const AnncAttr AEAnnc::ma_ = { "ri",  "rn",  "et",  "acpi", "lbl" };
const AnncAttr AEAnnc::oa_ = { "apn", "api", "aei", "poa",  "or", "nl" };

AEAnnc::AEAnnc(const AEClass& ae) : ae_(ae), p_aea_(getAEAnnc()) { }

bool AEAnnc::copyAnncFields() {
	return copyAnncFields(ma_, oa_);
}

bool AEAnnc::copyAnncFields(const AnncAttr& oa) {
	return copyAnncFields(ma_, oa);
}

bool AEAnnc::copyAnncFields(AnncAttr ma, AnncAttr oa) {
	ResourceBaseAnnc::copyAnncFields(ae_, ma, oa);

	const pb::AE& ae_pb_ = ae_.getElement<pb::AE>();
	CopyMessage cf_(ae_pb_, *p_aea_);
	cf_.copyFields(ma);
	cf_.copyFields(oa);
	if (ma.size() > 0) {
		cerr << "AEAnnc::copyAnncFields: MA fields missing in source:\n";
		std::copy(ma_.begin(), ma_.end(), ostream_iterator<string>(cout));
		return false;
	}
	return true;
}

AEAnnc::~AEAnnc() { }

}	// OneM2M

}	// MicroWireless


