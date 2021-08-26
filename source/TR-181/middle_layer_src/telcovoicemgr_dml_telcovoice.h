/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
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

#ifndef  _TELCOVOICEMGR_DML_TELCOVOICE_H
#define  _TELCOVOICEMGR_DML_TELCOVOICE_H
#include "slap_definitions.h"

/***********************************************************************

 APIs for Object:

    X_RDK_TelcoVoice.

    *  X_RDK_TelcoVoice_GetParamStringValue
    *  X_RDK_TelcoVoice_SetParamStringValue
    *  X_RDK_TelcoVoice_Validate
    *  X_RDK_TelcoVoice_Commit
    *  X_RDK_TelcoVoice_Rollback

***********************************************************************/
LONG
X_RDK_TelcoVoice_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    );

BOOL
X_RDK_TelcoVoice_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    );

BOOL
X_RDK_TelcoVoice_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
X_RDK_TelcoVoice_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
X_RDK_TelcoVoice_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

#endif
