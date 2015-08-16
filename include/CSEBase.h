/*
 * CSEBase.h
 *
 *  Created on: 2015年7月15日
 *      Author: weimi_000
 */

#ifndef INCLUDE_CSEBASE_H_
#define INCLUDE_CSEBASE_H_

#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "CommonTypes.pb.h"
#include "CSEBase.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;

#define DEFAULT_CSEBASE_FN	"CSEBase"

class CSEBase : public ResourceBase {

public:
	CSEBase();
	~CSEBase();

	CSEBase(const string& json, const string& id_str);
	CSEBase(const string& ri, ResourceStore<CSEBase>& rdb);

	bool setCSEBase(const string& ri, ResourceStore<CSEBase>& rdb);
	bool setCSEBase(const string &json, const string& id_str);

	CSEType getCSEType();
	const string &getCSEId();

	int getSupportedResource(SupportedResourceType *&rt);
	bool isResourceSupported(SupportedResourceType rt);

	string getJson();

protected:
	bool setCreateTimestamp(TimeStamp &ct);
	bool setLastModifiedTimestamp(TimeStamp &lt);

private:
	bool checkIdConsistency();

private:
	pb::CSEBase* p_cse_;
};

}	// OneM2M

}	// MicroWireless

#endif /* INCLUDE_CSEBASE_H_ */
