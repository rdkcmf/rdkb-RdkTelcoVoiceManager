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

#define HAL_DML_VOICESERVICE_POTS_FXO                          "Device.Services.VoiceService.%d.POTS.FXO.%d."
#define HAL_DML_VOICESERVICE_POTS_FXS                          "Device.Services.VoiceService.%d.POTS.FXS.%d."

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_POTS_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_POTS_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
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

    PDML_POTS pHEAD = &(pDmlVoiceService->POTS_obj);

    if( AnscEqualString(ParamName, "Region", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Region);
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

        BOOL TelcoVoiceMgrDml_POTS_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_POTS_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;

    ULONG uVsIndex = 0;

    char HalName[MAX_STR_LEN] = {0};

    if(ParamName == NULL || pString == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T  pDmlVoiceServiceCtrl = (PDML_VOICE_SERVICE_CTRL_T)hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pDmlVoiceServiceCtrl->dml);

    PDML_POTS pHEAD = &(pDmlVoiceService->POTS_obj);

    if (pHEAD == NULL  || pString == NULL )
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pString NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Region", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.POTS.Region",uVsIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Region, pString);

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

        BOOL TelcoVoiceMgrDml_POTS_FXOList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_FXOList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_POTS_FXOList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_POTS_FXOList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_POTS_FXOList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_POTS_FXOList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_POTS_FXO_LIST_T pPotsFxoList = &(pDmlVoiceService->POTS_obj.FXO);

    if(pPotsFxoList != NULL)
    {
        ret = pPotsFxoList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_POTS_FXOList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_POTS_FXOList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_POTS_FXO_LIST_T pFXOList = &(pDmlVoiceService->POTS_obj.FXO);

    if(pFXOList != NULL)
    {
        if(nIndex < pFXOList->ulQuantity)
        {
            PDML_POTS_FXO_CTRL_T pFXOCtrl = pFXOList->pdata[nIndex];
            if(pFXOCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pFXOCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXOList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_POTS_FXOList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPotsFxoIndex = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXO_CTRL_T pPotsFxoCtrl = (PDML_POTS_FXO_CTRL_T)hInsContext;

    PDML_POTS_FXO pHEAD = &(pPotsFxoCtrl->dml);

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

    uPotsFxoIndex = pHEAD->uInstanceNumber;

    if( AnscEqualString(ParamName, "TimeoutBeforeDialing", TRUE) )
    {
        *puLong = pHEAD->TimeoutBeforeDialing;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        //Fetch status from voice stack
        hal_param_t req_param;
        memset(&req_param, 0, sizeof(req_param));
        snprintf(req_param.name, sizeof(req_param.name), DML_VOICESERVICE_POTS_FXO_PARAM_NAME"%s", uVsIndex, uPotsFxoIndex, "Status");
        if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            *puLong = strtoul(req_param.value,NULL,10);
            ret = TRUE;
        }
        else
        {
            CcspTraceError(("%s:%d:: Status:get failed \n", __FUNCTION__, __LINE__));
            ret = FALSE;
        }
    }
    else if( AnscEqualString(ParamName, "SignalingMode", TRUE) )
    {
        *puLong = pHEAD->SignalingMode;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "RingNumber", TRUE) )
    {
        *puLong = pHEAD->RingNumber;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "RingingTimeout", TRUE) )
    {
        *puLong = pHEAD->RingingTimeout;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "OnHookMinDuration", TRUE) )
    {
        *puLong = pHEAD->OnHookMinDuration;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "DTMFDialoutInterval", TRUE) )
    {
        *puLong = pHEAD->DTMFDialoutInterval;
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

        BOOL TelcoVoiceMgrDml_POTS_FXOList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_POTS_FXOList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxoIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXO_CTRL_T pPotsFxoCtrl = (PDML_POTS_FXO_CTRL_T)hInsContext;

    PDML_POTS_FXO pHEAD = &(pPotsFxoCtrl->dml);

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

    uFxoIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "TimeoutBeforeDialing", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"TimeoutBeforeDialing",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TimeoutBeforeDialing = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "SignalingMode", TRUE) )
    {
        char enumValue[][STR_LEN_16] = {"LoopStart","GroundStart"};
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"SignalingMode",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->SignalingMode = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "RingNumber", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"RingNumber",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->RingNumber = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "RingingTimeout", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"RingingTimeout",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->RingingTimeout = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "OnHookMinDuration", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"OnHookMinDuration",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->OnHookMinDuration = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "DTMFDialoutInterval", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"DTMFDialoutInterval",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DTMFDialoutInterval = uValue;

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

        ULONG TelcoVoiceMgrDml_POTS_FXOList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_POTS_FXOList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXO_CTRL_T pPotsFxoCtrl = (PDML_POTS_FXO_CTRL_T)hInsContext;

    PDML_POTS_FXO pHEAD = &(pPotsFxoCtrl->dml);

    if( AnscEqualString(ParamName, "ToneEventProfile", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->ToneEventProfile);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Name", TRUE) )
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

        BOOL TelcoVoiceMgrDml_POTS_FXOList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_POTS_FXOList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxoIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXO_CTRL_T pPotsFxoCtrl = (PDML_POTS_FXO_CTRL_T)hInsContext;

    PDML_POTS_FXO pHEAD = &(pPotsFxoCtrl->dml);

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

    uFxoIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "ToneEventProfile", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"ToneEventProfile",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->ToneEventProfile,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Name", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"Name",uVsIndex,uFxoIndex);

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
    TELCOVOICEMGR_UNLOCK()

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXOList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_POTS_FXOList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXO_CTRL_T pPotsFxoCtrl = (PDML_POTS_FXO_CTRL_T)hInsContext;

    PDML_POTS_FXO pHEAD = &(pPotsFxoCtrl->dml);

    if( AnscEqualString(ParamName, "SecondStepDialing", TRUE) )
    {
        *pBool = pHEAD->SecondStepDialing;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        *pBool = pHEAD->Enable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallerIdDetectionEnable", TRUE) )
    {
        *pBool = pHEAD->CallerIdDetectionEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Active", TRUE) )
    {
        *pBool = pHEAD->Active;
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

        BOOL TelcoVoiceMgrDml_POTS_FXOList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_POTS_FXOList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxoIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXO_CTRL_T pPotsFxoCtrl = (PDML_POTS_FXO_CTRL_T)hInsContext;

    PDML_POTS_FXO pHEAD = &(pPotsFxoCtrl->dml);

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

    uFxoIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "SecondStepDialing", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"SecondStepDialing",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->SecondStepDialing = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "CallerIdDetectionEnable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"CallerIdDetectionEnable",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->CallerIdDetectionEnable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"Enable",uVsIndex,uFxoIndex);

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

        BOOL TelcoVoiceMgrDml_POTS_FXOList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_FXOList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXOList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_FXOList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXOList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_FXOList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXOList_DiagTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_POTS_FXOList_DiagTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXO_CTRL_T pPotsFxoCtrl = (PDML_POTS_FXO_CTRL_T)hInsContext;

    PDML_POTS_FXO pPotsFxo = &(pPotsFxoCtrl->dml);

    PDML_POTS_FXO_DIAGTESTS pHEAD = &(pPotsFxo->DiagTests);

    if( AnscEqualString(ParamName, "TestSelector", TRUE) )
    {
        *puLong = pHEAD->TestSelector;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "TestResult", TRUE) )
    {
        *puLong = pHEAD->TestResult;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "DiagnosticsState", TRUE) )
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

        BOOL TelcoVoiceMgrDml_POTS_FXOList_DiagTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_POTS_FXOList_DiagTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxoIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXO_CTRL_T pPotsFxoCtrl = (PDML_POTS_FXO_CTRL_T)hInsContext;

    PDML_POTS_FXO pPotsFxo = &(pPotsFxoCtrl->dml);

    PDML_POTS_FXO_DIAGTESTS pHEAD = &(pPotsFxo->DiagTests);

    if(pPotsFxo != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pPotsFxo->pParentVoiceService;
    }

    if (pPotsFxo == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pPotsFxo or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uFxoIndex = pPotsFxo->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "TestSelector", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Battery","DialTone"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"DiagTests.TestSelector",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TestSelector = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "DiagnosticsState", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"None","Requested","Complete","Error_Internal","Error_Other"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXO"DiagTests.DiagnosticsState",uVsIndex,uFxoIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DiagnosticsState = uValue;

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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_FXSList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_POTS_FXSList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_POTS_FXSList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_POTS_FXSList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_POTS_FXSList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_POTS_FXS_LIST_T pPotsFxsList = &(pDmlVoiceService->POTS_obj.FXS);

    if(pPotsFxsList !=NULL )
    {
        ret = pPotsFxsList->ulQuantity;
    }
    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_POTS_FXSList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_POTS_FXSList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_POTS_FXS_LIST_T pFXSList = &(pDmlVoiceService->POTS_obj.FXS);

    if(pFXSList != NULL)
    {
        if(nIndex < pFXSList->ulQuantity)
        {
            PDML_POTS_FXS_CTRL_T pFXSCtrl = pFXSList->pdata[nIndex];
            if(pFXSCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pFXSCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXSList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPotsFxsIndex = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pHEAD = &(pPotsFxsCtrl->dml);

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

    uPotsFxsIndex = pHEAD->uInstanceNumber;

    if( AnscEqualString(ParamName, "TerminalType", TRUE) )
    {
        *puLong = pHEAD->TerminalType;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        //Fetch status from voice stack
        hal_param_t req_param;
        memset(&req_param, 0, sizeof(req_param));
        snprintf(req_param.name, sizeof(req_param.name), DML_VOICESERVICE_POTS_FXS_PARAM_NAME"%s", uVsIndex, uPotsFxsIndex, "Status");
        if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            *puLong = strtoul(req_param.value,NULL,10);
            ret = TRUE;
        }
        else
        {
            CcspTraceError(("%s:%d:: Status:get failed \n", __FUNCTION__, __LINE__));
            ret = FALSE;
        }
    }
    else if( AnscEqualString(ParamName, "ModemPassThrough", TRUE) )
    {
        *puLong = pHEAD->ModemPassThrough;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "FaxPassThrough", TRUE) )
    {
        *puLong = pHEAD->FaxPassThrough;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "DialType", TRUE) )
    {
        *puLong = pHEAD->DialType;
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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxsIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pHEAD = &(pPotsFxsCtrl->dml);

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

    uFxsIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "TerminalType", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Audio","Fax","Modem","Any"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"TerminalType",uVsIndex,uFxsIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TerminalType = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "ModemPassThrough", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Disable","Auto","Force"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"ModemPassThrough",uVsIndex,uFxsIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ModemPassThrough = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "FaxPassThrough", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Disable","Auto","Force"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"FaxPassThrough",uVsIndex,uFxsIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->FaxPassThrough = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "DialType", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Tone","Pulse"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"DialType",uVsIndex,uFxsIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DialType = uValue;

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

        ULONG TelcoVoiceMgrDml_POTS_FXSList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_POTS_FXSList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pHEAD = &(pPotsFxsCtrl->dml);

    if( AnscEqualString(ParamName, "ToneEventProfile", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->ToneEventProfile);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Name", TRUE) )
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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxsIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pHEAD = &(pPotsFxsCtrl->dml);

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

    uFxsIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "ToneEventProfile", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"ToneEventProfile",uVsIndex,uFxsIndex);

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

    TELCOVOICEMGR_UNLOCK()

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXSList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pHEAD = &(pPotsFxsCtrl->dml);

    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        *pBool = pHEAD->Enable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ClipGeneration", TRUE) )
    {
        *pBool = pHEAD->ClipGeneration;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ChargingPulse", TRUE) )
    {
        *pBool = pHEAD->ChargingPulse;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Active", TRUE) )
    {
        *pBool = pHEAD->Active;
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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxsIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pHEAD = &(pPotsFxsCtrl->dml);

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

    uFxsIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"Enable",uVsIndex,uFxsIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Enable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "ClipGeneration", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"ClipGeneration",uVsIndex,uFxsIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ClipGeneration = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "ChargingPulse", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"ChargingPulse",uVsIndex,uFxsIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ChargingPulse = bValue;

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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_FXSList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXSList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_FXSList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXSList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_FXSList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pPotsFxs = &(pPotsFxsCtrl->dml);

    PDML_POTS_FXS_VOICEPROCESSING pHEAD = &(pPotsFxs->VoiceProcessing);

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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pPotsFxs = &(pPotsFxsCtrl->dml);

    PDML_POTS_FXS_VOICEPROCESSING pHEAD = &(pPotsFxs->VoiceProcessing);

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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxsIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pPotsFxs = &(pPotsFxsCtrl->dml);

    PDML_POTS_FXS_VOICEPROCESSING pHEAD = &(pPotsFxs->VoiceProcessing);

    if(pPotsFxs != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pPotsFxs->pParentVoiceService;
    }

    if (pPotsFxs == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pPotsFxs or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uFxsIndex = pPotsFxs->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "TransmitGain", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"VoiceProcessing.TransmitGain",uVsIndex,uFxsIndex);

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
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"VoiceProcessing.ReceiveGain",uVsIndex,uFxsIndex);

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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pPotsFxs = &(pPotsFxsCtrl->dml);

    PDML_POTS_FXS_VOICEPROCESSING pHEAD = &(pPotsFxs->VoiceProcessing);

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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxsIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pPotsFxs = &(pPotsFxsCtrl->dml);

    PDML_POTS_FXS_VOICEPROCESSING pHEAD = &(pPotsFxs->VoiceProcessing);

    if(pPotsFxs != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pPotsFxs->pParentVoiceService;
    }

    if (pPotsFxs == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pPotsFxs or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uFxsIndex = pPotsFxs->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "EchoCancellationEnable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"VoiceProcessing.EchoCancellationEnable",uVsIndex,uFxsIndex);

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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_DiagTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_DiagTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pPotsFxs = &(pPotsFxsCtrl->dml);

    PDML_POTS_FXS_DIAGTESTS pHEAD = &(pPotsFxs->DiagTests);

    if( AnscEqualString(ParamName, "TestSelector", TRUE) )
    {
        *puLong = pHEAD->TestSelector;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "TestResult", TRUE) )
    {
        *puLong = pHEAD->TestResult;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "DiagnosticsState", TRUE) )
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

        BOOL TelcoVoiceMgrDml_POTS_FXSList_DiagTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_POTS_FXSList_DiagTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uFxsIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_FXS_CTRL_T pPotsFxsCtrl = (PDML_POTS_FXS_CTRL_T)hInsContext;

    PDML_POTS_FXS pPotsFxs = &(pPotsFxsCtrl->dml);

    PDML_POTS_FXS_DIAGTESTS pHEAD = &(pPotsFxs->DiagTests);

    if(pPotsFxs != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pPotsFxs->pParentVoiceService;
    }

    if (pPotsFxs == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pPotsFxs or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uFxsIndex = pPotsFxs->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "TestSelector", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Hazard Potential","Foreign Voltage","Resistive Faults","Off-hook","REN"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"DiagTests.TestSelector",uVsIndex,uFxsIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TestSelector = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "DiagnosticsState", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"None","Requested","Complete","Error_Internal","Error_Other"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_POTS_FXS"DiagTests.DiagnosticsState",uVsIndex,uFxsIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DiagnosticsState = uValue;

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

        BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_POTS_Ringer_EventList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_POTS_Ringer_EventList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_POTS_RINGER_EVT_LIST_T pPotsRingerEvtList = &(pDmlVoiceService->POTS_obj.Ringer_Obj.Event);

    if(pPotsRingerEvtList != NULL)
    {
        ret = pPotsRingerEvtList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_POTS_Ringer_EventList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
     ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_POTS_RINGER_EVT_LIST_T pEventList = &(pDmlVoiceService->POTS_obj.Ringer_Obj.Event);

    if(pEventList != NULL)
    {
        if(nIndex < pEventList->ulQuantity)
        {
            PDML_POTS_RINGER_EVT_CTRL_T pEventCtrl = pEventList->pdata[nIndex];
            if(pEventCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pEventCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_RINGER_EVT_CTRL_T pPotsRingerEvtCtrl = (PDML_POTS_RINGER_EVT_CTRL_T)hInsContext;

    PDML_POTS_RINGER_EVT pHEAD = &(pPotsRingerEvtCtrl->dml);

    if( AnscEqualString(ParamName, "Function", TRUE) )
    {
        *puLong = pHEAD->Function;
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

        ULONG TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_POTS_Ringer_EventList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_RINGER_EVT_CTRL_T pPotsRingerEvtCtrl = (PDML_POTS_RINGER_EVT_CTRL_T)hInsContext;

    PDML_POTS_RINGER_EVT pHEAD = &(pPotsRingerEvtCtrl->dml);

    if( AnscEqualString(ParamName, "Cadence", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Cadence);
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

        BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uEvtIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_POTS_RINGER_EVT_CTRL_T pPotsRingerEvtCtrl = (PDML_POTS_RINGER_EVT_CTRL_T)hInsContext;

    PDML_POTS_RINGER_EVT pHEAD = &(pPotsRingerEvtCtrl->dml);

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

    uEvtIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Cadence", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.POTS.Ringer.Event.%d.Cadence",uVsIndex,uEvtIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Cadence,pString);

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

        BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_POTS_Ringer_EventList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}
