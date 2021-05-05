#include "ansc_platform.h"
#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_v2.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_ClientList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_MGCP_ClientList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_MGCP_CLIENT_LIST_T pMGCPClientList = &(pDmlVoiceService->MGCP_obj.Client);

    if(pMGCPClientList != NULL)
    {
        ret = pMGCPClientList->ulQuantity;
    }
    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_MGCP_ClientList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_MGCP_ClientList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_MGCP_CLIENT_LIST_T pMgcpClientList = &(pDmlVoiceService->MGCP_obj.Client);

    if(pMgcpClientList != NULL)
    {
        if(nIndex < pMgcpClientList->ulQuantity)
        {
            PDML_MGCP_CLIENT_CTRL_T pMgcpClientCtrl = pMgcpClientList->pdata[nIndex];
            if(pMgcpClientCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pMgcpClientCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        *puLong = pHEAD->Status;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "RegisterMode", TRUE) )
    {
        *puLong = pHEAD->RegisterMode;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MaxSessions", TRUE) )
    {
        *puLong = pHEAD->MaxSessions;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "LocalPort", TRUE) )
    {
        *puLong = pHEAD->LocalPort;
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

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uClientIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

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

    uClientIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "RegisterMode", TRUE) )
    {
        char enumValue[][STR_LEN_32]={"Wildcard","Individual"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.RegisterMode",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->RegisterMode = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "MaxSessions", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.MaxSessions",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxSessions = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "LocalPort", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.LocalPort",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->LocalPort = uValue;

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

        ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

    if( AnscEqualString(ParamName, "User", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->User);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Network", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Network);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Domain", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Domain);
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

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uClientIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

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

    uClientIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "User", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.User",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->User,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Network", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.Network",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Network,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "Domain", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.Domain",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Domain,pString);

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

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

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

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uClientIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

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

    uClientIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "QuiescentMode", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.QuiescentMode",uVsIndex,uClientIndex);

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
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.Enable",uVsIndex,uClientIndex);

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

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_NetworkList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_MGCP_NetworkList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_MGCP_NETWORK_LIST_T pMGCPNetworkList = &(pDmlVoiceService->MGCP_obj.Network);

    if(pMGCPNetworkList != NULL)
    {
        ret = pMGCPNetworkList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_MGCP_NetworkList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_MGCP_NetworkList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T pVoiceService = (PDML_VOICE_SERVICE_CTRL_T) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_MGCP_NETWORK_LIST_T pMgcpNetworkList = &(pDmlVoiceService->MGCP_obj.Network);

    if(pMgcpNetworkList != NULL)
    {
        if(nIndex < pMgcpNetworkList->ulQuantity)
        {
            PDML_MGCP_NETWORK_CTRL_T pMgcpNetworCtrl = pMgcpNetworkList->pdata[nIndex];
            if(pMgcpNetworCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pMgcpNetworCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if( AnscEqualString(ParamName, "Status", TRUE) )
    {
        *puLong = pHEAD->Status;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "RetranIntervalTimer", TRUE) )
    {
        *puLong = pHEAD->RetranIntervalTimer;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "NonVoiceBandwidthReservedUpstream", TRUE) )
    {
        *puLong = pHEAD->NonVoiceBandwidthReservedUpstream;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "NonVoiceBandwidthReservedDownstream", TRUE) )
    {
        *puLong = pHEAD->NonVoiceBandwidthReservedDownstream;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MaxSessions", TRUE) )
    {
        *puLong = pHEAD->MaxSessions;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MaxRetranCount", TRUE) )
    {
        *puLong = pHEAD->MaxRetranCount;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "DSCPMark", TRUE) )
    {
        *puLong = pHEAD->DSCPMark;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallAgentPort2", TRUE) )
    {
        *puLong = pHEAD->CallAgentPort2;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallAgentPort1", TRUE) )
    {
        *puLong = pHEAD->CallAgentPort1;
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

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uNetworkIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

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

    uNetworkIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "RetranIntervalTimer", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.RetranIntervalTimer",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->RetranIntervalTimer = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "NonVoiceBandwidthReservedUpstream", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.NonVoiceBandwidthReservedUpstream",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->NonVoiceBandwidthReservedUpstream = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "NonVoiceBandwidthReservedDownstream", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.NonVoiceBandwidthReservedDownstream",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->NonVoiceBandwidthReservedDownstream = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "MaxSessions", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.MaxSessions",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxSessions = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "MaxRetranCount", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.MaxRetranCount",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxRetranCount = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "DSCPMark", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.DSCPMark",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DSCPMark = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "CallAgentPort2", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CallAgentPort2",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->CallAgentPort2 = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "CallAgentPort1", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CallAgentPort1",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->CallAgentPort1 = uValue;

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

        ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if( AnscEqualString(ParamName, "VoIPProfile", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->VoIPProfile);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "STUNServer", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->STUNServer);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "CodecList", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->CodecList);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "CallAgent2", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->CallAgent2);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "CallAgent1", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->CallAgent1);
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

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uNetworkIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

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

    uNetworkIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "VoIPProfile", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.VoIPProfile",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->VoIPProfile,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "STUNServer", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.STUNServer",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->STUNServer,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "CodecList", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CodecList",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->CodecList,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "CallAgent2", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CallAgent2",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->CallAgent2,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "CallAgent1", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CallAgent1",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->CallAgent1,pString);

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

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

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

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if( AnscEqualString(ParamName, "VLANIDMark", TRUE) )
    {
        *pInt = pHEAD->VLANIDMark;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
    {
        *pInt = pHEAD->EthernetPriorityMark;
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

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uNetworkIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

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

    uNetworkIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "VLANIDMark", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.VLANIDMark",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->VLANIDMark = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.EthernetPriorityMark",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->EthernetPriorityMark = iValue;

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

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if( AnscEqualString(ParamName, "STUNEnable", TRUE) )
    {
        *pBool = pHEAD->STUNEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "SendRSIPImmediately", TRUE) )
    {
        *pBool = pHEAD->SendRSIPImmediately;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "QuiescentMode", TRUE) )
    {
        *pBool = pHEAD->QuiescentMode;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        *pBool = pHEAD->Enable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AllowPiggybackEvents", TRUE) )
    {
        *pBool = pHEAD->AllowPiggybackEvents;
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

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uNetworkIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

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

    uNetworkIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "STUNEnable", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.STUNEnable",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->STUNEnable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "SendRSIPImmediately", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.SendRSIPImmediately",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->SendRSIPImmediately = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "QuiescentMode", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.QuiescentMode",uVsIndex,uNetworkIndex);

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
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.Enable",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Enable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if( AnscEqualString(ParamName, "AllowPiggybackEvents", TRUE) )
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.AllowPiggybackEvents",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->AllowPiggybackEvents = bValue;

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

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}
