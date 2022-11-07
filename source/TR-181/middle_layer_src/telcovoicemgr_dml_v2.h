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
#ifndef  _TELCOVOICEMGR_DML_V2_H
#define  _TELCOVOICEMGR_DML_V2_H

#define STR_LEN_16  16
#define STR_LEN_32  32
#define MAX_STR_LEN 256

#define TELCOVOICEMGR_LOCK_OR_EXIT()   if(hInsContext == NULL )                                                       \
    {                                                                                                                 \
        CcspTraceError(("%s:%d:: hInsContext: NULL\n", __FUNCTION__, __LINE__));                                      \
        return ret;                                                                                                   \
    }                                                                                                                 \
    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlDataLock = TelcoVoiceMgrDmlGetDataLocked();                              \
    if(pTelcoVoiceMgrDmlDataLock == NULL)                                                                             \
    {                                                                                                                 \
        CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));                  \
        return ret;                                                                                                   \
    }

#define TELCOVOICEMGR_UNLOCK()     TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlDataLock);

BOOL VoiceService_IsUpdated(ANSC_HANDLE hInsContext);

ULONG VoiceService_Synchronize(ANSC_HANDLE hInsContext);

ULONG VoiceService_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE VoiceService_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL VoiceService_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL VoiceService_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

BOOL VoiceService_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL VoiceService_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

ULONG VoiceService_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL VoiceService_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL VoiceService_Validate(ANSC_HANDLE hInsContext);

BOOL VoiceService_Commit(ANSC_HANDLE hInsContext);

BOOL VoiceService_Rollback(ANSC_HANDLE hInsContext);

ULONG X_RDK_Debug_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL X_RDK_Debug_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL X_RDK_Debug_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL X_RDK_Debug_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

BOOL X_RDK_Debug_Validate(ANSC_HANDLE hInsContext);

BOOL X_RDK_Debug_Commit(ANSC_HANDLE hInsContext);

BOOL X_RDK_Debug_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_capabilities_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_capabilities_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_capabilities_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_capabilities_SIP_Client_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_capabilities_SIP_Client_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_capabilities_SIP_Registrar_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_capabilities_SIP_Registrar_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

ULONG TelcoVoiceMgrDml_capabilities_MGCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_capabilities_H323_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_capabilities_H323_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_capabilities_ISDN_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_capabilities_POTS_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_capabilities_POTS_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_capabilities_POTS_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_capabilities_CodecList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_capabilities_CodecList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_capabilities_CodecList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_capabilities_CodecList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_capabilities_CodecList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_capabilities_CodecList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_capabilities_CodecList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_capabilities_CodecList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_capabilities_CodecList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_capabilities_CodecList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_capabilities_CodecList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_capabilities_QualityIndicator_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_capabilities_QualityIndicator_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

ULONG TelcoVoiceMgrDml_ReservedPorts_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_ReservedPorts_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_ISDN_BRIList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_ISDN_BRIList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_ISDN_BRIList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_ISDN_BRIList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_ISDN_BRIList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_ISDN_BRIList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_ISDN_BRIList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_ISDN_BRIList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_ISDN_BRIList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_ISDN_BRIList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_ISDN_BRIList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_ISDN_BRIList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_ISDN_BRIList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_ISDN_PRIList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_ISDN_PRIList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_ISDN_PRIList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_ISDN_PRIList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_ISDN_PRIList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_ISDN_PRIList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_ISDN_PRIList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_ISDN_PRIList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_ISDN_PRIList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_ISDN_PRIList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_ISDN_PRIList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_ISDN_PRIList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_ISDN_PRIList_Rollback(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_POTS_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_POTS_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_POTS_FXOList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_POTS_FXOList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_POTS_FXOList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_POTS_FXOList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_POTS_FXOList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_POTS_FXOList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_POTS_FXOList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_POTS_FXOList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_POTS_FXOList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_POTS_FXOList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_POTS_FXOList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_POTS_FXOList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_POTS_FXOList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_POTS_FXOList_DiagTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_POTS_FXOList_DiagTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

BOOL TelcoVoiceMgrDml_POTS_FXSList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_POTS_FXSList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_POTS_FXSList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_POTS_FXSList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_POTS_FXSList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_POTS_FXSList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_POTS_FXSList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_POTS_FXSList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_POTS_FXSList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_POTS_FXSList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_POTS_FXSList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_POTS_FXSList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_POTS_FXSList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_POTS_FXSList_DiagTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_POTS_FXSList_DiagTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_POTS_Ringer_EventList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_DECT_BaseList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_DECT_BaseList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_DECT_BaseList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_DECT_BaseList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_DECT_BaseList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_DECT_BaseList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_DECT_BaseList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_DECT_BaseList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_DECT_BaseList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_DECT_BaseList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_DECT_BaseList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_DECT_BaseList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_DECT_BaseList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_DECT_BaseList_Stats_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_DECT_PortableList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_DECT_PortableList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_DECT_PortableList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_DECT_PortableList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_DECT_PortableList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_DECT_PortableList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_DECT_PortableList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_DECT_PortableList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_DECT_PortableList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_DECT_PortableList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_DECT_PortableList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_DECT_PortableList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_DECT_PortableList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ClientList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_ClientList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_ClientList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_ClientList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_ClientList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_SIP_ClientList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_SIP_ClientList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_ClientList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_ClientList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_ClientList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_ClientList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ClientList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ClientList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ClientList_ContactList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_ClientList_ContactList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_SIP_ClientList_ContactList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_ClientList_ContactList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_ClientList_ContactList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_ClientList_ContactList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_ClientList_ContactList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ClientList_ContactList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ClientList_ContactList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ClientList_EvtSubList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_NetworkList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_FQDNServerList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_EvtSubList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_NetworkList_RespMapList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ProxyList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_ProxyList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_ProxyList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_ProxyList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_ProxyList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_SIP_ProxyList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_SIP_ProxyList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_ProxyList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_ProxyList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_ProxyList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_ProxyList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ProxyList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_ProxyList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_RegistrarList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_RegistrarList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_RegistrarList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_SIP_RegistrarList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_SIP_RegistrarList_AccountList_ContactList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_MGCP_ClientList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_MGCP_ClientList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_MGCP_ClientList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_MGCP_ClientList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_MGCP_ClientList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_MGCP_ClientList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_MGCP_NetworkList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_MGCP_NetworkList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_H323_ClientList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_H323_ClientList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_H323_ClientList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_H323_ClientList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_H323_ClientList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_H323_ClientList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_H323_ClientList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_H323_ClientList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_H323_ClientList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_H323_ClientList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_H323_ClientList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_H323_ClientList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_H323_ClientList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_H323_NetworkList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_H323_NetworkList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_H323_NetworkList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_H323_NetworkList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_H323_NetworkList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_H323_NetworkList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_H323_NetworkList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_H323_NetworkList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_H323_NetworkList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_H323_NetworkList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_H323_NetworkList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_H323_NetworkList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_H323_NetworkList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_H323_NetworkList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_H323_NetworkList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TrunkList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TrunkList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TrunkList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_TrunkList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_TrunkList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_TrunkList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_TrunkList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_TrunkList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_TrunkList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_TrunkList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_TrunkList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_TrunkList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_TrunkList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TrunkList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TrunkList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

BOOL TelcoVoiceMgrDml_CallControl_LineList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_LineList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_LineList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_LineList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_LineList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallControl_LineList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_LineList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_LineList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_LineList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_LineList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_LineList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_LineList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_LineList_stats_InCalls_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_LineList_stats_OutCalls_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_LineList_stats_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_LineList_stats_DSP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_ExtensionList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_ExtensionList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_ExtensionList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallControl_ExtensionList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_InCalls_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_OutCalls_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_ExtensionList_Stats_DSP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_GroupList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_GroupList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_GroupList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_GroupList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_GroupList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_GroupList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_CallControl_GroupList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_GroupList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_GroupList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_GroupList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_GroupList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_MailboxList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_MailboxList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_MailboxList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_MailboxList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_MailboxList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_MailboxList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_CallControl_MailboxList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_MailboxList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_MailboxList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_MailboxList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_MailboxList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_MailboxList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_MailboxList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_IncomingMapList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_IncomingMapList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_IncomingMapList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_IncomingMapList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_IncomingMapList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_IncomingMapList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_CallControl_IncomingMapList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_IncomingMapList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_IncomingMapList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_IncomingMapList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_IncomingMapList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_IncomingMapList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_IncomingMapList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_OutgoingMapList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_OutgoingMapList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_OutgoingMapList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_OutgoingMapList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_OutgoingMapList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_OutgoingMapList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_OutgoingMapList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_OutgoingMapList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_OutgoingMapList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_NumberingPlanList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_CallControl_NumberingPlanList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_NumberingPlanList_PrefixInfoList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_CFTList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCFList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SCREJList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_FollowMeList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_Voice2Mail_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_InterworkList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_InterworkList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_InterworkList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_InterworkList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_InterworkList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_InterworkList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_InterworkList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_InterworkList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_InterworkList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_InterworkList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_InterworkList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_InterworkList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_InterworkList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_InterworkList_UIList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_InterworkList_UIList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_InterworkList_UIList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_InterworkList_UIList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_InterworkList_UIList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_InterworkList_UIList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_InterworkList_UIList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_InterworkList_UIList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_InterworkList_UIList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_InterworkList_UIList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_InterworkList_UIList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_InterworkList_MapList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_InterworkList_MapList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_InterworkList_MapList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_InterworkList_MapList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_InterworkList_MapList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_InterworkList_MapList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_InterworkList_MapList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_InterworkList_MapList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_InterworkList_MapList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_InterworkList_MapList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_InterworkList_MapList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_InterworkList_MapList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_InterworkList_MapList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallLogList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallLogList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallLogList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallLogList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallLogList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallLogList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CallLogList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallLogList_SignalPerfList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CallLogList_SessionList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_VoiceQuality_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_VoiceQuality_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_VoiceQuality_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_VoipProfileList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_VoipProfileList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_VoipProfileList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_VoipProfileList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_VoipProfileList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_VoipProfileList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_VoipProfileList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_VoipProfileList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_VoipProfileList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_VoipProfileList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_VoipProfileList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_VoipProfileList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_VoipProfileList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_VoipProfileList_FaxT38_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_VoipProfileList_FaxT38_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

BOOL TelcoVoiceMgrDml_CodecProfileList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CodecProfileList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_CodecProfileList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_CodecProfileList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_CodecProfileList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_CodecProfileList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_CodecProfileList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_CodecProfileList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_CodecProfileList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CodecProfileList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_CodecProfileList_Rollback(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_Tone_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_Tone_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_Tone_DescList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_Tone_DescList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_Tone_DescList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_Tone_DescList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_Tone_DescList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_Tone_DescList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_Tone_DescList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_Tone_DescList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_Tone_DescList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_Tone_DescList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_Tone_DescList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_DescList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_DescList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_PatternList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_Tone_PatternList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_Tone_PatternList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_Tone_PatternList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_Tone_PatternList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_Tone_PatternList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_Tone_PatternList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_Tone_PatternList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_Tone_PatternList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_PatternList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_PatternList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_Tone_EventProfileList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_TerminalList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

ULONG TelcoVoiceMgrDml_TerminalList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_TerminalList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_TerminalList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_TerminalList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_TerminalList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_AudioList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_AudioList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_AudioList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_TerminalList_AudioList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Rollback(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Rollback(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_ClkSync_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_ClkSync_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_ClkSync_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_ClkSync_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_ClkSync_ClkSourceList_IsUpdated(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_ClkSync_ClkSourceList_Synchronize(ANSC_HANDLE hInsContext);

ULONG TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetEntryCount(ANSC_HANDLE hInsContext);

ANSC_HANDLE TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

BOOL TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL TelcoVoiceMgrDml_ClkSync_ClkSourceList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

BOOL TelcoVoiceMgrDml_ClkSync_ClkSourceList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL TelcoVoiceMgrDml_ClkSync_ClkSourceList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL TelcoVoiceMgrDml_ClkSync_ClkSourceList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL TelcoVoiceMgrDml_ClkSync_ClkSourceList_Validate(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_ClkSync_ClkSourceList_Commit(ANSC_HANDLE hInsContext);

BOOL TelcoVoiceMgrDml_ClkSync_ClkSourceList_Rollback(ANSC_HANDLE hInsContext);

#endif  /* _TELCOVOICEMGR_DML_V2_H */
