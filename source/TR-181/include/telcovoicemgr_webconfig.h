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

 /***********************************
     Actual definition declaration
 ************************************/
 /*
     WEBCONFIG Part
 */

#define match(p, s) strncmp((p)->key.via.str.ptr, s, (p)->key.via.str.size)
#define member_size(type, member) sizeof(((type *)0)->member)

typedef struct
{
    unsigned int            uiLineInstanceNumber;
    unsigned char           CallWaitingEnable;	
    unsigned char           MWIEnable;
    unsigned char           X_RDK_Central_COM_ConferenceCallingEnable;
    unsigned char           X_RDK_Central_COM_HoldEnable;
    unsigned char           X_RDK_Central_COM_PhoneCallerIDEnable;
    char                    DirectoryNumber[BUFFER_LENGTH_32];
    char                    LineEnable[BUFFER_LENGTH_16];
    char                    SIPAuthUserName[BUFFER_LENGTH_64];
    char                    SIPAuthPassword[BUFFER_LENGTH_64];
    char                    SIPURI[BUFFER_LENGTH_32];
    int                     ReceiveGain;
    int                     TransmitGain;
} WebConfig_LineTable_t;

typedef struct
{
    unsigned int            uiServiceInstanceNumber;
    unsigned int            uiProfileInstanceNumber;
    char                    OutboundProxy[BUFFER_LENGTH_256];
    int                     OutboundProxyPort;
    char                    ProxyServer[BUFFER_LENGTH_256];
    int                     ProxyServerPort;
    char                    RegistrarServer[BUFFER_LENGTH_256];
    int                     RegistrarServerPort;
    char                    UserAgentDomain[BUFFER_LENGTH_256];
    int                     SIPDSCPMark;
    int                     SIPEthernetPriorityMark;
    char                    X_RDK_Central_COM_ConferencingURI[BUFFER_LENGTH_256];
    unsigned char           X_RDK_Central_COM_NetworkDisconnect;
    int                     RTPDSCPMark;
    int                     RTPEthernetPriorityMark;
    char                    X_RDK_Central_COM_DigitMap[BUFFER_LENGTH_2048];
    char                    X_RDK_Central_COM_EmergencyDigitMap[BUFFER_LENGTH_2048];
    int                     X_RDK_Central_COM_SDigitTimer;
    int                     X_RDK_Central_COM_ZDigitTimer;
    unsigned int            LineCount;
    WebConfig_LineTable_t   *pstLineInfo;
} WebConfig_VoiceServiceTable_t;

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

#endif /* _TELCOVOICEMGR_WEBCONFIG_H */
