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

#ifndef  _TELCOVOICEMGR_DML_REPORT_H
#define  _TELCOVOICEMGR_DML_REPORT_H

/***********************************************************************
 APIs for Object:

    Device.Services.X_RDK_Report.Voice.

    *  X_RDK_Report_VoiceService_GetParamUlongValue
    *  X_RDK_Report_VoiceService_SetParamUlongValue
    *  X_RDK_Report_VoiceService_GetParamStringValue
    *  X_RDK_Report_VoiceService_GetParamBoolValue
    *  X_RDK_Report_VoiceService_SetParamBoolValue
    *  X_RDK_Report_VoiceService_Commit
    *  X_RDK_Report_VoiceService_Validate

***********************************************************************/

BOOL X_RDK_Report_VoiceService_GetParamUlongValue( ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

BOOL X_RDK_Report_VoiceService_SetParamUlongValue( ANSC_HANDLE hInsContext, char* ParamName, ULONG  uValue);

ULONG X_RDK_Report_VoiceService_GetParamStringValue( ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

BOOL X_RDK_Report_VoiceService_GetParamBoolValue( ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool );

BOOL X_RDK_Report_VoiceService_SetParamBoolValue( ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue );

BOOL X_RDK_Report_VoiceService_Validate( ANSC_HANDLE hInsContext, char* pReturnParamName, ULONG* puLength );

ULONG X_RDK_Report_VoiceService_Commit( ANSC_HANDLE hInsContext );

ULONG X_RDK_Report_VoiceService_Rollback( ANSC_HANDLE hInsContext );

/***********************************************************************

 APIs for Object:

    Device.Services.X_RDK_Report.Voice.Default.

    *  X_RDK_Report_VoiceService_Default_GetParamUlongValue
    *  X_RDK_Report_VoiceService_Default_SetParamUlongValue

***********************************************************************/

BOOL X_RDK_Report_VoiceService_Default_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);
BOOL X_RDK_Report_VoiceService_Default_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG  uValue);


ANSC_STATUS GetNVRamULONGConfiguration(char* setting, ULONG* value);
ANSC_STATUS SetNVRamULONGConfiguration(char* setting, ULONG value);

#endif
