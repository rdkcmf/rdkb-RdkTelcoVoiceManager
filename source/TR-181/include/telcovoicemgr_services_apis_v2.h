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


#ifndef  _TELCOVOICEMGR_SERVICES_APIS_V2_H
#define  _TELCOVOICEMGR_SERVICES_APIS_V2_H

/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include "ansc_platform.h"
#include "ansc_string_util.h"

#define TELCOVOICE_DATA_MAX                                1
#define TELCOVOICEMGR_DATA_MAX_VOICE_SERVICES              1

#define  TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES        1
#define  TELCOVOICEMGR_DML_NUMBER_OF_VOIP_PROFILE          1
#define  TELCOVOICEMGR_DML_NUMBER_OF_SIP_NETWORK           1
#define  TELCOVOICEMGR_DML_NUMBER_OF_SIP_CLIENTS           1
#define  TELCOVOICEMGR_DML_NUMBER_OF_CALLCONTROL_LINES     1
#define  SYSEVENT_UPDATE_IFNAME             "telcovoicemgr_bound_ifname"
#define  SYSEVENT_UPDATE_IPFAMILY           "telcovoicemgr_ipaddr_family"
#define  MAX_INTERFACES                     10
#define  MAX_MARK_ENTRY                     10
#define  STR_TRUE                           "true"
#define  STR_FALSE                          "false"
#define  WAN_DBUS_PATH                      "/com/cisco/spvtg/ccsp/wanmanager"
#define  WAN_COMPONENT_NAME                 "eRT.com.cisco.spvtg.ccsp.wanmanager"
#define  TABLE_NAME_WAN_INTERFACE           "Device.X_RDK_WanManager.CPEInterface."
#define  TABLE_NAME_WAN_MARKING             "Marking."
#define  PARAM_NAME_WAN_ENABLE              "Wan.Enable"
#define  PARAM_NAME_WAN_REFRESH             "Wan.Refresh"
#define  PARAM_NAME_MARK_ALIAS              "Alias"
#define  PARAM_NAME_ETHERNET_PRIORITY_MARK  "EthernetPriorityMark"
#define  PARAM_NAME_SKB_MARK                "SKBMark"
#define  PARAM_NAME_DSCP_MARK               "DSCPMark"
#define  PARAM_NAME_IPV4_STATE              "IP.IPv4Status"
#define  PARAM_NAME_IPV6_STATE              "IP.IPv6Status"
#define  PARAM_VALUE_SIP_ALIAS              "voicesip1"
#define  PARAM_VALUE_RTP_ALIAS              "voicertp1"
#define  PARAM_VALUE_IP_STATE_UP            "Up"
#define  STR_IPV4                            "IPv4"
#define  STR_IPV6                            "IPv6"

#define  IP_ADDR_FAMILY_LENGTH               32
#define  BOUND_IF_NAME_LENGTH                32
#define  JSON_MAX_VAL_ARR_SIZE               256
#define  JSON_MAX_STR_ARR_SIZE               256
#define  IP_ADDR_LENGTH                      64
#define  BUF_LEN_16                          16
#define  BUF_LEN_512                         512
#define  BUF_LEN_256                         256

#define  TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES      1
#define  TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE       1
#define  TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE       1
#define  TELCOVOICEMGR_DML_NUMBER_OF_LINE                1

#define  STR_LEN_2                           2
#define  STR_LEN_4                           4
#define  STR_LEN_5                           5
#define  STR_LEN_13                          13
#define  STR_LEN_16                          16
#define  STR_LEN_20                          20
#define  STR_LEN_24                          24
#define  STR_LEN_32                          32
#define  STR_LEN_40                          40
#define  STR_LEN_42                          42
#define  STR_LEN_45                          45
#define  STR_LEN_64                          64
#define  STR_LEN_128                         128
#define  STR_LEN_256                         256
#define  STR_LEN_389                         389

 /***********************************
     Actual definition declaration
 ************************************/
 /*
     TelcoVoice Part
 */
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

typedef enum _TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM
{
    VOICE_CALLING_FEATURE_CALL_WAITING = 0,
    VOICE_CALLING_FEATURE_MSG_WAIT_INDICATOR,
    VOICE_CALLING_FEATURE_CONF_CALL,
    VOICE_CALLING_FEATURE_HOLD,
    VOICE_CALLING_FEATURE_CALLER_ID,
} TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM;

typedef enum _TELCOVOICEMGR_LINE_STATUS_ENUM
{
    VOICE_LINE_STATE_UP = 0,
    VOICE_LINE_STATE_ERROR,
    VOICE_LINE_STATE_TESTING,
    VOICE_LINE_STATE_QUIESCENT,
    VOICE_LINE_STATE_DISABLED
} TELCOVOICEMGR_LINE_STATUS_ENUM;

typedef enum _TELCOVOICEMGR_CALL_STATE_ENUM
{
    VOICE_CALL_STATE_IDLE = 0,
    VOICE_CALL_STATE_DIALING,
    VOICE_CALL_STATE_DELIVERED,
    VOICE_CALL_STATE_CONNECTED,
    VOICE_CALL_STATE_ALERTING,
    VOICE_CALL_STATE_DISCONNECTED
} TELCOVOICEMGR_CALL_STATE_ENUM;

typedef enum _TELCOVOICEMGR_VOICE_CREDENTIAL_TYPE_ENUM
{
    VOICE_HAL_AUTH_UNAME = 0,
    VOICE_HAL_AUTH_PWD
} TELCOVOICEMGR_VOICE_CREDENTIAL_TYPE_ENUM;

typedef enum _TELCOVOICEMGR_VOICE_IP_ADD_FAMILY
{
    VOICE_HAL_AF_INET_V4 = 0,
    VOICE_HAL_AF_INET_V6
}  TELCOVOICEMGR_VOICE_IP_ADD_FAMILY;

typedef enum _TELCOVOICEMGR_VOICE_IP_LINK_STATE
{
    VOICE_HAL_IP_LINK_STATE_DOWN = 0,
    VOICE_HAL_IP_LINK_STATE_UP
}  TELCOVOICEMGR_VOICE_IP_LINK_STATE;

typedef enum _TONEFILEFORMATS_ENUM
{
    TONE_FILEFORMAT_G_711MuLaw,
    TONE_FILEFORMAT_G_711ALaw,
    TONE_FILEFORMAT_G_729,
    TONE_FILEFORMAT_MP3,
    TONE_FILEFORMAT_WAV,
    TONE_FILEFORMAT_AMR
} TONEFILEFORMATS_ENUM;

typedef enum _PROTOCOL_TYPE
{
  SIP,
  RTP
}PROTOCOL_TYPE;

typedef struct _ethPriorityValStruct_t
{
  PROTOCOL_TYPE protocol;
  int32_t iEthPriorityMark;
  BOOL iUpdateStatus;
}ethPriorityValStruct_t;

typedef struct _retSkbMarksStruct_t
{
  BOOL bSipMarkRead;
  BOOL bRtpMarkRead;
  UINT sipSkbMark;
  UINT rtpSkbMark;
  char aIpStateParamName[STR_LEN_16];
}retSkbMarksStruct_t;

typedef enum _RINGFILEFORMATS_ENUM
{
    RING_FILEFORMAT_MIDI,
    RING_FILEFORMAT_SMAF_MMF,
    RING_FILEFORMAT_RTTTL,
    RING_FILEFORMAT_MP3,
    RING_FILEFORMAT_WAV,
    RING_FILEFORMAT_AMR
} RINGFILEFORMATS_ENUM;

typedef enum _CAPABILITIES_DIALTYPE_ENUM
{
    CAP_DIALTYPE_TONE,
    CAP_DIALTYPE_PULSE,
    CAP_DIALTYPE_TONE_PLUSE
} CAPABILITIES_DIALTYPE_ENUM;

typedef enum _POTS_DIALTYPE_ENUM
{
    POTS_DIALTYPE_TONE,
    POTS_DIALTYPE_PULSE
} POTS_DIALTYPE_ENUM;

typedef enum _CAPABILITIESCODEC_ENUM
{
   CAP_CODEC_AMR,
   CAP_CODEC_CLEARMODE,
   CAP_CODEC_EVRC1,
   CAP_CODEC_EVRCB1,
   CAP_CODEC_G_711MULAW,
   CAP_CODEC_G_711ALAW,
   CAP_CODEC_G_726,
   CAP_CODEC_G_729,
   CAP_CODEC_G_729_1,
   CAP_CODEC_G_729A,
   CAP_CODEC_G_729E,
   CAP_CODEC_G_728,
   CAP_CODEC_G_723_1,
   CAP_CODEC_G_722,
   CAP_CODEC_G_722_1,
   CAP_CODEC_G_722_2,
   CAP_CODEC_GENERICCOMFORTNOISE,
   CAP_CODEC_GSM_FR,
   CAP_CODEC_GSM_HR,
   CAP_CODEC_GSM_EFR,
   CAP_CODEC_ILBC,
   CAP_CODEC_SPEEX
} CAPABILITIESCODEC_ENUM;

typedef enum _FACILITYACTIONS_ENUM
{
    FACILITY_ACTION_AA_REGISTER,
    FACILITY_ACTION_AA_ERASE,
    FACILITY_ACTION_AA_INTERROGATE,
    FACILITY_ACTION_CA_ACTIVATE,
    FACILITY_ACTION_CCBS_ACTIVATE,
    FACILITY_ACTION_CCBS_DEACTIVATE,
    FACILITY_ACTION_CCBS_INTERROGATE,
    FACILITY_ACTION_CCNR_ACTIVATE,
    FACILITY_ACTION_CCNR_DEACTIVATE,
    FACILITY_ACTION_CCNR_INTERROGATE,
    FACILITY_ACTION_CFB_REGISTER,
    FACILITY_ACTION_CFB_ACTIVATE,
    FACILITY_ACTION_CFB_DEACTIVATE,
    FACILITY_ACTION_CFB_ERASE,
    FACILITY_ACTION_CFB_INTERROGATE,
    FACILITY_ACTION_CFNR_REGISTER,
    FACILITY_ACTION_CFNR_ACTIVATE,
    FACILITY_ACTION_CFNR_DEACTIVATE,
    FACILITY_ACTION_CFNR_ERASE,
    FACILITY_ACTION_CFNR_INTERROGATE,
    FACILITY_ACTION_CFNR_TIMER,
    FACILITY_ACTION_CFT_ACTIVATE,
    FACILITY_ACTION_CFT_DEACTIVATE,
    FACILITY_ACTION_CFT_INTERROGATE,
    FACILITY_ACTION_CFU_REGISTER,
    FACILITY_ACTION_CFU_ACTIVATE,
    FACILITY_ACTION_CFU_DEACTIVATE,
    FACILITY_ACTION_CFU_ERASE,
    FACILITY_ACTION_CFU_INTERROGATE,
    FACILITY_ACTION_CLIR_ACTIVATE,
    FACILITY_ACTION_CLIR_DEACTIVATE,
    FACILITY_ACTION_CLIR_INTERROGATE,
    FACILITY_ACTION_CP_INVOKE,
    FACILITY_ACTION_CW_ACTIVATE,
    FACILITY_ACTION_CW_DEACTIVATE,
    FACILITY_ACTION_CW_INVOKE,
    FACILITY_ACTION_DND_ACTIVATE,
    FACILITY_ACTION_DND_DEACTIVATE,
    FACILITY_ACTION_DND_INTERROGATE,
    FACILITY_ACTION_EXT_INVOKE,
    FACILITY_ACTION_LINE_INVOKE,
    FACILITY_ACTION_MAILBOX_INVOKE,
    FACILITY_ACTION_OCB_ACTIVATE,
    FACILITY_ACTION_OCB_DEACTIVATE,
    FACILITY_ACTION_OCB_INTERROGATE,
    FACILITY_ACTION_PSO_ACTIVATE,
    FACILITY_ACTION_PW_SET,
    FACILITY_ACTION_SCF_ACTIVATE,
    FACILITY_ACTION_SCF_DEACTIVATE,
    FACILITY_ACTION_SCF_INTERROGATE,
    FACILITY_ACTION_SCREJ_ACTIVATE,
    FACILITY_ACTION_SCREJ_DEACTIVATE,
    FACILITY_ACTION_SCREJ_INTERROGATE,
    FACILITY_ACTION_SR_ACTIVATE,
    FACILITY_ACTION_SR_DEACTIVATE,
    FACILITY_ACTION_SR_INTERROGATE
} FACILITYACTIONS_ENUM;

typedef enum _H325AUTHMETHODS_ENUM
{
    H325_AUTH_DHEXCH,
    H325_AUTH_PWDSYMENC,
    H325_AUTH_PWDHASH,
    H325_AUTH_CERTSIGN,
    H325_AUTH_IPSEC,
    H325_AUTH_TLS
} H325AUTHMETHODS_ENUM;

typedef enum _TLSAUTHENTICATIONPROTOCOLS_ENUM
{
    TLS_AUTH_PROTOCOL_NULL,
    TLS_AUTH_PROTOCOL_MD5,
    TLS_AUTH_PROTOCOL_SHA_1,
    TLS_AUTH_PROTOCOL_SHA_2,
    TLS_AUTH_PROTOCOL_AEAD
} TLSAUTHENTICATIONPROTOCOLS_ENUM;

typedef enum _TLSENCRYPTIONPROTOCOLS_ENUM
{
    TLS_ENCRYPT_PROTOCOL_NULL,
    TLS_ENCRYPT_PROTOCOL_RC4,
    TLS_ENCRYPT_PROTOCOL_RC2,
    TLS_ENCRYPT_PROTOCOL_DES,
    TLS_ENCRYPT_PROTOCOL_3DES,
    TLS_ENCRYPT_PROTOCOL_AES,
    TLS_ENCRYPT_PROTOCOL_CAMELLIA
} TLSENCRYPTIONPROTOCOLS_ENUM;

typedef enum _TLSKEYEXCHANGEPROTOCOLS_ENUM
{
    TLS_EXCHANGE_PROTOCOL_RSA,
    TLS_EXCHANGE_PROTOCOL_DSS,
    TLS_EXCHANGE_PROTOCOL_DHE_RSA,
    TLS_EXCHANGE_PROTOCOL_DHE_DSS,
    TLS_EXCHANGE_PROTOCOL_ECDHE_RSA,
    TLS_EXCHANGE_PROTOCOL_ECDHE_ECDSA
} TLSKEYEXCHANGEPROTOCOLS_ENUM;

typedef enum _STATUS_ENUM
{
    STATUS_UP,
    STATUS_ERROR,
    STATUS_TESTING,
    STATUS_DISABLED
} STATUS_ENUM;

typedef enum _SIP_STATUS_ENUM
{
    SIP_STATUS_UP,
    SIP_STATUS_ERROR,
    SIP_STATUS_DISABLED
} SIP_STATUS_ENUM;

typedef enum _APPLICATIONINTERFACE_ENUM
{
   APP_INTERFACE_VOIP,
   APP_INTERFACE_VOATM,
   APP_INTERFACE_DATA
} APPLICATIONINTERFACE_ENUM;

typedef enum _ISDN_BRI_PROTOCOL_ENUM
 {
    BRI_PROTOCOL_EUROISDN,
    BRI_PROTOCOL_NI_1,
    BRI_PROTOCOL_NI_2,
    BRI_PROTOCOL_5ESS,
    BRI_PROTOCOL_NTT,
    BRI_PROTOCOL_VN3,
    BRI_PROTOCOL_VN4,
    BRI_PROTOCOL_QSIG
} ISDN_BRI_PROTOCOL_ENUM;

typedef enum _ISDN_PRI_PROTOCOL_ENUM
{
    PRI_PROTOCOL_EUROISDN,
    PRI_PROTOCOL_NI_1,
    PRI_PROTOCOL_NI_2,
    PRI_PROTOCOL_4ESS,
    PRI_PROTOCOL_5ESS,
    PRI_PROTOCOL_NTT,
    PRI_PROTOCOL_QSIG
} ISDN_PRI_PROTOCOL_ENUM;

typedef enum _ISDN_PROTOCOL_ENUMULATION_ENUM
{
    ISDN_PROTO_ENUMULATION_TE,
    ISDN_PROTO_ENUMULATION_NT
} ISDN_PROTOCOL_ENUMULATION_ENUM;

typedef enum _ISDN_TEI_NEGOTIATION_ENUM
{
    ISDN_TEI_NEGO_STATIC,
    ISDN_TEI_NEGO_DYNAMIC
} ISDN_TEI_NEGOTIATION_ENUM;

typedef enum _ISDN_LINECODE_ENUM
{
    ISDN_LINECODE_AMI,
    ISDN_LINECODE_HDB3,
    ISDN_LINECODE_B8ZS
} ISDN_LINECODE_ENUM;

typedef enum _ISDN_ESF_ENUM
{
    ISDN_ESF_NONE,
    ISDN_ESF_DF,
    ISDN_ESF_MF,
    ISDN_ESF_EMF,
    ISDN_ESF_SF,
    ISDN_ESF_ESF,
    ISDN_ESF_AUTODETECT
}ISDN_ESF_ENUM;

typedef enum _ISDN_PHYINTERFACETYPE_ENUM
{
    ISDN_PHYINTERFACE_T1,
    ISDN_PHYINTERFACE_E1
}ISDN_PHYINTERFACETYPE_ENUM;

typedef enum _ISDN_CLOCKMODE_ENUM
{
    ISDN_CLKMODE_MASTER,
    ISDN_CLKMODE_SLAVE
} ISDN_CLOCKMODE_ENUM;

typedef enum _SIGNALING_MODE_ENUM
{
    SIGNALING_MODE_LOOPSTART,
    SIGNALING_MODE_GROUNDSTART
} SIGNALING_MODE_ENUM;

typedef enum _DIAG_STATE_ENUM
{
    DIAG_STATE_NONE,
    DIAG_STATE_REQUESTED,
    DIAG_STATE_COMPLETE,
    DIAG_STATE_ERROR_INTERNAL,
    DIAG_STATE_ERROR_OTHER
} DIAG_STATE_ENUM;

typedef enum _FXO_TEST_SELECTOR_ENUM
{
    FXO_TEST_SELECTOR_BATTERY,
    FXO_TEST_SELECTOR_DIALTONE
} FXO_TEST_SELECTOR_ENUM;

typedef enum _FXS_TEST_SELECTOR_ENUM
{
    FXS_TEST_SELECTOR_HAZARD_POTENTIAL,
    FXS_TEST_SELECTOR_FOREIGN_VOLTAGE,
    FXS_TEST_SELECTOR_RESISTIVE_FAULTS,
    FXS_TEST_SELECTOR_OFF_HOOK,
    FXS_TEST_SELECTOR_REN
} FXS_TEST_SELECTOR_ENUM;

typedef enum _TEST_RESULT_ENUM
{
    TEST_RESULT_SUCCESS,
    TEST_RESULT_FAILURE
} TEST_RESULT_ENUM;

typedef enum _TERMINAL_TYPE_ENUM
{
    TERMINAL_AUDIO,
    TERMINAL_FAX,
    TERMINAL_MODEM,
    TERMINAL_ANY
} TERMINAL_TYPE_ENUM;

typedef enum _STREAM_TYPE_ENUM
{
    STREAM_AUDIO,
    STREAM_VIDEO,
    STREAM_FAX,
    STREAM_MODEM
} STREAM_TYPE_ENUM;


typedef enum _FUNCTION_TYPE_ENUM
{
    FUNCTION_DEFAULT,
    FUNCTION_CCBSCALLBACK,
    FUNCTION_CCNRCALLBACK,
    FUNCTION_INTERNALCALL,
    FUNCTION_RINGSPLASH,
    FUNCTION_GROUP
} FUNCTION_TYPE_ENUM;

typedef enum _STANDARD_ENUM
{
    STANDARD_GAP,
    STANDARD_CAT_IQ_1_0,
    STANDARD_CAT_IQ_2_0,
    STANDARD_CAT_IQ_2_1,
    STANDARD_CAT_IQ_3_0,
    STANDARD_CAT_IQ_4_0
} STANDARD_ENUM, PORTABLE_TYPE_ENUM;

typedef enum _RF_POWER_CTRL_ENUM
{
    POWER_CTRL_NORMAL,
    POWER_CTRL_REDUCED
} RF_POWER_CTRL_ENUM;

typedef enum _ENCRYPTION_TYPE_ENUM
{
    ENCRYPTION_DSC,
    ENCRYPTION_DSC2,
    ENCRYPTION_CCM
} ENCRYPTION_TYPE_ENUM;

typedef enum _ORIGIN_ENUM
{
    ORIGIN_AUTOCONFIGURED,
    ORIGIN_STATIC
} ORIGIN_ENUM;

typedef enum _SIP_CLIENT_CONTACT_STATUS_ENUM
{
    SIP_CLIENT_CONTACT_STATUS_AUTOCONFIGURED,
    SIP_CLIENT_CONTACT_STATUS_STATIC
} SIP_CLIENT_CONTACT_STATUS_ENUM;

typedef enum _TRANSPORT_ENUM
{
    TRANSPORT_UDP,
    TRANSPORT_TCP,
    TRANSPORT_TLS,
    TRANSPORT_SCTP
} TRANSPORT_ENUM;

typedef enum _CODECPRIORITY_ENUM
{
    CODEC_PRIORITY_LOCAL,
    CODEC_PRIORITY_REMOTE
} CODECPRIORITY_ENUM;

typedef enum _PASS_THROUGH_ENUM
{
    PASS_THROUGH_DISABLE,
    PASS_THROUGH_AUTO,
    PASS_THROUGH_FORCE
}PASS_THROUGH_ENUM;

typedef enum _REGISTRATION_STATUS_ENUM
{
    REGISTRATION_IN_REACH,
    REGISTRATION_NOT_IN_REACH
} REGISTRATION_STATUS_ENUM;

typedef enum _CONTROL_ENUM
{
    CONTROL_UNREGISTER,
    CONTROL_DISABLE
}CONTROL_ENUM;

typedef enum _CLIENT_STATUS_ENUM
{
    CLIENT_STATUS_UP,
    CLIENT_STATUS_INITIALIZING,
    CLIENT_STATUS_REGISTERING,
    CLIENT_STATUS_DEREGISTERING,
    CLIENT_STATUS_ERROR_MISCONFIGURED,
    CLIENT_STATUS_ERROR_NETWORK,
    CLIENT_STATUS_ERROR_REGISTRATION,
    CLIENT_STATUS_QUIESCENT,
    CLIENT_STATUS_DISABLED
}CLIENT_STATUS_ENUM;

typedef enum _NETWORK_STATUS_ENUM
{
    NETWORK_STATUS_UP,
    NETWORK_STATUS_RESOLVING,
    NETWORK_STATUS_ERROR_DNS,
    NETWORK_STATUS_ERROR_OTHER,
    NETWORK_STATUS_DISABLED
}NETWORK_STATUS_ENUM;

typedef enum _TRUNK_STATUS_ENUM
{
    TRUNK_STATUS_UP,
    TRUNK_STATUS_INITIALIZING,
    TRUNK_STATUS_REGISTERING,
    TRUNK_STATUS_UNREGISTERING,
    TRUNK_STATUS_ERROR,
    TRUNK_STATUS_TESTING,
    TRUNK_STATUS_DISABLED
} TRUNK_STATUS_ENUM;

typedef enum _TERMINAL_STATUS_ENUM
{
    TERMINAL_STATUS_UP,
    TERMINAL_STATUS_INITIALIZING,
    TERMINAL_STATUS_REGISTERING,
    TERMINAL_STATUS_UNREGISTERING,
    TERMINAL_STATUS_ERROR,
    TERMINAL_STATUS_TESTING,
    TERMINAL_STATUS_QUIESCENT,
    TERMINAL_STATUS_DISABLED
} TERMINAL_STATUS_ENUM;

typedef enum _LINE_STATUS_ENUM
{
    LINE_STATUS_UP,
    LINE_STATUS_ERROR,
    LINE_STATUS_TESTING,
    LINE_STATUS_QUIESCENT,
    LINE_STATUS_DISABLED
} LINE_STATUS_ENUM;


typedef enum _INTERWORK_STATUS_ENUM
{
    INTERWORK_STATUS_UP,
    INTERWORK_STATUS_ERROR,
    INTERWORK_STATUS_QUIESCENT,
    INTERWORK_STATUS_DISABLED
} INTERWORK_STATUS_ENUM;

typedef enum _CALLCTRL_EXT_STATUS_ENUM
{
    CALLCTRL_CALLSTATUS_UP,
    CALLCTRL_CALLSTATUS_INITIALIZING,
    CALLCTRL_CALLSTATUS_ERROR,
    CALLCTRL_CALLSTATUS_TESTING,
    CALLCTRL_CALLSTATUS_QUIESCENT,
    CALLCTRL_CALLSTATUS_DISABLED
} CALLCTRL_EXT_STATUS_ENUM;

typedef enum _MGCP_REGISTER_MODE_ENUM
{
   MGCP_REGISTER_WILDCARD,
   MGCP_REGISTER_INDIVIDUAL
} MGCP_REGISTER_MODE_ENUM;

typedef enum _CALLSTATUS_ENUM
{
    CALLSTATUS_IDLE,
    CALLSTATUS_DIALING,
    CALLSTATUS_DELIVERED,
    CALLSTATUS_CONNECTED,
    CALLSTATUS_ALERTING,
    CALLSTATUS_DISCONNECTED
} CALLSTATUS_ENUM;

typedef enum _CALLCTRL_EXT_CALLSTATUS_ENUM
{
    CALLCTRL_STATUS_IDLE,
    CALLCTRL_STATUS_DIALING,
    CALLCTRL_STATUS_DELIVERED,
    CALLCTRL_STATUS_CONNECTED,
    CALLCTRL_STATUS_ALERTING,
    CALLCTRL_STATUS_DISCONNECTED
} CALLCTRL_EXT_CALLSTATUS_ENUM;

typedef enum _ACC_REGISTRATION_STATUS_ENUM
{
    ACC_REGISTERED,
    ACC_UNREGISTERED
} ACC_REGISTRATION_STATUS_ENUM;


typedef enum _SIP_REGISTRAR_ACC_ENABLE_ENUM
{
    SIP_REGISTRAR_ACC_ENABLE,
    SIP_REGISTRAR_ACC_QUIESCENT,
    SIP_REGISTRAR_ACC_DISABLE
} SIP_REGISTRAR_ACC_ENABLE_ENUM;

typedef enum _REGISTER_MODE_ENUM
{
    REGISTER_MODE_RFC3261,
    REGISTER_MODE_STATIC,
    REGISTER_MODE_RFC6140,
    REGISTER_MODE_TISPAN
} REGISTER_MODE_ENUM;

typedef enum _MGCP_NETWORK_REG_STATUS_ENUM
{
    MGCP_NETWORK_REG_STATUS_UP,
    MGCP_NETWORK_REG_STATUS_RESOLVING,
    MGCP_NETWORK_REG_STATUS_ERROR_DNS,
    MGCP_NETWORK_REG_STATUS_ERROR_OTHER,
    MGCP_NETWORK_REG_STATUS_DISABLED
} MGCP_NETWORK_REG_STATUS_ENUM;

typedef struct _DML_VOICESERVICE_CAPABILITIES_SIP_CLIENT
{
    CHAR                                 Extensions[STR_LEN_256];
    CHAR                                 URISchemes[STR_LEN_256];
    CHAR                                 EventTypes[STR_LEN_256];
    TLSAUTHENTICATIONPROTOCOLS_ENUM      TLSAuthenticationProtocols;
    CHAR                                 TLSAuthenticationKeySizes[STR_LEN_256];
    TLSENCRYPTIONPROTOCOLS_ENUM          TLSEncryptionProtocols;
    CHAR                                 TLSEncryptionKeySizes[STR_LEN_256];
    TLSKEYEXCHANGEPROTOCOLS_ENUM        TLSKeyExchangeProtocols;
} DML_VOICESERVICE_CAPABILITIES_SIP_CLIENT,*PDML_VOICESERVICE_CAPABILITIES_SIP_CLIENT;

typedef struct _DML_VOICESERVICE_CAPABILITIES_SIP_REGISTRAR
{
    CHAR                                Extensions[STR_LEN_256];
    CHAR                                URISchemes[STR_LEN_256];
    CHAR                                EventTypes[STR_LEN_256];
    TLSAUTHENTICATIONPROTOCOLS_ENUM     TLSAuthenticationProtocols;
    CHAR                                TLSAuthenticationKeySizes[STR_LEN_256];
    TLSENCRYPTIONPROTOCOLS_ENUM         TLSEncryptionProtocols;
    CHAR                                TLSEncryptionKeySizes[STR_LEN_256];
    TLSKEYEXCHANGEPROTOCOLS_ENUM       TLSKeyExchangeProtocols;
} DML_VOICESERVICE_CAPABILITIES_SIP_REGISTRAR, *PDML_VOICESERVICE_CAPABILITIES_SIP_REGISTRAR;


typedef struct _DML_VOICESERVICE_CAPABILITIES_SIP
 {
    DML_VOICESERVICE_CAPABILITIES_SIP_CLIENT        Client_Obj;
    DML_VOICESERVICE_CAPABILITIES_SIP_REGISTRAR     Registrar_Obj;
 } DML_VOICESERVICE_CAPABILITIES_SIP, *PDML_VOICESERVICE_CAPABILITIES_SIP;

typedef struct _DML_VOICESERVICE_CAPABILITIES_MGCP
 {
    CHAR Extensions[STR_LEN_256];
 } DML_VOICESERVICE_CAPABILITIES_MGCP,*PDML_VOICESERVICE_CAPABILITIES_MGCP;


typedef struct _DML_VOICESERVICE_CAPABILITIES_H323
 {
    BOOL FastStart;
    H325AUTHMETHODS_ENUM H235AuthenticationMethods;
 } DML_VOICESERVICE_CAPABILITIES_H323,*PDML_VOICESERVICE_CAPABILITIES_H323;

typedef struct _DML_VOICESERVICE_CAPABILITIES_ISDN
 {
    BOOL    MSN;
    BOOL    DDI;
    BOOL    MCID;
    BOOL    MWI;
    BOOL    AoC;
    BOOL    ECT;
 } DML_VOICESERVICE_CAPABILITIES_ISDN, *PDML_VOICESERVICE_CAPABILITIES_ISDN;

typedef struct _DML_VOICESERVICE_CAPABILITIES_POTS
 {
    CAPABILITIES_DIALTYPE_ENUM      DialType;
    BOOL                            ClipGeneration;
    BOOL                            ChargingPulse;
 } DML_VOICESERVICE_CAPABILITIES_POTS, *PDML_VOICESERVICE_CAPABILITIES_POTS;

typedef struct _DML_VOICESERVICE_CAPABILITIES_QUALITYINDICATOR
 {
    CHAR      QIModelsSupported[STR_LEN_256];
    UINT      MaxQIValues;
    UINT      MaxWorstQIValues;
 } DML_VOICESERVICE_CAPABILITIES_QUALITYINDICATOR, *PDML_VOICESERVICE_CAPABILITIES_QUALITYINDICATOR;

typedef struct _DML_VOICESERVICE_CAPABILITIES_CODEC
 {
    ULONG                       uInstanceNumber;
    void*                       pParentVoiceService;
    CHAR                        Alias[STR_LEN_64];
    CAPABILITIESCODEC_ENUM      Codec;
    UINT                        BitRate;
    CHAR                        PacketizationPeriod[STR_LEN_256];
    BOOL                        SilenceSuppression;
 } DML_VOICESERVICE_CAPABILITIES_CODEC, *PDML_VOICESERVICE_CAPABILITIES_CODEC;

typedef  struct _DML_VOICESERVICE_CAPABILITIES_CODEC_CTRL_T_
 {
    DML_VOICESERVICE_CAPABILITIES_CODEC     dml;
    BOOL                                    updated;
 } DML_VOICESERVICE_CAPABILITIES_CODEC_CTRL_T,*PDML_VOICESERVICE_CAPABILITIES_CODEC_CTRL_T;


typedef  struct _DML_VOICESERVICE_CAPABILITIES_CODEC_LIST_T_
 {
    UINT                                            ulQuantity;
    PDML_VOICESERVICE_CAPABILITIES_CODEC_CTRL_T     pdata[TELCOVOICE_DATA_MAX];
 } DML_VOICESERVICE_CAPABILITIES_CODEC_LIST_T,*PDML_VOICESERVICE_CAPABILITIES_CODEC_LIST_T;

typedef struct _DML_VOICESERVICE_CAPABILITIES
 {
    INT                                              MaxLineCount;
    INT                                              MaxExtensionCount;
    INT                                              MaxCallLogCount;
    INT                                              MaxSessionsPerLine;
    INT                                              MaxSessionsPerExtension;
    INT                                              MaxSessionCount;
    CHAR                                             NetworkConnectionModes[STR_LEN_256];
    CHAR                                             UserConnectionModes[STR_LEN_256];
    TONEFILEFORMATS_ENUM                             ToneFileFormats;
    RINGFILEFORMATS_ENUM                             RingFileFormats;
    FACILITYACTIONS_ENUM                             FacilityActions;
    DML_VOICESERVICE_CAPABILITIES_SIP                SIP;
    DML_VOICESERVICE_CAPABILITIES_MGCP               MGCP;
    DML_VOICESERVICE_CAPABILITIES_H323               H323;
    DML_VOICESERVICE_CAPABILITIES_ISDN               ISDN;
    DML_VOICESERVICE_CAPABILITIES_POTS               POTS;
    DML_VOICESERVICE_CAPABILITIES_CODEC_LIST_T       Codec;
    DML_VOICESERVICE_CAPABILITIES_QUALITYINDICATOR   QualityIndicator;
 } DML_VOICESERVICE_CAPABILITIES, *PDML_VOICESERVICE_CAPABILITIES;

typedef struct _DML_RESERVEDPORTS
{
    void*      pParentVoiceService;
    CHAR       WANPortRange[STR_LEN_256];
    CHAR       LANPortRange[STR_LEN_256];
} DML_RESERVEDPORTS,*PDML_RESERVEDPORTS;


typedef struct _DML_ISDN_BRI
{
    ULONG                            uInstanceNumber;
    void*                            pParentVoiceService;
    BOOL                             Enable;
    STATUS_ENUM                      Status;
    CHAR                             Alias[STR_LEN_64];
    CHAR                             Name[STR_LEN_64];
    CHAR                             ToneEventProfile[STR_LEN_256];
    APPLICATIONINTERFACE_ENUM        ApplicationInterface;
    ISDN_BRI_PROTOCOL_ENUM           Protocol;
    ISDN_PROTOCOL_ENUMULATION_ENUM   ProtocolEmulation;
    BOOL                             PermanentLayer2;
    BOOL                             PermanentLayer1;
    BOOL                             LapdDisconnectionTimeout;
    ISDN_TEI_NEGOTIATION_ENUM        TEINegotiation;
    UINT                             StaticTEI;
    BOOL                             LifeLineHold;
    BOOL                             PowerSourceOne;
    UINT                             MaxNumBChannels;
    CHAR                             OutboundOnlyBChannels[STR_LEN_256];
    CHAR                             InboundOnlyBChannels[STR_LEN_256];
    CHAR                             BidirectionalBChannels[STR_LEN_256];
} DML_ISDN_BRI, *PDML_ISDN_BRI;

typedef struct _DML_ISDN_PRI
{
    ULONG                             uInstanceNumber;
    void*                             pParentVoiceService;
    BOOL                              Enable;
    STATUS_ENUM                       Status;
    CHAR                              Alias[STR_LEN_64];
    CHAR                              Name[STR_LEN_64];
    CHAR                              ToneEventProfile[STR_LEN_256];
    APPLICATIONINTERFACE_ENUM         ApplicationInterface;
    ISDN_PRI_PROTOCOL_ENUM            Protocol;
    ISDN_PROTOCOL_ENUMULATION_ENUM    ProtocolEmulation;
    BOOL                              PermanentLayer2;
    ISDN_LINECODE_ENUM                Linecode;
    ISDN_ESF_ENUM                     ESF;
    ISDN_PHYINTERFACETYPE_ENUM        PhysicalInterfaceType;
    UINT                              MaxNumBChannels;
    CHAR                              OutboundOnlyBChannels[STR_LEN_256];
    CHAR                              InboundOnlyBChannels[STR_LEN_256];
    CHAR                              BidirectionalBChannels[STR_LEN_256];
    ISDN_CLOCKMODE_ENUM               ClockMode;

} DML_ISDN_PRI, *PDML_ISDN_PRI;

typedef  struct _DML_ISDN_BRI_CTRL_
 {
    DML_ISDN_BRI     dml;
    bool             updated;
 } DML_ISDN_BRI_CTRL_T, *PDML_ISDN_BRI_CTRL_T;

typedef  struct _DML_ISDN_BRI_LIST_
 {
     UINT                     ulQuantity;
     DML_ISDN_BRI_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_ISDN_BRI_LIST_T,*PDML_ISDN_BRI_LIST_T;

typedef  struct _DML_ISDN_PRI_CTRL_
 {
     DML_ISDN_PRI     dml;
     bool             updated;
 } DML_ISDN_PRI_CTRL_T,*PDML_ISDN_PRI_CTRL_T;

typedef  struct _DML_ISDN_PRI_LIST_
 {
     UINT                     ulQuantity;
     DML_ISDN_PRI_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_ISDN_PRI_LIST_T,*PDML_ISDN_PRI_LIST_T;

typedef struct _DML_ISDN
{
    DML_ISDN_BRI_LIST_T BRI;
    DML_ISDN_PRI_LIST_T PRI;
} DML_ISDN,*PDML_ISDN;


typedef struct _DML_POTS_FXO_DIAGTESTS
{
    DIAG_STATE_ENUM        DiagnosticsState;
    FXO_TEST_SELECTOR_ENUM TestSelector;
    TEST_RESULT_ENUM       TestResult;
} DML_POTS_FXO_DIAGTESTS,*PDML_POTS_FXO_DIAGTESTS;


typedef struct _DML_POTS_FXO
{
    ULONG                   uInstanceNumber;
    void*                   pParentVoiceService;
    BOOL                    Enable;
    STATUS_ENUM             Status;
    CHAR                    Alias[STR_LEN_64];
    CHAR                    Name[STR_LEN_64];
    CHAR                    ToneEventProfile[STR_LEN_256];
    BOOL                    SecondStepDialing;
    UINT                    TimeoutBeforeDialing;
    UINT                    RingingTimeout;
    UINT                    RingNumber;
    UINT                    OnHookMinDuration;
    SIGNALING_MODE_ENUM     SignalingMode;
    UINT                    DTMFDialoutInterval;
    BOOL                    CallerIdDetectionEnable;
    BOOL                    Active;
    DML_POTS_FXO_DIAGTESTS  DiagTests;
} DML_POTS_FXO,*PDML_POTS_FXO;

typedef struct _DML_POTS_FXS_DIAGTESTS
{
    DIAG_STATE_ENUM        DiagnosticsState;
    FXS_TEST_SELECTOR_ENUM TestSelector;
    TEST_RESULT_ENUM       TestResult;
} DML_POTS_FXS_DIAGTESTS,*PDML_POTS_FXS_DIAGTESTS;

typedef struct _DML_POTS_FXS_VOICEPROCESSING
{
    INT       TransmitGain;
    INT       ReceiveGain;
    BOOL      EchoCancellationEnable;
    BOOL      EchoCancellationInUse;
    UINT      EchoCancellationTail;
} DML_POTS_FXS_VOICEPROCESSING,*PDML_POTS_FXS_VOICEPROCESSING;

typedef struct _DML_POTS_FXS
{
    ULONG                         uInstanceNumber;
    void*                         pParentVoiceService;
    BOOL                          Enable;
    STATUS_ENUM                   Status;
    CHAR                          Alias[STR_LEN_64];
    CHAR                          Name[STR_LEN_64];
    CHAR                          ToneEventProfile[STR_LEN_256];
    PASS_THROUGH_ENUM             FaxPassThrough;
    PASS_THROUGH_ENUM             ModemPassThrough;
    POTS_DIALTYPE_ENUM            DialType;
    BOOL                          ClipGeneration;
    BOOL                          ChargingPulse;
    BOOL                          Active;
    TERMINAL_TYPE_ENUM            TerminalType;
    DML_POTS_FXS_VOICEPROCESSING  VoiceProcessing;
    DML_POTS_FXS_DIAGTESTS        DiagTests;
} DML_POTS_FXS,*PDML_POTS_FXS;


typedef  struct  _DML_POTS_FXS_CTRL_
 {
    DML_POTS_FXS     dml;
    bool             updated;
 } DML_POTS_FXS_CTRL_T, *PDML_POTS_FXS_CTRL_T;

typedef  struct _DML_POTS_FXS_LIST_
 {
    UINT                     ulQuantity;
    DML_POTS_FXS_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_POTS_FXS_LIST_T, *PDML_POTS_FXS_LIST_T;

typedef  struct _DML_POTS_FXO_CTRL_
 {
    DML_POTS_FXO     dml;
    bool             updated;
 } DML_POTS_FXO_CTRL_T, *PDML_POTS_FXO_CTRL_T;

typedef  struct _DML_POTS_FXO_LIST_
 {
    UINT                     ulQuantity;
    DML_POTS_FXO_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_POTS_FXO_LIST_T, *PDML_POTS_FXO_LIST_T;

typedef  struct _DML_POTS_RINGER_EVT
 {
    ULONG                         uInstanceNumber;
    void*                         pParentVoiceService;
    CHAR                          Alias[STR_LEN_64];
    FUNCTION_TYPE_ENUM            Function;
    CHAR                          Cadence[STR_LEN_256];
 } DML_POTS_RINGER_EVT,*PDML_POTS_RINGER_EVT;

typedef  struct _DML_POTS_RINGER_EVT_CTRL_
 {
    DML_POTS_RINGER_EVT     dml;
    bool                    updated;
 } DML_POTS_RINGER_EVT_CTRL_T, *PDML_POTS_RINGER_EVT_CTRL_T;

typedef  struct _DML_POTS_RINGER_EVT_LIST_
 {
    UINT                            ulQuantity;
    DML_POTS_RINGER_EVT_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_POTS_RINGER_EVT_LIST_T, *PDML_POTS_RINGER_EVT_LIST_T;

typedef struct _DML_POTS_RINGER
{
    DML_POTS_RINGER_EVT_LIST_T Event;
} DML_POTS_RINGER,*PDML_POTS_RINGER;

typedef struct _DML_POTS
{
    CHAR                 Region[STR_LEN_2];
    DML_POTS_RINGER      Ringer_Obj;
    DML_POTS_FXO_LIST_T  FXO;
    DML_POTS_FXS_LIST_T  FXS;
} DML_POTS,*PDML_POTS;

typedef struct _DML_DECT_BASE_STATS
{
    UINT   Handovers;
    UINT   HandoverFailures;
    UINT   ControlFieldErrors;
    UINT   PayloadFieldErrors;
    UINT   SyncFailures;
} DML_DECT_BASE_STATS,*PDML_DECT_BASE_STATS;

typedef struct _DML_DECT_BASE
{
    ULONG                     uInstanceNumber;
    void*                     pParentVoiceService;
    BOOL                      Enable;
    STATUS_ENUM               Status;
    CHAR                      Alias[STR_LEN_64];
    CHAR                      Name[STR_LEN_64];
    CHAR                      ToneEventProfile[STR_LEN_256];
    STANDARD_ENUM             Standard;
    CHAR                      RFPI[STR_LEN_5];
    UINT                      MaxSupportedPP;
    CHAR                      PIN[STR_LEN_4];
    BOOL                      RepeaterSupportEnabled;
    BOOL                      NEMOEnable;
    BOOL                      SubscriptionEnable;
    BOOL                      CipheringEnable;
    ENCRYPTION_TYPE_ENUM      EncryptionType;
    RF_POWER_CTRL_ENUM        RFPowerControl;
    CHAR                      FirmwareVersion[STR_LEN_20];
    CHAR                      EepromVersion[STR_LEN_20];
    CHAR                      HardwareVersion[STR_LEN_20];
    DML_DECT_BASE_STATS       Stats;
} DML_DECT_BASE,*PDML_DECT_BASE;

typedef struct _DML_DECT_PORTABLE
{
    ULONG                       uInstanceNumber;
    void*                       pParentVoiceService;
    BOOL                        Enable;
    STATUS_ENUM                 Status;
    CHAR                        Alias[STR_LEN_64];
    CHAR                        CodecList[STR_LEN_256];
    REGISTRATION_STATUS_ENUM    RegistrationStatus;
    CHAR                        IPUI[STR_LEN_13];
    UINT                        IPUILength;
    CHAR                        IPEI[STR_LEN_5];
    CHAR                        PARK[STR_LEN_5];
    CHAR                        BaseAttachedTo[STR_LEN_256];
    PORTABLE_TYPE_ENUM          PortableType;
    CHAR                        SubscriptionTime[STR_LEN_24];
    CONTROL_ENUM                Control;
    CHAR                        HardwareVersion[STR_LEN_20];
    CHAR                        SoftwareVersion[STR_LEN_20];
    BOOL                        SoftwareUpgrade;
    CHAR                        LastUpdateDateTime[STR_LEN_24];
    CHAR                        OperatorName[STR_LEN_32];
} DML_DECT_PORTABLE,*PDML_DECT_PORTABLE;

typedef  struct _DML_DECT_BASE_CTRL_
 {
    DML_DECT_BASE       dml;
    bool                updated;
 } DML_DECT_BASE_CTRL_T, *PDML_DECT_BASE_CTRL_T;

typedef  struct _DML_DECT_BASE_LIST_
 {
    UINT                        ulQuantity;
    DML_DECT_BASE_CTRL_T*       pdata[TELCOVOICE_DATA_MAX];
 } DML_DECT_BASE_LIST_T, *PDML_DECT_BASE_LIST_T;

typedef  struct _DML_DECT_PORTABLE_CTRL_
 {
    DML_DECT_PORTABLE      dml;
    bool                   updated;
 } DML_DECT_PORTABLE_CTRL_T, *PDML_DECT_PORTABLE_CTRL_T;

typedef  struct _DML_DECT_PORTABLE_LIST_
 {
    UINT                            ulQuantity;
    DML_DECT_PORTABLE_CTRL_T*       pdata[TELCOVOICE_DATA_MAX];
 } DML_DECT_PORTABLE_LIST_T, *PDML_DECT_PORTABLE_LIST_T;

typedef struct _DML_DECT
{
    DML_DECT_BASE_LIST_T        Base;
    DML_DECT_PORTABLE_LIST_T    Portable;
} DML_DECT,*PDML_DECT;

typedef struct _DML_SIP_CLIENT_CONTACT
{
    ULONG                           uInstanceNumber;
    void*                           pParentVoiceService;
    void*                           pParentSipClient;
    BOOL                            Enable;
    SIP_STATUS_ENUM                 Status;
    CHAR                            Alias[STR_LEN_64];
    ORIGIN_ENUM                     Origin;
    CHAR                            IPAddress[STR_LEN_45];
    UINT                            Port;
    CHAR                            ContactURI[STR_LEN_256];
    CHAR                            ExpireTime[STR_LEN_24];
    UINT                            PingInterval;
    CHAR                            UserAgent[STR_LEN_256];
} DML_SIP_CLIENT_CONTACT, *PDML_SIP_CLIENT_CONTACT;

typedef struct _DML_SIP_CLIENT_SIPEVENT
{
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    void*   pParentSipClient;
    BOOL    Enable;
    CHAR    Alias[STR_LEN_64];
    CHAR    Event[STR_LEN_32];
    CHAR    AuthUserName[STR_LEN_128];
    CHAR    AuthPassword[STR_LEN_128];
} DML_SIP_CLIENT_SIPEVENTSUB, *PDML_SIP_CLIENT_SIPEVENTSUB;

typedef  struct _DML_SIP_CLIENT_CONTACT_CTRL_
 {
    DML_SIP_CLIENT_CONTACT      dml;
    bool                        updated;
 } DML_SIP_CLIENT_CONTACT_CTRL_T, *PDML_SIP_CLIENT_CONTACT_CTRL_T;

typedef  struct _DML_SIP_CLIENT_CONTACT_LIST_
 {
    UINT                                ulQuantity;
    DML_SIP_CLIENT_CONTACT_CTRL_T*      pdata[TELCOVOICE_DATA_MAX];
 }  DML_SIP_CLIENT_CONTACT_LIST_T, *PDML_SIP_CLIENT_CONTACT_LIST_T;

typedef  struct _DML_SIP_CLIENT_SIPEVENTSUB_CTRL_
 {
    DML_SIP_CLIENT_SIPEVENTSUB      dml;
    bool                            updated;
 } DML_SIP_CLIENT_SIPEVENTSUB_CTRL_T, *PDML_SIP_CLIENT_SIPEVENTSUB_CTRL_T;

typedef  struct _DML_SIP_CLIENT_SIPEVENTSUB_LIST_
 {
    UINT                                    ulQuantity;
    DML_SIP_CLIENT_SIPEVENTSUB_CTRL_T*      pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_CLIENT_SIPEVENTSUB_LIST_T, *PDML_SIP_CLIENT_SIPEVENTSUB_LIST_T;

typedef  struct _DML_SIP_CLIENT
 {
    ULONG                             uInstanceNumber;
    void*                             pParentVoiceService;
    BOOL                              Enable;
    BOOL                              QuiescentMode;
    CLIENT_STATUS_ENUM                Status;
    CHAR                              Alias[STR_LEN_64];
    ORIGIN_ENUM                       Origin;
    REGISTER_MODE_ENUM                RegisterMode;
    CHAR                              AuthUserName[STR_LEN_128];
    CHAR                              AuthPassword[STR_LEN_128];
    CHAR                              Network[STR_LEN_256];
    UINT                              MaxSessions;
    CHAR                              RegisterURI[STR_LEN_389];
    BOOL                              E164Format;
    BOOL                              T38Enable;
    DML_SIP_CLIENT_CONTACT_LIST_T     Contact;
    DML_SIP_CLIENT_SIPEVENTSUB_LIST_T SIPEventSub;
 } DML_SIP_CLIENT,*PDML_SIP_CLIENT;

typedef struct _SIP_NETWORK_FQDNSERVER
{
    ULONG           uInstanceNumber;
    void*           pParentVoiceService;
    void*           pParentSipNetwork;
    BOOL            Enable;
    CHAR            Alias[STR_LEN_64];
    ORIGIN_ENUM     Origin;
    CHAR            Domain[STR_LEN_256];
    UINT            Weight;
    UINT            Priority;
    UINT            Port;
    CHAR IPAddresses[STR_LEN_256];
} DML_SIP_NETWORK_FQDNSERVER,*PDML_SIP_NETWORK_FQDNSERVER;


typedef struct _SIP_NETWORK_EVENTSUBSCRIBE
{
    ULONG           uInstanceNumber;
    void*           pParentVoiceService;
    void*           pParentSipNetwork;
    BOOL            Enable;
    CHAR            Alias[STR_LEN_64];
    CHAR            Event[STR_LEN_32];
    CHAR            Notifier[STR_LEN_256];
    UINT            NotifierPort;
    TRANSPORT_ENUM  NotifierTransport;
    UINT            ExpireTime;
} DML_SIP_NETWORK_EVENTSUBSCRIBE,*PDML_SIP_NETWORK_EVENTSUBSCRIBE;

typedef struct _SIP_NETWORK_RESPMAP
{
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    void*   pParentSipNetwork;
    CHAR    Alias[STR_LEN_64];
    UINT    SIPResponseNumber;
    BOOL    Enable;
    CHAR    TextMessage[STR_LEN_64];
    CHAR    Tone[STR_LEN_256];
} DML_SIP_NETWORK_RESPMAP,*PDML_SIP_NETWORK_RESPMAP;

typedef  struct _DML_SIP_NETWORK_FQDNSERVER_CTRL_
 {
    DML_SIP_NETWORK_FQDNSERVER      dml;
    bool                            updated;
 } DML_SIP_NETWORK_FQDNSERVER_CTRL_T, *PDML_SIP_NETWORK_FQDNSERVER_CTRL_T;

typedef  struct _DML_SIP_NETWORK_FQDNSERVER_LIST_
 {
    UINT                                    ulQuantity;
    DML_SIP_NETWORK_FQDNSERVER_CTRL_T*      pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_NETWORK_FQDNSERVER_LIST_T, *PDML_SIP_NETWORK_FQDNSERVER_LIST_T;

typedef  struct _DML_SIP_NETWORK_EVENTSUBSCRIBE_CTRL_
 {
    DML_SIP_NETWORK_EVENTSUBSCRIBE     dml;
    bool                               updated;
 } DML_SIP_NETWORK_EVENTSUBSCRIBE_CTRL_T, *PDML_SIP_NETWORK_EVENTSUBSCRIBE_CTRL_T;

typedef  struct _DML_SIP_NETWORK_EVENTSUBSCRIBE_LIST_
 {
    UINT                                        ulQuantity;
    DML_SIP_NETWORK_EVENTSUBSCRIBE_CTRL_T*      pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_NETWORK_EVENTSUBSCRIBE_LIST_T, *PDML_SIP_NETWORK_EVENTSUBSCRIBE_LIST_T;

typedef  struct _DML_SIP_NETWORK_RESPMAP_CTRL_
 {
    DML_SIP_NETWORK_RESPMAP     dml;
    bool                        updated;
 } DML_SIP_NETWORK_RESPMAP_CTRL_T, *PDML_SIP_NETWORK_RESPMAP_CTRL_T;

typedef  struct _DML_SIP_NETWORK_RESPMAP_LIST_
 {
    UINT                                ulQuantity;
    DML_SIP_NETWORK_RESPMAP_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_NETWORK_RESPMAP_LIST_T, *PDML_SIP_NETWORK_RESPMAP_LIST_T;

typedef enum _PRECEDENCE_ENUM
{
    PRECEDENCE_STATIC,
    PRECEDENCE_DHCP
} PRECEDENCE_ENUM;

typedef enum _INBOUNDAUTH_ENUM
{
    INBOUNDAUTH_NONE,
    INBOUNDAUTH_DIGEST,
    INBOUNDAUTH_SOURCEFILTER
}INBOUNDAUTH_ENUM;

typedef  struct _DML_SIP_NETWORK
 {
    ULONG                                   uInstanceNumber;
    void*                                   pParentVoiceService;
    BOOL                                    Enable;
    BOOL                                    QuiescentMode;
    NETWORK_STATUS_ENUM                     Status;
    CHAR                                    Alias[STR_LEN_64];
    CHAR                                    X_RDK_Firewall_Rule_Data[STR_LEN_256];
    CHAR                                    ProxyServer[STR_LEN_256];
    UINT                                    ProxyServerPort;
    TRANSPORT_ENUM                          ProxyServerTransport;
    CHAR                                    RegistrarServer[STR_LEN_256];
    UINT                                    RegistrarServerPort;
    TRANSPORT_ENUM                          RegistrarServerTransport;
    CHAR                                    ServerDomain[STR_LEN_256];
    CHAR                                    ChosenDomain[STR_LEN_256];
    CHAR                                    ChosenIPAddress[STR_LEN_45];
    UINT                                    ChosenPort;
    CHAR                                    UserAgentDomain[STR_LEN_256];
    UINT                                    UserAgentPort;
    TRANSPORT_ENUM                          UserAgentTransport;
    CHAR                                    OutboundProxy[STR_LEN_256];
    CHAR                                    OutboundProxyResolvedAddress[STR_LEN_45];
    PRECEDENCE_ENUM                         OutboundProxyPrecedence;
    UINT                                    OutboundProxyPort;
    BOOL                                    STUNEnable;
    CHAR                                    STUNServer[STR_LEN_256];
    UINT                                    NonVoiceBandwidthReservedUpstream;
    UINT                                    NonVoiceBandwidthReservedDownstream;
    CHAR                                    Organization[STR_LEN_256];
    UINT                                    RegistrationPeriod;
    CHAR                                    Realm[STR_LEN_256];
    UINT                                    TimerT1;
    UINT                                    TimerT2;
    UINT                                    TimerT4;
    UINT                                    TimerA;
    UINT                                    TimerB;
    UINT                                    TimerC;
    UINT                                    TimerD;
    UINT                                    TimerE;
    UINT                                    TimerF;
    UINT                                    TimerG;
    UINT                                    TimerH;
    UINT                                    TimerI;
    UINT                                    TimerJ;
    UINT                                    TimerK;
    UINT                                    InviteExpires;
    UINT                                    ReInviteExpires;
    UINT                                    RegisterExpires;
    UINT                                    RegisterRetryInterval;
    INBOUNDAUTH_ENUM                        InboundAuth;
    CHAR                                    InboundAuthUsername[STR_LEN_256];
    CHAR                                    InboundAuthPassword[STR_LEN_256];
    BOOL                                    UseCodecPriorityInSDPResponse;
    UINT                                    DSCPMark;
    INT                                     VLANIDMark;
    INT                                     EthernetPriorityMark;
    CHAR                                    ConferenceCallDomainURI[STR_LEN_256];
    UINT                                    TimerLoginRejected;
    BOOL                                    NoLoginRetry;
    UINT                                    TimerRegistrationFailed;
    UINT                                    TimerSubscriptionFailed;
    UINT                                    UnansweredRegistrationAttempts;
    CHAR                                    VoIPProfile[STR_LEN_256];
    CHAR                                    CodecList[STR_LEN_256];
    UINT                                    MaxSessions;
    UINT                                    X_RDK_SKBMark;
    BOOL                                    X_RDK_Central_COM_NetworkDisconnect;
    CHAR                                    X_RDK_Central_COM_ConferencingURI[STR_LEN_256];
    BOOL                                    X_RDK_PRACKRequired;
    DML_SIP_NETWORK_FQDNSERVER_LIST_T       FQDNServer;
    DML_SIP_NETWORK_EVENTSUBSCRIBE_LIST_T   EventSubscribe;
    DML_SIP_NETWORK_RESPMAP_LIST_T          ResponseMap;
 } DML_SIP_NETWORK, *PDML_SIP_NETWORK;

typedef  struct _DML_SIP_NETWORK_CTRL_
 {
    DML_SIP_NETWORK     dml;
    bool                updated;
 } DML_SIP_NETWORK_CTRL_T, *PDML_SIP_NETWORK_CTRL_T;

typedef  struct _DML_SIP_NETWORK_LIST_
 {
    UINT                        ulQuantity;
    DML_SIP_NETWORK_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_NETWORK_LIST_T, *PDML_SIP_NETWORK_LIST_T;

typedef  struct _DML_SIP_CLIENT_CTRL_
 {
    DML_SIP_CLIENT      dml;
    bool                updated;
 } DML_SIP_CLIENT_CTRL_T,*PDML_SIP_CLIENT_CTRL_T;

typedef  struct _DML_SIP_CLIENT_LIST_
 {
    UINT                       ulQuantity;
    DML_SIP_CLIENT_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_CLIENT_LIST_T, *PDML_SIP_CLIENT_LIST_T;

typedef struct _DML_SIP_PROXY
{
    ULONG               uInstanceNumber;
    void*               pParentVoiceService;
    BOOL                Enable;
    SIP_STATUS_ENUM     Status;
    CHAR                Alias[STR_LEN_64];
    ORIGIN_ENUM         Origin;
    CHAR                ProxyIPAddress[STR_LEN_45];
    UINT                ProxyPort;
    CHAR                ContactURI[STR_LEN_256];
    CHAR                VoIPProfile[STR_LEN_256];
} DML_SIP_PROXY, *PDML_SIP_PROXY;

typedef  struct _DML_SIP_PROXY_CTRL_
 {
    DML_SIP_PROXY       dml;
    bool                updated;
 } DML_SIP_PROXY_CTRL_T, *PDML_SIP_PROXY_CTRL_T;

typedef  struct _DML_SIP_PROXY_LIST_
 {
    UINT                      ulQuantity;
    DML_SIP_PROXY_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_PROXY_LIST_T, *PDML_SIP_PROXY_LIST_T;

typedef  struct _DML_SIP_REGISTRAR_ACCOUNT_CONTACT
 {
    ULONG           uInstanceNumber;
    void*           pParentVoiceService;
    void*           pParentSipRegister;
    void*           pParentSipRegisterAccount;
    BOOL            Enable;
    SIP_STATUS_ENUM Status;
    CHAR            Alias[STR_LEN_64];
    ORIGIN_ENUM     Origin;
    CHAR            IPAddress[STR_LEN_45];
    UINT            Port;
    CHAR            ContactURI[STR_LEN_256];
    CHAR            ExpireTime[STR_LEN_24];
    CHAR            UserAgent[STR_LEN_64];
 } DML_SIP_REGISTRAR_ACCOUNT_CONTACT,*PDML_SIP_REGISTRAR_ACCOUNT_CONTACT;

typedef  struct _DML_SIP_REGISTRAR_ACCOUNT_CONTACT_CTRL_
 {
    DML_SIP_REGISTRAR_ACCOUNT_CONTACT       dml;
    bool                                    updated;
 } DML_SIP_REGISTRAR_ACCOUNT_CONTACT_CTRL_T, *PDML_SIP_REGISTRAR_ACCOUNT_CONTACT_CTRL_T;

typedef  struct _DML_SIP_REGISTRAR_ACCOUNT_CONTACT_LIST_
 {
    UINT                                            ulQuantity;
    DML_SIP_REGISTRAR_ACCOUNT_CONTACT_CTRL_T*       pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_REGISTRAR_ACCOUNT_CONTACT_LIST_T, *PDML_SIP_REGISTRAR_ACCOUNT_CONTACT_LIST_T;

typedef  struct _DML_SIP_REGISTRAR_ACCOUNT
 {
    ULONG                                       uInstanceNumber;
    void*                                       pParentVoiceService;
    void*                                       pParentSipRegistrar;
    SIP_REGISTRAR_ACC_ENABLE_ENUM               Enable;
    BOOL                                        QuiescentMode;
    CLIENT_STATUS_ENUM                          Status;
    CHAR                                        Alias[STR_LEN_64];
    ORIGIN_ENUM                                 Origin;
    CALLSTATUS_ENUM                             CallStatus;
    CHAR                                        AuthUserName[STR_LEN_128];
    CHAR                                        AuthPassword[STR_LEN_128];
    ACC_REGISTRATION_STATUS_ENUM                RegistrationStatus;
    CHAR                                        URI[STR_LEN_389];
    CHAR                                        Domain[STR_LEN_256];
    CHAR                                        CodecList[STR_LEN_256];
    CHAR                                        VoIPProfile[STR_LEN_256];
    DML_SIP_REGISTRAR_ACCOUNT_CONTACT_LIST_T    Contact;
 } DML_SIP_REGISTRAR_ACCOUNT,*PDML_SIP_REGISTRAR_ACCOUNT;

typedef  struct _DML_SIP_REGISTRAR_ACCOUNT_CTRL_
 {
    DML_SIP_REGISTRAR_ACCOUNT       dml;
    bool                            updated;
 } DML_SIP_REGISTRAR_ACCOUNT_CTRL_T, *PDML_SIP_REGISTRAR_ACCOUNT_CTRL_T;

typedef  struct _DML_SIP_REGISTRAR_ACCOUNT_LIST_
 {
    UINT                                    ulQuantity;
    DML_SIP_REGISTRAR_ACCOUNT_CTRL_T*       pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_REGISTRAR_ACCOUNT_LIST_T, *PDML_SIP_REGISTRAR_ACCOUNT_LIST_T;

typedef struct _DML_SIP_REGISTRAR
{
    ULONG                               uInstanceNumber;
    void*                               pParentVoiceService;
    BOOL                                Enable;
    BOOL                                QuiescentMode;
    SIP_STATUS_ENUM                     Status;
    CHAR                                Alias[STR_LEN_64];
    ORIGIN_ENUM                         Origin;
    CHAR                                RegistrarIPAddress[STR_LEN_45];
    UINT                                RegistrarPort;
    UINT                                RegisterExpires;
    UINT                                PingInterval;
    CHAR                                Organization[STR_LEN_256];
    CHAR                                Realm[STR_LEN_256];
    CHAR                                VoIPProfile[STR_LEN_256];
    CHAR                                ContactURI[STR_LEN_256];
    DML_SIP_REGISTRAR_ACCOUNT_LIST_T    Account;
} DML_SIP_REGISTRAR, *PDML_SIP_REGISTRAR;

typedef  struct _DML_SIP_REGISTRAR_CTRL_
 {
    DML_SIP_REGISTRAR       dml;
    bool                    updated;
 } DML_SIP_REGISTRAR_CTRL_T, *PDML_SIP_REGISTRAR_CTRL_T;

typedef  struct _DML_SIP_REGISTRAR_LIST_
 {
    UINT                          ulQuantity;
    DML_SIP_REGISTRAR_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_SIP_REGISTRAR_LIST_T, *PDML_SIP_REGISTRAR_LIST_T;

typedef struct _DML_SIP
{
    DML_SIP_NETWORK_LIST_T      Network;
    DML_SIP_CLIENT_LIST_T      Client;
    DML_SIP_PROXY_LIST_T       Proxy;
    DML_SIP_REGISTRAR_LIST_T   Registrar;
} DML_SIP,*PDML_SIP;

typedef  struct _DML_MGCP_CLIENT
 {
    ULONG                   uInstanceNumber;
    void*                   pParentVoiceService;
    BOOL                    Enable;
    BOOL                    QuiescentMode;
    CLIENT_STATUS_ENUM      Status;
    CHAR                    Alias[STR_LEN_64];
    MGCP_REGISTER_MODE_ENUM RegisterMode;
    UINT                    LocalPort;
    CHAR                    Domain[STR_LEN_256];
    CHAR                    User[STR_LEN_64];
    CHAR                    Network[STR_LEN_256];
    UINT                    MaxSessions;
 } DML_MGCP_CLIENT,*PDML_MGCP_CLIENT;

typedef  struct _DML_MGCP_CLIENT_CTRL_
 {
    DML_MGCP_CLIENT             dml;
    bool                        updated;
 } DML_MGCP_CLIENT_CTRL_T, *PDML_MGCP_CLIENT_CTRL_T;

typedef  struct _DML_MGCP_CLIENT_LIST_
 {
    UINT                       ulQuantity;
    DML_MGCP_CLIENT_CTRL_T*    pdata[TELCOVOICE_DATA_MAX];
 } DML_MGCP_CLIENT_LIST_T, *PDML_MGCP_CLIENT_LIST_T;

typedef  struct _DML_MGCP_NETWORK
 {
    ULONG                           uInstanceNumber;
    void*                           pParentVoiceService;
    BOOL                            Enable;
    BOOL                            QuiescentMode;
    MGCP_NETWORK_REG_STATUS_ENUM    Status;
    CHAR                            Alias[STR_LEN_64];
    CHAR                            CallAgent1[STR_LEN_256];
    UINT                            CallAgentPort1;
    CHAR                            CallAgent2[STR_LEN_256];
    UINT                            CallAgentPort2;
    UINT                            RetranIntervalTimer;
    UINT                            MaxRetranCount;
    UINT                            DSCPMark;
    INT                             VLANIDMark;
    INT                             EthernetPriorityMark;
    BOOL                            AllowPiggybackEvents;
    BOOL                            SendRSIPImmediately;
    BOOL                            STUNEnable;
    CHAR                            STUNServer[STR_LEN_256];
    UINT                            NonVoiceBandwidthReservedUpstream;
    UINT                            NonVoiceBandwidthReservedDownstream;
    UINT                            MaxSessions;
    CHAR                            VoIPProfile[STR_LEN_256];
    CHAR                            CodecList[STR_LEN_256];
 } DML_MGCP_NETWORK,*PDML_MGCP_NETWORK;

typedef  struct _DML_MGCP_NETWORK_CTRL_
 {
    DML_MGCP_NETWORK            dml;
    bool                        updated;
 } DML_MGCP_NETWORK_CTRL_T, *PDML_MGCP_NETWORK_CTRL_T;

typedef  struct _DML_MGCP_NETWORK_LIST_
 {
    UINT                         ulQuantity;
    DML_MGCP_NETWORK_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_MGCP_NETWORK_LIST_T, *PDML_MGCP_NETWORK_LIST_T;

typedef struct _DML_MGCP
{
    DML_MGCP_CLIENT_LIST_T      Client;
    DML_MGCP_NETWORK_LIST_T     Network;
} DML_MGCP,*PDML_MGCP;

typedef  struct _DML_H323_CLIENT
 {
    ULONG               uInstanceNumber;
    void*               pParentVoiceService;
    BOOL                Enable;
    BOOL                QuiescentMode;
    CLIENT_STATUS_ENUM  Status;
    CHAR                Alias[STR_LEN_256];
    BOOL                H235Authentication;
    CHAR                AuthPassword[STR_LEN_128];
    CHAR                SendersID[STR_LEN_256];
    CHAR                Network[STR_LEN_256];
    CHAR                H323ID[STR_LEN_32];
    UINT                MaxSessions;
 } DML_H323_CLIENT,*PDML_H323_CLIENT;

typedef  struct _DML_H323_CLIENT_CTRL_
 {
    DML_H323_CLIENT     dml;
    bool                updated;
 } DML_H323_CLIENT_CTRL_T, *PDML_H323_CLIENT_CTRL_T;

typedef  struct _DML_H323_CLIENT_LIST_
 {
    UINT                        ulQuantity;
    DML_H323_CLIENT_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_H323_CLIENT_LIST_T, *PDML_H323_CLIENT_LIST_T;

typedef  struct _DML_H323_NETWORK
 {
    ULONG                   uInstanceNumber;
    void*                   pParentVoiceService;
    BOOL                    Enable;
    BOOL                    QuiescentMode;
    NETWORK_STATUS_ENUM     Status;
    CHAR                    Alias[STR_LEN_256];
    CHAR                    Gatekeeper[STR_LEN_256];
    UINT                    GatekeeperPort;
    CHAR                    GatekeeperID[STR_LEN_256];
    UINT                    TimeToLive;
    UINT                    DSCPMark;
    INT                     VLANIDMark;
    INT                     EthernetPriorityMark;
    BOOL                    STUNEnable;
    CHAR                    STUNServer[STR_LEN_256];
    UINT                    NonVoiceBandwidthReservedUpstream;
    UINT                    NonVoiceBandwidthReservedDownstream;
    UINT                    MaxSessions;
    CHAR                    VoIPProfile[STR_LEN_256];
    CHAR                    CodecList[STR_LEN_256];
 } DML_H323_NETWORK,*PDML_H323_NETWORK;

typedef  struct _DML_H323_NETWORK_CTRL_
 {
    DML_H323_NETWORK     dml;
    bool                 updated;
 } DML_H323_NETWORK_CTRL_T, *PDML_H323_NETWORK_CTRL_T;

typedef  struct _DML_H323_NETWORK_LIST_
 {
    UINT                         ulQuantity;
    DML_H323_NETWORK_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_H323_NETWORK_LIST_T, *PDML_H323_NETWORK_LIST_T;

typedef struct _DML_H323
{
    DML_H323_CLIENT_LIST_T      Client;
    DML_H323_NETWORK_LIST_T     Network;
} DML_H323,*PDML_H323;

typedef enum _LINE_ORIGIN_ENUM
{
    LINE_ORIGIN_STATIC,
    LINE_ORIGIN_DDI_RANGE
} LINE_ORIGIN_ENUM;

typedef struct _DML_TRUNK
{
   ULONG                   uInstanceNumber;
   void*                   pParentVoiceService;
   BOOL                    Enable;
   BOOL                    QuiescentMode;
   TRUNK_STATUS_ENUM       Status;
   CHAR                    Alias[STR_LEN_64];
   ORIGIN_ENUM             Origin;
   CHAR                    Name[STR_LEN_16];
   CHAR                    DDIRange[STR_LEN_256];
   BOOL                    LineObjectCreation;
   UINT                    MaxChannels;
   INT                     MaxOutboundChannelCount;
   INT                     MaxInboundChannelCount;
   CODECPRIORITY_ENUM      CodecPriority;
   CHAR                    Provider[STR_LEN_256];
} DML_TRUNK,*PDML_TRUNK;


typedef  struct _DML_TRUNK_CTRL_
 {
    DML_TRUNK                   dml;
    bool                        updated;
 } DML_TRUNK_CTRL_T, *PDML_TRUNK_CTRL_T;

typedef  struct _DML_TRUNK_LIST_
 {
    UINT                              ulQuantity;
    DML_TRUNK_CTRL_T*                 pdata[TELCOVOICE_DATA_MAX];
 } DML_TRUNK_LIST_T, *PDML_TRUNK_LIST_T;

typedef enum _GROUP_RINGTYPE_ENUM
{
    GROUP_RINGTYPE_SIMULTANEOUS,
    GROUP_RINGTYPE_HIERARCHICAL,
    GROUP_RINGTYPE_CYCLIC
} GROUP_RINGTYPE_ENUM;

typedef  struct _DML_CALLCONTROL_GROUP
 {
    ULONG                   uInstanceNumber;
    void*                   pParentVoiceService;
    CHAR                    Alias[STR_LEN_64];
    CHAR                    Extensions[STR_LEN_256];
    GROUP_RINGTYPE_ENUM     RingType;
    UINT                    RingTimeout;
 } DML_CALLCONTROL_GROUP,*PDML_CALLCONTROL_GROUP;

typedef  struct _DML_CALLCONTROL_GROUP_CTRL_
 {
    DML_CALLCONTROL_GROUP       dml;
    bool                        updated;
 } DML_CALLCONTROL_GROUP_CTRL_T, *PDML_CALLCONTROL_GROUP_CTRL_T;

typedef  struct _DML_CALLCONTROL_GROUP_LIST_
 {
    UINT                              ulQuantity;
    DML_CALLCONTROL_GROUP_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_GROUP_LIST_T, *PDML_CALLCONTROL_GROUP_LIST_T;

typedef  struct _DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO
 {
    ULONG       uInstanceNumber;
    void*       pParentVoiceService;
    void*       pParentCallCtrlNumPlan;
    CHAR        Alias[STR_LEN_64];
    BOOL        Enable;
    CHAR        PrefixRange[STR_LEN_42];
    UINT        PrefixMinNumberOfDigits;
    UINT        PrefixMaxNumberOfDigits;
    UINT        NumberOfDigitsToRemove;
    UINT        PosOfDigitsToRemove;
    CHAR        DialTone[STR_LEN_256];
    CHAR        FacilityAction[STR_LEN_64];
    CHAR        FacilityActionArgument[STR_LEN_256];
 } DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO,*PDML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO;

typedef  struct _DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_CTRL
 {
    DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO    dml;
    bool                                        updated;
 } DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_CTRL_T, *PDML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_CTRL_T;

typedef  struct _DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_LIST_
 {
    UINT                                                ulQuantity;
    DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_CTRL_T*    pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_LIST_T, *PDML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_LIST_T;

typedef  struct _DML_CALLCONTROL_NUMBERINGPLAN
 {
    ULONG                                               uInstanceNumber;
    void*                                               pParentVoiceService;
    CHAR                                                Alias[STR_LEN_64];
    UINT                                                MinimumNumberOfDigits;
    UINT                                                MaximumNumberOfDigits;
    UINT                                                InterDigitTimerStd;
    UINT                                                InterDigitTimerOpen;
    CHAR                                                TerminationDigit;
    CHAR                                                InvalidNumberTone[STR_LEN_256];
    UINT                                                PrefixInfoMaxEntries;
    DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_LIST_T     PrefixInfo;
 } DML_CALLCONTROL_NUMBERINGPLAN,*PDML_CALLCONTROL_NUMBERINGPLAN;

typedef  struct _DML_CALLCONTROL_NUMBERINGPLAN_CTRL_
 {
    DML_CALLCONTROL_NUMBERINGPLAN       dml;
    bool                                updated;
 } DML_CALLCONTROL_NUMBERINGPLAN_CTRL_T, *PDML_CALLCONTROL_NUMBERINGPLAN_CTRL_T;

typedef  struct _DML_CALLCONTROL_NUMBERINGPLAN_LIST_
 {
    UINT                                    ulQuantity;
    DML_CALLCONTROL_NUMBERINGPLAN_CTRL_T*   pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_NUMBERINGPLAN_LIST_T, *PDML_CALLCONTROL_NUMBERINGPLAN_LIST_T;

typedef  struct _DML_CALLCONTROL_OUTGOINGMAP
 {
    ULONG       uInstanceNumber;
    void*       pParentVoiceService;
    BOOL        Enable;
    CHAR        Alias[STR_LEN_64];
    CHAR        CLIPNoScreeningNumber[STR_LEN_32];
    CHAR        Extension[STR_LEN_256];
    CHAR        Line[STR_LEN_256];
    UINT        Order;
 } DML_CALLCONTROL_OUTGOINGMAP,*PDML_CALLCONTROL_OUTGOINGMAP;

typedef  struct _DML_CALLCONTROL_OUTGOINGMAP_CTRL_
 {
    DML_CALLCONTROL_OUTGOINGMAP     dml;
    bool                            updated;
 } DML_CALLCONTROL_OUTGOINGMAP_CTRL_T, *PDML_CALLCONTROL_OUTGOINGMAP_CTRL_T;

typedef  struct _DML_CALLCONTROL_OUTGOINGMAP_LIST_
 {
    UINT                                    ulQuantity;
    DML_CALLCONTROL_OUTGOINGMAP_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_OUTGOINGMAP_LIST_T, *PDML_CALLCONTROL_OUTGOINGMAP_LIST_T;

typedef  struct _DML_CALLCONTROL_INCOMINGMAP
 {
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    BOOL    Enable;
    CHAR    Alias[STR_LEN_64];
    CHAR    Line[STR_LEN_256];
    CHAR    Extension[STR_LEN_256];
    UINT    Order;
    UINT    Timeout;
 } DML_CALLCONTROL_INCOMINGMAP,*PDML_CALLCONTROL_INCOMINGMAP;

typedef  struct _DML_CALLCONTROL_INCOMINGMAP_CTRL_
 {
    DML_CALLCONTROL_INCOMINGMAP     dml;
    bool                            updated;
 } DML_CALLCONTROL_INCOMINGMAP_CTRL_T, *PDML_CALLCONTROL_INCOMINGMAP_CTRL_T;

typedef  struct _DML_CALLCONTROL_INCOMINGMAP_LIST_
 {
    UINT                                    ulQuantity;
    DML_CALLCONTROL_INCOMINGMAP_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_INCOMINGMAP_LIST_T, *PDML_CALLCONTROL_INCOMINGMAP_LIST_T;

typedef enum _SMTP_AUTH_TYPE_ENUM
 {
    SMTP_AUTH_NONE,
    SMTP_AUTH_SSL,
    SMTP_AUTH_TLS,
    SMTP_AUTH_AUTO
 } SMTP_AUTH_TYPE_ENUM;

typedef  struct _DML_CALLCONTROL_MAILBOX
 {
    ULONG                   uInstanceNumber;
    void*                   pParentVoiceService;
    BOOL                    Enable;
    CHAR                    Alias[STR_LEN_64];
    UINT                    MaxMsg;
    UINT                    MaxMessageTime;
    UINT                    MinSize;
    CHAR                    SMTPServerAddress[STR_LEN_256];
    UINT                    SMTPServerPort;
    CHAR                    SMTPUser[STR_LEN_256];
    CHAR                    SMTPPassword[STR_LEN_256];
    SMTP_AUTH_TYPE_ENUM     SMTPAuthenticationType;
    CHAR                    SMTPFrom[STR_LEN_256];
 } DML_CALLCONTROL_MAILBOX,*PDML_CALLCONTROL_MAILBOX;

typedef  struct _DML_CALLCONTROL_MAILBOX_CTRL_
 {
    DML_CALLCONTROL_MAILBOX     dml;
    bool                        updated;
 } DML_CALLCONTROL_MAILBOX_CTRL_T, *PDML_CALLCONTROL_MAILBOX_CTRL_T;

typedef  struct _DML_CALLCONTROL_MAILBOX_LIST_
 {
    UINT                                ulQuantity;
    DML_CALLCONTROL_MAILBOX_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_MAILBOX_LIST_T, *PDML_CALLCONTROL_MAILBOX_LIST_T;

typedef  struct _DML_CALLCONTROL_STATS_DSP
 {
    UINT    Overruns;
    UINT    Underruns;
 } DML_CALLCONTROL_STATS_DSP,*PDML_CALLCONTROL_STATS_DSP;

typedef  struct _DML_CALLCONTROL_STATS_INCALLS
 {
    UINT    CallsReceived;
    UINT    CallsConnected;
    UINT    CallsFailed;
    UINT    CallsDropped;
    UINT    TotalCallTime;
 } DML_CALLCONTROL_STATS_INCALLS,*PDML_CALLCONTROL_STATS_INCALLS;

typedef  struct _DML_CALLCONTROL_STATS_OUTCALLS
 {
    UINT    CallsAttempted;
    UINT    CallsConnected;
    UINT    CallsFailed;
    UINT    CallsDropped;
    UINT    TotalCallTime;
 }  DML_CALLCONTROL_STATS_OUTCALLS,*PDML_CALLCONTROL_STATS_OUTCALLS;

typedef  struct _DML_CALLCONTROL_STATS_RTP
 {
    UINT    PacketsReceived;
    UINT    PacketsSent;
    UINT    PacketsLost;
    UINT    BytesSent;
    UINT    BytesReceived;
 } DML_CALLCONTROL_STATS_RTP,*PDML_CALLCONTROL_STATS_RTP;

typedef  struct _DML_CALLCONTROL_EXTENSION_STATS
 {
    DML_CALLCONTROL_STATS_INCALLS   IncomingCalls;
    DML_CALLCONTROL_STATS_OUTCALLS  OutgoingCalls;
    DML_CALLCONTROL_STATS_RTP       RTP;
    DML_CALLCONTROL_STATS_DSP       DSP;
 } DML_CALLCONTROL_EXTENSION_STATS,*PDML_CALLCONTROL_EXTENSION_STATS;

typedef enum _CALL_WAITING_STATUS_ENUM
{
    CALL_WAITING_STATUS_DISABLED,
    CALL_WAITING_STATUS_IDLE,
    CALL_WAITING_STATUS_SECONDARYRINGING,
    CALL_WAITING_STATUS_SECONDARYCONNECTING,
    CALL_WAITING_STATUS_SECONDARYCONNECTED
} CALL_WAITING_STATUS_ENUM;

typedef enum _CONF_CALLING_STATUS_ENUM
{
    CONF_CALLING_STATUS_DISABLED,
    CONF_CALLING_STATUS_IDLE,
    CONF_CALLING_STATUS_SECONDARYCALLING,
    CONF_CALLING_STATUS_SECONDARYCONNECTING,
    CONF_CALLING_STATUS_SECONDARYCONNECTED,
    CONF_CALLING_STATUS_INCONFERENCECALL
} CONF_CALLING_STATUS_ENUM;

typedef  struct _DML_CALLCONTROL_EXTENSION
 {
    ULONG                               uInstanceNumber;
    void*                               pParentVoiceService;
    BOOL                                Enable;
    BOOL                                QuiescentMode;
    CALLCTRL_EXT_STATUS_ENUM            Status;
    CHAR                                Alias[STR_LEN_64];
    CALLCTRL_EXT_CALLSTATUS_ENUM        CallStatus;
    ORIGIN_ENUM                         Origin;
    CHAR                                Name[STR_LEN_256];
    CHAR                                ExtensionNumber[STR_LEN_32];
    CHAR                                Provider[STR_LEN_256];
    CHAR                                NumberingPlan[STR_LEN_256];
    CHAR                                CallingFeatures[STR_LEN_256];
    CALL_WAITING_STATUS_ENUM            CallWaitingStatus;
    CONF_CALLING_STATUS_ENUM            ConferenceCallingStatus;
    UINT                                ConferenceCallingSessionCount;
    CHAR                                VoiceMail[STR_LEN_256];
    DML_CALLCONTROL_EXTENSION_STATS     Stats;
 }  DML_CALLCONTROL_EXTENSION,*PDML_CALLCONTROL_EXTENSION;

typedef  struct _DML_CALLCONTROL_EXTENSION_CTRL_
 {
    DML_CALLCONTROL_EXTENSION     dml;
    bool                          updated;
 } DML_CALLCONTROL_EXTENSION_CTRL_T, *PDML_CALLCONTROL_EXTENSION_CTRL_T;

typedef  struct _DML_CALLCONTROL_EXTENSION_LIST_
 {
    UINT                                    ulQuantity;
    DML_CALLCONTROL_EXTENSION_CTRL_T*       pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_EXTENSION_LIST_T, *PDML_CALLCONTROL_EXTENSION_LIST_T;

typedef  struct _DML_CALLCONTROL_LINE_STATS
 {
    DML_CALLCONTROL_STATS_INCALLS   IncomingCalls;
    DML_CALLCONTROL_STATS_OUTCALLS  OutgoingCalls;
    DML_CALLCONTROL_STATS_RTP       RTP;
    DML_CALLCONTROL_STATS_DSP       DSP;
 } DML_CALLCONTROL_LINE_STATS,*PDML_CALLCONTROL_LINE_STATS;

typedef  struct _DML_CALLCONTROL_LINE
 {
    ULONG                                 uInstanceNumber;
    void*                                 pParentVoiceService;
    BOOL                                  Enable;
    BOOL                                  QuiescentMode;
    TELCOVOICEMGR_LINE_STATUS_ENUM        Status;
    CHAR                                  Alias[STR_LEN_64];
    TELCOVOICEMGR_CALL_STATE_ENUM         CallStatus;
    LINE_ORIGIN_ENUM                      Origin;
    CHAR                                  DirectoryNumber[STR_LEN_32];
    CHAR                                  Provider[STR_LEN_256];
    CHAR                                  CallingFeatures[STR_LEN_256];
    DML_CALLCONTROL_LINE_STATS            Stats;
 } DML_CALLCONTROL_LINE,*PDML_CALLCONTROL_LINE;

typedef  struct _DML_CALLCONTROL_LINE_CTRL_
 {
    DML_CALLCONTROL_LINE     dml;
    bool                     updated;
 } DML_CALLCONTROL_LINE_CTRL_T, *PDML_CALLCONTROL_LINE_CTRL_T;

typedef  struct _DML_CALLCONTROL_LINE_LIST_
 {
    UINT                             ulQuantity;
    DML_CALLCONTROL_LINE_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_LINE_LIST_T, *PDML_CALLCONTROL_LINE_LIST_T;

typedef enum _DAY_ENUM
 {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
    ALL
 } DAY_ENUM;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_CFT
 {
    ULONG       uInstanceNumber;
    void*       pParentVoiceService;
    void*       pParentCallCtrlSet;
    BOOL        Enable;
    CHAR        Alias[STR_LEN_64];
    DAY_ENUM    Day;
    CHAR        StartTime[STR_LEN_5];
    CHAR        EndTime[STR_LEN_5];
    CHAR        ForwardedToNumber[STR_LEN_32];
 } DML_CALLCONTROL_CALLINGFEATURES_SET_CFT,*PDML_CALLCONTROL_CALLINGFEATURES_SET_CFT;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_CFT_CTRL
 {
    DML_CALLCONTROL_CALLINGFEATURES_SET_CFT     dml;
    bool                                        updated;
 }  DML_CALLCONTROL_CALLINGFEATURES_SET_CFT_CTRL_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_CFT_CTRL_T;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_CFT_LIST_
 {
    UINT                                                ulQuantity;
    DML_CALLCONTROL_CALLINGFEATURES_SET_CFT_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_CALLINGFEATURES_SET_CFT_LIST_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_CFT_LIST_T;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_SCF
 {
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    void*   pParentCallCtrlSet;
    BOOL    Enable;
    CHAR    Alias[STR_LEN_64];
    CHAR    CallingNumber[STR_LEN_32];
    CHAR    ForwardedToNumber[STR_LEN_32];
 } DML_CALLCONTROL_CALLINGFEATURES_SET_SCF,*PDML_CALLCONTROL_CALLINGFEATURES_SET_SCF;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_SCF_CTRL
 {
    DML_CALLCONTROL_CALLINGFEATURES_SET_SCF     dml;
    bool                                        updated;
 } DML_CALLCONTROL_CALLINGFEATURES_SET_SCF_CTRL_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_SCF_CTRL_T;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_SCF_LIST_
 {
    UINT                                                ulQuantity;
    DML_CALLCONTROL_CALLINGFEATURES_SET_SCF_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_CALLINGFEATURES_SET_SCF_LIST_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_SCF_LIST_T;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ
 {
    ULONG       uInstanceNumber;
    void*       pParentVoiceService;
    void*       pParentCallCtrlSet;
    BOOL        Enable;
    CHAR        Alias[STR_LEN_64];
    CHAR        CallingNumber[STR_LEN_32];
 } DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ,*PDML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_CTRL
 {
    DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ       dml;
    bool                                            updated;
 } DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_CTRL_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_CTRL_T;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_LIST_
 {
    UINT                                                    ulQuantity;
    DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_CTRL_T*       pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_LIST_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_LIST_T;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME
 {
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    void*   pParentCallCtrlSet;
    BOOL    Enable;
    CHAR    Alias[STR_LEN_64];
    UINT    Delay;
    CHAR    Number[STR_LEN_32];
    UINT    Order;
 } DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME,*PDML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_CTRL
 {
    DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME    dml;
    bool                                            updated;
 } DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_CTRL_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_CTRL_T;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_LIST_
 {
     UINT                                                     ulQuantity;
     DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_LIST_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_LIST_T;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_VOICE2MAIL
 {
    BOOL        Enable;
    UINT        MaxMessageLength;
    BOOL        Attach;
    BOOL        KeepLocal;
    CHAR        EMailAddress[STR_LEN_256];
 } DML_CALLCONTROL_CALLINGFEATURES_SET_VOICE2MAIL, *PDML_CALLCONTROL_CALLINGFEATURES_SET_VOICE2MAIL;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET
 {
    ULONG                                                   uInstanceNumber;
    void*                                                   pParentVoiceService;
    CHAR                                                    Alias[STR_LEN_64];
    BOOL                                                    CallerIDEnable;
    BOOL                                                    CallerIDNameEnable;
    BOOL                                                    CallWaitingEnable;
    BOOL                                                    CallForwardUnconditionalEnable;
    CHAR                                                    CallForwardUnconditionalNumber[STR_LEN_32];
    BOOL                                                    CallForwardOnBusyEnable;
    CHAR                                                    CallForwardOnBusyNumber[STR_LEN_32];
    UINT                                                    CallForwardOnBusyRingTimeout;
    BOOL                                                    CallForwardOnNoAnswerEnable;
    CHAR                                                    CallForwardOnNoAnswerNumber[STR_LEN_32];
    UINT                                                    CallForwardOnNoAnswerRingTimeout;
    BOOL                                                    CallTransferEnable;
    BOOL                                                    MWIEnable;
    BOOL                                                    VMWIEnable;
    UINT                                                    LineMessagesWaiting;
    BOOL                                                    AnonymousCallRejectionEnable;
    BOOL                                                    AnonymousCallEnable;
    BOOL                                                    DoNotDisturbEnable;
    BOOL                                                    RepeatDialEnable;
    BOOL                                                    VoiceMailEnable;
    BOOL                                                    CallPickUpEnable;
    BOOL                                                    CCBSEnable;
    BOOL                                                    IIFCEnable;
    BOOL                                                    BlockForwardEnable;
    BOOL                                                    SecretForwarderEnable;
    BOOL                                                    FollowMeEnable;
    BOOL                                                    X_RDK_Central_COM_ConferenceCallingEnable;
    BOOL                                                    X_RDK_Central_COM_HoldEnable;
    BOOL                                                    X_RDK_Central_COM_PhoneCallerIDEnable;
    DML_CALLCONTROL_CALLINGFEATURES_SET_CFT_LIST_T          CFT;
    DML_CALLCONTROL_CALLINGFEATURES_SET_SCF_LIST_T          SCF;
    DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_LIST_T        SCREJ;
    DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_LIST_T     FollowMe;
    DML_CALLCONTROL_CALLINGFEATURES_SET_VOICE2MAIL          Voice2Mail;
 } DML_CALLCONTROL_CALLINGFEATURES_SET,*PDML_CALLCONTROL_CALLINGFEATURES_SET;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_
 {
    DML_CALLCONTROL_CALLINGFEATURES_SET     dml;
    bool                                    updated;
 } DML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_T;

typedef  struct _DML_CALLCONTROL_CALLINGFEATURES_SET_LIST_
 {
    UINT                                            ulQuantity;
    DML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLCONTROL_CALLINGFEATURES_SET_LIST_T, *PDML_CALLCONTROL_CALLINGFEATURES_SET_LIST_T;

typedef struct _DML_CALLCONTROL_CALLINGFEATURES
{
    DML_CALLCONTROL_CALLINGFEATURES_SET_LIST_T  Set;
} DML_CALLCONTROL_CALLINGFEATURES,*PDML_CALLCONTROL_CALLINGFEATURES;

typedef struct _DML_CALLCONTROL
{
    UINT                                    MaxNumberOfLines;
    UINT                                    MaxNumberOfExtensions;
    DML_CALLCONTROL_LINE_LIST_T             Line;
    DML_CALLCONTROL_EXTENSION_LIST_T        Extension;
    DML_CALLCONTROL_MAILBOX_LIST_T          Mailbox;
    DML_CALLCONTROL_INCOMINGMAP_LIST_T      IncommingMap;
    DML_CALLCONTROL_OUTGOINGMAP_LIST_T      OutgoingMap;
    DML_CALLCONTROL_NUMBERINGPLAN_LIST_T    NumberingPlan;
    DML_CALLCONTROL_GROUP_LIST_T            Group;
    DML_CALLCONTROL_CALLINGFEATURES         CallingFeatures;
} DML_CALLCONTROL,*PDML_CALLCONTROL;

typedef  struct _DML_INTERWORK_USERINTERFACE
 {
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    void*   pParentInterwork;
    BOOL    Enable;
    CHAR    Alias[STR_LEN_64];
    CHAR    Registrar[STR_LEN_256];
    CHAR    Network[STR_LEN_256];
 } DML_INTERWORK_USERINTERFACE,*PDML_INTERWORK_USERINTERFACE;

typedef  struct _DML_INTERWORK_USERINTERFACE_CTRL
 {
    DML_INTERWORK_USERINTERFACE     dml;
    bool                            updated;
 } DML_INTERWORK_USERINTERFACE_CTRL_T, *PDML_INTERWORK_USERINTERFACE_CTRL_T;

typedef  struct _DML_INTERWORK_USERINTERFACE_LIST_
 {
    UINT                                    ulQuantity;
    DML_INTERWORK_USERINTERFACE_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_INTERWORK_USERINTERFACE_LIST_T, *PDML_INTERWORK_USERINTERFACE_LIST_T;

typedef enum _MAP_STATUS_ENUM
{
    MAP_STATUS_ACTIVE,
    MAP_STATUS_INACTIVE
} MAP_STATUS_ENUM;

typedef  struct _DML_INTERWORK_MAP
 {
    ULONG               uInstanceNumber;
    void*               pParentVoiceService;
    void*               pParentInterwork;
    BOOL                Enable;
    MAP_STATUS_ENUM     Status;
    CHAR                Alias[STR_LEN_64];
    CHAR                StatusDescription[STR_LEN_256];
    CHAR                LastTime[STR_LEN_24];
    ORIGIN_ENUM         Origin;
    CHAR                NetworkConnection[STR_LEN_256];
    CHAR                UserConnection[STR_LEN_256];
    CHAR                DigitMap[STR_LEN_256];
    BOOL                DigitMapEnable;
    UINT                Priority;
 } DML_INTERWORK_MAP,*PDML_INTERWORK_MAP;

typedef  struct _DML_INTERWORK_MAP_CTRL
 {
    DML_INTERWORK_MAP     dml;
    bool                  updated;
 } DML_INTERWORK_MAP_CTRL_T, *PDML_INTERWORK_MAP_CTRL_T;

typedef  struct _DML_INTERWORK_MAP_LIST_
 {
    UINT                          ulQuantity;
    DML_INTERWORK_MAP_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_INTERWORK_MAP_LIST_T, *PDML_INTERWORK_MAP_LIST_T;

typedef enum _OPER_STATUS_ENUM
 {
    OPER_STATUS_INSERVICE,
    OPER_STATUS_OUTOFSERVICE,
    OPER_STATUS_NETWORK_SERVERS_OFFLINE
 } OPER_STATUS_ENUM;

typedef enum _NETWORK_CONNECTION_MODE_ENUM
 {
    NETWORK_CONN_MODE_STATIC,
    NETWORK_CONN_MODE_REGISTERDYNAMIC,
    NETWORK_CONN_MODE_REGISTERLEARN,
    NETWORK_CONN_MODE_REGISTERSTATIC
 } NETWORK_CONNECTION_MODE_ENUM;

typedef enum _USER_CONNECTION_MODE_ENUM
 {
    USER_CONN_MODE_STATIC,
    USER_CONN_MODE_REGISTERDYNAMIC,
    USER_CONN_MODE_REGISTERLEARN
 } USER_CONNECTION_MODE_ENUM;

typedef enum _CHALLENGE_MODE_ENUM
 {
    CHALLENGE_MODE_PASSTHRU,
    CHALLENGE_MODE_RESPONDLOCAL,
    CHALLENGE_MODE_HOP_BY_HOP
 } CHALLENGE_MODE_ENUM;

typedef struct _DML_INTERWORK
{
    ULONG                                   uInstanceNumber;
    void*                                   pParentVoiceService;
    BOOL                                    Enable;
    BOOL                                    QuiescentMode;
    INTERWORK_STATUS_ENUM                   Status;
    CHAR                                    Alias[STR_LEN_64];
    OPER_STATUS_ENUM                        OperationalStatus;
    CHAR                                    OperationalStatusReason[STR_LEN_256];
    NETWORK_CONNECTION_MODE_ENUM            NetworkConnectionMode;
    USER_CONNECTION_MODE_ENUM               UserConnectionMode;
    BOOL                                    E164Mode;
    CHALLENGE_MODE_ENUM                     NetworkAuthChallengeMode;
    CHAR                                    NetworkIPAddress[STR_LEN_45];
    CHAR                                    InterworkingRuleSetURI[STR_LEN_256];
    UINT                                    InterworkingRuleSetInterval;
    CHAR                                    InterworkingRuleSetTime[STR_LEN_24];
    CHAR                                    FirewallRuleSetURI[STR_LEN_256];
    UINT                                    FirewallRuleSetInterval;
    CHAR                                    FirewallRuleSetTime[STR_LEN_24];
    CHAR                                    InterworkName[STR_LEN_256];
    CHAR                                    ProxyServer[STR_LEN_256];
    CHAR                                    Networks[STR_LEN_256];
    CHAR                                    E164Client[STR_LEN_256];
    DML_INTERWORK_USERINTERFACE_LIST_T      UserInterface;
    DML_INTERWORK_MAP_LIST_T                Map;
} DML_INTERWORK,*PDML_INTERWORK;

typedef  struct _DML_INTERWORK_CTRL
 {
    DML_INTERWORK            dml;
    bool                     updated;
 } DML_INTERWORK_CTRL_T, *PDML_INTERWORK_CTRL_T;

typedef  struct _DML_INTERWORK_LIST_
 {
    UINT                             ulQuantity;
    DML_INTERWORK_CTRL_T*            pdata[TELCOVOICE_DATA_MAX];
 } DML_INTERWORK_LIST_T, *PDML_INTERWORK_LIST_T;

typedef enum _PROTOCOL_ENUM
{
    H323_PROTOCOL,
    SIP_PROTOCOL
}PROTOCOL_ENUM;

typedef  struct _DML_CALLLOG_SIGNALINGPERF
 {
    ULONG           uInstanceNumber;
    PROTOCOL_ENUM   Protocol;
    UINT            CallSetupDelay;
    INT             OutgoingMediaEstablishDelay;
    INT             IncomingMediaEstablishDelay;
 } DML_CALLLOG_SIGNALINGPERF,*PDML_CALLLOG_SIGNALINGPERF;

typedef  struct _DML_CALLLOG_SIGNALINGPERF_CTRL
 {
    DML_CALLLOG_SIGNALINGPERF       dml;
    bool                            updated;
 }  DML_CALLLOG_SIGNALINGPERF_CTRL_T, *PDML_CALLLOG_SIGNALINGPERF_CTRL_T;

typedef  struct _DML_CALLLOG_SIGNALINGPERF_LIST_
 {
    UINT                                    ulQuantity;
    DML_CALLLOG_SIGNALINGPERF_CTRL_T*       pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLLOG_SIGNALINGPERF_LIST_T, *PDML_CALLLOG_SIGNALINGPERF_LIST_T;

typedef struct _DML_CALLLOG_SESSION_RTP
{
    CHAR    FarEndIPAddress[STR_LEN_45];
    UINT    FarEndUDPPort;
    UINT    LocalUDPPort;
    INT     MinJitter;
    INT     MaxJitter;
    INT     MeanJitter;
    INT     PacketDelayVariation;
    INT     BufferDelay;
    UINT    BurstCount;
    UINT    PacketsReceived;
    UINT    PacketsSent;
    UINT    PacketsLost;
    UINT    PacketsDiscarded;
    UINT    BytesReceived;
    UINT    BytesSent;
    UINT    ReceivePacketLossRate;
    UINT    FarEndPacketLossRate;
    INT     ReceiveInterarrivalJitter;
    INT     FarEndInterarrivalJitter;
    INT     AverageReceiveInterarrivalJitter;
    INT     AverageFarEndInterarrivalJitter;
    INT     RoundTripDelay;
    INT     AverageRoundTripDelay;
    UINT    SamplingFrequency;
} DML_CALLLOG_SESSION_RTP,*PDML_CALLLOG_SESSION_RTP;

typedef struct _DML_CALLLOG_SESSION_DSP_RXCODEC
{
    CHAR        Codec[STR_LEN_256];
    BOOL        SilenceSuppression;
    UINT        Overruns;
    UINT        Underruns;
} DML_CALLLOG_SESSION_DSP_RXCODEC,*PDML_CALLLOG_SESSION_DSP_RXCODEC;

typedef struct _DML_CALLLOG_SESSION_DSP_TXCODEC
{
    CHAR        Codec[STR_LEN_256];
    BOOL        SilenceSuppression;
    UINT        PacketizationPeriod;
    UINT        Overruns;
    UINT        Underruns;
} DML_CALLLOG_SESSION_DSP_TXCODEC,*PDML_CALLLOG_SESSION_DSP_TXCODEC;

typedef struct _DML_CALLLOG_SESSION_DSP
{
    DML_CALLLOG_SESSION_DSP_RXCODEC     ReceiveCodec;
    DML_CALLLOG_SESSION_DSP_TXCODEC     TransmitCodec;
} DML_CALLLOG_SESSION_DSP,*PDML_CALLLOG_SESSION_DSP;

typedef struct _DML_CALLLOG_SESSION_VOICEQUALITY
{
    CHAR        VoIPQualityIndicator[STR_LEN_256];
    CHAR        WorstVoIPQualityIndicatorsValues[STR_LEN_256];
    CHAR        WorstVoIPQualityIndicatorTimestamps[STR_LEN_256];
} DML_CALLLOG_SESSION_VOICEQUALITY,*PDML_CALLLOG_SESSION_VOICEQUALITY;

typedef struct _DML_CALLLOG_SESSION_SOURCE
{
    DML_CALLLOG_SESSION_RTP             RTP;
    DML_CALLLOG_SESSION_DSP             DSP;
    DML_CALLLOG_SESSION_VOICEQUALITY    VoiceQuality;
} DML_CALLLOG_SESSION_SOURCE,*PDML_CALLLOG_SESSION_SOURCE;

typedef struct _DML_CALLLOG_SESSION_DESTINATION
{
    DML_CALLLOG_SESSION_RTP             RTP;
    DML_CALLLOG_SESSION_DSP             DSP;
    DML_CALLLOG_SESSION_VOICEQUALITY    VoiceQuality;
} DML_CALLLOG_SESSION_DESTINATION,*PDML_CALLLOG_SESSION_DESTINATION;

typedef  struct _DML_CALLLOG_SESSION
 {
    ULONG                           uInstanceNumber;
    STREAM_TYPE_ENUM                StreamType;
    CHAR                            Start[STR_LEN_24];
    UINT                            Duration;
    CHAR                            SessionID[STR_LEN_16];
    DML_CALLLOG_SESSION_SOURCE      Source;
    DML_CALLLOG_SESSION_DESTINATION Destination;
 } DML_CALLLOG_SESSION,*PDML_CALLLOG_SESSION;

typedef  struct _DML_CALLLOG_SESSION_CTRL
 {
    DML_CALLLOG_SESSION      dml;
    bool                     updated;
 } DML_CALLLOG_SESSION_CTRL_T, *PDML_CALLLOG_SESSION_CTRL_T;

typedef  struct _DML_CALLLOG_SESSION_LIST_
 {
    UINT                             ulQuantity;
    DML_CALLLOG_SESSION_CTRL_T*      pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLLOG_SESSION_LIST_T, *PDML_CALLLOG_SESSION_LIST_T;

typedef enum _DIRECTION_ENUM
{
    DIRECTION_INCOMING,
    DIRECTION_OUTGOING
} DIRECTION_ENUM;

typedef enum _CALL_TERMINATION_CAUSE_ENUM
{
    CALL_TERMINAL_CAUSE_NOEXTENSIONSMAPPED,
    CALL_TERMINAL_CAUSE_NOEXTENSIONSAVAILABLE,
    CALL_TERMINAL_CAUSE_ANONYMOUSCALLREJECTION,
    CALL_TERMINAL_CAUSE_CALLWAITINGREJECTED,
    CALL_TERMINAL_CAUSE_CALLFORWARDINGUNCONDITIONAL,
    CALL_TERMINAL_CAUSE_CALLFORWARDINGBUSY,
    CALL_TERMINAL_CAUSE_CALLFORWARDINGNOREPLY,
    CALL_TERMINAL_CAUSE_LOCALDISCONNECT,
    CALL_TERMINAL_CAUSE_LOCALFORBIDDEN,
    CALL_TERMINAL_CAUSE_LOCALTIMEOUT,
    CALL_TERMINAL_CAUSE_LOCALMEDIAERROR,
    CALL_TERMINAL_CAUSE_LOCALPRIORITY,
    CALL_TERMINAL_CAUSE_LOCALREJECT,
    CALL_TERMINAL_CAUSE_LOCALTRANSFER,
    CALL_TERMINAL_CAUSE_LOCALINTERNALERROR,
    CALL_TERMINAL_CAUSE_REMOTEDISCONNECT,
    CALL_TERMINAL_CAUSE_REMOTEBADREQUEST,
    CALL_TERMINAL_CAUSE_REMOTEFORBIDDEN,
    CALL_TERMINAL_CAUSE_REMOTENOTFOUND,
    CALL_TERMINAL_CAUSE_REMOTEREJECT,
    CALL_TERMINAL_CAUSE_REMOTENOTALLOWED,
    CALL_TERMINAL_CAUSE_REMOTENOTACCEPTABLE,
    CALL_TERMINAL_CAUSE_REMOTETIMEOUT,
    CALL_TERMINAL_CAUSE_REMOTEUNAVAILABLE,
    CALL_TERMINAL_CAUSE_REMOTEBUSY,
    CALL_TERMINAL_CAUSE_REMOTENOTSUPPORTED,
    CALL_TERMINAL_CAUSE_REMOTENETWORKFAILURE
} CALL_TERMINATION_CAUSE_ENUM;

typedef  struct _DML_CALLLOG
{
    ULONG                              uInstanceNumber;
    void*                              pParentVoiceService;
    CHAR                               Alias[STR_LEN_64];
    CHAR                               CallingPartyNumber[STR_LEN_256];
    CHAR                               CalledPartyNumber[STR_LEN_256];
    CHAR                               Source[STR_LEN_256];
    CHAR                               Destination[STR_LEN_256];
    CHAR                               RemoteParty[STR_LEN_32];
    CHAR                               UsedLine[STR_LEN_256];
    CHAR                               UsedExtensions[STR_LEN_256];
    DIRECTION_ENUM                     Direction;
    CHAR                               Start[STR_LEN_24];
    UINT                               Duration;
    CALL_TERMINATION_CAUSE_ENUM        CallTerminationCause;
    DML_CALLLOG_SIGNALINGPERF_LIST_T   SignalingPerformance;
    DML_CALLLOG_SESSION_LIST_T         Session;
} DML_CALLLOG,*PDML_CALLLOG;

typedef  struct  _DML_CALLLOG_CTRL
 {
    DML_CALLLOG     dml;
    bool            updated;
 }  DML_CALLLOG_CTRL_T, *PDML_CALLLOG_CTRL_T;

typedef  struct _DML_CALLLOG_LIST_
 {
    UINT                    ulQuantity;
    DML_CALLLOG_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_CALLLOG_LIST_T, *PDML_CALLLOG_LIST_T;

typedef struct _DML_VOIPPROFILE_RTP_RTCP
{
   BOOL       Enable;
   UINT       TxRepeatInterval;
   CHAR       LocalCName[STR_LEN_64];
}DML_VOIPPROFILE_RTP_RTCP,*PDML_VOIPPROFILE_RTP_RTCP;

typedef enum _KEYING_METHODS_ENUM
{
    KEYING_METHOD_NULL,
    KEYING_METHOD_STATIC,
    KEYING_METHOD_SDP,
    KEYING_METHOD_IKE
} KEYING_METHODS_ENUM;

typedef struct _DML_VOIPPROFILE_RTP_SRTP
{
    BOOL                    Enable;
    KEYING_METHODS_ENUM     KeyingMethods;
    CHAR                    EncryptionKeySizes[STR_LEN_256];
} DML_VOIPPROFILE_RTP_SRTP,*PDML_VOIPPROFILE_RTP_SRTP;

typedef struct _DML_VOIPPROFILE_RTP_REDUNDANCY
{
    BOOL        Enable;
    UINT        PayloadType;
    UINT        BlockPayloadType;
    INT         FaxAndModemRedundancy;
    INT         ModemRedundancy;
    INT         DTMFRedundancy;
    INT         VoiceRedundancy;
    UINT        MaxSessionsUsingRedundancy;
} DML_VOIPPROFILE_RTP_REDUNDANCY,*PDML_VOIPPROFILE_RTP_REDUNDANCY;

typedef enum _JITTER_TYPE_ENUM
{
    JITTER_STATIC,
    JITTER_DYNAMIC
} JITTER_TYPE_ENUM;

typedef struct _DML_VOIPPROFILE_RTP
{
    UINT                            LocalPortMin;
    UINT                            LocalPortMax;
    UINT                            Gmin;
    UINT                            DSCPMark;
    INT                             VLANIDMark;
    INT                             EthernetPriorityMark;
    UINT                            TelephoneEventPayloadType;
    JITTER_TYPE_ENUM                JitterBufferType;
    UINT                            JitterBufferMaxSize;
    UINT                            JitterBufferMinSize;
    UINT                            JitterBufferActualSize;
    UINT                            X_RDK_SKBMark;
    CHAR                            X_RDK_Firewall_Rule_Data[STR_LEN_256];
    DML_VOIPPROFILE_RTP_RTCP        RTCP;
    DML_VOIPPROFILE_RTP_SRTP        SRTP;
    DML_VOIPPROFILE_RTP_REDUNDANCY  Redundancy;
} DML_VOIPPROFILE_RTP,*PDML_VOIPPROFILE_RTP;

typedef enum _TCF_METHOD
{
    TCF_LOCAL,
    TCF_NETWORK
} TCF_METHOD_ENUM;

typedef struct _DML_VOIPPROFILE_FAXT38
{
    UINT                MaxBitRate;
    UINT                HighSpeedRedundancy;
    UINT                LowSpeedRedundancy;
    TCF_METHOD_ENUM     TCFMethod;
}DML_VOIPPROFILE_FAXT38, *PDML_VOIPPROFILE_FAXT38;

typedef enum _DTMF_METHOD
{
    DTMF_INBAND,
    DTMF_RFC4733,
    DTMF_SIPINFO
}DTMF_METHOD_ENUM;

typedef enum _DTMF_METHOD_G711
{
    DTMF_G711_INBAND,
    DTMF_G711_RFC4733,
    DTMF_G711_SIPINFO,
    DTMF_G711_EMPTY
}DTMF_METHOD_G711_ENUM;

typedef  struct  _DML_VOIPPROFILE
 {
    ULONG                   uInstanceNumber;
    void*                   pParentVoiceService;
    BOOL                    Enable;
    BOOL                    QuiescentMode;
    CHAR                    Name[STR_LEN_64];
    CHAR                    Alias[STR_LEN_64];
    DTMF_METHOD_ENUM        DTMFMethod;
    DTMF_METHOD_G711_ENUM   DTMFMethodG711;
    CHAR                    QIModelUsed[STR_LEN_128];
    UINT                    QICalculationInterval;
    UINT                    NumberOfWorstQIValues;
    CHAR                    X_RDK_Central_COM_DigitMap[STR_LEN_256];
    CHAR                    X_RDK_Central_COM_EmergencyDigitMap[STR_LEN_256];
    UINT                    X_RDK_Central_COM_SDigitTimer;
    UINT                    X_RDK_Central_COM_ZDigitTimer;
    DML_VOIPPROFILE_RTP     RTP;
    DML_VOIPPROFILE_FAXT38  FaxT38;
 } DML_VOIPPROFILE,*PDML_VOIPPROFILE;

typedef  struct  _DML_VOIPPROFILE_CTRL
 {
    DML_VOIPPROFILE     dml;
    bool                updated;
 } DML_VOIPPROFILE_CTRL_T, *PDML_VOIPPROFILE_CTRL_T;

typedef  struct  _DML_VOIPPROFILE_LIST_
 {
    UINT                        ulQuantity;
    DML_VOIPPROFILE_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_VOIPPROFILE_LIST_T, *PDML_VOIPPROFILE_LIST_T;

typedef  struct  _DML_CODECPROFILE
 {
    ULONG           uInstanceNumber;
    void*           pParentVoiceService;
    BOOL            Enable;
    CHAR            Alias[STR_LEN_64];
    CHAR            Codec[STR_LEN_256];
    CHAR            PacketizationPeriod[STR_LEN_64];
    BOOL            SilenceSuppression;
 } DML_CODECPROFILE,*PDML_CODECPROFILE;

typedef  struct  _DML_CODECPROFILE_CTRL
 {
    DML_CODECPROFILE            dml;
    bool                        updated;
 } DML_CODECPROFILE_CTRL_T, *PDML_CODECPROFILE_CTRL_T;

typedef  struct _DML_CODECPROFILE_LIST_
 {
    UINT                             ulQuantity;
    DML_CODECPROFILE_CTRL_T*         pdata[TELCOVOICE_DATA_MAX];
 } DML_CODECPROFILE_LIST_T, *PDML_CODECPROFILE_LIST_T;

typedef  struct  _DML_TONE_DESCRIPTION
 {
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    BOOL    ToneEnable;
    CHAR    Alias[STR_LEN_64];
    CHAR    ToneName[STR_LEN_64];
    CHAR    TonePattern[STR_LEN_256];
    CHAR    ToneFile[STR_LEN_256];
    UINT    ToneRepetitions;
    CHAR    ToneText[STR_LEN_64];
 } DML_TONE_DESCRIPTION,*PDML_TONE_DESCRIPTION;

typedef  struct  _DML_TONE_DESCRIPTION_CTRL
 {
    DML_TONE_DESCRIPTION     dml;
    bool                     updated;
 } DML_TONE_DESCRIPTION_CTRL_T, *PDML_TONE_DESCRIPTION_CTRL_T;

typedef  struct _DML_TONE_DESCRIPTION_LIST_
 {
    UINT                             ulQuantity;
    DML_TONE_DESCRIPTION_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_TONE_DESCRIPTION_LIST_T, *PDML_TONE_DESCRIPTION_LIST_T;

typedef  struct _DML_TONE_PATTERN
 {
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    BOOL    Enable;
    CHAR    Alias[STR_LEN_64];
    BOOL    ToneOn;
    UINT    Frequency1;
    INT     Power1;
    UINT    Frequency2;
    INT     Power2;
    UINT    Frequency3;
    INT     Power3;
    UINT    Frequency4;
    INT     Power4;
    UINT    ModulationFrequency;
    INT     ModulationPower;
    UINT    Duration;
    CHAR    NextPattern[STR_LEN_256];
 } DML_TONE_PATTERN,*PDML_TONE_PATTERN;

typedef  struct _DML_TONE_PATTERN_CTRL
 {
    DML_TONE_PATTERN     dml;
    bool                 updated;
 } DML_TONE_PATTERN_CTRL_T, *PDML_TONE_PATTERN_CTRL_T;

typedef  struct _DML_TONE_PATTERN_LIST_
 {
    UINT                             ulQuantity;
    DML_TONE_PATTERN_CTRL_T*         pdata[TELCOVOICE_DATA_MAX];
 } DML_TONE_PATTERN_LIST_T, *PDML_TONE_PATTERN_LIST_T;

typedef enum _FUNCTION_ENUM
{
    FUNCTION_BUSY,
    FUNCTION_CONFIRMATION,
    FUNCTION_DIAL,
    FUNCTION_LINEMESSAGESWAITING,
    FUNCTION_OFFHOOKWARNING,
    FUNCTION_RINGBACK,
    FUNCTION_REORDER,
    FUNCTION_STUTTERDIAL,
    FUNCTION_CALLWAITING1,
    FUNCTION_CALLWAITING2,
    FUNCTION_CALLWAITING3,
    FUNCTION_CALLWAITING4,
    FUNCTION_ALERTINGSIGNAL,
    FUNCTION_SPECIALDIAL,
    FUNCTION_SPECIALINFO,
    FUNCTION_RELEASE,
    FUNCTION_CONGESTION,
    FUNCTION_USERDEFINED1,
    FUNCTION_USERDEFINED2,
    FUNCTION_USERDEFINED3,
    FUNCTION_USERDEFINED4
} FUNCTION_ENUM;

typedef  struct _DML_TONE_EVENTPROFILE_EVENT
 {
    ULONG           uInstanceNumber;
    void*           pParentVoiceService;
    void*           pParentToneEvtProfile;
    CHAR            Alias[STR_LEN_64];
    FUNCTION_ENUM   Function;
    CHAR            Tone[STR_LEN_256];
 } DML_TONE_EVENTPROFILE_EVENT,*PDML_TONE_EVENTPROFILE_EVENT;

typedef  struct _DML_TONE_EVENTPROFILE_EVENT_CTRL
 {
     DML_TONE_EVENTPROFILE_EVENT     dml;
     bool                            updated;
 } DML_TONE_EVENTPROFILE_EVENT_CTRL_T, *PDML_TONE_EVENTPROFILE_EVENT_CTRL_T;

typedef  struct _DML_TONE_EVENTPROFILE_EVENT_LIST_
 {
    UINT                                    ulQuantity;
    DML_TONE_EVENTPROFILE_EVENT_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_TONE_EVENTPROFILE_EVENT_LIST_T, *PDML_TONE_EVENTPROFILE_EVENT_LIST_T;

typedef  struct _DML_TONE_EVENTPROFILE
 {
    ULONG                               uInstanceNumber;
    void*                               pParentVoiceService;
    CHAR                                Alias[STR_LEN_64];
    DML_TONE_EVENTPROFILE_EVENT_LIST_T  Event;
 } DML_TONE_EVENTPROFILE,*PDML_TONE_EVENTPROFILE;

typedef  struct _DML_TONE_EVENTPROFILE_CTRL
 {
    DML_TONE_EVENTPROFILE     dml;
    bool                      updated;
 } DML_TONE_EVENTPROFILE_CTRL_T, *PDML_TONE_EVENTPROFILE_CTRL_T;

typedef  struct _DML_TONE_EVENTPROFILE_LIST_
 {
    UINT                              ulQuantity;
    DML_TONE_EVENTPROFILE_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_TONE_EVENTPROFILE_LIST_T, *PDML_TONE_EVENTPROFILE_LIST_T;

typedef struct _DML_TONE
{
    CHAR                            DefautEventProfile[STR_LEN_256];
    DML_TONE_DESCRIPTION_LIST_T     Description;
    DML_TONE_PATTERN_LIST_T         Pattern;
    DML_TONE_EVENTPROFILE_LIST_T    EventProfile;
} DML_TONE, *PDML_TONE;

typedef struct _DML_TERMINAL_AUDIO_VOICEPROCESSING
{
    INT         TransmitGain;
    INT         ReceiveGain;
    BOOL        EchoCancellationEnable;
    BOOL        EchoCancellationInUse;
    UINT        EchoCancellationTail;
} DML_TERMINAL_AUDIO_VOICEPROCESSING, *PDML_TERMINAL_AUDIO_VOICEPROCESSING;

typedef  struct _DML_TERMINAL_AUDIO
 {
    ULONG                               uInstanceNumber;
    void*                               pParentVoiceService;
    void*                               pParentTerminal;
    CHAR                                Alias[STR_LEN_64];
    CHAR                                Name[STR_LEN_256];
    DML_TERMINAL_AUDIO_VOICEPROCESSING  VoiceProcessing;
 } DML_TERMINAL_AUDIO,*PDML_TERMINAL_AUDIO;

typedef  struct _DML_TERMINAL_AUDIO_CTRL
 {
    DML_TERMINAL_AUDIO     dml;
    bool                   updated;
 } DML_TERMINAL_AUDIO_CTRL_T, *PDML_TERMINAL_AUDIO_CTRL_T;

typedef  struct _DML_TERMINAL_AUDIO_LIST_
 {
    UINT                             ulQuantity;
    DML_TERMINAL_AUDIO_CTRL_T*       pdata[TELCOVOICE_DATA_MAX];
 } DML_TERMINAL_AUDIO_LIST_T, *PDML_TERMINAL_AUDIO_LIST_T;

typedef  struct _DML_TERMINAL_BUTTONMAP_BUTTON
 {
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    void*   pParentTerminal;
    CHAR    Alias[STR_LEN_64];
    CHAR    ButtonName[STR_LEN_16];
    CHAR    FacilityAction[STR_LEN_64];
    CHAR    FacilityActionArgument[STR_LEN_256];
    CHAR    QuickDialNumber[STR_LEN_40];
    CHAR    ButtonMessage[STR_LEN_64];
    BOOL    UserAccess;
 } DML_TERMINAL_BUTTONMAP_BUTTON,*PDML_TERMINAL_BUTTONMAP_BUTTON;

typedef  struct _DML_TERMINAL_BUTTONMAP_BUTTON_CTRL
 {
    DML_TERMINAL_BUTTONMAP_BUTTON       dml;
    bool                                updated;
 } DML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T, *PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T;

typedef  struct _DML_TERMINAL_BUTTONMAP_BUTTON_LIST_
 {
    UINT                                       ulQuantity;
    DML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_TERMINAL_BUTTONMAP_BUTTON_LIST_T, *PDML_TERMINAL_BUTTONMAP_BUTTON_LIST_T;

typedef struct _DML_TERMINAL_BUTTONMAP
 {
    DML_TERMINAL_BUTTONMAP_BUTTON_LIST_T Button;
 } DML_TERMINAL_BUTTONMAP, *PDML_TERMINAL_BUTTONMAP;

typedef  struct _DML_TERMINAL_RINGER_DESCRIPTION
 {
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    void*   pParentTerminal;
    BOOL    RingEnable;
    CHAR    Alias[STR_LEN_64];
    CHAR    RingName[STR_LEN_64];
    CHAR    RingPattern[STR_LEN_256];
    CHAR    RingFile[STR_LEN_256];
 } DML_TERMINAL_RINGER_DESCRIPTION,*PDML_TERMINAL_RINGER_DESCRIPTION;

typedef  struct _DML_TERMINAL_RINGER_DESCRIPTION_CTRL
 {
    DML_TERMINAL_RINGER_DESCRIPTION     dml;
    bool                                updated;
 }  DML_TERMINAL_RINGER_DESCRIPTION_CTRL_T, *PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T;

typedef  struct _DML_TERMINAL_RINGER_DESCRIPTION_LIST_
 {
    UINT                                        ulQuantity;
    DML_TERMINAL_RINGER_DESCRIPTION_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_TERMINAL_RINGER_DESCRIPTION_LIST_T, *PDML_TERMINAL_RINGER_DESCRIPTION_LIST_T;

typedef  struct _DML_TERMINAL_RINGER_PATTERN
 {
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    void*   pParentTerminal;
    BOOL    Enable;
    CHAR    Alias[STR_LEN_64];
    BOOL    RingerOn;
    UINT    Duration;
    CHAR    NextPattern[STR_LEN_256];
 } DML_TERMINAL_RINGER_PATTERN,*PDML_TERMINAL_RINGER_PATTERN;

typedef  struct _DML_TERMINAL_RINGER_PATTERN_CTRL
 {
    DML_TERMINAL_RINGER_PATTERN     dml;
    bool                            updated;
 } DML_TERMINAL_RINGER_PATTERN_CTRL_T, *PDML_TERMINAL_RINGER_PATTERN_CTRL_T;

typedef  struct _DML_TERMINAL_RINGER_PATTERN_LIST_
 {
    UINT                                    ulQuantity;
    DML_TERMINAL_RINGER_PATTERN_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 } DML_TERMINAL_RINGER_PATTERN_LIST_T, *PDML_TERMINAL_RINGER_PATTERN_LIST_T;

typedef struct _DML_TERMINAL_RINGER
 {
    DML_TERMINAL_RINGER_DESCRIPTION_LIST_T  Description;
    DML_TERMINAL_RINGER_PATTERN_LIST_T      Pattern;
 } DML_TERMINAL_RINGER, *PDML_TERMINAL_RINGER;

typedef enum _DIAG_TESTSELECTOR_ENUM
{
    DIAG_TESTSELECTOR_PHONE_CONNECTIVITY_TEST = 0,
    DIAG_TESTSELECTOR_HAZARD_POTENTIAL,
    DIAG_TESTSELECTOR_FOREIGN_VOLTAGE,
    DIAG_TESTSELECTOR_RESISTIVE_FAULTS,
    DIAG_TESTSELECTOR_OFF_HOOK,
    DIAG_TESTSELECTOR_REN
} DIAG_TESTSELECTOR_ENUM;

typedef struct _DML_TERMINAL_DIAGTESTS
 {
    DIAG_STATE_ENUM           DiagnosticsState;
    DIAG_TESTSELECTOR_ENUM    TestSelector;
    BOOL                      PhoneRinging;
    CHAR                      X_RDK_TestResult[STR_LEN_256];
 } DML_TERMINAL_DIAGTESTS,*PDML_TERMINAL_DIAGTESTS;

typedef  struct _DML_VOICESERVICE_TERMINAL
{
    ULONG                       uInstanceNumber;
    void*                       pParentVoiceService;
    BOOL                        Enable;
    BOOL                        QuiescentMode;
    TERMINAL_STATUS_ENUM        Status;
    CHAR                        Alias[STR_LEN_64];
    CHAR                        ToneEventProfile[STR_LEN_256];
    DML_TERMINAL_AUDIO_LIST_T   Audio;
    DML_TERMINAL_BUTTONMAP      ButtonMap;
    DML_TERMINAL_RINGER         Ringer;
    DML_TERMINAL_DIAGTESTS      DiagTests;
} DML_VOICESERVICE_TERMINAL,*PDML_VOICESERVICE_TERMINAL;

typedef  struct _DML_VOICESERVICE_TERMINAL_CTRL
 {
    DML_VOICESERVICE_TERMINAL       dml;
    bool                            updated;
 } DML_VOICESERVICE_TERMINAL_CTRL_T,*PDML_VOICESERVICE_TERMINAL_CTRL_T;

typedef  struct _DML_VOICESERVICE_TERMINAL_LIST_
{
   UINT                                  ulQuantity;
   DML_VOICESERVICE_TERMINAL_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
} DML_VOICESERVICE_TERMINAL_LIST_T,*PDML_VOICESERVICE_TERMINAL_LIST_T;

typedef  struct _DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE
{
    ULONG   uInstanceNumber;
    void*   pParentVoiceService;
    BOOL    Enable;
    CHAR    Alias[STR_LEN_64];
    UINT    Order;
    CHAR    Interface[STR_LEN_256];
} DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE,*PDML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE;

typedef  struct _DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_CTRL
 {
    DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE      dml;
    bool                                        updated;
 } DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_CTRL_T, *PDML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_CTRL_T;

typedef  struct _DML_VOICESERVICE_CLOCKSYNCICE_CLOCKSYNC_CLOCKSOURCE_LIST_
 {
    UINT                                               ulQuantity;
    DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_CTRL_T*     pdata[TELCOVOICE_DATA_MAX];
 }  DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_LIST_T, *PDML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_LIST_T;

typedef struct _DML_VOICESERVICE_CLOCKSYNC
{
    CHAR                                            CurrentSource[STR_LEN_256];
    CHAR                                            Description[STR_LEN_64];
    BOOL                                            AutoRevert;
    DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_LIST_T   ClockSource;
}DML_VOICESERVICE_CLOCKSYNC, *PDML_VOICESERVICE_CLOCKSYNC;

typedef  struct _TELCOVOICEMGR_DML_X_RDK_DEBUG
{
    CHAR                                     CCTKTraceGroup[STR_LEN_256];
    CHAR                                     CCTKTraceLevel[STR_LEN_256];
    CHAR                                     ModuleLogLevels[STR_LEN_256];
    CHAR                                     LogServer[STR_LEN_256];
    UINT                                     LogServerPort;
} TELCOVOICEMGR_DML_X_RDK_DEBUG, *PTELCOVOICEMGR_DML_X_RDK_DEBUG;

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

typedef struct _TELCOVOICEMGR_DML_VOICESERVICE
 {
    ULONG                                   InstanceNumber;
    CHAR                                    Alias[STR_LEN_64];
    BOOL                                    X_RDK_DisableLoopCurrentUntilRegistered;
    CHAR                                    X_RDK_BoundIfName[STR_LEN_256];
    CHAR                                    X_RDK_IpAddressFamily[STR_LEN_256];
    CHAR                                    X_RDK_BoundIpAddr[STR_LEN_256];
    TELCOVOICEMGR_VOICE_ENABLE_ENUM         X_RDK_Enable;
    TELCOVOICEMGR_VOICE_STATUS_ENUM         X_RDK_Status;
    BOOL                                    X_RDK_FactoryReset;
    TELCOVOICEMGR_DML_X_RDK_DEBUG           X_RDK_DebugObj;
    DML_VOICESERVICE_CAPABILITIES           Capabilities;
    DML_RESERVEDPORTS                       ReservedPorts;
    DML_ISDN                                ISDN_obj;
    DML_POTS                                POTS_obj;
    DML_DECT                                DECT_obj;
    DML_SIP                                 SIP_obj;
    DML_MGCP                                MGCP_obj;
    DML_H323                                H323_obj;
    PDML_TRUNK_LIST_T                       Trunk;
    DML_CALLCONTROL                         CallControl_obj;
    PDML_INTERWORK_LIST_T                   Interwork;
    PDML_CALLLOG_LIST_T                     CallLog;
    PDML_VOIPPROFILE_LIST_T                 VoIPProfile;
    PDML_CODECPROFILE_LIST_T                CodecProfile;
    DML_TONE                                Tone_obj;
    PDML_VOICESERVICE_TERMINAL_LIST_T       Terminal;
    DML_VOICESERVICE_CLOCKSYNC              ClockSynchronization_obj;
 } TELCOVOICEMGR_DML_VOICESERVICE, *PTELCOVOICEMGR_DML_VOICESERVICE;

typedef  struct _DML_VOICE_SERVICE_CTRL_
{
    TELCOVOICEMGR_DML_VOICESERVICE      dml;
    BOOL                                updated;
} DML_VOICE_SERVICE_CTRL_T, *PDML_VOICE_SERVICE_CTRL_T;

typedef  struct _DML_VOICE_SERVICE_LIST_
{
    UINT                            ulQuantity;
    DML_VOICE_SERVICE_CTRL_T*       pdata[TELCOVOICEMGR_DATA_MAX_VOICE_SERVICES];
} DML_VOICE_SERVICE_LIST_T,*PDML_VOICE_SERVICE_LIST_T;


typedef  struct _TELCOVOICEMGR_DML_SERVICE
{
    DML_VOICE_SERVICE_LIST_T    VoiceService;
} TELCOVOICEMGR_DML_SERVICE;

typedef  struct _TELCOVOICEMGR_DML_DATA
{
    TELCOVOICEMGR_DML_SERVICE  Service;
    pthread_mutex_t            mDataMutex;  //Mutex
} TELCOVOICEMGR_DML_DATA;

TELCOVOICEMGR_DML_DATA* TelcoVoiceMgrDmlGetDataLocked(void);
void TelcoVoiceMgrDmlGetDataRelease(TELCOVOICEMGR_DML_DATA* pDmlData);
TELCOVOICEMGR_DML_DATA* TelcoVoiceMgrDmlGetData(void);
ANSC_STATUS TelcoVoiceMgrDmlDataInit(void);
ANSC_STATUS TelcoVoiceMgrDmlDataDelete(void);
ANSC_STATUS TelcoVoiceMgrDmlSetDefaultData(TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrData);
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceService(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCapabilitiesCodec(PDML_VOICESERVICE_CAPABILITIES_CODEC_LIST_T pCapabilitiesCodecList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddIsdnBRI(PDML_ISDN_BRI_LIST_T pIsdnBRIList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddIsdnPRI(PDML_ISDN_PRI_LIST_T pIsdnPRIList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddPotsFXO(PDML_POTS_FXO_LIST_T pPotsFXOList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddPotsFXS(PDML_POTS_FXS_LIST_T pPotsFXSList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddPotsRingerEvt(PDML_POTS_RINGER_EVT_LIST_T pPotsRingerEvtList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddDectBase(PDML_DECT_BASE_LIST_T pDectBaseList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddDectPortable(PDML_DECT_PORTABLE_LIST_T pDectPortableList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipClient(PDML_SIP_CLIENT_LIST_T pSipCliList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipClientContact(PDML_SIP_CLIENT_CONTACT_LIST_T pSipCliContactList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipClientContactEvtSub(PDML_SIP_CLIENT_SIPEVENTSUB_LIST_T pSipCliEvtSubscribeList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipNetwork(PDML_SIP_NETWORK_LIST_T pSipNetworkList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipNetworkFQDNServer(PDML_SIP_NETWORK_FQDNSERVER_LIST_T pSipNetworkFqdnList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipNetworkEvtSub(PDML_SIP_NETWORK_EVENTSUBSCRIBE_LIST_T pSipNetworkEvtSubscribeList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipNetworkRespMap(PDML_SIP_NETWORK_RESPMAP_LIST_T pSipNetworkRespMapList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipProxy(PDML_SIP_PROXY_LIST_T pSipProxyList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipRegistrar(PDML_SIP_REGISTRAR_LIST_T pSipRegistrarList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipRegistrarAcc(PDML_SIP_REGISTRAR_ACCOUNT_LIST_T pSipRegistrarAccList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddSipRegistrarAccContact(PDML_SIP_REGISTRAR_ACCOUNT_CONTACT_LIST_T pSipRegistrarAccContactList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddMGCPClient(PDML_MGCP_CLIENT_LIST_T pMGCP_ClientList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddMGCPNetwork(PDML_MGCP_NETWORK_LIST_T pMGCP_NetworkList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddH323Client(PDML_H323_CLIENT_LIST_T pH323CliList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddH323Network(PDML_H323_NETWORK_LIST_T pH323NwkList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddTrunk(PDML_TRUNK_LIST_T pTrunkList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlLine(PDML_CALLCONTROL_LINE_LIST_T pCallCtrlLineList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlExt(PDML_CALLCONTROL_EXTENSION_LIST_T pCallCtrlExtList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlGrp(PDML_CALLCONTROL_GROUP_LIST_T pCallCtrlGrpList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlMailBox(PDML_CALLCONTROL_MAILBOX_LIST_T pCallCtrlMailBoxList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlInMap(PDML_CALLCONTROL_INCOMINGMAP_LIST_T pCallCtrlInMapList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlOutMap(PDML_CALLCONTROL_OUTGOINGMAP_LIST_T pCallCtrlOutMapList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlNumPlan(PDML_CALLCONTROL_NUMBERINGPLAN_LIST_T pCallCtrlNumPlanList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlNumPlanPrefixInfo(PDML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_LIST_T pCallCtrlNumPlanPrefixInfoList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSet(PDML_CALLCONTROL_CALLINGFEATURES_SET_LIST_T pCallCtrlCFSetList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSetCFT(PDML_CALLCONTROL_CALLINGFEATURES_SET_CFT_LIST_T pCallCtrlCFSetCFTList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSetSCF(PDML_CALLCONTROL_CALLINGFEATURES_SET_SCF_LIST_T pCallCtrlCFSetSCFList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSetSCREJ(PDML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_LIST_T pCallCtrlCFSetSCREJList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSetFollowMe(PDML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_LIST_T pCallCtrlCFSetFollowMeList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddInterwork(PDML_INTERWORK_LIST_T pInterworkList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddInterworkUI(PDML_INTERWORK_USERINTERFACE_LIST_T pInterworkUIList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddInterworkMap(PDML_INTERWORK_MAP_LIST_T pInterworkMapList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallLog(PDML_CALLLOG_LIST_T pCallLogList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallLogSignalPerf(PDML_CALLLOG_SIGNALINGPERF_LIST_T pCallLogSignPerfList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCallLogSession(PDML_CALLLOG_SESSION_LIST_T pCallLogSessionList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddVoipProfile(PDML_VOIPPROFILE_LIST_T pVoipProfileList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddCodecProfile(PDML_CODECPROFILE_LIST_T pCodecProfileList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddToneDesc(PDML_TONE_DESCRIPTION_LIST_T pToneDescList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddTonePattern(PDML_TONE_PATTERN_LIST_T pTonePatternList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddToneEvtProf(PDML_TONE_EVENTPROFILE_LIST_T pToneEvtProfList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddToneEvtProfEvt(PDML_TONE_EVENTPROFILE_EVENT_LIST_T pToneEvtProfEvtList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddTerminal(PDML_VOICESERVICE_TERMINAL_LIST_T pTerminalList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddTerminalAudio(PDML_TERMINAL_AUDIO_LIST_T pTerminalAudioList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddTermButtonMapButton(PDML_TERMINAL_BUTTONMAP_BUTTON_LIST_T pTermButtonMapButtonList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddTermRingerDesc(PDML_TERMINAL_RINGER_DESCRIPTION_LIST_T pTermRingerDescList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddTermRingerPattern(PDML_TERMINAL_RINGER_PATTERN_LIST_T pTermRingerPatternList,int index);
ANSC_STATUS TelcoVoiceMgrDmlAddClockSource(PDML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_LIST_T pClkSyncSrcList,int index);

ANSC_STATUS TelcoVoiceMgrDmlSetLoopcurrentdisabled(UINT uiService, BOOL Bool);
ANSC_STATUS TelcoVoiceMgrDmlFactoryReset(uint32_t uiService, TELCOVOICEMGR_VOICE_ENABLE_ENUM VoiceState);
ANSC_STATUS TelcoVoiceMgrDmlSetBoundIfname(UINT uiService, char* BoundIfname);
ANSC_STATUS TelcoVoiceMgrDmlSetIpAddressFamily(UINT uiService, char *IpAddressFamily);
ANSC_STATUS TelcoVoiceMgrDmlSetCCTKTraceGroup(UINT uiService, char *CCTKTraceGroup);
ANSC_STATUS TelcoVoiceMgrDmlSetCCTKTraceLevel(UINT uiService, char *CCTKTraceLevel);
ANSC_STATUS TelcoVoiceMgrDmlSetModuleLogLevel(UINT uiService, char *logLevels);
ANSC_STATUS TelcoVoiceMgrDmlSetVoiceProcessState(UINT uiService, UINT uState);
ANSC_STATUS TelcoVoiceMgrDmlSetLinkState(TELCOVOICEMGR_VOICE_IP_LINK_STATE linkState);

ANSC_STATUS TelcoVoiceMgrDmlSetDirectoryNumber(UINT uiService, UINT uiProfile, UINT uiLine, char* pDirName);
ANSC_STATUS TelcoVoiceMgrDmlSetLineEnable(UINT uiService, UINT uiProfile, UINT uiLine, BOOL LineEnable);
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
ANSC_STATUS TelcoVoiceMgrDmlResetLineStats(UINT uiService, UINT uiProfile, UINT uiLine);
ANSC_STATUS TelcoVoiceMgrDmlSetReceiveGain(UINT uiService, UINT uiProfile,UINT uiLine, int32_t iGain);
ANSC_STATUS TelcoVoiceMgrDmlSetTransmitGain(UINT uiService, UINT uiProfile,UINT uiLine, int32_t iGain);
ANSC_STATUS TelcoVoiceMgrDmlSetLineSipURI(UINT uiService, UINT uiProfile, UINT uiLine, char*pSipURI);
ANSC_STATUS TelcoVoiceMgrDmlSetPrackRequired(UINT uiService, UINT uiProfile, BOOL bValue);

#endif /* _TELCOVOICEMGR_SERVICES_APIS_H */
