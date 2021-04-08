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
/*****************************************************************************
* STANDARD INCLUDE FILES
*****************************************************************************/
#include <json-c/json.h>
#include <limits.h>
/*****************************************************************************
* PROJECT-SPECIFIC INCLUDE FILES
*****************************************************************************/
#include "rpc-cli/rpc_client.h"
#include "json_hal_client.h"
#include "ansc_platform.h"
#include "ccsp_trace.h"

#include "telcovoicemgr_dml_hal_param_v1.h"

#define FIRST_INDEX  1
#define SECOND_INDEX 2
#define THIRD_INDEX  3

ANSC_STATUS telcovoicemgr_hal_get_first_index(char* ParamName, char* Pattern, int* hal_index)
{
    if(sscanf(ParamName, Pattern, hal_index) != FIRST_INDEX)
    {
        AnscTraceError(("\n%s:%d:: \nInvalid index ParamName[%s]\nPattern[%s]\n", __FUNCTION__, __LINE__, ParamName,Pattern));
        return ANSC_STATUS_FAILURE;
    }

    if( hal_index <= 0 )
    {
        AnscTraceError(("\n%s:%d:: \nInvalid index ParamName[%s]\nPattern[%s]\n", __FUNCTION__, __LINE__, ParamName,Pattern));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS telcovoicemgr_hal_get_second_index(char* ParamName, char* Pattern,  int* hal_index)
{
    int tmp_index1;

    if(sscanf(ParamName, Pattern, &tmp_index1, hal_index) != SECOND_INDEX)
    {
        AnscTraceError(("\n%s:%d:: \nInvalid index ParamName[%s]\nPattern[%s]\n", __FUNCTION__, __LINE__, ParamName,Pattern));
        return ANSC_STATUS_FAILURE;
    }

    if( *hal_index <= 0 || tmp_index1 <= 0 )
    {
        AnscTraceError(("\n%s:%d:: \nInvalid index ParamName[%s]\nPattern[%s]\n", __FUNCTION__, __LINE__, ParamName,Pattern));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS telcovoicemgr_hal_get_third_index(char* ParamName, char* Pattern, int* hal_index)
{
    int tmp_index1, tmp_index2;

    if(sscanf(ParamName, Pattern, &tmp_index1, &tmp_index2, hal_index) != THIRD_INDEX)
    {
        AnscTraceError(("\n%s:%d:: \nInvalid index ParamName[%s]\nPattern[%s]\n", __FUNCTION__, __LINE__, ParamName,Pattern));
        return ANSC_STATUS_FAILURE;
    }

    if( *hal_index <= 0 || tmp_index1 <= 0 || tmp_index2 <= 0 )
    {
        AnscTraceError(("\n%s:%d:: \nInvalid index ParamName[%s]\nPattern[%s]\n", __FUNCTION__, __LINE__, ParamName,Pattern));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceService(DML_VOICE_SERVICE_LIST_T* pVoiceServiceList, char* ParamName, char* pValue)
{
    ANSC_STATUS retStatus = ANSC_STATUS_FAILURE;
    int voiceservice_index = 0;
    int voiceProfile_index = 0;
    int PhyInterface_index = 0;
    int hal_index = 0;
    char *err;

    if(ParamName == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    //VoiceService.{i}.
    if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceService_index(ParamName, DML_VOICESERVICE, &hal_index))
    {
        return ANSC_STATUS_FAILURE;
    }

    if( hal_index <= 0 )
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    // voiceService index
    voiceservice_index = hal_index - 1;

    DML_VOICE_SERVICE_CTRL_T* pVoiceServiceData = pVoiceServiceList->pdata[voiceservice_index];

    if(pVoiceServiceData == NULL)
    {
        //create new Voice Service
        if( TelcoVoiceMgrDmlAddVoiceService(pVoiceServiceList, voiceservice_index) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pVoiceServiceData = pVoiceServiceList->pdata[voiceservice_index];
        if(pVoiceServiceData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = &(pVoiceServiceData->dml);

    pVoiceService->InstanceNumber = hal_index;
    if( strstr(ParamName, "X_RDK_DisableLoopCurrentUntilRegistered"))
    {
       if(strstr(pValue, "true") || strstr(pValue, "1"))
       {
           pVoiceService->X_RDK_DisableLoopCurrentUntilRegistered = true;
       }
       else
       {
           pVoiceService->X_RDK_DisableLoopCurrentUntilRegistered = false;
       }
    }
    else if( strstr(ParamName, "X_RDK_Enable"))
    {
       /*Enable value not stored in hal, handled in manager*/
    }
    else if( strstr(ParamName, "X_RDK_FactoryReset"))
    {
       /*Factory Reset handled in manager*/
    }
    else if( strstr(ParamName, "X_RDK_BoundIfName"))
    {
       strncpy(pVoiceService->X_RDK_BoundIfName, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "X_RDK_IpAddressFamily"))
    {
       strncpy(pVoiceService->X_RDK_IpAddressFamily, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "X_RDK_BoundIpAddr"))
    {
       strncpy(pVoiceService->X_RDK_BoundIpAddr, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "X_RDK_Status"))
    {
       if(strstr(pValue, "Stopped"))
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_STOPPED;
       }
       else if(pValue, "Starting")
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_STARTING;
       }
       else if(pValue, "Started")
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_STARTED;
       }
       else if(pValue, "Stopping")
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_STOPPING;
       }
       else if(pValue, "Error")
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_ERROR;
       }
    }
    else if( strstr(ParamName, "X_RDK_Debug"))
    {
        if( strstr(ParamName, "LogServerPort"))
        {
           pVoiceService->X_RDK_DebugObj.LogServerPort = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "LogServer"))
        {
           strncpy(pVoiceService->X_RDK_DebugObj.LogServer, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CCTKTraceGroup"))
        {
           strncpy(pVoiceService->X_RDK_DebugObj.CCTKTraceGroup, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CCTKTraceLevel"))
        {
           strncpy(pVoiceService->X_RDK_DebugObj.CCTKTraceLevel, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ModuleLogLevels"))
        {
           strncpy(pVoiceService->X_RDK_DebugObj.ModuleLogLevels, pValue,strlen(pValue)+1);
        }
    }
    else if(strstr(ParamName,"VoiceProfile"))
    {
        //VoiceService.{i}.VoiceProfile{i}.
        retStatus = Map_hal_dml_voiceProfile(&(pVoiceService->VoiceProfileList), ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Mapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"PhyInterface"))
    {
        //VoiceService.{i}.PhyInterface{i}.
        retStatus = Map_hal_dml_phyInterface(&(pVoiceService->PhyInterfaceList), ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Mapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"Capabilities"))
    {
        //VoiceService.{i}.Capabilities.
        retStatus = Map_hal_dml_capabilities(&(pVoiceService->CapabilitiesObj), ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        /* code */
        fprintf(stderr, "\nUnknown ParamName[%s]\nParamValue[%s]",ParamName,pValue);
    }

    pVoiceServiceData->updated = true;
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS Map_hal_dml_capabilities(PTELCOVOICEMGR_DML_CAPABILITIES pCapabilities, char* ParamName, char* pValue)
{
    ANSC_STATUS retStatus = ANSC_STATUS_SUCCESS;
    char *err;
    int codec_index = 0;
    int hal_index = 0;

    if( strstr(ParamName, "MaxProfileCount"))
    {
       pCapabilities->MaxProfileCount = strtoul(pValue,&err,10);
    }
    else if( strstr(ParamName, "MaxLineCount"))
    {
       pCapabilities->MaxLineCount = strtoul(pValue,&err,10);
    }
    else if( strstr(ParamName, "MaxSessionsPerLine"))
    {
       pCapabilities->MaxSessionsPerLine = strtoul(pValue,&err,10);
    }
    else if( strstr(ParamName, "MaxSessionCount"))
    {
       pCapabilities->MaxSessionCount = strtoul(pValue,&err,10);
    }
    else if( strstr(ParamName, "SignalingProtocols"))
    {
       strncpy(pCapabilities->SignalingProtocols, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Regions"))
    {
       strncpy(pCapabilities->Regions, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "RTCP"))
    {
       if(strstr(pValue, "true") || strstr(pValue, "1"))
       {
           pCapabilities->RTCP = true;
       }
       else
       {
           pCapabilities->RTCP = false;
       }
    }
    else if( strstr(ParamName, "SRTPKeyingMethods"))
    {
       strncpy(pCapabilities->SRTPKeyingMethods, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "SRTPEncryptionKeySizes"))
    {
       strncpy(pCapabilities->SRTPEncryptionKeySizes, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "SRTP"))
    {
       if(strstr(pValue, "true") || strstr(pValue, "1"))
       {
           pCapabilities->SRTP = true;
       }
       else
       {
           pCapabilities->SRTP = false;
       }
    }
    else if( strstr(ParamName, "RTPRedundancy"))
    {
       if(strstr(pValue, "true") || strstr(pValue, "1"))
       {
           pCapabilities->RTPRedundancy = true;
       }
       else
       {
           pCapabilities->RTPRedundancy = false;
       }
    }
    else if( strstr(ParamName, "DSCPCoupled"))
    {
       if(strstr(pValue, "true") || strstr(pValue, "1"))
       {
           pCapabilities->DSCPCoupled = true;
       }
       else
       {
           pCapabilities->DSCPCoupled = false;
       }
    }
    else if( strstr(ParamName, "EthernetTaggingCoupled"))
    {
       if(strstr(pValue, "true") || strstr(pValue, "1"))
       {
           pCapabilities->EthernetTaggingCoupled = true;
       }
       else
       {
           pCapabilities->EthernetTaggingCoupled = false;
       }
    }
    else if( strstr(ParamName, "PSTNSoftSwitchOver"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->PSTNSoftSwitchOver = true;
       }
       else
       {
           pCapabilities->PSTNSoftSwitchOver = false;
       }
    }
    else if( strstr(ParamName, "FaxT38"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->FaxT38 = true;
       }
       else
       {
           pCapabilities->FaxT38 = false;
       }
    }
    else if( strstr(ParamName, "FaxPassThrough"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->FaxPassThrough = true;
       }
       else
       {
           pCapabilities->FaxPassThrough = false;
       }
    }
    else if( strstr(ParamName, "ModemPassThrough"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->ModemPassThrough = true;
       }
       else
       {
           pCapabilities->ModemPassThrough = false;
       }
    }
    else if( strstr(ParamName, "ToneDescriptionsEditable"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->ToneDescriptionsEditable = true;
       }
       else
       {
           pCapabilities->ToneDescriptionsEditable = false;
       }

    }
    else if( strstr(ParamName, "PatternBasedToneGeneration"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->PatternBasedToneGeneration = true;
       }
       else
       {
           pCapabilities->PatternBasedToneGeneration = false;
       }

    }
    else if( strstr(ParamName, "FileBasedToneGeneration"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->FileBasedToneGeneration = true;
       }
       else
       {
           pCapabilities->FileBasedToneGeneration = false;
       }

    }
    else if( strstr(ParamName, "ToneGeneration"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->ToneGeneration = true;
       }
       else
       {
           pCapabilities->ToneGeneration = false;
       }
    }
    else if( strstr(ParamName, "ToneFileFormats"))
    {

       strncpy(pCapabilities->ToneFileFormats, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "RingDescriptionsEditable"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->RingDescriptionsEditable = true;
       }
       else
       {
           pCapabilities->RingDescriptionsEditable = false;
       }

    }
    else if( strstr(ParamName, "PatternBasedRingGeneration"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->PatternBasedRingGeneration = true;
       }
       else
       {
           pCapabilities->PatternBasedRingGeneration = false;
       }

    }
    else if( strstr(ParamName, "RingPatternEditable"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->RingPatternEditable = true;
       }
       else
       {
           pCapabilities->RingPatternEditable = false;
       }

    }
    else if( strstr(ParamName, "FileBasedRingGeneration"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->FileBasedRingGeneration = true;
       }
       else
       {
           pCapabilities->FileBasedRingGeneration = false;
       }

    }
    else if( strstr(ParamName, "RingGeneration"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->RingGeneration = true;
       }
       else
       {
           pCapabilities->RingGeneration = false;
       }

    }
    else if( strstr(ParamName, "RingFileFormats"))
    {

       strncpy(pCapabilities->RingFileFormats, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "DigitMap"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->DigitMap = true;
       }
       else
       {
           pCapabilities->DigitMap = false;
       }

    }
    else if( strstr(ParamName, "NumberingPlan"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->NumberingPlan = true;
       }
       else
       {
           pCapabilities->NumberingPlan = false;
       }

    }
    else if( strstr(ParamName, "ButtonMap"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->ButtonMap = true;
       }
       else
       {
           pCapabilities->ButtonMap = false;
       }

    }
    else if( strstr(ParamName, "VoicePortTests"))
    {
       if(strstr(pValue, "1"))
       {
           pCapabilities->VoicePortTests = true;
       }
       else
       {
           pCapabilities->VoicePortTests = false;
       }
    }
    else if( strstr(ParamName, "SIP"))
    {
        //VoiceService.{i}.Capabilities.SIP.
        PTELCOVOICEMGR_DML_CAPABILITIES_SIP pCapabilities_SIP= &(pCapabilities->CapabilitiesSIPObj);

        if( strstr(ParamName, "Role"))
        {
            if(strstr(pValue, "UserAgent"))
            {
               pCapabilities_SIP->Role = SIP_ROLE_USER_AGENT;
            }
            else if(strstr(pValue, "BackToBackUserAgents"))
            {
               pCapabilities_SIP->Role = SIP_ROLE_B2B_USER_AGENT;
            }
            else if(strstr(pValue, "OutboundProxy"))
            {
               pCapabilities_SIP->Role = SIP_ROLE_OUTBOUND_PROXY;
            }
        }
        else if( strstr(ParamName, "Extensions"))
        {
            strncpy(pCapabilities_SIP->Extensions, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Transports"))
        {
            strncpy(pCapabilities_SIP->Transports, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "URISchemes"))
        {
            strncpy(pCapabilities_SIP->URISchemes, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "EventSubscription"))
        {
            if(strstr(pValue, "1"))
            {
                pCapabilities_SIP->EventSubscription = true;
            }
            else
            {
                pCapabilities_SIP->EventSubscription = false;
            }
        }
        else if( strstr(ParamName, "ResponseMap"))
        {
            if(strstr(pValue, "1"))
            {
                pCapabilities_SIP->ResponseMap = true;
            }
            else
            {
                pCapabilities_SIP->ResponseMap = false;
            }
        }
        else if( strstr(ParamName, "TLSAuthenticationProtocols"))
        {
            strncpy(pCapabilities_SIP->TLSAuthenticationProtocols, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "TLSAuthenticationKeySizes"))
        {
            strncpy(pCapabilities_SIP->TLSAuthenticationKeySizes, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "TLSEncryptionProtocols"))
        {
            strncpy(pCapabilities_SIP->TLSEncryptionProtocols, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "TLSEncryptionKeySizes"))
        {
            strncpy(pCapabilities_SIP->TLSEncryptionKeySizes, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "TLSKeyExchangeProtocols"))
        {
            strncpy(pCapabilities_SIP->TLSKeyExchangeProtocols, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "MGCP"))
    {
        //VoiceService.{i}.Capabilities.MGCP.
        PTELCOVOICEMGR_DML_CAPABILITIES_MGCP pCapabilities_MGCP= &(pCapabilities->CapabilitiesMGCPObj);

        if( strstr(ParamName, "Extensions"))
        {
            strncpy(pCapabilities_MGCP->Extensions, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "H323"))
    {
        //VoiceService.{i}.Capabilities.H323.
        PTELCOVOICEMGR_DML_CAPABILITIES_H323 pCapabilities_H323= &(pCapabilities->CapabilitiesH323Obj);

        if( strstr(ParamName, "FastStart"))
        {
            if(strstr(pValue, "1"))
            {
                pCapabilities_H323->FastStart = true;
            }
            else
            {
                pCapabilities_H323->FastStart = false;
            }
        }
        else if( strstr(ParamName, "H235AuthenticationMethods"))
        {
            strncpy(pCapabilities_H323->H235AuthenticationMethods, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Codecs"))
    {
        //VoiceService.{i}.Capabilities.Codecs.{i}.
        if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_capabilitiesCodec_index(ParamName, DML_VOICESERVICE_CAPABILITIES_CODECS, &hal_index))
        {
            return ANSC_STATUS_FAILURE;
        }
        // capabilities codecs index
        codec_index = hal_index - 1;

        DML_CAPABILITIES_CODECS_LIST_T* pCapabilitiesCodecList = &(pCapabilities->CapabilitiesCodecList);

        DML_CAPABILITIES_CODECS_CTRL_T* pCapabilitiesCodecData = pCapabilitiesCodecList->pdata[codec_index];

        if(pCapabilitiesCodecData == NULL)
        {
            //create new
            if( TelcoVoiceMgrDmlAddCapabilitiesCodec(pCapabilitiesCodecList, codec_index) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }
            pCapabilitiesCodecData = pCapabilitiesCodecList->pdata[codec_index];
            if(pCapabilitiesCodecData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        TELCOVOICEMGR_DML_CAPABILITIES_CODECS* pCapabilities_Codecs = &(pCapabilitiesCodecData->dml);

        pCapabilities_Codecs->InstanceNumber = hal_index;

        if( strstr(ParamName, "EntryID"))
        {
           pCapabilities_Codecs->EntryID = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "Codec"))
        {
            strncpy(pCapabilities_Codecs->Codec, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "BitRate"))
        {
            pCapabilities_Codecs->BitRate = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "PacketizationPeriod"))
        {
            strncpy(pCapabilities_Codecs->PacketizationPeriod, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SilenceSuppression"))
        {
            if(strstr(pValue,"1"))
            {
                pCapabilities_Codecs->SilenceSuppression = true;
            }
            else
            {
                pCapabilities_Codecs->SilenceSuppression = false;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_phyInterface(DML_PHYINTERFACE_LIST_T* pPhyInterfaceList, char* ParamName, char* pValue)
{
    ANSC_STATUS retStatus = ANSC_STATUS_FAILURE;
    char *err;
    int uVsIndex = 0;
    int uPhyIndex = 0;

    if(ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceService_index(ParamName, DML_VOICESERVICE, &uVsIndex))
    {
        return ANSC_STATUS_FAILURE;
    }
    if(ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_phyInterface_index(ParamName, DML_VOICESERVICE_PHYIFACE, &uPhyIndex))
    {
        return ANSC_STATUS_FAILURE;
    }

    if( uVsIndex <= 0  || uPhyIndex <= 0 )
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
    DML_PHYINTERFACE_CTRL_T* pPhyInterfaceData = pPhyInterfaceList->pdata[uPhyIndex - 1];

    if(pPhyInterfaceData == NULL)
    {
        //create new PhyInterface
        if( TelcoVoiceMgrDmlAddPhyInterface(pPhyInterfaceList, uPhyIndex-1) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pPhyInterfaceData = pPhyInterfaceList->pdata[uPhyIndex - 1];
        if(pPhyInterfaceData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_PHYINTERFACE* pPhyInterface = &(pPhyInterfaceData->dml);
    if(pPhyInterface)
    {
        pPhyInterface->InstanceNumber = uPhyIndex;

        if( strstr(ParamName, "PhyPort"))
        {
            strncpy(pPhyInterface->PhyPort, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "InterfaceID"))
        {
            pPhyInterface->InterfaceID = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "Description"))
        {
            strncpy(pPhyInterface->Description, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Tests"))
        {
            //VoiceService.{i}.PhyInterface.{i}.Tests.
            PTELCOVOICEMGR_DML_PHYINTERFACE_TESTS pPhyInterface_Tests= &(pPhyInterface->PhyInterfaceTestsObj);

            if( strstr(ParamName, "TestState"))
            {
                if(strstr(pValue, "None"))
                {
                    pPhyInterface_Tests->TestState = PHYINTERFACE_TESTSTATE_NONE;
                }
                else if(strstr(pValue, "Requested"))
                {
                    pPhyInterface_Tests->TestState = PHYINTERFACE_TESTSTATE_REQUESTED;
                }
                else if(strstr(pValue, "Complete"))
                {
                    pPhyInterface_Tests->TestState = PHYINTERFACE_TESTSTATE_COMPLETE;
                }
                else if(strstr(pValue, "Error_TestNotSupported"))
                {
                    pPhyInterface_Tests->TestState = PHYINTERFACE_TESTSTATE_ERROR_TESTNOTSUPPORTED;
                }
            }
            else if( strstr(ParamName, "TestSelector"))
            {
                if(strstr(pValue, "PhoneConnectivityTest"))
                {
                    pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_PHONE_CONNECTIVITY_TEST;
                }
                else if(strstr(pValue, "Hazard Potential"))
                {
                    pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_HAZARD_POTENTIAL;
                }
                else if(strstr(pValue, "Foreign Voltage"))
                {
                    pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_FOREIGN_VOLTAGE;
                }
                else if(strstr(pValue, "Resistive Faults"))
                {
                    pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_RESISTIVE_FAULTS;
                }
                else if(strstr(pValue, "Off-hook"))
                {
                    pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_OFF_HOOK;
                }
                else if(strstr(pValue, "REN"))
                {
                    pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_REN;
                }
            }
            else if( strstr(ParamName, "PhoneConnectivity"))
            {
                if(strstr(pValue, "1"))
                {
                    pPhyInterface_Tests->PhoneConnectivity = true;
                }
                else
                {
                    pPhyInterface_Tests->PhoneConnectivity = false;
                }
            }
            else if( strstr(ParamName, "X_RDK_TestResult"))
            {
                strncpy(pPhyInterface_Tests->X_RDK_TestResult, pValue,strlen(pValue)+1);
            }   
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    pPhyInterfaceData->updated = true;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine(DML_LINE_LIST_T* pVoiceProfile_Line_List, char* ParamName, char* pValue)
{
    int Line_index = 0;
    int Evtsubscribe_index = 0;
    int Event_index = 0;
    int RingerPattern_index = 0;
    int Session_index = 0;
    int CodecList_index = 0;
    int RingerDesc_index = 0;
    int hal_index = 0;
    char *err;

    //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.
    if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_line_index(ParamName, DML_VOICESERVICE_VOICEPROF_LINE, &hal_index))
    {
        return ANSC_STATUS_FAILURE;
    }
    // index
    Line_index = hal_index - 1;

    DML_LINE_CTRL_T* pVoiceProfile_Line_Data = pVoiceProfile_Line_List->pdata[Line_index];

    if(pVoiceProfile_Line_Data == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileLine(pVoiceProfile_Line_List, Line_index) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pVoiceProfile_Line_Data = pVoiceProfile_Line_List->pdata[Line_index];
        if(pVoiceProfile_Line_Data == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_LINE* pVoiceProfile_Line = &(pVoiceProfile_Line_Data->dml);

    pVoiceProfile_Line->InstanceNumber = hal_index;

    if( strstr(ParamName, "Enable"))
    {
        if(strstr(pValue, "Disabled"))
        {
            pVoiceProfile_Line->Enable = DISABLED;
        }
        else if(strstr(pValue, "Quiescent"))
        {
            pVoiceProfile_Line->Enable = QUIESCENT;
        }
        else if(strstr(pValue, "Enabled"))
        {
            pVoiceProfile_Line->Enable = ENABLED;
        }
    }
    else if( strstr(ParamName, "DirectoryNumber"))
    {
    strncpy(pVoiceProfile_Line->DirectoryNumber, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Status"))
    {
        if(strstr(pValue, "Up"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_UP;
        }
        else if(strstr(pValue, "Initializing"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_INITIALIZING;
        }
        else if(strstr(pValue, "Registering"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_REGISTERING;
        }
        else if(strstr(pValue, "Unregistering"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_UNREGISTERING;
        }
        else if(strstr(pValue, "Error"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_ERROR;
        }
        else if(strstr(pValue, "Testing"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_TESTING;
        }
        else if(strstr(pValue, "Quiescent"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_QUIESCENT;
        }
        else if(strstr(pValue, "Disabled"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_DISABLED;
        }
    }
    else if( strstr(ParamName, "CallState"))
    {
        if(strstr(pValue, "Idle"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_IDLE;
        }
        else if(strstr(pValue, "Calling"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_CALLING;
        }
        else if(strstr(pValue, "Ringing"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_RINGING;
        }
        else if(strstr(pValue, "Connecting"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_CONNECTING;
        }
        else if(strstr(pValue, "InCall"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_INCALL;
        }
        else if(strstr(pValue, "Hold"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_HOLD;
        }
        else if(strstr(pValue, "Disconnecting"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_DISCONNECTING;
        }
    }
    else if( strstr(ParamName, "PhyReferenceList"))
    {
        strncpy(pVoiceProfile_Line->PhyReferenceList, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "RingMuteStatus"))
    {
        if(strstr(pValue, "true") || strstr(pValue, "1"))
        {
            pVoiceProfile_Line->RingMuteStatus = true;
        }
        else
        {
            pVoiceProfile_Line->RingMuteStatus = false;
        }
    }
    else if( strstr(ParamName, "RingVolumeStatus"))
    {
        pVoiceProfile_Line->RingVolumeStatus = strtoul(pValue,&err,10);
    }
    else if( strstr(ParamName, "X_RDK_OutboundProxyAddresses"))
    {
        strncpy(pVoiceProfile_Line->X_RDK_OutboundProxyAddresses, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "SIP"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP.
        TELCOVOICEMGR_DML_LINE_SIP* pVoiceProfile_Line_SIP = &(pVoiceProfile_Line->LineSipObj);
        if( strstr(ParamName, "AuthUserName"))
        {
            strncpy(pVoiceProfile_Line_SIP->AuthUserName, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "AuthPassword"))
        {
            strncpy(pVoiceProfile_Line_SIP->AuthPassword, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "URI"))
        {
            strncpy(pVoiceProfile_Line_SIP->URI, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "EventSubscribe"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP.EventSubscribe.{i}.
            if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_line_sipEvtSubscribe_index(ParamName, DML_VOICESERVICE_VOICEPROF_LINE_SIP_EVTSUBSCRIBE, &hal_index))
            {
                return ANSC_STATUS_FAILURE;
            }
            // index
            Evtsubscribe_index = hal_index - 1;

            DML_LINE_SIP_EVENTSUBSCRIBE_LIST_T* pVoiceProfile_Line_SIP_EvtSubscribe_List = &(pVoiceProfile_Line_SIP->LineSipEventSubscribeList);

            DML_LINE_SIP_EVENTSUBSCRIBE_CTRL_T* pVoiceProfile_Line_SIP_EvtSubscribe_Data = pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[Evtsubscribe_index];

            if(pVoiceProfile_Line_SIP_EvtSubscribe_Data == NULL)
            {
                //create new
                if( TelcoVoiceMgrDmlAddVoiceProfileLineSIPEvtSubscribe(pVoiceProfile_Line_SIP_EvtSubscribe_List, Evtsubscribe_index) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }
                pVoiceProfile_Line_SIP_EvtSubscribe_Data = pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[Evtsubscribe_index];
                if(pVoiceProfile_Line_SIP_EvtSubscribe_Data == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            TELCOVOICEMGR_DML_LINE_SIP_EVENTSUBSCRIBE* pVoiceProfile_Line_SIP_EventSubscribe = &(pVoiceProfile_Line_SIP_EvtSubscribe_Data->dml);

            pVoiceProfile_Line_SIP_EventSubscribe->InstanceNumber = hal_index;

            if( strstr(ParamName, "Event"))
            {
                strncpy(pVoiceProfile_Line_SIP_EventSubscribe->Event, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "AuthUserName"))
            {
                strncpy(pVoiceProfile_Line_SIP_EventSubscribe->AuthUserName, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "AuthPassword"))
            {
                strncpy(pVoiceProfile_Line_SIP_EventSubscribe->AuthPassword, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "MGCP"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.MGCP.
        TELCOVOICEMGR_DML_LINE_MGCP* pVoiceProfile_Line_MGCP = &(pVoiceProfile_Line->LineMGCPObj);
        if( strstr(ParamName, "LineName"))
        {
            strncpy(pVoiceProfile_Line_MGCP->LineName, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "H323"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.H323.
        TELCOVOICEMGR_DML_LINE_H323* pVoiceProfile_Line_H323 = &(pVoiceProfile_Line->LineH323Obj);
        if( strstr(ParamName, "H323ID"))
        {
            strncpy(pVoiceProfile_Line_H323->H323ID, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Line.{i}.Ringer."))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.
        TELCOVOICEMGR_DML_LINE_RINGER* pVoiceProfile_Line_Ringer = &(pVoiceProfile_Line->LineRingerObj);
        if( strstr(ParamName, "Event"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.Event.{i}
            if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_line_ringerEvt_index(ParamName, DML_VOICESERVICE_VOICEPROF_LINE_RINGER_EVT, &hal_index))
            {
                return ANSC_STATUS_FAILURE;
            }
            // index
            Event_index = hal_index - 1;

            DML_LINE_RINGER_EVENT_LIST_T* pVoiceProfile_Line_Ringer_Event_List = &(pVoiceProfile_Line_Ringer->LineRingerEventList);

            DML_LINE_RINGER_PATTERN_CTRL_T* pVoiceProfile_Line_Ringer_Event_Data = pVoiceProfile_Line_Ringer_Event_List->pdata[Event_index];

            if(pVoiceProfile_Line_Ringer_Event_Data == NULL)
            {
                //create new
                if( TelcoVoiceMgrDmlAddVoiceProfileLineRingerEvent(pVoiceProfile_Line_Ringer_Event_List, Event_index) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pVoiceProfile_Line_Ringer_Event_Data = pVoiceProfile_Line_Ringer_Event_List->pdata[Event_index];
                if(pVoiceProfile_Line_Ringer_Event_Data == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            TELCOVOICEMGR_DML_LINE_RINGER_EVENT* pVoiceProfile_Line_Ringer_Event = &(pVoiceProfile_Line_Ringer_Event_Data->dml);

            pVoiceProfile_Line_Ringer_Event->InstanceNumber = hal_index;

            if( strstr(ParamName, "Function"))
            {
                strncpy(pVoiceProfile_Line_Ringer_Event->Function, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "RingID"))
            {
                pVoiceProfile_Line_Ringer_Event->RingID = strtoul(pValue,&err,10);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "Description"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.Description.{i}
            if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_line_ringerDesc_index(ParamName, DML_VOICESERVICE_VOICEPROF_LINE_RINGER_DESC, &hal_index))
            {
                return ANSC_STATUS_FAILURE;
            }
            // index
            RingerDesc_index = hal_index - 1;

            DML_LINE_RINGER_DESCRIPTION_LIST_T* pVoiceProfile_Line_Ringer_Desc_List = &(pVoiceProfile_Line_Ringer->LineRingerDescriptionList);

            DML_LINE_RINGER_DESCRIPTION_CTRL_T* pVoiceProfile_Line_Ringer_Desc_Data = pVoiceProfile_Line_Ringer_Desc_List->pdata[RingerDesc_index];

            if(pVoiceProfile_Line_Ringer_Desc_Data == NULL)
            {
                //create new
                if( TelcoVoiceMgrDmlAddVoiceProfileLineRingerDesc(pVoiceProfile_Line_Ringer_Desc_List, RingerDesc_index) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;   
                }
                pVoiceProfile_Line_Ringer_Desc_Data = pVoiceProfile_Line_Ringer_Desc_List->pdata[RingerDesc_index];
                if(pVoiceProfile_Line_Ringer_Desc_Data == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            TELCOVOICEMGR_DML_LINE_RINGER_DESCRIPTION* pVoiceProfile_Line_Ringer_Desc = &(pVoiceProfile_Line_Ringer_Desc_Data->dml);

            pVoiceProfile_Line_Ringer_Desc->InstanceNumber = hal_index;

            if( strstr(ParamName, "EntryID"))
            {
                pVoiceProfile_Line_Ringer_Desc->EntryID = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RingEnable"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProfile_Line_Ringer_Desc->RingEnable = true;
                }
                else
                {
                    pVoiceProfile_Line_Ringer_Desc->RingEnable = false;
                }
            }
            else if( strstr(ParamName, "RingName"))
            {
                strncpy(pVoiceProfile_Line_Ringer_Desc->RingName, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "RingPattern"))
            {
                pVoiceProfile_Line_Ringer_Desc->RingPattern = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RingFile"))
            {
                strncpy(pVoiceProfile_Line_Ringer_Desc->RingFile, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "Pattern"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.Pattern.{i}
            if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_line_ringerPattern_index(ParamName, DML_VOICESERVICE_VOICEPROF_LINE_RINGER_PATTERN, &hal_index))
            {
                return ANSC_STATUS_FAILURE;
            }
            // index
            RingerPattern_index = hal_index - 1;

            DML_LINE_RINGER_PATTERN_LIST_T* pVoiceProfile_Line_Ringer_Pattern_List = &(pVoiceProfile_Line_Ringer->LineRingerPatternList);

            DML_LINE_RINGER_PATTERN_CTRL_T* pVoiceProfile_Line_Ringer_Pattern_Data = pVoiceProfile_Line_Ringer_Pattern_List->pdata[RingerPattern_index];

            if(pVoiceProfile_Line_Ringer_Pattern_Data == NULL)
            {
                //create new
                if( TelcoVoiceMgrDmlAddVoiceProfileLineRingerPattern(pVoiceProfile_Line_Ringer_Pattern_List, RingerPattern_index) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;   
                }
                pVoiceProfile_Line_Ringer_Pattern_Data = pVoiceProfile_Line_Ringer_Pattern_List->pdata[RingerPattern_index];
                if(pVoiceProfile_Line_Ringer_Pattern_Data == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            TELCOVOICEMGR_DML_LINE_RINGER_PATTERN* pVoiceProfile_Line_Ringer_Pattern = &(pVoiceProfile_Line_Ringer_Pattern_Data->dml);

            pVoiceProfile_Line_Ringer_Pattern->InstanceNumber = hal_index;

            if( strstr(ParamName, "EntryID"))
            {
                pVoiceProfile_Line_Ringer_Pattern->EntryID = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RingerOn"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProfile_Line_Ringer_Pattern->RingerOn = true;
                }
                else
                {
                    pVoiceProfile_Line_Ringer_Pattern->RingerOn = false;
                }
            }
            else if( strstr(ParamName, "Duration"))
            {
                pVoiceProfile_Line_Ringer_Pattern->Duration = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "NextEntryID"))
            {
                pVoiceProfile_Line_Ringer_Pattern->NextEntryID = strtoul(pValue,&err,10);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "CallingFeatures"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.CallingFeatures.
        TELCOVOICEMGR_DML_LINE_CALLINGFEATURES* pVoiceProfile_Line_CallingFeatures = &(pVoiceProfile_Line->LineCallingFeaturesObj);
        if( strstr(ParamName, "CallerIDEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->CallerIDEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->CallerIDEnable = false;
            }
        }
        else if( strstr(ParamName, "CallerIDNameEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->CallerIDNameEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->CallerIDNameEnable = false;
            }
        }
        else if( strstr(ParamName, "CallerIDName"))
        {
            strncpy(pVoiceProfile_Line_CallingFeatures->CallerIDName, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "X_RDK-Central_COM_ConferenceCallingEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->ConferenceCallingEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->ConferenceCallingEnable = false;
            }
        }
        else if( strstr(ParamName, "X_RDK-Central_COM_HoldEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->HoldEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->HoldEnable = false;
            }
        }
        else if( strstr(ParamName, "X_RDK-Central_COM_PhoneCallerIDEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->PhoneCallerIDEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->PhoneCallerIDEnable = false;
            }
        }
        else if( strstr(ParamName, "CallWaitingEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->CallWaitingEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->CallWaitingEnable = false;
            }
        }
        else if( strstr(ParamName, "CallWaitingStatus"))
        {
            if(strstr(pValue, "Disabled"))
            {
                pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_DISABLED;
            }
            else if(strstr(pValue, "Idle"))
            {
                pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_IDLE;
            }
            else if(strstr(pValue, "SecondaryRinging"))
            {
                pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_SECONDARY_RINGING;
            }
            else if(strstr(pValue, "SecondaryConnecting"))
            {
                pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_SECONDARY_CONNECTING;
            }
            else if(strstr(pValue, "SecondaryConnected"))
            {
                pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_SECONDARY_CONNECTED;
            }
        }
        else if( strstr(ParamName, "MaxSessions"))
        {
            pVoiceProfile_Line_CallingFeatures->MaxSessions = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "ConferenceCallingStatus"))
        {
            if(strstr(pValue, "Disabled"))
            {
                pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_DISABLED;
            }
            else if(strstr(pValue, "Idle"))
            {
                pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_IDLE;
            }
            else if(strstr(pValue, "SecondaryCalling"))
            {
                pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_SECONDARY_RINGING;
            }
            else if(strstr(pValue, "SecondaryConnecting"))
            {
                pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_SECONDARY_CONNECTING;
            }
            else if(strstr(pValue, "SecondaryConnected"))
            {
                pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_SECONDARY_CONNECTED;
            }
            else if(strstr(pValue, "InConferenceCall"))
            {
                pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_IN_CONFERENCE_CALL;
            }
        }
        else if( strstr(ParamName, "ConferenceCallingSessionCount"))
        {
            pVoiceProfile_Line_CallingFeatures->ConferenceCallingSessionCount = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "CallForwardUnconditionalEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->CallForwardUnconditionalEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->CallForwardUnconditionalEnable = false;
            }
        }
        else if( strstr(ParamName, "CallForwardUnconditionalNumber"))
        {
            strncpy(pVoiceProfile_Line_CallingFeatures->CallForwardUnconditionalNumber, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CallForwardOnBusyEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->CallForwardOnBusyEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->CallForwardOnBusyEnable = false;
            }
        }
        else if( strstr(ParamName, "CallForwardOnBusyNumber"))
        {
            strncpy(pVoiceProfile_Line_CallingFeatures->CallForwardOnBusyNumber, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CallForwardOnNoAnswerEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->CallForwardOnNoAnswerEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->CallForwardOnNoAnswerEnable = false;
            }
        }
        else if( strstr(ParamName, "CallForwardOnNoAnswerNumber"))
        {
            strncpy(pVoiceProfile_Line_CallingFeatures->CallForwardOnNoAnswerNumber, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CallForwardOnNoAnswerRingCount"))
        {
            pVoiceProfile_Line_CallingFeatures->CallForwardOnNoAnswerRingCount = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "CallTransferEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->CallTransferEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->CallTransferEnable = false;
            }
        }
        else if( strstr(ParamName, "MWIEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->MWIEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->MWIEnable = false;
            }
        }
        else if( strstr(ParamName, "MessageWaiting"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->MessageWaiting = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->MessageWaiting = false;
            }
        }
        else if( strstr(ParamName, "AnonymousCallBlockEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->AnonymousCallBlockEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->AnonymousCallBlockEnable = false;
            }
        }
        else if( strstr(ParamName, "AnonymousCalEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->AnonymousCalEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->AnonymousCalEnable = false;
            }
        }
        else if( strstr(ParamName, "DoNotDisturbEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->DoNotDisturbEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->DoNotDisturbEnable = false;
            }
        }
        else if( strstr(ParamName, "CallReturnEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->CallReturnEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->CallReturnEnable = false;
            }
        }
        else if( strstr(ParamName, "RepeatDialEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_CallingFeatures->RepeatDialEnable = true;
            }
            else
            {
                pVoiceProfile_Line_CallingFeatures->RepeatDialEnable = false;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "VoiceProcessing"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.VoiceProcessing.
        TELCOVOICEMGR_DML_LINE_VOICEPROCESSING* pVoiceProfile_Line_VoiceProcessing = &(pVoiceProfile_Line->LineVoiceProcessingObj);
        if( strstr(ParamName, "TransmitGain"))
        {
            pVoiceProfile_Line_VoiceProcessing->TransmitGain = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "ReceiveGain"))
        {
            pVoiceProfile_Line_VoiceProcessing->ReceiveGain = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "EchoCancellationEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_VoiceProcessing->EchoCancellationEnable = true;
            }
            else
            {
                pVoiceProfile_Line_VoiceProcessing->EchoCancellationEnable = false;
            }
        }
        else if( strstr(ParamName, "EchoCancellationInUse"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_VoiceProcessing->EchoCancellationInUse = true;
            }
            else
            {
                pVoiceProfile_Line_VoiceProcessing->EchoCancellationInUse = false;
            }
        }
        else if( strstr(ParamName, "EchoCancellationTail"))
        {
            pVoiceProfile_Line_VoiceProcessing->EchoCancellationTail = strtoul(pValue,&err,10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Codec"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Codec.
        TELCOVOICEMGR_DML_LINE_CODEC* pVoiceProfile_Line_Codec = &(pVoiceProfile_Line->LineCodecObj);
        if( strstr(ParamName, "TransmitCodec"))
        {
            strncpy(pVoiceProfile_Line_Codec->TransmitCodec, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ReceiveCodec"))
        {
            strncpy(pVoiceProfile_Line_Codec->ReceiveCodec, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "TransmitBitRate"))
        {
            pVoiceProfile_Line_Codec->TransmitBitRate = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "ReceiveBitRate"))
        {
            pVoiceProfile_Line_Codec->ReceiveBitRate = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "TransmitSilenceSuppression"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_Codec->TransmitSilenceSuppression = true;
            }
            else
            {
                pVoiceProfile_Line_Codec->TransmitSilenceSuppression = false;
            }
        }
        else if( strstr(ParamName, "ReceiveSilenceSuppression"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_Codec->ReceiveSilenceSuppression = true;
            }
            else
            {
                pVoiceProfile_Line_Codec->ReceiveSilenceSuppression = false;
            }
        }
        else if( strstr(ParamName, "TransmitPacketizationPeriod"))
        {
            pVoiceProfile_Line_Codec->TransmitPacketizationPeriod = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "List"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Codec.List.{i}.
            if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_line_codecList_index(ParamName, DML_VOICESERVICE_VOICEPROF_LINE_CODEC_LIST, &hal_index))
            {
                return ANSC_STATUS_FAILURE;
            }
            // index
            CodecList_index = hal_index - 1;

            DML_LINE_CODECLIST_LIST_T* pVoiceProfile_Line_CodecList_List = &(pVoiceProfile_Line_Codec->LineCodecList);

            DML_LINE_CODECLIST_CTRL_T* pVoiceProfile_Line_CodecList_Data = pVoiceProfile_Line_CodecList_List->pdata[CodecList_index];

            if(pVoiceProfile_Line_CodecList_Data == NULL)
            {
                //create new
                if( TelcoVoiceMgrDmlAddVoiceProfileLineCodecList(pVoiceProfile_Line_CodecList_List, CodecList_index) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }
                pVoiceProfile_Line_CodecList_Data = pVoiceProfile_Line_CodecList_List->pdata[CodecList_index];
                if(pVoiceProfile_Line_CodecList_Data == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            TELCOVOICEMGR_DML_LINE_CODECLIST* pVoiceProfile_Line_Codec_List = &(pVoiceProfile_Line_CodecList_Data->dml);

            pVoiceProfile_Line_Codec_List->InstanceNumber = hal_index;

            if( strstr(ParamName, "EntryID"))
            {
                pVoiceProfile_Line_Codec_List->EntryID = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "Codec"))
            {
                strncpy(pVoiceProfile_Line_Codec_List->Codec, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "BitRate"))
            {
                pVoiceProfile_Line_Codec_List->BitRate = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "PacketizationPeriod"))
            {
                strncpy(pVoiceProfile_Line_Codec_List->PacketizationPeriod, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "SilenceSuppression"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProfile_Line_Codec_List->SilenceSuppression = true;
                }
                else
                {
                    pVoiceProfile_Line_Codec_List->SilenceSuppression = false;
                }
            }
            else if( strstr(ParamName, "Enable"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProfile_Line_Codec_List->Enable = true;
                }
                else
                {
                    pVoiceProfile_Line_Codec_List->Enable = false;
                }
            }
            else if( strstr(ParamName, "Priority"))
            {
                pVoiceProfile_Line_Codec_List->Priority = strtoul(pValue,&err,10);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Session"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Session.{i}.
        if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_line_session_index(ParamName, DML_VOICESERVICE_VOICEPROF_LINE_SESSION, &hal_index))
        {
            return ANSC_STATUS_FAILURE;
        }
        // index
        Session_index = hal_index - 1;

        DML_LINE_SESSION_LIST_T* pVoiceProfile_Line_Session_List = &(pVoiceProfile_Line->LineSessionList);

        DML_LINE_SESSION_CTRL_T* pVoiceProfile_Line_Session_Data = pVoiceProfile_Line_Session_List->pdata[Session_index];

        if(pVoiceProfile_Line_Session_Data == NULL)
        {
            //create new
            if( TelcoVoiceMgrDmlAddVoiceProfileLineSession(pVoiceProfile_Line_Session_List, Session_index) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }
            pVoiceProfile_Line_Session_Data = pVoiceProfile_Line_Session_List->pdata[Session_index];
            if(pVoiceProfile_Line_Session_Data == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        TELCOVOICEMGR_DML_LINE_SESSION* pVoiceProfile_Line_Session = &(pVoiceProfile_Line_Session_Data->dml);

        pVoiceProfile_Line_Session->InstanceNumber = hal_index;

        if( strstr(ParamName, "SessionStartTime"))
        {
            //dateTime
            strncpy(pVoiceProfile_Line_Session->SessionStartTime, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SessionDuration"))
        {
            pVoiceProfile_Line_Session->SessionDuration = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "FarEndIPAddress"))
        {
            strncpy(pVoiceProfile_Line_Session->FarEndIPAddress, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "FarEndUDPPort"))
        {
            pVoiceProfile_Line_Session->FarEndUDPPort = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "LocalUDPPort"))
        {
            pVoiceProfile_Line_Session->LocalUDPPort = strtoul(pValue,&err,10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Stats"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Stats.
        TELCOVOICEMGR_DML_LINE_STATS* pVoiceProfile_Line_Stats = &(pVoiceProfile_Line->LineStatsObj);

        if( strstr(ParamName, "ResetStatistics"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProfile_Line_Stats->ResetStatistics = true;
            }
            else
            {
                pVoiceProfile_Line_Stats->ResetStatistics = false;
            }
        }
        else if( strstr(ParamName, "PacketsSent"))
        {
            pVoiceProfile_Line_Stats->PacketsSent = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "PacketsReceived"))
        {
            pVoiceProfile_Line_Stats->PacketsReceived = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "BytesSent"))
        {
            pVoiceProfile_Line_Stats->BytesSent = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "BytesReceived"))
        {
            pVoiceProfile_Line_Stats->BytesReceived = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "PacketsLost"))
        {
            pVoiceProfile_Line_Stats->PacketsLost = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "Overruns"))
        {
            pVoiceProfile_Line_Stats->Overruns = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "Underruns"))
        {
            pVoiceProfile_Line_Stats->Underruns = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "IncomingCallsReceived"))
        {
            pVoiceProfile_Line_Stats->IncomingCallsReceived = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "IncomingCallsAnswered"))
        {
            pVoiceProfile_Line_Stats->IncomingCallsAnswered = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "IncomingCallsConnected"))
        {
            pVoiceProfile_Line_Stats->IncomingCallsConnected = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "IncomingCallsFailed"))
        {
            pVoiceProfile_Line_Stats->IncomingCallsFailed = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "OutgoingCallsAttempted"))
        {
            pVoiceProfile_Line_Stats->OutgoingCallsAttempted = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "OutgoingCallsAnswered"))
        {
            pVoiceProfile_Line_Stats->OutgoingCallsAnswered = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "OutgoingCallsConnected"))
        {
            pVoiceProfile_Line_Stats->OutgoingCallsConnected = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "OutgoingCallsFailed"))
        {
            pVoiceProfile_Line_Stats->OutgoingCallsFailed = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "CallsDropped"))
        {
            pVoiceProfile_Line_Stats->CallsDropped = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "TotalCallTime"))
        {
            pVoiceProfile_Line_Stats->TotalCallTime = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "ServerDownTime"))
        {
            pVoiceProfile_Line_Stats->ServerDownTime = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "ReceivePacketLossRate"))
        {
            pVoiceProfile_Line_Stats->ReceivePacketLossRate = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "FarEndPacketLossRate"))
        {
            pVoiceProfile_Line_Stats->FarEndPacketLossRate = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "ReceiveInterarrivalJitter"))
        {
            pVoiceProfile_Line_Stats->ReceiveInterarrivalJitter = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "FarEndInterarrivalJitter"))
        {
            pVoiceProfile_Line_Stats->FarEndInterarrivalJitter = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "RoundTripDelay"))
        {
            pVoiceProfile_Line_Stats->RoundTripDelay = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "AverageReceiveInterarrivalJitter"))
        {
            pVoiceProfile_Line_Stats->AverageReceiveInterarrivalJitter = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "AverageFarEndInterarrivalJitter"))
        {
            pVoiceProfile_Line_Stats->AverageFarEndInterarrivalJitter = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "AverageRoundTripDelay"))
        {
            pVoiceProfile_Line_Stats->AverageRoundTripDelay = strtoul(pValue,&err,10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile(DML_PROFILE_LIST_T* pVoiceProfileList, char* ParamName, char* pValue)
{
    char *err;
    int uVsIndex = 0;
    int uVpIndex = 0;
    int sipEvtSubscribe_index = 0;
    int SipRespMap_index = 0;
    int PrefixInfo_index = 0;
    int ToneEvent_index = 0;
    int ToneDesc_index = 0;
    int TonePattern_index = 0;
    int Button_index = 0;
    int hal_index = 0;

    if(ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceService_index(ParamName, DML_VOICESERVICE, &uVsIndex))
    {
        return ANSC_STATUS_FAILURE;
    }
    if(ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_index(ParamName, DML_VOICESERVICE_VOICEPROF, &uVpIndex))
    {
        return ANSC_STATUS_FAILURE;
    }

    if( uVsIndex <= 0  || uVpIndex <= 0 )
    {
        AnscTraceError(("\n%s:%d:: \nInvalid index ParamName[%s]", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    DML_PROFILE_CTRL_T* pVoiceProfile = pVoiceProfileList->pdata[uVpIndex - 1];

    if(pVoiceProfile == NULL)
    {
        //create new VoiceProfile
        if( TelcoVoiceMgrDmlAddVoiceProfile(pVoiceProfileList, uVpIndex - 1) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pVoiceProfile = pVoiceProfileList->pdata[uVpIndex - 1];
        if(pVoiceProfile == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProf = &(pVoiceProfile->dml);
    if (pVoiceProf)
    {
        //VoiceService.{i}.VoiceProfile.{i}.
        if( strstr(ParamName, "Line"))
        { 
            //VoiceService.{i}.VoiceProfile{i}.Line.{i}.
            if(Map_hal_dml_voiceProfileLine(&(pVoiceProf->LineList), ParamName, pValue) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Mapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
                return ANSC_STATUS_FAILURE;
            }          
        }
        else if( strstr(ParamName, "ServiceProviderInfo"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.ServiceProviderInfo.
            PTELCOVOICEMGR_DML_SERVICE_PROVIDER_INFO pVoiceProf_ServiceProviderInfo = &(pVoiceProf->ServiceProviderInfoObj);

            if( strstr(ParamName, "Name"))
            {
                strncpy(pVoiceProf_ServiceProviderInfo->Name, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "URL"))
            {
                strncpy(pVoiceProf_ServiceProviderInfo->URL, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "ContactPhoneNumber"))
            {
                strncpy(pVoiceProf_ServiceProviderInfo->ContactPhoneNumber, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "EmailAddress"))
            {
                strncpy(pVoiceProf_ServiceProviderInfo->EmailAddress, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "SIP"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.SIP.
            PTELCOVOICEMGR_DML_SIP pVoiceProf_SIP = &(pVoiceProf->SIPObj);


            if( strstr(ParamName, "ProxyServerPort"))
            {
                pVoiceProf_SIP->ProxyServerPort = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "ProxyServerTransport"))
            {
                strncpy(pVoiceProf_SIP->ProxyServerTransport, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "ProxyServer"))
            {
                strncpy(pVoiceProf_SIP->ProxyServer, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "RegistrarServerPort"))
            {
                pVoiceProf_SIP->RegistrarServerPort = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RegistrarServerTransport"))
            {
                strncpy(pVoiceProf_SIP->RegistrarServerTransport, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "RegistrarServer"))
            {
                strncpy(pVoiceProf_SIP->RegistrarServer, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "UserAgentDomain"))
            {
                strncpy(pVoiceProf_SIP->UserAgentDomain, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "UserAgentPort"))
            {
                pVoiceProf_SIP->UserAgentPort = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "UserAgentTransport"))
            {
                strncpy(pVoiceProf_SIP->UserAgentTransport, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "OutboundProxyPort"))
            {
                pVoiceProf_SIP->OutboundProxyPort = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "OutboundProxy"))
            {
                strncpy(pVoiceProf_SIP->OutboundProxy, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "Organization"))
            {
                strncpy(pVoiceProf_SIP->Organization, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "RegistrationPeriod"))
            {
                pVoiceProf_SIP->RegistrationPeriod = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerT1"))
            {
                pVoiceProf_SIP->TimerT1 = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerT2"))
            {
                pVoiceProf_SIP->TimerT2 = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerT4"))
            {
                pVoiceProf_SIP->TimerT4 = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerA"))
            {
                pVoiceProf_SIP->TimerA = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerB"))
            {
                pVoiceProf_SIP->TimerB = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerC"))
            {
                pVoiceProf_SIP->TimerC = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerD"))
            {
                pVoiceProf_SIP->TimerD = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerE"))
            {
                pVoiceProf_SIP->TimerE = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerF"))
            {
                pVoiceProf_SIP->TimerF = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerG"))
            {
                pVoiceProf_SIP->TimerG = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerH"))
            {
                pVoiceProf_SIP->TimerH = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerI"))
            {
                pVoiceProf_SIP->TimerI = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerJ"))
            {
                pVoiceProf_SIP->TimerJ = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TimerK"))
            {
                pVoiceProf_SIP->TimerK = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "InviteExpires"))
            {
                pVoiceProf_SIP->InviteExpires = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "ReInviteExpires"))
            {
                pVoiceProf_SIP->ReInviteExpires = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RegisterExpires"))
            {
                pVoiceProf_SIP->RegisterExpires = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RegistersMinExpires"))
            {
                pVoiceProf_SIP->RegistersMinExpires = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RegisterRetryInterval"))
            {
                pVoiceProf_SIP->RegisterRetryInterval = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "InboundAuthUsername"))
            {
                strncpy(pVoiceProf_SIP->InboundAuthUsername, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "InboundAuthPassword"))
            {
                strncpy(pVoiceProf_SIP->InboundAuthPassword, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "InboundAuth"))
            {
                if(strstr(pValue, "None"))
                {
                    pVoiceProf_SIP->InboundAuth = SIP_INBOUNDAUTH_NONE;
                }
                else if(strstr(pValue, "Digest"))
                {
                    pVoiceProf_SIP->InboundAuth = SIP_INBOUNDAUTH_DIGEST;
                }
            }
            else if( strstr(ParamName, "UseCodecPriorityInSDPResponse"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProf_SIP->UseCodecPriorityInSDPResponse = true;
                }
                else
                {
                    pVoiceProf_SIP->UseCodecPriorityInSDPResponse = false;
                }
            }
            else if( strstr(ParamName, "DSCPMark"))
            {
                pVoiceProf_SIP->DSCPMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "VLANIDMark"))
            {
                pVoiceProf_SIP->VLANIDMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "EthernetPriorityMark"))
            {
                pVoiceProf_SIP->EthernetPriorityMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "X_RDK_Firewall_Rule_Data"))
            {
                strncpy(pVoiceProf_SIP->X_RDK_Firewall_Rule_Data, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "X_RDK_SKBMark"))
            {
                pVoiceProf_SIP->X_RDK_SKBMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "X_RDK-Central_COM_ConferencingURI"))
            {
                strncpy(pVoiceProf_SIP->ConferencingURI, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "X_RDK-Central_COM_NetworkDisconnect"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProf_SIP->NetworkDisconnect = true;
                }
                else
                {
                    pVoiceProf_SIP->NetworkDisconnect = false;
                }
            }
            else if( strstr(ParamName, "X_RDK_PRACKRequired"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProf_SIP->X_RDK_PRACKRequired = true;
                }
                else
                {
                    pVoiceProf_SIP->X_RDK_PRACKRequired = false;
                }
            } 
            else if( strstr(ParamName, "EventSubscribe"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.SIP.EventSubscribe.{i}.

                if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_sipEvtSubscribe_index(ParamName, DML_VOICESERVICE_VOICEPROF_SIP_EVTSUBSCRIBE, &hal_index))
                {
                    return ANSC_STATUS_FAILURE;
                }

                //index
                sipEvtSubscribe_index = hal_index - 1;

                DML_SIP_EVENTSUBSCRIBE_LIST_T* psipEvtSubscribeList = &(pVoiceProf_SIP->SipEventSubscribeList);

                DML_SIP_EVENTSUBSCRIBE_CTRL_T* psipEvtSubscribeData = psipEvtSubscribeList->pdata[sipEvtSubscribe_index];

                if(psipEvtSubscribeData == NULL)
                {
                    //create new
                    if( TelcoVoiceMgrDmlAddVoiceProfileSIPEventSubscribe(psipEvtSubscribeList, sipEvtSubscribe_index) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    psipEvtSubscribeData = psipEvtSubscribeList->pdata[sipEvtSubscribe_index];
                    if(psipEvtSubscribeData == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                TELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE* pVoiceProfile_SIP_EventSubscribe = &(psipEvtSubscribeData->dml);

                pVoiceProfile_SIP_EventSubscribe->InstanceNumber = hal_index;

                if( strstr(ParamName, "Event"))
                {
                    strncpy(pVoiceProfile_SIP_EventSubscribe->Event, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "NotifierPort"))
                {
                    pVoiceProfile_SIP_EventSubscribe->NotifierPort = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "NotifierTransport"))
                {
                    strncpy(pVoiceProfile_SIP_EventSubscribe->NotifierTransport, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "Notifier"))
                {
                    strncpy(pVoiceProfile_SIP_EventSubscribe->Notifier, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "ExpireTime"))
                {
                    pVoiceProfile_SIP_EventSubscribe->ExpireTime = strtoul(pValue,&err,10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "ResponseMap"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.SIP.ResponseMap.{i}.
                if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_sipRespMap_index(ParamName, DML_VOICESERVICE_VOICEPROF_SIP_RESPMAP, &hal_index))
                {
                    return ANSC_STATUS_FAILURE;
                }
                //   index
                SipRespMap_index = hal_index - 1;

                DML_SIP_RESPONSEMAP_LIST_T* pSIPResponseMapList = &(pVoiceProf_SIP->SipResponseMapList);

                DML_SIP_RESPONSEMAP_CTRL_T* pSIPResponseMapData = pSIPResponseMapList->pdata[SipRespMap_index];

                if(pSIPResponseMapData == NULL)
                {
                    //create new
                    if( TelcoVoiceMgrDmlAddVoiceProfileSIPRespMap(pSIPResponseMapList, SipRespMap_index) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pSIPResponseMapData = pSIPResponseMapList->pdata[SipRespMap_index];
                    if(pSIPResponseMapData == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                TELCOVOICEMGR_DML_SIP_RESPONSEMAP* pVoiceProfile_SIP_RespMap = &(pSIPResponseMapData->dml);

                pVoiceProfile_SIP_RespMap->InstanceNumber = hal_index;

                if( strstr(ParamName, "SIPResponseNumber"))
                {
                    pVoiceProfile_SIP_RespMap->SIPResponseNumber = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "TextMessage"))
                {
                    strncpy( pVoiceProfile_SIP_RespMap->TextMessage, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "Tone"))
                {
                    pVoiceProfile_SIP_RespMap->Tone = strtoul(pValue,&err,10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }  // END_OF_SIP
        else if( strstr(ParamName, "MGCP"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.MGCP.
            TELCOVOICEMGR_DML_MGCP* pVoiceProfile_MGCP = &(pVoiceProf->MGCPObj);
            if( strstr(ParamName, "CallAgent1"))
            {
            strncpy(pVoiceProfile_MGCP->CallAgent1, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "CallAgentPort1"))
            {
                pVoiceProfile_MGCP->CallAgentPort1 = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "CallAgent2"))
            {
            strncpy(pVoiceProfile_MGCP->CallAgent2, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "CallAgentPort2"))
            {
                pVoiceProfile_MGCP->CallAgentPort2 = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RetranIntervalTimer"))
            {
                pVoiceProfile_MGCP->RetranIntervalTimer = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "MaxRetranCount"))
            {
                pVoiceProfile_MGCP->MaxRetranCount = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RegisterMode"))
            {
                if(strstr(pValue, "WildCard"))
                {
                    pVoiceProfile_MGCP->RegisterMode = MGCP_REGISTERMODE_WILDCARD;
                }
                else if(strstr(pValue, "Individual"))
                {
                    pVoiceProfile_MGCP->RegisterMode = MGCP_REGISTERMODE_INDIVIDUAL;
                }
            }
            else if( strstr(ParamName, "LocalPort"))
            {
                pVoiceProfile_MGCP->LocalPort = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "Domain"))
            {
                strncpy(pVoiceProfile_MGCP->Domain, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "User"))
            {
                strncpy(pVoiceProfile_MGCP->User, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "DSCPMark"))
            {
                pVoiceProfile_MGCP->DSCPMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "VLANIDMark"))
            {
                pVoiceProfile_MGCP->VLANIDMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "EthernetPriorityMark"))
            {
                pVoiceProfile_MGCP->EthernetPriorityMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "AllowPiggybackEvents"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProfile_MGCP->AllowPiggybackEvents = true;
                }
                else
                {
                    pVoiceProfile_MGCP->AllowPiggybackEvents = false;
                }
            }
            else if( strstr(ParamName, "SendRSIPImmediately"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProfile_MGCP->SendRSIPImmediately = true;
                }
                else
                {
                    pVoiceProfile_MGCP->SendRSIPImmediately = false;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "H323"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.H323.
            TELCOVOICEMGR_DML_H323* pVoiceProfile_H323 = &(pVoiceProf->H323Obj);
            if( strstr(ParamName, "Gatekeeper"))
            {
                strncpy(pVoiceProfile_H323->Gatekeeper, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "GatekeeperPort"))
            {
                pVoiceProfile_H323->GatekeeperPort = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "GatekeeperID"))
            {
                strncpy(pVoiceProfile_H323->GatekeeperID, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "TimeToLive"))
            {
                pVoiceProfile_H323->TimeToLive = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "H235Authentication"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProfile_H323->H235Authentication = true;
                }
                else
                {
                    pVoiceProfile_H323->H235Authentication = false;
                }
            }
            else if( strstr(ParamName, "AuthPassword"))
            {
            strncpy(pVoiceProfile_H323->AuthPassword, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "SendersID"))
            {
            strncpy(pVoiceProfile_H323->SendersID, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "DSCPMark"))
            {
                pVoiceProfile_H323->DSCPMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "VLANIDMark"))
            {
                pVoiceProfile_H323->VLANIDMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "EthernetPriorityMark"))
            {
                pVoiceProfile_H323->EthernetPriorityMark = strtoul(pValue,&err,10);
            }
            else
            {
                AnscTraceError(("\n%s:%d:: \nInvalid ParamName[%s] \nparamValue[%s].", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "RTP"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.RTP.
            TELCOVOICEMGR_DML_RTP* pVoiceProfile_RTP = &(pVoiceProf->RTPObj);

            if( strstr(ParamName, "LocalPortMin"))
            {
                pVoiceProfile_RTP->LocalPortMin = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "LocalPortMax"))
            {
                pVoiceProfile_RTP->LocalPortMax = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "DSCPMark"))
            {
                pVoiceProfile_RTP->DSCPMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "VLANIDMark"))
            {
                pVoiceProfile_RTP->VLANIDMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "EthernetPriorityMark"))
            {
                pVoiceProfile_RTP->EthernetPriorityMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TelephoneEventPayloadType"))
            {
                pVoiceProfile_RTP->TelephoneEventPayloadType = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "X_RDK_Firewall_Rule_Data"))
            {
                strncpy(pVoiceProfile_RTP->X_RDK_Firewall_Rule_Data, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "X_RDK_SKBMark"))
            {
                pVoiceProfile_RTP->X_RDK_SKBMark = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "RTCP"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.RTP.RTCP.
                TELCOVOICEMGR_DML_RTP_RTCP* pVoiceProfile_RTP_RTCP = &(pVoiceProfile_RTP->RTCPObj);
                if( strstr(ParamName, "Enable"))
                {
                    if(strstr(pValue, "true") || strstr(pValue, "1"))
                    {
                        pVoiceProfile_RTP_RTCP->Enable = true;
                    }
                    else
                    {
                        pVoiceProfile_RTP_RTCP->Enable = false;
                    }
                }
                else if( strstr(ParamName, "TxRepeatInterval"))
                {
                    pVoiceProfile_RTP_RTCP->TxRepeatInterval = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "LocalCName"))
                {
                    strncpy(pVoiceProfile_RTP_RTCP->LocalCName, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "SRTP"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.RTP.SRTP.
                TELCOVOICEMGR_DML_RTP_SRTP* pVoiceProfile_RTP_SRTP = &(pVoiceProfile_RTP->SRTPObj);

                if( strstr(ParamName, "Enable"))
                {
                    if(strstr(pValue, "true") || strstr(pValue, "1"))
                    {
                        pVoiceProfile_RTP_SRTP->Enable = true;
                    }
                    else
                    {
                        pVoiceProfile_RTP_SRTP->Enable = false;
                    }
                }
                else if( strstr(ParamName, "KeyingMethods"))
                {
                    strncpy(pVoiceProfile_RTP_SRTP->KeyingMethods, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "EncryptionKeySizes"))
                {
                    strncpy(pVoiceProfile_RTP_SRTP->EncryptionKeySizes, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "Redundancy"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.RTP.Redundancy.
                TELCOVOICEMGR_DML_RTP_REDUNDANCY* pVoiceProfile_RTP_Redundancy = &(pVoiceProfile_RTP->RedundancyObj);

                if( strstr(ParamName, "Enable"))
                {
                    if(strstr(pValue, "true") || strstr(pValue, "1"))
                    {
                        pVoiceProfile_RTP_Redundancy->Enable = true;
                    }
                    else
                    {
                        pVoiceProfile_RTP_Redundancy->Enable  = false;
                    }
                }
                else if( strstr(ParamName, "BlockPayloadType"))
                {
                    pVoiceProfile_RTP_Redundancy->BlockPayloadType = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "PayloadType"))
                {
                    pVoiceProfile_RTP_Redundancy->PayloadType = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "FaxAndModemRedundancy"))
                {
                    pVoiceProfile_RTP_Redundancy->FaxAndModemRedundancy = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "ModemRedundancy"))
                {
                    pVoiceProfile_RTP_Redundancy->ModemRedundancy = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "DTMFRedundancy"))
                {
                    pVoiceProfile_RTP_Redundancy->DTMFRedundancy = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "VoiceRedundancy"))
                {
                    pVoiceProfile_RTP_Redundancy->VoiceRedundancy = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "MaxSessionsUsingRedundancy"))
                {
                    pVoiceProfile_RTP_Redundancy->MaxSessionsUsingRedundancy = strtoul(pValue,&err,10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "NumberingPlan"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.NumberingPlan.
            TELCOVOICEMGR_DML_NUMBERINGPLAN* pVoiceProfile_NumberingPlan = &(pVoiceProf->NumberingPlanObj);
            if( strstr(ParamName, "MinimumNumberOfDigits"))
            {
                pVoiceProfile_NumberingPlan->MinimumNumberOfDigits = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "MaximumNumberOfDigits"))
            {
                pVoiceProfile_NumberingPlan->MaximumNumberOfDigits = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "InterDigitTimerStd"))
            {
                pVoiceProfile_NumberingPlan->InterDigitTimerStd = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "InterDigitTimerOpen"))
            {
                pVoiceProfile_NumberingPlan->InterDigitTimerOpen = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "InvalidNumberTone"))
            {
                pVoiceProfile_NumberingPlan->InvalidNumberTone = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "PrefixInfoMaxEntries"))
            {
                pVoiceProfile_NumberingPlan->PrefixInfoMaxEntries = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "PrefixInfo"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.NumberingPlan.PrefixInfo.{i}.
                if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_numPlanPrefixInfo_index(ParamName, DML_VOICESERVICE_VOICEPROF_NUMPLAN_PREFIXINFO, &hal_index))
                {
                    return ANSC_STATUS_FAILURE;
                }
                // index
                PrefixInfo_index = hal_index - 1;

                DML_NUMBERINGPLAN_PREFIXINFO_LIST_T* pVoiceProfile_Numberingplan_PrefixInfo_List = &(pVoiceProfile_NumberingPlan->PrefixInfoList);

                DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T* pVoiceProfile_Numberingplan_PrefixInfo_Data = pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[PrefixInfo_index];

                if(pVoiceProfile_Numberingplan_PrefixInfo_Data == NULL)
                {
                    //create new Numberingplan PrefixInfo
                    if( TelcoVoiceMgrDmlAddNumberingplanPrefixInfo(pVoiceProfile_Numberingplan_PrefixInfo_List, PrefixInfo_index) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }
                    pVoiceProfile_Numberingplan_PrefixInfo_Data = pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[PrefixInfo_index];
                    if(pVoiceProfile_Numberingplan_PrefixInfo_Data == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO* pVoiceProfile_NumberingPlan_PrefixInfo = &(pVoiceProfile_Numberingplan_PrefixInfo_Data->dml);

                pVoiceProfile_NumberingPlan_PrefixInfo->InstanceNumber = hal_index;

                if( strstr(ParamName, "PrefixRange"))
                {
                    strncpy(pVoiceProfile_NumberingPlan_PrefixInfo->PrefixRange, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "PrefixMinNumberOfDigits"))
                {
                    pVoiceProfile_NumberingPlan_PrefixInfo->PrefixMinNumberOfDigits = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "PrefixMaxNumberOfDigits"))
                {
                    pVoiceProfile_NumberingPlan_PrefixInfo->PrefixMaxNumberOfDigits = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "NumberOfDigitsToRemove"))
                {
                    pVoiceProfile_NumberingPlan_PrefixInfo->NumberOfDigitsToRemove = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "PosOfDigitsToRemove"))
                {
                    pVoiceProfile_NumberingPlan_PrefixInfo->PosOfDigitsToRemove = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "DialTone"))
                {
                    pVoiceProfile_NumberingPlan_PrefixInfo->DialTone = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "FacilityAction"))
                {
                    strncpy(pVoiceProfile_NumberingPlan_PrefixInfo->FacilityAction, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "FacilityActionArgument"))
                {
                    strncpy(pVoiceProfile_NumberingPlan_PrefixInfo->FacilityActionArgument, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "Tone"))
        {
            // VoiceService.{i}.VoiceProfile.{i}.Tone.
            TELCOVOICEMGR_DML_TONE* pVoiceProfile_Tone = &(pVoiceProf->ToneObj);
            if( strstr(ParamName, "Event"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.Tone.Event.{i}.
                if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_toneEvt_index(ParamName, DML_VOICESERVICE_VOICEPROF_TONE_EVT, &hal_index))
                {
                    return ANSC_STATUS_FAILURE;
                }
                // capabilities codecs index
                ToneEvent_index = hal_index - 1;

                DML_TONE_EVENT_LIST_T* pVoiceProfile_Tone_Event_List = &(pVoiceProfile_Tone->ToneEventList);

                DML_TONE_EVENT_CTRL_T* pVoiceProfile_Tone_Event_Data = pVoiceProfile_Tone_Event_List->pdata[ToneEvent_index];

                if(pVoiceProfile_Tone_Event_Data == NULL)
                {
                    //create new pVoiceProfile_Tone_Event
                    if( TelcoVoiceMgrDmlAddVoiceProfileToneEvent(pVoiceProfile_Tone_Event_List, ToneEvent_index) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pVoiceProfile_Tone_Event_Data = pVoiceProfile_Tone_Event_List->pdata[ToneEvent_index];
                    if(pVoiceProfile_Tone_Event_Data == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                TELCOVOICEMGR_DML_TONE_EVENT* pVoiceProfile_Tone_Event = &(pVoiceProfile_Tone_Event_Data->dml);

                pVoiceProfile_Tone_Event->InstanceNumber = hal_index;

                if( strstr(ParamName, "Function"))
                {
                    strncpy(pVoiceProfile_Tone_Event->Function, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "ToneID"))
                {
                    pVoiceProfile_Tone_Event->ToneID = strtoul(pValue,&err,10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "Description"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.Tone.Description.{i}.
                if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_toneDesc_index(ParamName, DML_VOICESERVICE_VOICEPROF_TONE_DESC, &hal_index))
                {
                    return ANSC_STATUS_FAILURE;
                }
                // index
                ToneDesc_index = hal_index - 1;

                DML_TONE_DESCRIPTION_LIST_T* pVoiceProfile_Tone_Desc_List = &(pVoiceProfile_Tone->ToneDescriptionList);

                DML_TONE_DESCRIPTION_CTRL_T* pVoiceProfile_Tone_Desc_Data = pVoiceProfile_Tone_Desc_List->pdata[ToneDesc_index];

                if(pVoiceProfile_Tone_Desc_Data == NULL)
                {
                    //create new Profile Tone Desc
                    if( TelcoVoiceMgrDmlAddVoiceProfileToneDesc(pVoiceProfile_Tone_Desc_List, ToneDesc_index) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pVoiceProfile_Tone_Desc_Data = pVoiceProfile_Tone_Desc_List->pdata[ToneDesc_index];
                    if(pVoiceProfile_Tone_Desc_Data == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                TELCOVOICEMGR_DML_TONE_DESCRIPTION* pVoiceProfile_Tone_Desc = &(pVoiceProfile_Tone_Desc_Data->dml);

                pVoiceProfile_Tone_Desc->InstanceNumber = hal_index;

                if( strstr(ParamName, "EntryID"))
                {
                    pVoiceProfile_Tone_Desc->EntryID = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "ToneEnable"))
                {
                    if(strstr(pValue, "true") || strstr(pValue, "1"))
                    {
                        pVoiceProfile_Tone_Desc->ToneEnable = true;
                    }
                    else
                    {
                        pVoiceProfile_Tone_Desc->ToneEnable = false;
                    }
                }
                else if( strstr(ParamName, "ToneName"))
                {
                    strncpy(pVoiceProfile_Tone_Desc->ToneName, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "TonePattern"))
                {
                    pVoiceProfile_Tone_Desc->TonePattern = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "ToneFile"))
                {
                    strncpy(pVoiceProfile_Tone_Desc->ToneFile, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "ToneRepetitions"))
                {
                    pVoiceProfile_Tone_Desc->ToneRepetitions = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "ToneText"))
                {
                    strncpy(pVoiceProfile_Tone_Desc->ToneText, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "Pattern"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.Tone.Pattern.{i}.
                if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_tonePattern_index(ParamName, DML_VOICESERVICE_VOICEPROF_TONE_PATTERN, &hal_index))
                {
                    return ANSC_STATUS_FAILURE;
                }
                // index
                TonePattern_index = hal_index - 1;

                DML_TONE_PATTERN_LIST_T* pVoiceProfile_Tone_Pattern_List = &(pVoiceProfile_Tone->TonePatternList);

                DML_TONE_PATTERN_CTRL_T* pVoiceProfile_Tone_Pattern_Data = pVoiceProfile_Tone_Pattern_List->pdata[TonePattern_index];

                if(pVoiceProfile_Tone_Pattern_Data == NULL)
                {
                    //create new Veip
                    if( TelcoVoiceMgrDmlAddVoiceProfileTonePattern(pVoiceProfile_Tone_Pattern_List, TonePattern_index) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pVoiceProfile_Tone_Pattern_Data = pVoiceProfile_Tone_Pattern_List->pdata[TonePattern_index];
                    if(pVoiceProfile_Tone_Pattern_Data == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                TELCOVOICEMGR_DML_TONE_PATTERN* pVoiceProfile_Tone_Pattern = &(pVoiceProfile_Tone_Pattern_Data->dml);

                pVoiceProfile_Tone_Pattern->InstanceNumber = hal_index;
                if( strstr(ParamName, "EntryID"))
                {
                    pVoiceProfile_Tone_Pattern->EntryID = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "ToneOn"))
                {
                    if(strstr(pValue, "true") || strstr(pValue, "1"))
                    {
                        pVoiceProfile_Tone_Pattern->ToneOn = true;
                    }
                    else
                    {
                        pVoiceProfile_Tone_Pattern->ToneOn = false;
                    }
                }
                else if( strstr(ParamName, "Frequency1"))
                {
                    pVoiceProfile_Tone_Pattern->Frequency1 = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "Power1"))
                {
                    pVoiceProfile_Tone_Pattern->Power1 = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "Frequency2"))
                {
                    pVoiceProfile_Tone_Pattern->Frequency2 = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "Power2"))
                {
                    pVoiceProfile_Tone_Pattern->Power2 = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "Frequency3"))
                {
                    pVoiceProfile_Tone_Pattern->Frequency3 = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "Power3"))
                {
                    pVoiceProfile_Tone_Pattern->Power3 = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "Frequency4"))
                {
                    pVoiceProfile_Tone_Pattern->Frequency4 = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "Power4"))
                {
                    pVoiceProfile_Tone_Pattern->Power4 = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "ModulationFrequency"))
                {
                    pVoiceProfile_Tone_Pattern->ModulationFrequency = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "ModulationPower"))
                {
                    pVoiceProfile_Tone_Pattern->ModulationPower = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "Duration"))
                {
                    pVoiceProfile_Tone_Pattern->Duration = strtoul(pValue,&err,10);
                }
                else if( strstr(ParamName, "NextEntryID"))
                {
                    pVoiceProfile_Tone_Pattern->NextEntryID = strtoul(pValue,&err,10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "ButtonMap"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.ButtonMap.
            TELCOVOICEMGR_DML_BUTTONMAP* pVoiceProfile_ButtonMap = &(pVoiceProf->ButtonMapObj);

            if( strstr(ParamName, "NumberOfButtons"))
            {
                pVoiceProfile_ButtonMap->NumberOfButtons = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "Button"))
            {
                //VoiceService.{i}.VoiceProfile.{i}.ButtonMap.Button.{i}.

                if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceProfile_buttonMapButton_index(ParamName, DML_VOICESERVICE_VOICEPROF_BUTTONMAP_BUTTON, &hal_index))
                {
                    return ANSC_STATUS_FAILURE;
                }
                // index
                Button_index = hal_index - 1;

                DML_BUTTON_LIST_T* pVoiceProfile_ButtonMap_Button_List = &(pVoiceProfile_ButtonMap->ButtonList);

                DML_BUTTON_CTRL_T* pVoiceProfile_ButtonMap_Button_Data = pVoiceProfile_ButtonMap_Button_List->pdata[Button_index];

                if(pVoiceProfile_ButtonMap_Button_Data == NULL)
                {
                    //create new 
                    if( TelcoVoiceMgrDmlAddVoiceProfileButtonMapButton(pVoiceProfile_ButtonMap_Button_List, Button_index) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }
                    pVoiceProfile_ButtonMap_Button_Data = pVoiceProfile_ButtonMap_Button_List->pdata[Button_index];
                    if(pVoiceProfile_ButtonMap_Button_Data == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                TELCOVOICEMGR_DML_BUTTON* pVoiceProfile_ButtonMap_Button = &(pVoiceProfile_ButtonMap_Button_Data->dml);

                pVoiceProfile_ButtonMap_Button->InstanceNumber = hal_index;

                if( strstr(ParamName, "ButtonName"))
                {
                    strncpy(pVoiceProfile_ButtonMap_Button->ButtonName, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "FacilityAction"))
                {
                    strncpy(pVoiceProfile_ButtonMap_Button->FacilityAction, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "FacilityActionArgument"))
                {
                    strncpy(pVoiceProfile_ButtonMap_Button->FacilityActionArgument, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "QuickDialNumber"))
                {
                    strncpy(pVoiceProfile_ButtonMap_Button->QuickDialNumber, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "ButtonMessage"))
                {
                    strncpy(pVoiceProfile_ButtonMap_Button->ButtonMessage, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "UserAccess"))
                {
                    if(strstr(pValue, "true") || strstr(pValue, "1"))
                    {
                        pVoiceProfile_ButtonMap_Button->UserAccess = true;
                    }
                    else
                    {
                        pVoiceProfile_ButtonMap_Button->UserAccess = false;
                    }
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "FaxT38"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.FaxT38.
            TELCOVOICEMGR_DML_FAXT38* pVoiceProfile_FaxT38 = &(pVoiceProf->Fax38Obj);

            if( strstr(ParamName, "Enable"))
            {
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pVoiceProfile_FaxT38->Enable = true;
                }
                else
                {
                    pVoiceProfile_FaxT38->Enable = false;
                }
            }
            else if( strstr(ParamName, "BitRate"))
            {
                pVoiceProfile_FaxT38->BitRate = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "HighSpeedPacketRate"))
            {
                pVoiceProfile_FaxT38->HighSpeedPacketRate = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "HighSpeedRedundancy"))
            {
                pVoiceProfile_FaxT38->HighSpeedRedundancy = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "LowSpeedRedundancy"))
            {
                pVoiceProfile_FaxT38->LowSpeedRedundancy = strtoul(pValue,&err,10);
            }
            else if( strstr(ParamName, "TCFMethod"))
            {
                if(strstr(pValue, "Local"))
                {
                    pVoiceProfile_FaxT38->TCFMethod = TCFMETHOD_LOCAL;
                }
                else if( strstr(ParamName, "Network"))
                {
                    pVoiceProfile_FaxT38->TCFMethod = TCFMETHOD_NETWORK;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "Enable"))
        {
            strncpy(pVoiceProf->Enable, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Reset"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProf->Reset = true;
            }
            else
            {
                pVoiceProf->Reset = false;
            }
        }
        else if( strstr(ParamName, "NumberOfLines"))
        {
            pVoiceProf->NumberOfLines = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "Name"))
        {
            strncpy(pVoiceProf->Name, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SignalingProtocol"))
        {
            strncpy(pVoiceProf->SignalingProtocol, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "MaxSessions"))
        {
            pVoiceProf->MaxSessions = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "DTMFMethodG711"))
        {
            strncpy(pVoiceProf->DTMFMethodG711, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "DTMFMethod"))
        {
            strncpy(pVoiceProf->DTMFMethod, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Region"))
        {
            strncpy(pVoiceProf->Region, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "X_RDK-Central_COM_DigitMap"))
        {
            strncpy(pVoiceProf->X_RDK_DigitMap, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "X_RDK-Central_COM_EmergencyDigitMap"))
        {
            strncpy(pVoiceProf->EmergencyDigitMap, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "X_RDK-Central_COM_SDigitTimer"))
        {
            pVoiceProf->SDigitTimer = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "X_RDK-Central_COM_ZDigitTimer"))
        {
            pVoiceProf->ZDigitTimer = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "DigitMapEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProf->DigitMapEnable = true;
            }
            else
            {
                pVoiceProf->DigitMapEnable = false;
            }
        }
        else if( strstr(ParamName, "DigitMap"))
        {
            strncpy(pVoiceProf->DigitMap, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "STUNEnable"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProf->STUNEnable = true;
            }
            else
            {
                pVoiceProf->STUNEnable = false;
            }
        }
        else if( strstr(ParamName, "STUNServer"))
        {
            strncpy(pVoiceProf->STUNServer, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "NonVoiceBandwidthReservedUpstream"))
        {
            pVoiceProf->NonVoiceBandwidthReservedUpstream = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "NonVoiceBandwidthReservedDownstream"))
        {
            pVoiceProf->NonVoiceBandwidthReservedDownstream = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "PSTNFailOver"))
        {
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pVoiceProf->PSTNFailOver = true;
            }
            else
            {
                pVoiceProf->PSTNFailOver = false;
            }
        }
        else if( strstr(ParamName, "FaxPassThrough"))
        {
            if(strstr(pValue, "Disable"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_FAXPASSTHROUGH_DISABLE;
            }
            else if(strstr(pValue, "Auto"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_FAXPASSTHROUGH_AUTO;
            }
            else if(strstr(pValue, "Force"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_FAXPASSTHROUGH_FORCE;
            }
        }
        else if( strstr(ParamName, "ModemPassThrough"))
        {
            if(strstr(pValue, "Disable"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_MODEMPASSTHROUGH_DISABLE;
            }
            else if(strstr(pValue, "Auto"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_MODEMPASSTHROUGH_AUTO;
            }
            else if(strstr(pValue, "Force"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_MODEMPASSTHROUGH_FORCE;
            }
        }
    }
    pVoiceProfile->updated = true;
    return ANSC_STATUS_SUCCESS;
}
