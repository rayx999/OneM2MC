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

namespace MicroWireless {

namespace OneM2M {

using namespace std;

#define DEFAULT_CSEBASE_FN	"data/CSEBase.res"

class CSEBase {

public:
	CSEBase();
	CSEBase(const char* fn);
	CSEBase(const string& json);
	bool setCSEBase(const char * fn);
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

	bool outToFile(const char * fn);
	string getJson();

protected:
	bool setCreateTimestamp(TimeStamp &ct);
	bool setLastModifiedTimestamp(TimeStamp &lt);

private:
	bool checkIdConsistency();

private:
	pb::CSEBase cse_base_;
	string domain_;
};

}	// OneM2M

}	// MicroWireless

#endif /* INCLUDE_CSEBASE_H_ */
