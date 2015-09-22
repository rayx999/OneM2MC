/*
 * ResourceBaseAnnc.h
 *
 *  Created on: 2015年9月19日
 *      Author: weimi_000
 */

#ifndef INCLUDE_RESOURCEBASEANNC_H_
#define INCLUDE_RESOURCEBASEANNC_H_

#include <iostream>
#include <string>

#include "ResourceBase.pb.h"
#include "CommonTypes.h"

namespace MicroWireless {

namespace OneM2M {

using namespace MicroWireless::OneM2M;

class ResourceBase;

class ResourceBaseAnnc {
public:
	ResourceBaseAnnc();
	virtual ~ResourceBaseAnnc();

	pb::AE* getAEAnnc();
	void copyAnncFields(const ResourceBase&, AnncAttr&, AnncAttr&);
	bool serializeToString(std::string* pc);
	bool compare(pb::ResourceBase& tgt);
	std::string getJson();

protected:
	pb::ResourceBase base_;
};

}	// OneM2M

}	// MicroWireless


#endif /* INCLUDE_RESOURCEBASEANNC_H_ */
