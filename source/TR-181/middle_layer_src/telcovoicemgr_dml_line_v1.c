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
#include "telcovoicemgr_dml_line_v1.h"
#include "telcovoicemgr_services_apis_v1.h"
#include "telcovoicemgr_dml_hal.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"

/***********************************************************************

 APIs for Object:

    Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}.

    *  Line_IsUpdated
    *  Line_Synchronize
    *  Line_GetParamStringValue
    *  Line_SetParamStringValue
    *  Line_GetParamUlongValue
    *  Line_GetEntryCount
    *  Line_GetEntry
    *  Line_Validate
    *  Line_Commit
    *  Line_Rollback


***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Line_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if Line updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    

BOOL Line_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;

    return bIsUpdated;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG EventSubscribe_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if EventSubscribe updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    

BOOL EventSubscribe_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;

    return bIsUpdated;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Event_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if Event updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    

BOOL Event_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;

    return bIsUpdated;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Description_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if Description updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    

BOOL Description_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;

    return bIsUpdated;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Pattern_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if Pattern updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    

BOOL Pattern_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;

    return bIsUpdated;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG List_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if List updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    

BOOL List_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;

    return bIsUpdated;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Session_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if Session updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    

BOOL Session_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;

    return bIsUpdated;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Line_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize Line data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG Line_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG EventSubscribe_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize EventSubscribe data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG EventSubscribe_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Description_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize Description data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG Description_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Event_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize Event data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG Event_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Pattern_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize Pattern data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG Pattern_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG List_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize List data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG List_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG Session_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize Session data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG Session_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Line_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

   return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/
LONG Line_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    char    buf[1024] = {0};
    LONG ret = -1;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine && pValue && pUlSize)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                if( AnscEqualString(ParamName, "DirectoryNumber", TRUE) )
                {
                    snprintf(buf, sizeof(buf), "%s", pLine->DirectoryNumber);
                    if ( AnscSizeOfString(buf) < *pUlSize)
                    {
                        AnscCopyString(pValue, buf);
                        ret = 0;
                    }
                    else
                    {
                        *pUlSize = AnscSizeOfString(pValue);
                        ret = 1;
                    }
                }
                else if( AnscEqualString(ParamName, "X_RDK_OutboundProxyAddresses", TRUE) )
                {
                    //Fetch status from voice stack
                    hal_param_t req_param;
                    memset(&req_param, 0, sizeof(req_param));
                    snprintf(req_param.name, sizeof(req_param.name), TELCOVOICE_QUERY_VOICEPROFILE_LINE"%s",
                              pVoiceService->InstanceNumber, pVoiceProfile->InstanceNumber, pLine->InstanceNumber,"X_RDK_OutboundProxyAddresses");
                    if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
                    {
                        snprintf(buf, sizeof(buf), "%s",  req_param.value);
                        if ( AnscSizeOfString(buf) < *pUlSize)
                        {
                            AnscCopyString(pValue, buf);
                            ret = 0;
                        }
                        else
                        {
                            *pUlSize = AnscSizeOfString(pValue);
                            ret = 1;
                        }
                    }
                    else
                    {
                        CcspTraceError(("%s:%d:: X_RDK_OutbandProxyAddresses:get failed \n", __FUNCTION__, __LINE__));
                        *pUlSize = AnscSizeOfString(pValue);
                        ret = 1;
                    }
                }
                else if( AnscEqualString(ParamName, "Alias", TRUE) )
                {
                    snprintf(buf, sizeof(buf), "%s", pLine->Alias);
                    if ( AnscSizeOfString(buf) < *pUlSize)
                    {
                        AnscCopyString(pValue, buf);
                        ret = 0;
                    }
                    else
                    {
                        *pUlSize = AnscSizeOfString(pValue);
                        ret = 1;
                    }
                }
                else if( AnscEqualString(ParamName, "PhyReferenceList", TRUE) )
                {
                    snprintf(buf, sizeof(buf), "%s", pLine->PhyReferenceList);
                    if ( AnscSizeOfString(buf) < *pUlSize)
                    {
                        AnscCopyString(pValue, buf);
                        ret = 0;
                    }
                    else
                    {
                        *pUlSize = AnscSizeOfString(pValue);
                        ret = 1;
                    }
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                    ret = 1;
                }
            }

            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Line_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer to set value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL Line_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    BOOL bStatus = TRUE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;
    ULONG uLineIndex = 0;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine && pString)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                uVsIndex = pVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                uLineIndex = pLine->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "DirectoryNumber", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetDirectoryNumber(uVsIndex, uVpIndex, uLineIndex, pString) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            AnscCopyString(pLine->DirectoryNumber, pString);
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                    else
                    {
                        bStatus =  FALSE;
                    }
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                    bStatus =  FALSE;
                }
            }
            else
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            }
        }
    }

    return bStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Line_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      pValue
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL Line_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    ULONG ret = 1;
    TELCOVOICEMGR_CALL_STATE_ENUM  callState = VOICE_CALL_STATE_IDLE;
    TELCOVOICEMGR_LINE_STATUS_ENUM lineStatus = VOICE_LINE_STATE_DISABLED;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine && pValue)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                if( AnscEqualString(ParamName, "CallState", TRUE) )
                {
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetLineCallState(pVoiceService->InstanceNumber, pVoiceProfile->InstanceNumber, 
                                                  pLine->InstanceNumber, &callState))
                    {
                        pLine->CallState = callState;
                        *pValue = pLine->CallState;
                        ret = TRUE;
                    }
                }
                else if( AnscEqualString(ParamName, "Enable", TRUE) )
                {
                    *pValue = pLine->Enable;
                    ret =TRUE;
                }
                else if( AnscEqualString(ParamName, "Status", TRUE) )
                {
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetLineStatus(pVoiceService->InstanceNumber, pVoiceProfile->InstanceNumber, 
                                                  pLine->InstanceNumber, &lineStatus))
                    {
                        pLine->Status = lineStatus;
                        *pValue = pLine->Status;
                        ret = TRUE;
                    }
                }
                else if( AnscEqualString(ParamName, "RingVolumeStatus", TRUE) )
                {
                    *pValue = pLine->RingVolumeStatus;
                    ret = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                    ret = FALSE;
                }
            }

            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Line_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL Line_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    BOOL bStatus = TRUE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;
    ULONG uLineIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }
            if (pVoiceService && pVoiceProfile && pLine)
            {
                uVsIndex = pVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                uLineIndex = pLine->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "Enable", TRUE) )
                {
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetLineEnable(uVsIndex, uVpIndex, uLineIndex, uValue))
                    {
                        CcspTraceWarning(("%s::ParamName:%s Val : %d set succeeded (%d, %d, %d)\n", __FUNCTION__, ParamName, uValue, uVsIndex, uVpIndex, uLineIndex));
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pLine->Enable = uValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                    bStatus =  FALSE;
                }
            }
            else
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            }
        }
    }

    return bStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

    BOOL Line_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:
        This function is called to retrieve BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
            The instance handle;

            char*                       ParamName,
            The parameter name;

            BOOL*                      pBool
            The buffer of returned pBool;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL Line_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;

    if(pTelcoVoiceMgrCtrl == NULL)
    {
        CcspTraceError(("%s:%d:: Invalid object \n", __FUNCTION__, __LINE__));
        return ret;
    }

    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrDmlData == NULL)
    {
        CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        return ret;
    }

    TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
    if(pLine == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pLine: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    else
    {
        if( AnscEqualString(ParamName, "RingMuteStatus", TRUE))
        {
            *pBool = pLine->RingMuteStatus;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
            ret = FALSE;
        }
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    }
    

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG Line_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG Line_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_PROFILE       pDmlVoiceProfile    = NULL;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T*) hInsContext;

    if(pVoiceProfile != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceProfile = &(pVoiceProfile->dml);   
            count = pDmlVoiceProfile->LineList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return count;
}
/**********************************************************************
    VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Codec.List.{i}.
    caller:     owner of this object

    prototype:

        ULONG List_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG List_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pTelcoVoiceMgrCtrl->dml);
            count = pDmlLine->LineCodecObj.LineCodecList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return count;
}

/**********************************************************************
    VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Session.{i}.
    caller:     owner of this object

    prototype:

        ULONG Session_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG Session_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pTelcoVoiceMgrCtrl->dml);
            count = pDmlLine->LineSessionList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return count;
}
/**********************************************************************
    VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP.EventSubscribe.{i}.
    caller:     owner of this object

    prototype:

        ULONG EventSubscribe_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG EventSubscribe_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pTelcoVoiceMgrCtrl->dml);
            count = pDmlLine->LineSipObj.LineSipEventSubscribeList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return count;
}
/**********************************************************************
    VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.Event.{i}.
    caller:     owner of this object

    prototype:

        ULONG Event_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG Event_GetEntryCount(ANSC_HANDLE hInsContext)
{

    ULONG count = 0;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pTelcoVoiceMgrCtrl->dml);
            count = pDmlLine->LineRingerObj.LineRingerEventList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return count;
}
/**********************************************************************
    VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.Description.{i}.
    caller:     owner of this object

    prototype:

        ULONG Description_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG Description_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pTelcoVoiceMgrCtrl->dml);
            count = pDmlLine->LineRingerObj.LineRingerDescriptionList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return count;
}
/**********************************************************************
    VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.Pattern.{i}.
    caller:     owner of this object

    prototype:

        ULONG Pattern_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG Pattern_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pTelcoVoiceMgrCtrl->dml);
            count = pDmlLine->LineRingerObj.LineRingerPatternList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return count;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE Line_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE Line_GetEntry(ANSC_HANDLE hInsContext, ULONG  nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_PROFILE          pDmlVoiceProfile    = NULL;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T*) hInsContext;

    if(pVoiceProfile != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceProfile = &(pVoiceProfile->dml);  
            if(nIndex < pDmlVoiceProfile->LineList.ulQuantity)
            {
                DML_LINE_CTRL_T* pDmlLine = pDmlVoiceProfile->LineList.pdata[nIndex];
                if ( pDmlLine != NULL )
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pDmlLine;
                }
            }
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return pDmlEntry;  /* return the handle */
}
/**********************************************************************
    VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Codec.List.{i}.
    caller:     owner of this object

    prototype:

        ANSC_HANDLE List_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE List_GetEntry(ANSC_HANDLE hInsContext, ULONG  nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pLine = (DML_PROFILE_CTRL_T*) hInsContext;

    if(pLine != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pLine->dml);
            if(nIndex < pDmlLine->LineCodecObj.LineCodecList.ulQuantity)
            {
                DML_LINE_CODECLIST_CTRL_T* pDmlCodecList = pDmlLine->LineCodecObj.LineCodecList.pdata[nIndex];
                if ( pDmlCodecList != NULL )
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pDmlCodecList;
                }
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return pDmlEntry;  /* return the handle */
}
/**********************************************************************
    VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Session.{i}.
    caller:     owner of this object

    prototype:

        ANSC_HANDLE Session_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE Session_GetEntry(ANSC_HANDLE hInsContext, ULONG  nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pLine = (DML_PROFILE_CTRL_T*) hInsContext;

    if(pLine != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pLine->dml);
            if(nIndex < pDmlLine->LineSessionList.ulQuantity)
            {
                DML_LINE_SESSION_CTRL_T* pDmlLineSession = pDmlLine->LineSessionList.pdata[nIndex];
                if ( pDmlLineSession != NULL )
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pDmlLineSession;
                }
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return pDmlEntry;  /* return the handle */
}

/**********************************************************************
    VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP.EventSubscribe.{i}.
    caller:     owner of this object

    prototype:

        ANSC_HANDLE EventSubscribe_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE EventSubscribe_GetEntry(ANSC_HANDLE hInsContext, ULONG  nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pLine = (DML_PROFILE_CTRL_T*) hInsContext;

    if(pLine != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pLine->dml);
            if(nIndex < pDmlLine->LineSipObj.LineSipEventSubscribeList.ulQuantity)
            {
                DML_LINE_SESSION_CTRL_T* pDmlLineEventSubscribe = pDmlLine->LineSipObj.LineSipEventSubscribeList.pdata[nIndex];
                if ( pDmlLineEventSubscribe != NULL )
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pDmlLineEventSubscribe;
                }
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return pDmlEntry;  /* return the handle */

}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE Event_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE Event_GetEntry(ANSC_HANDLE hInsContext, ULONG  nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pLine = (DML_PROFILE_CTRL_T*) hInsContext;

    if(pLine != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pLine->dml);
            if(nIndex < pDmlLine->LineRingerObj.LineRingerEventList.ulQuantity)
            {
                DML_LINE_SESSION_CTRL_T* pDmlLineRingerEvent = pDmlLine->LineRingerObj.LineRingerEventList.pdata[nIndex];
                if ( pDmlLineRingerEvent != NULL )
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pDmlLineRingerEvent;
                }
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return pDmlEntry;  /* return the handle */
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE Description_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE Description_GetEntry(ANSC_HANDLE hInsContext, ULONG  nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pLine = (DML_PROFILE_CTRL_T*) hInsContext;

    if(pLine != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pLine->dml);
            if(nIndex < pDmlLine->LineRingerObj.LineRingerDescriptionList.ulQuantity)
            {
                DML_LINE_SESSION_CTRL_T* pDmlLineRingerDescription = pDmlLine->LineRingerObj.LineRingerDescriptionList.pdata[nIndex];
                if ( pDmlLineRingerDescription != NULL )
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pDmlLineRingerDescription;
                }
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return pDmlEntry;  /* return the handle */
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE Pattern_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE Pattern_GetEntry(ANSC_HANDLE hInsContext, ULONG  nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_LINE          pDmlLine    = NULL;
    DML_LINE_CTRL_T* pLine = (DML_PROFILE_CTRL_T*) hInsContext;

    if(pLine != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlLine = &(pLine->dml);
            if(nIndex < pDmlLine->LineRingerObj.LineRingerPatternList.ulQuantity)
            {
                DML_LINE_SESSION_CTRL_T* pDmlLineRingerPattern = pDmlLine->LineRingerObj.LineRingerPatternList.pdata[nIndex];
                if ( pDmlLineRingerPattern != NULL )
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pDmlLineRingerPattern;
                }
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return pDmlEntry;  /* return the handle */
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Line_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/
BOOL Line_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Line_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

ULONG Line_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Line_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG Line_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/***********************************************************************

 APIs for Object:

    Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}.VoiceProcessing

    *  VoiceProcessing_GetParamIntValue
    *  VoiceProcessing_SetParamIntValue
    *  VoiceProcessing_Validate
    *  VoiceProcessing_Commit
    *  VoiceProcessing_Rollback

***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceProcessing_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL VoiceProcessing_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceProcessing_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      pValue
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL VoiceProcessing_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL    bStatus = FALSE;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine && pValue)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                if( AnscEqualString(ParamName, "EchoCancellationTail", TRUE) )
                {
                    *pValue = pLine->LineVoiceProcessingObj.EchoCancellationTail;
                    bStatus = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                    bStatus = FALSE;
                }
            }

            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    CcspTraceWarning(("%s::Exit, bStatus:%d\n", __FUNCTION__, bStatus));
    return bStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceProcessing_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

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
BOOL VoiceProcessing_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL    bStatus = FALSE;

    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine && pInt)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                if( AnscEqualString(ParamName, "ReceiveGain", TRUE) )
                {
                    * pInt = pLine->LineVoiceProcessingObj.ReceiveGain;
                    bStatus = TRUE;
                }
                else if( AnscEqualString(ParamName, "TransmitGain", TRUE) )
                {
                    * pInt = pLine->LineVoiceProcessingObj.TransmitGain;
                    bStatus = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                    bStatus =  FALSE;
                }
            }

            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    CcspTraceWarning(("%s::Exit, bStatus:%d\n", __FUNCTION__, bStatus));
    return bStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceProcessing_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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
BOOL VoiceProcessing_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL    bStatus = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;
    ULONG uLineIndex = 0;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_LINE pLine = NULL;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                uVsIndex = pVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                uLineIndex = pLine->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "ReceiveGain", TRUE) )
                {
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetReceiveGain(uVsIndex,
                                                                    uVpIndex,
                                                                    uLineIndex,
                                                                    iValue))
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pLine->LineVoiceProcessingObj.ReceiveGain = iValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "TransmitGain", TRUE) )
                {
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetTransmitGain(uVsIndex,
                                                                    uVpIndex,
                                                                    uLineIndex,
                                                                    iValue))
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pLine->LineVoiceProcessingObj.TransmitGain = iValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                    bStatus =  FALSE;
                }

            }
            else
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            }
        }
    }
    CcspTraceWarning(("%s::Exit, bStatus:%d\n", __FUNCTION__, bStatus));
    return bStatus;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL VoiceProcessing_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

description:
    This function is called to retrieve ULONG parameter value;

argument:   ANSC_HANDLE                 hInsContext,
            The instance handle;

            char*                       ParamName,
            The parameter name;

            BOOL*                      pBool
            The buffer of returned pBool;

return:     TRUE if succeeded.

**********************************************************************/
BOOL VoiceProcessing_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL    bStatus = FALSE;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine && pBool)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                if( AnscEqualString(ParamName, "EchoCancellationEnable", TRUE))
                {
                    *pBool = pLine->LineVoiceProcessingObj.EchoCancellationEnable;
                    bStatus = TRUE;
                }
                else if( AnscEqualString(ParamName, "EchoCancellationInUse", TRUE))
                {
                    *pBool = pLine->LineVoiceProcessingObj.EchoCancellationInUse;
                    bStatus = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                    bStatus =  FALSE;
                }
            }

            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    CcspTraceWarning(("%s::Exit, bStatus:%d\n", __FUNCTION__, bStatus));
    return bStatus;
}

BOOL VoiceProcessing_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL    bStatus = FALSE;

    CcspTraceWarning(("%s::Exit\n", __FUNCTION__));
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceProcessing_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
BOOL VoiceProcessing_Validate(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG VoiceProcessing_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

ULONG VoiceProcessing_Commit(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG VoiceProcessing_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG VoiceProcessing_Rollback(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL EventSubscribe_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
BOOL EventSubscribe_Validate(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG EventSubscribe_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

ULONG EventSubscribe_Commit(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG EventSubscribe_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG EventSubscribe_Rollback(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Event_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
BOOL Event_Validate(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG Event_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

ULONG Event_Commit(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG Event_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG Event_Rollback(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Description_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
BOOL Description_Validate(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG Description_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

ULONG Description_Commit(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG Description_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG Description_Rollback(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Pattern_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
BOOL Pattern_Validate(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG Pattern_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

ULONG Pattern_Commit(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG Pattern_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG Pattern_Rollback(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}


/***********************************************************************

 APIs for Object:

    Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Stats

    *  Stats_GetParamBoolValue
    *  Stats_GetParamUlongValue
    *  Stats_SetParamBoolValue

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL        Stats_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                      pBool
                The buffer of returned pBool;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL Stats_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    PTELCOVOICEMGR_DML_LINE pLine = (PTELCOVOICEMGR_DML_LINE) hInsContext;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));

    if (pLine && pBool)
    {
        if( AnscEqualString(ParamName, "ResetStatistics", TRUE) )
        {
            /*Always return FALSE when read the ResetStatistics param*/
            *pBool = FALSE;
            return TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
            return FALSE;
        }
    }
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Stats_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bvalue);

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                      bvalue
                The buffer of returned pBool;

    return:     TRUE if succeeded.

**********************************************************************/


BOOL Stats_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL    bStatus = FALSE;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_LINE pLine = NULL;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;
    ULONG uLineIndex = 0;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                uVsIndex = pVoiceProfile->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                uLineIndex = pLine->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "ResetStatistics", TRUE) )
                {
                    if(TRUE == bValue)
                    {
                        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlResetLineStats(uVsIndex,
                                                                        uVpIndex,
                                                                        uLineIndex))
                        {
                            return TRUE;
                        }
                    }
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                }
            }
            else
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            }
        }
    }
    return bStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Stats_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned puLong;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL Stats_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL  bStatus = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;
    ULONG uLineIndex = 0;
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE            pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE       pVoiceService = NULL;
    TELCOVOICEMGR_DML_VOICESERVICE_STATS  stVoiceStats;
    memset(&stVoiceStats, 0, sizeof(stVoiceStats));
    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine && puLong)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                uVsIndex = pVoiceProfile->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                uLineIndex = pLine->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetLineStats(uVsIndex,
                                                                uVpIndex,
                                                                uLineIndex,
                                                                &stVoiceStats))
                {
                    bStatus = TRUE;

                    if( AnscEqualString(ParamName, "AverageFarEndInterarrivalJitter", TRUE) )
                    {
                        *puLong = stVoiceStats.AverageFarEndInterarrivalJitter;
                    }
                    else if( AnscEqualString(ParamName, "AverageReceiveInterarrivalJitter", TRUE) )
                    {
                        *puLong = stVoiceStats.AverageReceiveInterarrivalJitter;
                    }
                    else if( AnscEqualString(ParamName, "AverageRoundTripDelay", TRUE) )
                    {
                        *puLong = stVoiceStats.AverageRoundTripDelay;
                    }
                    else if( AnscEqualString(ParamName, "BytesReceived", TRUE) )
                    {
                        *puLong = stVoiceStats.BytesReceived;
                    }
                    else if( AnscEqualString(ParamName, "BytesSent", TRUE) )
                    {
                        *puLong = stVoiceStats.BytesSent;
                    }
                    else if( AnscEqualString(ParamName, "FarEndInterarrivalJitter", TRUE) )
                    {
                        *puLong = stVoiceStats.FarEndInterarrivalJitter;
                    }
                    else if( AnscEqualString(ParamName, "FarEndPacketLossRate", TRUE) )
                    {
                        *puLong = stVoiceStats.FarEndPacketLossRate;
                    }
                    else if( AnscEqualString(ParamName, "IncomingCallsAnswered", TRUE) )
                    {
                        *puLong = stVoiceStats.IncomingCallsAnswered;
                    }
                    else if( AnscEqualString(ParamName, "IncomingCallsConnected", TRUE) )
                    {
                        *puLong = stVoiceStats.IncomingCallsConnected;
                    }
                    else if( AnscEqualString(ParamName, "IncomingCallsFailed", TRUE) )
                    {
                        *puLong = stVoiceStats.IncomingCallsFailed;
                    }
                    else if( AnscEqualString(ParamName, "IncomingCallsReceived", TRUE) )
                    {
                        *puLong = stVoiceStats.IncomingCallsReceived;
                    }
                    else if( AnscEqualString(ParamName, "OutgoingCallsAnswered", TRUE) )
                    {
                        *puLong = stVoiceStats.OutgoingCallsAnswered;
                    }
                    else if( AnscEqualString(ParamName, "OutgoingCallsAttempted", TRUE) )
                    {
                        *puLong = stVoiceStats.OutgoingCallsAttempted;
                    }
                    else if( AnscEqualString(ParamName, "OutgoingCallsConnected", TRUE) )
                    {
                        *puLong = stVoiceStats.OutgoingCallsConnected;
                    }
                    else if( AnscEqualString(ParamName, "OutgoingCallsFailed", TRUE) )
                    {
                        *puLong = stVoiceStats.OutgoingCallsFailed;
                    }
                    else if( AnscEqualString(ParamName, "PacketsLost", TRUE) )
                    {
                        *puLong = stVoiceStats.PacketsLost;
                    }
                    else if( AnscEqualString(ParamName, "PacketsReceived", TRUE) )
                    {
                        *puLong = stVoiceStats.PacketsReceived;
                    }
                    else if( AnscEqualString(ParamName, "PacketsSent", TRUE) )
                    {
                        *puLong = stVoiceStats.PacketsSent;
                    }
                    else if( AnscEqualString(ParamName, "ReceiveInterarrivalJitter", TRUE) )
                    {
                        *puLong = stVoiceStats.ReceiveInterarrivalJitter;
                    }
                    else if( AnscEqualString(ParamName, "ReceivePacketLossRate", TRUE) )
                    {
                        *puLong = stVoiceStats.ReceivePacketLossRate;
                    }
                    else if( AnscEqualString(ParamName, "RoundTripDelay", TRUE) )
                    {
                        *puLong = stVoiceStats.RoundTripDelay;
                    }
                    else if( AnscEqualString(ParamName, "Overruns", TRUE) )
                    {
                        *puLong = stVoiceStats.Overruns;
                    }
                    else if( AnscEqualString(ParamName, "Underruns", TRUE) )
                    {
                        *puLong = stVoiceStats.Underruns;
                    }
                    else if( AnscEqualString(ParamName, "CallsDropped", TRUE) )
                    {
                        *puLong = stVoiceStats.CallsDropped;
                    }
                    else if( AnscEqualString(ParamName, "TotalCallTime", TRUE) )
                    {
                        *puLong = stVoiceStats.TotalCallTime;
                    }
                    else if( AnscEqualString(ParamName, "ServerDownTime", TRUE) )
                    {
                        *puLong = stVoiceStats.ServerDownTime;
                    }
                    else
                    {
                        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                        bStatus = FALSE;
                    }
                }
            }
            else
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            }
        }
    }

    CcspTraceWarning(("%s::Exit, bStatus:%d\n", __FUNCTION__, bStatus));
    return bStatus;
}

/***********************************************************************

 APIs for Object:

    Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}.CallingFeatures

    *  CallingFeatures_GetParamBoolValue
    *  CallingFeatures_GetParamUlongValue
    *  CallingFeatures_Validate
    *  CallingFeatures_Commit
    *  CallingFeatures_Rollback

***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL CallingFeatures_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL CallingFeatures_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL CallingFeatures_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      pValue
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL CallingFeatures_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{

    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;

    TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);

    if( !pLine || !pValue || !ParamName )
    {
        return FALSE;
    }

    if( AnscEqualString(ParamName, "MaxSessions", TRUE) )
    {
        *pValue = pLine->LineCallingFeaturesObj.MaxSessions;
    }
    else if( AnscEqualString(ParamName, "ConferenceCallingSessionCount", TRUE) )
    {
        *pValue = pLine->LineCallingFeaturesObj.ConferenceCallingSessionCount;
    }
    else if( AnscEqualString(ParamName, "CallForwardOnNoAnswerRingCount", TRUE) )
    {
        *pValue = pLine->LineCallingFeaturesObj.CallForwardOnNoAnswerRingCount;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
        return FALSE;
    }
    return TRUE;
}

LONG CallingFeatures_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;

    TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);

    if( !pLine || !pValue || !ParamName )
    {
        return -1;
    }

    if( AnscEqualString(ParamName, "CallerIDName", TRUE) )
    {
        AnscCopyString(pValue, pLine->LineCallingFeaturesObj.CallerIDName);
    }
    else if( AnscEqualString(ParamName, "CallWaitingStatus", TRUE) )
    {
        switch( pLine->LineCallingFeaturesObj.CallWaitingStatus )
        {
            case VOICE_CW_STATE_DISABLED:
                AnscCopyString(pValue, "Disabled");
                break;

            case VOICE_CW_STATE_IDLE:
                AnscCopyString(pValue, "Idle");
                break;

            case VOICE_CW_STATE_SECONDARY_RINGING:
                AnscCopyString(pValue, "Ringing");
                break;

            case VOICE_CW_STATE_SECONDARY_CONNECTING:
                AnscCopyString(pValue, "Connecting");
                break;

            case VOICE_CW_STATE_SECONDARY_CONNECTED:
                AnscCopyString(pValue, "Connected");
                break;

            default:
                AnscCopyString(pValue, "");
                break;
        }
    }
    else if( AnscEqualString(ParamName, "ConferenceCallingStatus", TRUE) )
    {
        switch( pLine->LineCallingFeaturesObj.ConferenceCallingStatus )
        {
            case VOICE_CONFERENCE_CALLING_STATE_DISABLED:
                AnscCopyString(pValue, "Disabled");
                break;

            case VOICE_CONFERENCE_CALLING_STATE_IDLE:
                AnscCopyString(pValue, "Idle");
                break;

            case VOICE_CONFERENCE_CALLING_STATE_SECONDARY_RINGING:
                AnscCopyString(pValue, "Ringing");
                break;

            case VOICE_CONFERENCE_CALLING_STATE_SECONDARY_CONNECTING:
                AnscCopyString(pValue, "Connecting");
                break;

            case VOICE_CONFERENCE_CALLING_STATE_SECONDARY_CONNECTED:
                AnscCopyString(pValue, "Connected");
                break;

            case VOICE_CONFERENCE_CALLING_STATE_IN_CONFERENCE_CALL:
                AnscCopyString(pValue, "In Call");
                break;

            default:
                AnscCopyString(pValue, "");
                break;
        }
    }
    else if( AnscEqualString(ParamName, "CallForwardOnBusyNumber", TRUE) )
    {
        AnscCopyString(pValue, pLine->LineCallingFeaturesObj.CallForwardOnBusyNumber);
    }
    else if( AnscEqualString(ParamName, "CallForwardUnconditionalNumber", TRUE) )
    {
        AnscCopyString(pValue, pLine->LineCallingFeaturesObj.CallForwardUnconditionalNumber);
    }
    else if( AnscEqualString(ParamName, "CallForwardOnNoAnswerNumber", TRUE) )
    {
        AnscCopyString(pValue, pLine->LineCallingFeaturesObj.CallForwardOnNoAnswerNumber);
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
        return -1;
    }
    return 0;
}
BOOL CallingFeatures_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    return FALSE;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL CallingFeatures_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                      pBool
                The buffer of returned pBool;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL CallingFeatures_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{

    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;

    TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);

    BOOL ret = FALSE;

    if( !pLine || !pBool || !ParamName )
    {
        return ret;
    }
    
    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));
    if( AnscEqualString(ParamName, "CallerIDEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.CallerIDEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AnonymousCallBlockEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.AnonymousCallBlockEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AnonymousCalEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.AnonymousCalEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "DoNotDisturbEnable", TRUE) )
    {
        *pBool =  pLine->LineCallingFeaturesObj.DoNotDisturbEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallReturnEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.CallReturnEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "RepeatDialEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.RepeatDialEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallForwardOnNoAnswerEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.CallForwardOnNoAnswerEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallTransferEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.CallTransferEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MessageWaiting", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.MessageWaiting;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallForwardUnconditionalEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.CallForwardUnconditionalEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallerIDNameEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.CallerIDNameEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallForwardOnBusyEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.CallForwardOnBusyEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "CallWaitingEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.CallWaitingEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MWIEnable", TRUE) )
    {
        *pBool = pLine->LineCallingFeaturesObj.MWIEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "X_RDK-Central_COM_ConferenceCallingEnable", TRUE) )
    {
        * pBool = pLine->LineCallingFeaturesObj.ConferenceCallingEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "X_RDK-Central_COM_HoldEnable", TRUE) )
    {
        * pBool = pLine->LineCallingFeaturesObj.HoldEnable;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "X_RDK-Central_COM_PhoneCallerIDEnable", TRUE) )
    {
        * pBool = pLine->LineCallingFeaturesObj.PhoneCallerIDEnable;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown_a ParamName:%s\n", __FUNCTION__, ParamName));
        ret = FALSE;
    } 
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL CallingFeatures_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                      bValue
                The buffer of returned pBool;

    return:     TRUE if succeeded.

**********************************************************************/


BOOL CallingFeatures_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM eFeature;
    BOOL bStatus = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;
    ULONG uLineIndex = 0;
    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pLine)
            {
                uVsIndex = pVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                uLineIndex = pLine->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "CallWaitingEnable", TRUE) )
                {
                    eFeature = VOICE_CALLING_FEATURE_CALL_WAITING;
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex,
                                                                    uVpIndex,
                                                                    uLineIndex,
                                                                    eFeature,
                                                                    bValue))
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                           pLine->LineCallingFeaturesObj.CallWaitingEnable = bValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "MWIEnable", TRUE) )
                {
                    eFeature = VOICE_CALLING_FEATURE_MSG_WAIT_INDICATOR;
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex,
                                                                    uVpIndex,
                                                                    uLineIndex,
                                                                    eFeature,
                                                                    bValue))
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pLine->LineCallingFeaturesObj.MWIEnable = bValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "X_RDK-Central_COM_ConferenceCallingEnable", TRUE) )
                {
                    eFeature = VOICE_CALLING_FEATURE_CONF_CALL;
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex,
                                                                    uVpIndex,
                                                                    uLineIndex,
                                                                    eFeature,
                                                                    bValue))
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pLine->LineCallingFeaturesObj.ConferenceCallingEnable = bValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "X_RDK-Central_COM_HoldEnable", TRUE) )
                {
                    eFeature = VOICE_CALLING_FEATURE_HOLD;
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex,
                                                                    uVpIndex,
                                                                    uLineIndex,
                                                                    eFeature,
                                                                    bValue))
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pLine->LineCallingFeaturesObj.HoldEnable = bValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "X_RDK-Central_COM_PhoneCallerIDEnable", TRUE) )
                {
                    eFeature = VOICE_CALLING_FEATURE_CALLER_ID;
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex,
                                                                    uVpIndex,
                                                                    uLineIndex,
                                                                    eFeature,
                                                                    bValue))
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pLine->LineCallingFeaturesObj.PhoneCallerIDEnable = bValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                    bStatus = FALSE;
                }
            }
            else
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            }
        }
    }
    return bStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL CallingFeatures_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
BOOL CallingFeatures_Validate(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG CallingFeatures_Commit(ANSC_HANDLEhInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

ULONG CallingFeatures_Commit(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG CallingFeatures_Rollback(ANSC_HANDLEhInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG CallingFeatures_Rollback(ANSC_HANDLE hInsContext)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return 0;
}


/***********************************************************************

 APIs for Object:

    Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP

    *  LineSIP_GetParamStringValue
    *  LineSIP_SetParamStringValue

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG LineSIP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

   return:     0 if succeeded;
               1 if short of buffer size; (*pUlSize = required size)
               -1 if not supported.

**********************************************************************/
LONG LineSIP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    PTELCOVOICEMGR_DML_LINE pLine = NULL;
    char    buf[1024] = {0};
    LONG ret = -1;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine && pValue && pUlSize)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
                if (pVoiceService && pVoiceProfile && pLine)
                {
                    if( AnscEqualString(ParamName, "AuthUserName", TRUE) )
                    {
                        snprintf(buf, sizeof(buf), "%s", pLine->LineSipObj.AuthUserName);
                        if ( AnscSizeOfString(buf) < *pUlSize)
                        {
                            AnscCopyString(pValue, buf);
                            ret = 0;
                        }
                        else
                        {
                            *pUlSize = AnscSizeOfString(pValue);
                            ret = 1;
                        }
                    }
                    else if( AnscEqualString(ParamName, "AuthPassword", TRUE) )
                    {
                        //Avoid returning password in dmcli get.
                        ret = 0;
                    }
                    else if( AnscEqualString(ParamName, "URI", TRUE) )
                    {
                        snprintf(buf, sizeof(buf), "%s", pLine->LineSipObj.URI);
                        if ( AnscSizeOfString(buf) < *pUlSize)
                        {
                            AnscCopyString(pValue, buf);
                            ret = 0;
                        }
                        else
                        {
                            *pUlSize = AnscSizeOfString(pValue);
                            ret = 1;
                        }
                    }
                    else
                    {
                        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                        ret = -1;
                    }
                }
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return ret;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL LineSIP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer to set value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL LineSIP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    DML_LINE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_LINE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    PTELCOVOICEMGR_DML_LINE pLine = NULL;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;
    ULONG uLineIndex = 0;

    CcspTraceWarning(("%s::ParamName:%s, Value:%s\n", __FUNCTION__, ParamName, pString));
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_LINE* pLine = &(pTelcoVoiceMgrCtrl->dml);
            if (pLine && pString)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pLine->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
                if (pVoiceService && pVoiceProfile && pLine)
                {
                    uVsIndex = pVoiceService->InstanceNumber;
                    uVpIndex = pVoiceProfile->InstanceNumber;
                    uLineIndex = pLine->InstanceNumber;
                    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                    if( AnscEqualString(ParamName, "AuthUserName", TRUE) )
                    {
                        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetLineSIPAuthCredentials(uVsIndex,
                                                                    uVpIndex,
                                                                    pLine->InstanceNumber,
                                                                    VOICE_HAL_AUTH_UNAME,
                                                                    pString))
                        {
                            TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                            if(pTelcoVoiceMgrDmlData != NULL)
                            {
                                AnscCopyString(pLine->LineSipObj.AuthUserName, pString);
                                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                                return TRUE;
                            }
                        }
                    }
                    else if( AnscEqualString(ParamName, "AuthPassword", TRUE) )
                    {
                        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetLineSIPAuthCredentials(uVsIndex,
                                                                    uVpIndex,
                                                                    uLineIndex,
                                                                    VOICE_HAL_AUTH_PWD,
                                                                    pString))
                        {
                            TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                            if(pTelcoVoiceMgrDmlData != NULL)
                            {
                                AnscCopyString(pLine->LineSipObj.AuthPassword, pString);
                                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                                return TRUE;
                            }
                        }
                    }
                    else if( AnscEqualString(ParamName, "URI", TRUE) )
                    {
                        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetLineSipURI(uVsIndex,
                                                                    uVpIndex,
                                                                    uLineIndex,
                                                                    pString))
                        {
                            TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                            if(pTelcoVoiceMgrDmlData != NULL)
                            {
                                AnscCopyString(pLine->LineSipObj.URI, pString);
                                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                                return TRUE;
                            }
                        }
                    }
                    else
                    {
                        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
                        ret = FALSE;
                    }
                }
            }
            else
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            }
        }
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL Event_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,

                    The instance handle;

                    char*                       ParamName,
                    The parameter name;

                    ULONG*                      pValue
                    The value returned

        return:     TRUE if succeeded.

**********************************************************************/

BOOL Event_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;

    if( AnscEqualString(ParamName, "Event_GetParamUlongValue", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    if( AnscEqualString(ParamName, "RingID", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Event_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:
        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     TRUE if succeeded.
                FALSE if not

**********************************************************************/

BOOL Event_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        return TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
        return FALSE;
    }
    
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG Event_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/

LONG Event_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else if( AnscEqualString(ParamName, "Function", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Event_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer to set value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL Event_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL Description_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,

                    The instance handle;

                    char*                       ParamName,
                    The parameter name;

                    ULONG*                      pValue
                    The value returned

        return:     TRUE if succeeded.

**********************************************************************/
BOOL Description_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return TRUE;
}
BOOL Description_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return TRUE;
}
BOOL Description_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;

    if( AnscEqualString(ParamName, "EntryID", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"RingPattern", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"RingEnable", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}
BOOL Description_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "RingEnable", TRUE))
    {
        *pBool = TRUE;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}
BOOL Description_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG Description_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/

LONG Description_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else if( AnscEqualString(ParamName, "RingName", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else if( AnscEqualString(ParamName, "RingFile", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL List_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,

                    The instance handle;

                    char*                       ParamName,
                    The parameter name;

                    ULONG*                      pValue
                    The value returned

        return:     TRUE if succeeded.

**********************************************************************/

BOOL List_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;

    if( AnscEqualString(ParamName, "EntryID", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"BitRate", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"Priority", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL List_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

description:
    This function is called to retrieve ULONG parameter value;

argument:   ANSC_HANDLE                 hInsContext,
            The instance handle;

            char*                       ParamName,
            The parameter name;

            BOOL*                      pBool
            The buffer of returned pBool;

return:     TRUE if succeeded.

**********************************************************************/
BOOL List_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
        BOOL ret = FALSE;
        if( AnscEqualString(ParamName, "SilenceSuppression", TRUE))
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "Enable", TRUE))
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
        }
        return ret;
}
BOOL List_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL    bStatus = FALSE;

    CcspTraceWarning(("%s::Exit\n", __FUNCTION__));
    return TRUE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL List_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL List_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "Priority", TRUE) )
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG List_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/

LONG List_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else if( AnscEqualString(ParamName, "Codec", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else if( AnscEqualString(ParamName, "PacketizationPeriod", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL List_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer to set value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL List_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "PacketizationPeriod", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL Pattern_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,

                    The instance handle;

                    char*                       ParamName,
                    The parameter name;

                    ULONG*                      pValue
                    The value returned

        return:     TRUE if succeeded.

**********************************************************************/
BOOL Pattern_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    CcspTraceWarning(("%s::Enter\n", __FUNCTION__));
    return TRUE;
}
BOOL Pattern_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;

    if( AnscEqualString(ParamName, "Duration", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"NextEntryID", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"EntryID", TRUE))
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Pattern_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer to set value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL Pattern_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "EntryID", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Duration", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "NextEntryID", TRUE) )
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG Pattern_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/

LONG Pattern_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
        return -1; 
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

    BOOL Pattern_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:
        This function is called to retrieve BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
            The instance handle;

            char*                       ParamName,
            The parameter name;

            BOOL*                      pBool
            The buffer of returned pBool;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL Pattern_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
        BOOL ret = FALSE;
        if( AnscEqualString(ParamName, "RingerOn", TRUE))
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
        }
        return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Pattern_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer to set value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL Pattern_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "RingerOn", TRUE) )
    {
        ret = FALSE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL LineCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

description:
    This function is called to retrieve ULONG parameter value;

argument:   ANSC_HANDLE                 hInsContext,
            The instance handle;

            char*                       ParamName,
            The parameter name;

            BOOL*                      pBool
            The buffer of returned pBool;

return:     TRUE if succeeded.

**********************************************************************/
BOOL LineCodec_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
        BOOL ret = FALSE;
        if( AnscEqualString(ParamName, "TransmitSilenceSuppression", TRUE))
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "ReceiveSilenceSuppression", TRUE))
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
        }
        return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL LineCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,

                    The instance handle;

                    char*                       ParamName,
                    The parameter name;

                    ULONG*                      pValue
                    The value returned

        return:     TRUE if succeeded.

**********************************************************************/

BOOL LineCodec_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;

    if( AnscEqualString(ParamName, "TransmitBitRate", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"ReceiveBitRate", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"TransmitPacketizationPeriod", TRUE))
    {
        *pValue = 0;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG LineCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/

LONG LineCodec_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;
    if( AnscEqualString(ParamName, "TransmitCodec", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else if( AnscEqualString(ParamName, "ReceiveCodec", TRUE) )
    {
        AnscCopyString(pValue, "");
        return 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL EventSubscribe_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer to set value;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL EventSubscribe_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AuthUserName", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "AuthPassword", TRUE) )
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG EventSubscribe_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/

LONG EventSubscribe_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue, "");
        ret  = 0;
    }
    else if( AnscEqualString(ParamName, "Event", TRUE) )
    {
        AnscCopyString(pValue, "");
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "AuthUserName", TRUE) )
    {
        AnscCopyString(pValue, "");
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "AuthPassword", TRUE) )
    {
        AnscCopyString(pValue, "");
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Session_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:
        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     

**********************************************************************/

BOOL Session_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "SessionDuration", TRUE) )
    {
        *puLong = 0;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "FarEndUDPPort", TRUE) )
    {
        *puLong = 0;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "LocalUDPPort", TRUE) )
    {
        *puLong = 0;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG Session_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/

LONG Session_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;

    if( AnscEqualString(ParamName, "SessionStartTime", TRUE) )
    {
        AnscCopyString(pValue, "");
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "FarEndIPAddress", TRUE) )
    {
        AnscCopyString(pValue, "");
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

LONG LineMGCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;
    PTELCOVOICEMGR_DML_LINE_SESSION pSession = NULL;
    if( AnscEqualString(ParamName, "LineName", TRUE) )
    {
        AnscCopyString(pValue, "");
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}
BOOL LineMGCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
        
    return ret;
}
LONG LineH323_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;
    if( AnscEqualString(ParamName, "LineName", TRUE) )
    {
        AnscCopyString(pValue, "");
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "H323ID", TRUE) )
    {
        AnscCopyString(pValue, "");
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}
BOOL LineH323_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;

    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
        
    return ret;
}

BOOL LineRinger_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, ParamName));
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL List_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/
BOOL List_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL List_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

ULONG List_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL List_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG List_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Session_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/
BOOL Session_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Session_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

ULONG Session_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Session_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG Session_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}
