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

        BOOL VoiceService_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL VoiceService_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG VoiceService_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG VoiceService_Synchronize(ANSC_HANDLE hInsContext)
{
    /* No need to query all objects from voice hal for each dmcli get request */
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG VoiceService_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG VoiceService_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    PTELCOVOICEMGR_DATAMODEL_SERVICES     pMyObject  = (PTELCOVOICEMGR_DATAMODEL_SERVICES)g_pTelcoVoiceBEManager->hServices;

    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData  =  pMyObject->data;

    if(pTelcoVoiceMgrDmlData != NULL)
    {
       ret = pTelcoVoiceMgrDmlData->Service.VoiceService.ulQuantity;
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE VoiceService_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE VoiceService_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlDataLock = TelcoVoiceMgrDmlGetDataLocked();

    if(pTelcoVoiceMgrDmlDataLock == NULL)
    {
        CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DATAMODEL_SERVICES     pMyObject  = (PTELCOVOICEMGR_DATAMODEL_SERVICES)g_pTelcoVoiceBEManager->hServices;

    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = pMyObject->data;

    if(pTelcoVoiceMgrDmlData != NULL)
    {

        pTelcoVoiceMgrDmlData->Service.VoiceService.ulQuantity = 1;

        if(nIndex < pTelcoVoiceMgrDmlData->Service.VoiceService.ulQuantity)
        {

            DML_VOICE_SERVICE_CTRL_T* pDmlVoiceService = pTelcoVoiceMgrDmlData->Service.VoiceService.pdata[nIndex];

            if(pDmlVoiceService != NULL)
            {

               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pDmlVoiceService;
            }

        }

    }

    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlDataLock);

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceService_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL VoiceService_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    TELCOVOICEMGR_VOICE_STATUS_ENUM voiceStatus;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T pHEADCtrl = (PDML_VOICE_SERVICE_CTRL_T)hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "X_RDK_Status", TRUE) )
    {
        if(TelcoVoiceMgrDmlGetVoiceProcessStatus(pHEAD->InstanceNumber, &voiceStatus) == ANSC_STATUS_SUCCESS)
        {
            pHEAD->X_RDK_Status = voiceStatus;
            *puLong = pHEAD->X_RDK_Status;
            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "X_RDK_Enable", TRUE) )
    {
        *puLong = pHEAD->X_RDK_Enable;
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

        BOOL VoiceService_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL VoiceService_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
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

    if (pVoiceService == NULL)
    {
        CcspTraceError(("%s:%d:: pVoiceService NULL\n", __FUNCTION__, __LINE__));
        TELCOVOICEMGR_UNLOCK()
        return ret;
    }

    uVsIndex = pVoiceService->InstanceNumber;
    
    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "X_RDK_Enable", TRUE) )
    {
        TELCOVOICEMGR_VOICE_ENABLE_ENUM eState = uValue;
        if(TelcoVoiceMgrDmlSetVoiceProcessState(uVsIndex, eState) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()
            pVoiceService->X_RDK_Enable = eState;
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

        BOOL VoiceService_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL VoiceService_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }
    
    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T  pHEADCtrl = (PDML_VOICE_SERVICE_CTRL_T)hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "X_RDK_FactoryReset", TRUE) )
    {
        *pBool = FALSE;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "X_RDK_DisableLoopCurrentUntilRegistered", TRUE) )
    {
        *pBool = pHEAD->X_RDK_DisableLoopCurrentUntilRegistered;
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

        BOOL VoiceService_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL VoiceService_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
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

    if (pVoiceService == NULL)
    {
            TELCOVOICEMGR_UNLOCK()
            CcspTraceError(("%s:%d:: pVoiceService NULL\n", __FUNCTION__, __LINE__));
            return ret;
    }
    uVsIndex = pVoiceService->InstanceNumber;
    ULONG VoiceState = pVoiceService->X_RDK_Enable;
    TELCOVOICEMGR_UNLOCK()
    if( AnscEqualString(ParamName, "X_RDK_DisableLoopCurrentUntilRegistered", TRUE))
    {
        if (TelcoVoiceMgrDmlSetLoopCurrentDisabled(uVsIndex, bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pVoiceService->X_RDK_DisableLoopCurrentUntilRegistered = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "X_RDK_FactoryReset", TRUE))
    {
        if(TRUE == bValue)
        {
            if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlFactoryReset(uVsIndex, VoiceState))
            {
                CcspTraceInfo(("%s:%d:: TelcoVoiceMgrDmlFactoryReset Done\n", __FUNCTION__, __LINE__));
                ret = TRUE;
            }
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

        ULONG VoiceService_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG VoiceService_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    int ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }
    
    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T  pHEADCtrl = (PDML_VOICE_SERVICE_CTRL_T)hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "X_RDK_IpAddressFamily", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->X_RDK_IpAddressFamily);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "X_RDK_BoundIpAddr", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->X_RDK_BoundIpAddr);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "X_RDK_BoundIfName", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->X_RDK_BoundIfName);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Alias);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "X_RDK_Firewall_Rule_Data", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->X_RDK_Firewall_Rule_Data);
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
        ret = 1;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceService_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL VoiceService_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
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
        TELCOVOICEMGR_UNLOCK()
        CcspTraceError(("%s:%d:: pVoiceService or pString NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    uVsIndex = pVoiceService->InstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "X_RDK_BoundIfName", TRUE) )
    {
        if(TelcoVoiceMgrDmlSetBoundIfname(pVoiceService->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pVoiceService->X_RDK_BoundIfName, pString);

            TELCOVOICEMGR_UNLOCK()

            (void)storeObjectString(uVsIndex, 1, 1, 1, "Set-BoundIfName", pString);

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "X_RDK_IpAddressFamily", TRUE) )
    {
        if(TelcoVoiceMgrDmlSetIpAddressFamily(pVoiceService->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pVoiceService->X_RDK_IpAddressFamily, pString);

            TELCOVOICEMGR_UNLOCK()

            (void)storeObjectString(uVsIndex, 1, 1, 1, "Set-IpAddressFamily", pString);

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        TELCOVOICEMGR_LOCK_OR_EXIT()

        AnscCopyString(pVoiceService->Alias, pString);

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

        BOOL VoiceService_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL VoiceService_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceService_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL VoiceService_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceService_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL VoiceService_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}
