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

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

#include "ansc_platform.h"
#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_numberingplan_v1.h"
#include "telcovoicemgr_services_apis_v1.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"


/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL NumberingPlan_GetParamUlongValue (ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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

BOOL NumberingPlan_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
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
                ret = FALSE;
            }
            else if( AnscEqualString(ParamName, "MinimumNumberOfDigits", TRUE))
            {
                * pValue = pVoiceProfile->NumberingPlanObj.MinimumNumberOfDigits;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName,"MaximumNumberOfDigits", TRUE))
            {
                * pValue = pVoiceProfile->NumberingPlanObj.MaximumNumberOfDigits;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName,"InterDigitTimerStd", TRUE))
            {
                * pValue = pVoiceProfile->NumberingPlanObj.InterDigitTimerStd;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName,"InterDigitTimerOpen", TRUE))
            {
                * pValue = pVoiceProfile->NumberingPlanObj.InterDigitTimerOpen;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName,"InvalidNumberTone", TRUE))
            {
                * pValue = pVoiceProfile->NumberingPlanObj.InvalidNumberTone;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName,"PrefixInfoMaxEntries", TRUE))
            {
                * pValue = pVoiceProfile->NumberingPlanObj.PrefixInfoMaxEntries;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                ret =  TRUE;
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL NumberingPlan_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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
BOOL NumberingPlan_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{

    DML_PROFILE_CTRL_T* pTelcoVoiceMgrCtrl = (DML_PROFILE_CTRL_T*) hInsContext;
    BOOL ret = FALSE;

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
            else if( AnscEqualString(ParamName, "MinimumNumberOfDigits", TRUE))
            {
                pVoiceProfile->NumberingPlanObj.MinimumNumberOfDigits = uValue;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "MaximumNumberOfDigits", TRUE))
            {
                pVoiceProfile->NumberingPlanObj.MaximumNumberOfDigits = uValue;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "InterDigitTimerStd", TRUE))
            {
                pVoiceProfile->NumberingPlanObj.InterDigitTimerStd = uValue;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "InterDigitTimerOpen", TRUE))
            {
                pVoiceProfile->NumberingPlanObj.InterDigitTimerOpen = uValue;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "InvalidNumberTone", TRUE))
            {
                pVoiceProfile->NumberingPlanObj.InvalidNumberTone = uValue;
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

        ULONG PrefixInfo_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG PrefixInfo_GetEntryCount(ANSC_HANDLE hInsContext)
{
    return 0;
}
/**********************************************************************
    caller:     owner of this object

    prototype:

        ANSC_HANDLE PrefixInfo_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE PrefixInfo_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    return pDmlEntry;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL PrefixInfo_GetParamUlongValue (ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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

BOOL PrefixInfo_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T* pTelcoVoiceMgrCtrl = (DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    BOOL ret = FALSE;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO* pPrefixInfo = &(pTelcoVoiceMgrCtrl->dml);
            if (pPrefixInfo && pValue)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pPrefixInfo->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pPrefixInfo)
            {
                if( AnscEqualString(ParamName, "PrefixMinNumberOfDigits", TRUE))
                {
                    * pValue = pPrefixInfo->PrefixMinNumberOfDigits;
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName,"PrefixMaxNumberOfDigits", TRUE))
                {
                    * pValue = pPrefixInfo->PrefixMaxNumberOfDigits;
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName,"NumberOfDigitsToRemove", TRUE))
                {
                    * pValue = pPrefixInfo->NumberOfDigitsToRemove;
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName,"PosOfDigitsToRemove", TRUE))
                {
                    * pValue = pPrefixInfo->PosOfDigitsToRemove;
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName,"DialTone", TRUE))
                {
                    * pValue = pPrefixInfo->DialTone;
                    ret = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                    ret =  TRUE;
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

        LONG PrefixInfo_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG PrefixInfo_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T* pTelcoVoiceMgrCtrl = (DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    ULONG ret = 1;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO* pPrefixInfo = &(pTelcoVoiceMgrCtrl->dml);
            if (pPrefixInfo && pValue)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pPrefixInfo->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pPrefixInfo)
            {
                if( AnscEqualString(ParamName, "Alias", TRUE) )
                {
                    AnscCopyString(pValue, pPrefixInfo->Alias);
                    ret = 0;
                }
                else if( AnscEqualString(ParamName, "FacilityAction", TRUE) )
                {
                    AnscCopyString(pValue, pPrefixInfo->FacilityAction);
                    ret = 0;
                }
                else if( AnscEqualString(ParamName, "FacilityActionArgument", TRUE) )
                {
                    AnscCopyString(pValue, pPrefixInfo->FacilityActionArgument);
                    ret = 0;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                    ret = 0;
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

        BOOL PrefixInfo_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL PrefixInfo_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T* pTelcoVoiceMgrCtrl = (DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    BOOL ret = FALSE;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO* pPrefixInfo = &(pTelcoVoiceMgrCtrl->dml);
            if (pPrefixInfo && pString)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pPrefixInfo->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pPrefixInfo)
            {
                if( AnscEqualString(ParamName, "Alias", TRUE) )
                {
                    AnscCopyString(pPrefixInfo->Alias, pString); 
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName, "PrefixRange", TRUE) )
                {
                    AnscCopyString(pPrefixInfo->PrefixRange, pString); 
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName, "FacilityAction", TRUE) )
                {
                    AnscCopyString(pPrefixInfo->FacilityAction, pString); 
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName, "FacilityActionArgument", TRUE) )
                {
                    AnscCopyString(pPrefixInfo->FacilityActionArgument, pString); 
                    ret = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                    ret =  TRUE;
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

        BOOL PrefixInfo_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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
BOOL PrefixInfo_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T* pTelcoVoiceMgrCtrl = (DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T*) hInsContext;
    PTELCOVOICEMGR_DML_PROFILE pVoiceProfile = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = NULL;
    BOOL ret = FALSE;

    CcspTraceWarning(("%s::ParamName:%s\n", __FUNCTION__, ParamName));

    if(pTelcoVoiceMgrCtrl != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO* pPrefixInfo = &(pTelcoVoiceMgrCtrl->dml);
            if (pPrefixInfo)
            {
                pVoiceProfile = (PTELCOVOICEMGR_DML_PROFILE) pPrefixInfo->pParentVoiceProfile;
                if (pVoiceProfile)
                {
                    pVoiceService = pVoiceProfile->pParentVoiceService;
                }
            }

            if (pVoiceService && pVoiceProfile && pPrefixInfo)
            {
                if( AnscEqualString(ParamName, "PrefixMinNumberOfDigits", TRUE))
                {
                    pPrefixInfo->PrefixMinNumberOfDigits = uValue;
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName,"PrefixMaxNumberOfDigits", TRUE))
                {
                    pPrefixInfo->PrefixMaxNumberOfDigits = uValue;
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName,"NumberOfDigitsToRemove", TRUE))
                {
                    pPrefixInfo->NumberOfDigitsToRemove = uValue;
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName,"PosOfDigitsToRemove", TRUE))
                {
                    pPrefixInfo->PosOfDigitsToRemove = uValue;
                    ret = TRUE;
                }
                else if( AnscEqualString(ParamName,"DialTone", TRUE))
                {
                    pPrefixInfo->DialTone = uValue;
                    ret = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                    ret =  TRUE;
                }
            }

            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }   
    }

    return ret;
}

