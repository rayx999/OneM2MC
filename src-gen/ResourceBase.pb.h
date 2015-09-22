// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ResourceBase.proto

#ifndef PROTOBUF_ResourceBase_2eproto__INCLUDED
#define PROTOBUF_ResourceBase_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "google/protobuf/timestamp.pb.h"
#include "CommonTypes.pb.h"
#include "CSEBase.pb.h"
#include "AE.pb.h"
#include "Request.pb.h"
// @@protoc_insertion_point(includes)

namespace MicroWireless {
namespace OneM2M {
namespace pb {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_ResourceBase_2eproto();
void protobuf_AssignDesc_ResourceBase_2eproto();
void protobuf_ShutdownFile_ResourceBase_2eproto();

class ResourceBase;

// ===================================================================

class ResourceBase : public ::google::protobuf::Message {
 public:
  ResourceBase();
  virtual ~ResourceBase();

  ResourceBase(const ResourceBase& from);

  inline ResourceBase& operator=(const ResourceBase& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ResourceBase& default_instance();

  enum ResourceCase {
    kAe = 30002,
    kAeA = 40002,
    kCsb = 30005,
    kReq = 30017,
    RESOURCE_NOT_SET = 0,
  };

  void Swap(ResourceBase* other);

  // implements Message ----------------------------------------------

  inline ResourceBase* New() const { return New(NULL); }

  ResourceBase* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ResourceBase& from);
  void MergeFrom(const ResourceBase& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(ResourceBase* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .MicroWireless.OneM2M.pb.CommonTypes.SupportedResourceType ty = 1;
  void clear_ty();
  static const int kTyFieldNumber = 1;
  ::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType ty() const;
  void set_ty(::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType value);

  // optional string ri = 2;
  void clear_ri();
  static const int kRiFieldNumber = 2;
  const ::std::string& ri() const;
  void set_ri(const ::std::string& value);
  void set_ri(const char* value);
  void set_ri(const char* value, size_t size);
  ::std::string* mutable_ri();
  ::std::string* release_ri();
  void set_allocated_ri(::std::string* ri);

  // optional string rn = 3;
  void clear_rn();
  static const int kRnFieldNumber = 3;
  const ::std::string& rn() const;
  void set_rn(const ::std::string& value);
  void set_rn(const char* value);
  void set_rn(const char* value, size_t size);
  ::std::string* mutable_rn();
  ::std::string* release_rn();
  void set_allocated_rn(::std::string* rn);

  // optional string pi = 4;
  void clear_pi();
  static const int kPiFieldNumber = 4;
  const ::std::string& pi() const;
  void set_pi(const ::std::string& value);
  void set_pi(const char* value);
  void set_pi(const char* value, size_t size);
  ::std::string* mutable_pi();
  ::std::string* release_pi();
  void set_allocated_pi(::std::string* pi);

  // optional .google.protobuf.Timestamp ct = 5;
  bool has_ct() const;
  void clear_ct();
  static const int kCtFieldNumber = 5;
  const ::google::protobuf::Timestamp& ct() const;
  ::google::protobuf::Timestamp* mutable_ct();
  ::google::protobuf::Timestamp* release_ct();
  void set_allocated_ct(::google::protobuf::Timestamp* ct);

  // optional .google.protobuf.Timestamp lt = 6;
  bool has_lt() const;
  void clear_lt();
  static const int kLtFieldNumber = 6;
  const ::google::protobuf::Timestamp& lt() const;
  ::google::protobuf::Timestamp* mutable_lt();
  ::google::protobuf::Timestamp* release_lt();
  void set_allocated_lt(::google::protobuf::Timestamp* lt);

  // optional .google.protobuf.Timestamp et = 7;
  bool has_et() const;
  void clear_et();
  static const int kEtFieldNumber = 7;
  const ::google::protobuf::Timestamp& et() const;
  ::google::protobuf::Timestamp* mutable_et();
  ::google::protobuf::Timestamp* release_et();
  void set_allocated_et(::google::protobuf::Timestamp* et);

  // repeated string acpi = 8;
  int acpi_size() const;
  void clear_acpi();
  static const int kAcpiFieldNumber = 8;
  const ::std::string& acpi(int index) const;
  ::std::string* mutable_acpi(int index);
  void set_acpi(int index, const ::std::string& value);
  void set_acpi(int index, const char* value);
  void set_acpi(int index, const char* value, size_t size);
  ::std::string* add_acpi();
  void add_acpi(const ::std::string& value);
  void add_acpi(const char* value);
  void add_acpi(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& acpi() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_acpi();

  // repeated string lbl = 9;
  int lbl_size() const;
  void clear_lbl();
  static const int kLblFieldNumber = 9;
  const ::std::string& lbl(int index) const;
  ::std::string* mutable_lbl(int index);
  void set_lbl(int index, const ::std::string& value);
  void set_lbl(int index, const char* value);
  void set_lbl(int index, const char* value, size_t size);
  ::std::string* add_lbl();
  void add_lbl(const ::std::string& value);
  void add_lbl(const char* value);
  void add_lbl(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& lbl() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_lbl();

  // optional string aa = 10;
  void clear_aa();
  static const int kAaFieldNumber = 10;
  const ::std::string& aa() const;
  void set_aa(const ::std::string& value);
  void set_aa(const char* value);
  void set_aa(const char* value, size_t size);
  ::std::string* mutable_aa();
  ::std::string* release_aa();
  void set_allocated_aa(::std::string* aa);

  // optional string at = 11;
  void clear_at();
  static const int kAtFieldNumber = 11;
  const ::std::string& at() const;
  void set_at(const ::std::string& value);
  void set_at(const char* value);
  void set_at(const char* value, size_t size);
  ::std::string* mutable_at();
  ::std::string* release_at();
  void set_allocated_at(::std::string* at);

  // optional int32 st = 12;
  void clear_st();
  static const int kStFieldNumber = 12;
  ::google::protobuf::int32 st() const;
  void set_st(::google::protobuf::int32 value);

  // optional .MicroWireless.OneM2M.pb.AE ae = 30002;
  bool has_ae() const;
  void clear_ae();
  static const int kAeFieldNumber = 30002;
  const ::MicroWireless::OneM2M::pb::AE& ae() const;
  ::MicroWireless::OneM2M::pb::AE* mutable_ae();
  ::MicroWireless::OneM2M::pb::AE* release_ae();
  void set_allocated_ae(::MicroWireless::OneM2M::pb::AE* ae);

  // optional .MicroWireless.OneM2M.pb.AE aeA = 40002;
  bool has_aea() const;
  void clear_aea();
  static const int kAeAFieldNumber = 40002;
  const ::MicroWireless::OneM2M::pb::AE& aea() const;
  ::MicroWireless::OneM2M::pb::AE* mutable_aea();
  ::MicroWireless::OneM2M::pb::AE* release_aea();
  void set_allocated_aea(::MicroWireless::OneM2M::pb::AE* aea);

  // optional .MicroWireless.OneM2M.pb.CSEBase csb = 30005;
  bool has_csb() const;
  void clear_csb();
  static const int kCsbFieldNumber = 30005;
  const ::MicroWireless::OneM2M::pb::CSEBase& csb() const;
  ::MicroWireless::OneM2M::pb::CSEBase* mutable_csb();
  ::MicroWireless::OneM2M::pb::CSEBase* release_csb();
  void set_allocated_csb(::MicroWireless::OneM2M::pb::CSEBase* csb);

  // optional .MicroWireless.OneM2M.pb.Request req = 30017;
  bool has_req() const;
  void clear_req();
  static const int kReqFieldNumber = 30017;
  const ::MicroWireless::OneM2M::pb::Request& req() const;
  ::MicroWireless::OneM2M::pb::Request* mutable_req();
  ::MicroWireless::OneM2M::pb::Request* release_req();
  void set_allocated_req(::MicroWireless::OneM2M::pb::Request* req);

  ResourceCase resource_case() const;
  // @@protoc_insertion_point(class_scope:MicroWireless.OneM2M.pb.ResourceBase)
 private:
  inline void set_has_ae();
  inline void set_has_aea();
  inline void set_has_csb();
  inline void set_has_req();

  inline bool has_resource() const;
  void clear_resource();
  inline void clear_has_resource();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr ri_;
  ::google::protobuf::internal::ArenaStringPtr rn_;
  ::google::protobuf::internal::ArenaStringPtr pi_;
  ::google::protobuf::Timestamp* ct_;
  ::google::protobuf::Timestamp* lt_;
  int ty_;
  ::google::protobuf::int32 st_;
  ::google::protobuf::Timestamp* et_;
  ::google::protobuf::RepeatedPtrField< ::std::string> acpi_;
  ::google::protobuf::RepeatedPtrField< ::std::string> lbl_;
  ::google::protobuf::internal::ArenaStringPtr aa_;
  ::google::protobuf::internal::ArenaStringPtr at_;
  union ResourceUnion {
    ResourceUnion() {}
    ::MicroWireless::OneM2M::pb::AE* ae_;
    ::MicroWireless::OneM2M::pb::AE* aea_;
    ::MicroWireless::OneM2M::pb::CSEBase* csb_;
    ::MicroWireless::OneM2M::pb::Request* req_;
  } resource_;
  mutable int _cached_size_;
  ::google::protobuf::uint32 _oneof_case_[1];

  friend void  protobuf_AddDesc_ResourceBase_2eproto();
  friend void protobuf_AssignDesc_ResourceBase_2eproto();
  friend void protobuf_ShutdownFile_ResourceBase_2eproto();

  void InitAsDefaultInstance();
  static ResourceBase* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// ResourceBase

// optional .MicroWireless.OneM2M.pb.CommonTypes.SupportedResourceType ty = 1;
inline void ResourceBase::clear_ty() {
  ty_ = 0;
}
inline ::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType ResourceBase::ty() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.ty)
  return static_cast< ::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType >(ty_);
}
inline void ResourceBase::set_ty(::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType value) {
  
  ty_ = value;
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.ResourceBase.ty)
}

// optional string ri = 2;
inline void ResourceBase::clear_ri() {
  ri_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& ResourceBase::ri() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.ri)
  return ri_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_ri(const ::std::string& value) {
  
  ri_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.ResourceBase.ri)
}
inline void ResourceBase::set_ri(const char* value) {
  
  ri_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.ResourceBase.ri)
}
inline void ResourceBase::set_ri(const char* value, size_t size) {
  
  ri_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.ResourceBase.ri)
}
inline ::std::string* ResourceBase::mutable_ri() {
  
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.ri)
  return ri_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ResourceBase::release_ri() {
  
  return ri_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_allocated_ri(::std::string* ri) {
  if (ri != NULL) {
    
  } else {
    
  }
  ri_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ri);
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.ri)
}

// optional string rn = 3;
inline void ResourceBase::clear_rn() {
  rn_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& ResourceBase::rn() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.rn)
  return rn_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_rn(const ::std::string& value) {
  
  rn_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.ResourceBase.rn)
}
inline void ResourceBase::set_rn(const char* value) {
  
  rn_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.ResourceBase.rn)
}
inline void ResourceBase::set_rn(const char* value, size_t size) {
  
  rn_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.ResourceBase.rn)
}
inline ::std::string* ResourceBase::mutable_rn() {
  
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.rn)
  return rn_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ResourceBase::release_rn() {
  
  return rn_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_allocated_rn(::std::string* rn) {
  if (rn != NULL) {
    
  } else {
    
  }
  rn_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), rn);
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.rn)
}

// optional string pi = 4;
inline void ResourceBase::clear_pi() {
  pi_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& ResourceBase::pi() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.pi)
  return pi_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_pi(const ::std::string& value) {
  
  pi_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.ResourceBase.pi)
}
inline void ResourceBase::set_pi(const char* value) {
  
  pi_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.ResourceBase.pi)
}
inline void ResourceBase::set_pi(const char* value, size_t size) {
  
  pi_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.ResourceBase.pi)
}
inline ::std::string* ResourceBase::mutable_pi() {
  
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.pi)
  return pi_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ResourceBase::release_pi() {
  
  return pi_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_allocated_pi(::std::string* pi) {
  if (pi != NULL) {
    
  } else {
    
  }
  pi_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), pi);
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.pi)
}

// optional .google.protobuf.Timestamp ct = 5;
inline bool ResourceBase::has_ct() const {
  return !_is_default_instance_ && ct_ != NULL;
}
inline void ResourceBase::clear_ct() {
  if (GetArenaNoVirtual() == NULL && ct_ != NULL) delete ct_;
  ct_ = NULL;
}
inline const ::google::protobuf::Timestamp& ResourceBase::ct() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.ct)
  return ct_ != NULL ? *ct_ : *default_instance_->ct_;
}
inline ::google::protobuf::Timestamp* ResourceBase::mutable_ct() {
  
  if (ct_ == NULL) {
    ct_ = new ::google::protobuf::Timestamp;
  }
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.ct)
  return ct_;
}
inline ::google::protobuf::Timestamp* ResourceBase::release_ct() {
  
  ::google::protobuf::Timestamp* temp = ct_;
  ct_ = NULL;
  return temp;
}
inline void ResourceBase::set_allocated_ct(::google::protobuf::Timestamp* ct) {
  delete ct_;
  ct_ = ct;
  if (ct) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.ct)
}

// optional .google.protobuf.Timestamp lt = 6;
inline bool ResourceBase::has_lt() const {
  return !_is_default_instance_ && lt_ != NULL;
}
inline void ResourceBase::clear_lt() {
  if (GetArenaNoVirtual() == NULL && lt_ != NULL) delete lt_;
  lt_ = NULL;
}
inline const ::google::protobuf::Timestamp& ResourceBase::lt() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.lt)
  return lt_ != NULL ? *lt_ : *default_instance_->lt_;
}
inline ::google::protobuf::Timestamp* ResourceBase::mutable_lt() {
  
  if (lt_ == NULL) {
    lt_ = new ::google::protobuf::Timestamp;
  }
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.lt)
  return lt_;
}
inline ::google::protobuf::Timestamp* ResourceBase::release_lt() {
  
  ::google::protobuf::Timestamp* temp = lt_;
  lt_ = NULL;
  return temp;
}
inline void ResourceBase::set_allocated_lt(::google::protobuf::Timestamp* lt) {
  delete lt_;
  lt_ = lt;
  if (lt) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.lt)
}

// optional .google.protobuf.Timestamp et = 7;
inline bool ResourceBase::has_et() const {
  return !_is_default_instance_ && et_ != NULL;
}
inline void ResourceBase::clear_et() {
  if (GetArenaNoVirtual() == NULL && et_ != NULL) delete et_;
  et_ = NULL;
}
inline const ::google::protobuf::Timestamp& ResourceBase::et() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.et)
  return et_ != NULL ? *et_ : *default_instance_->et_;
}
inline ::google::protobuf::Timestamp* ResourceBase::mutable_et() {
  
  if (et_ == NULL) {
    et_ = new ::google::protobuf::Timestamp;
  }
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.et)
  return et_;
}
inline ::google::protobuf::Timestamp* ResourceBase::release_et() {
  
  ::google::protobuf::Timestamp* temp = et_;
  et_ = NULL;
  return temp;
}
inline void ResourceBase::set_allocated_et(::google::protobuf::Timestamp* et) {
  delete et_;
  et_ = et;
  if (et) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.et)
}

// repeated string acpi = 8;
inline int ResourceBase::acpi_size() const {
  return acpi_.size();
}
inline void ResourceBase::clear_acpi() {
  acpi_.Clear();
}
inline const ::std::string& ResourceBase::acpi(int index) const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.acpi)
  return acpi_.Get(index);
}
inline ::std::string* ResourceBase::mutable_acpi(int index) {
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.acpi)
  return acpi_.Mutable(index);
}
inline void ResourceBase::set_acpi(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.ResourceBase.acpi)
  acpi_.Mutable(index)->assign(value);
}
inline void ResourceBase::set_acpi(int index, const char* value) {
  acpi_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.ResourceBase.acpi)
}
inline void ResourceBase::set_acpi(int index, const char* value, size_t size) {
  acpi_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.ResourceBase.acpi)
}
inline ::std::string* ResourceBase::add_acpi() {
  return acpi_.Add();
}
inline void ResourceBase::add_acpi(const ::std::string& value) {
  acpi_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:MicroWireless.OneM2M.pb.ResourceBase.acpi)
}
inline void ResourceBase::add_acpi(const char* value) {
  acpi_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:MicroWireless.OneM2M.pb.ResourceBase.acpi)
}
inline void ResourceBase::add_acpi(const char* value, size_t size) {
  acpi_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:MicroWireless.OneM2M.pb.ResourceBase.acpi)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
ResourceBase::acpi() const {
  // @@protoc_insertion_point(field_list:MicroWireless.OneM2M.pb.ResourceBase.acpi)
  return acpi_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
ResourceBase::mutable_acpi() {
  // @@protoc_insertion_point(field_mutable_list:MicroWireless.OneM2M.pb.ResourceBase.acpi)
  return &acpi_;
}

// repeated string lbl = 9;
inline int ResourceBase::lbl_size() const {
  return lbl_.size();
}
inline void ResourceBase::clear_lbl() {
  lbl_.Clear();
}
inline const ::std::string& ResourceBase::lbl(int index) const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.lbl)
  return lbl_.Get(index);
}
inline ::std::string* ResourceBase::mutable_lbl(int index) {
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.lbl)
  return lbl_.Mutable(index);
}
inline void ResourceBase::set_lbl(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.ResourceBase.lbl)
  lbl_.Mutable(index)->assign(value);
}
inline void ResourceBase::set_lbl(int index, const char* value) {
  lbl_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.ResourceBase.lbl)
}
inline void ResourceBase::set_lbl(int index, const char* value, size_t size) {
  lbl_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.ResourceBase.lbl)
}
inline ::std::string* ResourceBase::add_lbl() {
  return lbl_.Add();
}
inline void ResourceBase::add_lbl(const ::std::string& value) {
  lbl_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:MicroWireless.OneM2M.pb.ResourceBase.lbl)
}
inline void ResourceBase::add_lbl(const char* value) {
  lbl_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:MicroWireless.OneM2M.pb.ResourceBase.lbl)
}
inline void ResourceBase::add_lbl(const char* value, size_t size) {
  lbl_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:MicroWireless.OneM2M.pb.ResourceBase.lbl)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
ResourceBase::lbl() const {
  // @@protoc_insertion_point(field_list:MicroWireless.OneM2M.pb.ResourceBase.lbl)
  return lbl_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
ResourceBase::mutable_lbl() {
  // @@protoc_insertion_point(field_mutable_list:MicroWireless.OneM2M.pb.ResourceBase.lbl)
  return &lbl_;
}

// optional string aa = 10;
inline void ResourceBase::clear_aa() {
  aa_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& ResourceBase::aa() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.aa)
  return aa_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_aa(const ::std::string& value) {
  
  aa_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.ResourceBase.aa)
}
inline void ResourceBase::set_aa(const char* value) {
  
  aa_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.ResourceBase.aa)
}
inline void ResourceBase::set_aa(const char* value, size_t size) {
  
  aa_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.ResourceBase.aa)
}
inline ::std::string* ResourceBase::mutable_aa() {
  
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.aa)
  return aa_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ResourceBase::release_aa() {
  
  return aa_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_allocated_aa(::std::string* aa) {
  if (aa != NULL) {
    
  } else {
    
  }
  aa_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), aa);
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.aa)
}

// optional string at = 11;
inline void ResourceBase::clear_at() {
  at_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& ResourceBase::at() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.at)
  return at_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_at(const ::std::string& value) {
  
  at_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.ResourceBase.at)
}
inline void ResourceBase::set_at(const char* value) {
  
  at_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.ResourceBase.at)
}
inline void ResourceBase::set_at(const char* value, size_t size) {
  
  at_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.ResourceBase.at)
}
inline ::std::string* ResourceBase::mutable_at() {
  
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.at)
  return at_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* ResourceBase::release_at() {
  
  return at_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void ResourceBase::set_allocated_at(::std::string* at) {
  if (at != NULL) {
    
  } else {
    
  }
  at_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), at);
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.at)
}

// optional int32 st = 12;
inline void ResourceBase::clear_st() {
  st_ = 0;
}
inline ::google::protobuf::int32 ResourceBase::st() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.st)
  return st_;
}
inline void ResourceBase::set_st(::google::protobuf::int32 value) {
  
  st_ = value;
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.ResourceBase.st)
}

// optional .MicroWireless.OneM2M.pb.AE ae = 30002;
inline bool ResourceBase::has_ae() const {
  return resource_case() == kAe;
}
inline void ResourceBase::set_has_ae() {
  _oneof_case_[0] = kAe;
}
inline void ResourceBase::clear_ae() {
  if (has_ae()) {
    delete resource_.ae_;
    clear_has_resource();
  }
}
inline const ::MicroWireless::OneM2M::pb::AE& ResourceBase::ae() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.ae)
  return has_ae() ? *resource_.ae_
                      : ::MicroWireless::OneM2M::pb::AE::default_instance();
}
inline ::MicroWireless::OneM2M::pb::AE* ResourceBase::mutable_ae() {
  if (!has_ae()) {
    clear_resource();
    set_has_ae();
    resource_.ae_ = new ::MicroWireless::OneM2M::pb::AE;
  }
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.ae)
  return resource_.ae_;
}
inline ::MicroWireless::OneM2M::pb::AE* ResourceBase::release_ae() {
  if (has_ae()) {
    clear_has_resource();
    ::MicroWireless::OneM2M::pb::AE* temp = resource_.ae_;
    resource_.ae_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
inline void ResourceBase::set_allocated_ae(::MicroWireless::OneM2M::pb::AE* ae) {
  clear_resource();
  if (ae) {
    set_has_ae();
    resource_.ae_ = ae;
  }
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.ae)
}

// optional .MicroWireless.OneM2M.pb.AE aeA = 40002;
inline bool ResourceBase::has_aea() const {
  return resource_case() == kAeA;
}
inline void ResourceBase::set_has_aea() {
  _oneof_case_[0] = kAeA;
}
inline void ResourceBase::clear_aea() {
  if (has_aea()) {
    delete resource_.aea_;
    clear_has_resource();
  }
}
inline const ::MicroWireless::OneM2M::pb::AE& ResourceBase::aea() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.aeA)
  return has_aea() ? *resource_.aea_
                      : ::MicroWireless::OneM2M::pb::AE::default_instance();
}
inline ::MicroWireless::OneM2M::pb::AE* ResourceBase::mutable_aea() {
  if (!has_aea()) {
    clear_resource();
    set_has_aea();
    resource_.aea_ = new ::MicroWireless::OneM2M::pb::AE;
  }
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.aeA)
  return resource_.aea_;
}
inline ::MicroWireless::OneM2M::pb::AE* ResourceBase::release_aea() {
  if (has_aea()) {
    clear_has_resource();
    ::MicroWireless::OneM2M::pb::AE* temp = resource_.aea_;
    resource_.aea_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
inline void ResourceBase::set_allocated_aea(::MicroWireless::OneM2M::pb::AE* aea) {
  clear_resource();
  if (aea) {
    set_has_aea();
    resource_.aea_ = aea;
  }
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.aeA)
}

// optional .MicroWireless.OneM2M.pb.CSEBase csb = 30005;
inline bool ResourceBase::has_csb() const {
  return resource_case() == kCsb;
}
inline void ResourceBase::set_has_csb() {
  _oneof_case_[0] = kCsb;
}
inline void ResourceBase::clear_csb() {
  if (has_csb()) {
    delete resource_.csb_;
    clear_has_resource();
  }
}
inline const ::MicroWireless::OneM2M::pb::CSEBase& ResourceBase::csb() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.csb)
  return has_csb() ? *resource_.csb_
                      : ::MicroWireless::OneM2M::pb::CSEBase::default_instance();
}
inline ::MicroWireless::OneM2M::pb::CSEBase* ResourceBase::mutable_csb() {
  if (!has_csb()) {
    clear_resource();
    set_has_csb();
    resource_.csb_ = new ::MicroWireless::OneM2M::pb::CSEBase;
  }
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.csb)
  return resource_.csb_;
}
inline ::MicroWireless::OneM2M::pb::CSEBase* ResourceBase::release_csb() {
  if (has_csb()) {
    clear_has_resource();
    ::MicroWireless::OneM2M::pb::CSEBase* temp = resource_.csb_;
    resource_.csb_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
inline void ResourceBase::set_allocated_csb(::MicroWireless::OneM2M::pb::CSEBase* csb) {
  clear_resource();
  if (csb) {
    set_has_csb();
    resource_.csb_ = csb;
  }
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.csb)
}

// optional .MicroWireless.OneM2M.pb.Request req = 30017;
inline bool ResourceBase::has_req() const {
  return resource_case() == kReq;
}
inline void ResourceBase::set_has_req() {
  _oneof_case_[0] = kReq;
}
inline void ResourceBase::clear_req() {
  if (has_req()) {
    delete resource_.req_;
    clear_has_resource();
  }
}
inline const ::MicroWireless::OneM2M::pb::Request& ResourceBase::req() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.ResourceBase.req)
  return has_req() ? *resource_.req_
                      : ::MicroWireless::OneM2M::pb::Request::default_instance();
}
inline ::MicroWireless::OneM2M::pb::Request* ResourceBase::mutable_req() {
  if (!has_req()) {
    clear_resource();
    set_has_req();
    resource_.req_ = new ::MicroWireless::OneM2M::pb::Request;
  }
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.ResourceBase.req)
  return resource_.req_;
}
inline ::MicroWireless::OneM2M::pb::Request* ResourceBase::release_req() {
  if (has_req()) {
    clear_has_resource();
    ::MicroWireless::OneM2M::pb::Request* temp = resource_.req_;
    resource_.req_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
inline void ResourceBase::set_allocated_req(::MicroWireless::OneM2M::pb::Request* req) {
  clear_resource();
  if (req) {
    set_has_req();
    resource_.req_ = req;
  }
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.ResourceBase.req)
}

inline bool ResourceBase::has_resource() const {
  return resource_case() != RESOURCE_NOT_SET;
}
inline void ResourceBase::clear_has_resource() {
  _oneof_case_[0] = RESOURCE_NOT_SET;
}
inline ResourceBase::ResourceCase ResourceBase::resource_case() const {
  return ResourceBase::ResourceCase(_oneof_case_[0]);
}
#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace OneM2M
}  // namespace MicroWireless

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ResourceBase_2eproto__INCLUDED
