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
#include "telcovoicemgr_dml_report.h"
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

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_IsUpdated",VoiceService_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_Synchronize",VoiceService_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetEntryCount",VoiceService_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetEntry",VoiceService_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetParamUlongValue",VoiceService_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_SetParamUlongValue",VoiceService_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetParamBoolValue",VoiceService_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_SetParamBoolValue",VoiceService_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_GetParamStringValue",VoiceService_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_SetParamStringValue",VoiceService_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_Validate",VoiceService_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_Commit",VoiceService_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "VoiceService_Rollback",VoiceService_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_GetParamStringValue",X_RDK_Debug_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_GetParamUlongValue",X_RDK_Debug_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_SetParamStringValue",X_RDK_Debug_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_SetParamUlongValue",X_RDK_Debug_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_Validate",X_RDK_Debug_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_Commit",X_RDK_Debug_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_Rollback",X_RDK_Debug_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_GetParamUlongValue",TelcoVoiceMgrDml_capabilities_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_GetParamStringValue",TelcoVoiceMgrDml_capabilities_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_GetParamIntValue",TelcoVoiceMgrDml_capabilities_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_SIP_Client_GetParamUlongValue",TelcoVoiceMgrDml_capabilities_SIP_Client_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_SIP_Client_GetParamStringValue",TelcoVoiceMgrDml_capabilities_SIP_Client_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_SIP_Registrar_GetParamUlongValue",TelcoVoiceMgrDml_capabilities_SIP_Registrar_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_SIP_Registrar_GetParamStringValue",TelcoVoiceMgrDml_capabilities_SIP_Registrar_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_MGCP_GetParamStringValue",TelcoVoiceMgrDml_capabilities_MGCP_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_H323_GetParamUlongValue",TelcoVoiceMgrDml_capabilities_H323_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_H323_GetParamBoolValue",TelcoVoiceMgrDml_capabilities_H323_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_ISDN_GetParamBoolValue",TelcoVoiceMgrDml_capabilities_ISDN_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_POTS_GetParamUlongValue",TelcoVoiceMgrDml_capabilities_POTS_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_POTS_GetParamStringValue",TelcoVoiceMgrDml_capabilities_POTS_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_POTS_GetParamBoolValue",TelcoVoiceMgrDml_capabilities_POTS_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_IsUpdated",TelcoVoiceMgrDml_capabilities_CodecList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_Synchronize",TelcoVoiceMgrDml_capabilities_CodecList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_GetEntryCount",TelcoVoiceMgrDml_capabilities_CodecList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_GetEntry",TelcoVoiceMgrDml_capabilities_CodecList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_GetParamUlongValue",TelcoVoiceMgrDml_capabilities_CodecList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_GetParamStringValue",TelcoVoiceMgrDml_capabilities_CodecList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_SetParamStringValue",TelcoVoiceMgrDml_capabilities_CodecList_SetParamStringValue   );
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_GetParamBoolValue",TelcoVoiceMgrDml_capabilities_CodecList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_Validate",TelcoVoiceMgrDml_capabilities_CodecList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_Commit",TelcoVoiceMgrDml_capabilities_CodecList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_CodecList_Rollback",TelcoVoiceMgrDml_capabilities_CodecList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_QualityIndicator_GetParamUlongValue",TelcoVoiceMgrDml_capabilities_QualityIndicator_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_capabilities_QualityIndicator_GetParamStringValue",TelcoVoiceMgrDml_capabilities_QualityIndicator_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ReservedPorts_GetParamStringValue",TelcoVoiceMgrDml_ReservedPorts_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ReservedPorts_SetParamStringValue",TelcoVoiceMgrDml_ReservedPorts_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_IsUpdated",TelcoVoiceMgrDml_ISDN_BRIList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_Synchronize",TelcoVoiceMgrDml_ISDN_BRIList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_GetEntryCount",TelcoVoiceMgrDml_ISDN_BRIList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_GetEntry",TelcoVoiceMgrDml_ISDN_BRIList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_GetParamUlongValue",TelcoVoiceMgrDml_ISDN_BRIList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_SetParamUlongValue",TelcoVoiceMgrDml_ISDN_BRIList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_GetParamStringValue",TelcoVoiceMgrDml_ISDN_BRIList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_SetParamStringValue",TelcoVoiceMgrDml_ISDN_BRIList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_GetParamBoolValue",TelcoVoiceMgrDml_ISDN_BRIList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_SetParamBoolValue",TelcoVoiceMgrDml_ISDN_BRIList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_Validate",TelcoVoiceMgrDml_ISDN_BRIList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_Commit",TelcoVoiceMgrDml_ISDN_BRIList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_BRIList_Rollback",TelcoVoiceMgrDml_ISDN_BRIList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_IsUpdated",TelcoVoiceMgrDml_ISDN_PRIList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_Synchronize",TelcoVoiceMgrDml_ISDN_PRIList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_GetEntryCount",TelcoVoiceMgrDml_ISDN_PRIList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_GetEntry",TelcoVoiceMgrDml_ISDN_PRIList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_GetParamUlongValue",TelcoVoiceMgrDml_ISDN_PRIList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_SetParamUlongValue",TelcoVoiceMgrDml_ISDN_PRIList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_GetParamStringValue",TelcoVoiceMgrDml_ISDN_PRIList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_SetParamStringValue",TelcoVoiceMgrDml_ISDN_PRIList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_GetParamBoolValue",TelcoVoiceMgrDml_ISDN_PRIList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_SetParamBoolValue",TelcoVoiceMgrDml_ISDN_PRIList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_Validate",TelcoVoiceMgrDml_ISDN_PRIList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_Commit",TelcoVoiceMgrDml_ISDN_PRIList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ISDN_PRIList_Rollback",TelcoVoiceMgrDml_ISDN_PRIList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_GetParamStringValue",TelcoVoiceMgrDml_POTS_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_SetParamStringValue",TelcoVoiceMgrDml_POTS_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_IsUpdated",TelcoVoiceMgrDml_POTS_FXOList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_Synchronize",TelcoVoiceMgrDml_POTS_FXOList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_GetEntryCount",TelcoVoiceMgrDml_POTS_FXOList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_GetEntry",TelcoVoiceMgrDml_POTS_FXOList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_GetParamUlongValue",TelcoVoiceMgrDml_POTS_FXOList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_SetParamUlongValue",TelcoVoiceMgrDml_POTS_FXOList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_GetParamStringValue",TelcoVoiceMgrDml_POTS_FXOList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_SetParamStringValue",TelcoVoiceMgrDml_POTS_FXOList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_GetParamBoolValue",TelcoVoiceMgrDml_POTS_FXOList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_SetParamBoolValue",TelcoVoiceMgrDml_POTS_FXOList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_Validate",TelcoVoiceMgrDml_POTS_FXOList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_Commit",TelcoVoiceMgrDml_POTS_FXOList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_Rollback",TelcoVoiceMgrDml_POTS_FXOList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_DiagTests_GetParamUlongValue",TelcoVoiceMgrDml_POTS_FXOList_DiagTests_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXOList_DiagTests_SetParamUlongValue",TelcoVoiceMgrDml_POTS_FXOList_DiagTests_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_IsUpdated",TelcoVoiceMgrDml_POTS_FXSList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_Synchronize",TelcoVoiceMgrDml_POTS_FXSList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_GetEntryCount",TelcoVoiceMgrDml_POTS_FXSList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_GetEntry",TelcoVoiceMgrDml_POTS_FXSList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_GetParamUlongValue",TelcoVoiceMgrDml_POTS_FXSList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_SetParamUlongValue",TelcoVoiceMgrDml_POTS_FXSList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_GetParamStringValue",TelcoVoiceMgrDml_POTS_FXSList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_SetParamStringValue",TelcoVoiceMgrDml_POTS_FXSList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_GetParamBoolValue",TelcoVoiceMgrDml_POTS_FXSList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_SetParamBoolValue",TelcoVoiceMgrDml_POTS_FXSList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_Validate",TelcoVoiceMgrDml_POTS_FXSList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_Commit",TelcoVoiceMgrDml_POTS_FXSList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_Rollback",TelcoVoiceMgrDml_POTS_FXSList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamUlongValue",TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamIntValue",TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamIntValue",TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamBoolValue",TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamBoolValue",TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_DiagTests_GetParamUlongValue",TelcoVoiceMgrDml_POTS_FXSList_DiagTests_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_FXSList_DiagTests_SetParamUlongValue",TelcoVoiceMgrDml_POTS_FXSList_DiagTests_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_IsUpdated",TelcoVoiceMgrDml_POTS_Ringer_EventList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_Synchronize",TelcoVoiceMgrDml_POTS_Ringer_EventList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntryCount",TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntry",TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamUlongValue",TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamStringValue",TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_SetParamStringValue",TelcoVoiceMgrDml_POTS_Ringer_EventList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_Validate",TelcoVoiceMgrDml_POTS_Ringer_EventList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_Commit",TelcoVoiceMgrDml_POTS_Ringer_EventList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_POTS_Ringer_EventList_Rollback",TelcoVoiceMgrDml_POTS_Ringer_EventList_Rollback);

 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_IsUpdated",TelcoVoiceMgrDml_DECT_BaseList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_Synchronize",TelcoVoiceMgrDml_DECT_BaseList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_GetEntryCount",TelcoVoiceMgrDml_DECT_BaseList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_GetEntry",TelcoVoiceMgrDml_DECT_BaseList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_GetParamUlongValue",TelcoVoiceMgrDml_DECT_BaseList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_SetParamUlongValue",TelcoVoiceMgrDml_DECT_BaseList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_GetParamStringValue",TelcoVoiceMgrDml_DECT_BaseList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_SetParamStringValue",TelcoVoiceMgrDml_DECT_BaseList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_GetParamBoolValue",TelcoVoiceMgrDml_DECT_BaseList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_SetParamBoolValue",TelcoVoiceMgrDml_DECT_BaseList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_Validate",TelcoVoiceMgrDml_DECT_BaseList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_Commit",TelcoVoiceMgrDml_DECT_BaseList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_Rollback",TelcoVoiceMgrDml_DECT_BaseList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_BaseList_Stats_GetParamUlongValue",TelcoVoiceMgrDml_DECT_BaseList_Stats_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_IsUpdated",TelcoVoiceMgrDml_DECT_PortableList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_Synchronize",TelcoVoiceMgrDml_DECT_PortableList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_GetEntryCount",TelcoVoiceMgrDml_DECT_PortableList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_GetEntry",TelcoVoiceMgrDml_DECT_PortableList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_GetParamUlongValue",TelcoVoiceMgrDml_DECT_PortableList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_SetParamUlongValue",TelcoVoiceMgrDml_DECT_PortableList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_GetParamStringValue",TelcoVoiceMgrDml_DECT_PortableList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_SetParamStringValue",TelcoVoiceMgrDml_DECT_PortableList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_GetParamBoolValue",TelcoVoiceMgrDml_DECT_PortableList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_SetParamBoolValue",TelcoVoiceMgrDml_DECT_PortableList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_Validate",TelcoVoiceMgrDml_DECT_PortableList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_Commit",TelcoVoiceMgrDml_DECT_PortableList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_DECT_PortableList_Rollback",TelcoVoiceMgrDml_DECT_PortableList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_IsUpdated",TelcoVoiceMgrDml_SIP_ClientList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_Synchronize",TelcoVoiceMgrDml_SIP_ClientList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_GetEntryCount",TelcoVoiceMgrDml_SIP_ClientList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_GetEntry",TelcoVoiceMgrDml_SIP_ClientList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_ClientList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_SetParamUlongValue",TelcoVoiceMgrDml_SIP_ClientList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_GetParamStringValue",TelcoVoiceMgrDml_SIP_ClientList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_SetParamStringValue",TelcoVoiceMgrDml_SIP_ClientList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_ClientList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_ClientList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_Validate",TelcoVoiceMgrDml_SIP_ClientList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_Commit",TelcoVoiceMgrDml_SIP_ClientList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_Rollback",TelcoVoiceMgrDml_SIP_ClientList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_IsUpdated",TelcoVoiceMgrDml_SIP_ClientList_ContactList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_Synchronize",TelcoVoiceMgrDml_SIP_ClientList_ContactList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetEntryCount",TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetEntry",TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_SetParamUlongValue",TelcoVoiceMgrDml_SIP_ClientList_ContactList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetParamStringValue",TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_SetParamStringValue",TelcoVoiceMgrDml_SIP_ClientList_ContactList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_ClientList_ContactList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_Validate",TelcoVoiceMgrDml_SIP_ClientList_ContactList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_Commit",TelcoVoiceMgrDml_SIP_ClientList_ContactList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_ContactList_Rollback",TelcoVoiceMgrDml_SIP_ClientList_ContactList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_IsUpdated",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Synchronize",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetEntryCount",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetEntry",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetParamStringValue",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_SetParamStringValue",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Validate",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Commit",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Rollback",TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_IsUpdated",TelcoVoiceMgrDml_SIP_NetworkList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_Synchronize",TelcoVoiceMgrDml_SIP_NetworkList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_GetEntryCount",TelcoVoiceMgrDml_SIP_NetworkList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_GetEntry",TelcoVoiceMgrDml_SIP_NetworkList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_NetworkList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue",TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_GetParamStringValue",TelcoVoiceMgrDml_SIP_NetworkList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_SetParamStringValue",TelcoVoiceMgrDml_SIP_NetworkList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_GetParamIntValue",TelcoVoiceMgrDml_SIP_NetworkList_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_SetParamIntValue",TelcoVoiceMgrDml_SIP_NetworkList_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_NetworkList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_NetworkList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_Validate",TelcoVoiceMgrDml_SIP_NetworkList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_Commit",TelcoVoiceMgrDml_SIP_NetworkList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_Rollback",TelcoVoiceMgrDml_SIP_NetworkList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_IsUpdated",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Synchronize",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetEntryCount",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetEntry",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_SetParamUlongValue",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetParamStringValue",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_SetParamStringValue",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Validate",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Commit",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Rollback",TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_IsUpdated",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Synchronize",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetEntryCount",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetEntry",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_SetParamUlongValue",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetParamStringValue",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_SetParamStringValue",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Validate",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Commit",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Rollback",TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_IsUpdated",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Synchronize",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetEntryCount",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetEntry",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_SetParamUlongValue",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetParamStringValue",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_SetParamStringValue",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Validate",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Commit",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Rollback",TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_IsUpdated",TelcoVoiceMgrDml_SIP_ProxyList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_Synchronize",TelcoVoiceMgrDml_SIP_ProxyList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_GetEntryCount",TelcoVoiceMgrDml_SIP_ProxyList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_GetEntry",TelcoVoiceMgrDml_SIP_ProxyList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_ProxyList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_SetParamUlongValue",TelcoVoiceMgrDml_SIP_ProxyList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_GetParamStringValue",TelcoVoiceMgrDml_SIP_ProxyList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_SetParamStringValue",TelcoVoiceMgrDml_SIP_ProxyList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_ProxyList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_ProxyList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_Validate",TelcoVoiceMgrDml_SIP_ProxyList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_Commit",TelcoVoiceMgrDml_SIP_ProxyList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_ProxyList_Rollback",TelcoVoiceMgrDml_SIP_ProxyList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_IsUpdated",TelcoVoiceMgrDml_SIP_RegistrarList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_Synchronize",TelcoVoiceMgrDml_SIP_RegistrarList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_GetEntryCount",TelcoVoiceMgrDml_SIP_RegistrarList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_GetEntry",TelcoVoiceMgrDml_SIP_RegistrarList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_RegistrarList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_SetParamUlongValue",TelcoVoiceMgrDml_SIP_RegistrarList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_GetParamStringValue",TelcoVoiceMgrDml_SIP_RegistrarList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_SetParamStringValue",TelcoVoiceMgrDml_SIP_RegistrarList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_RegistrarList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_RegistrarList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_Validate",TelcoVoiceMgrDml_SIP_RegistrarList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_Commit",TelcoVoiceMgrDml_SIP_RegistrarList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_Rollback",TelcoVoiceMgrDml_SIP_RegistrarList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_IsUpdated",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Synchronize",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetEntryCount",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetEntry",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_SetParamUlongValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetParamStringValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_SetParamStringValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Validate",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Commit",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Rollback",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_IsUpdated",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Synchronize",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetEntryCount",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetEntry",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetParamUlongValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetParamStringValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_SetParamStringValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetParamBoolValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_SetParamBoolValue",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Validate",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Commit",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Rollback",TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_IsUpdated",TelcoVoiceMgrDml_MGCP_ClientList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_Synchronize",TelcoVoiceMgrDml_MGCP_ClientList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_GetEntryCount",TelcoVoiceMgrDml_MGCP_ClientList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_GetEntry",TelcoVoiceMgrDml_MGCP_ClientList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_GetParamUlongValue",TelcoVoiceMgrDml_MGCP_ClientList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_SetParamUlongValue",TelcoVoiceMgrDml_MGCP_ClientList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_GetParamStringValue",TelcoVoiceMgrDml_MGCP_ClientList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_SetParamStringValue",TelcoVoiceMgrDml_MGCP_ClientList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_GetParamBoolValue",TelcoVoiceMgrDml_MGCP_ClientList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_SetParamBoolValue",TelcoVoiceMgrDml_MGCP_ClientList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_Validate",TelcoVoiceMgrDml_MGCP_ClientList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_Commit",TelcoVoiceMgrDml_MGCP_ClientList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_ClientList_Rollback",TelcoVoiceMgrDml_MGCP_ClientList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_IsUpdated",TelcoVoiceMgrDml_MGCP_NetworkList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_Synchronize",TelcoVoiceMgrDml_MGCP_NetworkList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_GetEntryCount",TelcoVoiceMgrDml_MGCP_NetworkList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_GetEntry",TelcoVoiceMgrDml_MGCP_NetworkList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_GetParamUlongValue",TelcoVoiceMgrDml_MGCP_NetworkList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_SetParamUlongValue",TelcoVoiceMgrDml_MGCP_NetworkList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_GetParamStringValue",TelcoVoiceMgrDml_MGCP_NetworkList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_SetParamStringValue",TelcoVoiceMgrDml_MGCP_NetworkList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_GetParamIntValue",TelcoVoiceMgrDml_MGCP_NetworkList_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_SetParamIntValue",TelcoVoiceMgrDml_MGCP_NetworkList_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_GetParamBoolValue",TelcoVoiceMgrDml_MGCP_NetworkList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_SetParamBoolValue",TelcoVoiceMgrDml_MGCP_NetworkList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_Validate",TelcoVoiceMgrDml_MGCP_NetworkList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_Commit",TelcoVoiceMgrDml_MGCP_NetworkList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_MGCP_NetworkList_Rollback",TelcoVoiceMgrDml_MGCP_NetworkList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_IsUpdated",TelcoVoiceMgrDml_H323_ClientList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_Synchronize",TelcoVoiceMgrDml_H323_ClientList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_GetEntryCount",TelcoVoiceMgrDml_H323_ClientList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_GetEntry",TelcoVoiceMgrDml_H323_ClientList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_GetParamUlongValue",TelcoVoiceMgrDml_H323_ClientList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_SetParamUlongValue",TelcoVoiceMgrDml_H323_ClientList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_GetParamStringValue",TelcoVoiceMgrDml_H323_ClientList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_SetParamStringValue",TelcoVoiceMgrDml_H323_ClientList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_GetParamBoolValue",TelcoVoiceMgrDml_H323_ClientList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_SetParamBoolValue",TelcoVoiceMgrDml_H323_ClientList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_Validate",TelcoVoiceMgrDml_H323_ClientList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_Commit",TelcoVoiceMgrDml_H323_ClientList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_ClientList_Rollback",TelcoVoiceMgrDml_H323_ClientList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_IsUpdated",TelcoVoiceMgrDml_H323_NetworkList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_Synchronize",TelcoVoiceMgrDml_H323_NetworkList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_GetEntryCount",TelcoVoiceMgrDml_H323_NetworkList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_GetEntry",TelcoVoiceMgrDml_H323_NetworkList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_GetParamUlongValue",TelcoVoiceMgrDml_H323_NetworkList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_SetParamUlongValue",TelcoVoiceMgrDml_H323_NetworkList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_GetParamStringValue",TelcoVoiceMgrDml_H323_NetworkList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_SetParamStringValue",TelcoVoiceMgrDml_H323_NetworkList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_GetParamIntValue",TelcoVoiceMgrDml_H323_NetworkList_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_SetParamIntValue",TelcoVoiceMgrDml_H323_NetworkList_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_GetParamBoolValue",TelcoVoiceMgrDml_H323_NetworkList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_SetParamBoolValue",TelcoVoiceMgrDml_H323_NetworkList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_Validate",TelcoVoiceMgrDml_H323_NetworkList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_Commit",TelcoVoiceMgrDml_H323_NetworkList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_H323_NetworkList_Rollback",TelcoVoiceMgrDml_H323_NetworkList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_IsUpdated",TelcoVoiceMgrDml_TrunkList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_Synchronize",TelcoVoiceMgrDml_TrunkList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_GetEntryCount",TelcoVoiceMgrDml_TrunkList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_GetEntry",TelcoVoiceMgrDml_TrunkList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_GetParamUlongValue",TelcoVoiceMgrDml_TrunkList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_SetParamUlongValue",TelcoVoiceMgrDml_TrunkList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_GetParamStringValue",TelcoVoiceMgrDml_TrunkList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_SetParamStringValue",TelcoVoiceMgrDml_TrunkList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_GetParamIntValue",TelcoVoiceMgrDml_TrunkList_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_SetParamIntValue",TelcoVoiceMgrDml_TrunkList_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_GetParamBoolValue",TelcoVoiceMgrDml_TrunkList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_SetParamBoolValue",TelcoVoiceMgrDml_TrunkList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_Validate",TelcoVoiceMgrDml_TrunkList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_Commit",TelcoVoiceMgrDml_TrunkList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TrunkList_Rollback",TelcoVoiceMgrDml_TrunkList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_IsUpdated",TelcoVoiceMgrDml_CallControl_LineList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_Synchronize",TelcoVoiceMgrDml_CallControl_LineList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_GetEntryCount",TelcoVoiceMgrDml_CallControl_LineList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_GetEntry",TelcoVoiceMgrDml_CallControl_LineList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_LineList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_LineList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_LineList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_LineList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_LineList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_Validate",TelcoVoiceMgrDml_CallControl_LineList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_Commit",TelcoVoiceMgrDml_CallControl_LineList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_Rollback",TelcoVoiceMgrDml_CallControl_LineList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_stats_InCalls_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_LineList_stats_InCalls_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_stats_OutCalls_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_LineList_stats_OutCalls_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_stats_RTP_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_LineList_stats_RTP_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_LineList_stats_DSP_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_LineList_stats_DSP_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_IsUpdated",TelcoVoiceMgrDml_CallControl_ExtensionList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_Synchronize",TelcoVoiceMgrDml_CallControl_ExtensionList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_GetEntryCount",TelcoVoiceMgrDml_CallControl_ExtensionList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_GetEntry",TelcoVoiceMgrDml_CallControl_ExtensionList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_ExtensionList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_ExtensionList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_ExtensionList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_ExtensionList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_ExtensionList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_Validate",TelcoVoiceMgrDml_CallControl_ExtensionList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_Commit",TelcoVoiceMgrDml_CallControl_ExtensionList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_Rollback",TelcoVoiceMgrDml_CallControl_ExtensionList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_InCalls_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_InCalls_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_OutCalls_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_OutCalls_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_RTP_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_RTP_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_DSP_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_DSP_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_IsUpdated",TelcoVoiceMgrDml_CallControl_GroupList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_Synchronize",TelcoVoiceMgrDml_CallControl_GroupList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_GetEntryCount",TelcoVoiceMgrDml_CallControl_GroupList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_GetEntry",TelcoVoiceMgrDml_CallControl_GroupList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_GroupList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_GroupList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_GroupList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_GroupList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_Validate",TelcoVoiceMgrDml_CallControl_GroupList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_Commit",TelcoVoiceMgrDml_CallControl_GroupList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_GroupList_Rollback",TelcoVoiceMgrDml_CallControl_GroupList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_IsUpdated",TelcoVoiceMgrDml_CallControl_MailboxList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_Synchronize",TelcoVoiceMgrDml_CallControl_MailboxList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_GetEntryCount",TelcoVoiceMgrDml_CallControl_MailboxList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_GetEntry",TelcoVoiceMgrDml_CallControl_MailboxList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_MailboxList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_MailboxList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_MailboxList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_MailboxList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_MailboxList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_MailboxList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_Validate",TelcoVoiceMgrDml_CallControl_MailboxList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_Commit",TelcoVoiceMgrDml_CallControl_MailboxList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_MailboxList_Rollback",TelcoVoiceMgrDml_CallControl_MailboxList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_IsUpdated",TelcoVoiceMgrDml_CallControl_IncomingMapList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_Synchronize",TelcoVoiceMgrDml_CallControl_IncomingMapList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_GetEntryCount",TelcoVoiceMgrDml_CallControl_IncomingMapList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_GetEntry",TelcoVoiceMgrDml_CallControl_IncomingMapList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_IncomingMapList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_IncomingMapList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_IncomingMapList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_IncomingMapList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_IncomingMapList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_IncomingMapList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_Validate",TelcoVoiceMgrDml_CallControl_IncomingMapList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_Commit",TelcoVoiceMgrDml_CallControl_IncomingMapList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_IncomingMapList_Rollback",TelcoVoiceMgrDml_CallControl_IncomingMapList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_IsUpdated",TelcoVoiceMgrDml_CallControl_OutgoingMapList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_Synchronize",TelcoVoiceMgrDml_CallControl_OutgoingMapList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetEntryCount",TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetEntry",TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_OutgoingMapList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_OutgoingMapList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_OutgoingMapList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_Validate",TelcoVoiceMgrDml_CallControl_OutgoingMapList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_Commit",TelcoVoiceMgrDml_CallControl_OutgoingMapList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_OutgoingMapList_Rollback",TelcoVoiceMgrDml_CallControl_OutgoingMapList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_IsUpdated",TelcoVoiceMgrDml_CallControl_NumberingPlanList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_Synchronize",TelcoVoiceMgrDml_CallControl_NumberingPlanList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetEntryCount",TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetEntry",TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_Validate",TelcoVoiceMgrDml_CallControl_NumberingPlanList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_Commit",TelcoVoiceMgrDml_CallControl_NumberingPlanList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_Rollback",TelcoVoiceMgrDml_CallControl_NumberingPlanList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_IsUpdated",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Synchronize",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetEntryCount",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetEntry",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Validate",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Commit",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Rollback",TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_IsUpdated",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Synchronize",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetEntryCount",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetEntry",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Validate",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Commit",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Rollback",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_IsUpdated",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Synchronize",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetEntryCount",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetEntry",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Validate",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Commit",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Rollback",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_IsUpdated",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Synchronize",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetEntryCount",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetEntry",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Validate",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Commit",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Rollback",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_IsUpdated",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Synchronize",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetEntryCount",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetEntry",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Validate",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Commit",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Rollback",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_IsUpdated",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Synchronize",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetEntryCount",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetEntry",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_SetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Validate",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Commit",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Rollback",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_GetParamUlongValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_SetParamUlongValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_GetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_SetParamStringValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_GetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_SetParamBoolValue",TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_IsUpdated",TelcoVoiceMgrDml_InterworkList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_Synchronize",TelcoVoiceMgrDml_InterworkList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_GetEntryCount",TelcoVoiceMgrDml_InterworkList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_GetEntry",TelcoVoiceMgrDml_InterworkList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_GetParamUlongValue",TelcoVoiceMgrDml_InterworkList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_SetParamUlongValue",TelcoVoiceMgrDml_InterworkList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_GetParamStringValue",TelcoVoiceMgrDml_InterworkList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_SetParamStringValue",TelcoVoiceMgrDml_InterworkList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_GetParamBoolValue",TelcoVoiceMgrDml_InterworkList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_SetParamBoolValue",TelcoVoiceMgrDml_InterworkList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_Validate",TelcoVoiceMgrDml_InterworkList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_Commit",TelcoVoiceMgrDml_InterworkList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_Rollback",TelcoVoiceMgrDml_InterworkList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_IsUpdated",TelcoVoiceMgrDml_InterworkList_UIList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_Synchronize",TelcoVoiceMgrDml_InterworkList_UIList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_GetEntryCount",TelcoVoiceMgrDml_InterworkList_UIList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_GetEntry",TelcoVoiceMgrDml_InterworkList_UIList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_GetParamStringValue",TelcoVoiceMgrDml_InterworkList_UIList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_SetParamStringValue",TelcoVoiceMgrDml_InterworkList_UIList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_GetParamBoolValue",TelcoVoiceMgrDml_InterworkList_UIList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_SetParamBoolValue",TelcoVoiceMgrDml_InterworkList_UIList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_Validate",TelcoVoiceMgrDml_InterworkList_UIList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_Commit",TelcoVoiceMgrDml_InterworkList_UIList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_UIList_Rollback",TelcoVoiceMgrDml_InterworkList_UIList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_IsUpdated",TelcoVoiceMgrDml_InterworkList_MapList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_Synchronize",TelcoVoiceMgrDml_InterworkList_MapList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_GetEntryCount",TelcoVoiceMgrDml_InterworkList_MapList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_GetEntry",TelcoVoiceMgrDml_InterworkList_MapList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_GetParamUlongValue",TelcoVoiceMgrDml_InterworkList_MapList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_SetParamUlongValue",TelcoVoiceMgrDml_InterworkList_MapList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_GetParamStringValue",TelcoVoiceMgrDml_InterworkList_MapList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_SetParamStringValue",TelcoVoiceMgrDml_InterworkList_MapList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_GetParamBoolValue",TelcoVoiceMgrDml_InterworkList_MapList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_SetParamBoolValue",TelcoVoiceMgrDml_InterworkList_MapList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_Validate",TelcoVoiceMgrDml_InterworkList_MapList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_Commit",TelcoVoiceMgrDml_InterworkList_MapList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_InterworkList_MapList_Rollback",TelcoVoiceMgrDml_InterworkList_MapList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_IsUpdated",TelcoVoiceMgrDml_CallLogList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_Synchronize",TelcoVoiceMgrDml_CallLogList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_GetEntryCount",TelcoVoiceMgrDml_CallLogList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_GetEntry",TelcoVoiceMgrDml_CallLogList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SetParamStringValue",TelcoVoiceMgrDml_CallLogList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_Validate",TelcoVoiceMgrDml_CallLogList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_Commit",TelcoVoiceMgrDml_CallLogList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_Rollback",TelcoVoiceMgrDml_CallLogList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_IsUpdated",TelcoVoiceMgrDml_CallLogList_SignalPerfList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_Synchronize",TelcoVoiceMgrDml_CallLogList_SignalPerfList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntryCount",TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntry",TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamIntValue",TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamIntValue",TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_Validate",TelcoVoiceMgrDml_CallLogList_SignalPerfList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_Commit",TelcoVoiceMgrDml_CallLogList_SignalPerfList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SignalPerfList_Rollback",TelcoVoiceMgrDml_CallLogList_SignalPerfList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_IsUpdated",TelcoVoiceMgrDml_CallLogList_SessionList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Synchronize",TelcoVoiceMgrDml_CallLogList_SessionList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_GetEntryCount",TelcoVoiceMgrDml_CallLogList_SessionList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_GetEntry",TelcoVoiceMgrDml_CallLogList_SessionList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SessionList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_SessionList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Validate",TelcoVoiceMgrDml_CallLogList_SessionList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Commit",TelcoVoiceMgrDml_CallLogList_SessionList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Rollback",TelcoVoiceMgrDml_CallLogList_SessionList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamIntValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamBoolValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamBoolValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_VoiceQuality_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_VoiceQuality_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_source_VoiceQuality_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SessionList_source_VoiceQuality_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamIntValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamBoolValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamUlongValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamBoolValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CallLogList_SessionList_Dest_VoiceQuality_GetParamStringValue",TelcoVoiceMgrDml_CallLogList_SessionList_Dest_VoiceQuality_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_IsUpdated",TelcoVoiceMgrDml_VoipProfileList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_Synchronize",TelcoVoiceMgrDml_VoipProfileList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_GetEntryCount",TelcoVoiceMgrDml_VoipProfileList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_GetEntry",TelcoVoiceMgrDml_VoipProfileList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_GetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_SetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_GetParamStringValue",TelcoVoiceMgrDml_VoipProfileList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_SetParamStringValue",TelcoVoiceMgrDml_VoipProfileList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_GetParamBoolValue",TelcoVoiceMgrDml_VoipProfileList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_SetParamBoolValue",TelcoVoiceMgrDml_VoipProfileList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_Validate",TelcoVoiceMgrDml_VoipProfileList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_Commit",TelcoVoiceMgrDml_VoipProfileList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_Rollback",TelcoVoiceMgrDml_VoipProfileList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamStringValue",TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamIntValue",TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamIntValue",TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamStringValue",TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamStringValue",TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamBoolValue",TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamBoolValue",TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamStringValue",TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamStringValue",TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamBoolValue",TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamBoolValue",TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamIntValue",TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamIntValue",TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamBoolValue",TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamBoolValue",TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_FaxT38_GetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_FaxT38_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_VoipProfileList_FaxT38_SetParamUlongValue",TelcoVoiceMgrDml_VoipProfileList_FaxT38_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_IsUpdated",TelcoVoiceMgrDml_CodecProfileList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_Synchronize",TelcoVoiceMgrDml_CodecProfileList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_GetEntryCount",TelcoVoiceMgrDml_CodecProfileList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_GetEntry",TelcoVoiceMgrDml_CodecProfileList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_GetParamStringValue",TelcoVoiceMgrDml_CodecProfileList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_SetParamStringValue",TelcoVoiceMgrDml_CodecProfileList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_GetParamBoolValue",TelcoVoiceMgrDml_CodecProfileList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_SetParamBoolValue",TelcoVoiceMgrDml_CodecProfileList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_Validate",TelcoVoiceMgrDml_CodecProfileList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_Commit",TelcoVoiceMgrDml_CodecProfileList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_CodecProfileList_Rollback",TelcoVoiceMgrDml_CodecProfileList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_GetParamStringValue",TelcoVoiceMgrDml_Tone_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_SetParamStringValue",TelcoVoiceMgrDml_Tone_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_IsUpdated",TelcoVoiceMgrDml_Tone_DescList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_Synchronize",TelcoVoiceMgrDml_Tone_DescList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_GetEntryCount",TelcoVoiceMgrDml_Tone_DescList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_GetEntry",TelcoVoiceMgrDml_Tone_DescList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_GetParamUlongValue",TelcoVoiceMgrDml_Tone_DescList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_SetParamUlongValue",TelcoVoiceMgrDml_Tone_DescList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_GetParamStringValue",TelcoVoiceMgrDml_Tone_DescList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_SetParamStringValue",TelcoVoiceMgrDml_Tone_DescList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_GetParamBoolValue",TelcoVoiceMgrDml_Tone_DescList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_SetParamBoolValue",TelcoVoiceMgrDml_Tone_DescList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_Validate",TelcoVoiceMgrDml_Tone_DescList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_Commit",TelcoVoiceMgrDml_Tone_DescList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_DescList_Rollback",TelcoVoiceMgrDml_Tone_DescList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_IsUpdated",TelcoVoiceMgrDml_Tone_PatternList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_Synchronize",TelcoVoiceMgrDml_Tone_PatternList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_GetEntryCount",TelcoVoiceMgrDml_Tone_PatternList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_GetEntry",TelcoVoiceMgrDml_Tone_PatternList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_GetParamUlongValue",TelcoVoiceMgrDml_Tone_PatternList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_SetParamUlongValue",TelcoVoiceMgrDml_Tone_PatternList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_GetParamStringValue",TelcoVoiceMgrDml_Tone_PatternList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_SetParamStringValue",TelcoVoiceMgrDml_Tone_PatternList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_GetParamIntValue",TelcoVoiceMgrDml_Tone_PatternList_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_SetParamIntValue",TelcoVoiceMgrDml_Tone_PatternList_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_GetParamBoolValue",TelcoVoiceMgrDml_Tone_PatternList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_SetParamBoolValue",TelcoVoiceMgrDml_Tone_PatternList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_Validate",TelcoVoiceMgrDml_Tone_PatternList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_Commit",TelcoVoiceMgrDml_Tone_PatternList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_PatternList_Rollback",TelcoVoiceMgrDml_Tone_PatternList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_IsUpdated",TelcoVoiceMgrDml_Tone_EventProfileList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_Synchronize",TelcoVoiceMgrDml_Tone_EventProfileList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_GetEntryCount",TelcoVoiceMgrDml_Tone_EventProfileList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_GetEntry",TelcoVoiceMgrDml_Tone_EventProfileList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_GetParamStringValue",TelcoVoiceMgrDml_Tone_EventProfileList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_SetParamStringValue",TelcoVoiceMgrDml_Tone_EventProfileList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_Validate",TelcoVoiceMgrDml_Tone_EventProfileList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_Commit",TelcoVoiceMgrDml_Tone_EventProfileList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_Rollback",TelcoVoiceMgrDml_Tone_EventProfileList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_IsUpdated",TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Synchronize",TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntryCount",TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntry",TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetParamStringValue",TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_SetParamStringValue",TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Validate",TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Commit",TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Rollback",TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_IsUpdated",TelcoVoiceMgrDml_TerminalList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Synchronize",TelcoVoiceMgrDml_TerminalList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_GetEntryCount",TelcoVoiceMgrDml_TerminalList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_GetEntry",TelcoVoiceMgrDml_TerminalList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_GetParamUlongValue",TelcoVoiceMgrDml_TerminalList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_GetParamStringValue",TelcoVoiceMgrDml_TerminalList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_SetParamStringValue",TelcoVoiceMgrDml_TerminalList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_GetParamBoolValue",TelcoVoiceMgrDml_TerminalList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_SetParamBoolValue",TelcoVoiceMgrDml_TerminalList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Validate",TelcoVoiceMgrDml_TerminalList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Commit",TelcoVoiceMgrDml_TerminalList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Rollback",TelcoVoiceMgrDml_TerminalList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_IsUpdated",TelcoVoiceMgrDml_TerminalList_AudioList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_Synchronize",TelcoVoiceMgrDml_TerminalList_AudioList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_GetEntryCount",TelcoVoiceMgrDml_TerminalList_AudioList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_GetEntry",TelcoVoiceMgrDml_TerminalList_AudioList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_GetParamStringValue",TelcoVoiceMgrDml_TerminalList_AudioList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_SetParamStringValue",TelcoVoiceMgrDml_TerminalList_AudioList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_Validate",TelcoVoiceMgrDml_TerminalList_AudioList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_Commit",TelcoVoiceMgrDml_TerminalList_AudioList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_Rollback",TelcoVoiceMgrDml_TerminalList_AudioList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamUlongValue",TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamIntValue",TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamIntValue",TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamIntValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamBoolValue",TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamBoolValue",TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_IsUpdated",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Synchronize",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntryCount",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntry",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamStringValue",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamStringValue",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamBoolValue",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamBoolValue",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Validate",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Commit",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Rollback",TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_IsUpdated",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Synchronize",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntryCount",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntry",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamStringValue",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamStringValue",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamBoolValue",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamBoolValue",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Validate",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Commit",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Rollback",TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_IsUpdated",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Synchronize",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntryCount",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntry",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamUlongValue",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamUlongValue",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamStringValue",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamStringValue",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamBoolValue",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamBoolValue",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Validate",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Commit",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Rollback",TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Rollback);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamStringValue",TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamStringValue",TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamBoolValue",TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamUlongValue",TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamUlongValue",TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_GetParamStringValue",TelcoVoiceMgrDml_ClkSync_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_SetParamStringValue",TelcoVoiceMgrDml_ClkSync_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_GetParamBoolValue",TelcoVoiceMgrDml_ClkSync_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_SetParamBoolValue",TelcoVoiceMgrDml_ClkSync_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_IsUpdated",TelcoVoiceMgrDml_ClkSync_ClkSourceList_IsUpdated);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_Synchronize",TelcoVoiceMgrDml_ClkSync_ClkSourceList_Synchronize);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetEntryCount",TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetEntryCount);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetEntry",TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetEntry);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetParamUlongValue",TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_SetParamUlongValue",TelcoVoiceMgrDml_ClkSync_ClkSourceList_SetParamUlongValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetParamStringValue",TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_SetParamStringValue",TelcoVoiceMgrDml_ClkSync_ClkSourceList_SetParamStringValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetParamBoolValue",TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_SetParamBoolValue",TelcoVoiceMgrDml_ClkSync_ClkSourceList_SetParamBoolValue);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_Validate",TelcoVoiceMgrDml_ClkSync_ClkSourceList_Validate);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_Commit",TelcoVoiceMgrDml_ClkSync_ClkSourceList_Commit);
 pPlugInfo->RegisterFunction(pPlugInfo->hContext, "TelcoVoiceMgrDml_ClkSync_ClkSourceList_Rollback",TelcoVoiceMgrDml_ClkSync_ClkSourceList_Rollback);
#else
    // TR104V1 APIS
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
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Debug_SetParamUlongValue",X_RDK_Debug_SetParamUlongValue);

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
#endif

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_TelcoVoice_GetParamStringValue", X_RDK_TelcoVoice_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_TelcoVoice_SetParamStringValue", X_RDK_TelcoVoice_SetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_TelcoVoice_Validate",  X_RDK_TelcoVoice_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_TelcoVoice_Commit",  X_RDK_TelcoVoice_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_TelcoVoice_Rollback", X_RDK_TelcoVoice_Rollback );

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_GetParamUlongValue",  X_RDK_Report_VoiceService_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_SetParamUlongValue",  X_RDK_Report_VoiceService_SetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_GetParamStringValue",  X_RDK_Report_VoiceService_GetParamStringValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_GetParamBoolValue",  X_RDK_Report_VoiceService_GetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_SetParamBoolValue",  X_RDK_Report_VoiceService_SetParamBoolValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_Validate",  X_RDK_Report_VoiceService_Validate);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_Commit",  X_RDK_Report_VoiceService_Commit);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_Rollback",  X_RDK_Report_VoiceService_Rollback);

    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_Default_GetParamUlongValue", 
                                 X_RDK_Report_VoiceService_Default_GetParamUlongValue);
    pPlugInfo->RegisterFunction(pPlugInfo->hContext, "X_RDK_Report_VoiceService_Default_SetParamUlongValue",
                                 X_RDK_Report_VoiceService_Default_SetParamUlongValue);

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
