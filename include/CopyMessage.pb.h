/*
 * CopyMessage.pb.h
 *
 *  Created on: 2015年9月19日
 *      Author: weimi_000
 */

#ifndef INCLUDE_COPYMESSAGE_PB_H_
#define INCLUDE_COPYMESSAGE_PB_H_


#include <iostream>
#include <string>
#include <set>

#include "google/protobuf/Message.h"
#include "google/protobuf/Descriptor.h"

namespace google {

namespace protobuf {

using google::protobuf::Message;
using google::protobuf::Descriptor;
using google::protobuf::Reflection;
using google::protobuf::FieldDescriptor;

class CopyMessage {
	const Message& sm_;
	Message& dm_;
	const Descriptor* sd_;
	const Descriptor* dd_;
	const Reflection* sr_;
	const Reflection* dr_;

public:
	CopyMessage(const Message&, Message&);
	// Copy all fields
	void copyFields();
	// Copy selected fields in attributes set
	void copyFields(std::set<std::string>&);

private:
	bool copyField(const std::string&);
	bool copyField(const std::string&, const FieldDescriptor*, const FieldDescriptor*);
	bool copySubMessage(const FieldDescriptor*, const FieldDescriptor*);
	bool copyRepeatedSubMessage(const int, const FieldDescriptor*, const FieldDescriptor*);
};

bool compareMessage(const Message&, const Message&);

}	// protobuf

}	// google
#endif /* INCLUDE_COPYMESSAGE_PB_H_ */
