// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CSEBase.proto

#ifndef PROTOBUF_CSEBase_2eproto__INCLUDED
#define PROTOBUF_CSEBase_2eproto__INCLUDED

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
#include "CommonTypes.pb.h"
// @@protoc_insertion_point(includes)

namespace MicroWireless {
namespace OneM2M {
namespace pb {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_CSEBase_2eproto();
void protobuf_AssignDesc_CSEBase_2eproto();
void protobuf_ShutdownFile_CSEBase_2eproto();

class CSEBase;

// ===================================================================

class CSEBase : public ::google::protobuf::Message {
 public:
  CSEBase();
  virtual ~CSEBase();

  CSEBase(const CSEBase& from);

  inline CSEBase& operator=(const CSEBase& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const CSEBase& default_instance();

  void Swap(CSEBase* other);

  // implements Message ----------------------------------------------

  inline CSEBase* New() const { return New(NULL); }

  CSEBase* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CSEBase& from);
  void MergeFrom(const CSEBase& from);
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
  void InternalSwap(CSEBase* other);
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

  // optional .MicroWireless.OneM2M.pb.CommonTypes.CSEType cst = 100;
  void clear_cst();
  static const int kCstFieldNumber = 100;
  ::MicroWireless::OneM2M::pb::CommonTypes_CSEType cst() const;
  void set_cst(::MicroWireless::OneM2M::pb::CommonTypes_CSEType value);

  // optional string csi = 101;
  void clear_csi();
  static const int kCsiFieldNumber = 101;
  const ::std::string& csi() const;
  void set_csi(const ::std::string& value);
  void set_csi(const char* value);
  void set_csi(const char* value, size_t size);
  ::std::string* mutable_csi();
  ::std::string* release_csi();
  void set_allocated_csi(::std::string* csi);

  // repeated .MicroWireless.OneM2M.pb.CommonTypes.SupportedResourceType srt = 102;
  int srt_size() const;
  void clear_srt();
  static const int kSrtFieldNumber = 102;
  ::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType srt(int index) const;
  void set_srt(int index, ::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType value);
  void add_srt(::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType value);
  const ::google::protobuf::RepeatedField<int>& srt() const;
  ::google::protobuf::RepeatedField<int>* mutable_srt();

  // repeated string poa = 103;
  int poa_size() const;
  void clear_poa();
  static const int kPoaFieldNumber = 103;
  const ::std::string& poa(int index) const;
  ::std::string* mutable_poa(int index);
  void set_poa(int index, const ::std::string& value);
  void set_poa(int index, const char* value);
  void set_poa(int index, const char* value, size_t size);
  ::std::string* add_poa();
  void add_poa(const ::std::string& value);
  void add_poa(const char* value);
  void add_poa(const char* value, size_t size);
  const ::google::protobuf::RepeatedPtrField< ::std::string>& poa() const;
  ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_poa();

  // optional string nl = 104;
  void clear_nl();
  static const int kNlFieldNumber = 104;
  const ::std::string& nl() const;
  void set_nl(const ::std::string& value);
  void set_nl(const char* value);
  void set_nl(const char* value, size_t size);
  ::std::string* mutable_nl();
  ::std::string* release_nl();
  void set_allocated_nl(::std::string* nl);

  // optional string ncp = 105;
  void clear_ncp();
  static const int kNcpFieldNumber = 105;
  const ::std::string& ncp() const;
  void set_ncp(const ::std::string& value);
  void set_ncp(const char* value);
  void set_ncp(const char* value, size_t size);
  ::std::string* mutable_ncp();
  ::std::string* release_ncp();
  void set_allocated_ncp(::std::string* ncp);

  // @@protoc_insertion_point(class_scope:MicroWireless.OneM2M.pb.CSEBase)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr csi_;
  ::google::protobuf::RepeatedField<int> srt_;
  mutable int _srt_cached_byte_size_;
  ::google::protobuf::RepeatedPtrField< ::std::string> poa_;
  ::google::protobuf::internal::ArenaStringPtr nl_;
  ::google::protobuf::internal::ArenaStringPtr ncp_;
  int cst_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_CSEBase_2eproto();
  friend void protobuf_AssignDesc_CSEBase_2eproto();
  friend void protobuf_ShutdownFile_CSEBase_2eproto();

  void InitAsDefaultInstance();
  static CSEBase* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// CSEBase

// optional .MicroWireless.OneM2M.pb.CommonTypes.CSEType cst = 100;
inline void CSEBase::clear_cst() {
  cst_ = 0;
}
inline ::MicroWireless::OneM2M::pb::CommonTypes_CSEType CSEBase::cst() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.CSEBase.cst)
  return static_cast< ::MicroWireless::OneM2M::pb::CommonTypes_CSEType >(cst_);
}
inline void CSEBase::set_cst(::MicroWireless::OneM2M::pb::CommonTypes_CSEType value) {
  
  cst_ = value;
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.CSEBase.cst)
}

// optional string csi = 101;
inline void CSEBase::clear_csi() {
  csi_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CSEBase::csi() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.CSEBase.csi)
  return csi_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CSEBase::set_csi(const ::std::string& value) {
  
  csi_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.CSEBase.csi)
}
inline void CSEBase::set_csi(const char* value) {
  
  csi_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.CSEBase.csi)
}
inline void CSEBase::set_csi(const char* value, size_t size) {
  
  csi_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.CSEBase.csi)
}
inline ::std::string* CSEBase::mutable_csi() {
  
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.CSEBase.csi)
  return csi_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CSEBase::release_csi() {
  
  return csi_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CSEBase::set_allocated_csi(::std::string* csi) {
  if (csi != NULL) {
    
  } else {
    
  }
  csi_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), csi);
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.CSEBase.csi)
}

// repeated .MicroWireless.OneM2M.pb.CommonTypes.SupportedResourceType srt = 102;
inline int CSEBase::srt_size() const {
  return srt_.size();
}
inline void CSEBase::clear_srt() {
  srt_.Clear();
}
inline ::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType CSEBase::srt(int index) const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.CSEBase.srt)
  return static_cast< ::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType >(srt_.Get(index));
}
inline void CSEBase::set_srt(int index, ::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType value) {
  srt_.Set(index, value);
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.CSEBase.srt)
}
inline void CSEBase::add_srt(::MicroWireless::OneM2M::pb::CommonTypes_SupportedResourceType value) {
  srt_.Add(value);
  // @@protoc_insertion_point(field_add:MicroWireless.OneM2M.pb.CSEBase.srt)
}
inline const ::google::protobuf::RepeatedField<int>&
CSEBase::srt() const {
  // @@protoc_insertion_point(field_list:MicroWireless.OneM2M.pb.CSEBase.srt)
  return srt_;
}
inline ::google::protobuf::RepeatedField<int>*
CSEBase::mutable_srt() {
  // @@protoc_insertion_point(field_mutable_list:MicroWireless.OneM2M.pb.CSEBase.srt)
  return &srt_;
}

// repeated string poa = 103;
inline int CSEBase::poa_size() const {
  return poa_.size();
}
inline void CSEBase::clear_poa() {
  poa_.Clear();
}
inline const ::std::string& CSEBase::poa(int index) const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.CSEBase.poa)
  return poa_.Get(index);
}
inline ::std::string* CSEBase::mutable_poa(int index) {
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.CSEBase.poa)
  return poa_.Mutable(index);
}
inline void CSEBase::set_poa(int index, const ::std::string& value) {
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.CSEBase.poa)
  poa_.Mutable(index)->assign(value);
}
inline void CSEBase::set_poa(int index, const char* value) {
  poa_.Mutable(index)->assign(value);
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.CSEBase.poa)
}
inline void CSEBase::set_poa(int index, const char* value, size_t size) {
  poa_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.CSEBase.poa)
}
inline ::std::string* CSEBase::add_poa() {
  return poa_.Add();
}
inline void CSEBase::add_poa(const ::std::string& value) {
  poa_.Add()->assign(value);
  // @@protoc_insertion_point(field_add:MicroWireless.OneM2M.pb.CSEBase.poa)
}
inline void CSEBase::add_poa(const char* value) {
  poa_.Add()->assign(value);
  // @@protoc_insertion_point(field_add_char:MicroWireless.OneM2M.pb.CSEBase.poa)
}
inline void CSEBase::add_poa(const char* value, size_t size) {
  poa_.Add()->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_add_pointer:MicroWireless.OneM2M.pb.CSEBase.poa)
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
CSEBase::poa() const {
  // @@protoc_insertion_point(field_list:MicroWireless.OneM2M.pb.CSEBase.poa)
  return poa_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
CSEBase::mutable_poa() {
  // @@protoc_insertion_point(field_mutable_list:MicroWireless.OneM2M.pb.CSEBase.poa)
  return &poa_;
}

// optional string nl = 104;
inline void CSEBase::clear_nl() {
  nl_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CSEBase::nl() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.CSEBase.nl)
  return nl_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CSEBase::set_nl(const ::std::string& value) {
  
  nl_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.CSEBase.nl)
}
inline void CSEBase::set_nl(const char* value) {
  
  nl_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.CSEBase.nl)
}
inline void CSEBase::set_nl(const char* value, size_t size) {
  
  nl_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.CSEBase.nl)
}
inline ::std::string* CSEBase::mutable_nl() {
  
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.CSEBase.nl)
  return nl_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CSEBase::release_nl() {
  
  return nl_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CSEBase::set_allocated_nl(::std::string* nl) {
  if (nl != NULL) {
    
  } else {
    
  }
  nl_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), nl);
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.CSEBase.nl)
}

// optional string ncp = 105;
inline void CSEBase::clear_ncp() {
  ncp_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& CSEBase::ncp() const {
  // @@protoc_insertion_point(field_get:MicroWireless.OneM2M.pb.CSEBase.ncp)
  return ncp_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CSEBase::set_ncp(const ::std::string& value) {
  
  ncp_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:MicroWireless.OneM2M.pb.CSEBase.ncp)
}
inline void CSEBase::set_ncp(const char* value) {
  
  ncp_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:MicroWireless.OneM2M.pb.CSEBase.ncp)
}
inline void CSEBase::set_ncp(const char* value, size_t size) {
  
  ncp_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:MicroWireless.OneM2M.pb.CSEBase.ncp)
}
inline ::std::string* CSEBase::mutable_ncp() {
  
  // @@protoc_insertion_point(field_mutable:MicroWireless.OneM2M.pb.CSEBase.ncp)
  return ncp_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* CSEBase::release_ncp() {
  
  return ncp_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void CSEBase::set_allocated_ncp(::std::string* ncp) {
  if (ncp != NULL) {
    
  } else {
    
  }
  ncp_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ncp);
  // @@protoc_insertion_point(field_set_allocated:MicroWireless.OneM2M.pb.CSEBase.ncp)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS

// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace OneM2M
}  // namespace MicroWireless

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_CSEBase_2eproto__INCLUDED
