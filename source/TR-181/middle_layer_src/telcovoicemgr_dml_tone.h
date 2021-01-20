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

#ifndef  _TELCOVOICEMGR_DML_TONE_H
#define  _TELCOVOICEMGR_DML_TONE_H

ULONG ToneEvent_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE ToneEvent_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
LONG ToneEvent_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL ToneEvent_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL ToneEvent_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL ToneEvent_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

ULONG ToneDescription_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE ToneDescription_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL ToneDescription_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
LONG ToneDescription_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL ToneDescription_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

ULONG TonePattern_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE TonePattern_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL TonePattern_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
LONG TonePattern_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL TonePattern_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);
BOOL TonePattern_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);
BOOL TonePattern_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

BOOL ToneDescription_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
BOOL ToneDescription_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
BOOL ToneDescription_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);


#endif
