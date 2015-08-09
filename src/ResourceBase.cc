/*
 * ResourceBase.cc
 *
 *  Created on: 2015年8月5日
 *      Author: weimi_000
 */


#include <iostream>

#include "CommonTypes.pb.h"
#include "ResourceBase.pb.h"
#include "ResourceBase.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

pb::CSEBase& ResourceBase::getCSEBase() {
	return *base_.mutable_cse();
}

SupportedResourceType ResourceBase::getResourceBase() {
	SupportedResourceType srt_ = SRT_NONE;
	if (base_.has_cse()) {
		srt_ = CSE_BASE;
	}
	return srt_;
}

string ResourceBase::getJson() {
	return pb2json(base_);
}

}	// OneM2M

}	// MicroWireless
