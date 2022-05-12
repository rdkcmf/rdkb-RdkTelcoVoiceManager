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


#ifndef  _TELCOVOICEMGR_WEBCONFIG_H
#define  _TELCOVOICEMGR_WEBCONFIG_H

/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

#include <time.h>
#include <errno.h>
#include <msgpack.h>
#include <stdarg.h>
#include "ansc_platform.h"
#include "ansc_string_util.h"
#include "webconfig_framework.h"

/* Buffer Length Macros */

#define BUFFER_LENGTH_16                16
#define BUFFER_LENGTH_32                32
#define BUFFER_LENGTH_64                64
#define BUFFER_LENGTH_128               128
#define BUFFER_LENGTH_256               256
#define BUFFER_LENGTH_512               512
#define BUFFER_LENGTH_1024              1024
#define BUFFER_LENGTH_2048              2048

#define TELCOVOICEDATA_DEFAULT_TIMEOUT  15

#define TELCOVOICEDATA_SUBDOC_COUNT     1   

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#define CURRENT_DML_VERSION  "TR104V2"
#else
#define CURRENT_DML_VERSION  "TR104V1"
#endif

 /***********************************
     Actual definition declaration
 ************************************/
 /*
     WEBCONFIG Part
 */

#define match(p, s) strncmp((p)->key.via.str.ptr, s, (p)->key.via.str.size)
#define member_size(type, member) sizeof(((type *)0)->member)

#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
typedef struct
{
    unsigned int            uiLineInstanceNumber;
    unsigned char           CallWaitingEnable;
    unsigned char           IsCallWaitingEnablePresent;
    unsigned char           MWIEnable;
    unsigned char           IsMWIEnablePresent;
    unsigned char           X_RDK_Central_COM_ConferenceCallingEnable;
    unsigned char           IsConferenceCallingEnablePresent;
    unsigned char           X_RDK_Central_COM_HoldEnable;
    unsigned char           IsHoldEnablePresent;
    unsigned char           X_RDK_Central_COM_PhoneCallerIDEnable;
    unsigned char           IsPhoneCallerIDEnablePresent;
    char                    DirectoryNumber[BUFFER_LENGTH_32];
    unsigned char           IsDirectoryNumberPresent;
    char                    LineEnable[BUFFER_LENGTH_16];
    unsigned char           IsLineEnablePresent;
    char                    SIPAuthUserName[BUFFER_LENGTH_64];
    unsigned char           IsSIPAuthUserNamePresent;
    char                    SIPAuthPassword[BUFFER_LENGTH_64];
    unsigned char           IsSIPAuthPasswordPresent;
    char                    SIPURI[BUFFER_LENGTH_32];
    unsigned char           IsSIPURIPresent;
    int                     ReceiveGain;
    unsigned char           IsReceiveGainPresent;
    int                     TransmitGain;
    unsigned char           IsTransmitGainPresent;
} WebConfig_LineTable_t;

typedef struct
{
    unsigned int            uiServiceInstanceNumber;
    unsigned int            uiProfileInstanceNumber;
    char                    OutboundProxy[BUFFER_LENGTH_256];
    unsigned char           IsOutboundProxyPresent;
    int                     OutboundProxyPort;
    unsigned char           IsOutboundProxyPortPresent;
    char                    ProxyServer[BUFFER_LENGTH_256];
    unsigned char           IsProxyServerPresent;
    int                     ProxyServerPort;
    unsigned char           IsProxyServerPortPresent;
    char                    RegistrarServer[BUFFER_LENGTH_256];
    unsigned char           IsRegistrarServerPresent;
    int                     RegistrarServerPort;
    unsigned char           IsRegistrarServerPortPresent;
    char                    UserAgentDomain[BUFFER_LENGTH_256];
    unsigned char           IsUserAgentDomainPresent;
    int                     SIPDSCPMark;
    unsigned char           IsSIPDSCPMarkPresent;
    int                     SIPEthernetPriorityMark;
    unsigned char           IsSIPEthernetPriorityMarkPresent;
    char                    X_RDK_Central_COM_ConferencingURI[BUFFER_LENGTH_256];
    unsigned char           IsConferencingURIPresent;
    unsigned char           X_RDK_Central_COM_NetworkDisconnect;
    unsigned char           IsNetworkDisconnectPresent;
    int                     RTPDSCPMark;
    unsigned char           IsRTPDSCPMarkPresent;
    int                     RTPEthernetPriorityMark;
    unsigned char           IsRTPEthernetPriorityMarkPresent;
    char                    X_RDK_Central_COM_DigitMap[BUFFER_LENGTH_2048];
    unsigned char           IsDigitMapPresent;
    char                    X_RDK_Central_COM_EmergencyDigitMap[BUFFER_LENGTH_2048];
    unsigned char           IsEmergencyDigitMapPresent;
    int                     X_RDK_Central_COM_SDigitTimer;
    unsigned char           IsSDigitTimerPresent;
    int                     X_RDK_Central_COM_ZDigitTimer;
    unsigned char           IsZDigitTimerPresent;
    unsigned int            LineCount;
    WebConfig_LineTable_t   *pstLineInfo;
} WebConfig_VoiceServiceTable_t;
#else
typedef struct
{
    unsigned int            uiSipNwInstanceNumber;
    unsigned char           SIPNetworkEnable;
    unsigned char           IsSIPNetworkEnablePresent;
    char                    OutboundProxy[BUFFER_LENGTH_256];
    unsigned char           IsOutboundProxyPresent;
    int                     OutboundProxyPort;
    unsigned char           IsOutboundProxyPortPresent;
    char                    ProxyServer[BUFFER_LENGTH_256];
    unsigned char           IsProxyServerPresent;
    int                     ProxyServerPort;
    unsigned char           IsProxyServerPortPresent;
    char                    ProxyServerTransport[BUFFER_LENGTH_256];
    unsigned char           IsProxyServerTransportPresent;
    char                    RegistrarServer[BUFFER_LENGTH_256];
    unsigned char           IsRegistrarServerPresent;
    int                     RegistrarServerPort;
    unsigned char           IsRegistrarServerPortPresent;
    char                    RegistrarServerTransport[BUFFER_LENGTH_256];
    unsigned char           IsRegistrarServerTransportPresent;
    char                    UserAgentDomain[BUFFER_LENGTH_256];
    unsigned char           IsUserAgentDomainPresent;
    int                     UserAgentPort;
    unsigned char           IsUserAgentPortPresent;
    char                    UserAgentTransport[BUFFER_LENGTH_256];
    unsigned char           IsUserAgentTransportPresent;
    char                    SIPNetworkVoIPProfile[BUFFER_LENGTH_256];
    unsigned char           IsSIPNetworkVoIPProfilePresent;
    int                     SIPDSCPMark;
    unsigned char           IsSIPDSCPMarkPresent;
    int                     SIPEthernetPriorityMark;
    unsigned char           IsSIPEthernetPriorityMarkPresent;
    char                    ConferencingURI[BUFFER_LENGTH_256];
    unsigned char           IsConferencingURIPresent;
    unsigned char           NetworkDisconnect;
    unsigned char           IsNetworkDisconnectPresent;
    unsigned char           PRACKRequired;
    unsigned char           IsPRACKRequiredPresent;
} WebConfig_SipNwTable_t;

typedef struct
{
    unsigned int            uiSipClientInstanceNumber;
    unsigned char           SIPClientEnable;
    unsigned char           IsSIPClientEnablePresent;
    char                    SIPClientNetwork[BUFFER_LENGTH_64];
    unsigned char           IsSIPClientNetworkPresent;
    char                    SIPAuthUserName[BUFFER_LENGTH_64];
    unsigned char           IsSIPAuthUserNamePresent;
    char                    SIPAuthPassword[BUFFER_LENGTH_64];
    unsigned char           IsSIPAuthPasswordPresent;
    char                    SIPURI[BUFFER_LENGTH_32];
    unsigned char           IsSIPURIPresent;
} WebConfig_SipClientTable_t;

typedef struct
{
    unsigned int            uiProfileInstanceNumber;
    unsigned char           ProfileEnable;
    unsigned char           IsProfileEnablePresent;
    int                     RTPDSCPMark;
    unsigned char           IsRTPDSCPMarkPresent;
    int                     RTPEthernetPriorityMark;
    unsigned char           IsRTPEthernetPriorityMarkPresent;
    char                    DigitMap[BUFFER_LENGTH_2048];
    unsigned char           IsDigitMapPresent;
    char                    EmergencyDigitMap[BUFFER_LENGTH_2048];
    unsigned char           IsEmergencyDigitMapPresent;
    int                     SDigitTimer;
    unsigned char           IsSDigitTimerPresent;
    int                     ZDigitTimer;
    unsigned char           IsZDigitTimerPresent;
} WebConfig_VoIPTable_t;

typedef struct
{
    unsigned int            uiLineInstanceNumber;
    unsigned char           CallWaitingEnable;
    unsigned char           IsCallWaitingEnablePresent;
    unsigned char           MWIEnable;
    unsigned char           IsMWIEnablePresent;
    unsigned char           ConferenceCallingEnable;
    unsigned char           IsConferenceCallingEnablePresent;
    unsigned char           HoldEnable;
    unsigned char           IsHoldEnablePresent;
    unsigned char           PhoneCallerIDEnable;
    unsigned char           IsPhoneCallerIDEnablePresent;
    char                    DirectoryNumber[BUFFER_LENGTH_32];
    unsigned char           IsDirectoryNumberPresent;
    char                    LineProvider[BUFFER_LENGTH_32];
    unsigned char           IsLineProviderPresent;
    char                    LineCallingFeatures[BUFFER_LENGTH_32];
    unsigned char           IsLineCallingFeaturesPresent;
    unsigned char           LineEnable;
    unsigned char           IsLineEnablePresent;
} WebConfig_LineTable_t;

typedef struct
{
    unsigned int            uiFXSInstanceNumber;
    unsigned char           FXSEnable;
    unsigned char           IsFXSEnablePresent;
    int                     ReceiveGain;
    unsigned char           IsReceiveGainPresent;
    int                     TransmitGain;
    unsigned char           IsTransmitGainPresent;
} WebConfig_POTSFxsTable_t;

typedef struct
{
    unsigned int                uiServiceInstanceNumber;
    unsigned int                SipNwCount;
    WebConfig_SipNwTable_t      *pstSipNwInfo;
    unsigned int                SipClientCount;
    WebConfig_SipClientTable_t  *pstSipClientInfo;
    unsigned int                VoIPCount;
    WebConfig_VoIPTable_t       *pstVoIPInfo;
    unsigned int                LineCount;
    WebConfig_LineTable_t       *pstLineInfo;
    unsigned int                FXSCount;
    WebConfig_POTSFxsTable_t   *pstPOTSFxsInfo;
} WebConfig_VoiceServiceTable_t;
#endif

typedef struct {
    char                            spec[BUFFER_LENGTH_64];
    char                            subdoc_name[BUFFER_LENGTH_64];
    unsigned int                    version;
    unsigned short int              transaction_id;
    unsigned int                    ServiceCount;
    WebConfig_VoiceServiceTable_t   *pstServiceInfo;
} TelcoVoice_WebConfig_t;

ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_TelcoVoipParams(msgpack_object obj, TelcoVoice_WebConfig_t *pWebConfig);
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_VoiceServiceParams( WebConfig_VoiceServiceTable_t *e, msgpack_object_map *map );
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_LineTable( WebConfig_LineTable_t *e, msgpack_object_map *map );

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_SipNetworkTable( WebConfig_SipNwTable_t *e, msgpack_object_map *map );
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_SipClientTable( WebConfig_SipClientTable_t *e, msgpack_object_map *map );
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_ProfileTable( WebConfig_VoIPTable_t *e, msgpack_object_map *map );
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_POTSFxsTable( WebConfig_POTSFxsTable_t *e, msgpack_object_map *map );
#endif

#endif /* _TELCOVOICEMGR_WEBCONFIG_H */
