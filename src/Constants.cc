/*
 * Constants.cc
 *
 *  Created on: 2015年9月29日
 *      Author: weimi_000
 */

#include <iostream>
#include <string>

#include "Constants.h"

namespace MicroWireless {

namespace OneM2M {

using namespace MicroWireless::OneM2M;

// ResourceBase
const std::string& AttrName::TY()   { static const std::string s_ = "ty";  return s_; }
const std::string& AttrName::RI()   { static const std::string s_ = "ri";  return s_; }
const std::string& AttrName::RN()   { static const std::string s_ = "rn";  return s_; }
const std::string& AttrName::PI()   { static const std::string s_ = "pi";  return s_; }
const std::string& AttrName::CT()   { static const std::string s_ = "ct";  return s_; }
const std::string& AttrName::LT()   { static const std::string s_ = "lt";  return s_; }
const std::string& AttrName::ET()   { static const std::string s_ = "et";  return s_; }
const std::string& AttrName::ACPI() { static const std::string s_ = "acpi";return s_; }
const std::string& AttrName::LBL()  { static const std::string s_ = "lbl"; return s_; }
const std::string& AttrName::AA()   { static const std::string s_ = "aa";  return s_; }
const std::string& AttrName::AT()   { static const std::string s_ = "at";  return s_; }
const std::string& AttrName::ST()   { static const std::string s_ = "st";  return s_; }

// Common
const std::string& AttrName::POA()  { static const std::string s_ = "poa"; return s_; }
const std::string& AttrName::RR()   { static const std::string s_ = "rr";  return s_; }
const std::string& AttrName::NL()   { static const std::string s_ = "nl";  return s_; }

// CSE

// AE
const std::string& AttrName::APN()  { static const std::string s_ = "apn"; return s_; }
const std::string& AttrName::API()  { static const std::string s_ = "api"; return s_; }
const std::string& AttrName::AEI()  { static const std::string s_ = "aei"; return s_; }
const std::string& AttrName::OR()   { static const std::string s_ = "or";  return s_; }

// RemoteCSE
const std::string& AttrName::CST()  { static const std::string s_ = "cst"; return s_; }
const std::string& AttrName::CB()   { static const std::string s_ = "cb";  return s_; }
const std::string& AttrName::CSI()  { static const std::string s_ = "csi"; return s_; }
const std::string& AttrName::MEI()  { static const std::string s_ = "mei"; return s_; }
const std::string& AttrName::TRI()  { static const std::string s_ = "tri"; return s_; }

}	// OneM2M

}	// MicroWireless
