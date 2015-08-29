/*
 * CommonUtils.cc
 *
 *  Created on: 2015年8月11日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>
#include <boost/regex.hpp>
#include "CommonUtils.h"

namespace MicroWireless {

namespace OneM2M {

using namespace std;
using namespace MicroWireless::OneM2M;

bool isMatching(const string& id, const string& regex) {
	boost::regex pattern_(regex, boost::regex::icase);
    return boost::regex_match(id, pattern_);
}

bool parseFullIds(const string& id_str, const string& csi_regex,
		string& domain, string& csi, string& ri)
{
	static const boost::regex pattern_("^(//([\\w-]*?)\\.?([\\w-]*?)\\.?([\\w-]+\\.[\\w-]+))/([\\w-]+)(/[\\w/-]+)*");
	boost::smatch sm_;

	boost::regex_match(id_str, sm_, pattern_);
	// sm_[1]:	whole sp_id with possible cse_id.
	// sm_[2]:	possible cse_id.
	// sm_[3]:   possible cse_id or subdomain
	// sm_[4]:   root domain
	// sm_[5]:	cse_id or resource id
	// sm_[6]:	possible resource id

	if (sm_.size() != 7) {
		cerr << "parseFullIds: bad id str: " << id_str <<endl;
		return false;
	}

	if (sm_[4].str().empty()) {
		cerr << "parseFullIds: missing domain. id_str:" << id_str << endl;
		return false;
	}

	domain = sm_[4];

	if (!sm_[3].str().empty()) {
		if (isMatching(sm_[3].str(), csi_regex)) {
			csi = sm_[3].str();
		} else {
			// cancatenate subdomains:
			domain = sm_[3] + "." + domain;
		}
	}

	if (!sm_[2].str().empty()) {
		if (isMatching(sm_[2].str(), csi_regex)) {
			if (csi.empty()) {
				csi = sm_[2].str();
			} else {
				cerr << "parseFullIds: inconsistent CSE ids:" << sm_[2] << " and "<< csi << endl;
				return false;
			}
		} else {
			// cancatenate subdomain:
			domain = sm_[2] + "." + domain;
		}
	}

	if (sm_[5].str().empty()) {
		cerr << "parseFullIds: missing cse id or resource id.\n";
		return false;
	}

	if (isMatching(sm_[5].str(), csi_regex)) {
		if (csi.empty()) {
			csi = sm_[5].str();
		} else if (csi != sm_[5].str()) {
			cerr << "parseFullIds: inconsistent CSE ids:" << csi << " and "<< sm_[5] << endl;
			return false;
		} else {
			cerr << "parseFullIds: warning: duplicate CSE id in :" << id_str << endl;
		}
	} else {
		ri = sm_[5].str();
	}

	if (csi.empty()) {
		cerr << "parseFullIds: missing CSE id in " << id_str << endl;
		return false;
	}

	ri += sm_[6].str();
/*
	if (ri.empty()) {
		cerr << "parseFullIds: missing resource id in " << id_str << endl;
		return false;
	}
*/
	domain = "//" + domain;
	csi = "/" + csi;
	if (!ri.empty() && ri.at(0) == '/') {
		ri = ri.substr(1);
	}
	return true;
}

bool parsePartialIds(const string& id_str, const string& csi_regex,
		string& csi, string& ri)
{
	static boost::regex pattern_("^/([\\w-]+)/?([\\w/-]+)*");
	boost::smatch sm_;

	boost::regex_match(id_str, sm_, pattern_);
	// sm_[1]:	cse_id.
	// sm_[2]:	possible resource id

	if (sm_.size() != 3) {
		cerr << "parsePartialIds: bad id str: " << id_str <<endl;
		return false;
	}

	if (sm_[1].str().empty() || !isMatching(sm_[1].str(), csi_regex)) {
		cerr << "parsePartialIds: missing CSE id.\n";
		return false;
	}

	csi = sm_[1];

	if (sm_[2].str().empty()) {
		cerr << "parsePartialIds: missing resource id.\n";
		return false;
	}

	csi = "/" + csi;
	ri = sm_[2];
	return true;
}

bool parseIds(const string& id_str, const string& csi_regex,
		string& domain, string& csi, string& ri)
{
	domain.clear();
	csi.clear();
	ri.clear();

	try {
		if (id_str.substr(0,2) == "//") {
			return parseFullIds(id_str, csi_regex, domain, csi, ri);
		} else if (id_str.at(0) == '/') {
			return parsePartialIds(id_str, csi_regex, csi, ri);
		} else {
			ri = id_str;
			return true;
		}
	} catch (exception &e) {
		cerr << "Parseids exception:" << e.what() << endl;
		return false;
	}
}

} 	// OneM2M
}	// MicroWireless

// Protobuf 3 Timestamp operator overloading
using google::protobuf::Timestamp;

bool operator == (const Timestamp& ts1,	const Timestamp& ts2) {
	return ts1.seconds() == ts2.seconds() &&
		   ts1.nanos() == ts2.nanos();
}

bool operator < (const Timestamp& ts1,	const Timestamp& ts2) {
	return (ts1.seconds() < ts2.seconds() ||
		   (ts1.seconds() ==  ts2.seconds() &&
		    ts1.nanos() < ts2.nanos()));
}

const Timestamp& max(const Timestamp& ts1,	const Timestamp& ts2) {
	return (ts1 < ts2 ? ts2 : ts1);
}

const Timestamp& min(const Timestamp& ts1,	const Timestamp& ts2) {
	return (ts1 < ts2 ? ts1 : ts2);
}
