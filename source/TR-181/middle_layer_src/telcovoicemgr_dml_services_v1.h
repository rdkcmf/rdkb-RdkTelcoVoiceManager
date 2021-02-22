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
#ifndef  _TELCOVOICEMGR_DML_SERVICES_H
#define  _TELCOVOICEMGR_DML_SERVICES_H
#include "slap_definitions.h"

BOOL Services_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG VoiceService_Synchronize(ANSC_HANDLE hInsContext);
BOOL VoiceService_IsUpdated(ANSC_HANDLE hInsContext);
ULONG VoiceService_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE VoiceService_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL VoiceService_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);
BOOL VoiceService_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL VoiceService_GetParamBoolValue(ANSC_HANDLE hInsContext,char*  ParamName,BOOL *pBool);
LONG VoiceService_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL VoiceService_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL VoiceService_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bvalue);
BOOL VoiceService_GetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName, ULONG* pValue);
BOOL VoiceService_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL VoiceService_Validate(ANSC_HANDLE hInsContext, char* pReturnParamName, ULONG* puLength);
ULONG VoiceService_Commit(ANSC_HANDLE hInsContext);
ULONG VoiceService_Rollback(ANSC_HANDLE hInsContext);

ANSC_HANDLE PhyInterface_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex,ULONG* pInsNumber);
BOOL PhyInterface_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL PhyInterfaceTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
ULONG PhyInterface_GetEntryCount(ANSC_HANDLE hInsContext);
BOOL PhyInterface_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
LONG PhyInterfaceTests_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL PhyInterfaceTests_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL PhyInterfaceTests_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL PhyInterfaceTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);
LONG PhyInterface_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL PhyInterface_IsUpdated(ANSC_HANDLE hInsContext);
ULONG PhyInterface_Synchronize(ANSC_HANDLE hInsContext);

ULONG CapabilitiesCodecs_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE CapabilitiesCodecs_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL Capabilities_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL Capabilities_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
LONG Capabilities_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL CapabilitiesSIP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
LONG CapabilitiesSIP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
LONG CapabilitiesMGCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL CapabilitiesH323_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
LONG CapabilitiesH323_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL CapabilitiesCodecs_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
LONG CapabilitiesCodecs_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL CapabilitiesCodecs_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL CapabilitiesCodecs_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

BOOL X_RDK_Debug_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
LONG X_RDK_Debug_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL X_RDK_Debug_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL X_RDK_Debug_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL X_RDK_Debug_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
#endif
