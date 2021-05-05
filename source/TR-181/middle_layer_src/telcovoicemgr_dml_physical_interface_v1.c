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
/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG PhyInterface_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG PhyInterface_GetEntryCount(ANSC_HANDLE hInsContext)
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
            count =  pDmlVoiceService->PhyInterfaceList.ulQuantity;

            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
    return count;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE PhyInterface_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE PhyInterface_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE pDmlEntry = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE    pDmlVoiceService    = NULL;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            pDmlVoiceService = &(pVoiceService->dml);
            if(nIndex < pDmlVoiceService->PhyInterfaceList.ulQuantity)
            {   
                DML_PHYINTERFACE_CTRL_T* pPhyInterface = pDmlVoiceService->PhyInterfaceList.pdata[nIndex];
    
                if(pPhyInterface != NULL)
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pPhyInterface;
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

        LONG PhyInterface_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG PhyInterface_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_PHYINTERFACE_CTRL_T* pPhyInterface = (DML_PHYINTERFACE_CTRL_T *) hInsContext;
    if(pPhyInterface == NULL)
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

    PTELCOVOICEMGR_DML_PHYINTERFACE pDmlPhyInterface = &(pPhyInterface->dml);

    if(pDmlPhyInterface == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlPhyInterface: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    if( AnscEqualString(ParamName, "Alias", TRUE) )
    {
        AnscCopyString(pValue,pDmlPhyInterface->Alias);
        ret = 0;
    }
    else if( AnscEqualString(ParamName, "PhyPort", TRUE) )
    {
        AnscCopyString(pValue,pDmlPhyInterface->PhyPort);
        ret = 0;
    } 
    else if( AnscEqualString(ParamName, "Description", TRUE) )
    {
        AnscCopyString(pValue,pDmlPhyInterface->Description);
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
        BOOL PhyInterface_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL PhyInterface_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;
    DML_PHYINTERFACE_CTRL_T* pPhyInterface = (DML_PHYINTERFACE_CTRL_T *) hInsContext;
    if(pPhyInterface == NULL)
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

    PTELCOVOICEMGR_DML_PHYINTERFACE pDmlPhyInterface = &(pPhyInterface->dml);

    if(pDmlPhyInterface == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlPhyInterface: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    if( AnscEqualString(ParamName, "InterfaceID", TRUE))
    {
        *pValue = pDmlPhyInterface->InterfaceID;
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

    BOOL PhyInterfaceTests_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL PhyInterfaceTests_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_PHYINTERFACE_CTRL_T* pPhyInterface = (DML_PHYINTERFACE_CTRL_T *) hInsContext;
    if(pPhyInterface == NULL)
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

    PTELCOVOICEMGR_DML_PHYINTERFACE pDmlPhyInterface = &(pPhyInterface->dml);
    if(pDmlPhyInterface == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlPhyInterface: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_PHYINTERFACE_TESTS pDmlPhyInterfaceTests = &(pDmlPhyInterface->PhyInterfaceTestsObj);
    if(pDmlPhyInterfaceTests != NULL)
    {
        if( AnscEqualString(ParamName, "PhoneConnectivity", TRUE))
        {
            *pBool = pDmlPhyInterfaceTests->PhoneConnectivity;
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

        BOOL PhyInterfaceTests_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL PhyInterfaceTests_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL  ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uPhyIndex = 0;
    ULONG tmpBuf = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    DML_PHYINTERFACE_CTRL_T* pPhyInterface = (DML_PHYINTERFACE_CTRL_T *) hInsContext;
    if(pPhyInterface == NULL || pString == NULL)
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

    PTELCOVOICEMGR_DML_PHYINTERFACE pDmlPhyInterface = &(pPhyInterface->dml);
    if(pDmlPhyInterface == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlPhyInterface: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DML_PHYINTERFACE_TESTS pDmlPhyInterfaceTests = &(pDmlPhyInterface->PhyInterfaceTestsObj);
    if(pDmlPhyInterfaceTests != NULL)
    {
        pDmlVoiceService = pDmlPhyInterface->pParentVoiceService;
        if(pDmlVoiceService != NULL)
        {
            uVsIndex = pDmlVoiceService->InstanceNumber;
            uPhyIndex =  pDmlPhyInterface->InstanceNumber;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
            if( AnscEqualString(ParamName, "TestSelector", TRUE) )
            {
                if(!strcmp(pString, "PhoneConnectivityTest"))
                {
                    tmpBuf= PHYINTERFACE_TESTSELECTOR_PHONE_CONNECTIVITY_TEST;
                    ret = TRUE;
                }
                else if(!strcmp(pString, "Hazard Potential"))
                {
                    tmpBuf = PHYINTERFACE_TESTSELECTOR_HAZARD_POTENTIAL;
                    ret = TRUE;
                }
                else if(!strcmp(pString, "Foreign Voltage"))
                {
                    tmpBuf = PHYINTERFACE_TESTSELECTOR_FOREIGN_VOLTAGE;
                    ret = TRUE;
                }
                else if(!strcmp(pString, "Resistive Faults"))
                {
                    tmpBuf = PHYINTERFACE_TESTSELECTOR_RESISTIVE_FAULTS;
                    ret = TRUE;
                }
                else if(!strcmp(pString, "Off-hook"))
                {
                    tmpBuf = PHYINTERFACE_TESTSELECTOR_OFF_HOOK;
                    ret = TRUE;
                }
                else if(!strcmp(pString, "REN"))
                {
                    tmpBuf = PHYINTERFACE_TESTSELECTOR_REN;
                    ret = TRUE;
                }
                else
                {
                    CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
                    return ret;
                }        
                if(TelcoVoiceMgrDmlSetTestSelector(uVsIndex, uPhyIndex, tmpBuf) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        pDmlPhyInterfaceTests->TestSelector = tmpBuf;
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
    else
    {
        CcspTraceError(("%s:%d:: pDmlPhyInterfaceTests: NULL\n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL PhyInterfaceTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL PhyInterfaceTests_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex = 0;
    ULONG uPhyIndex = 0;
    ULONG tmpBuf = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    DML_PHYINTERFACE_CTRL_T* pPhyInterface = (DML_PHYINTERFACE_CTRL_T *) hInsContext;
    if(pPhyInterface == NULL)
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

    PTELCOVOICEMGR_DML_PHYINTERFACE pDmlPhyInterface = &(pPhyInterface->dml);
    if(pDmlPhyInterface == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlPhyInterface: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_PHYINTERFACE_TESTS pDmlPhyInterfaceTests = &(pDmlPhyInterface->PhyInterfaceTestsObj);
    if(pDmlPhyInterfaceTests != NULL)
    {
        pDmlVoiceService = pDmlPhyInterface->pParentVoiceService;
        if(pDmlVoiceService != NULL)
        {
            uVsIndex = pDmlVoiceService->InstanceNumber;
            uPhyIndex =  pDmlPhyInterface->InstanceNumber;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);

            if( AnscEqualString(ParamName, "TestState", TRUE) )
            {
                if(TelcoVoiceMgrDmlSetTestState(uVsIndex, uPhyIndex, uValue) == ANSC_STATUS_SUCCESS)
                {
                    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
                    if(pTelcoVoiceMgrDmlData != NULL)
                    {
                        pDmlPhyInterfaceTests->TestState = uValue;
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
    else
    {
        CcspTraceError(("%s:%d:: pDmlPhyInterfaceTests: NULL\n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL PhyInterface_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL PhyInterface_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    DML_PHYINTERFACE_CTRL_T* pPhyInterface = (DML_PHYINTERFACE_CTRL_T *) hInsContext;
    if(pPhyInterface == NULL)
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

    PTELCOVOICEMGR_DML_PHYINTERFACE pDmlPhyInterface = &(pPhyInterface->dml);
    if(pDmlPhyInterface == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlPhyInterface: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_PHYINTERFACE_TESTS pDmlPhyInterfaceTests = &(pDmlPhyInterface->PhyInterfaceTestsObj);
    if(pDmlPhyInterfaceTests != NULL)
    {

        if( AnscEqualString(ParamName, "Alias", TRUE) )
        {
            //TODO
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

        LONG PhyInterfaceTests_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG PhyInterfaceTests_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)

{
    ULONG ret = 1;
    DML_PHYINTERFACE_CTRL_T* pPhyInterface = (DML_PHYINTERFACE_CTRL_T *) hInsContext;
    if(pPhyInterface == NULL)
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

    PTELCOVOICEMGR_DML_PHYINTERFACE pDmlPhyInterface = &(pPhyInterface->dml);
    if(pDmlPhyInterface == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlPhyInterface: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_PHYINTERFACE_TESTS pDmlPhyInterfaceTests = &(pDmlPhyInterface->PhyInterfaceTestsObj);
    if(pDmlPhyInterfaceTests != NULL)
    {
        if( AnscEqualString(ParamName, "TestSelector", TRUE) )
        {
                 
            if(pDmlPhyInterfaceTests->TestSelector == PHYINTERFACE_TESTSELECTOR_PHONE_CONNECTIVITY_TEST)
            {
                AnscCopyString(pValue,"PhoneConnectivityTest");
                ret = 0;
            }
            else if(pDmlPhyInterfaceTests->TestSelector == PHYINTERFACE_TESTSELECTOR_HAZARD_POTENTIAL)
            {
                AnscCopyString(pValue,"Hazard Potential");
                ret = 0;
            }
            else if(pDmlPhyInterfaceTests->TestSelector == PHYINTERFACE_TESTSELECTOR_FOREIGN_VOLTAGE)
            {
                AnscCopyString(pValue,"Foreign Voltage");
                ret = 0;
            }   
            else if(pDmlPhyInterfaceTests->TestSelector == PHYINTERFACE_TESTSELECTOR_RESISTIVE_FAULTS)
            {
                AnscCopyString(pValue,"Resistive Faults");
                ret = 0;
            }   
            else if(pDmlPhyInterfaceTests->TestSelector == PHYINTERFACE_TESTSELECTOR_OFF_HOOK)
            {
                AnscCopyString(pValue,"Off-hook");
                ret = 0;
            }   
            else if(pDmlPhyInterfaceTests->TestSelector == PHYINTERFACE_TESTSELECTOR_REN)
            {
                AnscCopyString(pValue,"REN");
                ret = 0;
            }   
        }
        else if( AnscEqualString(ParamName, "X_RDK_TestResult", TRUE) )
        {
            AnscCopyString(pValue,pDmlPhyInterfaceTests->X_RDK_TestResult);
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

        LONG PhyInterfaceTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

BOOL PhyInterfaceTests_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;
    DML_PHYINTERFACE_CTRL_T* pPhyInterface = (DML_PHYINTERFACE_CTRL_T *) hInsContext;
    if(pPhyInterface == NULL)
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

    PTELCOVOICEMGR_DML_PHYINTERFACE pDmlPhyInterface = &(pPhyInterface->dml);
    if(pDmlPhyInterface == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlPhyInterface: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }
    
    PTELCOVOICEMGR_DML_PHYINTERFACE_TESTS pDmlPhyInterfaceTests = &(pDmlPhyInterface->PhyInterfaceTestsObj);
    if(pDmlPhyInterfaceTests != NULL)
    {

        if( AnscEqualString(ParamName, "TestState", TRUE) )
        {
            *pValue = pDmlPhyInterfaceTests->TestState;
            ret = TRUE;
        }
            else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }  
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG PhyInterface_IsUpdated(ANSC_HANDLE hInsContext);

    description:

               This function is called to check if VoiceService updated .

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
    

BOOL PhyInterface_IsUpdated(ANSC_HANDLE hInsContext)
{
    BOOL        bIsUpdated = TRUE;
    return bIsUpdated;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

                ULONG PhyInterface_Synchronize(ANSC_HANDLE hInsContext);

    description:

               This function is called to synchronize VoiceService data.

    argument:   ANSC_HANDLE      hInsContext, 
               The instance handle;

    return:     The status of the operation.

**********************************************************************/
ULONG PhyInterface_Synchronize(ANSC_HANDLE hInsContext)
{
    ANSC_STATUS            returnStatus   = ANSC_STATUS_SUCCESS;
    return returnStatus;
}
