#include "ansc_platform.h"
#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_v2.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_Tone_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
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

    PDML_TONE pHEAD = &(pDmlVoiceService->Tone_obj);

    if( AnscEqualString(ParamName, "DefautEventProfile", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->DefautEventProfile);
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

        BOOL TelcoVoiceMgrDml_Tone_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_Tone_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
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

    PDML_TONE pHEAD = &(pDmlVoiceService->Tone_obj);

    if (pHEAD == NULL  || pString == NULL )
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pString NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "DefautEventProfile", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.DefautEventProfile",uVsIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->DefautEventProfile, pString);

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

        BOOL TelcoVoiceMgrDml_Tone_DescList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_DescList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_DescList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_Tone_DescList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_DescList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_Tone_DescList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_TONE_DESCRIPTION_LIST_T pToneDescList = &(pDmlVoiceService->Tone_obj.Description);

    if(pToneDescList != NULL)
    {
        ret = pToneDescList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_Tone_DescList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_Tone_DescList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T pVoiceService = (PDML_VOICE_SERVICE_CTRL_T) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_TONE_DESCRIPTION_LIST_T pDescList = &(pDmlVoiceService->Tone_obj.Description);

    if(pDescList != NULL)
    {
        if(nIndex < pDescList->ulQuantity)
        {
            PDML_TONE_DESCRIPTION_CTRL_T pDescCtrl = pDescList->pdata[nIndex];
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

        BOOL TelcoVoiceMgrDml_Tone_DescList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_Tone_DescList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TONE_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TONE_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "ToneRepetitions", TRUE) )
    {
        *puLong = pHEAD->ToneRepetitions;
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

        BOOL TelcoVoiceMgrDml_Tone_DescList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_Tone_DescList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uDescIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TONE_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TONE_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

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

    uDescIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "ToneRepetitions", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Description.%d.ToneRepetitions",uVsIndex,uDescIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ToneRepetitions = uValue;

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

        ULONG TelcoVoiceMgrDml_Tone_DescList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_Tone_DescList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TONE_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TONE_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "ToneText", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->ToneText);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "TonePattern", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->TonePattern);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "ToneName", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->ToneName);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "ToneFile", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->ToneFile);
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

    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_DescList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_Tone_DescList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uDescIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TONE_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TONE_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

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

    uDescIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "ToneText", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Description.%d.ToneText",uVsIndex,uDescIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->ToneText,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "TonePattern", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Description.%d.TonePattern",uVsIndex,uDescIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->TonePattern,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "ToneName", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Description.%d.ToneName",uVsIndex,uDescIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->ToneName,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "ToneFile", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Description.%d.ToneFile",uVsIndex,uDescIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->ToneFile,pString);

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

        BOOL TelcoVoiceMgrDml_Tone_DescList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_Tone_DescList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TONE_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TONE_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "ToneEnable", TRUE) )
    {
        *pBool = pHEAD->ToneEnable;
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

        BOOL TelcoVoiceMgrDml_Tone_DescList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_Tone_DescList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uDescIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_DESCRIPTION_CTRL_T pHEADCtrl = (PDML_TONE_DESCRIPTION_CTRL_T)hInsContext;

    PDML_TONE_DESCRIPTION pHEAD = &(pHEADCtrl->dml);

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

    uDescIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "ToneEnable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Description.%d.ToneEnable",uVsIndex,uDescIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ToneEnable = bValue;

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

        BOOL TelcoVoiceMgrDml_Tone_DescList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_DescList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_DescList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_DescList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_DescList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_DescList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_PatternList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_PatternList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_PatternList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_Tone_PatternList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_PatternList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_Tone_PatternList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_TONE_PATTERN_LIST_T pTonePatternList = &(pDmlVoiceService->Tone_obj.Pattern);

    if(pTonePatternList != NULL)
    {
        ret = pTonePatternList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_Tone_PatternList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_Tone_PatternList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T pVoiceService = (PDML_VOICE_SERVICE_CTRL_T) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_TONE_PATTERN_LIST_T pPatternList = &(pDmlVoiceService->Tone_obj.Pattern);

    if(pPatternList != NULL)
    {
        if(nIndex < pPatternList->ulQuantity)
        {
            PDML_TONE_PATTERN_CTRL_T pPatternCtrl = pPatternList->pdata[nIndex];
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

        BOOL TelcoVoiceMgrDml_Tone_PatternList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_Tone_PatternList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_PATTERN_CTRL_T pHEADCtrl = (PDML_TONE_PATTERN_CTRL_T)hInsContext;

    PDML_TONE_PATTERN pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "ModulationFrequency", TRUE) )
    {
        *puLong = pHEAD->ModulationFrequency;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Frequency4", TRUE) )
    {
        *puLong = pHEAD->Frequency4;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Frequency3", TRUE) )
    {
        *puLong = pHEAD->Frequency3;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Frequency2", TRUE) )
    {
        *puLong = pHEAD->Frequency2;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Frequency1", TRUE) )
    {
        *puLong = pHEAD->Frequency1;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Duration", TRUE) )
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

        BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPatternIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_PATTERN_CTRL_T pHEADCtrl = (PDML_TONE_PATTERN_CTRL_T)hInsContext;

    PDML_TONE_PATTERN pHEAD = &(pHEADCtrl->dml);

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

    uPatternIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "ModulationFrequency", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.ModulationFrequency",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ModulationFrequency = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Frequency4", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Frequency4",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Frequency4 = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Frequency3", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Frequency3",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Frequency3 = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Frequency2", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Frequency2",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Frequency2 = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Frequency1", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Frequency1",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Frequency1 = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Duration", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Duration",uVsIndex,uPatternIndex);

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

        ULONG TelcoVoiceMgrDml_Tone_PatternList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_Tone_PatternList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_PATTERN_CTRL_T pHEADCtrl = (PDML_TONE_PATTERN_CTRL_T)hInsContext;

    PDML_TONE_PATTERN pHEAD = &(pHEADCtrl->dml);

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

        BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPatternIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_PATTERN_CTRL_T pHEADCtrl = (PDML_TONE_PATTERN_CTRL_T)hInsContext;

    PDML_TONE_PATTERN pHEAD = &(pHEADCtrl->dml);

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

    uPatternIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "NextPattern", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.NextPattern",uVsIndex,uPatternIndex);

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

        BOOL TelcoVoiceMgrDml_Tone_PatternList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

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

BOOL TelcoVoiceMgrDml_Tone_PatternList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_PATTERN_CTRL_T pHEADCtrl = (PDML_TONE_PATTERN_CTRL_T)hInsContext;

    PDML_TONE_PATTERN pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "Power4", TRUE) )
    {
        *pInt = pHEAD->Power4;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Power3", TRUE) )
    {
        *pInt = pHEAD->Power3;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Power2", TRUE) )
    {
        *pInt = pHEAD->Power2;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Power1", TRUE) )
    {
        *pInt = pHEAD->Power1;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ModulationPower", TRUE) )
    {
        *pInt = pHEAD->ModulationPower;
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

        BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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

BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPatternIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_PATTERN_CTRL_T pHEADCtrl = (PDML_TONE_PATTERN_CTRL_T)hInsContext;

    PDML_TONE_PATTERN pHEAD = &(pHEADCtrl->dml);

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

    uPatternIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Power4", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Power4",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Power4 = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Power3", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Power3",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Power3 = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Power2", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Power2",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Power2 = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Power1", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Power1",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Power1 = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "ModulationPower", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.ModulationPower",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ModulationPower = iValue;

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

        BOOL TelcoVoiceMgrDml_Tone_PatternList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_Tone_PatternList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_PATTERN_CTRL_T pHEADCtrl = (PDML_TONE_PATTERN_CTRL_T)hInsContext;

    PDML_TONE_PATTERN pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "ToneOn", TRUE) )
    {
        *pBool = pHEAD->ToneOn;
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

        BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_Tone_PatternList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPatternIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_PATTERN_CTRL_T pHEADCtrl = (PDML_TONE_PATTERN_CTRL_T)hInsContext;

    PDML_TONE_PATTERN pHEAD = &(pHEADCtrl->dml);

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

    uPatternIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "ToneOn", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.ToneOn",uVsIndex,uPatternIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ToneOn = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.Pattern.%d.Enable",uVsIndex,uPatternIndex);

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

        BOOL TelcoVoiceMgrDml_Tone_PatternList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_PatternList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_PatternList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_PatternList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_PatternList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_PatternList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_EventProfileList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_EventProfileList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_Tone_EventProfileList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_TONE_EVENTPROFILE_LIST_T pToneEvtProfList = &(pDmlVoiceService->Tone_obj.EventProfile);

    if(pToneEvtProfList != NULL)
    {
        ret = pToneEvtProfList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_Tone_EventProfileList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_Tone_EventProfileList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T pVoiceService = (PDML_VOICE_SERVICE_CTRL_T) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_TONE_EVENTPROFILE_LIST_T pEvtProfileList = &(pDmlVoiceService->Tone_obj.EventProfile);

    if(pEvtProfileList != NULL)
    {
        if(nIndex < pEvtProfileList->ulQuantity)
        {
            PDML_TONE_EVENTPROFILE_CTRL_T pEvtProfileCtrl = pEvtProfileList->pdata[nIndex];
            if(pEvtProfileCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pEvtProfileCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_EventProfileList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_EVENTPROFILE_CTRL_T pHEADCtrl = (PDML_TONE_EVENTPROFILE_CTRL_T)hInsContext;

    PDML_TONE_EVENTPROFILE pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "Alias", TRUE) )
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

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pString == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_EVENTPROFILE_CTRL_T pHEADCtrl = (PDML_TONE_EVENTPROFILE_CTRL_T)hInsContext;

    PDML_TONE_EVENTPROFILE pHEAD = &(pHEADCtrl->dml);


    if (pHEAD == NULL )
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Alias", TRUE) )
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

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_TONE_EVENTPROFILE_CTRL_T* pToneEvtProfileList = (DML_TONE_EVENTPROFILE_CTRL_T*) hInsContext;

    PDML_TONE_EVENTPROFILE pDmlToneEvtProfile = &(pToneEvtProfileList->dml);

    PDML_TONE_EVENTPROFILE_EVENT_LIST_T pToneEvtProfileEvtList = &(pDmlToneEvtProfile->Event);

    if(pToneEvtProfileEvtList != NULL)
    {
        ret = pToneEvtProfileEvtList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_EVENTPROFILE_CTRL_T pEvtProfileCtrl = (PDML_TONE_EVENTPROFILE_CTRL_T) hInsContext;

    PDML_TONE_EVENTPROFILE pDmlEvtProfile = &(pEvtProfileCtrl->dml);

    PDML_TONE_EVENTPROFILE_EVENT_LIST_T pEventList = &(pDmlEvtProfile->Event);

    if(pEventList != NULL)
    {
        if(nIndex < pEventList->ulQuantity)
        {
            PDML_TONE_EVENTPROFILE_EVENT_CTRL_T pEventCtrl = pEventList->pdata[nIndex];
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

        ULONG TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_EVENTPROFILE_EVENT_CTRL_T pHEADCtrl = (PDML_TONE_EVENTPROFILE_EVENT_CTRL_T)hInsContext;

    PDML_TONE_EVENTPROFILE_EVENT pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "Tone", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Tone);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Function", TRUE) )
    {
        //AnscCopyString(pValue,pHEAD->Function);
        //ret = 0;
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

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uEvtProfIndex = 0;
    ULONG uEvtIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    PDML_TONE_EVENTPROFILE pDmlEvtProf = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_TONE_EVENTPROFILE_EVENT_CTRL_T pHEADCtrl = (PDML_TONE_EVENTPROFILE_EVENT_CTRL_T)hInsContext;

    PDML_TONE_EVENTPROFILE_EVENT pHEAD = &(pHEADCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlEvtProf = (PDML_TONE_EVENTPROFILE)pHEAD->pParentToneEvtProfile;
    }

    if(pDmlEvtProf != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlEvtProf->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlEvtProf == NULL || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlEvtProf or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uEvtProfIndex = pDmlEvtProf->uInstanceNumber;

    uEvtIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Tone", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.EventProfile.%d.Event.%d.Tone",uVsIndex,uEvtProfIndex,uEvtIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Tone,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Function", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Busy","Confirmation","Dial","LineMessagesWaiting","OffHookWarning",
        "RingBack","ReOrder","Stutterdial","CallWaiting1","CallWaiting2","CallWaiting3","CallWaiting4",
        "AlertingSignal","SpecialDial","SpecialInfo","Release","Congestion","UserDefined1","UserDefined2",
        "UserDefined3","UserDefined4"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.Tone.EventProfile.%d.Event.%d.Function",uVsIndex,uEvtProfIndex,uEvtIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            //AnscCopyString(pHEAD->Function,pString);

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

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_Tone_EventProfileList_EvtList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}
