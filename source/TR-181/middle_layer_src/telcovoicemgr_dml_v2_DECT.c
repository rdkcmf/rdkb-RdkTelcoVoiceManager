#include "ansc_platform.h"
#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_v2.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"

#define HAL_DML_VOICESERVICE_DECT_BASE                         "Device.Services.VoiceService.%d.DECT.Base.%d."
#define HAL_DML_VOICESERVICE_DECT_PORTABLE                     "Device.Services.VoiceService.%d.DECT.Portable.%d."

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_DECT_BaseList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_DECT_BaseList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_DECT_BaseList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_DECT_BaseList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_DECT_BaseList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_DECT_BaseList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_DECT_BASE_LIST_T pDectBaseList = &(pDmlVoiceService->DECT_obj.Base);

    if(pDectBaseList != NULL)
    {
        ret = pDectBaseList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_DECT_BaseList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_DECT_BaseList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_DECT_BASE_LIST_T pBaseList = &(pDmlVoiceService->DECT_obj.Base);

    if(pBaseList != NULL)
    {
        if(nIndex < pBaseList->ulQuantity)
        {
            PDML_DECT_BASE_CTRL_T pBaseCtrl = pBaseList->pdata[nIndex];
            if(pBaseCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pBaseCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_DECT_BaseList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_DECT_BaseList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_BASE_CTRL_T pDectBaseCtrl = (PDML_DECT_BASE_CTRL_T)hInsContext;

    PDML_DECT_BASE pHEAD = &(pDectBaseCtrl->dml);

    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        *puLong = pHEAD->Status;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Standard", TRUE) )
    {
        *puLong = pHEAD->Standard;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "RFPowerControl", TRUE) )
    {
        *puLong = pHEAD->RFPowerControl;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MaxSupportedPP", TRUE) )
    {
        *puLong = pHEAD->MaxSupportedPP;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "EncryptionType", TRUE) )
    {
        *puLong = pHEAD->EncryptionType;
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

        BOOL TelcoVoiceMgrDml_DECT_BaseList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_DECT_BaseList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uBaseIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_BASE_CTRL_T pDectBaseCtrl = (PDML_DECT_BASE_CTRL_T)hInsContext;

    PDML_DECT_BASE pHEAD = &(pDectBaseCtrl->dml);

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

    uBaseIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "RFPowerControl", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Normal","Reduced"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_BASE"RFPowerControl",uVsIndex,uBaseIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->RFPowerControl = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "MaxSupportedPP", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_BASE"MaxSupportedPP",uVsIndex,uBaseIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxSupportedPP = uValue;

            TELCOVOICEMGR_UNLOCK()

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

        ULONG TelcoVoiceMgrDml_DECT_BaseList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_DECT_BaseList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_BASE_CTRL_T pDectBaseCtrl = (PDML_DECT_BASE_CTRL_T)hInsContext;

    PDML_DECT_BASE pHEAD = &(pDectBaseCtrl->dml);

    if( AnscEqualString(ParamName, "ToneEventProfile", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->ToneEventProfile);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "RFPI", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->RFPI);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "PIN", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->PIN);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Name", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Name);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "HardwareVersion", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->HardwareVersion);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "FirmwareVersion", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->FirmwareVersion);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "EepromVersion", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->EepromVersion);
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

        BOOL TelcoVoiceMgrDml_DECT_BaseList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_DECT_BaseList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uBaseIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_BASE_CTRL_T pDectBaseCtrl = (PDML_DECT_BASE_CTRL_T)hInsContext;

    PDML_DECT_BASE pHEAD = &(pDectBaseCtrl->dml);

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

    uBaseIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "ToneEventProfile", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_BASE"ToneEventProfile",uVsIndex,uBaseIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->ToneEventProfile,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "PIN", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_BASE"PIN",uVsIndex,uBaseIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->PIN,pString);

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

        BOOL TelcoVoiceMgrDml_DECT_BaseList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_DECT_BaseList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_BASE_CTRL_T pDectBaseCtrl = (PDML_DECT_BASE_CTRL_T)hInsContext;

    PDML_DECT_BASE pHEAD = &(pDectBaseCtrl->dml);

    if( AnscEqualString(ParamName, "SubscriptionEnable", TRUE) )
    {
        *pBool = pHEAD->SubscriptionEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "RepeaterSupportEnabled", TRUE) )
    {
        *pBool = pHEAD->RepeaterSupportEnabled;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "NEMOEnable", TRUE) )
    {
        *pBool = pHEAD->NEMOEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CipheringEnable", TRUE) )
    {
        *pBool = pHEAD->CipheringEnable;
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

        BOOL TelcoVoiceMgrDml_DECT_BaseList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_DECT_BaseList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uBaseIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_BASE_CTRL_T pDectBaseCtrl = (PDML_DECT_BASE_CTRL_T)hInsContext;

    PDML_DECT_BASE pHEAD = &(pDectBaseCtrl->dml);

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

    uBaseIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "SubscriptionEnable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_BASE"SubscriptionEnable",uVsIndex,uBaseIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->SubscriptionEnable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "NEMOEnable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_BASE"NEMOEnable",uVsIndex,uBaseIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->NEMOEnable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "CipheringEnable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_BASE"CipheringEnable",uVsIndex,uBaseIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->CipheringEnable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_BASE"Enable",uVsIndex,uBaseIndex);

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

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_DECT_BaseList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_DECT_BaseList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_DECT_BaseList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_DECT_BaseList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_DECT_BaseList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_DECT_BaseList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_DECT_BaseList_Stats_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_DECT_BaseList_Stats_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_BASE_CTRL_T pDectBaseCtrl = (PDML_DECT_BASE_CTRL_T)hInsContext;

    PDML_DECT_BASE pDectBase = &(pDectBaseCtrl->dml);

    PDML_DECT_BASE_STATS pHEAD = &(pDectBase->Stats);

    if( AnscEqualString(ParamName, "SyncFailures", TRUE) )
    {
        *puLong = pHEAD->SyncFailures;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PayloadFieldErrors", TRUE) )
    {
        *puLong = pHEAD->PayloadFieldErrors;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Handovers", TRUE) )
    {
        *puLong = pHEAD->Handovers;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "HandoverFailures", TRUE) )
    {
        *puLong = pHEAD->HandoverFailures;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ControlFieldErrors", TRUE) )
    {
        *puLong = pHEAD->ControlFieldErrors;
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

        BOOL TelcoVoiceMgrDml_DECT_PortableList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_DECT_PortableList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_DECT_PortableList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_DECT_PortableList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_DECT_PortableList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_DECT_PortableList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_DECT_PORTABLE_LIST_T pDectPortableList = &(pDmlVoiceService->DECT_obj.Portable);

    if(pDectPortableList != NULL)
    {
        ret = pDectPortableList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_DECT_PortableList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_DECT_PortableList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_DECT_PORTABLE_LIST_T pPortableList = &(pDmlVoiceService->DECT_obj.Portable);

    if(pPortableList != NULL)
    {
        if(nIndex < pPortableList->ulQuantity)
        {
            PDML_DECT_PORTABLE_CTRL_T pPortableCtrl = pPortableList->pdata[nIndex];
            if(pPortableCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pPortableCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_DECT_PortableList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_DECT_PortableList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_PORTABLE_CTRL_T pDectPortableCtrl = (PDML_DECT_PORTABLE_CTRL_T)hInsContext;

    PDML_DECT_PORTABLE pHEAD = &(pDectPortableCtrl->dml);

    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        *puLong = pHEAD->Status;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "RegistrationStatus", TRUE) )
    {
        *puLong = pHEAD->RegistrationStatus;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PortableType", TRUE) )
    {
        *puLong = pHEAD->PortableType;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Control", TRUE) )
    {
        *puLong = pHEAD->Control;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "IPUILength", TRUE) )
    {
        *puLong = pHEAD->IPUILength;
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

        BOOL TelcoVoiceMgrDml_DECT_PortableList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_DECT_PortableList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPortableIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_PORTABLE_CTRL_T pDectBaseCtrl = (PDML_DECT_PORTABLE_CTRL_T)hInsContext;

    PDML_DECT_PORTABLE pHEAD = &(pDectBaseCtrl->dml);

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

    uPortableIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Control", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Unregister","Disable"};

        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_PORTABLE"Control",uVsIndex,uPortableIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Control = uValue;

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

        ULONG TelcoVoiceMgrDml_DECT_PortableList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_DECT_PortableList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_PORTABLE_CTRL_T pDectPortableCtrl = (PDML_DECT_PORTABLE_CTRL_T)hInsContext;

    PDML_DECT_PORTABLE pHEAD = &(pDectPortableCtrl->dml);

    if( AnscEqualString(ParamName, "SubscriptionTime", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->SubscriptionTime);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "SoftwareVersion", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->SoftwareVersion);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "PARK", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->PARK);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "OperatorName", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->OperatorName);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "LastUpdateDateTime", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->LastUpdateDateTime);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "IPUI", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->IPUI);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "IPEI", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->IPEI);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "HardwareVersion", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->HardwareVersion);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "CodecList", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->CodecList);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "BaseAttachedTo", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->BaseAttachedTo);
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

        BOOL TelcoVoiceMgrDml_DECT_PortableList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_DECT_PortableList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPortableIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_PORTABLE_CTRL_T pDectBaseCtrl = (PDML_DECT_PORTABLE_CTRL_T)hInsContext;

    PDML_DECT_PORTABLE pHEAD = &(pDectBaseCtrl->dml);

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

    uPortableIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "OperatorName", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_PORTABLE"OperatorName",uVsIndex,uPortableIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->OperatorName,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "CodecList", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_PORTABLE"CodecList",uVsIndex,uPortableIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->CodecList,pString);

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

        BOOL TelcoVoiceMgrDml_DECT_PortableList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_DECT_PortableList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_PORTABLE_CTRL_T pDectPortableCtrl = (PDML_DECT_PORTABLE_CTRL_T)hInsContext;

    PDML_DECT_PORTABLE pHEAD = &(pDectPortableCtrl->dml);

    if( AnscEqualString(ParamName, "SoftwareUpgrade", TRUE) )
    {
        *pBool = pHEAD->SoftwareUpgrade;
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

        BOOL TelcoVoiceMgrDml_DECT_PortableList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_DECT_PortableList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPortableIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_DECT_PORTABLE_CTRL_T pDectBaseCtrl = (PDML_DECT_PORTABLE_CTRL_T)hInsContext;

    PDML_DECT_PORTABLE pHEAD = &(pDectBaseCtrl->dml);

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

    uPortableIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, HAL_DML_VOICESERVICE_DECT_PORTABLE"Enable",uVsIndex,uPortableIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,bValue) == ANSC_STATUS_SUCCESS)
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

        BOOL TelcoVoiceMgrDml_DECT_PortableList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_DECT_PortableList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_DECT_PortableList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_DECT_PortableList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_DECT_PortableList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_DECT_PortableList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}
