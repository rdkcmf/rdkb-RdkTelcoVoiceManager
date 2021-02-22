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

BOOL NumberingPlan_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL NumberingPlan_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

ULONG PrefixInfo_GetEntryCount(ANSC_HANDLE hInsContext);
ANSC_HANDLE PrefixInfo_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);
BOOL PrefixInfo_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);
BOOL PrefixInfo_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);
LONG PrefixInfo_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);
BOOL PrefixInfo_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);
