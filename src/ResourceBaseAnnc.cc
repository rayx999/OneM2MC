/*
 * ResourceBaseAnnc.cc
 *
 *  Created on: 2015年9月19日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>

#include "CopyMessage.pb.h"
#include "ResourceBase.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"
#include "ResourceBaseAnnc.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

using google::protobuf::CopyMessage;

ResourceBaseAnnc::ResourceBaseAnnc() : base_() { }

pb::AE* ResourceBaseAnnc::getAEAnnc() {
	return base_.mutable_aea();
}

void ResourceBaseAnnc::copyAnncFields(const ResourceBase& src, AnncAttr& ma, AnncAttr& oa) {
	CopyMessage cf_(src.getResourceBase(), base_);
	cf_.copyFields(ma);
	cf_.copyFields(oa);
}

bool ResourceBaseAnnc::serializeToString(string* pc) {
	return base_.SerializeToString(pc);
}

ResourceBaseAnnc::~ResourceBaseAnnc() { }

}	// OneM2M

}	// MicroWireless
