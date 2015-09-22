/*
 * ResourceBaseAnnc.cc
 *
 *  Created on: 2015年9月19日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include <json2pb.h>

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

bool ResourceBaseAnnc::compare(pb::ResourceBase& tgt) {
	return compareMessage(base_, tgt);
}

string ResourceBaseAnnc::getJson() {
	return pb2json(base_);
}

ResourceBaseAnnc::~ResourceBaseAnnc() { }

}	// OneM2M

}	// MicroWireless
