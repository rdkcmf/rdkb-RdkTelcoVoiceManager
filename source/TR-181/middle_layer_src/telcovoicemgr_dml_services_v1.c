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
#include "telcovoicemgr_dml_services_v1.h"
#include "telcovoicemgr_services_apis_v1.h"
#include "telcovoicemgr_dml_hal.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"

/***********************************************************************

 APIs for Object:

    Services.

    *  Services_GetParamUlongValue

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Services_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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
BOOL Services_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    ULONG count = 0;
    PTELCOVOICEMGR_DATAMODEL_SERVICES  pMyObject  = (PTELCOVOICEMGR_DATAMODEL_SERVICES)g_pTelcoVoiceBEManager->hServices;
   
    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData  =  pMyObject->data;

    if(pTelcoVoiceMgrDmlData != NULL)
    {
      if (puLong)
      {
        if( AnscEqualString(ParamName, "VoiceServiceNumberOfEntries", TRUE) )
        {
            count = pTelcoVoiceMgrDmlData->Service.VoiceService.ulQuantity;
            *puLong = count;
            ret = TRUE;;
        }
      }
    }

    return ret;
}
/***********************************************************************

 APIs for Object:

    Services.VoiceService.{i}.
    
    *  VoiceService_IsUpdated
    *  VoiceService_Synchronize
    *  VoiceService_GetEntryCount
    *  VoiceService_GetEntry
    *  VoiceService_GetParamUlongValue
    *  VoiceService_SetParamUlongValue
    *  VoiceService_GetParamBoolValue
    *  VoiceService_SetParamBoolValue
    *  VoiceService_GetParamStringValue
    *  VoiceService_SetParamStringValue
    *  VoiceService_Validate
    *  VoiceService_Commit
    *  VoiceService_Rollback

***********************************************************************/

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG VoiceService_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if VoiceService updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    

BOOL VoiceService_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;
    return bIsUpdated;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG VoiceService_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize VoiceService data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG VoiceService_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
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
    ULONG count = 0;
    PTELCOVOICEMGR_DATAMODEL_SERVICES     pMyObject  = (PTELCOVOICEMGR_DATAMODEL_SERVICES)g_pTelcoVoiceBEManager->hServices;
   
    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData  =  pMyObject->data;

    if(pTelcoVoiceMgrDmlData != NULL)
    {
       count = pTelcoVoiceMgrDmlData->Service.VoiceService.ulQuantity;
    }
    return count;
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
    ANSC_HANDLE pDmlEntry = NULL;
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
               pDmlEntry = (ANSC_HANDLE) pDmlVoiceService;
            }
        }
    }

     return pDmlEntry;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceService_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL VoiceService_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{

    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
       TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
       if(pTelcoVoiceMgrDmlData != NULL)
       {
            TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);
            if (!pVoiceService || !pBool)
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                CcspTraceError(("%s:%d:: pVoiceService or pBool NULL\n", __FUNCTION__, __LINE__));
                return ret;
            }
            if( AnscEqualString(ParamName, "X_RDK_DisableLoopCurrentUntilRegistered", TRUE))
            {
                * pBool = pVoiceService->X_RDK_DisableLoopCurrentUntilRegistered;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "X_RDK_FactoryReset", TRUE))
            {
                /*Always return FALSE when read the X_RDK_FactoryReset param*/
                *pBool = FALSE;
                ret = TRUE;
            }

           TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
       }
    }

    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceService_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL VoiceService_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);
            if (!pVoiceService)
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                CcspTraceError(("%s:%d:: pVoiceService NULL\n", __FUNCTION__, __LINE__));
                return ret;
            }
            uVsIndex = pVoiceService->InstanceNumber;
            ULONG VoiceState = pVoiceService->X_RDK_Enable;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            if( AnscEqualString(ParamName, "X_RDK_DisableLoopCurrentUntilRegistered", TRUE))
            {
                if (TelcoVoiceMgrDmlSetLoopCurrentDisabled(uVsIndex, bValue) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        pVoiceService->X_RDK_DisableLoopCurrentUntilRegistered = bValue;
                        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                        return TRUE;
                    }
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
        }
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG VoiceService_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/
LONG VoiceService_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{

    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    ULONG ret = 1;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
          TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);

          if (pVoiceService && pValue && pUlSize)
          {
            if( AnscEqualString(ParamName, "X_RDK_BoundIfName", TRUE) )
            { 
                AnscCopyString(pValue, pVoiceService->X_RDK_BoundIfName);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "X_RDK_IpAddressFamily", TRUE) )
            {
                AnscCopyString(pValue, pVoiceService->X_RDK_IpAddressFamily);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "X_RDK_BoundIpAddr", TRUE) )
            {
                AnscCopyString(pValue, pVoiceService->X_RDK_BoundIpAddr);
                ret = 0;
            }
          }

          TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
        
    }

    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceService_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL VoiceService_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceService || !pString)
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                CcspTraceError(("%s:%d:: pVoiceService or pString NULL\n", __FUNCTION__, __LINE__));
                return ret;
            }
            uVsIndex = pVoiceService->InstanceNumber;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            if( AnscEqualString(ParamName, "X_RDK_BoundIfName", TRUE) )
            {
                if(TelcoVoiceMgrDmlSetBoundIfname(uVsIndex, pString) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        AnscCopyString(pVoiceService->X_RDK_BoundIfName, pString);
                        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                        return TRUE;
                    }
                }
            }
            else if( AnscEqualString(ParamName, "X_RDK_IpAddressFamily", TRUE) )
            {
                if(TelcoVoiceMgrDmlSetIpAddressFamily(pVoiceService->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        AnscCopyString(pVoiceService->X_RDK_IpAddressFamily, pString);
                        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                        return TRUE;
                    }
                }
            }
        }
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceService_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL VoiceService_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;
    TELCOVOICEMGR_VOICE_STATUS_ENUM voiceStatus;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
       TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
       if(pTelcoVoiceMgrDmlData != NULL)
       {
            TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);
            if (!pVoiceService || !pValue)
            {
                CcspTraceError(("%s:%d:: pVoiceService or pValue NULL\n", __FUNCTION__, __LINE__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData); 
                return ret;
            }
            if( AnscEqualString(ParamName, "X_RDK_Enable", TRUE))
            {
                * pValue = pVoiceService->X_RDK_Enable;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "X_RDK_Status", TRUE))
            {
                if(TelcoVoiceMgrDmlGetVoiceProcessStatus(pVoiceService->InstanceNumber, &voiceStatus) == ANSC_STATUS_SUCCESS)
                {
                    pVoiceService->X_RDK_Status = voiceStatus;
                    * pValue = pVoiceService->X_RDK_Status;
                    ret = TRUE;
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

        BOOL VoiceService_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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
BOOL VoiceService_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceService)
            {
                CcspTraceError(("%s:%d:: pVoiceService NULL\n", __FUNCTION__, __LINE__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData); 
                return ret;
            }
            uVsIndex = pVoiceService->InstanceNumber;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            if( AnscEqualString(ParamName, "X_RDK_Enable", TRUE) )
            {
                TELCOVOICEMGR_VOICE_ENABLE_ENUM eState = uValue;
                if(TelcoVoiceMgrDmlSetVoiceProcessState(uVsIndex, eState) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        pVoiceService->X_RDK_Enable = eState;
                        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                        return TRUE;
                    }
                }
            }
        }
    }

    return ret;
}

BOOL VoiceService_Validate(ANSC_HANDLE hInsContext, char* pReturnParamName, ULONG* puLength)
{
    /* Stub */
    return TRUE;
}

ULONG VoiceService_Commit(ANSC_HANDLE hInsContext)
{
    /* Stub */
    return 0;
}

ULONG VoiceService_Rollback(ANSC_HANDLE hInsContext)
{
    /* Stub */
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG X_RDK_Debug_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/
LONG X_RDK_Debug_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    ULONG ret = 1;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
          TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);

          if (pVoiceService && pValue && pUlSize)
          {
            if( AnscEqualString(ParamName, "CCTKTraceGroup", TRUE) )
            {
                AnscCopyString(pValue, pVoiceService->X_RDK_DebugObj.CCTKTraceGroup);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "CCTKTraceLevel", TRUE) )
            {
                AnscCopyString(pValue, pVoiceService->X_RDK_DebugObj.CCTKTraceLevel);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "ModuleLogLevels", TRUE) )
            {
                AnscCopyString(pValue, pVoiceService->X_RDK_DebugObj.ModuleLogLevels);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "LogServer", TRUE) )
            {
                AnscCopyString(pValue, pVoiceService->X_RDK_DebugObj.LogServer);
                ret = 0;
            }
          }
          else
          {
            CcspTraceError(("%s:%d:: pVoiceService , pValue or pUlSize NULL\n", __FUNCTION__, __LINE__));
          }

          TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
        
    }

    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL X_RDK_Debug_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL X_RDK_Debug_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceService || !pString)
            {
                CcspTraceError(("%s:%d:: pVoiceService or pString NULL\n", __FUNCTION__, __LINE__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData); 
                return ret;
            }
            uVsIndex = pVoiceService->InstanceNumber;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            if( AnscEqualString(ParamName, "CCTKTraceGroup", TRUE) )
            {
                if(TelcoVoiceMgrDmlSetCCTKTraceGroup(pVoiceService->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        AnscCopyString(pVoiceService->X_RDK_DebugObj.CCTKTraceGroup, pString);
                        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                        return TRUE;
                    }
                }
            }
            else if( AnscEqualString(ParamName, "CCTKTraceLevel", TRUE) )
            {
                if(TelcoVoiceMgrDmlSetCCTKTraceLevel(pVoiceService->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        AnscCopyString(pVoiceService->X_RDK_DebugObj.CCTKTraceLevel, pString);
                        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                        return TRUE;
                    }
                }
            }
            else if( AnscEqualString(ParamName, "ModuleLogLevels", TRUE) )
            {
                if(TelcoVoiceMgrDmlSetModuleLogLevel(pVoiceService->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        AnscCopyString(pVoiceService->X_RDK_DebugObj.ModuleLogLevels, pString);
                        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                        return TRUE;
                    }
                }
            }
            else if( AnscEqualString(ParamName, "LogServer", TRUE) )
            {
                if(TelcoVoiceMgrDmlSetLogServer(pVoiceService->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        AnscCopyString(pVoiceService->X_RDK_DebugObj.LogServer, pString);
                        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                        return TRUE;
                    }
                }
            }
            else
            {
                CcspTraceError(("%s:%d:: Invalid Parameter [%s] \n", __FUNCTION__, __LINE__, ParamName));
            }
        }
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL X_RDK_Debug_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL X_RDK_Debug_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
       TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
       if(pTelcoVoiceMgrDmlData != NULL)
       {
            TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);
            if (pVoiceService && pValue)
            {
                if( AnscEqualString(ParamName, "LogServerPort", TRUE))
                {
                    * pValue = pVoiceService->X_RDK_DebugObj.LogServerPort;
                    ret = TRUE;
                }
            }
            else
            {
                CcspTraceError(("%s:%d:: pVoiceService or  pValue Null \n", __FUNCTION__, __LINE__));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData); 
       }
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL X_RDK_Debug_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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
BOOL X_RDK_Debug_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    DML_VOICE_SERVICE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICESERVICE* pVoiceService = &(pTelcoVoiceMgrCtrl->dml);

            if (pVoiceService)
            {
                uVsIndex = pVoiceService->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "LogServerPort", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetLogServerPort(pVoiceService->InstanceNumber, uValue) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceService->X_RDK_DebugObj.LogServerPort = uValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
            }
            else
            {
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                CcspTraceError(("%s:%d:: pVoiceService Null \n", __FUNCTION__, __LINE__));
            }
        }
    }

    return ret;
}
