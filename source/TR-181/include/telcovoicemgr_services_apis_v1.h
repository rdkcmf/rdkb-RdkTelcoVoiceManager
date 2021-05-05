/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 Sky
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef  _TELCOVOICEMGR_SERVICES_APIS_H
#define  _TELCOVOICEMGR_SERVICES_APIS_H

/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include "ansc_platform.h"
#include "ansc_string_util.h"

#define SYSEVENT_UPDATE_IFNAME             "telcovoicemgr_bound_ifname"
#define SYSEVENT_UPDATE_IPFAMILY           "telcovoicemgr_ipaddr_family"
#define MAX_INTERFACES                     10
#define MAX_MARK_ENTRY                     10
#define STR_TRUE                           "true"
#define STR_FALSE                          "false"
#define WAN_DBUS_PATH                      "/com/cisco/spvtg/ccsp/wanmanager"
#define WAN_COMPONENT_NAME                 "eRT.com.cisco.spvtg.ccsp.wanmanager"
#define TABLE_NAME_WAN_INTERFACE           "Device.X_RDK_WanManager.CPEInterface."
#define TABLE_NAME_WAN_MARKING             "Marking."
#define PARAM_NAME_WAN_ENABLE              "Wan.Enable"
#define PARAM_NAME_WAN_REFRESH             "Wan.Refresh"
#define PARAM_NAME_MARK_ALIAS              "Alias"
#define PARAM_NAME_ETHERNET_PRIORITY_MARK  "EthernetPriorityMark"
#define PARAM_NAME_SKB_MARK                "SKBMark"
#define PARAM_NAME_DSCP_MARK               "DSCPMark"
#define PARAM_NAME_IPV4_STATE              "IP.IPv4Status"
#define PARAM_NAME_IPV6_STATE              "IP.IPv6Status"
#define PARAM_VALUE_SIP_ALIAS              "voicesip1"
#define PARAM_VALUE_RTP_ALIAS              "voicertp1"
#define PARAM_VALUE_IP_STATE_UP            "Up"
#define STR_IPV4                               "IPv4"
#define STR_IPV6                               "IPv6"

#define IP_ADDR_FAMILY_LENGTH               32
#define BOUND_IF_NAME_LENGTH                32
#define JSON_MAX_VAL_ARR_SIZE               256
#define JSON_MAX_STR_ARR_SIZE               256
#define IP_ADDR_LENGTH                      64
#define BUF_LEN_16                          16
#define BUF_LEN_512                         512
#define BUF_LEN_256                         256

#define  TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES      1
#define  TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE       1
#define  TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE       1
#define  TELCOVOICEMGR_DML_NUMBER_OF_LINE                1


#define TELCOVOICEMGR_DATA_MAX_VOICE_SERVICES                1
#define TELCOVOICEMGR_DATA_MAX_VOICE_CAPABILITIES_CODECS     1
#define TELCOVOICEMGR_DATA_MAX_PROFILE                       1
#define TELCOVOICEMGR_DATA_MAX_LINE                          1
#define TELCOVOICEMGR_DATA_MAX_PHYINTERFACE                  1
#define TELCOVOICEMGR_DATA_MAX_DML_LINE_CODECLIST            1
#define TELCOVOICEMGR_DATA_MAX_DML_LINE_RINGER_PATTERN       1
#define TELCOVOICEMGR_DATA_MAX_DML_LINE_RINGER_DESCRIPTION   1
#define TELCOVOICEMGR_DATA_MAX_DML_LINE_RINGER_EVENT         1
#define TELCOVOICEMGR_DATA_MAX_DML_LINE_SIP_EVENTSUBSCRIBE   1
#define TELCOVOICEMGR_DATA_MAX_DML_TONE_PATTERN              1
#define TELCOVOICEMGR_DATA_MAX_TONE_DESCRIPTION              1
#define TELCOVOICEMGR_DATA_MAX_TONE_EVENT                    1
#define TELCOVOICEMGR_DATA_MAX_DML_LINE_SESSION              1
#define TELCOVOICEMGR_DATA_MAX_DML_SIP_EVENTSUBSCRIBE        1
#define TELCOVOICEMGR_DATA_MAX_DML_SIP_RESPONSEMAP           1
#define TELCOVOICEMGR_DATA_MAX_NUMBERINGPLAN_PREFIXINFO      1
#define TELCOVOICEMGR_DATA_MAX_BUTTON                        1


typedef struct
{
    UINT                   id;   /**< for internal use only */
    BOOL                   ResetStatistics;  /**< ResetStatistics */
    UINT                   PacketsSent; /**< Total number of RTP packets sent for this line. */
    UINT                   PacketsReceived; /**< Total number of RTP payload bytes received for this line. */
    UINT                   BytesSent; /**< Total number of RTP payload bytes sent for this line.  */
    UINT                   BytesReceived; /**< Total number of RTP payload bytes received for this line. */
    UINT                   PacketsLost; /**< Total number of RTP packets that have been lost for this line. */
    UINT                   Overruns;
    UINT                   Underruns;
    UINT                   IncomingCallsReceived; /**< Total incoming calls received. */
    UINT                   IncomingCallsAnswered; /**< Total incoming calls answered by the local user. */
    UINT                   IncomingCallsConnected; /**< Total incoming calls that successfully completed call setup signalling. */
    UINT                   IncomingCallsFailed; /**< Total incoming calls that failed to successfully complete call setup signalling. */
    UINT                   OutgoingCallsAttempted; /**< Total outgoing calls attempted. */
    UINT                   OutgoingCallsAnswered; /**< Total outgoing calls answered by the remote user. */
    UINT                   OutgoingCallsConnected; /**< Total outgoing calls that successfully completed call setup signalling. */
    UINT                   OutgoingCallsFailed; /**< Total outgoing calls that failed to successfully complete call setup signaling. */
    UINT                   CallsDropped;
    UINT                   TotalCallTime;
    UINT                   ServerDownTime;
    UINT                   ReceivePacketLossRate; /**< Current receive packet loss rate in percent, calculated as defined in [section 6.4-RFC3550] */
    UINT                   FarEndPacketLossRate; /**< Current far end receive packet lost rate in percent, calculated as defined in  [Section6.4/RFC3550]. */
    UINT                   ReceiveInterarrivalJitter; /**< Current receive interarrival jitter in�| microseconds. Calculated from J(i) as defined in [Section6.4/RFC3550], with units converted to�| microseconds. */
    UINT                   FarEndInterarrivalJitter; /**< Current Interarrival jitter in microseconds as reported from the far-end device via RTCP. Calculated from J(i) as defined in [Section64./RFC3550], with units converted to microseconds. */
    UINT                   RoundTripDelay; /**< Current round trip delay in microseconds calculated as defined in [section 6.4-RFC3550]. */
    UINT                   AverageReceiveInterarrivalJitter; /**< Average receive interarrival jitter in microseconds since the beginning of the current call. Calculated as the average of D(i,j) as defined in [Section6.4/RFC3550], with units converted to microseconds.*/
    UINT                   AverageFarEndInterarrivalJitter; /**< Average far-end interarrival jitter in microseconds since the beginning of the current call. Calculated as the average of the interarrival jitter values reported by the far-end, with units converted to microseconds. */
    UINT                   AverageRoundTripDelay; /**< Average round trip delay in microseconds since the beginning of the current call. Average of the RoundTripDelay statistic accumulated each time the delay is calculated. */
} TELCOVOICEMGR_DML_VOICESERVICE_STATS;

typedef enum _PROTOCOL_TYPE {
  SIP,
  RTP,
}PROTOCOL_TYPE;

typedef struct {
  PROTOCOL_TYPE protocol;
  int32_t iEthPriorityMark;
  BOOL iUpdateStatus;
}ethPriorityValStruct_t;

typedef struct {
  BOOL bSipMarkRead;
  BOOL bRtpMarkRead;
  UINT sipSkbMark;
  UINT rtpSkbMark;
  char aIpStateParamName[16];
}retSkbMarksStruct_t;

typedef enum
{
    VOICE_HAL_AUTH_UNAME = 0,
    VOICE_HAL_AUTH_PWD
} TELCOVOICEMGR_VOICE_CREDENTIAL_TYPE_ENUM;

typedef enum
{
    VOICE_HAL_AF_INET_V4 = 0,
    VOICE_HAL_AF_INET_V6
}  TELCOVOICEMGR_VOICE_IP_ADD_FAMILY;

typedef enum
{
    VOICE_HAL_IP_LINK_STATE_DOWN = 0,
    VOICE_HAL_IP_LINK_STATE_UP
}  TELCOVOICEMGR_VOICE_IP_LINK_STATE;

typedef  enum
{
    VOICE_CALLING_FEATURE_CALL_WAITING = 0,
    VOICE_CALLING_FEATURE_MSG_WAIT_INDICATOR,
    VOICE_CALLING_FEATURE_CONF_CALL,
    VOICE_CALLING_FEATURE_HOLD,
    VOICE_CALLING_FEATURE_CALLER_ID,
} TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM;

typedef enum _TELCOVOICEMGR_ENABLE_ENUM
{
    DISABLED = 0,
    QUIESCENT,
    ENABLED
} TELCOVOICEMGR_ENABLE_ENUM;

typedef enum _TELCOVOICEMGR_PROFILE_DTMF_METHOD_ENUM
{
    VOICE_PROFILE_DTMF_METHOD_INBAND = 0,
    VOICE_PROFILE_DTMF_METHOD_RFC2833,
    VOICE_PROFILE_DTMF_METHOD_SIPINFO
} TELCOVOICEMGR_PROFILE_DTMF_METHOD_ENUM;

typedef enum _TELCOVOICEMGR_PROFILE_DTMF_METHODG711_ENUM
{
    VOICE_PROFILE_DTMF_METHODG711_INBAND = 0,
    VOICE_PROFILE_DTMF_METHODG711_RFC2833,
    VOICE_PROFILE_DTMF_METHODG711_SIPINFO
} TELCOVOICEMGR_PROFILE_DTMF_METHODG711_ENUM;

typedef enum _TELCOVOICEMGR_PROFILE_FAXPASSTHROUGH_ENUM
{
    VOICE_PROFILE_FAXPASSTHROUGH_DISABLE = 0,
    VOICE_PROFILE_FAXPASSTHROUGH_AUTO,
    VOICE_PROFILE_FAXPASSTHROUGH_FORCE
} TELCOVOICEMGR_PROFILE_FAXPASSTHROUGH_ENUM;

typedef enum _TELCOVOICEMGR_PROFILE_MODEMPASSTHROUGH_ENUM
{
    VOICE_PROFILE_MODEMPASSTHROUGH_DISABLE = 0,
    VOICE_PROFILE_MODEMPASSTHROUGH_AUTO,
    VOICE_PROFILE_MODEMPASSTHROUGH_FORCE
} TELCOVOICEMGR_PROFILE_MODEMPASSTHROUGH_ENUM;

typedef enum _TELCOVOICEMGR_PHYINTERFACE_TESTSTATE_ENUM
{
    PHYINTERFACE_TESTSTATE_NONE = 0,
    PHYINTERFACE_TESTSTATE_REQUESTED,
    PHYINTERFACE_TESTSTATE_COMPLETE,
    PHYINTERFACE_TESTSTATE_ERROR_TESTNOTSUPPORTED
} TELCOVOICEMGR_PHYINTERFACE_TESTSTATE_ENUM;

typedef enum _TELCOVOICEMGR_PHYINTERFACE_TESTSELECTOR_ENUM
{
    PHYINTERFACE_TESTSELECTOR_PHONE_CONNECTIVITY_TEST = 0,
    PHYINTERFACE_TESTSELECTOR_HAZARD_POTENTIAL,
    PHYINTERFACE_TESTSELECTOR_FOREIGN_VOLTAGE,
    PHYINTERFACE_TESTSELECTOR_RESISTIVE_FAULTS,
    PHYINTERFACE_TESTSELECTOR_OFF_HOOK,
    PHYINTERFACE_TESTSELECTOR_REN
} TELCOVOICEMGR_PHYINTERFACE_TESTSELECTOR_ENUM;

typedef enum _TELCOVOICEMGR_SIP_INBOUNDAUTH_ENUM
{
    SIP_INBOUNDAUTH_NONE = 0,
    SIP_INBOUNDAUTH_DIGEST
} TELCOVOICEMGR_SIP_INBOUNDAUTH_ENUM;

typedef enum _TELCOVOICEMGR_MGCP_REGISTERMODE_ENUM
{
    MGCP_REGISTERMODE_WILDCARD= 0,
    MGCP_REGISTERMODE_INDIVIDUAL
} TELCOVOICEMGR_MGCP_REGISTERMODE_ENUM;

typedef enum _TELCOVOICEMGR_LINE_STATUS_ENUM
{
    VOICE_LINE_STATE_UP = 0,
    VOICE_LINE_STATE_INITIALIZING,
    VOICE_LINE_STATE_REGISTERING,
    VOICE_LINE_STATE_UNREGISTERING,
    VOICE_LINE_STATE_ERROR,
    VOICE_LINE_STATE_TESTING,
    VOICE_LINE_STATE_QUIESCENT,
    VOICE_LINE_STATE_DISABLED
} TELCOVOICEMGR_LINE_STATUS_ENUM;

typedef enum _TELCOVOICEMGR_CALL_STATE_ENUM
{
    VOICE_CALL_STATE_IDLE = 0,
    VOICE_CALL_STATE_CALLING,
    VOICE_CALL_STATE_RINGING,
    VOICE_CALL_STATE_CONNECTING,
    VOICE_CALL_STATE_INCALL,
    VOICE_CALL_STATE_HOLD,
    VOICE_CALL_STATE_DISCONNECTING
} TELCOVOICEMGR_CALL_STATE_ENUM;

typedef enum _TELCOVOICEMGR_LINE_CWSTATUS_ENUM
{
    VOICE_CW_STATE_DISABLED = 0,
    VOICE_CW_STATE_IDLE,
    VOICE_CW_STATE_SECONDARY_RINGING,
    VOICE_CW_STATE_SECONDARY_CONNECTING,
    VOICE_CW_STATE_SECONDARY_CONNECTED
} TELCOVOICEMGR_LINE_CWSTATUS_ENUM;

typedef enum _TELCOVOICEMGR_LINE_CONFERENCE_CALLING_STATUS_ENUM
{
    VOICE_CONFERENCE_CALLING_STATE_DISABLED = 0,
    VOICE_CONFERENCE_CALLING_STATE_IDLE,
    VOICE_CONFERENCE_CALLING_STATE_SECONDARY_RINGING,
    VOICE_CONFERENCE_CALLING_STATE_SECONDARY_CONNECTING,
    VOICE_CONFERENCE_CALLING_STATE_SECONDARY_CONNECTED,
    VOICE_CONFERENCE_CALLING_STATE_IN_CONFERENCE_CALL
} TELCOVOICEMGR_LINE_CONFERENCE_CALLING_STATUS_ENUM;

typedef enum _TELCOVOICEMGR_VOICE_ENABLE_ENUM
{
    VOICE_SERVICE_DISABLE = 0,
    VOICE_SERVICE_ENABLE
} TELCOVOICEMGR_VOICE_ENABLE_ENUM;

typedef enum _TELCOVOICEMGR_VOICE_STATUS_ENUM
{
    VOICE_PROCESS_STATUS_STOPPED = 0,
    VOICE_PROCESS_STATUS_STARTING,
    VOICE_PROCESS_STATUS_STARTED,
    VOICE_PROCESS_STATUS_STOPPING,
    VOICE_PROCESS_STATUS_ERROR
} TELCOVOICEMGR_VOICE_STATUS_ENUM;

typedef enum _TELCOVOICEMGR_TCFMETHOD_ENUM
{
    TCFMETHOD_LOCAL = 0,
    TCFMETHOD_NETWORK
} TELCOVOICEMGR_TCFMETHOD_ENUM;

typedef enum _TELCOVOICEMGR_CAPABILITIES_SIP_ROLE_ENUM
{
    SIP_ROLE_USER_AGENT = 0,
    SIP_ROLE_B2B_USER_AGENT,
    SIP_ROLE_OUTBOUND_PROXY
} TELCOVOICEMGR_CAPABILITIES_SIP_ROLE_ENUM;

typedef struct _TELCOVOICEMGR_DML_SERVICE_PROVIDER_INFO
{
    CHAR          Name[BUF_LEN_256];
    CHAR          URL[BUF_LEN_256];
    CHAR          ContactPhoneNumber[BUF_LEN_256];
    CHAR          EmailAddress[BUF_LEN_256];
} TELCOVOICEMGR_DML_SERVICE_PROVIDER_INFO, *PTELCOVOICEMGR_DML_SERVICE_PROVIDER_INFO;

typedef struct _TELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE
{
    ULONG         InstanceNumber;
    CHAR          Alias[64];
    CHAR          Event[32];
    CHAR          Notifier[BUF_LEN_256];
    UINT          NotifierPort;
    CHAR          NotifierTransport[BUF_LEN_256];
    UINT          ExpireTime;
} TELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE, *PTELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE;

typedef struct _DML_SIP_EVENTSUBSCRIBE_CTRL_
{
    TELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE     dml;
    BOOL        updated;
} DML_SIP_EVENTSUBSCRIBE_CTRL_T;

typedef  struct _DML_SIP_EVENTSUBSCRIBE_LIST__
{
    UINT                          ulQuantity;
    DML_SIP_EVENTSUBSCRIBE_CTRL_T*     pdata[TELCOVOICEMGR_DATA_MAX_DML_SIP_EVENTSUBSCRIBE];
} DML_SIP_EVENTSUBSCRIBE_LIST_T;


typedef struct _TELCOVOICEMGR_DML_SIP_RESPONSEMAP
{
    ULONG         InstanceNumber;
    CHAR          Alias[64];
    UINT          SIPResponseNumber;
    CHAR          TextMessage[64];
    UINT          Tone;
} TELCOVOICEMGR_DML_SIP_RESPONSEMAP, *PTELCOVOICEMGR_DML_SIP_RESPONSEMAP;


typedef struct _DML_SIP_RESPONSEMAP_CTRL_
{
    TELCOVOICEMGR_DML_SIP_RESPONSEMAP     dml;
    BOOL        updated;
} DML_SIP_RESPONSEMAP_CTRL_T;

typedef struct _DML_SIP_RESPONSEMAP_LIST__
{
    UINT                            ulQuantity;
    DML_SIP_RESPONSEMAP_CTRL_T*     pdata[TELCOVOICEMGR_DATA_MAX_DML_SIP_RESPONSEMAP];
} DML_SIP_RESPONSEMAP_LIST_T;


typedef struct _TELCOVOICEMGR_DML_SIP
{
    CHAR        ProxyServer[BUF_LEN_256];
    UINT        ProxyServerPort;
    CHAR        ProxyServerTransport[BUF_LEN_256];
    CHAR        RegistrarServer[BUF_LEN_256];
    UINT        RegistrarServerPort;
    CHAR        RegistrarServerTransport[BUF_LEN_256];
    CHAR        UserAgentDomain[BUF_LEN_256];
    UINT        UserAgentPort;
    CHAR        UserAgentTransport[BUF_LEN_256];
    CHAR        OutboundProxy[BUF_LEN_256];
    UINT        OutboundProxyPort;
    CHAR        Organization[BUF_LEN_256];
    UINT        RegistrationPeriod;
    UINT        TimerT1;
    UINT        TimerT2;
    UINT        TimerT4;
    UINT        TimerA;
    UINT        TimerB;
    UINT        TimerC;
    UINT        TimerD;
    UINT        TimerE;
    UINT        TimerF;
    UINT        TimerG;
    UINT        TimerH;
    UINT        TimerI;
    UINT        TimerJ;
    UINT        TimerK;
    UINT        InviteExpires;
    UINT        ReInviteExpires;
    UINT        RegisterExpires;
    UINT        RegistersMinExpires;
    UINT        RegisterRetryInterval;
    TELCOVOICEMGR_SIP_INBOUNDAUTH_ENUM   InboundAuth;
    CHAR        InboundAuthUsername[BUF_LEN_256];
    CHAR        InboundAuthPassword[BUF_LEN_256];
    BOOL        UseCodecPriorityInSDPResponse;
    UINT        DSCPMark;
    INT         VLANIDMark;
    INT         EthernetPriorityMark;
    UINT        X_RDK_SKBMark;
    CHAR        ConferencingURI[BUF_LEN_256];
    BOOL        NetworkDisconnect;
    BOOL        X_RDK_PRACKRequired;
    CHAR        X_RDK_Firewall_Rule_Data[BUF_LEN_256];
    DML_SIP_EVENTSUBSCRIBE_LIST_T   SipEventSubscribeList;
    DML_SIP_RESPONSEMAP_LIST_T     SipResponseMapList;
} TELCOVOICEMGR_DML_SIP, *PTELCOVOICEMGR_DML_SIP;

typedef struct _TELCOVOICEMGR_DML_MGCP
{
    CHAR                                  CallAgent1[BUF_LEN_256];
    UINT                                  CallAgentPort1;
    CHAR                                  CallAgent2[BUF_LEN_256];
    UINT                                  CallAgentPort2;
    UINT                                  RetranIntervalTimer;
    UINT                                  MaxRetranCount;
    TELCOVOICEMGR_MGCP_REGISTERMODE_ENUM  RegisterMode;
    UINT                                  LocalPort;
    CHAR                                  Domain[BUF_LEN_256];
    CHAR                                  User[64];
    UINT                                  DSCPMark;
    INT                                   VLANIDMark;
    INT                                   EthernetPriorityMark;
    BOOL                                  AllowPiggybackEvents;
    BOOL                                  SendRSIPImmediately;
} TELCOVOICEMGR_DML_MGCP, *PTELCOVOICEMGR_DML_MGCP;

typedef struct _TELCOVOICEMGR_DML_H323
{
    CHAR          Gatekeeper[BUF_LEN_256];
    UINT          GatekeeperPort;
    CHAR          GatekeeperID[BUF_LEN_256];
    UINT          TimeToLive;
    BOOL          H235Authentication;
    CHAR          AuthPassword[BUF_LEN_256];
    CHAR          SendersID[BUF_LEN_256];
    UINT          DSCPMark;
    INT           VLANIDMark;
    INT           EthernetPriorityMark;
} TELCOVOICEMGR_DML_H323, *PTELCOVOICEMGR_DML_H323;

typedef struct _TELCOVOICEMGR_DML_RTP_RTCP
{
    BOOL          Enable;
    UINT          TxRepeatInterval;
    CHAR          LocalCName[64];
} TELCOVOICEMGR_DML_RTP_RTCP, *PTELCOVOICEMGR_DML_RTP_RTCP;

typedef struct _TELCOVOICEMGR_DML_RTP_SRTP
{
    BOOL          Enable;
    CHAR          KeyingMethods[BUF_LEN_256];
    CHAR          EncryptionKeySizes[BUF_LEN_256];
} TELCOVOICEMGR_DML_RTP_SRTP, *PTELCOVOICEMGR_DML_RTP_SRTP;

typedef struct _TELCOVOICEMGR_DML_RTP_REDUNDANCY
{
    BOOL          Enable;
    UINT          PayloadType;
    UINT          BlockPayloadType;
    INT           FaxAndModemRedundancy;
    INT           ModemRedundancy;
    INT           DTMFRedundancy;
    INT           VoiceRedundancy;
    UINT          MaxSessionsUsingRedundancy;
} TELCOVOICEMGR_DML_RTP_REDUNDANCY, *PTELCOVOICEMGR_DML_RTP_REDUNDANCY;

typedef struct _TELCOVOICEMGR_DML_RTP
{
    UINT                              LocalPortMin;
    UINT                              LocalPortMax;
    UINT                              DSCPMark;
    INT                               VLANIDMark;
    INT                               EthernetPriorityMark;
    UINT                              TelephoneEventPayloadType;
    UINT                              X_RDK_SKBMark;
    CHAR                              X_RDK_Firewall_Rule_Data[BUF_LEN_256];
    TELCOVOICEMGR_DML_RTP_RTCP        RTCPObj;
    TELCOVOICEMGR_DML_RTP_SRTP        SRTPObj;
    TELCOVOICEMGR_DML_RTP_REDUNDANCY  RedundancyObj;
} TELCOVOICEMGR_DML_RTP, *PTELCOVOICEMGR_DML_RTP;

typedef struct _TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO
{
    UINT           InstanceNumber;
    void           *pParentVoiceProfile;
    CHAR           Alias[64];
    CHAR           PrefixRange[42];
    UINT           PrefixMinNumberOfDigits;
    UINT           PrefixMaxNumberOfDigits;
    UINT           NumberOfDigitsToRemove;
    UINT           PosOfDigitsToRemove;
    UINT           DialTone;
    CHAR           FacilityAction[64];
    CHAR           FacilityActionArgument[BUF_LEN_256];
} TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO, *PTELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO;


typedef struct _DML_NUMBERINGPLAN_PREFIXINFO_CTRL_
{
    TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO    dml;
    BOOL                                          updated;
} DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T;

typedef struct _DML_NUMBERINGPLAN_PREFIXINFO_LIST_
{
    UINT                             ulQuantity;
    DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T*   pdata[TELCOVOICEMGR_DATA_MAX_NUMBERINGPLAN_PREFIXINFO];
} DML_NUMBERINGPLAN_PREFIXINFO_LIST_T;

typedef struct _TELCOVOICEMGR_DML_NUMBERINGPLAN
{
    UINT                                    MinimumNumberOfDigits;
    UINT                                    MaximumNumberOfDigits;
    UINT                                    InterDigitTimerStd;
    UINT                                    InterDigitTimerOpen;
    UINT                                    InvalidNumberTone;
    UINT                                    PrefixInfoMaxEntries;
    DML_NUMBERINGPLAN_PREFIXINFO_LIST_T     PrefixInfoList;
} TELCOVOICEMGR_DML_NUMBERINGPLAN, *PTELCOVOICEMGR_DML_NUMBERINGPLAN;

typedef struct _TELCOVOICEMGR_DML_TONE_EVENT
{
    UINT          InstanceNumber;
    CHAR          Alias[64];
    CHAR          Function[64];
    UINT          ToneID;
} TELCOVOICEMGR_DML_TONE_EVENT, *PTELCOVOICEMGR_DML_TONE_EVENT;

typedef struct _TELCOVOICEMGR_DML_TONE_DESCRIPTION
{
    UINT          InstanceNumber;
    CHAR          Alias[64];
    UINT          EntryID;
    BOOL          ToneEnable;
    CHAR          ToneName[64];
    UINT          TonePattern;
    CHAR          ToneFile[BUF_LEN_256];
    UINT          ToneRepetitions;
    CHAR          ToneText[64];
} TELCOVOICEMGR_DML_TONE_DESCRIPTION, *PTELCOVOICEMGR_DML_TONE_DESCRIPTION;

typedef struct _TELCOVOICEMGR_DML_TONE_PATTERN
{
    UINT          InstanceNumber;
    CHAR          Alias[64];
    UINT          EntryID;
    BOOL          ToneOn;
    UINT          Frequency1;
    INT           Power1;
    UINT          Frequency2;
    INT           Power2;
    UINT          Frequency3;
    INT           Power3;
    UINT          Frequency4;
    INT           Power4;
    UINT          ModulationFrequency;
    INT           ModulationPower;
    UINT          Duration;
    UINT          NextEntryID;
} TELCOVOICEMGR_DML_TONE_PATTERN, *PTELCOVOICEMGR_DML_TONE_PATTERN;


typedef struct _DML_TONE_PATTERN_CTRL_
{
    TELCOVOICEMGR_DML_TONE_PATTERN    dml;
    BOOL                              updated;
} DML_TONE_PATTERN_CTRL_T;

typedef struct _DML_TONE_PATTERN_LIST_
{
    UINT                             ulQuantity;
    DML_TONE_PATTERN_CTRL_T*         pdata[TELCOVOICEMGR_DATA_MAX_DML_TONE_PATTERN];
} DML_TONE_PATTERN_LIST_T;


typedef struct _DML_TONE_DESCRIPTION_CTRL_
{
    TELCOVOICEMGR_DML_TONE_DESCRIPTION    dml;
    BOOL                                  updated;
} DML_TONE_DESCRIPTION_CTRL_T;

typedef struct _DML_TONE_DESCRIPTION_LIST_
{
    UINT                             ulQuantity;
    DML_TONE_DESCRIPTION_CTRL_T*           pdata[TELCOVOICEMGR_DATA_MAX_TONE_DESCRIPTION];
} DML_TONE_DESCRIPTION_LIST_T;


typedef struct _DML_TONE_EVENT_CTRL_
{
    TELCOVOICEMGR_DML_TONE_EVENT    dml;
    BOOL                            updated;
} DML_TONE_EVENT_CTRL_T;

typedef struct _DML_TONE_EVENT_LIST_
{
    UINT                             ulQuantity;
    DML_TONE_EVENT_CTRL_T*           pdata[TELCOVOICEMGR_DATA_MAX_TONE_EVENT];
} DML_TONE_EVENT_LIST_T;


typedef struct _TELCOVOICEMGR_DML_TONE
{
    DML_TONE_EVENT_LIST_T        ToneEventList;
    DML_TONE_DESCRIPTION_LIST_T  ToneDescriptionList;
    DML_TONE_PATTERN_LIST_T      TonePatternList;
} TELCOVOICEMGR_DML_TONE, *PTELCOVOICEMGR_DML_TONE;

typedef struct _TELCOVOICEMGR_DML_LINE_SIP_EVENTSUBSCRIBE
{
    ULONG        InstanceNumber;
    CHAR         Alias[64];
    CHAR         Event[32];
    CHAR         AuthUserName[128];
    CHAR         AuthPassword[128];
} TELCOVOICEMGR_DML_LINE_SIP_EVENTSUBSCRIBE, *PTELCOVOICEMGR_DML_LINE_SIP_EVENTSUBSCRIBE;

typedef struct _DML_LINE_SIP_EVENTSUBSCRIBE_CTRL_
{
    TELCOVOICEMGR_DML_LINE_SIP_EVENTSUBSCRIBE    dml;
    BOOL                                         updated;
} DML_LINE_SIP_EVENTSUBSCRIBE_CTRL_T;

typedef struct _DML_LINE_SIP_EVENTSUBSCRIBE_LIST_
{
    UINT                                     ulQuantity;
    DML_LINE_SIP_EVENTSUBSCRIBE_CTRL_T*      pdata[TELCOVOICEMGR_DATA_MAX_DML_LINE_SIP_EVENTSUBSCRIBE];
} DML_LINE_SIP_EVENTSUBSCRIBE_LIST_T;

typedef struct _TELCOVOICEMGR_DML_LINE_SIP
{
    CHAR           AuthUserName[128];
    CHAR           AuthPassword[128];
    CHAR           URI[389];
    DML_LINE_SIP_EVENTSUBSCRIBE_LIST_T    LineSipEventSubscribeList;
} TELCOVOICEMGR_DML_LINE_SIP, *PTELCOVOICEMGR_DML_LINE_SIP;

typedef struct _TELCOVOICEMGR_DML_LINE_MGCP
{
    CHAR           LineName[32];
} TELCOVOICEMGR_DML_LINE_MGCP, *PTELCOVOICEMGR_DML_LINE_MGCP;

typedef struct _TELCOVOICEMGR_DML_LINE_H323
{
    CHAR           H323ID[32];
} TELCOVOICEMGR_DML_LINE_H323, *PTELCOVOICEMGR_DML_LINE_H323;

typedef struct _TELCOVOICEMGR_DML_LINE_RINGER_EVENT
{
    ULONG         InstanceNumber;
    CHAR          Alias[64];
    CHAR          Function[64];
    UINT          RingID;
} TELCOVOICEMGR_DML_LINE_RINGER_EVENT, *PTELCOVOICEMGR_DML_LINE_RINGER_EVENT;

typedef struct _TELCOVOICEMGR_DML_LINE_RINGER_DESCRIPTION
{
    ULONG         InstanceNumber;
    CHAR          Alias[64];
    UINT          EntryID;
    BOOL          RingEnable;
    CHAR          RingName[64];
    UINT          RingPattern;
    CHAR          RingFile[BUF_LEN_256];
} TELCOVOICEMGR_DML_LINE_RINGER_DESCRIPTION, *PTELCOVOICEMGR_DML_LINE_RINGER_DESCRIPTION;

typedef struct _TELCOVOICEMGR_DML_LINE_RINGER_PATTERN
{
    ULONG         InstanceNumber;
    CHAR          Alias[64];
    UINT          EntryID;
    BOOL          RingerOn;
    UINT          Duration;
    UINT          NextEntryID;
} TELCOVOICEMGR_DML_LINE_RINGER_PATTERN, *PTELCOVOICEMGR_DML_LINE_RINGER_PATTERN;


typedef struct _DML_LINE_RINGER_DESCRIPTION_CTRL_
{
    TELCOVOICEMGR_DML_LINE_RINGER_DESCRIPTION    dml;
    BOOL                                         updated;
} DML_LINE_RINGER_DESCRIPTION_CTRL_T;

typedef struct _DML_LINE_RINGER_DESCRIPTION_LIST_
{
    UINT                                     ulQuantity;
    DML_LINE_RINGER_DESCRIPTION_CTRL_T*      pdata[TELCOVOICEMGR_DATA_MAX_DML_LINE_RINGER_DESCRIPTION];
} DML_LINE_RINGER_DESCRIPTION_LIST_T;


typedef struct _DML_LINE_RINGER_EVENT_CTRL_
{
    TELCOVOICEMGR_DML_LINE_RINGER_EVENT    dml;
    BOOL                                   updated;
} DML_LINE_RINGER_EVENT_CTRL_T;

typedef struct _DML_LINE_RINGER_EVENT_LIST_
{
    UINT                                     ulQuantity;
    DML_LINE_RINGER_EVENT_CTRL_T*            pdata[TELCOVOICEMGR_DATA_MAX_DML_LINE_RINGER_EVENT];
} DML_LINE_RINGER_EVENT_LIST_T;


typedef struct _DML_LINE_RINGER_PATTERN_CTRL_
{
    TELCOVOICEMGR_DML_LINE_RINGER_PATTERN    dml;
    BOOL                                     updated;
} DML_LINE_RINGER_PATTERN_CTRL_T;

typedef struct _DML_LINE_RINGER_PATTERN_LIST_
{
    UINT                                 ulQuantity;
    DML_LINE_RINGER_PATTERN_CTRL_T*      pdata[TELCOVOICEMGR_DATA_MAX_DML_LINE_RINGER_PATTERN];
} DML_LINE_RINGER_PATTERN_LIST_T;

typedef struct _TELCOVOICEMGR_DML_LINE_RINGER
{
    DML_LINE_RINGER_EVENT_LIST_T        LineRingerEventList;
    DML_LINE_RINGER_DESCRIPTION_LIST_T  LineRingerDescriptionList;
    DML_LINE_RINGER_PATTERN_LIST_T      LineRingerPatternList;
} TELCOVOICEMGR_DML_LINE_RINGER, *PTELCOVOICEMGR_DML_LINE_RINGER;

typedef struct _TELCOVOICEMGR_DML_LINE_CALLINGFEATURES
{
    BOOL         CallerIDEnable;
    BOOL         CallerIDNameEnable;
    CHAR         CallerIDName[BUF_LEN_256];
    BOOL         CallWaitingEnable;
    TELCOVOICEMGR_LINE_CWSTATUS_ENUM                    CallWaitingStatus;
    UINT                                                MaxSessions;
    TELCOVOICEMGR_LINE_CONFERENCE_CALLING_STATUS_ENUM   ConferenceCallingStatus;
    UINT         ConferenceCallingSessionCount;
    BOOL         CallForwardUnconditionalEnable;
    CHAR         CallForwardUnconditionalNumber[32];
    BOOL         CallForwardOnBusyEnable;
    CHAR         CallForwardOnBusyNumber[32];
    BOOL         CallForwardOnNoAnswerEnable;
    CHAR         CallForwardOnNoAnswerNumber[32];
    UINT         CallForwardOnNoAnswerRingCount;
    BOOL         CallTransferEnable;
    BOOL         MWIEnable;
    BOOL         MessageWaiting;
    BOOL         AnonymousCallBlockEnable;
    BOOL         AnonymousCalEnable;
    BOOL         DoNotDisturbEnable;
    BOOL         CallReturnEnable;
    BOOL         RepeatDialEnable;
    BOOL         ConferenceCallingEnable;
    BOOL         HoldEnable;
    BOOL         PhoneCallerIDEnable;
} TELCOVOICEMGR_DML_LINE_CALLINGFEATURES, *PTELCOVOICEMGR_DML_LINE_CALLINGFEATURES;

typedef struct _TELCOVOICEMGR_DML_LINE_VOICEPROCESSING
{
    INT          TransmitGain;
    INT          ReceiveGain;
    BOOL         EchoCancellationEnable;
    BOOL         EchoCancellationInUse;
    BOOL         EchoCancellationTail;
} TELCOVOICEMGR_DML_LINE_VOICEPROCESSING, *PTELCOVOICEMGR_DML_LINE_VOICEPROCESSING;

typedef struct _TELCOVOICEMGR_DML_LINE_CODECLIST
{
    ULONG        InstanceNumber;
    CHAR         Alias[64];
    UINT         EntryID;
    CHAR         Codec[64];
    UINT         BitRate;
    CHAR         PacketizationPeriod[64];
    BOOL         SilenceSuppression;
    BOOL         Enable;
    UINT         Priority;
} TELCOVOICEMGR_DML_LINE_CODECLIST, *PTELCOVOICEMGR_DML_LINE_CODECLIST;

typedef struct _DML_LINE_CODECLIST_CTRL_
{
    TELCOVOICEMGR_DML_LINE_CODECLIST         dml;
    BOOL                                     updated;
} DML_LINE_CODECLIST_CTRL_T;

typedef struct _DML_LINE_CODECLIST_LIST_
{
    UINT                                 ulQuantity;
    DML_LINE_CODECLIST_CTRL_T*           pdata[TELCOVOICEMGR_DATA_MAX_DML_LINE_CODECLIST];
} DML_LINE_CODECLIST_LIST_T;

typedef struct _TELCOVOICEMGR_DML_LINE_CODEC
{
    CHAR         TransmitCodec[64];
    CHAR         ReceiveCodec[64];
    UINT         TransmitBitRate;
    UINT         ReceiveBitRate;
    BOOL         TransmitSilenceSuppression;
    BOOL         ReceiveSilenceSuppression;
    UINT         TransmitPacketizationPeriod;
    DML_LINE_CODECLIST_LIST_T  LineCodecList;
} TELCOVOICEMGR_DML_LINE_CODEC, *PTELCOVOICEMGR_DML_LINE_CODEC;

typedef struct _TELCOVOICEMGR_DML_LINE_SESSION
{
    ULONG        InstanceNumber;
    CHAR         SessionStartTime[64];
    UINT         SessionDuration;
    CHAR         FarEndIPAddress[45];
    UINT         FarEndUDPPort;
    UINT         LocalUDPPort;
} TELCOVOICEMGR_DML_LINE_SESSION, *PTELCOVOICEMGR_DML_LINE_SESSION;

typedef struct _TELCOVOICEMGR_DML_LINE_STATS
{
    BOOL         ResetStatistics;
    UINT         PacketsSent;
    UINT         PacketsReceived;
    UINT         BytesSent;
    UINT         BytesReceived;
    UINT         PacketsLost;
    UINT         Overruns;
    UINT         Underruns;
    UINT         IncomingCallsReceived;
    UINT         IncomingCallsAnswered;
    UINT         IncomingCallsConnected;
    UINT         IncomingCallsFailed;
    UINT         OutgoingCallsAttempted;
    UINT         OutgoingCallsAnswered;
    UINT         OutgoingCallsConnected;
    UINT         OutgoingCallsFailed;
    UINT         CallsDropped;
    UINT         TotalCallTime;
    UINT         ServerDownTime;
    UINT         ReceivePacketLossRate;
    UINT         FarEndPacketLossRate;
    UINT         ReceiveInterarrivalJitter;
    UINT         FarEndInterarrivalJitter;
    UINT         RoundTripDelay;
    UINT         AverageReceiveInterarrivalJitter;
    UINT         AverageFarEndInterarrivalJitter;
    UINT         AverageRoundTripDelay;
} TELCOVOICEMGR_DML_LINE_STATS, *PTELCOVOICEMGR_DML_LINE_STATS;

typedef struct _DML_LINE_SESSION_CTRL_
{
    TELCOVOICEMGR_DML_LINE_SESSION    dml;
    BOOL                              updated;
} DML_LINE_SESSION_CTRL_T;

typedef struct _DML_LINE_SESSION_LIST_
{
    UINT                             ulQuantity;
    DML_LINE_SESSION_CTRL_T*         pdata[TELCOVOICEMGR_DATA_MAX_DML_LINE_SESSION];
} DML_LINE_SESSION_LIST_T;

typedef struct _TELCOVOICEMGR_DML_LINE
{
    ULONG                                      InstanceNumber;
    void                                       *pParentVoiceProfile;
    TELCOVOICEMGR_ENABLE_ENUM                  Enable;
    CHAR                                       Alias[64];
    CHAR                                       DirectoryNumber[32];
    TELCOVOICEMGR_LINE_STATUS_ENUM             Status;
    TELCOVOICEMGR_CALL_STATE_ENUM              CallState;
    CHAR                                       X_RDK_OutboundProxyAddresses[BUF_LEN_256];
    CHAR                                       PhyReferenceList[32];
    BOOL                                       RingMuteStatus;
    UINT                                       RingVolumeStatus;
    TELCOVOICEMGR_DML_LINE_SIP                 LineSipObj;
    TELCOVOICEMGR_DML_LINE_MGCP                LineMGCPObj;
    TELCOVOICEMGR_DML_LINE_H323                LineH323Obj;
    TELCOVOICEMGR_DML_LINE_RINGER              LineRingerObj;
    TELCOVOICEMGR_DML_LINE_CALLINGFEATURES     LineCallingFeaturesObj;
    TELCOVOICEMGR_DML_LINE_VOICEPROCESSING     LineVoiceProcessingObj;
    TELCOVOICEMGR_DML_LINE_CODEC               LineCodecObj;
    DML_LINE_SESSION_LIST_T                    LineSessionList;
    TELCOVOICEMGR_DML_LINE_STATS               LineStatsObj;
} TELCOVOICEMGR_DML_LINE, *PTELCOVOICEMGR_DML_LINE;


typedef struct _DML_LINE_CTRL_
{
    TELCOVOICEMGR_DML_LINE    dml;
    BOOL                      updated;
} DML_LINE_CTRL_T;

typedef struct _DML_LINE_LIST_
{
    UINT                             ulQuantity;
    DML_LINE_CTRL_T*                 pdata[TELCOVOICEMGR_DATA_MAX_LINE];
} DML_LINE_LIST_T;


typedef struct _TELCOVOICEMGR_DML_FAXT38
{
    BOOL                           Enable;
    UINT                           BitRate;
    UINT                           HighSpeedPacketRate;
    UINT                           HighSpeedRedundancy;
    UINT                           LowSpeedRedundancy;
    TELCOVOICEMGR_TCFMETHOD_ENUM   TCFMethod;
} TELCOVOICEMGR_DML_FAXT38,  *PTELCOVOICEMGR_DML_FAXT38;


typedef struct _TELCOVOICEMGR_DML_BUTTON
{
    ULONG                  InstanceNumber;
    CHAR                   Alias[64];
    CHAR                   ButtonName[16];
    CHAR                   FacilityAction[64];
    CHAR                   FacilityActionArgument[BUF_LEN_256];
    CHAR                   QuickDialNumber[BUF_LEN_256];
    CHAR                   ButtonMessage[64];
    BOOL                   UserAccess;
} TELCOVOICEMGR_DML_BUTTON,  *PTELCOVOICEMGR_DML_BUTTON;

typedef struct _DML_BUTTON_CTRL_
{
    TELCOVOICEMGR_DML_BUTTON    dml;
    BOOL                        updated;
} DML_BUTTON_CTRL_T;

typedef struct _DML_BUTTON_LIST_
{
    UINT                             ulQuantity;
    DML_BUTTON_CTRL_T*               pdata[TELCOVOICEMGR_DATA_MAX_BUTTON];
} DML_BUTTON_LIST_T;

typedef struct _TELCOVOICEMGR_DML_BUTTONMAP
{
    ULONG                                       NumberOfButtons;
    DML_BUTTON_LIST_T                           ButtonList;
} TELCOVOICEMGR_DML_BUTTONMAP,  *PTELCOVOICEMGR_DML_BUTTONMAP;

typedef struct _TELCOVOICEMGR_DML_VOICEPROFILE
{
    ULONG                                        InstanceNumber;
    void*                                        *pParentVoiceService;
    TELCOVOICEMGR_ENABLE_ENUM                    Enable[64];
    CHAR                                         Alias[64];
    BOOL                                         Reset;
    ULONG                                        NumberOfLines;
    CHAR                                         Name[64];
    CHAR                                         SignalingProtocol[128];
    UINT                                         MaxSessions;
    TELCOVOICEMGR_PROFILE_DTMF_METHOD_ENUM       DTMFMethod[64];
    TELCOVOICEMGR_PROFILE_DTMF_METHODG711_ENUM   DTMFMethodG711[64];
    CHAR                                         Region[128];
    CHAR                                         DigitMap[BUF_LEN_256];
    BOOL                                         DigitMapEnable;
    CHAR                                         X_RDK_DigitMap[BUF_LEN_256];
    CHAR                                         EmergencyDigitMap[BUF_LEN_256];
    UINT                                         SDigitTimer;
    UINT                                         ZDigitTimer;
    BOOL                                         STUNEnable;
    CHAR                                         STUNServer[BUF_LEN_256];
    UINT                                         NonVoiceBandwidthReservedUpstream;
    UINT                                         NonVoiceBandwidthReservedDownstream;
    BOOL                                         PSTNFailOver;
    TELCOVOICEMGR_PROFILE_FAXPASSTHROUGH_ENUM    FaxPassThrough;
    TELCOVOICEMGR_PROFILE_MODEMPASSTHROUGH_ENUM  ModemPassThrough;
    TELCOVOICEMGR_DML_SERVICE_PROVIDER_INFO      ServiceProviderInfoObj;
    TELCOVOICEMGR_DML_SIP                        SIPObj;
    TELCOVOICEMGR_DML_MGCP                       MGCPObj;
    TELCOVOICEMGR_DML_H323                       H323Obj;
    TELCOVOICEMGR_DML_RTP                        RTPObj;
    TELCOVOICEMGR_DML_NUMBERINGPLAN              NumberingPlanObj;
    TELCOVOICEMGR_DML_TONE                       ToneObj;
    TELCOVOICEMGR_DML_BUTTONMAP                  ButtonMapObj;
    TELCOVOICEMGR_DML_FAXT38                     Fax38Obj;
    DML_LINE_LIST_T                              LineList;
} TELCOVOICEMGR_DML_VOICEPROFILE, *PTELCOVOICEMGR_DML_PROFILE;


typedef struct _DML_PROFILE_CTRL_
{
    TELCOVOICEMGR_DML_VOICEPROFILE     dml;
    BOOL        updated;
} DML_PROFILE_CTRL_T;

typedef struct _DML_PROFILE_LIST__
{
    UINT                          ulQuantity;
    DML_PROFILE_CTRL_T*     pdata[TELCOVOICEMGR_DATA_MAX_PROFILE];
} DML_PROFILE_LIST_T;


typedef struct _TELCOVOICEMGR_DML_PHYINTERFACE_TESTS
{
    TELCOVOICEMGR_PHYINTERFACE_TESTSTATE_ENUM       TestState;
    TELCOVOICEMGR_PHYINTERFACE_TESTSELECTOR_ENUM    TestSelector;
    CHAR                                            X_RDK_TestResult[64];
    BOOL                                            PhoneConnectivity;
} TELCOVOICEMGR_DML_PHYINTERFACE_TESTS,  *PTELCOVOICEMGR_DML_PHYINTERFACE_TESTS;

typedef struct _TELCOVOICEMGR_DML_PHYINTERFACE
{
    ULONG                                    InstanceNumber;
    void*                                    *pParentVoiceService;
    CHAR                                     Alias[64];
    CHAR                                     PhyPort[2];
    UINT                                     InterfaceID;
    CHAR                                     Description[32];
    TELCOVOICEMGR_DML_PHYINTERFACE_TESTS     PhyInterfaceTestsObj;
} TELCOVOICEMGR_DML_PHYINTERFACE, *PTELCOVOICEMGR_DML_PHYINTERFACE;


typedef struct _DML_PHYINTERFACE_CTRL_
{
    TELCOVOICEMGR_DML_PHYINTERFACE    dml;
    BOOL                              updated;
} DML_PHYINTERFACE_CTRL_T;

typedef struct _DML_PHYINTERFACE_LIST_
{
    UINT                              ulQuantity;
    DML_PHYINTERFACE_CTRL_T*    pdata[TELCOVOICEMGR_DATA_MAX_PHYINTERFACE];
} DML_PHYINTERFACE_LIST_T;


typedef struct _TELCOVOICEMGR_DML_X_RDK_DEBUG
{
    CHAR                                     CCTKTraceGroup[BUF_LEN_256];
    CHAR                                     CCTKTraceLevel[BUF_LEN_256];
    CHAR                                     ModuleLogLevels[BUF_LEN_256];
    CHAR                                     LogServer[BUF_LEN_256];
    UINT                                     LogServerPort;
} TELCOVOICEMGR_DML_X_RDK_DEBUG, *PTELCOVOICEMGR_DML_X_RDK_DEBUG;

typedef struct _TELCOVOICEMGR_DML_CAPABILITIES_SIP
{
    TELCOVOICEMGR_CAPABILITIES_SIP_ROLE_ENUM   Role;
    CHAR                                       Extensions[BUF_LEN_256];
    CHAR                                       Transports[BUF_LEN_256];
    CHAR                                       URISchemes[BUF_LEN_256];
    BOOL                                       EventSubscription;
    BOOL                                       ResponseMap;
    CHAR                                       TLSAuthenticationProtocols[BUF_LEN_256];
    CHAR                                       TLSAuthenticationKeySizes[BUF_LEN_256];
    CHAR                                       TLSEncryptionProtocols[BUF_LEN_256];
    CHAR                                       TLSEncryptionKeySizes[BUF_LEN_256];
    CHAR                                       TLSKeyExchangeProtocols[BUF_LEN_256];
} TELCOVOICEMGR_DML_CAPABILITIES_SIP, *PTELCOVOICEMGR_DML_CAPABILITIES_SIP;

typedef struct _TELCOVOICEMGR_DML_CAPABILITIES_MGCP
{
    CHAR                                       Extensions[BUF_LEN_256];
} TELCOVOICEMGR_DML_CAPABILITIES_MGCP, *PTELCOVOICEMGR_DML_CAPABILITIES_MGCP;

typedef struct _TELCOVOICEMGR_DML_CAPABILITIES_H323
{
    BOOL                                       FastStart;
    CHAR                                       H235AuthenticationMethods[BUF_LEN_256];
} TELCOVOICEMGR_DML_CAPABILITIES_H323, *PTELCOVOICEMGR_DML_CAPABILITIES_H323;

typedef struct _TELCOVOICEMGR_DML_CAPABILITIES_CODECS
{
    ULONG                                      InstanceNumber;
    CHAR                                       Alias[64];
    ULONG                                      EntryID;
    CHAR                                       Codec[64];
    ULONG                                      BitRate;
    CHAR                                       PacketizationPeriod[64];
    BOOL                                       SilenceSuppression;
} TELCOVOICEMGR_DML_CAPABILITIES_CODECS, *PTELCOVOICEMGR_DML_CAPABILITIES_CODECS;

typedef struct _DML_CAPABILITIES_CODECS_CTRL_
{
    TELCOVOICEMGR_DML_CAPABILITIES_CODECS      dml;
    BOOL                                       updated;
} DML_CAPABILITIES_CODECS_CTRL_T;

typedef struct _DML_CAPABILITIES_CODECS_LIST_
{
    UINT                                  ulQuantity;
    DML_CAPABILITIES_CODECS_CTRL_T*       pdata[TELCOVOICEMGR_DATA_MAX_VOICE_CAPABILITIES_CODECS];
} DML_CAPABILITIES_CODECS_LIST_T;

typedef struct _TELCOVOICEMGR_DML_CAPABILITIES
{
    ULONG                                      MaxProfileCount;
    ULONG                                      MaxLineCount;
    ULONG                                      MaxSessionsPerLine;
    ULONG                                      MaxSessionCount;
    CHAR                                       SignalingProtocols[BUF_LEN_256];
    CHAR                                       Regions[BUF_LEN_256];
    BOOL                                       RTCP;
    BOOL                                       SRTP;
    CHAR                                       SRTPKeyingMethods[BUF_LEN_256];
    CHAR                                       SRTPEncryptionKeySizes[BUF_LEN_256];
    BOOL                                       RTPRedundancy;
    BOOL                                       DSCPCoupled;
    BOOL                                       EthernetTaggingCoupled;
    BOOL                                       PSTNSoftSwitchOver;
    BOOL                                       FaxT38;
    BOOL                                       FaxPassThrough;
    BOOL                                       ModemPassThrough;
    BOOL                                       ToneGeneration;
    BOOL                                       ToneDescriptionsEditable;
    BOOL                                       PatternBasedToneGeneration;
    BOOL                                       FileBasedToneGeneration;
    CHAR                                       ToneFileFormats[BUF_LEN_256];
    BOOL                                       RingGeneration;
    BOOL                                       RingDescriptionsEditable;
    BOOL                                       PatternBasedRingGeneration;
    BOOL                                       RingPatternEditable;
    BOOL                                       FileBasedRingGeneration;
    CHAR                                       RingFileFormats[BUF_LEN_256];
    BOOL                                       DigitMap;
    BOOL                                       NumberingPlan;
    BOOL                                       ButtonMap;
    BOOL                                       VoicePortTests;
    TELCOVOICEMGR_DML_CAPABILITIES_SIP         CapabilitiesSIPObj;
    TELCOVOICEMGR_DML_CAPABILITIES_MGCP        CapabilitiesMGCPObj;
    TELCOVOICEMGR_DML_CAPABILITIES_H323        CapabilitiesH323Obj;
    DML_CAPABILITIES_CODECS_LIST_T             CapabilitiesCodecList;
} TELCOVOICEMGR_DML_CAPABILITIES, *PTELCOVOICEMGR_DML_CAPABILITIES;

typedef struct _TELCOVOICEMGR_DML_VOICESERVICE
{
    ULONG                                      InstanceNumber;
    CHAR                                       Alias[64];
    BOOL                                       X_RDK_DisableLoopCurrentUntilRegistered;
    CHAR                                       X_RDK_BoundIfName[BUF_LEN_256];
    CHAR                                       X_RDK_IpAddressFamily[BUF_LEN_256];
    CHAR                                       X_RDK_BoundIpAddr[BUF_LEN_256];
    TELCOVOICEMGR_VOICE_ENABLE_ENUM            X_RDK_Enable;
    TELCOVOICEMGR_VOICE_STATUS_ENUM            X_RDK_Status;
    BOOL                                       X_RDK_FactoryReset;
    TELCOVOICEMGR_DML_CAPABILITIES             CapabilitiesObj;
    TELCOVOICEMGR_DML_X_RDK_DEBUG              X_RDK_DebugObj;
    DML_PROFILE_LIST_T                         VoiceProfileList;
    DML_PHYINTERFACE_LIST_T                    PhyInterfaceList;
} TELCOVOICEMGR_DML_VOICESERVICE, *PTELCOVOICEMGR_DML_VOICESERVICE;

typedef struct _DML_VOICE_SERVICE_CTRL_
{
    TELCOVOICEMGR_DML_VOICESERVICE      dml;
    BOOL                                updated;
} DML_VOICE_SERVICE_CTRL_T;

typedef struct _DML_VOICE_SERVICE_LIST_
{
    UINT                            ulQuantity;
    DML_VOICE_SERVICE_CTRL_T*       pdata[TELCOVOICEMGR_DATA_MAX_VOICE_SERVICES];
} DML_VOICE_SERVICE_LIST_T;


typedef struct _TELCOVOICEMGR_DML_SERVICE
{
    DML_VOICE_SERVICE_LIST_T    VoiceService;
} TELCOVOICEMGR_DML_SERVICE;

typedef struct _TELCOVOICEMGR_DML_DATA
{    
    TELCOVOICEMGR_DML_SERVICE  Service;

    //mutex
    pthread_mutex_t     mDataMutex;
} TELCOVOICEMGR_DML_DATA;


TELCOVOICEMGR_DML_DATA* TelcoVoiceMgrDmlGetDataLocked(void);
void TelcoVoiceMgrDmlGetDataRelease(TELCOVOICEMGR_DML_DATA* pDmlData);
TELCOVOICEMGR_DML_DATA* TelcoVoiceMgrDmlGetData(void);
ANSC_STATUS TelcoVoiceMgrDmlDataInit(void);
ANSC_STATUS TelcoVoiceMgrDmlDataDelete(void);
ANSC_STATUS TelcoVoiceMgrDmlSetDefaultData(TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrData);
int TelcoVoiceMgrSyseventInit();
void TelcoVoiceMgrSyseventDeInit();
int firewall_restart_for_voice(unsigned long timeout_ms);

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceService(DML_VOICE_SERVICE_LIST_T* pVoiceProfileList, int vs_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfile(DML_PROFILE_LIST_T* pVoiceProfileList, int vp_index);
ANSC_STATUS TelcoVoiceMgrDmlAddPhyInterface(DML_PHYINTERFACE_LIST_T* pPhyInterfaceList, int pi_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileSIPEventSubscribe(DML_SIP_EVENTSUBSCRIBE_LIST_T* psipEvtSubscribeList, int sipEvtSubscribe_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileSIPRespMap(DML_SIP_RESPONSEMAP_LIST_T* pSIPResponseMapList, int SipRespMap_index);
ANSC_STATUS TelcoVoiceMgrDmlAddNumberingplanPrefixInfo(DML_NUMBERINGPLAN_PREFIXINFO_LIST_T* pVoiceProfile_Numberingplan_PrefixInfo_List, int PrefixInfo_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileToneEvent(DML_TONE_EVENT_LIST_T* pVoiceProfile_Tone_Event_List, int ToneEvent_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileToneDesc(DML_TONE_DESCRIPTION_LIST_T* pVoiceProfile_Tone_Desc_List, int ToneDesc_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileTonePattern(DML_TONE_PATTERN_LIST_T* pVoiceProfile_Tone_Pattern_List, int TonePattern_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileButtonMapButton(DML_BUTTON_LIST_T* pVoiceProfile_ButtonMap_Button_List, int Button_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLine(DML_LINE_LIST_T* pVoiceProfile_Line_List, int Line_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineSIPEvtSubscribe(DML_LINE_SIP_EVENTSUBSCRIBE_LIST_T* pVoiceProfile_Line_SIP_EvtSubscribe_List, int Evtsubscribe_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineRingerEvent(DML_LINE_RINGER_EVENT_LIST_T* pVoiceProfile_Line_Ringer_Event_List, int Event_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineRingerDesc(DML_LINE_RINGER_DESCRIPTION_LIST_T* pVoiceProfile_Line_Ringer_Desc_List, int RingerDesc_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineRingerPattern(DML_LINE_RINGER_PATTERN_LIST_T* pVoiceProfile_Line_Ringer_Pattern_List, int RingerPattern_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineCodecList(DML_LINE_CODECLIST_LIST_T* pVoiceProfile_Line_CodecList_List, int CodecList_index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineSession(DML_LINE_SESSION_LIST_T* pVoiceProfile_Line_Session_List, int Session_index);
ANSC_STATUS TelcoVoiceMgrDmlAddCapabilitiesCodec(DML_CAPABILITIES_CODECS_LIST_T* pCapabilitiesCodecList, int codec_index);

ANSC_STATUS TelcoVoiceMgrDmlSetLoopCurrentDisabled(UINT uiService, BOOL Bool);
ANSC_STATUS TelcoVoiceMgrDmlFactoryReset(uint32_t uiService, TELCOVOICEMGR_VOICE_ENABLE_ENUM VoiceState);
ANSC_STATUS TelcoVoiceMgrDmlSetBoundIfname(UINT uiService, char* BoundIfname);
ANSC_STATUS TelcoVoiceMgrDmlSetIpAddressFamily(UINT uiService, char *IpAddressFamily);
ANSC_STATUS TelcoVoiceMgrDmlSetCCTKTraceGroup(UINT uiService, char *CCTKTraceGroup);
ANSC_STATUS TelcoVoiceMgrDmlSetCCTKTraceLevel(UINT uiService, char *CCTKTraceLevel);
ANSC_STATUS TelcoVoiceMgrDmlSetModuleLogLevel(UINT uiService, char *logLevels);
ANSC_STATUS TelcoVoiceMgrDmlSetVoiceProcessState(UINT uiService, TELCOVOICEMGR_VOICE_ENABLE_ENUM uState);
ANSC_STATUS TelcoVoiceMgrDmlSetLinkState(TELCOVOICEMGR_VOICE_IP_LINK_STATE linkState);

ANSC_STATUS TelcoVoiceMgrDmlSetDirectoryNumber(UINT uiService, UINT uiProfile, UINT uiLine, char* pDirName);
ANSC_STATUS TelcoVoiceMgrDmlSetLineEnable(UINT uiService, UINT uiProfile, UINT uiLine, ULONG LineEnable);
ANSC_STATUS TelcoVoiceMgrDmlGetLineStatus(UINT uiService, UINT uiProfile, UINT uiLine, UINT *pLineStatus);
ANSC_STATUS TelcoVoiceMgrDmlGetLineCallState(UINT uiService, UINT uiProfile, UINT uiLine, ULONG* pCallState);

ANSC_STATUS TelcoVoiceMgrDmlSetOutboundProxy(UINT uiService, UINT uiProfile, char* pProxyName);
ANSC_STATUS TelcoVoiceMgrDmlSetProxyServer(UINT uiService, UINT uiProfile, char* pProxyServer);
ANSC_STATUS TelcoVoiceMgrDmlSetRegistrarServer(UINT uiService, UINT uiProfile, char* pRegServer);
ANSC_STATUS TelcoVoiceMgrDmlSetUserAgentDomain(UINT uiService, UINT uiProfile, char* pUsrAgent);
ANSC_STATUS TelcoVoiceMgrDmlSetConferencingURI(UINT uiService, UINT uiProfile, char* pConfURI);
ANSC_STATUS TelcoVoiceMgrDmlSetSDigitTimer(UINT uiService, UINT uiProfile, ULONG uDgtTimer);
ANSC_STATUS TelcoVoiceMgrDmlSetZDigitTimer(UINT uiService, UINT uiProfile, ULONG uDgtTimer);

ANSC_STATUS TelcoVoiceMgrDmlSetLogServer(UINT uiService, char* pLogServer);
ANSC_STATUS TelcoVoiceMgrDmlSetDigitMap(uint32_t uiService, uint32_t uiProfile, char* pDigitMap, char* typeOfDigitMap);
ANSC_STATUS TelcoVoiceMgrDmlGetLineStats(UINT uiService, UINT uiProfile, UINT uiLine, TELCOVOICEMGR_DML_VOICESERVICE_STATS *pStats);
ANSC_STATUS TelcoVoiceMgrDmlResetLineStats(UINT uiService, UINT uiProfile, UINT uiLine);
ANSC_STATUS TelcoVoiceMgrDmlSetReceiveGain(UINT uiService, UINT uiProfile,UINT uiLine, int32_t iGain);
ANSC_STATUS TelcoVoiceMgrDmlSetTransmitGain(UINT uiService, UINT uiProfile,UINT uiLine, int32_t iGain);
ANSC_STATUS TelcoVoiceMgrDmlSetLineCallingFeatures(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM eFeature, BOOL bStatus);
ANSC_STATUS TelcoVoiceMgrDmlSetLineSIPAuthCredentials(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, TELCOVOICEMGR_VOICE_CREDENTIAL_TYPE_ENUM eAuthCredential, char* pBuffer);
ANSC_STATUS TelcoVoiceMgrDmlSetLineSipURI(UINT uiService, UINT uiProfile, UINT uiLine, char*pSipURI);
ANSC_STATUS TelcoVoiceMgrDmlSetPrackRequired(UINT uiService, UINT uiProfile, BOOL bValue);
ANSC_STATUS TelcoVoiceMgrInitMark(uint32_t uiService, uint32_t uiProfile, int iValue, PROTOCOL_TYPE protocol, char* paramName);
ANSC_STATUS TelcoVoiceMgrDmlSetBoundIpAddress(uint32_t uiService, char *BoundIpAddress);
ANSC_STATUS TelcoVoiceMgrDmlGetEthernetPriorityMark(uint32_t uiService, uint32_t uiProfile, PROTOCOL_TYPE protocol, int* pValue);
ANSC_STATUS TelcoVoiceMgrDmlSetX_RDK_FirewallRuleData(PTELCOVOICEMGR_DML_SIP pvoiceProfileSip, PTELCOVOICEMGR_DML_RTP pvoiceProfileRtp, PROTOCOL_TYPE protocol );

#endif /* _TELCOVOICEMGR_SERVICES_APIS_H */
