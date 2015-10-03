/*
 * CommonTypes.h
 *
 *  Created on: 2015年7月15日
 *      Author: weimi_000
 */

#ifndef INCLUDE_COMMONTYPES_H_
#define INCLUDE_COMMONTYPES_H_


#include <sys/time.h>
#include <iostream>
#include <string>
#include <set>

#include "Constants.h"

namespace MicroWireless {

namespace OneM2M {

typedef struct timeval	TimeStamp;
typedef struct timeval	Duration;

enum class ResourceType
{
  NORMAL = 1,
  VIRTUAL = 2,
  ANNOUNCED = 3,
};

enum class CSEType
{
  IN_CSE = 1,
  MN_CSE = 2,
  ASN_CSE = 3,
};

// also used for MemberType
enum class SupportedResourceType
{
  SRT_NONE = 0,

  // Normal Resources

  ACCESS_CONTROL_POLICY = 1,
  AE = 2,
  CONTAINER = 3,
  CONTENT_INSTANCE = 4,
  CSE_BASE = 5,
  DELIVERY = 6,
  EVENT_CONFIG = 7,
  EXEC_INSTANCE = 8,
  GROUP = 9,
  LOCATION_POLICY = 10,
  MGMT_CMD = 11,
  MGMT_OBJ = 12,
  SERVICE_SUB_PROFILE = 13, // m2mServiceSubscriptionProfile
  NODE = 14,
  POLLING_CHANNEL = 15,
  REMOTE_CSE = 16,
  REQUEST = 17,
  SCHEDULE = 18,
  SERVICE_SUB_NODE = 19,    // serviceSubscribedNode
  STATS_COLLECT = 20,
  STATS_CONFIG = 21,
  SUBSCRIPTION = 22,
  SERVICE_SUB_APP_RULE = 23,    // serviceSubscribedAppRule

  MEMBER_MIXED = 24,	// for MemberType only

  // Announced Resources, not for MemberType

  ACCESS_CONTROL_POLICY_ANNC = 10001,
  AE_ANNC = 10002,
  CONTAINER_ANNC = 10003,
  CONTENT_INSTANCE_ANNC = 10004,
  GROUP_ANNC = 10009,
  LOCATION_POLICY_ANNC = 10010,
  MGMT_OBJ_ANNC = 10012,
  NODE_ANNC = 10014,
  REMOTE_CSE_ANNC = 10016,
  SCHEDULE_ANNC = 10018,

  // Virtual Resources, not for MemberType

  LATEST = 20001,
  OLDEST = 20002,
  FAN_OUT_POINT = 20003,
  POLLING_CHANNEL_URI = 20004
};

enum class LocationSource
{
  NETWORK_BASED = 1,
  DEVICE_BASED = 2,
  SHARING_BASED = 3,
};

enum class EventCat
{
  // stdEventCats from 1 to 4:

  DEFAULT = 1,
  IMMEDIATE = 2,
  BEST_EFFORT = 3,
  LATEST = 4,

  // user defined events 100-999
};

// RFC 6838 defined media types
enum class ContentType
{
  APPLICATION_XML = 1,
  APPLICATION_JSON = 2,
};

enum class FilterUsage
{
  DISCOVERY_CRITERIA = 1,
  EVENT_NOTIFICATION_CRITERIA = 2,
};

enum class DiscoveryResultType
{
  HIERARCHICAL = 1,
  NON_HIERARCHICAL = 2,
  CSEID_RESOURCEID = 3,
};

enum class Operation
{
  CREATE = 1,
  RETRIEVE = 2,
  UPDATE = 3,
  DDELETE = 4,	// prefix 'D' to avoid wired conflict using boost
  NOTIFY = 5,
};

enum class ResponseType
{
  NON_BLOCKING_REQUEST_SYNC = 1,
  NON_BLOCKING_REQUEST_ASYNC = 2,
  BLOCKING_REQUEST = 3,
};

enum class ResultContent
{
  NOTHING = 0,
  ATTRIBUTES = 1,
  HADDR = 2,	// Hierachical address
  HADDR_ATTR = 3,	// Hierachical address and attributes
  ATTR_CHILD = 4,	// Attributes and child
  ATTR_CHILD_REF = 5,	// Attributes and child reference
  CHILD = 6,	// Child resource references
  ORIGINAL = 7,	// Original resource
};

enum class RequestStatus
{
  COMPLETED = 1,
  FAILED = 2,
  PENDING = 3,
  FORWARDED = 4,
};

enum class ConsistencyStrategy
{
  ABONDON_MEMBER = 1,
  ABONDON_GROUP = 2,
  SET_MIXED = 3,
};

enum class CmdType
{
  RESET = 1,
  REBOOT = 2,
  UPLOAD = 3,
  DOWNLOAD = 4,
  SW_INSTALL = 5,
  SW_UNINSTALL = 6,
  SW_UPDATE = 7,
};

enum class ExecModeType
{
  IMMEDIATE_ONCE = 1,
  IMMEDIATE_REPEAT = 2,
  RANDOM_ONCE = 3,
  RANDOM_REPEAT = 4,
};

enum class ExecStatusType
{
  INITIATED = 1,
  PENDING = 2,
  FINISHED = 3,
  CANCELLING = 4,
  CANCELLED = 5,
  UNCANCELLABLE = 6,
};

enum class ExecResultType
{
  EXEC_RESULT_TYPE_NONE = 0,

};

 enum class ResponseStatusCode
 {
   NONE = 0,

    // Informational response class
   ACCEPTED = 1000,

   // Successful response class
   OK = 2000,
   CREATED = 2001,
   DELETED = 2002,
   CHANGED = 2003,

   // Redirection class

   // Originator Error class
   BAD_REQUEST = 4000,
   NOT_FOUND = 4004,
   OPERATION_NOT_ALLOWED = 4005,
   REQUEST_TIMEOUT = 4008,
   SUB_CREATOR_HAS_NO_PRIVILEGE = 4101,
   CONTENTS_UNACCEPTABLE = 4102,
   ACCESS_DENIED = 4103,
   GROUP_REQUEST_ID_EXISTS = 4104,
   CONFLICT = 4105,

   // Receiver error class
   INTERNAL_SERVER_ERROR = 5000,
   NOT_IMPLEMENTED = 5001,
   TARGET_NOT_REACHABLE = 5103,
   NO_PREVILEGE = 5105,
   ALREADY_EXISTS = 5106,
   TAGET_NOT_SUBSCIBABLE = 5203,
   SUB_VERIFICATION_INIT_FAILED = 5204,
   SUB_HOST_HAS_NO_PRIVILEGE = 5205,
   NON_BLOCKING_REQUEST_NOT_SUPPORTED = 5206,

   // Network error class
   EXT_OBJ_NOT_REACHABLE = 6003,
   EXT_OBJ_NOT_FOUND = 6005,
   MAX_MEMBER_EXCEEDED = 6010,
   MEMBER_TYPE_INCONSISTENT = 6011,
   MGMT_SESSION_CANNOT_ESTABLISH = 6020,
   MGMT_SESSION_ESTABLISHMENT_TIMEOUT = 6021,
   INVALID_CMDTYPE = 6022,
   INVALID_ARGUMENTS = 6023,
   INSUFFICIENT_ARGUMENTS = 6024,
   MGMT_CONVERSION_ERROR = 6025,
   MGMT_CANCELATION_FAILURE = 6026,
   ALREADY_COMPLETE = 6028,
   COMMAND_NOT_CANCALLABLE = 6029,

};

enum class AnncType {
	MA = 0,
	OA = 1
};

typedef std::set<std::string> AnncAttr;

}	// OneM2M
}	// MicroWireless

#endif /* INCLUDE_COMMONTYPES_H_ */
