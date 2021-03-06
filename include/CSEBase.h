/*
 * CSEBase.h
 *
 *  Created on: 2015年7月15日
 *      Author: weimi_000
 */

#ifndef INCLUDE_CSEBASE_H_
#define INCLUDE_CSEBASE_H_

#include <iostream>
#include <fstream>
#include <sstream>

#include "CommonTypes.pb.h"
#include "CSEBase.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"

namespace MicroWireless {

namespace OneM2M {

#define DEFAULT_CSEBASE_FN	"CSEBase"

class ResponsePrim;

class CSEBase : public ResourceBase {

public:
	CSEBase();
	~CSEBase();

	CSEBase(const std::string& json, const std::string& id_str);
	CSEBase(const std::string& ri, ResourceStore<CSEBase>& rdb);

	bool setCSEBase(const std::string& ri, ResourceStore<CSEBase>& rdb);
	bool setCSEBase(const std::string &json, const std::string& id_str);
	bool setResourceBase(const ResponsePrim&);

	CSEType getCSEType();
	const std::string &getCSEId();

	int getSupportedResource(SupportedResourceType *&rt);
	bool isResourceSupported(SupportedResourceType rt);

protected:

private:
	bool checkIdConsistency();

private:
	pb::CSEBase* p_cse_;
};

}	// OneM2M

}	// MicroWireless

#endif /* INCLUDE_CSEBASE_H_ */
