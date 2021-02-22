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

#ifndef  _TELCOVOICEMGR_DML_LINE_H
#define  _TELCOVOICEMGR_DML_LINE_H

/***********************************************************************

 APIs for Object:

    Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}

    *  Line_GetEntryCount
    *  Line_GetEntry
    *  Line_GetParamStringValue
    *  Line_SetParamStringValue
    *  Line_Validate
    *  Line_Commit
    *  Line_Rollback

***********************************************************************/
BOOL Line_IsUpdated(ANSC_HANDLE hInsContext);
ULONG Line_Synchronize(ANSC_HANDLE hInsContext);
ULONG Line_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE Line_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
LONG Line_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL Line_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL Line_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL Line_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL Line_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL Line_Validate(ANSC_HANDLE hInsContext);
ULONG Line_Commit(ANSC_HANDLE hInsContext);
ULONG Line_Rollback(ANSC_HANDLE hInsContext);

BOOL VoiceProcessing_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);
BOOL VoiceProcessing_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);
BOOL VoiceProcessing_Validate(ANSC_HANDLE hInsContext);
ULONG VoiceProcessing_Commit(ANSC_HANDLE hInsContext);
ULONG VoiceProcessing_Rollback(ANSC_HANDLE hInsContext);
BOOL VoiceProcessing_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL VoiceProcessing_SetParamBoolValue (ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL VoiceProcessing_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL VoiceProcessing_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

LONG LineSIP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL LineSIP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
LONG LineMGCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL LineMGCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
LONG LineH323_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL LineH323_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL LineRinger_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

BOOL CallingFeatures_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL CallingFeatures_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL CallingFeatures_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL *pBool);
BOOL CallingFeatures_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
LONG CallingFeatures_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL CallingFeatures_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL CallingFeatures_Validate(ANSC_HANDLE hInsContext);
ULONG CallingFeatures_Commit(ANSC_HANDLE hInsContext);
ULONG CallingFeatures_Rollback(ANSC_HANDLE hInsContext);

BOOL Stats_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL *pBool);
BOOL Stats_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL Stats_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL Event_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL Event_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
LONG Event_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

BOOL Description_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL Description_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
LONG Description_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL Description_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL Description_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL Description_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL List_IsUpdated(ANSC_HANDLE hInsContext);
ULONG List_Synchronize(ANSC_HANDLE hInsContext);
ULONG List_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE List_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL List_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL List_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);
LONG List_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL List_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL List_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL List_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);
BOOL List_Validate(ANSC_HANDLE hInsContext);
ULONG List_Commit(ANSC_HANDLE hInsContext);
ULONG List_Rollback(ANSC_HANDLE hInsContext);

BOOL Pattern_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL Pattern_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
LONG Pattern_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL Pattern_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL Pattern_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL Pattern_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

BOOL LineCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL LineCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
LONG LineCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

BOOL Session_IsUpdated(ANSC_HANDLE hInsContext);
ULONG Session_Synchronize(ANSC_HANDLE hInsContext);
ULONG Session_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE Session_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL Session_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
LONG Session_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL Session_Validate(ANSC_HANDLE hInsContext);
ULONG Session_Commit(ANSC_HANDLE hInsContext);
ULONG Session_Rollback(ANSC_HANDLE hInsContext);

BOOL Event_IsUpdated(ANSC_HANDLE hInsContext);
ULONG Event_Synchronize(ANSC_HANDLE hInsContext);
ULONG Event_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE Event_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL Event_Validate(ANSC_HANDLE hInsContext);
ULONG Event_Commit(ANSC_HANDLE hInsContext);
ULONG Event_Rollback(ANSC_HANDLE hInsContext);

BOOL Description_IsUpdated(ANSC_HANDLE hInsContext);
ULONG Description_Synchronize(ANSC_HANDLE hInsContext);
ULONG Description_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE Description_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL Description_Validate(ANSC_HANDLE hInsContext);
ULONG Description_Commit(ANSC_HANDLE hInsContext);
ULONG Description_Rollback(ANSC_HANDLE hInsContext);

BOOL Pattern_IsUpdated(ANSC_HANDLE hInsContext);
ULONG Pattern_Synchronize(ANSC_HANDLE hInsContext);
ULONG Pattern_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE Pattern_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL Pattern_Validate(ANSC_HANDLE hInsContext);
ULONG Pattern_Commit(ANSC_HANDLE hInsContext);
ULONG Pattern_Rollback(ANSC_HANDLE hInsContext);

BOOL EventSubscribe_IsUpdated(ANSC_HANDLE hInsContext);
ULONG EventSubscribe_Synchronize(ANSC_HANDLE hInsContext);
ULONG EventSubscribe_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE EventSubscribe_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL EventSubscribe_Validate(ANSC_HANDLE hInsContext);
ULONG EventSubscribe_Commit(ANSC_HANDLE hInsContext);
ULONG EventSubscribe_Rollback(ANSC_HANDLE hInsContext);

#endif
