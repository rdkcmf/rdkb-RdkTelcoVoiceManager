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

#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_report.h"
#include "voice_report.h"


#define EMPTY_BUFFER "Schema Buffer is empty"

char * VoiceServiceReportStatusEnable              = "eRT.com.cisco.spvtg.ccsp.voipreport.Enabled";
char * VoiceServiceReportStatusDfltReportingPeriod = "eRT.com.cisco.spvtg.ccsp.voipreport.Default.ReportingPeriod";
char * VoiceServiceReportStatusReportingPeriod     = "eRT.com.cisco.spvtg.ccsp.voipreport.ReportingPeriod";

extern ANSC_HANDLE                   bus_handle;
extern char                          g_Subsystem[32];


/***********************************************************************

 APIs for Object:

    Device.Services.X_RDK_Report.Voice.

    *  X_RDK_Report_VoiceService_GetParamUlongValue
    *  X_RDK_Report_VoiceService_SetParamUlongValue
    *  X_RDK_Report_VoiceService_GetParamStringValue
    *  X_RDK_Report_VoiceService_GetParamBoolValue
    *  X_RDK_Report_VoiceService_SetParamBoolValue

***********************************************************************/


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_RDK_Report_VoiceService_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

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
BOOL
X_RDK_Report_VoiceService_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;

    /* check the parameter name and set the corresponding value */
    if(AnscEqualString(ParamName, "ReportingPeriod", TRUE) && (puLong != NULL))
    {
        *puLong = pVoiceServiceReport->ReportingPeriod;
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_RDK_Report_VoiceService_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

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
BOOL
X_RDK_Report_VoiceService_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;

    /* check the parameter name and set the corresponding value */
    if(AnscEqualString(ParamName, "ReportingPeriod", TRUE))
    {
        pVoiceServiceReport->bReportingPeriodChanged = TRUE;
        pVoiceServiceReport->ReportingPeriod = uValue;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        X_RDK_Report_VoiceService_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

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
ULONG
X_RDK_Report_VoiceService_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;

    /* check the parameter name and set the corresponding value */
    if(AnscEqualString(ParamName, "Schema", TRUE))
    {
        /* collect value */
        int bufsize = VoiceServiceReportGetSchemaBufferSize();
        if(!bufsize)
        {
            AnscCopyString(pValue, EMPTY_BUFFER);
            return FALSE;
        }
        else
        {
            if (bufsize < *pUlSize)
            {
                AnscCopyString(pValue, VoiceServiceReportGetSchemaBuffer());
                return FALSE;
            }
            else
            {
                *pUlSize = bufsize + 1;
                return TRUE;
            }
        }
    }

    /* check the parameter name and set the corresponding value */
    if(AnscEqualString(ParamName, "SchemaID", TRUE))
    {
        /* collect value */
        int bufsize = VoiceServiceReportGetSchemaIDBufferSize();
        if(!bufsize)
        {
            char result[1024] = "Schema Buffer is empty";
            AnscCopyString(pValue, (char*)&result);
            return FALSE;
        }
        else
        {
            if (bufsize < *pUlSize)
            {
                AnscCopyString(pValue, VoiceServiceReportGetSchemaIDBuffer());
                return FALSE;
            }
            else
            {
                *pUlSize = bufsize + 1;
                return TRUE;
            }
        }
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_RDK_Report_VoiceService_GetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL*                       pBool
            );

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
BOOL
X_RDK_Report_VoiceService_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;

    if(AnscEqualString(ParamName, "Enabled", TRUE) && (pBool != NULL))
    {
        *pBool = pVoiceServiceReport->Enabled;
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_RDK_Report_VoiceService_SetParamBoolValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                BOOL                        bValue
            );

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
BOOL
X_RDK_Report_VoiceService_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;

    if(AnscEqualString(ParamName, "Enabled", TRUE))
    {
        pVoiceServiceReport->Enabled = bValue;
        pVoiceServiceReport->bEnableChanged = TRUE;
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_RDK_Report_VoiceService_Validate
        (
            ANSC_HANDLE                 hInsContext,
            char*                       pReturnParamName,
            ULONG*                      puLength
        )
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

BOOL
X_RDK_Report_VoiceService_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;

    if (pVoiceServiceReport->bReportingPeriodChanged)
    {
        BOOL validated = VoiceServiceReportValidateReportingPeriod(pVoiceServiceReport->ReportingPeriod);
        if(!validated)
        {
            AnscCopyString(pReturnParamName, "ReportingPeriod");
            *puLength = AnscSizeOfString("ReportingPeriod");
            return FALSE;
        }
    }

    return TRUE;
}


/**********************************************************************
    caller:     owner of this object

    prototype:
        ULONG
        X_RDK_Report_VoiceService_Commit
            (
                ANSC_HANDLE                 hInsContext
            );

    description:
        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.
**********************************************************************/
ULONG
X_RDK_Report_VoiceService_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;
    ULONG psmValue = 0;

    if (pVoiceServiceReport->bEnableChanged)
    {
        VoiceServiceReportSetStatus(pVoiceServiceReport->Enabled);
        pVoiceServiceReport->bEnableChanged = false;
        psmValue = pVoiceServiceReport->Enabled;
        SetNVRamULONGConfiguration (VoiceServiceReportStatusEnable, psmValue);
    }

    if (pVoiceServiceReport->bReportingPeriodChanged)
    {
        VoiceServiceReportSetReportingPeriod(pVoiceServiceReport->ReportingPeriod);
        pVoiceServiceReport->bReportingPeriodChanged = false;
        psmValue = pVoiceServiceReport->ReportingPeriod;
        SetNVRamULONGConfiguration (VoiceServiceReportStatusReportingPeriod, psmValue);
    }
    return 0;
}


/**********************************************************************
    caller:     owner of this object

    prototype:
        ULONG
        X_RDK_Report_VoiceService_Rollback
            (
                ANSC_HANDLE                 hInsContext
            );

    description:
        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.
**********************************************************************/

ULONG
X_RDK_Report_VoiceService_Rollback
(
    ANSC_HANDLE                 hInsContext
)
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;

    if (pVoiceServiceReport->bEnableChanged)
    {
        pVoiceServiceReport->Enabled = VoiceServiceReportGetStatus();
        pVoiceServiceReport->bEnableChanged = false;
    }

    if (pVoiceServiceReport->bReportingPeriodChanged)
    {
        pVoiceServiceReport->ReportingPeriod = VoiceServiceReportGetReportingPeriod();
        pVoiceServiceReport->bReportingPeriodChanged = false;
    }

    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_RDK_Report_VoiceService_Default_GetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG*                      puLong
            );

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
BOOL
X_RDK_Report_VoiceService_Default_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    )
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;
    PDML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT    pVoiceServiceReportDflt  = (PDML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT)pVoiceServiceReport->pVoiceServiceDefaultReport;

    /* check the parameter name and set the corresponding value */
    if(AnscEqualString(ParamName, "OverrideTTL", TRUE) && (puLong != NULL))
    {
        pVoiceServiceReportDflt->OverrideTTL = VoiceServiceReportGetDefaultOverrideTTL();
        *puLong = pVoiceServiceReportDflt->OverrideTTL;
        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if(AnscEqualString(ParamName, "ReportingPeriod", TRUE) && (puLong != NULL))
    {
        *puLong = pVoiceServiceReportDflt->ReportingPeriod;
        return TRUE;
    }

    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
    X_RDK_Report_VoiceService_Default_SetParamUlongValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                ULONG                       uValue
            );

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
BOOL
X_RDK_Report_VoiceService_Default_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    )
{
    PTELCOVOICE_CONTEXT_LINK_OBJECT         pMyObject             = (PTELCOVOICE_CONTEXT_LINK_OBJECT)g_pTelcoVoiceBEManager->hServices;
    PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport   = (PDML_X_RDK_REPORT_VOICE_SERVICE)pMyObject->pVoiceServiceReport;
    PDML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT    pVoiceServiceReportDflt  = (PDML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT)pVoiceServiceReport->pVoiceServiceDefaultReport;

    /* check the parameter name and set the corresponding value */
    if(AnscEqualString(ParamName, "OverrideTTL", TRUE))
    {
        pVoiceServiceReportDflt->OverrideTTL = uValue;
        VoiceServiceReportSetDefaultOverrideTTL (uValue);
        return TRUE;
    }

    /* check the parameter name and set the corresponding value */
    if(AnscEqualString(ParamName, "ReportingPeriod", TRUE))
    {
        if(!VoiceServiceReportValidateReportingPeriod(uValue))
        {
            return FALSE;
        }
        ULONG psmValue = 0;
        pVoiceServiceReportDflt->ReportingPeriod = uValue;
        psmValue = pVoiceServiceReportDflt->ReportingPeriod;
        VoiceServiceReportSetDefaultReportingPeriod (uValue);
        SetNVRamULONGConfiguration (VoiceServiceReportStatusDfltReportingPeriod, psmValue);
        return TRUE;
    }

    return FALSE;
}

ANSC_STATUS GetNVRamULONGConfiguration(char* setting, ULONG* value)
{
    char *strValue = NULL;
    int retPsmGet = 0;

    retPsmGet = PSM_Get_Record_Value2(bus_handle,g_Subsystem, setting, NULL, &strValue);
    if (retPsmGet == CCSP_SUCCESS)
    {
        *value = _ansc_atoi(strValue);
        ((CCSP_MESSAGE_BUS_INFO *)bus_handle)->freefunc(strValue);
    }
    return retPsmGet;
}

ANSC_STATUS SetNVRamULONGConfiguration (char * setting, ULONG value)
{
    int retPsmSet = 0;
    char psmValue[32] = {0};
    ULONG psm_value = 0;

    retPsmSet = GetNVRamULONGConfiguration(setting,&psm_value);
    if ((retPsmSet == CCSP_SUCCESS) && (psm_value == value))
    {
        return retPsmSet;
    }

    snprintf(psmValue,sizeof(psmValue),"%d",value);
    retPsmSet = PSM_Set_Record_Value2(bus_handle,g_Subsystem, setting, ccsp_string, psmValue);
    return retPsmSet;
}
