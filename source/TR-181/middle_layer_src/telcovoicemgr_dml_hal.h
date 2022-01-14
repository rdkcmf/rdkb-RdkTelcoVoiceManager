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

#ifndef _TELCOVOICEMGR_DML_HAL_H
#define _TELCOVOICEMGR_DML_HAL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_hal_common.h"
#include "json_hal_client.h"

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_dml_hal_param_v2.h"
#else
#include "telcovoicemgr_dml_hal_param_v1.h"
#endif

/**
 * Standard supported functions.
 */
#define GET_PARAMETER_METHOD "getParameters"
#define SET_PARAMETER_METHOD "setParameters"

#define JSON_RPC_PARAM_ARR_INDEX            0
#define JSON_RPC_FIELD_PARAMS               "params"
#define JSON_RPC_FIELD_NAME                 "name"
#define JSON_RPC_FIELD_VALUE                "value"
#define JSON_SUBSCRIBE_ON_CHANGE            "onChange"
#define JSON_SUBSCRIBE_ON_CHANGE_SYNC_TIMEOUT "onChangeSyncTimeout"

#define JSON_STR_TRUE                       "true"
#define JSON_STR_FALSE                      "false"

#define JSON_MAX_VAL_ARR_SIZE               256
#define JSON_MAX_STR_ARR_SIZE               256

#define HAL_CONNECTION_RETRY_MAX_COUNT      10
#define DML_GTC_FETCH_INTERVAL              10

#define   VOICE_SERVICE_TABLE_NAME            "Device.Services.VoiceService.%d."
#define   VOICE_SERVICE_STATUS                "Device.Services.VoiceService.%d.X_RDK_Status"
#define   X_RDK_DEBUG_TABLE_NAME              "Device.Services.VoiceService.%d.X_RDK_Debug."

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
/*  TR104 V2 DML Tables */
#define   PHYINTERFACE_TABLE_NAME             "Device.Services.VoiceService.%d.POTS.FXS.%d.DiagTests."
#define   PROFILE_TABLE_NAME                  "Device.Services.VoiceService.%d.VoIPProfile.%d."
#define   SIP_TABLE_NAME                      "Device.Services.VoiceService.%d.SIP.Network.%d."
#define   RTP_TABLE_NAME                      "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP."
#define   LINE_TABLE_NAME                     "Device.Services.VoiceService.%d.CallControl.Line.%d."
#define   LINE_SIP_TABLE_NAME                 "Device.Services.VoiceService.%d.SIP.Client.%d."
#define   LINE_VOICE_PROCESSING_TABLE_NAME    "Device.Services.VoiceService.%d.POTS.FXS.%d.VoiceProcessing."
#define   LINE_CALING_FEATURE_TABLE_NAME      "Device.Services.VoiceService.%d.CallControl.CallingFeatures.Set.%d."
#define   LINE_STATUS                         "Device.Services.VoiceService.%d.CallControl.Line.%d.Status"
#define   CALL_STATE                          "Device.Services.VoiceService.%d.CallControl.Line.%d.CallStatus"
#define   LINE_STATS_TABLE_NAME               "Device.Services.VoiceService.%d.CallControl.Line.%d.Stats."
#else
/* TR104 V1 DML Tables*/
#define   PHYINTERFACE_TABLE_NAME             "Device.Services.VoiceService.%d.PhyInterface.%d.Tests."
#define   PROFILE_TABLE_NAME                  "Device.Services.VoiceService.%d.VoiceProfile.%d."
#define   SIP_TABLE_NAME                      "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP."
#define   RTP_TABLE_NAME                      "Device.Services.VoiceService.%d.VoiceProfile.%d.RTP."
#define   LINE_TABLE_NAME                     "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d."
#define   LINE_SIP_TABLE_NAME                 "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.SIP."
#define   LINE_VOICE_PROCESSING_TABLE_NAME    "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.VoiceProcessing."
#define   LINE_STATS_TABLE_NAME               "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.Stats."
#define   LINE_CALING_FEATURE_TABLE_NAME      "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallingFeatures."
#define   LINE_STATUS                         "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.Status"
#define   CALL_STATE                          "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallState"
#define   TELCOVOICE_QUERY_VOICEPROFILE       "Device.Services.VoiceService.%d.VoiceProfile."
#define   TELCOVOICE_QUERY_VOICEPROFILE_LINE  "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d."
#define   TELCOVOICE_QUERY_PHYINTERFACE       "Device.Services.VoiceService.%d.PhyInterface."
#endif /*FEATURE_RDKB_VOICE_DM_TR104_V2*/

/**
 * telcovoice_manager_conf.json contains the port number of server as well as schema path.
 * This port number can be used in manager as well as
 * vendor software for rpc communication.
 */
#define TELCOVOICEMGR_CONF_FILE "/etc/rdk/conf/telcovoice_manager_conf.json"
#define NULL_TYPE 0
#define JSON_RPC_PARAM_ARR_INDEX 0

#define TELCOVOICE_QUERY_VOICESERVICE    "Device.Services.VoiceService."
#define TELCOVOICE_QUERY_CAPABILITIES    "Device.Services.VoiceService.%d.Capabilities."

#define FIREWALL_RULE_DATA          "Device.Services.VoiceService.%d.X_RDK_Firewall_Rule_Data"

#define FIREWALL_RULE_DATA_EVENT    "X_RDK_Firewall_Rule_Data"

#define CHECK(expr)                                                \
    if (!(expr))                                                   \
    {                                                              \
        CcspTraceError(("%s - %d Invalid parameter error \n!!!")); \
        return ANSC_STATUS_FAILURE;                                \
    }

#define FREE_JSON_OBJECT(expr) \
    if(expr)                   \
        {                      \
        json_object_put(expr); \
        }                      \

/**
 * @brief Initialise the TelcoVoice features
 *
 * @return Status of the operation
 * @retval ANSC_STATUS_SUCCESS if successful
 * @retval ANSC_STATUS_FAILURE if any error is detected
 */
ANSC_STATUS TelcoVoiceMgrHal_Init(void);
ANSC_STATUS TelcoVoiceMgrHal_SetParam(char *pName, eParamType pType, char *pValue);
ANSC_STATUS TelcoVoiceMgrHal_GetVoiceServices(DML_VOICE_SERVICE_LIST_T* pVoiceServiceList);
ANSC_STATUS TelcoVoiceMgrHal_InitData(bool bStatus);
ANSC_STATUS TelcoVoiceMgrHal_SetParamBool(char* HalName, BOOL bValue);
ANSC_STATUS TelcoVoiceMgrHal_SetParamInt(char* HalName, INT iValue);
ANSC_STATUS TelcoVoiceMgrHal_SetParamULong(char* HalName, ULONG uValue);
ANSC_STATUS TelcoVoiceMgrHal_SetParamString(char* HalName, char* sValue);
ANSC_STATUS TelcoVoiceMgrHal_SendJsonRequest(json_object *jmsg);
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
ANSC_STATUS TelcoVoiceMgrHal_GetCapabilities(PTELCOVOICEMGR_DML_CAPABILITIES pCapabilities);
ANSC_STATUS TelcoVoiceMgrHal_GetVoiceProfile(DML_PROFILE_LIST_T* pVoiceProfileList, int vsIndex);
ANSC_STATUS TelcoVoiceMgrHal_GetPhyInterface(DML_PHYINTERFACE_LIST_T* pPhyInterfaceList, int vsIndex);
#endif
ANSC_STATUS TelcoVoiceHal_GetLineStats(const char *param_name, TELCOVOICEMGR_DML_VOICESERVICE_STATS *pLineStats);
ANSC_STATUS TelcoVoiceMgrHal_GetInitData(void);
ANSC_STATUS TelcoVoiceMgrHal_EventSubscribe(event_callback callback, const char* event_name, const char* event_notification_type);
void eventcb_FirewallRuleData(const char *msg, const int len);
#endif
