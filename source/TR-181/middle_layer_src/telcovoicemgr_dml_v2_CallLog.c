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

        BOOL TelcoVoiceMgrDml_CallLogList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_CallLogList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_CallLogList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_CallLogList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_CallLogList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_CALLLOG_LIST_T  pCallLogList = pDmlVoiceService->CallLog;

    if(pCallLogList != NULL)
    {
        ret = pCallLogList->ulQuantity;
    }
    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_CallLogList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_CallLogList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T pVoiceService = (PDML_VOICE_SERVICE_CTRL_T) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_CALLLOG_LIST_T pCallLogList = pDmlVoiceService->CallLog;

    if(pCallLogList != NULL)
    {
        if(nIndex < pCallLogList->ulQuantity)
        {
            PDML_CALLLOG_CTRL_T pCallLogCtrl = pCallLogList->pdata[nIndex];
            if(pCallLogCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pCallLogCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_CallLogList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_CTRL_T pHEADCtrl = (PDML_CALLLOG_CTRL_T)hInsContext;

    PDML_CALLLOG pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "Duration", TRUE) )
    {
        *puLong = pHEAD->Duration;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Direction", TRUE) )
    {
        *puLong = pHEAD->Direction;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallTerminationCause", TRUE) )
    {
        *puLong = pHEAD->CallTerminationCause;
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

        ULONG TelcoVoiceMgrDml_CallLogList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_CTRL_T pHEADCtrl = (PDML_CALLLOG_CTRL_T)hInsContext;

    PDML_CALLLOG pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "UsedLine", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->UsedLine);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "UsedExtensions", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->UsedExtensions);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Start", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Start);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Source", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Source);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "RemoteParty", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->RemoteParty);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Destination", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Destination);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "CallingPartyNumber", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->CallingPartyNumber);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "CalledPartyNumber", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->CalledPartyNumber);
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

        BOOL TelcoVoiceMgrDml_CallLogList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_CallLogList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pString == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_CTRL_T pHEADCtrl = (PDML_CALLLOG_CTRL_T)hInsContext;

    PDML_CALLLOG pHEAD = &(pHEADCtrl->dml);

    if (pHEAD == NULL )
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD  NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    TELCOVOICEMGR_UNLOCK()

    if( AnscEqualString(ParamName, "Alias", TRUE) )
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

        BOOL TelcoVoiceMgrDml_CallLogList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_CallLogList_SignalPerfList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_CallLogList_SignalPerfList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_CALLLOG_CTRL_T* pCallLogCtrl = (DML_CALLLOG_CTRL_T*) hInsContext;

    PDML_CALLLOG pDmlCallLog = &(pCallLogCtrl->dml);

    PDML_CALLLOG_SIGNALINGPERF_LIST_T pCallLogSignPerfList = &(pDmlCallLog->SignalingPerformance);

    if(pCallLogSignPerfList != NULL)
    {
        ret = pCallLogSignPerfList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_CTRL_T pCallLogCtrl = (PDML_CALLLOG_CTRL_T) hInsContext;

    PDML_CALLLOG pDmlCallLog = &(pCallLogCtrl->dml);

    PDML_CALLLOG_SIGNALINGPERF_LIST_T pSignalPerfList = &(pDmlCallLog->SignalingPerformance);

    if(pSignalPerfList != NULL)
    {
        if(nIndex < pSignalPerfList->ulQuantity)
        {
            PDML_CALLLOG_SIGNALINGPERF_CTRL_T pSignalPerfCtrl = pSignalPerfList->pdata[nIndex];
            if(pSignalPerfCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pSignalPerfCtrl;
            }
        }
    }
    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SIGNALINGPERF_CTRL_T pHEADCtrl = (PDML_CALLLOG_SIGNALINGPERF_CTRL_T)hInsContext;

    PDML_CALLLOG_SIGNALINGPERF pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "Protocol", TRUE) )
    {
        *puLong = pHEAD->Protocol;
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "CallSetupDelay", TRUE) )
    {
        *puLong = pHEAD->CallSetupDelay;
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

        BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = TRUE;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()


    TELCOVOICEMGR_UNLOCK()
    CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

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

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SIGNALINGPERF_CTRL_T pHEADCtrl = (PDML_CALLLOG_SIGNALINGPERF_CTRL_T)hInsContext;

    PDML_CALLLOG_SIGNALINGPERF pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "OutgoingMediaEstablishDelay", TRUE) )
    {
        *pInt = pHEAD->OutgoingMediaEstablishDelay;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "IncomingMediaEstablishDelay", TRUE) )
    {
        *pInt = pHEAD->IncomingMediaEstablishDelay;
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

        BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()


    TELCOVOICEMGR_UNLOCK()
    CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_SignalPerfList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_CallLogList_SessionList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_CALLLOG_CTRL_T* pCallLogCtrl = (DML_CALLLOG_CTRL_T*) hInsContext;

    PDML_CALLLOG pDmlCallLog = &(pCallLogCtrl->dml);

    PDML_CALLLOG_SESSION_LIST_T pCallLogSessionList = &(pDmlCallLog->Session);

    if(pCallLogSessionList != NULL)
    {
        ret = pCallLogSessionList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_CallLogList_SessionList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_CallLogList_SessionList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_CTRL_T pCallLogCtrl = (PDML_CALLLOG_CTRL_T) hInsContext;

    PDML_CALLLOG pDmlCallLog = &(pCallLogCtrl->dml);

    PDML_CALLLOG_SESSION_LIST_T pSessionList = &(pDmlCallLog->Session);

    if(pSessionList != NULL)
    {
        if(nIndex < pSessionList->ulQuantity)
        {
            PDML_CALLLOG_SESSION_CTRL_T pSessionCtrl = pSessionList->pdata[nIndex];
            if(pSessionCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pSessionCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pHEADCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "StreamType", TRUE) )
    {
        *puLong = pHEAD->StreamType;
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

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pHEADCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pHEAD = &(pHEADCtrl->dml);

    if( AnscEqualString(ParamName, "Start", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Start);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "SessionID", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->SessionID);
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_RTP pHEAD = &(pDmlCallLogSession->Source.RTP);

    if( AnscEqualString(ParamName, "SamplingFrequency", TRUE) )
    {
        *puLong = pHEAD->SamplingFrequency;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ReceivePacketLossRate", TRUE) )
    {
        *puLong = pHEAD->ReceivePacketLossRate;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketsSent", TRUE) )
    {
        *puLong = pHEAD->PacketsSent;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketsReceived", TRUE) )
    {
        *puLong = pHEAD->PacketsReceived;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketsLost", TRUE) )
    {
        *puLong = pHEAD->PacketsLost;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketsDiscarded", TRUE) )
    {
        *puLong = pHEAD->PacketsDiscarded;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "LocalUDPPort", TRUE) )
    {
        *puLong = pHEAD->LocalUDPPort;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "FarEndUDPPort", TRUE) )
    {
        *puLong = pHEAD->FarEndUDPPort;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "FarEndPacketLossRate", TRUE) )
    {
        *puLong = pHEAD->FarEndPacketLossRate;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "BytesSent", TRUE) )
    {
        *puLong = pHEAD->BytesSent;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "BytesReceived", TRUE) )
    {
        *puLong = pHEAD->BytesReceived;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "BurstCount", TRUE) )
    {
        *puLong = pHEAD->BurstCount;
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

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_RTP pHEAD = &(pDmlCallLogSession->Source.RTP);

    if( AnscEqualString(ParamName, "FarEndIPAddress", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->FarEndIPAddress);
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_RTP pHEAD = &(pDmlCallLogSession->Source.RTP);

    if( AnscEqualString(ParamName, "RoundTripDelay", TRUE) )
    {
        *pInt = pHEAD->RoundTripDelay;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ReceiveInterarrivalJitter", TRUE) )
    {
        *pInt = pHEAD->ReceiveInterarrivalJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketDelayVariation", TRUE) )
    {
        *pInt = pHEAD->PacketDelayVariation;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MinJitter", TRUE) )
    {
        *pInt = pHEAD->MinJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MeanJitter", TRUE) )
    {
        *pInt = pHEAD->MeanJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MaxJitter", TRUE) )
    {
        *pInt = pHEAD->MaxJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "FarEndInterarrivalJitter", TRUE) )
    {
        *pInt = pHEAD->FarEndInterarrivalJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "BufferDelay", TRUE) )
    {
        *pInt = pHEAD->BufferDelay;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AverageRoundTripDelay", TRUE) )
    {
        *pInt = pHEAD->AverageRoundTripDelay;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AverageReceiveInterarrivalJitter", TRUE) )
    {
        *pInt = pHEAD->AverageReceiveInterarrivalJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AverageFarEndInterarrivalJitter", TRUE) )
    {
        *pInt = pHEAD->AverageFarEndInterarrivalJitter;
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_RXCODEC pHEAD = &(pDmlCallLogSession->Source.DSP.ReceiveCodec);

    if( AnscEqualString(ParamName, "Underruns", TRUE) )
    {
        *puLong = pHEAD->Underruns;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Overruns", TRUE) )
    {
        *puLong = pHEAD->Overruns;
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

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_RXCODEC pHEAD = &(pDmlCallLogSession->Source.DSP.ReceiveCodec);

    if( AnscEqualString(ParamName, "Codec", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Codec);
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_RecvCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_RXCODEC pHEAD = &(pDmlCallLogSession->Source.DSP.ReceiveCodec);

    if( AnscEqualString(ParamName, "SilenceSuppression", TRUE) )
    {
        *pBool = pHEAD->SilenceSuppression;
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_TXCODEC pHEAD = &(pDmlCallLogSession->Source.DSP.TransmitCodec);

    if( AnscEqualString(ParamName, "Underruns", TRUE) )
    {
        *puLong = pHEAD->Underruns;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketizationPeriod", TRUE) )
    {
        *puLong = pHEAD->PacketizationPeriod;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Overruns", TRUE) )
    {
        *puLong = pHEAD->Overruns;
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

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_TXCODEC pHEAD = &(pDmlCallLogSession->Source.DSP.TransmitCodec);

    if( AnscEqualString(ParamName, "Codec", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Codec);
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_source_DSP_TXCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_TXCODEC pHEAD = &(pDmlCallLogSession->Source.DSP.TransmitCodec);

    if( AnscEqualString(ParamName, "SilenceSuppression", TRUE) )
    {
        *pBool = pHEAD->SilenceSuppression;
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

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_VoiceQuality_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_source_VoiceQuality_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_VOICEQUALITY pHEAD = &(pDmlCallLogSession->Source.VoiceQuality);

    if( AnscEqualString(ParamName, "WorstVoIPQualityIndicatorTimestamps", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->WorstVoIPQualityIndicatorTimestamps);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "WorstVoIPQualityIndicatorsValues", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->WorstVoIPQualityIndicatorsValues);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "VoIPQualityIndicator", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->VoIPQualityIndicator);
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_RTP pHEAD = &(pDmlCallLogSession->Destination.RTP);

    if( AnscEqualString(ParamName, "SamplingFrequency", TRUE) )
    {
        *puLong = pHEAD->SamplingFrequency;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ReceivePacketLossRate", TRUE) )
    {
        *puLong = pHEAD->ReceivePacketLossRate;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketsSent", TRUE) )
    {
        *puLong = pHEAD->PacketsSent;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketsReceived", TRUE) )
    {
        *puLong = pHEAD->PacketsReceived;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketsLost", TRUE) )
    {
        *puLong = pHEAD->PacketsLost;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketsDiscarded", TRUE) )
    {
        *puLong = pHEAD->PacketsDiscarded;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "LocalUDPPort", TRUE) )
    {
        *puLong = pHEAD->LocalUDPPort;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "FarEndUDPPort", TRUE) )
    {
        *puLong = pHEAD->FarEndUDPPort;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "FarEndPacketLossRate", TRUE) )
    {
        *puLong = pHEAD->FarEndPacketLossRate;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "BytesSent", TRUE) )
    {
        *puLong = pHEAD->BytesSent;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "BytesReceived", TRUE) )
    {
        *puLong = pHEAD->BytesReceived;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "BurstCount", TRUE) )
    {
        *puLong = pHEAD->BurstCount;
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

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_RTP pHEAD = &(pDmlCallLogSession->Destination.RTP);

    if( AnscEqualString(ParamName, "FarEndIPAddress", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->FarEndIPAddress);
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_RTP pHEAD = &(pDmlCallLogSession->Destination.RTP);

    if( AnscEqualString(ParamName, "RoundTripDelay", TRUE) )
    {
        *pInt = pHEAD->RoundTripDelay;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ReceiveInterarrivalJitter", TRUE) )
    {
        *pInt = pHEAD->ReceiveInterarrivalJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketDelayVariation", TRUE) )
    {
        *pInt = pHEAD->PacketDelayVariation;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MinJitter", TRUE) )
    {
        *pInt = pHEAD->MinJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MeanJitter", TRUE) )
    {
        *pInt = pHEAD->MeanJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MaxJitter", TRUE) )
    {
        *pInt = pHEAD->MaxJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "FarEndInterarrivalJitter", TRUE) )
    {
        *pInt = pHEAD->FarEndInterarrivalJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "BufferDelay", TRUE) )
    {
        *pInt = pHEAD->BufferDelay;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AverageRoundTripDelay", TRUE) )
    {
        *pInt = pHEAD->AverageRoundTripDelay;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AverageReceiveInterarrivalJitter", TRUE) )
    {
        *pInt = pHEAD->AverageReceiveInterarrivalJitter;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AverageFarEndInterarrivalJitter", TRUE) )
    {
        *pInt = pHEAD->AverageFarEndInterarrivalJitter;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_RXCODEC pHEAD = &(pDmlCallLogSession->Destination.DSP.ReceiveCodec);

    if( AnscEqualString(ParamName, "Underruns", TRUE) )
    {
        *puLong = pHEAD->Underruns;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Overruns", TRUE) )
    {
        *puLong = pHEAD->Overruns;
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

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_RXCODEC pHEAD = &(pDmlCallLogSession->Destination.DSP.ReceiveCodec);

    if( AnscEqualString(ParamName, "Codec", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Codec);
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_RxCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_RXCODEC pHEAD = &(pDmlCallLogSession->Destination.DSP.ReceiveCodec);

    if( AnscEqualString(ParamName, "SilenceSuppression", TRUE) )
    {
        *pBool = pHEAD->SilenceSuppression;
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_TXCODEC pHEAD = &(pDmlCallLogSession->Destination.DSP.TransmitCodec);

    if( AnscEqualString(ParamName, "Underruns", TRUE) )
    {
        *puLong = pHEAD->Underruns;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PacketizationPeriod", TRUE) )
    {
        *puLong = pHEAD->PacketizationPeriod;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Overruns", TRUE) )
    {
        *puLong = pHEAD->Overruns;
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

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_TXCODEC pHEAD = &(pDmlCallLogSession->Destination.DSP.TransmitCodec);

    if( AnscEqualString(ParamName, "Codec", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->Codec);
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

        BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_CallLogList_SessionList_Dest_DSP_TxCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_DSP_TXCODEC pHEAD = &(pDmlCallLogSession->Destination.DSP.TransmitCodec);

    if( AnscEqualString(ParamName, "SilenceSuppression", TRUE) )
    {
        *pBool = pHEAD->SilenceSuppression;
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

        ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_VoiceQuality_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_CallLogList_SessionList_Dest_VoiceQuality_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_CALLLOG_SESSION_CTRL_T pCallLogSessionCtrl = (PDML_CALLLOG_SESSION_CTRL_T)hInsContext;

    PDML_CALLLOG_SESSION pDmlCallLogSession = &(pCallLogSessionCtrl->dml);

    PDML_CALLLOG_SESSION_VOICEQUALITY pHEAD = &(pDmlCallLogSession->Destination.VoiceQuality);

    if( AnscEqualString(ParamName, "WorstVoIPQualityIndicatorTimestamps", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->WorstVoIPQualityIndicatorTimestamps);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "WorstVoIPQualityIndicatorsValues", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->WorstVoIPQualityIndicatorsValues);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "VoIPQualityIndicator", TRUE) )
    {
        AnscCopyString(pValue,pHEAD->VoIPQualityIndicator);
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}
