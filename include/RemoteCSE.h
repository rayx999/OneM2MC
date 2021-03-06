/*
 * RemoteCSE.h
 *
 *  Created on: 2015年10月22日
 *      Author: weimi_000
 */

#ifndef INCLUDE_REMOTECSE_H_
#define INCLUDE_REMOTECSE_H_

#include <iostream>

#include "CommonTypes.pb.h"
#include "RemoteCSE.pb.h"
#include "CommonTypes.h"
#include "ResourceBase.h"

namespace MicroWireless {

namespace OneM2M {

class CSEBase;

class RemoteCSE : public ResourceBase {

public:
	RemoteCSE();
	~RemoteCSE();

	RemoteCSE(const std::string& json, const std::string& id_str);
	RemoteCSE(const std::string& ri, ResourceStore<CSEBase>& rdb);
	RemoteCSE(CSEBase& cb);

	bool setResourceBase(const std::string& ri, ResourceStore<CSEBase>& rdb);
	bool setResourceBase(const std::string &json, const std::string& id_str);
	bool setResourceBase(const std::string& pc, const std::string& id_str, Operation op);
	bool setNewAttr(const std::string&, const std::string&,	const std::string&);
	bool setNewAttr(const std::string&, const std::string&,	const std::string&, RemoteCSE*);
	bool updateResource(const RemoteCSE& upd);

	CSEType getCSEType();
	const std::string& getCSEBase() const;
	const std::string& getCSEId() const;

	const std::string& getM2MExtId() const;
	bool setM2MExtId(const std::string&);

	const std::string& getTriggerRecipientId() const;
	bool setTriggerRecipientId(const std::string&);

	bool getRequestReachability() const;
	bool setRequestReachability(bool);

	const std::string& getNodeLink() const;
	bool setNodeLink(const std::string&);

protected:

private:
	bool checkIdConsistency();
	bool checkAttributes(Operation op);

private:
	pb::RemoteCSE* p_csr_;
	static std::map<const std::string,std:: map<Operation, attrOption>> allowAttr;
};

}	// OneM2M

}	// MicroWireless



#endif /* INCLUDE_REMOTECSE_H_ */
