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

#include "ansc_platform.h"
#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_v2.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG X_RDK_Debug_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/

ULONG X_RDK_Debug_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T  pDmlVoiceServiceCtrl = (PDML_VOICE_SERVICE_CTRL_T)hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pDmlVoiceServiceCtrl->dml);

    PTELCOVOICEMGR_DML_X_RDK_DEBUG pHEAD = &(pDmlVoiceService->X_RDK_DebugObj);

    if( AnscEqualString(ParamName, "ModuleLogLevels", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->ModuleLogLevels);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "LogServer", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->LogServer);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "CCTKTraceLevel", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->CCTKTraceLevel);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "CCTKTraceGroup", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->CCTKTraceGroup);
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLX_RDK_Debug_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL X_RDK_Debug_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T  pDmlVoiceServiceCtrl = (PDML_VOICE_SERVICE_CTRL_T)hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pDmlVoiceServiceCtrl->dml);

    PTELCOVOICEMGR_DML_X_RDK_DEBUG pHEAD = &(pDmlVoiceService->X_RDK_DebugObj);

    if( AnscEqualString(ParamName, "LogServerPort", TRUE) )
    {
        *puLong = pHEAD->LogServerPort;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL X_RDK_Debug_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL X_RDK_Debug_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;

    ULONG uVsIndex = 0;

    if(ParamName == NULL || pString == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);

    if (pVoiceService == NULL || pString == NULL )
    {
         CcspTraceError(("%s:%d:: pVoiceService or pString NULL\n", __FUNCTION__, __LINE__));
         TELCOVOICEMGR_UNLOCK()
         return ret;
    }

    uVsIndex = pVoiceService->InstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "CCTKTraceGroup", TRUE) )
    {
        if(TelcoVoiceMgrDmlSetCCTKTraceGroup(uVsIndex, pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()
            AnscCopyString(pVoiceService->X_RDK_DebugObj.CCTKTraceGroup, pString);
            TELCOVOICEMGR_UNLOCK()
            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "CCTKTraceLevel", TRUE) )
    {
        if(TelcoVoiceMgrDmlSetCCTKTraceLevel(uVsIndex, pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()
            AnscCopyString(pVoiceService->X_RDK_DebugObj.CCTKTraceLevel, pString);
            TELCOVOICEMGR_UNLOCK()
            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "ModuleLogLevels", TRUE) )
    {
        if(TelcoVoiceMgrDmlSetModuleLogLevel(uVsIndex, pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()
            AnscCopyString(pVoiceService->X_RDK_DebugObj.ModuleLogLevels, pString);
            TELCOVOICEMGR_UNLOCK()
            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "LogServer", TRUE) )
    {
        //Security Vulnerability Check
        if( TRUE != TelcoVoiceMgrAnscValidateInputString(pString) )
        {
            return FALSE;
        }

        if(TelcoVoiceMgrDmlSetLogServer(uVsIndex, pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()
            AnscCopyString(pVoiceService->X_RDK_DebugObj.LogServer, pString);
            TELCOVOICEMGR_UNLOCK()
            ret = TRUE;
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Invalid Parameter [%s] \n", __FUNCTION__, __LINE__, ParamName));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLX_RDK_Debug_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL X_RDK_Debug_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;

    ULONG uVsIndex = 0;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);

    if (pVoiceService)
    {
        uVsIndex = pVoiceService->InstanceNumber;

        TELCOVOICEMGR_UNLOCK()

        if( AnscEqualString(ParamName, "LogServerPort", TRUE) )
        {
            if(TelcoVoiceMgrDmlSetLogServerPort(uVsIndex, uValue) == ANSC_STATUS_SUCCESS)
            {
                TELCOVOICEMGR_LOCK_OR_EXIT()
                pVoiceService->X_RDK_DebugObj.LogServerPort = uValue;
                TELCOVOICEMGR_UNLOCK()
                ret = TRUE;
            }
        }
        else
        {
            CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
        }
    }
    else
    {
        TELCOVOICEMGR_UNLOCK()
        CcspTraceError(("%s:%d:: pVoiceService Null \n", __FUNCTION__, __LINE__));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLX_RDK_Debug_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL X_RDK_Debug_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLX_RDK_Debug_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL X_RDK_Debug_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLX_RDK_Debug_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL X_RDK_Debug_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}
