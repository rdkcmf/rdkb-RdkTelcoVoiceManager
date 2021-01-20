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

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

/***********************************************************************

    module: plugin_main.c

        Implement COSA Data Model Library Init and Unload apis.

    ---------------------------------------------------------------

    author:

        COSA XML TOOL CODE GENERATOR 1.0

    ---------------------------------------------------------------

    revision:

        01/14/2011    initial revision.

**********************************************************************/
#include "ansc_platform.h"
#include "ansc_load_library.h"
#include "cosa_plugin_api.h"
#include "telcovoicemgr_dml_plugin_main.h"
#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_services.h"
#include "telcovoicemgr_dml_profile.h"
#include "telcovoicemgr_dml_line.h"
#include "telcovoicemgr_dml_numberingplan.h"
#include "telcovoicemgr_dml_tone.h"
//#include "voice_hal.h"
#include "ccsp_trace.h"

PCOSA_BACKEND_MANAGER_OBJECT g_pTelcoVoiceBEManager;
void *                       g_pDslhDmlAgent;
extern ANSC_HANDLE     g_MessageBusHandle_Irep;
extern char            g_SubSysPrefix_Irep[32];
extern COSARepopulateTableProc            g_COSARepopulateTable;

#define THIS_PLUGIN_VERSION                         1

int ANSC_EXPORT_API TELCOVOICEMGR_Init(ULONG uMaxVersionSupported, void* hCosaPlugInfo)
{
    PCOSA_PLUGIN_INFO               pPlugInfo                   = (PCOSA_PLUGIN_INFO                 )hCosaPlugInfo;
    COSAGetParamValueByPathNameProc pGetParamValueByPathNameProc = (COSAGetParamValueByPathNameProc)NULL;
    COSASetParamValueByPathNameProc pSetParamValueByPathNameProc = (COSASetParamValueByPathNameProc)NULL;
    COSAGetParamValueStringProc     pGetStringProc              = (COSAGetParamValueStringProc       )NULL;
    COSAGetParamValueUlongProc      pGetParamValueUlongProc     = (COSAGetParamValueUlongProc        )NULL;
    COSAGetParamValueIntProc        pGetParamValueIntProc       = (COSAGetParamValueIntProc          )NULL;
    COSAGetParamValueBoolProc       pGetParamValueBoolProc      = (COSAGetParamValueBoolProc         )NULL;
    COSASetParamValueStringProc     pSetStringProc              = (COSASetParamValueStringProc       )NULL;
    COSASetParamValueUlongProc      pSetParamValueUlongProc     = (COSASetParamValueUlongProc        )NULL;
    COSASetParamValueIntProc        pSetParamValueIntProc       = (COSASetParamValueIntProc          )NULL;
    COSASetParamValueBoolProc       pSetParamValueBoolProc      = (COSASetParamValueBoolProc         )NULL;
    COSAGetInstanceNumbersProc      pGetInstanceNumbersProc     = (COSAGetInstanceNumbersProc        )NULL;

    COSAGetCommonHandleProc         pGetCHProc                  = (COSAGetCommonHandleProc           )NULL;
    COSAValidateHierarchyInterfaceProc
                                    pValInterfaceProc           = (COSAValidateHierarchyInterfaceProc)NULL;
    COSAGetHandleProc               pGetRegistryRootFolder      = (COSAGetHandleProc                 )NULL;
    COSAGetInstanceNumberByIndexProc
                                    pGetInsNumberByIndexProc    = (COSAGetInstanceNumberByIndexProc  )NULL;
    COSAGetHandleProc               pGetMessageBusHandleProc    = (COSAGetHandleProc                 )NULL;
    COSAGetInterfaceByNameProc      pGetInterfaceByNameProc     = (COSAGetInterfaceByNameProc        )NULL;
    ULONG                           ret                         = 0;

    if ( uMaxVersionSupported < THIS_PLUGIN_VERSION )
    {
      /* this version is not supported */
        return -1;
    }

    pPlugInfo->uPluginVersion       = THIS_PLUGIN_VERSION;
    g_pDslhDmlAgent                 = pPlugInfo->hDmlAgent;

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Services_GetParamUlongValue",  Services_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_IsUpdated", VoiceService_IsUpdated );
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_Synchronize", VoiceService_Synchronize );
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetEntryCount", VoiceService_GetEntryCount );
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetEntry",  VoiceService_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetParamBoolValue",VoiceService_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_SetParamBoolValue",VoiceService_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetParamStringValue", VoiceService_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_SetParamStringValue", VoiceService_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetParamUlongValue", VoiceService_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_SetParamUlongValue", VoiceService_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetParamBoolValue", VoiceService_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetParamStringValue", VoiceService_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_Validate",  VoiceService_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_Commit",  VoiceService_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_Rollback", VoiceService_Rollback );

    
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_GetParamStringValue",  X_RDK_Debug_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_GetParamUlongValue",  X_RDK_Debug_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_SetParamStringValue",  X_RDK_Debug_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_IsUpdated", VoiceProfile_IsUpdated);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_Synchronize", VoiceProfile_Synchronize);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_GetParamUlongValue", VoiceProfile_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_SetParamUlongValue", VoiceProfile_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_GetParamStringValue", VoiceProfile_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_SetParamStringValue", VoiceProfile_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_GetParamStringValue", VoiceProfile_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_GetEntryCount", VoiceProfile_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_SetParamBoolValue",  VoiceProfile_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_GetParamBoolValue", VoiceProfile_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_GetEntry", VoiceProfile_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_Validate",  VoiceProfile_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_Commit",  VoiceProfile_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProfile_Rollback", VoiceProfile_Rollback );

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_GetParamStringValue", SIP_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_SetParamStringValue", SIP_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_GetParamUlongValue", SIP_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_SetParamUlongValue", SIP_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_GetParamBoolValue", SIP_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_SetParamBoolValue", SIP_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_GetParamIntValue", SIP_GetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_SetParamIntValue", SIP_SetParamIntValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTP_GetParamUlongValue", RTP_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTP_SetParamUlongValue", RTP_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTP_GetParamIntValue", RTP_GetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTP_GetParamStringValue", RTP_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTP_SetParamIntValue", RTP_SetParamIntValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_IsUpdated",  Line_IsUpdated);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_Synchronize",  Line_Synchronize);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_GetEntry",  Line_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_GetEntryCount",  Line_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_GetParamStringValue", Line_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_SetParamStringValue", Line_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_GetParamUlongValue", Line_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_GetParamStringValue", Line_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_GetParamBoolValue", Line_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_SetParamUlongValue", Line_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_Validate",  Line_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_Commit",  Line_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Line_Rollback", Line_Rollback );

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProcessing_GetParamUlongValue", VoiceProcessing_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProcessing_SetParamUlongValue", VoiceProcessing_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProcessing_GetParamIntValue", VoiceProcessing_GetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProcessing_SetParamIntValue", VoiceProcessing_SetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProcessing_GetParamBoolValue", VoiceProcessing_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProcessing_Validate",  VoiceProcessing_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProcessing_Commit",  VoiceProcessing_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceProcessing_Rollback", VoiceProcessing_Rollback);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineSIP_GetParamStringValue", LineSIP_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineSIP_SetParamStringValue", LineSIP_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineMGCP_GetParamStringValue", LineMGCP_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineMGCP_SetParamStringValue", LineMGCP_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineH323_GetParamStringValue", LineH323_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineH323_SetParamStringValue", LineH323_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CallingFeatures_GetParamUlongValue", CallingFeatures_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CallingFeatures_SetParamUlongValue", CallingFeatures_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CallingFeatures_GetParamBoolValue",  CallingFeatures_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CallingFeatures_SetParamBoolValue",  CallingFeatures_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CallingFeatures_GetParamStringValue", CallingFeatures_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CallingFeatures_SetParamStringValue", CallingFeatures_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Stats_GetParamBoolValue",  Stats_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Stats_GetParamUlongValue",  Stats_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Stats_SetParamBoolValue",  Stats_SetParamBoolValue);

    // VoiceService.{i}.Capabilities.
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Capabilities_GetParamUlongValue", Capabilities_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Capabilities_GetParamStringValue", Capabilities_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Capabilities_GetParamBoolValue", Capabilities_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesSIP_GetParamBoolValue", CapabilitiesSIP_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesSIP_GetParamStringValue", CapabilitiesSIP_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesMGCP_GetParamStringValue", CapabilitiesMGCP_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesH323_GetParamStringValue", CapabilitiesH323_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesH323_GetParamBoolValue", CapabilitiesH323_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesCodecs_GetParamStringValue", CapabilitiesCodecs_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesCodecs_GetParamBoolValue", CapabilitiesCodecs_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesCodecs_SetParamStringValue", CapabilitiesCodecs_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesCodecs_GetParamUlongValue", CapabilitiesCodecs_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesCodecs_GetEntryCount", CapabilitiesCodecs_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "CapabilitiesCodecs_GetEntry", CapabilitiesCodecs_GetEntry);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterface_GetParamUlongValue", PhyInterface_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterface_SetParamStringValue", PhyInterface_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterfaceTests_GetParamBoolValue", PhyInterfaceTests_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterfaceTests_SetParamStringValue", PhyInterfaceTests_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterfaceTests_GetParamUlongValue", PhyInterfaceTests_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterfaceTests_SetParamUlongValue", PhyInterfaceTests_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterface_IsUpdated", PhyInterface_IsUpdated);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterface_Synchronize", PhyInterface_Synchronize);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterface_GetEntry",  PhyInterface_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterface_GetEntryCount", PhyInterface_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterface_GetParamStringValue", PhyInterface_GetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTCP_GetParamUlongValue", RTCP_GetParamUlongValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SRTP_GetParamBoolValue",    SRTP_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SRTP_GetParamStringValue",  SRTP_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SRTP_SetParamStringValue",  SRTP_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SRTP_SetParamBoolValue",    SRTP_SetParamBoolValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "NumberingPlan_GetParamUlongValue", NumberingPlan_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "NumberingPlan_SetParamUlongValue",  NumberingPlan_SetParamUlongValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PrefixInfo_GetEntryCount", PrefixInfo_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PrefixInfo_GetEntry", PrefixInfo_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PrefixInfo_GetParamUlongValue", PrefixInfo_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PrefixInfo_GetParamStringValue", PrefixInfo_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PrefixInfo_SetParamStringValue",  PrefixInfo_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PrefixInfo_SetParamUlongValue",  PrefixInfo_SetParamUlongValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneDescription_GetEntryCount", ToneDescription_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneDescription_GetEntry", ToneDescription_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneDescription_GetParamStringValue", ToneDescription_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneDescription_GetParamBoolValue", ToneDescription_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneDescription_GetParamUlongValue", ToneDescription_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneDescription_SetParamUlongValue",  ToneDescription_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneDescription_SetParamStringValue",  ToneDescription_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneDescription_SetParamBoolValue",  ToneDescription_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneDescription_GetParamBoolValue", ToneDescription_GetParamBoolValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneEvent_GetEntryCount", ToneEvent_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneEvent_GetEntry", ToneEvent_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneEvent_GetParamUlongValue", ToneEvent_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneEvent_GetParamStringValue", ToneEvent_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneEvent_SetParamUlongValue",  ToneEvent_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ToneEvent_SetParamStringValue",  ToneEvent_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TonePattern_GetEntryCount", TonePattern_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TonePattern_GetEntry", TonePattern_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TonePattern_GetParamUlongValue", TonePattern_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TonePattern_GetParamStringValue", TonePattern_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TonePattern_SetParamIntValue",  TonePattern_SetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TonePattern_GetParamBoolValue", TonePattern_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TonePattern_GetParamIntValue", TonePattern_GetParamIntValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ButtonMap_GetParamUlongValue", ButtonMap_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Button_GetEntryCount", Button_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Button_GetEntry", Button_GetEntry);   
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Button_GetParamBoolValue", Button_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Button_GetParamStringValue", Button_GetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "FaxT38_GetParamUlongValue", FaxT38_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "FaxT38_GetParamBoolValue", FaxT38_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "FaxT38_GetParamStringValue", FaxT38_GetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Event_IsUpdated",  Event_IsUpdated);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Event_Synchronize",  Event_Synchronize);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Event_GetEntry",  Event_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Event_GetEntryCount",  Event_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Event_GetParamUlongValue", Event_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Event_GetParamStringValue", Event_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Event_Validate",  Event_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Event_Commit",  Event_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Event_Rollback", Event_Rollback );
    
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Description_IsUpdated",  Description_IsUpdated);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Description_Synchronize",  Description_Synchronize);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Description_GetEntry",  Description_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Description_GetEntryCount",  Description_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Description_GetParamUlongValue", Description_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Description_GetParamStringValue", Description_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Description_Validate",  Description_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Description_Commit",  Description_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Description_Rollback", Description_Rollback );

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_GetParamBoolValue", SIP_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIP_GetParamStringValue", SIP_GetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPResponseMap_GetEntryCount", SIPResponseMap_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPResponseMap_GetEntry", SIPResponseMap_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPResponseMap_GetParamUlongValue", SIPResponseMap_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPResponseMap_GetParamStringValue", SIPResponseMap_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPResponseMap_SetParamUlongValue", SIPResponseMap_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPResponseMap_SetParamStringValue", SIPResponseMap_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "MGCP_GetParamBoolValue", MGCP_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "MGCP_GetParamStringValue", MGCP_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "MGCP_GetParamIntValue", MGCP_GetParamIntValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "H323_GetParamStringValue", H323_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "H323_GetParamIntValue", H323_GetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "H323_GetParamBoolValue", H323_GetParamBoolValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_IsUpdated",  Pattern_IsUpdated);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_Synchronize",  Pattern_Synchronize);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_GetEntry",  Pattern_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_GetEntryCount",  Pattern_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_GetParamBoolValue", Pattern_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_GetParamUlongValue", Pattern_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_Validate",  Pattern_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_Commit",  Pattern_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_Rollback", Pattern_Rollback );

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_IsUpdated",      List_IsUpdated);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_Synchronize",    List_Synchronize);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_GetEntry",       List_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_GetEntryCount",  List_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_GetParamUlongValue", List_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_GetParamBoolValue", List_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_GetParamStringValue", List_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_Validate",    List_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_Commit",      List_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "List_Rollback",    List_Rollback );

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineCodec_GetParamBoolValue", LineCodec_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineCodec_GetParamUlongValue", LineCodec_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineCodec_GetParamStringValue", LineCodec_GetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "PhyInterfaceTests_GetParamStringValue", PhyInterfaceTests_GetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ServiceProviderInfo_GetParamStringValue", ServiceProviderInfo_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "ServiceProviderInfo_SetParamStringValue",  ServiceProviderInfo_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "LineSIP_GetParamStringValue", LineSIP_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Pattern_GetParamStringValue", Pattern_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Redundancy_GetParamIntValue", Redundancy_GetParamIntValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPEventSubscribe_GetEntryCount", SIPEventSubscribe_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPEventSubscribe_GetEntry", SIPEventSubscribe_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPEventSubscribe_SetParamUlongValue",  SIPEventSubscribe_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPEventSubscribe_SetParamStringValue",  SIPEventSubscribe_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPEventSubscribe_GetParamUlongValue",  SIPEventSubscribe_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "SIPEventSubscribe_GetParamStringValue",  SIPEventSubscribe_GetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "MGCP_SetParamBoolValue",  MGCP_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "MGCP_SetParamIntValue",  MGCP_SetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "MGCP_SetParamUlongValue",  MGCP_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "MGCP_SetParamStringValue",  MGCP_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "H323_SetParamBoolValue",  H323_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "H323_SetParamIntValue",  H323_SetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "H323_SetParamUlongValue",  H323_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "H323_SetParamStringValue",  H323_SetParamStringValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTCP_SetParamUlongValue",  RTCP_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTCP_SetParamBoolValue",  RTCP_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTCP_GetParamBoolValue",  RTCP_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTCP_SetParamStringValue",  RTCP_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "RTCP_GetParamStringValue",  RTCP_GetParamStringValue);


    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Redundancy_GetParamUlongValue",  Redundancy_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Redundancy_SetParamUlongValue",  Redundancy_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Redundancy_SetParamIntValue",  Redundancy_SetParamIntValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Redundancy_SetParamBoolValue",  Redundancy_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Redundancy_GetParamBoolValue",  Redundancy_GetParamBoolValue);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "EventSubscribe_IsUpdated",  EventSubscribe_IsUpdated);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "EventSubscribe_Synchronize",  EventSubscribe_Synchronize);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "EventSubscribe_GetEntry",  EventSubscribe_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "EventSubscribe_GetEntryCount",  EventSubscribe_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "EventSubscribe_SetParamStringValue",  EventSubscribe_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "EventSubscribe_GetParamStringValue",  EventSubscribe_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "EventSubscribe_Validate",  EventSubscribe_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "EventSubscribe_Commit",  EventSubscribe_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "EventSubscribe_Rollback", EventSubscribe_Rollback );

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Session_IsUpdated",            Session_IsUpdated);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Session_Synchronize",          Session_Synchronize);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Session_GetEntry",             Session_GetEntry);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Session_GetEntryCount",        Session_GetEntryCount);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Session_GetParamUlongValue",   Session_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Session_GetParamStringValue",  Session_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Session_Validate",             Session_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Session_Commit",               Session_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "Session_Rollback",             Session_Rollback );
    pGetParamValueByPathNameProc = (COSAGetParamValueByPathNameProc)pPlugInfo->AcquireFunction("COSAGetParamValueByPathName");

    if( pGetParamValueByPathNameProc != NULL)
    {
        g_GetParamValueByPathNameProc = pGetParamValueByPathNameProc;
    }
    else
    {
        goto EXIT;
    }

    pSetParamValueByPathNameProc = (COSASetParamValueByPathNameProc)pPlugInfo->AcquireFunction("COSASetParamValueByPathName");

    if( pSetParamValueByPathNameProc != NULL)
    {
        g_SetParamValueByPathNameProc = pSetParamValueByPathNameProc;
    }
    else
    {
        goto EXIT;
    }

    pGetStringProc = (COSAGetParamValueStringProc)pPlugInfo->AcquireFunction("COSAGetParamValueString");

    if( pGetStringProc != NULL)
    {
        g_GetParamValueString = pGetStringProc;
    }
    else
    {
        goto EXIT;
    }

    pGetParamValueUlongProc = (COSAGetParamValueUlongProc)pPlugInfo->AcquireFunction("COSAGetParamValueUlong");

    if( pGetParamValueUlongProc != NULL)
    {
        g_GetParamValueUlong = pGetParamValueUlongProc;
    }
    else
    {
        goto EXIT;
    }


    pGetParamValueIntProc = (COSAGetParamValueUlongProc)pPlugInfo->AcquireFunction("COSAGetParamValueInt");

    if( pGetParamValueIntProc != NULL)
    {
        g_GetParamValueInt = pGetParamValueIntProc;
    }
    else
    {
        goto EXIT;
    }

    pGetParamValueBoolProc = (COSAGetParamValueBoolProc)pPlugInfo->AcquireFunction("COSAGetParamValueBool");

    if( pGetParamValueBoolProc != NULL)
    {
        g_GetParamValueBool = pGetParamValueBoolProc;
    }
    else
    {
        goto EXIT;
    }

    pSetStringProc = (COSASetParamValueStringProc)pPlugInfo->AcquireFunction("COSASetParamValueString");

    if( pSetStringProc != NULL)
    {
        g_SetParamValueString = pSetStringProc;
    }
    else
    {
        goto EXIT;
    }

    pSetParamValueUlongProc = (COSASetParamValueUlongProc)pPlugInfo->AcquireFunction("COSASetParamValueUlong");

    if( pSetParamValueUlongProc != NULL)
    {
        g_SetParamValueUlong = pSetParamValueUlongProc;
    }
    else
    {
        goto EXIT;
    }


    pSetParamValueIntProc = (COSASetParamValueIntProc)pPlugInfo->AcquireFunction("COSASetParamValueInt");

    if( pSetParamValueIntProc != NULL)
    {
        g_SetParamValueInt = pSetParamValueIntProc;
    }
    else
    {
        goto EXIT;
    }

    pSetParamValueBoolProc = (COSASetParamValueBoolProc)pPlugInfo->AcquireFunction("COSASetParamValueBool");

    if( pSetParamValueBoolProc != NULL)
    {
        g_SetParamValueBool = pSetParamValueBoolProc;
    }
    else
    {
        goto EXIT;
    }

    pGetInstanceNumbersProc = (COSAGetInstanceNumbersProc)pPlugInfo->AcquireFunction("COSAGetInstanceNumbers");

    if( pGetInstanceNumbersProc != NULL)
    {
        g_GetInstanceNumbers = pGetInstanceNumbersProc;
    }
    else
    {
        goto EXIT;
    }

    pValInterfaceProc = (COSAValidateHierarchyInterfaceProc)pPlugInfo->AcquireFunction("COSAValidateHierarchyInterface");

    if ( pValInterfaceProc )
    {
        g_ValidateInterface = pValInterfaceProc;
    }
    else
    {
        goto EXIT;
    }

    pGetRegistryRootFolder = (COSAGetHandleProc)pPlugInfo->AcquireFunction("COSAGetRegistryRootFolder");

    if ( pGetRegistryRootFolder != NULL )
    {
        g_GetRegistryRootFolder = pGetRegistryRootFolder;
    }
    else
    {
        printf("!!! haha, catcha !!!\n");
        goto EXIT;
    }

    pGetInsNumberByIndexProc = (COSAGetInstanceNumberByIndexProc)pPlugInfo->AcquireFunction("COSAGetInstanceNumberByIndex");

    if ( pGetInsNumberByIndexProc != NULL )
    {
        g_GetInstanceNumberByIndex = pGetInsNumberByIndexProc;
    }
    else
    {
        goto EXIT;
    }

    pGetInterfaceByNameProc = (COSAGetInterfaceByNameProc)pPlugInfo->AcquireFunction("COSAGetInterfaceByName");

    if ( pGetInterfaceByNameProc != NULL )
    {
        g_GetInterfaceByName = pGetInterfaceByNameProc;
    }
    else
    {
        goto EXIT;
    }

    g_pPnmCcdIf = g_GetInterfaceByName(g_pDslhDmlAgent, CCSP_CCD_INTERFACE_NAME);

    if ( !g_pPnmCcdIf )
    {
        CcspTraceError(("g_pPnmCcdIf is NULL !\n"));

        goto EXIT;
    }

    g_RegisterCallBackAfterInitDml = (COSARegisterCallBackAfterInitDmlProc)pPlugInfo->AcquireFunction("COSARegisterCallBackAfterInitDml");

    if ( !g_RegisterCallBackAfterInitDml )
    {
        goto EXIT;
    }

    g_COSARepopulateTable = (COSARepopulateTableProc)pPlugInfo->AcquireFunction("COSARepopulateTable");

    if ( !g_COSARepopulateTable )
    {
        goto EXIT;
    }

    /* Get Message Bus Handle */
    g_GetMessageBusHandle = (PFN_CCSPCCDM_APPLY_CHANGES)pPlugInfo->AcquireFunction("COSAGetMessageBusHandle");
    if ( g_GetMessageBusHandle == NULL )
    {
        goto EXIT;
    }

    g_MessageBusHandle = (ANSC_HANDLE)g_GetMessageBusHandle(g_pDslhDmlAgent);
    if ( g_MessageBusHandle == NULL )
    {
        goto EXIT;
    }
    g_MessageBusHandle_Irep = g_MessageBusHandle;

    /* Get Subsystem prefix */
    g_GetSubsystemPrefix = (COSAGetSubsystemPrefixProc)pPlugInfo->AcquireFunction("COSAGetSubsystemPrefix");
    if ( g_GetSubsystemPrefix != NULL )
    {
        char*   tmpSubsystemPrefix;

        if ( tmpSubsystemPrefix = g_GetSubsystemPrefix(g_pDslhDmlAgent) )
        {
            AnscCopyString(g_SubSysPrefix_Irep, tmpSubsystemPrefix);
        }

        /* retrieve the subsystem prefix */
        g_SubsystemPrefix = g_GetSubsystemPrefix(g_pDslhDmlAgent);
    }

    /* Create backend framework */
    g_pTelcoVoiceBEManager = (PCOSA_BACKEND_MANAGER_OBJECT)TelcoVoiceMgr_BackEndManagerCreate();

    if ( g_pTelcoVoiceBEManager && g_pTelcoVoiceBEManager->Initialize )
    {
        g_pTelcoVoiceBEManager->hCosaPluginInfo = pPlugInfo;

        g_pTelcoVoiceBEManager->Initialize   ((ANSC_HANDLE)g_pTelcoVoiceBEManager);
    }
    return  0;

EXIT:

    return -1;

}

int ANSC_EXPORT_API COSA_Async_Init(ULONG uMaxVersionSupported, void* hCosaPlugInfo)
{
    PCOSA_PLUGIN_INFO               pPlugInfo      = (PCOSA_PLUGIN_INFO)hCosaPlugInfo;

    return 0;
}

BOOL ANSC_EXPORT_API TELCOVOICEMGR_IsObjSupported(char* pObjName)
{
    return TRUE;
}

void ANSC_EXPORT_API TELCOVOICEMGR_Unload(void)
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;

    /* unload the memory here */

    returnStatus  =  TelcoVoiceMgr_BackEndManagerRemove(g_pTelcoVoiceBEManager);

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        g_pTelcoVoiceBEManager = NULL;
    }
    else
    {
        /* print error trace*/
        g_pTelcoVoiceBEManager = NULL;
    }
}

void ANSC_EXPORT_API COSA_MemoryCheck(void)
{
    ANSC_STATUS                     returnStatus            = ANSC_STATUS_SUCCESS;
    PCOSA_PLUGIN_INFO               pPlugInfo               = (PCOSA_PLUGIN_INFO)g_pTelcoVoiceBEManager->hCosaPluginInfo;

    /* unload the memory here */

    returnStatus  =  TelcoVoiceMgr_BackEndManagerRemove(g_pTelcoVoiceBEManager);

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        g_pTelcoVoiceBEManager = NULL;
    }
    else
    {
        g_pTelcoVoiceBEManager = NULL;
    }

    COSA_MemoryUsage();
    COSA_MemoryTable();

    g_pTelcoVoiceBEManager = (PCOSA_BACKEND_MANAGER_OBJECT)TelcoVoiceMgr_BackEndManagerCreate();

    if ( g_pTelcoVoiceBEManager && g_pTelcoVoiceBEManager->Initialize )
    {
        g_pTelcoVoiceBEManager->hCosaPluginInfo = pPlugInfo;

        g_pTelcoVoiceBEManager->Initialize   ((ANSC_HANDLE)g_pTelcoVoiceBEManager);
    }
}

void ANSC_EXPORT_API COSA_MemoryUsage(void)
{
    /*AnscTraceMemoryUsage();*/
}

void ANSC_EXPORT_API COSA_MemoryTable(void)
{
    /*CcspTraceMemoryTable();*/
}
