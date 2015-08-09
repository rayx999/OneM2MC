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

	CSEBase(const string& json);
	CSEBase(const string& ri, ResourceStore<CSEBase>& rdb);

	bool setCSEBase(const string& ri, ResourceStore<CSEBase>& rdb);
	bool outToResourceStore(const string& ri, ResourceStore<CSEBase>& rdb);

	bool setCSEBase(const string &json);

	const string &getDomain();
	const string &getCSEId();
	CSEType getCSEType();

	const string& getResourceId();
	const string& getResourceName();

	bool getCreateTimestamp(TimeStamp &create_time);
	bool getLastModifiedTimestamp(TimeStamp &create_time);

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
	bool cse_orphan_;
	string domain_;
};

}	// OneM2M

}	// MicroWireless

#endif /* INCLUDE_CSEBASE_H_ */
