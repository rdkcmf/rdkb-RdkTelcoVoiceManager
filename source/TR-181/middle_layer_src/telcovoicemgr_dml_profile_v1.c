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
#include "telcovoicemgr_dml_profile_v1.h"
#include "telcovoicemgr_services_apis_v1.h"
#include "telcovoicemgr_dml_hal.h"
#include "telcovoicemgr_dml_json_cfg_init.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"

/***********************************************************************/
/***********************************************************************

 APIs for Object:

    Services.VoiceService.{i}.VoiceProfile.{i}
   
    *  VoiceProfile_IsUpdated
    *  VoiceProfile_Synchronize
    *  VoiceProfile_GetParamStringValue
    *  VoiceProfile_SetParamStringValue
    *  VoiceProfile_GetParamUlongValue
    *  VoiceProfile_SetParamUlongValue
    *  VoiceProfile_GetEntryCount
    *  VoiceProfile_GetEntry
    *  VoiceProfile_Validate
    *  VoiceProfile_Commit
    *  VoiceProfile_Rollback

    Services.VoiceService.{i}.VoiceProfile.{i}.SIP.

    *  SIP_IsUpdated
    *  SIP_Synchronize
    *  SIP_GetParamStringValue
    *  SIP_SetParamStringValue
    *  SIP_GetParamUlongValue
    *  SIP_SetParamUlongValue
    *  SIP_GetParamBoolValue
    *  SIP_SetParamBoolValue

***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG VoiceProfile_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if VoiceProfile updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    
BOOL VoiceProfile_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;
    return bIsUpdated;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG VoiceProfile_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize VoiceProfile data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG VoiceProfile_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
}

/******************************************** **************************

    caller:     owner of this object

    prototype:

        BOOL VoiceProfile_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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
LONG VoiceProfile_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{

    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    ULONG ret = 1;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
          TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

          if (pVoiceProfile && pValue && pUlSize)
          {
            if( AnscEqualString(ParamName, "X_RDK-Central_COM_DigitMap", TRUE) )
            { 
                if(strlen(pVoiceProfile->X_RDK_DigitMap) < *pUlSize)
                {
                  AnscCopyString(pValue, pVoiceProfile->X_RDK_DigitMap);
                  ret = 0;
                }
                else
                {
	           CcspTraceWarning(("%s: Buffer length insufficient, ParamName:%s\n", __FUNCTION__, ParamName));
                   //X_RDK-Central_COM_DigitMap supports maximum 2048 characters
                   // But by default,  the DML library will allocate 1023 buffer size only
                   // If we need more size just put the new size and return 1
                   // If return value is 1 ,the library will allocate required buffer space for us if new size is specified  
                   *pUlSize = MAXLENGTH_DIGITMAP;                
                   ret = 1;
                }
            }
            else if( AnscEqualString(ParamName, "Enable", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->Enable);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "X_RDK-Central_COM_EmergencyDigitMap", TRUE) )
            {
                if(strlen(pVoiceProfile->EmergencyDigitMap) < *pUlSize)
                {
                  AnscCopyString(pValue, pVoiceProfile->EmergencyDigitMap);
                  ret = 0;
                }
                else
                {
                   CcspTraceWarning(("%s: Buffer length insufficient, ParamName:%s\n", __FUNCTION__, ParamName));
                   //X_RDK-Central_COM_EmergencyDigitMap suppors maximum 2048 characters
                   // But by default, the DML library will allocate 1023 buffer size only
                   // If we need more size just put the new size and return 1
                   // If return value is 1 ,the library will allocate required buffer space for us if new size is specified  
                   *pUlSize = MAXLENGTH_DIGITMAP;                
                   ret = 1;
                }
            }
            else if( AnscEqualString(ParamName, "Alias", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->Alias);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "Name", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->Name);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "SignalingProtocol", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SignalingProtocol);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "DTMFMethod", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->DTMFMethod);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "DTMFMethodG711", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->DTMFMethodG711);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "Region", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->Region);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "DigitMap", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->DigitMap);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "STUNServer", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->STUNServer);
                ret = 0;
            }
            else
            {
                CcspTraceError(("%s: Invalid ParamName:%s \n", __FUNCTION__, ParamName));
            }
          }
          TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }   
    }

    CcspTraceWarning(("%s: ParamName:%s Value : %s\n", __FUNCTION__, ParamName, pValue));
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceProfile_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to Set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer to set value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL VoiceProfile_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pString)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return FALSE;
            }
            else
            {
                pDmlVoiceService = pVoiceProfile->pParentVoiceService;
                uVsIndex = pDmlVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "X_RDK-Central_COM_DigitMap", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetDigitMap(uVsIndex, uVpIndex, pString, "X_RDK-Central_COM_DigitMap") == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            AnscCopyString(pVoiceProfile->X_RDK_DigitMap, pString);
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "X_RDK-Central_COM_EmergencyDigitMap", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetDigitMap(uVsIndex, uVpIndex, pString, "X_RDK-Central_COM_EmergencyDigitMap") == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            AnscCopyString(pVoiceProfile->EmergencyDigitMap, pString);
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "DigitMap", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetDigitMap(uVsIndex, uVpIndex, pString, "DigitMap") == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            AnscCopyString(pVoiceProfile->DigitMap, pString);
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                }
            }
        }
    }
    else
    {
       CcspTraceWarning(("%s::Invalid Object :%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceProfile_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       puLong,
                The ULong value to return;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL VoiceProfile_GetParamUlongValue(ANSC_HANDLE hInsContext, char*  ParamName, ULONG* puValue)
{

    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);
            if (!pVoiceProfile || !puValue)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }
            if( AnscEqualString(ParamName, "X_RDK-Central_COM_SDigitTimer", TRUE) )
            {
                * puValue = pVoiceProfile->SDigitTimer;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "X_RDK-Central_COM_ZDigitTimer", TRUE) )
            {
                * puValue = pVoiceProfile->ZDigitTimer;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "Enable", TRUE) )
            {
                *puValue = pVoiceProfile->Enable;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "NumberOfLines", TRUE) )
            {
                *puValue = pVoiceProfile->NumberOfLines;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "MaxSessions", TRUE) )
            {
                *puValue = pVoiceProfile->MaxSessions;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "NonVoiceBandwidthReservedUpstream", TRUE) )
            {
                *puValue = pVoiceProfile->NonVoiceBandwidthReservedUpstream;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "NonVoiceBandwidthReservedDownstream", TRUE) )
            {
                *puValue = pVoiceProfile->NonVoiceBandwidthReservedDownstream;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "FaxPassThrough", TRUE) )
            {
                *puValue = pVoiceProfile->FaxPassThrough;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "ModemPassThrough", TRUE) )
            {
                *puValue = pVoiceProfile->ModemPassThrough;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
        }
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL VoiceProfile_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to retrieve Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue,
                The ULong value to set;

    return:     TRUE if succeeded.

**********************************************************************/
BOOL VoiceProfile_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{

    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }

            pDmlVoiceService = pVoiceProfile->pParentVoiceService;
            if(pDmlVoiceService)
            {
                uVsIndex = pDmlVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "X_RDK-Central_COM_SDigitTimer", TRUE) )
                {
                    if( TelcoVoiceMgrDmlSetSDigitTimer(uVsIndex, uVpIndex, uValue )== ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceProfile->SDigitTimer  =  uValue ;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "X_RDK-Central_COM_ZDigitTimer", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetZDigitTimer(uVsIndex, uVpIndex, uValue ) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceProfile->ZDigitTimer  =  uValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
            }
            else
            {
                CcspTraceWarning(("%s::Invalid Object pDmlVoiceService\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            }
        }    
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG VoiceProfile_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG VoiceProfile_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceService = &(pVoiceService->dml);   
            count =  pDmlVoiceService->VoiceProfileList.ulQuantity;

            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return count;
}

/**********************************************************************

    caller:     owner of this object
        BOOL VoiceProfile_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL VoiceProfile_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "Reset", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "DigitMapEnable", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "STUNEnable", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "PSTNFailOver", TRUE) )
    {
        ret = TRUE;
    }
    else
    {
         CcspTraceWarning(("%s::Unknown ParamName_Ret_OK:%s\n", __FUNCTION__, ParamName));
        ret = TRUE;
    }
    
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

    BOOL VoiceProfile_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL VoiceProfile_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlPhyInterface: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    if(pDmlVoiceProfile != NULL)
    {
        if( AnscEqualString(ParamName, "Reset", TRUE))
        {
            *pBool = pDmlVoiceProfile->Reset;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "DigitMapEnable", TRUE))
        {
            *pBool = pDmlVoiceProfile->DigitMapEnable;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "STUNEnable", TRUE))
        {
            *pBool = pDmlVoiceProfile->STUNEnable;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "PSTNFailOver", TRUE))
        {
            *pBool = pDmlVoiceProfile->PSTNFailOver;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE VoiceProfile_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE VoiceProfile_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceService = &(pVoiceService->dml);
            if(nIndex < pDmlVoiceService->VoiceProfileList.ulQuantity)
            {   
                DML_PROFILE_CTRL_T* pDmlVoiceProfile = pDmlVoiceService->VoiceProfileList.pdata[nIndex];
                if(pDmlVoiceProfile != NULL)
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pDmlVoiceProfile;
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

        BOOL VoiceProfile_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
BOOL VoiceProfile_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL VoiceProfile_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG VoiceProfile_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************  

    caller:     owner of this object 

    prototype: 

        BOOL VoiceProfile_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/
ULONG VoiceProfile_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL SIP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

    return:     0  if succeeded.
                1  if short of buufer
                -1 if not supported

**********************************************************************/
LONG SIP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{

    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    ULONG ret = 1;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
          TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

          if (pVoiceProfile != NULL)
          {
            if( AnscEqualString(ParamName, "OutboundProxy", TRUE) )
            { 
                AnscCopyString(pValue, pVoiceProfile->SIPObj.OutboundProxy);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "ProxyServer", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.ProxyServer);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "RegistrarServer", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.RegistrarServer);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "UserAgentDomain", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.UserAgentDomain);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "X_RDK-Central_COM_ConferencingURI", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.ConferencingURI);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "X_RDK_Firewall_Rule_Data", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.X_RDK_Firewall_Rule_Data);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "UserAgentTransport", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.UserAgentTransport);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "RegistrarServerTransport", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.RegistrarServerTransport);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "Organization", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.Organization);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "InboundAuth", TRUE) )
            {
                if(pVoiceProfile->SIPObj.InboundAuth == SIP_INBOUNDAUTH_NONE)
                {
                    AnscCopyString(pValue, "None");
                }
                else if(pVoiceProfile->SIPObj.InboundAuth == SIP_INBOUNDAUTH_DIGEST)
                {
                    AnscCopyString(pValue, "Digest");
                }
                
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "InboundAuthUsername", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.InboundAuthUsername);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "InboundAuthPassword", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.InboundAuthPassword);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "RegistrarServerTransport", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.RegistrarServerTransport);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "ProxyServerTransport", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->SIPObj.ProxyServerTransport);
                ret = 0;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
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

        BOOL SIP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL SIP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pString)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return FALSE;
            }
            else
            {
                pDmlVoiceService = pVoiceProfile->pParentVoiceService;
                uVsIndex = pDmlVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if ( AnscEqualString(ParamName, "OutboundProxy", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetOutboundProxy(uVsIndex,pVoiceProfile->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            AnscCopyString(pVoiceProfile->SIPObj.OutboundProxy, pString);
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "ProxyServer", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetProxyServer(uVsIndex,pVoiceProfile->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            AnscCopyString(pVoiceProfile->SIPObj.ProxyServer, pString);
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "RegistrarServer", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetRegistrarServer(uVsIndex,pVoiceProfile->InstanceNumber, pString) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            AnscCopyString(pVoiceProfile->SIPObj.RegistrarServer, pString);
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "UserAgentDomain", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetUserAgentDomain(uVsIndex, uVpIndex, pString) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            AnscCopyString(pVoiceProfile->SIPObj.UserAgentDomain, pString);
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "X_RDK-Central_COM_ConferencingURI", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetConferencingURI(uVsIndex, uVpIndex, pString) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            AnscCopyString(pVoiceProfile->SIPObj.ConferencingURI, pString);
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "UserAgentTransport", TRUE) )
                {
                    ret = 0;
                }
                else if( AnscEqualString(ParamName, "RegistrarServerTransport", TRUE) )
                {
                    ret = 0;
                }
                else if( AnscEqualString(ParamName, "Organization", TRUE) )
                {
                    ret = 0;
                }
                else if( AnscEqualString(ParamName, "InboundAuth", TRUE) )
                {
                    ret = 0;
                }
                else if( AnscEqualString(ParamName, "InboundAuthUsername", TRUE) )
                {
                    ret = 0;
                }
                else if( AnscEqualString(ParamName, "InboundAuthPassword", TRUE) )
                {
                    ret = 0;
                }
                else if( AnscEqualString(ParamName, "RegistrarServerTransport", TRUE) )
                {
                    ret = 0;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                    ret =  0;
                }
            }
        }
        
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL SIP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL SIP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{

    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pValue)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }

            if( AnscEqualString(ParamName, "OutboundProxyPort", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.OutboundProxyPort;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "ProxyServerPort", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.ProxyServerPort;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "RegistrarServerPort", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.RegistrarServerPort;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "DSCPMark", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.DSCPMark;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "X_RDK_SKBMark", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.X_RDK_SKBMark;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "UserAgentPort", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.UserAgentPort;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "RegistrationPeriod", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.RegistrationPeriod;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerT1", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerT1;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerT2", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerT2;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerT4", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerT4;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerA", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerA;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerB", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerB;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerC", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerC;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerD", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerD;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerE", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerE;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerF", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerF;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerG", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerG;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerH", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerH;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerI", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerI;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerJ", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerJ;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TimerK", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.TimerK;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "InviteExpires", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.InviteExpires;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "ReInviteExpires", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.ReInviteExpires;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "RegisterExpires", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.RegisterExpires;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "RegistersMinExpires", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.RegistersMinExpires;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "RegisterRetryInterval", TRUE) )
            {
                * pValue = pVoiceProfile->SIPObj.RegisterRetryInterval;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }    
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL SIP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL SIP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }
            pDmlVoiceService = pVoiceProfile->pParentVoiceService;
            if(pDmlVoiceService)
            {
                uVsIndex = pDmlVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "OutboundProxyPort", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetOutboundProxyPort(uVsIndex,uVpIndex, uValue) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceProfile->SIPObj.OutboundProxyPort  =  uValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "ProxyServerPort", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetProxyServerPort(uVsIndex,uVpIndex, uValue) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceProfile->SIPObj.ProxyServerPort  =  uValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "RegistrarServerPort", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetRegistrarServerPort(uVsIndex,uVpIndex, uValue) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceProfile->SIPObj.RegistrarServerPort  =  uValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "DSCPMark", TRUE) )
                {
                        pVoiceProfile->SIPObj.DSCPMark  =  uValue;
                        (void)storeObjectInteger(uVsIndex, uVpIndex, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "DSCPMark", uValue);
                        ret = TRUE;
                }
                else if( AnscEqualString(ParamName, "UserAgentPort", TRUE) )
                {
                        pVoiceProfile->SIPObj.UserAgentPort  =  uValue;
                        ret = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                    ret =  FALSE;
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

        BOOL SIP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

    description:

        This function is called to retrieve int parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL SIP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pInt)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }

            if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
            {
               * pInt = pVoiceProfile->SIPObj.EthernetPriorityMark;
               ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "VLANIDMark", TRUE) )
            {
               * pInt = pVoiceProfile->SIPObj.VLANIDMark;
               ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                ret =  FALSE;
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
        
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL SIP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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
BOOL SIP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }

            pDmlVoiceService = pVoiceProfile->pParentVoiceService;
            if(pDmlVoiceService)
            {
                uVsIndex = pDmlVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetSipEthernetPriorityMark(uVsIndex,uVpIndex,iValue) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceProfile->SIPObj.EthernetPriorityMark  =  iValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "VLANIDMark", TRUE) )
                {
                    ret = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                    ret =  FALSE;
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

        BOOL SIP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

    return:     TRUE if succeeded.

**********************************************************************/
BOOL SIP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pBool)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }

            if( AnscEqualString(ParamName, "X_RDK-Central_COM_NetworkDisconnect", TRUE) )
            {
               * pBool = pVoiceProfile->SIPObj.NetworkDisconnect;
               ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "X_RDK_PRACKRequired", TRUE) )
            {
               * pBool = pVoiceProfile->SIPObj.X_RDK_PRACKRequired;
               ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "UseCodecPriorityInSDPResponse", TRUE) )
            {
               * pBool = pVoiceProfile->SIPObj.UseCodecPriorityInSDPResponse;
               ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }   
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL SIP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL SIP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }
            pDmlVoiceService = pVoiceProfile->pParentVoiceService;
            if(pDmlVoiceService)
            {
                uVsIndex = pDmlVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "X_RDK-Central_COM_NetworkDisconnect", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetNetworkDisconnect(uVsIndex,uVpIndex, bValue) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceProfile->SIPObj.NetworkDisconnect  =  bValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else if( AnscEqualString(ParamName, "X_RDK_PRACKRequired", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetPrackRequired(uVsIndex,uVpIndex, bValue) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceProfile->SIPObj.X_RDK_PRACKRequired  =  bValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
                    }
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
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

        BOOL RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pValue)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }

            if( AnscEqualString(ParamName, "DSCPMark", TRUE) )
            {
               *pValue = pVoiceProfile->RTPObj.DSCPMark;
               ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "X_RDK_SKBMark", TRUE) )
            {
               *pValue = pVoiceProfile->RTPObj.X_RDK_SKBMark;
               ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "LocalPortMin", TRUE) )
            {
               *pValue = pVoiceProfile->RTPObj.LocalPortMin;
               ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "LocalPortMax", TRUE) )
            {
               *pValue = pVoiceProfile->RTPObj.LocalPortMax;
               ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "TelephoneEventPayloadType", TRUE) )
            {
               *pValue = pVoiceProfile->RTPObj.TelephoneEventPayloadType;
               ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL RTP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL RTP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                ret = FALSE;
            }
            else
            {
                pDmlVoiceService = pVoiceProfile->pParentVoiceService;
                if( AnscEqualString(ParamName, "DSCPMark", TRUE) )
                {
                    pVoiceProfile->RTPObj.DSCPMark  =  uValue;
                    (void)storeObjectInteger(uVsIndex, uVpIndex, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "RtpDSCPMark", uValue);
                    ret = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
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

        BOOL RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

    description:

        This function is called to retrieve int parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pInt)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }

            if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
            {
               * pInt = pVoiceProfile->RTPObj.EthernetPriorityMark;
               ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "VLANIDMark", TRUE) )
            {
               * pInt = pVoiceProfile->RTPObj.VLANIDMark;
               ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL RTP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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
BOOL RTP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{

    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uVpIndex = 0;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return FALSE;
            }
            else
            {
                pDmlVoiceService = pVoiceProfile->pParentVoiceService;
                uVsIndex = pDmlVoiceService->InstanceNumber;
                uVpIndex = pVoiceProfile->InstanceNumber;
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
                {
                    if(TelcoVoiceMgrDmlSetRtpEthernetPriorityMark(uVsIndex,uVpIndex, iValue) == ANSC_STATUS_SUCCESS)
                    {
                        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                        if(pTelcoVoiceMgrDmlData != NULL)
                        {
                            pVoiceProfile->RTPObj.EthernetPriorityMark  =  iValue;
                            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                            return TRUE;
                        }
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
        BOOL Redundancy_GetParamUlongValue (ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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

BOOL Redundancy_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pValue)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }

            if( AnscEqualString(ParamName,"BlockPayloadType", TRUE))
            {
                *pValue = pVoiceProfile->RTPObj.RedundancyObj.BlockPayloadType;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "PayloadType", TRUE))
            {
                *pValue = pVoiceProfile->RTPObj.RedundancyObj.PayloadType;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName,"MaxSessionsUsingRedundancy", TRUE))
            {
                *pValue = pVoiceProfile->RTPObj.RedundancyObj.MaxSessionsUsingRedundancy;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
        }
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Redundancy_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL Redundancy_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;

    if( AnscEqualString(ParamName, "PayloadType", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "MaxSessionsUsingRedundancy", TRUE))
    {
        ret = TRUE;
    }
    return ret;
}



/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Redundancy_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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
BOOL Redundancy_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "FaxAndModemRedundancy", TRUE) )
    {
        ret = FALSE;
    }
    else if( AnscEqualString(ParamName, "ModemRedundancy", TRUE) )
    {
        ret = FALSE;
    }
    else if( AnscEqualString(ParamName, "DTMFRedundancy", TRUE) )
    {
        ret = FALSE;
    }
    else if( AnscEqualString(ParamName, "VoiceRedundancy", TRUE) )
    {
        ret = FALSE;
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Redundancy_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL Redundancy_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "X_RDK-Central_COM_NetworkDisconnect", TRUE) )
    {
        ret = FALSE;
    }
    else if( AnscEqualString(ParamName, "X_RDK-Central_COM_NetworkDisconnect", TRUE) )
    {
            ret = FALSE;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL  Redundancy_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)

    description:

        This function is called to retrieve int parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL Redundancy_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pInt)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }
            if( AnscEqualString(ParamName, "ModemRedundancy", TRUE) )
            {
                *pInt = pVoiceProfile->RTPObj.RedundancyObj.ModemRedundancy;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "FaxAndModemRedundancy", TRUE) )
            {
                *pInt = pVoiceProfile->RTPObj.RedundancyObj.FaxAndModemRedundancy;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "DTMFRedundancy", TRUE) )
            {
                *pInt = pVoiceProfile->RTPObj.RedundancyObj.DTMFRedundancy;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "VoiceRedundancy", TRUE) )
            {
                *pInt = pVoiceProfile->RTPObj.RedundancyObj.VoiceRedundancy;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

    BOOL Redundancy_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL Redundancy_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pBool)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }
            if( AnscEqualString(ParamName, "Enable", TRUE))
            {
                *pBool = pVoiceProfile->RTPObj.RedundancyObj.Enable;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL RTCP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL RTCP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pValue)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }
            if( AnscEqualString(ParamName, "TxRepeatInterval", TRUE))
            {
                *pValue =  pVoiceProfile->RTPObj.RTCPObj.TxRepeatInterval;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG RTCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG RTCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pValue)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                ret = FALSE;
            }

            if( AnscEqualString(ParamName, "LocalCName", TRUE) )
            {
                AnscCopyString(pValue,pVoiceProfile->RTPObj.RTCPObj.LocalCName);
                ret = 0;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL RTCP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL RTCP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "TxRepeatInterval", TRUE))
    {
        ret = TRUE;
    }
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL RTCP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL RTCP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pVoiceProfile || !pBool)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return ret;
            }
            if( AnscEqualString(ParamName, "Enable", TRUE))
            {
                *pBool = pVoiceProfile->RTPObj.RTCPObj.Enable;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL RTCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL RTCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "LocalCName", TRUE) )
    {
        ret = TRUE;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL RTCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL RTCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        ret = TRUE;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

    return:     0  if succeeded.
                1  if short of buufer
                -1 if not supported

**********************************************************************/

LONG RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
          TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

          if (pVoiceProfile != NULL)
          {
            if( AnscEqualString(ParamName, "X_RDK_Firewall_Rule_Data", TRUE) )
            {
                AnscCopyString(pValue, pVoiceProfile->RTPObj.X_RDK_Firewall_Rule_Data);
                ret = 0;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
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

        BOOL SIPEventSubscribe_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL SIPEventSubscribe_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    DML_SIP_EVENTSUBSCRIBE_CTRL_T* pSIPEvtSubscribe = (DML_SIP_EVENTSUBSCRIBE_CTRL_T *) hInsContext;
    if(pSIPEvtSubscribe == NULL)
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

    PTELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE pDmlSIPEvtSubscribe = &(pSIPEvtSubscribe->dml);
    if(pDmlSIPEvtSubscribe == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlSIPEvtSubscribe: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Event", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Notifier", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "NotifierTransport", TRUE) )
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
    }
    
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL SRTP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL SRTP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_VOICEPROFILE* pDmlVoiceProfile = &(pTelcoVoiceMgrCtrl->dml);

            if (!pDmlVoiceProfile || !pBool)
            {
                CcspTraceError(("%s: NULL Pointer\n", __FUNCTION__));
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
                return FALSE;
            }

            PTELCOVOICEMGR_DML_RTP_SRTP pDmlRtpSRTP = &(pDmlVoiceProfile->RTPObj.SRTPObj);

            if( AnscEqualString(ParamName, "Enable", TRUE))
            {
                *pBool = pDmlRtpSRTP->Enable;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL SRTP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL SRTP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_RTP_SRTP pDml_RTP_SRTP = &(pDmlVoiceProfile->RTPObj.SRTPObj);
    if(pDml_RTP_SRTP != NULL)
    {

        if( AnscEqualString(ParamName, "Enable", TRUE) )
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
        
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL SRTP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL SRTP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_RTP_SRTP pDml_RTP_SRTP = &(pDmlVoiceProfile->RTPObj.SRTPObj);
    if(pDml_RTP_SRTP != NULL)
    {
        if( AnscEqualString(ParamName, "KeyingMethods", TRUE) )
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "EncryptionKeySizes", TRUE) )
        {
            ret = TRUE;
        }
        else
        {
           CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName)); 
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG SRTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG SRTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_RTP_SRTP pDml_RTP_SRTP = &(pDmlVoiceProfile->RTPObj.SRTPObj);
    if(pDml_RTP_SRTP != NULL)
    {
        if( AnscEqualString(ParamName, "KeyingMethods", TRUE) )
        {
            AnscCopyString(pValue,pDml_RTP_SRTP->KeyingMethods);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "EncryptionKeySizes", TRUE) )
        {
            AnscCopyString(pValue,pDml_RTP_SRTP->EncryptionKeySizes);
            ret = 0;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            ret = -1;
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL H323_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL H323_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_H323 pDmlH323 = &(pDmlVoiceProfile->H323Obj);
    if(pDmlH323 != NULL)
    {
        if( AnscEqualString(ParamName, "GatekeeperPort", TRUE))
        {
            *pValue = pDmlH323->GatekeeperPort;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "TimeToLive", TRUE))
        {
            *pValue = pDmlH323->TimeToLive;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "DSCPMark", TRUE))
        {
            *pValue = pDmlH323->DSCPMark;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL H323_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL H323_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_H323 pDmlH323 = &(pDmlVoiceProfile->H323Obj);
    if(pDmlH323 != NULL)
    {
        if( AnscEqualString(ParamName, "H235Authentication", TRUE))
        {
            *pBool = pDmlH323->H235Authentication;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG H323_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG H323_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_H323 pDmlH323 = &(pDmlVoiceProfile->H323Obj);
    if(pDmlH323 != NULL)
    {
        if( AnscEqualString(ParamName, "GatekeeperID", TRUE) )
        {
            AnscCopyString(pValue,pDmlH323->GatekeeperID);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "Gatekeeper", TRUE) )
        {
            AnscCopyString(pValue,pDmlH323->Gatekeeper);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "AuthPassword", TRUE) )
        {
            AnscCopyString(pValue,pDmlH323->AuthPassword);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "SendersID", TRUE) )
        {
            AnscCopyString(pValue,pDmlH323->SendersID);
            ret = 0;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL H323_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL H323_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_H323 pDmlH323 = &(pDmlVoiceProfile->H323Obj);
    if(pDmlH323 != NULL)
    {
        if( AnscEqualString(ParamName, "H235Authentication", TRUE) )
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL H323_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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
BOOL H323_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_H323 pDmlH323 = &(pDmlVoiceProfile->H323Obj);
    if(pDmlH323 != NULL)
    {
        if( AnscEqualString(ParamName, "VLANIDMark", TRUE) )
        {
            ret = FALSE;
        }
        else if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
        {
            ret = FALSE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}   

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL H323_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL H323_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_H323 pDmlH323 = &(pDmlVoiceProfile->H323Obj);
    if(pDmlH323 != NULL)
    {
        if( AnscEqualString(ParamName, "GatekeeperPort", TRUE))
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "TimeToLive", TRUE))
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "DSCPMark", TRUE))
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL H323_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL H323_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_H323 pDmlH323 = &(pDmlVoiceProfile->H323Obj);
    if(pDmlH323 != NULL)
    {
        if( AnscEqualString(ParamName, "Gatekeeper", TRUE) )
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "GatekeeperID", TRUE) )
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "AuthPassword", TRUE) )
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "SendersID", TRUE) )
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL  H323_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)

    description:

        This function is called to retrieve int parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL H323_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_H323 pDmlH323 = &(pDmlVoiceProfile->H323Obj);
    if(pDmlH323 != NULL)
    {
        if( AnscEqualString(ParamName, "VLANIDMark", TRUE) )
        {
            *pInt = pDmlH323->VLANIDMark;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
        {
            *pInt = pDmlH323->EthernetPriorityMark;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "GatekeeperPort", TRUE) )
        {
            *pInt = pDmlH323->GatekeeperPort;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "TimeToLive", TRUE) )
        {
            *pInt = pDmlH323->TimeToLive;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "DSCPMark", TRUE) )
        {
            *pInt = pDmlH323->DSCPMark;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:                                                                                                                                                                                               
        BOOL MGCP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL MGCP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_MGCP pDmlMGCP = &(pDmlVoiceProfile->MGCPObj);
    if(pDmlMGCP != NULL)
    {
        if( AnscEqualString(ParamName, "CallAgentPort1", TRUE))
        {
            *pValue = pDmlMGCP->CallAgentPort1;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "CallAgentPort2", TRUE))
        {
            *pValue = pDmlMGCP->CallAgentPort2;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "RetranIntervalTimer", TRUE))
        {
            *pValue = pDmlMGCP->RetranIntervalTimer;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "MaxRetranCount", TRUE))
        {
            *pValue = pDmlMGCP->MaxRetranCount;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "LocalPort", TRUE))
        {
            *pValue = pDmlMGCP->LocalPort;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "DSCPMark", TRUE))
        {
            *pValue = pDmlMGCP->DSCPMark;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL MGCP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL MGCP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{

    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_MGCP pDmlMGCP = &(pDmlVoiceProfile->MGCPObj);
    if(pDmlMGCP != NULL)
    {

        if( AnscEqualString(ParamName, "CallAgentPort1", TRUE))
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "CallAgentPort2", TRUE))
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "RetranIntervalTimer", TRUE))
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "MaxRetranCount", TRUE))
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "LocalPort", TRUE))
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "DSCPMark", TRUE))
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }

    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL MGCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL MGCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
        DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_MGCP pDmlMGCP = &(pDmlVoiceProfile->MGCPObj);
    if(pDmlMGCP != NULL)
    {

        if( AnscEqualString(ParamName, "AllowPiggybackEvents", TRUE) )
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "SendRSIPImmediately", TRUE) )
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL MGCP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL MGCP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_MGCP pDmlMGCP = &(pDmlVoiceProfile->MGCPObj);
    if(pDmlMGCP != NULL)
    {

        if( AnscEqualString(ParamName, "AllowPiggybackEvents", TRUE))
        {
            *pBool = pDmlMGCP->AllowPiggybackEvents; 
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "SendRSIPImmediately", TRUE))
        {
            *pBool = pDmlMGCP->SendRSIPImmediately;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL MGCP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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
BOOL MGCP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_MGCP pDmlMGCP = &(pDmlVoiceProfile->MGCPObj);
    if(pDmlMGCP != NULL)
    {
        if( AnscEqualString(ParamName, "VLANIDMark", TRUE) )
        {
            ret = FALSE;
        }
        else if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
        {
            ret = FALSE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}   

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL ButtonMap_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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

BOOL ButtonMap_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_BUTTONMAP pDmlButtonMap = &(pDmlVoiceProfile->ButtonMapObj);
    if(pDmlButtonMap != NULL)
    {
        if( AnscEqualString(ParamName, "NumberOfButtons", TRUE))
        {
            *pValue = pDmlButtonMap->NumberOfButtons;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG Button_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG Button_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_PROFILE    pDmlVoiceProfile    = NULL;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pVoiceProfile != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceProfile = &(pVoiceProfile->dml);   
            count =  pDmlVoiceProfile->ButtonMapObj.ButtonList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return count;
}
/**********************************************************************
    caller:     owner of this object

    prototype:

        ANSC_HANDLE Button_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE Button_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_PROFILE    pDmlVoiceProfile    = NULL;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pVoiceProfile != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceProfile = &(pVoiceProfile->dml);
            if(nIndex < pDmlVoiceProfile->ButtonMapObj.ButtonList.ulQuantity)
            {   
                DML_BUTTON_CTRL_T* pButton = pDmlVoiceProfile->ButtonMapObj.ButtonList.pdata[nIndex];
                if(pButton != NULL)
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pButton;
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

        BOOL Button_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL Button_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "UserAccess", TRUE) )
    {
        ret = FALSE;
    }
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL Button_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL Button_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
        BOOL ret = FALSE;
        if( AnscEqualString(ParamName, "UserAccess", TRUE))
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            ret =  TRUE;
        }
        return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG Button_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG Button_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "ButtonName", TRUE) )
    {
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "FacilityAction", TRUE) )
    {
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "FacilityActionArgument", TRUE) )
    {
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "QuickDialNumber", TRUE) )
    {
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "ButtonMessage", TRUE) )
    {
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL FaxT38_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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

BOOL FaxT38_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;

    if( AnscEqualString(ParamName, "BitRate", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"HighSpeedPacketRate", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"HighSpeedRedundancy", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName,"LowSpeedRedundancy", TRUE))
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        ret =  TRUE;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL FaxT38_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL FaxT38_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;

    if( AnscEqualString(ParamName, "BitRate", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "HighSpeedPacketRate", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "HighSpeedRedundancy", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "LowSpeedRedundancy", TRUE))
    {
        ret = TRUE;
    }
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL FaxT38_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL FaxT38_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "Enable", TRUE))
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        ret =  TRUE;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL FaxT38_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL FaxT38_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "Enable", TRUE) )
    {
        ret = FALSE;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL FaxT38_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL FaxT38_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "TCFMethod", TRUE) )
    {
        ret = TRUE;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG FaxT38_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG FaxT38_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    if( AnscEqualString(ParamName, "TCFMethod", TRUE) )
    {
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        ret =  0;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG SIPResponseMap_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG SIPResponseMap_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_PROFILE    pDmlVoiceProfile    = NULL;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pVoiceProfile != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceProfile = &(pVoiceProfile->dml);   
            count =  pDmlVoiceProfile->SIPObj.SipResponseMapList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return count;
}
/**********************************************************************
    caller:     owner of this object

    prototype:

        ANSC_HANDLE SIPResponseMap_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE SIPResponseMap_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_PROFILE    pDmlVoiceProfile    = NULL;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pVoiceProfile != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceProfile = &(pVoiceProfile->dml);
            if(nIndex < pDmlVoiceProfile->SIPObj.SipResponseMapList.ulQuantity)
            {   
                DML_SIP_RESPONSEMAP_CTRL_T* pSIPRespMap = pDmlVoiceProfile->SIPObj.SipResponseMapList.pdata[nIndex];
                if(pSIPRespMap != NULL)
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pSIPRespMap;
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

        BOOL SIPResponseMap_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL SIPResponseMap_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "SIPResponseNumber", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Tone", TRUE) )
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        ret =  TRUE;
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG SIPResponseMap_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG SIPResponseMap_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "TextMessage", TRUE) )
    {
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        ret =  0;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL SIPResponseMap_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG pValue);

    description:

        This function is called to set Ulong parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The value need to set

    return:     TRUE if succeeded.

**********************************************************************/
BOOL SIPResponseMap_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG pValue)
{
    BOOL ret = FALSE;

    if( AnscEqualString(ParamName, "SIPResponseNumber", TRUE))
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "Tone", TRUE))
    {
        ret = TRUE;
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL SIPResponseMap_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL SIPResponseMap_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "TextMessage", TRUE) )
    {
        ret = TRUE;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG SIPEventSubscribe_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG SIPEventSubscribe_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG count = 0;
    PTELCOVOICEMGR_DML_PROFILE    pDmlVoiceProfile    = NULL;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pVoiceProfile != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceProfile = &(pVoiceProfile->dml);   
            count =  pDmlVoiceProfile->SIPObj.SipEventSubscribeList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return count;
}
/**********************************************************************
    caller:     owner of this object

    prototype:

        ANSC_HANDLE SIPEventSubscribe_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE SIPEventSubscribe_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_PROFILE    pDmlVoiceProfile    = NULL;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T*) hInsContext;
    if(pVoiceProfile != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceProfile = &(pVoiceProfile->dml);
            if(nIndex < pDmlVoiceProfile->SIPObj.SipEventSubscribeList.ulQuantity)
            {   
                DML_SIP_EVENTSUBSCRIBE_CTRL_T* pSIPEvtSubscribe = pDmlVoiceProfile->SIPObj.SipEventSubscribeList.pdata[nIndex];
                if(pSIPEvtSubscribe != NULL)
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pSIPEvtSubscribe;
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

        LONG SIPEventSubscribe_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG SIPEventSubscribe_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_SIP_EVENTSUBSCRIBE_CTRL_T* pSIPEvtSubscribe = (DML_SIP_EVENTSUBSCRIBE_CTRL_T *) hInsContext;
    if(pSIPEvtSubscribe == NULL)
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

    PTELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE pDmlSIPEvtSubscribe = &(pSIPEvtSubscribe->dml);
    if(pDmlSIPEvtSubscribe == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlSIPEvtSubscribe: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue,pDmlSIPEvtSubscribe->Alias);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Event", TRUE) )
    {
        AnscCopyString(pValue,pDmlSIPEvtSubscribe->Event);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "NotifierTransport", TRUE) )
    {
        AnscCopyString(pValue,pDmlSIPEvtSubscribe->NotifierTransport);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "Notifier", TRUE) )
    {
        AnscCopyString(pValue,pDmlSIPEvtSubscribe->Notifier);
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL SIPEventSubscribe_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL SIPEventSubscribe_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;
    DML_SIP_EVENTSUBSCRIBE_CTRL_T* pSIPEvtSubscribe = (DML_SIP_EVENTSUBSCRIBE_CTRL_T *) hInsContext;
    if(pSIPEvtSubscribe == NULL)
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

    PTELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE pDmlSIPEvtSubscribe = &(pSIPEvtSubscribe->dml);
    if(pDmlSIPEvtSubscribe == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlSIPEvtSubscribe: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    

    if( AnscEqualString(ParamName, "NotifierPort", TRUE))
    {
        *pValue = pDmlSIPEvtSubscribe->NotifierPort;
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ExpireTime", TRUE))
    {
        *pValue = pDmlSIPEvtSubscribe->ExpireTime;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL SIPEventSubscribe_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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
BOOL SIPEventSubscribe_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    DML_SIP_EVENTSUBSCRIBE_CTRL_T* pSIPEvtSubscribe = (DML_SIP_EVENTSUBSCRIBE_CTRL_T *) hInsContext;
    if(pSIPEvtSubscribe == NULL)
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

    PTELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE pDmlSIPEvtSubscribe = &(pSIPEvtSubscribe->dml);
    if(pDmlSIPEvtSubscribe == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlSIPEvtSubscribe: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    if( AnscEqualString(ParamName, "NotifierPort", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ExpireTime", TRUE) )
    {
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG ServiceProviderInfo_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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
LONG ServiceProviderInfo_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    if( AnscEqualString(ParamName, "Name", TRUE) )
    {
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "ContactPhoneNumber", TRUE) )
    {
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "URL", TRUE) )
    {
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "EmailAddress", TRUE) )
    {
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        ret =  0;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL ServiceProviderInfo_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL ServiceProviderInfo_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    if( AnscEqualString(ParamName, "Name", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "ContactPhoneNumber", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "URL", TRUE) )
    {
        ret = TRUE;
    }
    else if( AnscEqualString(ParamName, "EmailAddress", TRUE) )
    {
        ret = TRUE;
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG MGCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG MGCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_MGCP pDmlMGCP = &(pDmlVoiceProfile->MGCPObj);
    if(pDmlMGCP != NULL)
    {
        if( AnscEqualString(ParamName, "CallAgent1", TRUE) )
        {
            AnscCopyString(pValue, pDmlMGCP->CallAgent1);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "CallAgent2", TRUE) )
        {
            AnscCopyString(pValue, pDmlMGCP->CallAgent1);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "RegisterMode", TRUE) )
        {
            if (MGCP_REGISTERMODE_WILDCARD == pDmlMGCP->RegisterMode)
            {
                AnscCopyString(pValue, "Wildcard");
                ret = 0;
            }
            else if (MGCP_REGISTERMODE_INDIVIDUAL == pDmlMGCP->RegisterMode)
            {
                AnscCopyString(pValue, "Individual");
                ret = 0;
            }
        }
        else if( AnscEqualString(ParamName, "Domain", TRUE) )
        {
            AnscCopyString(pValue, pDmlMGCP->Domain);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "User", TRUE) )
        {
            AnscCopyString(pValue, pDmlMGCP->User);
            ret = 0;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL  MGCP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)

    description:

        This function is called to retrieve int parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The value returned

    return:     TRUE if succeeded.

**********************************************************************/
BOOL MGCP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_MGCP pDmlMGCP = &(pDmlVoiceProfile->MGCPObj);
    if(pDmlMGCP != NULL)
    {

        if( AnscEqualString(ParamName, "VLANIDMark", TRUE) )
        {
            *pInt = pDmlMGCP->VLANIDMark;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "EthernetPriorityMark", TRUE) )
        {
            *pInt = pDmlMGCP->EthernetPriorityMark;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "CallAgentPort1", TRUE) )
        {
            *pInt = pDmlMGCP->CallAgentPort1;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "CallAgentPort2", TRUE) )
        {
            *pInt = pDmlMGCP->CallAgentPort2;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "RetranIntervalTimer", TRUE) )
        {
            *pInt = pDmlMGCP->RetranIntervalTimer;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "MaxRetranCount", TRUE) )
        {
            *pInt = pDmlMGCP->MaxRetranCount;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "LocalPort", TRUE) )
        {
            *pInt = pDmlMGCP->LocalPort;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "DSCPMark", TRUE) )
        {
            *pInt = pDmlMGCP->DSCPMark;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL MGCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL MGCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    DML_PROFILE_CTRL_T* pVoiceProfile = (DML_PROFILE_CTRL_T *) hInsContext;
    if(pVoiceProfile == NULL)
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

    PTELCOVOICEMGR_DML_PROFILE pDmlVoiceProfile = &(pVoiceProfile->dml);
    if(pDmlVoiceProfile == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_MGCP pDmlMGCP = &(pDmlVoiceProfile->MGCPObj);
    if(pDmlMGCP != NULL)
    {
        if( AnscEqualString(ParamName, "CallAgent1", TRUE) )
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "CallAgent2", TRUE) )
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "RegisterMode", TRUE) )
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "Domain", TRUE) )
        {
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "User", TRUE) )
        {
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        } 
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

