/*
 * CopyMessage.pb.cc
 *
 *  Created on: 2015年9月19日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>

#include "CopyMessage.pb.h"

namespace google {

namespace protobuf {

using google::protobuf::Descriptor;
using google::protobuf::Reflection;
using google::protobuf::FieldDescriptor;

CopyMessage::CopyMessage(const Message& sm, Message& dm) : sm_(sm), dm_(dm) {
	sd_ = sm_.GetDescriptor();
	dd_ = dm_.GetDescriptor();
	sr_ = sm_.GetReflection();
	dr_ = dm_.GetReflection();
}

// Copy all fields
void CopyMessage::copyFields() {
	std::vector<const FieldDescriptor*> fs_;
	sr_->ListFields(sm_, &fs_);
	for (const auto sfd_ : fs_) {
		const std::string& fn_ = sfd_->name();
		const FieldDescriptor* dfd_ = dd_->FindFieldByName(fn_);
		if (!copyField(fn_, sfd_, dfd_)) {
			std::cout << "CopyMessage::copyFields: [" << sfd_->name() << "] failed\n";
		}
	}
}

// Copy selected fields in attributes list
void CopyMessage::copyFields(std::set<std::string>& at) {
	std::set<std::string> at_(at);
	for (const auto fn : at_) {
		if (copyField(fn)) {
			at.erase(fn);
		} else {
			std::cout << "CopyMessage::copyFields: [" << fn << "] missing\n";
		}
	}
}

bool CopyMessage::copySubMessage(const FieldDescriptor* sfd, const FieldDescriptor*dfd) {
	const Message& s_subm_ = sr_->GetMessage(sm_, sfd);
	Message* d_subm_ = s_subm_.New();
	if (d_subm_ == NULL) {
		//std::cerr << "CopyMessage::copySubMessage: message type [" <<
		//		s_subm_.GetDescriptor()->name() << "] unknown.\n";
		return false;
	}
	CopyMessage copy_sub_(s_subm_, *d_subm_);
	copy_sub_.copyFields();
	dr_->SetAllocatedMessage(&dm_, d_subm_, dfd);
	return true;
}

bool CopyMessage::copyRepeatedSubMessage(const int i,
		const FieldDescriptor* sfd, const FieldDescriptor*dfd) {
	const Message& s_subm_ = sr_->GetRepeatedMessage(sm_, sfd, i);
	Message* d_subm_ = dr_->AddMessage(&dm_, dfd);
	if (d_subm_ == NULL) {
		//std::cerr << "CopyMessage::copyRepeatedSubMessage: message type [" <<
		//		s_subm_.GetDescriptor()->name() << "] unknown.\n";
		return false;
	}
	CopyMessage copy_sub_(s_subm_, *d_subm_);
	copy_sub_.copyFields();
	return true;
}

bool CopyMessage::copyField(const std::string& fn) {

	const FieldDescriptor* sfd_ = sd_->FindFieldByName(fn);
	const FieldDescriptor* dfd_ = dd_->FindFieldByName(fn);
	return copyField(fn, sfd_, dfd_);
}

bool CopyMessage::copyField(const std::string& fn, const FieldDescriptor* sfd,
		const FieldDescriptor* dfd) {

	if (sfd == NULL || dfd == NULL) {
		//std::cerr << "CopyMessage::copyField: field [" << fn << "] missing.\n";
		return false;
	}

	if (sfd->label() == FieldDescriptor::LABEL_REPEATED) {
		int size_ = sr_->FieldSize(sm_, sfd);
		if (size_ == 0) {
			// Repeated field size should > 0;
			return false;
		}

		switch (sfd->cpp_type()) {
		case FieldDescriptor::CPPTYPE_INT32:
			for (int i = 0; i < size_; i++)
				dr_->AddInt32(&dm_, dfd,
						sr_->GetRepeatedInt32(sm_, sfd, i));
			break;
		case FieldDescriptor::CPPTYPE_INT64:
			for (int i = 0; i < size_; i++)
				dr_->AddInt64(&dm_, dfd,
						sr_->GetRepeatedInt64(sm_, sfd, i));
			break;
		case FieldDescriptor::CPPTYPE_UINT32:
			for (int i = 0; i < size_; i++)
				dr_->AddUInt32(&dm_, dfd,
						sr_->GetRepeatedUInt32(sm_, sfd, i));
			break;
		case FieldDescriptor::CPPTYPE_UINT64:
			for (int i = 0; i < size_; i++)
				dr_->AddUInt64(&dm_, dfd,
						sr_->GetRepeatedUInt64(sm_, sfd, i));
			break;
		case FieldDescriptor::CPPTYPE_DOUBLE:
			for (int i = 0; i < size_; i++)
				dr_->AddDouble(&dm_, dfd,
						sr_->GetRepeatedDouble(sm_, sfd, i));
			break;
		case FieldDescriptor::CPPTYPE_FLOAT:
			for (int i = 0; i < size_; i++)
				dr_->AddFloat(&dm_, dfd,
						sr_->GetRepeatedFloat(sm_, sfd, i));
			break;
		case FieldDescriptor::CPPTYPE_BOOL:
			for (int i = 0; i < size_; i++)
				dr_->AddBool(&dm_, dfd,
						sr_->GetRepeatedBool(sm_, sfd, i));
			break;
		case FieldDescriptor::CPPTYPE_ENUM:
			for (int i = 0; i < size_; i++)
				dr_->AddEnumValue(&dm_, dfd,
						sr_->GetRepeatedEnumValue(sm_, sfd, i));
			break;
		case FieldDescriptor::CPPTYPE_STRING:
			for (int i = 0; i < size_; i++)
				dr_->AddString(&dm_, dfd,
						sr_->GetRepeatedString(sm_, sfd, i));
			break;
		case FieldDescriptor::CPPTYPE_MESSAGE:
			for (int i = 0; i < size_; i++)
				copyRepeatedSubMessage(i, sfd, dfd);
			break;
		default:
			std::cerr << "CopyMessage::copyField: Unknown field type:"
				<< sfd->cpp_type() << std::endl;
			return false;
		}
		return true;

	} else if (sr_->HasField(sm_, sfd)) {

		switch (sfd->cpp_type()) {
		case FieldDescriptor::CPPTYPE_INT32:
			dr_->SetInt32(&dm_, dfd, sr_->GetInt32(sm_, sfd));
			break;
		case FieldDescriptor::CPPTYPE_INT64:
			dr_->SetInt64(&dm_, dfd, sr_->GetInt64(sm_, sfd));
			break;
		case FieldDescriptor::CPPTYPE_UINT32:
			dr_->SetUInt32(&dm_, dfd, sr_->GetUInt32(sm_, sfd));
			break;
		case FieldDescriptor::CPPTYPE_UINT64:
			dr_->SetUInt64(&dm_, dfd, sr_->GetUInt64(sm_, sfd));
			break;
		case FieldDescriptor::CPPTYPE_DOUBLE:
			dr_->SetDouble(&dm_, dfd, sr_->GetDouble(sm_, sfd));
			break;
		case FieldDescriptor::CPPTYPE_FLOAT:
			dr_->SetFloat(&dm_, dfd, sr_->GetFloat(sm_, sfd));
			break;
		case FieldDescriptor::CPPTYPE_BOOL:
			dr_->SetBool(&dm_, dfd, sr_->GetBool(sm_, sfd));
			break;
		case FieldDescriptor::CPPTYPE_ENUM:
			dr_->SetEnumValue(&dm_, dfd, sr_->GetEnumValue(sm_, sfd));
			break;
		case FieldDescriptor::CPPTYPE_STRING:
			dr_->SetString(&dm_, dfd, sr_->GetString(sm_, sfd));
			break;
		case FieldDescriptor::CPPTYPE_MESSAGE:
			copySubMessage(sfd, dfd);
			break;
		default:
			std::cerr << "CopyMessage::copy: Unknown field type:" << sfd->cpp_type() << std::endl;
			return false;
		}
		return true;
	}
	//std::cout << "CopyMessage::copyField: [" << fn << "] not present\n";
	return false;
}

bool compareMessage(const Message& sm, const Message& dm) {
	std::string ss_, ds_;
	if (!sm.SerializePartialToString(&ss_) || !dm.SerializePartialToString(&ds_)) {
		std::cerr << "compareMessage: SerializePartialToString failed.\n";
		return false;
	}
	return ss_ == ds_;
}

}	// protobuf

}	// google
