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
#include "telcovoicemgr_dml_services_v1.h"
#include "telcovoicemgr_services_apis_v1.h"
#include "telcovoicemgr_dml_hal.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG CapabilitiesCodecs_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG CapabilitiesCodecs_GetEntryCount(ANSC_HANDLE hInsContext)
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
            count = pDmlVoiceService->CapabilitiesObj.CapabilitiesCodecList.ulQuantity;
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }

    return count;
}
/**********************************************************************
    caller:     owner of this object

    prototype:

        ANSC_HANDLE CapabilitiesCodecs_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE CapabilitiesCodecs_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
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
            if(nIndex < pDmlVoiceService->CapabilitiesObj.CapabilitiesCodecList.ulQuantity)
            {
                DML_CAPABILITIES_CODECS_CTRL_T* pDmlCapCodecList = pDmlVoiceService->CapabilitiesObj.CapabilitiesCodecList.pdata[nIndex];
                if(pDmlCapCodecList != NULL)
                {
                    *pInsNumber = nIndex + 1;
                    pDmlEntry = (ANSC_HANDLE) pDmlCapCodecList;
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

    BOOL Capabilities_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL Capabilities_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService == NULL)
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

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);
    if(pDmlVoiceService == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DML_CAPABILITIES pDmlCapabilities = &(pDmlVoiceService->CapabilitiesObj);

    if(pDmlCapabilities != NULL)
    {
        if( AnscEqualString(ParamName, "RTCP", TRUE))
        {
            *pBool = pDmlCapabilities->RTCP;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "RTPRedundancy", TRUE))
        {
            *pBool = pDmlCapabilities->RTPRedundancy;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "SRTP", TRUE))
        {
            *pBool = pDmlCapabilities->SRTP;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "DSCPCoupled", TRUE))
        {
            *pBool = pDmlCapabilities->DSCPCoupled;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "EthernetTaggingCoupled", TRUE))
        {
            *pBool = pDmlCapabilities->EthernetTaggingCoupled;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "PSTNSoftSwitchOver", TRUE))
        {
            *pBool = pDmlCapabilities->PSTNSoftSwitchOver;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "FaxT38", TRUE))
        {
            *pBool = pDmlCapabilities->FaxT38;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "FaxPassThrough", TRUE))
        {
            *pBool = pDmlCapabilities->FaxPassThrough;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "ModemPassThrough", TRUE))
        {
            *pBool = pDmlCapabilities->ModemPassThrough;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "ToneDescriptionsEditable", TRUE))
        {
            *pBool = pDmlCapabilities->ToneDescriptionsEditable;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "PatternBasedToneGeneration", TRUE))
        {
            *pBool = pDmlCapabilities->PatternBasedToneGeneration;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "FileBasedToneGeneration", TRUE))
        {
            *pBool = pDmlCapabilities->FileBasedToneGeneration;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "ToneGeneration", TRUE))
        {
            *pBool = pDmlCapabilities->ToneGeneration;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "RingDescriptionsEditable", TRUE))
        {
            *pBool = pDmlCapabilities->RingDescriptionsEditable;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "PatternBasedRingGeneration", TRUE))
        {
            *pBool = pDmlCapabilities->PatternBasedRingGeneration;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "RingPatternEditable", TRUE))
        {
            *pBool = pDmlCapabilities->RingPatternEditable;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "FileBasedRingGeneration", TRUE))
        {
            *pBool = pDmlCapabilities->FileBasedRingGeneration;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "RingGeneration", TRUE))
        {
            *pBool = pDmlCapabilities->RingGeneration;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "DigitMap", TRUE))
        {
            *pBool = pDmlCapabilities->DigitMap;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "NumberingPlan", TRUE))
        {
            *pBool = pDmlCapabilities->NumberingPlan;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "ButtonMap", TRUE))
        {
            *pBool = pDmlCapabilities->ButtonMap;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "VoicePortTests", TRUE))
        {
            *pBool = pDmlCapabilities->VoicePortTests;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Null object \n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL Capabilities_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL Capabilities_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService == NULL)
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

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);
    if(pDmlVoiceService == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DML_CAPABILITIES pDmlCapabilities = &(pDmlVoiceService->CapabilitiesObj);

    if(pDmlCapabilities != NULL)
    {
        if( AnscEqualString(ParamName, "MaxProfileCount", TRUE))
        {
            *pValue = pDmlCapabilities->MaxProfileCount;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "MaxLineCount", TRUE))
        {
            *pValue = pDmlCapabilities->MaxLineCount;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "MaxSessionsPerLine", TRUE))
        {
            *pValue = pDmlCapabilities->MaxSessionsPerLine;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "MaxSessionCount", TRUE))
        {
            *pValue = pDmlCapabilities->MaxSessionCount;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Null object \n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG Capabilities_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG Capabilities_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService == NULL)
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

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);
    if(pDmlVoiceService == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DML_CAPABILITIES pDmlCapabilities = &(pDmlVoiceService->CapabilitiesObj);

    if(pDmlCapabilities != NULL)
    {

        if( AnscEqualString(ParamName, "SignalingProtocols", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilities->SignalingProtocols);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "Regions", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilities->Regions);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "SRTPKeyingMethods", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilities->SRTPKeyingMethods);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "SRTPEncryptionKeySizes", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilities->SRTPEncryptionKeySizes);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "ToneFileFormats", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilities->ToneFileFormats);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "RingFileFormats", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilities->RingFileFormats);
            ret = 0;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            ret =  -1;
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Null object \n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL CapabilitiesSIP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL CapabilitiesSIP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    
    BOOL ret = FALSE;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService == NULL)
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

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);
    if(pDmlVoiceService == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DML_CAPABILITIES_SIP pDmlCapabilitiesSIP = &(pDmlVoiceService->CapabilitiesObj.CapabilitiesSIPObj);

    if(pDmlCapabilitiesSIP != NULL)
    {
        if( AnscEqualString(ParamName, "EventSubscription", TRUE))
        {
            *pBool = pDmlCapabilitiesSIP->EventSubscription;
            ret = TRUE;
        }
        else if( AnscEqualString(ParamName, "ResponseMap", TRUE))
        {
            *pBool = pDmlCapabilitiesSIP->ResponseMap;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Null object \n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG CapabilitiesSIP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG CapabilitiesSIP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    LONG ret = -1;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService == NULL)
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

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);
    if(pDmlVoiceService == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DML_CAPABILITIES_SIP pDmlCapabilitiesSIP = &(pDmlVoiceService->CapabilitiesObj.CapabilitiesSIPObj);

    if(pDmlCapabilitiesSIP != NULL)
    {
        if( AnscEqualString(ParamName, "Role", TRUE) )
        {
            if(pDmlCapabilitiesSIP->Role == SIP_ROLE_USER_AGENT)
            {
                AnscCopyString(pValue,"UserAgent");
                ret = 0;
            }
            else if(pDmlCapabilitiesSIP->Role == SIP_ROLE_B2B_USER_AGENT)
            {
                AnscCopyString(pValue,"BackToBackUserAgents");
                ret = 0;
            }
            else if(pDmlCapabilitiesSIP->Role == SIP_ROLE_OUTBOUND_PROXY)
            {
                AnscCopyString(pValue,"OutboundProxy");
                ret = 0;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
        }
        else if( AnscEqualString(ParamName, "Extensions", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesSIP->Extensions);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "Transports", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesSIP->Transports);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "URISchemes", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesSIP->URISchemes);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "TLSAuthenticationProtocols", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesSIP->TLSAuthenticationProtocols);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "TLSAuthenticationKeySizes", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesSIP->TLSAuthenticationKeySizes);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "TLSEncryptionKeySizes", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesSIP->TLSEncryptionKeySizes);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "TLSKeyExchangeProtocols", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesSIP->TLSKeyExchangeProtocols);
            ret = 0;
        }
        else if( AnscEqualString(ParamName, "TLSEncryptionProtocols", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesSIP->TLSEncryptionProtocols);
            ret = 0;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Null object \n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG CapabilitiesMGCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG CapabilitiesMGCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService == NULL)
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

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);
    if(pDmlVoiceService == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DML_CAPABILITIES_MGCP pDmlCapabilitiesMGCP = &(pDmlVoiceService->CapabilitiesObj.CapabilitiesMGCPObj);

    if(pDmlCapabilitiesMGCP != NULL)
    {

        if( AnscEqualString(ParamName, "Extensions", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesMGCP->Extensions);
            ret = 0;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Null object \n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL CapabilitiesH323_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL CapabilitiesH323_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService == NULL)
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

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);
    if(pDmlVoiceService == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DML_CAPABILITIES_H323 pDmlCapabilitiesH323 = &(pDmlVoiceService->CapabilitiesObj.CapabilitiesH323Obj);

    if(pDmlCapabilitiesH323 != NULL)
    {

        if( AnscEqualString(ParamName, "FastStart", TRUE))
        {
            *pBool = pDmlCapabilitiesH323->FastStart;
            ret = TRUE;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Null object \n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG CapabilitiesH323_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG CapabilitiesH323_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;
    if(pVoiceService == NULL)
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

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);
    if(pDmlVoiceService == NULL )
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        return ret;
    }

    PTELCOVOICEMGR_DML_CAPABILITIES_H323 pDmlCapabilitiesH323 = &(pDmlVoiceService->CapabilitiesObj.CapabilitiesH323Obj);

    if(pDmlCapabilitiesH323 != NULL)
    {
        if( AnscEqualString(ParamName, "H235AuthenticationMethods", TRUE) )
        {
            AnscCopyString(pValue,pDmlCapabilitiesH323->H235AuthenticationMethods);
            ret = 0;
        }
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Null object \n", __FUNCTION__, __LINE__));
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return ret;
}

/**********************************************************************

        caller:     owner of this object

        prototype:

    BOOL CapabilitiesCodecs_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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
BOOL CapabilitiesCodecs_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;
    DML_CAPABILITIES_CODECS_CTRL_T* pDmlCapCodecList = (DML_CAPABILITIES_CODECS_CTRL_T*) hInsContext;
    CcspTraceWarning(("%s:: ParamName :%s\n", __FUNCTION__, ParamName));
    if(pDmlCapCodecList != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            PTELCOVOICEMGR_DML_CAPABILITIES_CODECS pDmlCapCodecs = &(pDmlCapCodecList->dml);

            if( AnscEqualString(ParamName, "SilenceSuppression", TRUE))
            {
                *pBool = pDmlCapCodecs->SilenceSuppression;
                ret = TRUE;
            }
            else
            {
                ret =  TRUE;
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
        else
        {
            CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Invalid object \n", __FUNCTION__, __LINE__));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG CapabilitiesCodecs_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize);

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

LONG CapabilitiesCodecs_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pUlSize)
{
    ULONG ret = 1;
    DML_CAPABILITIES_CODECS_CTRL_T* pDmlCapCodecList = (DML_CAPABILITIES_CODECS_CTRL_T*) hInsContext;
    if(pDmlCapCodecList != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            PTELCOVOICEMGR_DML_CAPABILITIES_CODECS pDmlCapCodecs = &(pDmlCapCodecList->dml);
            if( AnscEqualString(ParamName, "Alias", TRUE) )
            {
                AnscCopyString(pValue, pDmlCapCodecs->Alias);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "Codec", TRUE) )
            {
                AnscCopyString(pValue, pDmlCapCodecs->Codec);
                ret = 0;
            }
            else if( AnscEqualString(ParamName, "PacketizationPeriod", TRUE) )
            {
                AnscCopyString(pValue, pDmlCapCodecs->PacketizationPeriod);
                ret = 0;
            }
            else
            {
                CcspTraceWarning(("%s::Unknown ParamName :%s\n", __FUNCTION__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
        else
        {
            CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Invalid object \n", __FUNCTION__, __LINE__));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL CapabilitiesCodecs_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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
BOOL CapabilitiesCodecs_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    DML_CAPABILITIES_CODECS_CTRL_T* pDmlCapCodecList = (DML_CAPABILITIES_CODECS_CTRL_T*) hInsContext;
    if(pDmlCapCodecList != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            PTELCOVOICEMGR_DML_CAPABILITIES_CODECS pDmlCapCodecs = &(pDmlCapCodecList->dml);
            if( AnscEqualString(ParamName, "Alias", TRUE) )
            {
                AnscCopyString(pDmlCapCodecs->Alias,pString);
                CcspTraceDebug(("%s:%d:: Copy Alias Done\n", __FUNCTION__, __LINE__));
                ret = TRUE;
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
        else
        {
            CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Invalid object \n", __FUNCTION__, __LINE__));
    }
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:
        BOOL CapabilitiesCodecs_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue);

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
BOOL CapabilitiesCodecs_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* pValue)
{
    BOOL ret = FALSE;

    DML_CAPABILITIES_CODECS_CTRL_T* pDmlCapCodecList = (DML_CAPABILITIES_CODECS_CTRL_T*) hInsContext;
    if(pDmlCapCodecList != NULL)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlData != NULL)
        {
            PTELCOVOICEMGR_DML_CAPABILITIES_CODECS pDmlCapCodecs = &(pDmlCapCodecList->dml);
            if( AnscEqualString(ParamName, "EntryID", TRUE))
            {
                *pValue=pDmlCapCodecs->EntryID;
                ret = TRUE;
            }
            else if( AnscEqualString(ParamName, "BitRate", TRUE))
            {
                *pValue = pDmlCapCodecs->BitRate;
                ret = TRUE;
            }
            else
            {
                CcspTraceWarning(("%s:%d::Unknown ParamName :%s\n", __FUNCTION__, __LINE__, ParamName));
            }
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
        else
        {
            CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        }
    }
    else
    {
        CcspTraceError(("%s:%d:: Invalid object \n", __FUNCTION__, __LINE__));
    }
    return ret;
}
