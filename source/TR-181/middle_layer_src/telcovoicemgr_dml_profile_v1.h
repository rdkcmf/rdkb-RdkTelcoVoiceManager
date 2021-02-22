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

#ifndef  _TELCOVOICEMGR_DML_PROFILE_H
#define  _TELCOVOICEMGR_DML_PROFILE_H

/***********************************************************************

 APIs for Object:

    Services.VoiceService.{i}.VoiceProfile.{i}

    *  VoiceProfile_GetEntryCount
    *  VoiceProfile_GetEntry
    *  VoiceProfile_GetParamStringValue
    *  VoiceProfile_SetParamStringValue
    *  VoiceProfile_GetParamUlongValue
    *  VoiceProfile_Validate
    *  VoiceProfile_Commit
    *  VoiceProfile_Rollback

    Services.VoiceService.{i}.VoiceProfile.{i}.SIP.

    *  SIP_GetParamStringValue
    *  SIP_SetParamStringValue
    *  SIP_GetParamBoolValue
    *  SIP_SetParamBoolValue

***********************************************************************/
BOOL VoiceProfile_IsUpdated(ANSC_HANDLE hInsContext);
ULONG VoiceProfile_Synchronize(ANSC_HANDLE hInsContext);
ULONG VoiceProfile_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE VoiceProfile_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
LONG VoiceProfile_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* PValue, ULONG* pUlSize);
BOOL VoiceProfile_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName,char* PString);
BOOL VoiceProfile_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong );
BOOL VoiceProfile_SetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName, ULONG uValue);
BOOL VoiceProfile_Validate(ANSC_HANDLE hInsContext);
ULONG VoiceProfile_Commit(ANSC_HANDLE hInsContext);
ULONG VoiceProfile_Rollback(ANSC_HANDLE hInsContext);
BOOL VoiceProfile_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL VoiceProfile_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

LONG SIP_GetParamStringValue(ANSC_HANDLE hInsContext,char* ParamName, char* PValue, ULONG* pUlSize);
BOOL SIP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* PString);
BOOL SIP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL SIP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL SIP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL*  PBool);
BOOL SIP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bvalue);
BOOL SIP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int*  PInt);
BOOL SIP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL RTP_SetParamUlongValue(ANSC_HANDLE hInsContext,char* ParamName, ULONG uValue);
BOOL RTP_GetParamIntValue(ANSC_HANDLE hInsContext,char* ParamName, int* PInt);
LONG RTP_GetParamStringValue(ANSC_HANDLE hInsContext,char* ParamName, char* PValue, ULONG* pUlSize);
BOOL RTP_SetParamIntValue(ANSC_HANDLE hInsContext,char* ParamName, int iValue);

BOOL Redundancy_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL Redundancy_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL Redundancy_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);
BOOL Redundancy_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL Redundancy_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);
BOOL Redundancy_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

BOOL RTCP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL RTCP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL RTCP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL RTCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
LONG RTCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL RTCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL SRTP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL SRTP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
LONG SRTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL SRTP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL H323_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL H323_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
LONG H323_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL H323_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL H323_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);
BOOL H323_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL H323_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
LONG H323_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL H323_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

BOOL MGCP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL MGCP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL MGCP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL MGCP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);
BOOL MGCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL MGCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL ButtonMap_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
ULONG Button_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE Button_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL Button_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL Button_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
LONG Button_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

BOOL FaxT38_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL FaxT38_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL FaxT38_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL FaxT38_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL FaxT38_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
LONG FaxT38_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

ULONG SIPResponseMap_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE SIPResponseMap_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL SIPResponseMap_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
LONG SIPResponseMap_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL SIPResponseMap_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG pValue);
BOOL SIPResponseMap_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL SIPResponseMap_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

ULONG SIPEventSubscribe_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE SIPEventSubscribe_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
LONG SIPEventSubscribe_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL SIPEventSubscribe_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL SIPEventSubscribe_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);
BOOL SIPEventSubscribe_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

LONG ServiceProviderInfo_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL ServiceProviderInfo_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

LONG MGCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL MGCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL MGCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL MGCP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);
BOOL MGCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

BOOL EventSubscribe_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
LONG EventSubscribe_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

#endif
