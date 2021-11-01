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
#include "telcovoicemgr_dml_hal_param_v2.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_TerminalList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_TerminalList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_VOICESERVICE_TERMINAL_LIST_T  pTerminalList = pDmlVoiceService->Terminal;

    if (pTerminalList != NULL)
    {
        ret = pTerminalList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T pVoiceService = (PDML_VOICE_SERVICE_CTRL_T) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_VOICESERVICE_TERMINAL_LIST_T pTerminalList = pDmlVoiceService->Terminal;

    if(pTerminalList != NULL)
    {
        if(nIndex < pTerminalList->ulQuantity)
        {
            PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = pTerminalList->pdata[nIndex];
            if(pTerminalCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pTerminalCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_TerminalList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTerminalIndex = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pHEADCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;

    PDML_VOICESERVICE_TERMINAL pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTerminalIndex = pHEAD->uInstanceNumber;

    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        //Fetch status from voice stack
        hal_param_t req_param;
        memset(&req_param, 0, sizeof(req_param));
        snprintf(req_param.name, sizeof(req_param.name), DML_VOICESERVICE_TERMINAL_PARAM_NAME"%s", uVsIndex, uTerminalIndex, "Status");
        if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            *puLong = strtoul(req_param.value,NULL,10);
            ret = TRUE;
        }
        else
        {
            CcspTraceError(("%s:%d:: Status:get failed \n", __FUNCTION__, __LINE__));
            *puLong = CLIENT_STATUS_DISABLED;
            ret = TRUE;
        }
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

        ULONG TelcoVoiceMgrDml_TerminalList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_TerminalList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pHEADCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;
    if(pHEADCtrl)
    {
        PDML_VOICESERVICE_TERMINAL pHEAD = &(pHEADCtrl->dml);
        if (pHEAD == NULL)
        {
            TELCOVOICEMGR_UNLOCK()

            CcspTraceError(("%s:%d:: pHEAD NULL\n", __FUNCTION__, __LINE__));

            return ret;
        }
        if( AnscEqualString(ParamName, "ToneEventProfile", TRUE) )
        {
            AnscCopyString(pValue,pHEAD->ToneEventProfile);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "Alias", TRUE) )
        {
            AnscCopyString(pValue,pHEAD->Alias);
            ret = 0;
        }
        else
        {
            CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_TerminalList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTerminalIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pHEADCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;

    PDML_VOICESERVICE_TERMINAL pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTerminalIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "ToneEventProfile", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.ToneEventProfile",uVsIndex,uTerminalIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->ToneEventProfile,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        TELCOVOICEMGR_LOCK_OR_EXIT()

        AnscCopyString(pHEAD->Alias,pString);

        TELCOVOICEMGR_UNLOCK()

        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pHEADCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;

    PDML_VOICESERVICE_TERMINAL pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "QuiescentMode", TRUE) )
    {
        *pBool = pHEAD->QuiescentMode;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        *pBool = pHEAD->Enable;
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

        BOOL TelcoVoiceMgrDml_TerminalList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTerminalIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pHEADCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;

    PDML_VOICESERVICE_TERMINAL pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTerminalIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "QuiescentMode", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.QuiescentMode",uVsIndex,uTerminalIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->QuiescentMode = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Enable",uVsIndex,uTerminalIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Enable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_AudioList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_TerminalList_AudioList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_AudioList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_TerminalList_AudioList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICESERVICE_TERMINAL_CTRL_T* pTerminalCtrl = (DML_VOICESERVICE_TERMINAL_CTRL_T*) hInsContext;

    if(pTerminalCtrl)
    {
        PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);
        if(pDmlTerminal)
        {
            PDML_TERMINAL_AUDIO_LIST_T pTerminalAudioList = &(pDmlTerminal->Audio);

            if(pTerminalAudioList != NULL)
            {
                ret = pTerminalAudioList->ulQuantity;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_AudioList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_AudioList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T) hInsContext;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);

    PDML_TERMINAL_AUDIO_LIST_T pAudioList = &(pDmlTerminal->Audio);

    if(pAudioList != NULL)
    {
        if(nIndex < pAudioList->ulQuantity)
        {
            PDML_TERMINAL_AUDIO_CTRL_T pAudioCtrl = pAudioList->pdata[nIndex];
            if(pAudioCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pAudioCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_AudioList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_TerminalList_AudioList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_AUDIO_CTRL_T pHEADCtrl = (PDML_TERMINAL_AUDIO_CTRL_T)hInsContext;

    PDML_TERMINAL_AUDIO pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "Name", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Name);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Alias);
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

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTermIndex = 0;
    ULONG uAudioIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_AUDIO_CTRL_T pHEADCtrl = (PDML_TERMINAL_AUDIO_CTRL_T)hInsContext;

    PDML_TERMINAL_AUDIO pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pHEAD->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTermIndex = pDmlTerminal->uInstanceNumber;

    uAudioIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Name", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Audio.%d.Name",uVsIndex,uTermIndex,uAudioIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Name,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        TELCOVOICEMGR_LOCK_OR_EXIT()

        AnscCopyString(pHEAD->Alias,pString);

        TELCOVOICEMGR_UNLOCK()

        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_AUDIO_CTRL_T pHEADCtrl = (PDML_TERMINAL_AUDIO_CTRL_T)hInsContext;

    PDML_TERMINAL_AUDIO pTerminalAudio = &(pHEADCtrl->dml);

    PDML_TERMINAL_AUDIO_VOICEPROCESSING pHEAD = &(pTerminalAudio->VoiceProcessing);

    if( AnscEqualString(ParamName, "EchoCancellationTail", TRUE) )
    {
        *puLong = pHEAD->EchoCancellationTail;
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

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

    description:

        This function is called to retrieve integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_AUDIO_CTRL_T pHEADCtrl = (PDML_TERMINAL_AUDIO_CTRL_T)hInsContext;

    PDML_TERMINAL_AUDIO pTerminalAudio = &(pHEADCtrl->dml);

    PDML_TERMINAL_AUDIO_VOICEPROCESSING pHEAD = &(pTerminalAudio->VoiceProcessing);

    if( AnscEqualString(ParamName, "TransmitGain", TRUE) )
    {
        *pInt = pHEAD->TransmitGain;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ReceiveGain", TRUE) )
    {
        *pInt = pHEAD->ReceiveGain;
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

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

    description:

        This function is called to set integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTermIndex = 0;
    ULONG uAudioIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_AUDIO_CTRL_T pHEADCtrl = (PDML_TERMINAL_AUDIO_CTRL_T)hInsContext;

    PDML_TERMINAL_AUDIO pTerminalAudio = &(pHEADCtrl->dml);

    PDML_TERMINAL_AUDIO_VOICEPROCESSING pHEAD = &(pTerminalAudio->VoiceProcessing);

    if(pTerminalAudio != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pTerminalAudio->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTermIndex = pDmlTerminal->uInstanceNumber;

    uAudioIndex = pTerminalAudio->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "TransmitGain", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Audio.%d.VoiceProcessing.TransmitGain",uVsIndex,uTermIndex,uAudioIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TransmitGain = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "ReceiveGain", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Audio.%d.VoiceProcessing.ReceiveGain",uVsIndex,uTermIndex,uAudioIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ReceiveGain = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_AUDIO_CTRL_T pHEADCtrl = (PDML_TERMINAL_AUDIO_CTRL_T)hInsContext;

    PDML_TERMINAL_AUDIO pTerminalAudio = &(pHEADCtrl->dml);

    PDML_TERMINAL_AUDIO_VOICEPROCESSING pHEAD = &(pTerminalAudio->VoiceProcessing);

    if( AnscEqualString(ParamName, "EchoCancellationInUse", TRUE) )
    {
        *pBool = pHEAD->EchoCancellationInUse;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "EchoCancellationEnable", TRUE) )
    {
        *pBool = pHEAD->EchoCancellationEnable;
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

        BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_AudioList_VoiceProcessing_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTermIndex = 0;
    ULONG uAudioIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_AUDIO_CTRL_T pHEADCtrl = (PDML_TERMINAL_AUDIO_CTRL_T)hInsContext;

    PDML_TERMINAL_AUDIO pTerminalAudio = &(pHEADCtrl->dml);

    PDML_TERMINAL_AUDIO_VOICEPROCESSING pHEAD = &(pTerminalAudio->VoiceProcessing);

    if(pTerminalAudio != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pTerminalAudio->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTermIndex = pDmlTerminal->uInstanceNumber;

    uAudioIndex = pTerminalAudio->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "EchoCancellationEnable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Audio.%d.VoiceProcessing.EchoCancellationEnable",uVsIndex,uTermIndex,uAudioIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->EchoCancellationEnable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICESERVICE_TERMINAL_CTRL_T* pTerminalCtrl = (DML_VOICESERVICE_TERMINAL_CTRL_T*) hInsContext;
    if(pTerminalCtrl)
    {
        PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);
        if(pDmlTerminal)
        {
            PDML_TERMINAL_BUTTONMAP_BUTTON_LIST_T pTerminalButtonMapButtonList = &(pDmlTerminal->ButtonMap.Button);

            if(pTerminalButtonMapButtonList != NULL)
            {
                ret = pTerminalButtonMapButtonList->ulQuantity;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T) hInsContext;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);

    PDML_TERMINAL_BUTTONMAP_BUTTON_LIST_T pButtonList = &(pDmlTerminal->ButtonMap.Button);

    if(pButtonList != NULL)
    {
        if(nIndex < pButtonList->ulQuantity)
        {
            PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T pButtonCtrl = pButtonList->pdata[nIndex];
            if(pButtonCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pButtonCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T pHEADCtrl = (PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T)hInsContext;

    PDML_TERMINAL_BUTTONMAP_BUTTON pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "QuickDialNumber", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->QuickDialNumber);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "FacilityActionArgument", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->FacilityActionArgument);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "FacilityAction", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->FacilityAction);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "ButtonName", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->ButtonName);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "ButtonMessage", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->ButtonMessage);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Alias);
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

        BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTerminalIndex = 0;
    ULONG uButtonIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T pHEADCtrl = (PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T)hInsContext;

    PDML_TERMINAL_BUTTONMAP_BUTTON pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pHEAD->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTerminalIndex = pDmlTerminal->uInstanceNumber;

    uButtonIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "QuickDialNumber", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.ButtonMap.Button.%d.QuickDialNumber",uVsIndex,uTerminalIndex,uButtonIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->QuickDialNumber, pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "FacilityActionArgument", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.ButtonMap.Button.%d.FacilityActionArgument",uVsIndex,uTerminalIndex,uButtonIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->FacilityActionArgument, pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "FacilityAction", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.ButtonMap.Button.%d.FacilityAction",uVsIndex,uTerminalIndex,uButtonIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->FacilityAction, pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "ButtonMessage", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.ButtonMap.Button.%d.ButtonMessage",uVsIndex,uTerminalIndex,uButtonIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->ButtonMessage, pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        TELCOVOICEMGR_LOCK_OR_EXIT()

        AnscCopyString(pHEAD->Alias, pString);

        TELCOVOICEMGR_UNLOCK()

        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T pHEADCtrl = (PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T)hInsContext;

    PDML_TERMINAL_BUTTONMAP_BUTTON pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "UserAccess", TRUE) )
    {
        *pBool = pHEAD->UserAccess;
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

        BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTerminalIndex = 0;
    ULONG uButtonIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T pHEADCtrl = (PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T)hInsContext;

    PDML_TERMINAL_BUTTONMAP_BUTTON pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pHEAD->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTerminalIndex = pDmlTerminal->uInstanceNumber;

    uButtonIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "UserAccess", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.ButtonMap.Button.%d.UserAccess",uVsIndex,uTerminalIndex,uButtonIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->UserAccess, bValue);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_ButtonMap_ButtonList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICESERVICE_TERMINAL_CTRL_T* pTerminalCtrl = (DML_VOICESERVICE_TERMINAL_CTRL_T*) hInsContext;

    if(pTerminalCtrl)
    {
        PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);
        if(pDmlTerminal)
        {
            PDML_TERMINAL_RINGER_DESCRIPTION_LIST_T  pTerminalRingerDescList = &(pDmlTerminal->Ringer.Description);

            if(pTerminalRingerDescList != NULL)
            {
                ret = pTerminalRingerDescList->ulQuantity;
            }
        }
    }
    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T) hInsContext;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);

    PDML_TERMINAL_RINGER_DESCRIPTION_LIST_T pDescList = &(pDmlTerminal->Ringer.Description);

    if(pDescList != NULL)
    {
        if(nIndex < pDescList->ulQuantity)
        {
            PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T pDescCtrl = pDescList->pdata[nIndex];
            if(pDescCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pDescCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "RingPattern", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->RingPattern);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "RingName", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->RingName);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "RingFile", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->RingFile);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Alias);
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

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTermIndex = 0;
    ULONG uRingerDescIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pHEAD->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTermIndex = pDmlTerminal->uInstanceNumber;

    uRingerDescIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "RingPattern", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Ringer.Description.%d.RingPattern",uVsIndex,uTermIndex,uRingerDescIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->RingPattern,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "RingName", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Ringer.Description.%d.RingName",uVsIndex,uTermIndex,uRingerDescIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->RingName,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "RingFile", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Ringer.Description.%d.RingFile",uVsIndex,uTermIndex,uRingerDescIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->RingFile,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        TELCOVOICEMGR_LOCK_OR_EXIT()

        AnscCopyString(pHEAD->Alias,pString);

        TELCOVOICEMGR_UNLOCK()

        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "RingEnable", TRUE) )
    {
        *pBool = pHEAD->RingEnable;
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

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTermIndex = 0;
    ULONG uRingerDescIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pHEAD->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTermIndex = pDmlTerminal->uInstanceNumber;

    uRingerDescIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "RingEnable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Ringer.Description.%d.RingEnable",uVsIndex,uTermIndex,uRingerDescIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->RingEnable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_DescList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICESERVICE_TERMINAL_CTRL_T* pTerminalCtrl = (DML_VOICESERVICE_TERMINAL_CTRL_T*) hInsContext;

    if(pTerminalCtrl)
    {
        PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);
        if(pDmlTerminal)
        {
            PDML_TERMINAL_RINGER_PATTERN_LIST_T pRingerPatternList = &(pDmlTerminal->Ringer.Pattern);

            if(pRingerPatternList != NULL)
            {
                ret = pRingerPatternList->ulQuantity;
            }
        }
    }
    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T) hInsContext;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);

    PDML_TERMINAL_RINGER_PATTERN_LIST_T pPatternList = &(pDmlTerminal->Ringer.Pattern);

    if(pPatternList != NULL)
    {
        if(nIndex < pPatternList->ulQuantity)
        {
            PDML_TERMINAL_RINGER_PATTERN_CTRL_T pPatternCtrl = pPatternList->pdata[nIndex];
            if(pPatternCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pPatternCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_PATTERN_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_PATTERN_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_PATTERN pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "Duration", TRUE) )
    {
        *puLong = pHEAD->Duration;
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

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTermIndex = 0;
    ULONG uRingerPatternIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_PATTERN_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_PATTERN_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_PATTERN pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pHEAD->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTermIndex = pDmlTerminal->uInstanceNumber;

    uRingerPatternIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Duration", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Ringer.Pattern.%d.Duration",uVsIndex,uTermIndex,uRingerPatternIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Duration = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_PATTERN_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_PATTERN_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_PATTERN pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "NextPattern", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->NextPattern);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Alias);
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

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTermIndex = 0;
    ULONG uRingerPatternIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_PATTERN_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_PATTERN_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_PATTERN pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pHEAD->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTermIndex = pDmlTerminal->uInstanceNumber;

    uRingerPatternIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "NextPattern", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Ringer.Pattern.%d.NextPattern",uVsIndex,uTermIndex,uRingerPatternIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->NextPattern,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        TELCOVOICEMGR_LOCK_OR_EXIT()

        AnscCopyString(pHEAD->Alias,pString);

        TELCOVOICEMGR_UNLOCK()

        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_PATTERN_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_PATTERN_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_PATTERN pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "RingerOn", TRUE) )
    {
        *pBool = pHEAD->RingerOn;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        *pBool = pHEAD->Enable;
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

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTermIndex = 0;
    ULONG uRingerPatternIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TERMINAL_RINGER_PATTERN_CTRL_T pHEADCtrl = (PDML_TERMINAL_RINGER_PATTERN_CTRL_T)hInsContext;

    PDML_TERMINAL_RINGER_PATTERN pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlTerminal = (PDML_VOICESERVICE_TERMINAL)pHEAD->pParentTerminal;
    }

    if(pDmlTerminal != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlTerminal == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlTerminal or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTermIndex = pDmlTerminal->uInstanceNumber;

    uRingerPatternIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "RingerOn", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Ringer.Pattern.%d.RingerOn",uVsIndex,uTermIndex,uRingerPatternIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->RingerOn,bValue);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.Ringer.Pattern.%d.Enable",uVsIndex,uTermIndex,uRingerPatternIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Enable,bValue);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_Ringer_PatternList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);

    PDML_TERMINAL_DIAGTESTS pHEAD = &(pDmlTerminal->DiagTests);

    pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;

    ULONG uVsIndex = pDmlVoiceService->InstanceNumber;
    ULONG uTerminalIndex = pDmlTerminal->uInstanceNumber;

    if( AnscEqualString(ParamName, "X_RDK_TestResult", TRUE) )
    {
        //Fetch status from voice stack
        hal_param_t req_param;
        memset(&req_param, 0, sizeof(req_param));
        snprintf(req_param.name, sizeof(req_param.name), DML_VOICESERVICE_TERMINAL_PARAM_NAME"%s", uVsIndex, uTerminalIndex, "DiagTests.X_RDK_TestResult");
        if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            AnscCopyString(pValue,req_param.value);
            ret = TRUE;
        }
        else
        {
            CcspTraceError(("%s:%d:: Result:get failed \n", __FUNCTION__, __LINE__));
            AnscCopyString(pValue,"Failure");
            ret = TRUE;
        }

        ret = 0;
    }
    else if( AnscEqualString(ParamName, "TestSelector", TRUE) )
    {
        if(pHEAD->TestSelector == DIAG_TESTSELECTOR_PHONE_CONNECTIVITY_TEST)
        {
            AnscCopyString(pValue,"PhoneConnectivityTest");
            ret = 0;
        }
        else if(pHEAD->TestSelector == DIAG_TESTSELECTOR_HAZARD_POTENTIAL)
        {
            AnscCopyString(pValue,"Hazard Potential");
            ret = 0;
        }
        else if(pHEAD->TestSelector == DIAG_TESTSELECTOR_FOREIGN_VOLTAGE)
        {
            AnscCopyString(pValue,"Foreign Voltage");
            ret = 0;
        }
        else if(pHEAD->TestSelector == DIAG_TESTSELECTOR_RESISTIVE_FAULTS)
        {
            AnscCopyString(pValue,"Resistive Faults");
            ret = 0;
        }
        else if(pHEAD->TestSelector == DIAG_TESTSELECTOR_OFF_HOOK)
        {
            AnscCopyString(pValue,"Off-hook");
            ret = 0;
        }
        else if(pHEAD->TestSelector == DIAG_TESTSELECTOR_REN)
        {
            AnscCopyString(pValue,"REN");
            ret = 0;
        }
        else
        {
            CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return ret ;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTerminalIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);

    PDML_TERMINAL_DIAGTESTS pHEAD = &(pDmlTerminal->DiagTests);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTerminalIndex = pDmlTerminal->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "X_RDK_TestResult", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.DiagTests.X_RDK_TestResult",uVsIndex,uTerminalIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->X_RDK_TestResult,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "TestSelector", TRUE) )
    {
        ULONG uValue = 0;
        if(AnscEqualString(pString, "PhoneConnectivityTest", TRUE))
        {
            uValue = DIAG_TESTSELECTOR_PHONE_CONNECTIVITY_TEST;
        }
        else if(AnscEqualString(pString, "Hazard Potential", TRUE))
        {
            uValue = DIAG_TESTSELECTOR_HAZARD_POTENTIAL;
        }
        else if(AnscEqualString(pString, "Foreign Voltage", TRUE))
        {
            uValue = DIAG_TESTSELECTOR_FOREIGN_VOLTAGE;
        }
        else if(AnscEqualString(pString, "Resistive Faults", TRUE))
        {
            uValue = DIAG_TESTSELECTOR_RESISTIVE_FAULTS;
        }
        else if(AnscEqualString(pString, "Off-hook", TRUE))
        {
            uValue = DIAG_TESTSELECTOR_OFF_HOOK;
        }
        else if(AnscEqualString(pString, "REN", TRUE))
        {
            uValue = DIAG_TESTSELECTOR_REN;
        }
        else
        {
            CcspTraceWarning(("%s: Unsupported pString '%s'\n", __func__,ParamName));

            return ret ;
        }

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.DiagTests.TestSelector",uVsIndex,uTerminalIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TestSelector = uValue;

            TELCOVOICEMGR_UNLOCK()

            (void)storeObjectString(uVsIndex, TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE, 
                                    uTerminalIndex, "TestSelector", pString);
            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret ;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);

    PDML_TERMINAL_DIAGTESTS pHEAD = &(pDmlTerminal->DiagTests);

    if( AnscEqualString(ParamName, "PhoneRinging", TRUE) )
    {
        *pBool = pHEAD->PhoneRinging;
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

        BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);

    PDML_TERMINAL_DIAGTESTS pHEAD = &(pDmlTerminal->DiagTests);

    if( AnscEqualString(ParamName, "DiagnosticsState", TRUE) )
    {
        *puLong = pHEAD->DiagnosticsState;
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

        BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_TerminalList_DiagTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uTerminalIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalCtrl = (PDML_VOICESERVICE_TERMINAL_CTRL_T)hInsContext;

    PDML_VOICESERVICE_TERMINAL pDmlTerminal = &(pTerminalCtrl->dml);

    PDML_TERMINAL_DIAGTESTS pHEAD = &(pDmlTerminal->DiagTests);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlTerminal->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uTerminalIndex = pDmlTerminal->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "DiagnosticsState", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"None","Requested","Complete","Error_Internal","Error_Other"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Terminal.%d.DiagTests.DiagnosticsState",uVsIndex,uTerminalIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DiagnosticsState = DIAG_STATE_COMPLETE;

            TELCOVOICEMGR_UNLOCK()

            (void)storeObjectString(uVsIndex, TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE, 
                                    uTerminalIndex, "DiagnosticsState", enumValue[uValue]);

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}
