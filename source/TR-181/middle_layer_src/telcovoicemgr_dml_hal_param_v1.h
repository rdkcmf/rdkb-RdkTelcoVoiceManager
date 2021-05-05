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

#ifndef _TELCOVOICEMGR_DML_HAL_PARAM_H 
#define _TELCOVOICEMGR_DML_HAL_PARAM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_hal_common.h"
#include "json_hal_client.h"
#include "telcovoicemgr_services_apis_v1.h"

#define MAX_STR_LEN 256

#define DML_VOICESERVICE                                 "Device.Services.VoiceService.%d."
#define DML_VOICESERVICE_CAPABILITIES_CODECS             DML_VOICESERVICE"Capabilities.Codecs.%d."
#define DML_VOICESERVICE_VOICEPROF                       DML_VOICESERVICE"VoiceProfile.%d."
#define DML_VOICESERVICE_VOICEPROF_SIP_EVTSUBSCRIBE      DML_VOICESERVICE_VOICEPROF"SIP.EventSubscribe.%d."
#define DML_VOICESERVICE_VOICEPROF_SIP_RESPMAP           DML_VOICESERVICE_VOICEPROF"SIP.ResponseMap.%d."
#define DML_VOICESERVICE_VOICEPROF_NUMPLAN_PREFIXINFO    DML_VOICESERVICE_VOICEPROF"NumberingPlan.PrefixInfo.%d."
#define DML_VOICESERVICE_VOICEPROF_TONE_EVT              DML_VOICESERVICE_VOICEPROF"Tone.Event.%d."
#define DML_VOICESERVICE_VOICEPROF_TONE_DESC             DML_VOICESERVICE_VOICEPROF"Tone.Description.%d."
#define DML_VOICESERVICE_VOICEPROF_TONE_PATTERN          DML_VOICESERVICE_VOICEPROF"Tone.Pattern.%d."
#define DML_VOICESERVICE_VOICEPROF_BUTTONMAP_BUTTON      DML_VOICESERVICE_VOICEPROF"ButtonMap.Button.%d."
#define DML_VOICESERVICE_VOICEPROF_LINE                  DML_VOICESERVICE_VOICEPROF"Line.%d."
#define DML_VOICESERVICE_VOICEPROF_LINE_SIP_EVTSUBSCRIBE DML_VOICESERVICE_VOICEPROF_LINE"SIP.EventSubscribe.%d."
#define DML_VOICESERVICE_VOICEPROF_LINE_RINGER_EVT       DML_VOICESERVICE_VOICEPROF_LINE"Ringer.Event.%d."
#define DML_VOICESERVICE_VOICEPROF_LINE_RINGER_DESC      DML_VOICESERVICE_VOICEPROF_LINE"Ringer.Description.%d."
#define DML_VOICESERVICE_VOICEPROF_LINE_RINGER_PATTERN   DML_VOICESERVICE_VOICEPROF_LINE"Ringer.Pattern.%d."
#define DML_VOICESERVICE_VOICEPROF_LINE_CODEC_LIST       DML_VOICESERVICE_VOICEPROF_LINE"Codec.List.%d."
#define DML_VOICESERVICE_VOICEPROF_LINE_SESSION          DML_VOICESERVICE_VOICEPROF_LINE"Session.%d."
#define DML_VOICESERVICE_PHYIFACE                        DML_VOICESERVICE"PhyInterface.%d."


#define telcovoicemgr_hal_get_voiceService_index                        telcovoicemgr_hal_get_first_index
#define telcovoicemgr_hal_get_capabilitiesCodec_index                   telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_voiceProfile_index                        telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_phyInterface_index                        telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_voiceProfile_sipEvtSubscribe_index        telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_sipRespMap_index             telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_numPlanPrefixInfo_index      telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_toneEvt_index                telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_toneDesc_index               telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_tonePattern_index            telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_buttonMapButton_index        telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_line_index                   telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_line_sipEvtSubscribe_index   telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_line_ringerEvt_index         telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_line_ringerDesc_index        telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_line_ringerPattern_index     telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_line_codecList_index         telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_voiceProfile_line_session_index           telcovoicemgr_hal_get_third_index

ANSC_STATUS Map_hal_dml_capabilities(PTELCOVOICEMGR_DML_CAPABILITIES pCapabilities, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_phyInterface(DML_PHYINTERFACE_LIST_T* pPhyInterfaceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_voiceProfile(DML_PROFILE_LIST_T* pVoiceProfileList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_voiceProfileLine(DML_LINE_LIST_T* pVoiceProfile_Line_List, char* ParamName, char* pValue);
#endif //_TELCOVOICEMGR_DML_HAL_PARAM_H
