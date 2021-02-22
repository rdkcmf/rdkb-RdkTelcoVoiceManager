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

#include "telcovoicemgr_dml_hal_param_v2.h"

ANSC_STATUS telcovoicemgr_hal_get_first_index(char* ParamName, char* Pattern, int* hal_index)
{
    if(ParamName == NULL || Pattern == NULL || hal_index == NULL) 
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter\n",__FUNCTION__,__LINE__));
        return ANSC_STATUS_FAILURE;
    }
    sscanf(ParamName, Pattern, hal_index);

    if( hal_index <= 0 )
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS telcovoicemgr_hal_get_second_index(char* ParamName, char* Pattern,  int* hal_index)
{
    int tmp_index1;

    if(ParamName == NULL || Pattern == NULL || hal_index == NULL) 
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter\n",__FUNCTION__,__LINE__));
        return ANSC_STATUS_FAILURE;
    }

    sscanf(ParamName, Pattern, &tmp_index1, hal_index);

    if( *hal_index <= 0 || tmp_index1 <= 0 )
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS telcovoicemgr_hal_get_third_index(char* ParamName, char* Pattern, int* hal_index)
{
    int tmp_index1, tmp_index2;

    if(ParamName == NULL || Pattern == NULL || hal_index == NULL) 
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter\n",__FUNCTION__,__LINE__));
        return ANSC_STATUS_FAILURE;
    }

    sscanf(ParamName, Pattern, &tmp_index1, &tmp_index2, hal_index);

    if( *hal_index <= 0 || tmp_index1 <= 0 || tmp_index2 <= 0 )
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS telcovoicemgr_hal_get_fourth_index(char* ParamName, char* Pattern, int* hal_index)
{
    int tmp_index1, tmp_index2, tmp_index3;

    if(ParamName == NULL || Pattern == NULL || hal_index == NULL) 
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter\n",__FUNCTION__,__LINE__));
        return ANSC_STATUS_FAILURE;
    }

    sscanf(ParamName, Pattern, &tmp_index1, &tmp_index2, &tmp_index3, hal_index);

    if( *hal_index <= 0 || tmp_index1 <= 0 || tmp_index2 <= 0 || tmp_index3 <= 0 )
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

PTELCOVOICEMGR_DML_VOICESERVICE getVoiceService(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList,char *ParamName)
{
    int voiceservice_index = 0;
    int hal_index = 0;

    //VoiceService.{i}.
    if( (ParamName == NULL) || (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_index(ParamName, DML_VOICESERVICE, &hal_index)) ||
        ( hal_index <= 0 ))
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return NULL;
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
            return NULL;
        }

        pVoiceServiceData = pVoiceServiceList->pdata[voiceservice_index];

        if(pVoiceServiceData == NULL)
        {
            return NULL;
        }

    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = &(pVoiceServiceData->dml);

    pVoiceService->InstanceNumber = hal_index;

    return pVoiceService;
}


ANSC_STATUS Map_hal_dml_voiceService(DML_VOICE_SERVICE_LIST_T* pVoiceServiceList, char* ParamName, char* pValue)
{
    ANSC_STATUS retStatus = ANSC_STATUS_FAILURE;
    int voiceservice_index = 0;
    int hal_index = 0;
    char *err;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    //VoiceService.{i}.
    if( ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_index(ParamName, DML_VOICESERVICE, &hal_index))
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
    else if( strstr(ParamName, "X_RDK_FactoryReset"))
    {
        // Do nothing   
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
    else if( strstr(ParamName, "X_RDK_Enable"))
    {
        // Do nothing
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
        if( strstr(ParamName, "LogServer"))
        {
           strncpy(pVoiceService->X_RDK_DebugObj.LogServer, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "LogServerPort"))
        {
           pVoiceService->X_RDK_DebugObj.LogServerPort = strtoul(pValue,&err,10);
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
        else
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
        }
    }
    else if(strstr(ParamName,"capabilities"))
    {
        //VoiceService.{i}.capabilities{i}.
        retStatus = Map_hal_dml_capabilities(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"reservePorts"))
    {
        //VoiceService.{i}.reservePorts{i}.
        retStatus = Map_hal_dml_reservePorts(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"ISDN"))
    {
        //VoiceService.{i}.ISDN{i}.
        retStatus = Map_hal_dml_ISDN(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"POTS"))
    {
        //VoiceService.{i}.POTS{i}.
        retStatus = Map_hal_dml_POTS(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"DECT"))
    {
        //VoiceService.{i}.DECT{i}.
        retStatus = Map_hal_dml_DECT(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"SIP"))
    {
        //VoiceService.{i}.SIP{i}.
        retStatus = Map_hal_dml_SIP(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"MGCP"))
    {
        //VoiceService.{i}.MGCP{i}.
        retStatus = Map_hal_dml_MGCP(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
        else if(strstr(ParamName,"H323"))
    {
        //VoiceService.{i}.H323{i}.
        retStatus = Map_hal_dml_H323(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"Trunk"))
    {
        //VoiceService.{i}.Trunk{i}.
        retStatus = Map_hal_dml_Trunk(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"CallControl"))
    {
        //VoiceService.{i}.CallControl{i}.
        retStatus = Map_hal_dml_CallControl(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"Interwork"))
    {
        //VoiceService.{i}.Interwork{i}.
        retStatus = Map_hal_dml_Interwork(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"CallLog"))
    {
        //VoiceService.{i}.CallLog{i}.
        retStatus = Map_hal_dml_CallLog(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"VoipProfile"))
    {
        //VoiceService.{i}.VoipProfile{i}.
        retStatus = Map_hal_dml_VoipProfile(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"codecProfile"))
    {
        //VoiceService.{i}.codecProfile{i}.
        retStatus = Map_hal_dml_codecProfile(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"Tone"))
    {
        //VoiceService.{i}.Tone{i}.
        retStatus = Map_hal_dml_Tone(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"Terminal"))
    {
        //VoiceService.{i}.Terminal{i}.
        retStatus = Map_hal_dml_Terminal(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if(strstr(ParamName,"clockSync"))
    {
        //VoiceService.{i}.clockSync{i}.
        retStatus = Map_hal_dml_clockSync(pVoiceServiceList, ParamName, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: \nMapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,ParamName));
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


ANSC_STATUS Map_hal_dml_capabilities(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_VOICESERVICE_CAPABILITIES pCapabilities = &(pVoiceService->Capabilities);

    if( strstr(ParamName, "MaxLineCount") )
    {
        //VoiceService.{i}.Capabilities.MaxLineCount
        pCapabilities->MaxLineCount = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "MaxExtensionCount") )
    {
        //VoiceService.{i}.Capabilities.MaxExtensionCount
        pCapabilities->MaxExtensionCount = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "MaxCallLogCount") )
    {
        //VoiceService.{i}.Capabilities.MaxCallLogCount
        pCapabilities->MaxCallLogCount = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "MaxSessionsPerLine") )
    {
        //VoiceService.{i}.Capabilities.MaxSessionsPerLine
        pCapabilities->MaxSessionsPerLine = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "MaxSessionsPerExtension") )
    {
        //VoiceService.{i}.Capabilities.MaxSessionsPerExtension
        pCapabilities->MaxSessionsPerExtension = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "MaxSessionCount") )
    {
        //VoiceService.{i}.Capabilities.MaxSessionCount
        pCapabilities->MaxSessionCount = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "NetworkConnectionModes") )
    {
        //VoiceService.{i}.Capabilities.NetworkConnectionModes
        strncpy(pCapabilities->NetworkConnectionModes, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "UserConnectionModes") )
    {
        //VoiceService.{i}.Capabilities.UserConnectionModes
        strncpy(pCapabilities->UserConnectionModes, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "ToneFileFormats") )
    {
        //VoiceService.{i}.Capabilities.ToneFileFormats
        if (strstr(pValue,"G.711MuLaw"))
        {
            pCapabilities->ToneFileFormats = TONE_FILEFORMAT_G_711MuLaw;
        }
        else if (strstr(pValue,"G.711ALaw"))
        {
            pCapabilities->ToneFileFormats = TONE_FILEFORMAT_G_711ALaw;
        }
        else if (strstr(pValue,"G.729"))
        {
            pCapabilities->ToneFileFormats = TONE_FILEFORMAT_G_729;
        }
        else if (strstr(pValue,"MP3"))
        {
            pCapabilities->ToneFileFormats = TONE_FILEFORMAT_MP3;
        }
        else if (strstr(pValue,"WAV"))
        {
            pCapabilities->ToneFileFormats = TONE_FILEFORMAT_WAV;
        }
        else if (strstr(pValue,"AMR"))
        {
            pCapabilities->ToneFileFormats = TONE_FILEFORMAT_AMR;
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
        }
    }
    else if( strstr(ParamName, "RingFileFormats") )
    {
        //VoiceService.{i}.Capabilities.RingFileFormats
        if (strstr(pValue,"MIDI"))
        {
            pCapabilities->RingFileFormats = RING_FILEFORMAT_MIDI;
        }
        else if (strstr(pValue,"SMAF"))
        {
            pCapabilities->RingFileFormats = RING_FILEFORMAT_SMAF_MMF;
        }
        else if (strstr(pValue,"RTTTL"))
        {
            pCapabilities->RingFileFormats = RING_FILEFORMAT_RTTTL;
        }
        else if (strstr(pValue,"MP3"))
        {
            pCapabilities->RingFileFormats = RING_FILEFORMAT_MP3;
        }
        else if (strstr(pValue,"WAV"))
        {
            pCapabilities->RingFileFormats = RING_FILEFORMAT_WAV;
        }
        else if (strstr(pValue,"AMR"))
        {
            pCapabilities->RingFileFormats = RING_FILEFORMAT_AMR;
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
        }
    }
    else if( strstr(ParamName, "FacilityActions") )
    {
        //VoiceService.{i}.Capabilities.FacilityActions
        if (strstr(pValue,"AA_REGISTER"))
        {
            pCapabilities->FacilityActions = AA_REGISTER;
        }
        else if (strstr(pValue,"AA_ERASE"))
        {
            pCapabilities->FacilityActions = AA_ERASE;
        }
        else if (strstr(pValue,"AA_INTERROGATE"))
        {
            pCapabilities->FacilityActions = AA_INTERROGATE;
        }
        else if (strstr(pValue,"CA_ACTIVATE"))
        {
            pCapabilities->FacilityActions = CA_ACTIVATE;
        }
        else if (strstr(pValue,"CCBS_ACTIVATE"))
        {
            pCapabilities->FacilityActions = CCBS_ACTIVATE;
        }
        else if (strstr(pValue,"CCBS_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = CCBS_DEACTIVATE;
        }
        else if (strstr(pValue,"CCBS_INTERROGATE"))
        {
            pCapabilities->FacilityActions = CCBS_INTERROGATE;
        }
        else if (strstr(pValue,"CCNR_ACTIVATE"))
        {
            pCapabilities->FacilityActions = CCNR_ACTIVATE;
        }
        else if (strstr(pValue,"CCNR_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = CCNR_DEACTIVATE;
        }
        else if (strstr(pValue,"CCNR_INTERROGATE"))
        {
            pCapabilities->FacilityActions = CCNR_INTERROGATE;
        }
        else if (strstr(pValue,"CFB_REGISTER"))
        {
            pCapabilities->FacilityActions = CFB_REGISTER;
        }
        else if (strstr(pValue,"CFB_ACTIVATE"))
        {
            pCapabilities->FacilityActions = CFB_ACTIVATE;
        }
        else if (strstr(pValue,"CFB_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = CFB_DEACTIVATE;
        }
        else if (strstr(pValue,"CFB_ERASE"))
        {
            pCapabilities->FacilityActions = CFB_ERASE;
        }
        else if (strstr(pValue,"CFB_INTERROGATE"))
        {
            pCapabilities->FacilityActions = CFB_INTERROGATE;
        }
        else if (strstr(pValue,"CFNR_REGISTER"))
        {
            pCapabilities->FacilityActions = CFNR_REGISTER;
        }
        else if (strstr(pValue,"CFNR_ACTIVATE"))
        {
            pCapabilities->FacilityActions = CFNR_ACTIVATE;
        }
        else if (strstr(pValue,"CFNR_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = CFNR_DEACTIVATE;
        }
        else if (strstr(pValue,"CFNR_ERASE"))
        {
            pCapabilities->FacilityActions = CFNR_ERASE;
        }
        else if (strstr(pValue,"CFNR_INTERROGATE"))
        {
            pCapabilities->FacilityActions = CFNR_INTERROGATE;
        }
        else if (strstr(pValue,"CFNR_TIMER"))
        {
            pCapabilities->FacilityActions = CFNR_TIMER;
        }
        else if (strstr(pValue,"CFT_ACTIVATE"))
        {
            pCapabilities->FacilityActions = CFT_ACTIVATE;
        }
        else if (strstr(pValue,"CFT_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = CFT_DEACTIVATE;
        }
        else if (strstr(pValue,"CFT_INTERROGATE"))
        {
            pCapabilities->FacilityActions = CFT_INTERROGATE;
        }
        else if (strstr(pValue,"CFU_REGISTER"))
        {
            pCapabilities->FacilityActions = CFU_REGISTER;
        }
        else if (strstr(pValue,"CFU_ACTIVATE"))
        {
            pCapabilities->FacilityActions = CFU_ACTIVATE;
        }
        else if (strstr(pValue,"CFU_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = CFU_DEACTIVATE;
        }
        else if (strstr(pValue,"CFU_ERASE"))
        {
            pCapabilities->FacilityActions = CFU_ERASE;
        }
        else if (strstr(pValue,"CFU_INTERROGATE"))
        {
            pCapabilities->FacilityActions = CFU_INTERROGATE;
        }
        else if (strstr(pValue,"CLIR_ACTIVATE"))
        {
            pCapabilities->FacilityActions = CLIR_ACTIVATE;
        }
        else if (strstr(pValue,"CLIR_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = CLIR_DEACTIVATE;
        }
        else if (strstr(pValue,"CLIR_INTERROGATE"))
        {
            pCapabilities->FacilityActions = CLIR_INTERROGATE;
        }
        else if (strstr(pValue,"CP_INVOKE"))
        {
            pCapabilities->FacilityActions = CP_INVOKE;
        }
        else if (strstr(pValue,"CW_ACTIVATE"))
        {
            pCapabilities->FacilityActions = CW_ACTIVATE;
        }
        else if (strstr(pValue,"CW_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = CW_DEACTIVATE;
        }
        else if (strstr(pValue,"CW_INVOKE"))
        {
            pCapabilities->FacilityActions = CW_INVOKE;
        }
        else if (strstr(pValue,"DND_ACTIVATE"))
        {
            pCapabilities->FacilityActions = DND_ACTIVATE;
        }
        else if (strstr(pValue,"DND_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = DND_DEACTIVATE;
        }
        else if (strstr(pValue,"DND_INTERROGATE"))
        {
            pCapabilities->FacilityActions = DND_INTERROGATE;
        }
        else if (strstr(pValue,"EXT_INVOKE"))
        {
            pCapabilities->FacilityActions = EXT_INVOKE;
        }
        else if (strstr(pValue,"LINE_INVOKE"))
        {
            pCapabilities->FacilityActions = LINE_INVOKE;
        }
        else if (strstr(pValue,"MAILBOX_INVOKE"))
        {
            pCapabilities->FacilityActions = MAILBOX_INVOKE;
        }
        else if (strstr(pValue,"OCB_ACTIVATE"))
        {
            pCapabilities->FacilityActions = OCB_ACTIVATE;
        }
        else if (strstr(pValue,"OCB_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = OCB_DEACTIVATE;
        }
        else if (strstr(pValue,"OCB_INTERROGATE"))
        {
            pCapabilities->FacilityActions = OCB_INTERROGATE;
        }
        else if (strstr(pValue,"PSO_ACTIVATE"))
        {
            pCapabilities->FacilityActions = PSO_ACTIVATE;
        }
        else if (strstr(pValue,"PW_SET"))
        {
            pCapabilities->FacilityActions = PW_SET;
        }
        else if (strstr(pValue,"SCF_ACTIVATE"))
        {
            pCapabilities->FacilityActions = SCF_ACTIVATE;
        }
        else if (strstr(pValue,"SCF_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = SCF_DEACTIVATE;
        }
        else if (strstr(pValue,"SCF_INTERROGATE"))
        {
            pCapabilities->FacilityActions = SCF_INTERROGATE;
        }
        else if (strstr(pValue,"SCREJ_ACTIVATE"))
        {
            pCapabilities->FacilityActions = SCREJ_ACTIVATE;
        }
        else if (strstr(pValue,"SCREJ_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = SCREJ_DEACTIVATE;
        }
        else if (strstr(pValue,"SCREJ_INTERROGATE"))
        {
            pCapabilities->FacilityActions = SCREJ_INTERROGATE;
        }
        else if (strstr(pValue,"SR_ACTIVATE"))
        {
            pCapabilities->FacilityActions = SR_ACTIVATE;
        }
        else if (strstr(pValue,"SR_DEACTIVATE"))
        {
            pCapabilities->FacilityActions = SR_DEACTIVATE;
        }
        else if (strstr(pValue,"SR_INTERROGATE"))
        {
            pCapabilities->FacilityActions = SR_INTERROGATE;
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
        }
    }
    else if( strstr(ParamName, "SIP") )
    {
        //VoiceService.{i}.Capabilities.SIP.
        if( strstr(ParamName, "Client") )
        {
            PDML_VOICESERVICE_CAPABILITIES_SIP_CLIENT pCapabilitiesSipClient = &(pCapabilities->SIP.Client_Obj);
            //VoiceService.{i}.Capabilities.SIP.Client.
            if( strstr(ParamName, "Extensions") )
            {
                //VoiceService.{i}.Capabilities.SIP.Client.Extensions
                strncpy(pCapabilitiesSipClient->Extensions, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "URISchemes") )
            {
                //VoiceService.{i}.Capabilities.SIP.Client.URISchemes
                strncpy(pCapabilitiesSipClient->URISchemes, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "EventTypes") )
            {
                //VoiceService.{i}.Capabilities.SIP.Client.EventTypes
                strncpy(pCapabilitiesSipClient->EventTypes, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "TLSAuthenticationProtocols") )
            {
                //VoiceService.{i}.Capabilities.SIP.Client.TLSAuthenticationProtocols
                if (strstr(pValue,"Null"))
                {
                    pCapabilitiesSipClient->TLSAuthenticationProtocols = TLS_AUTH_PROTOCOL_NULL;
                }
                else if (strstr(pValue,"MD5"))
                {
                    pCapabilitiesSipClient->TLSAuthenticationProtocols = TLS_AUTH_PROTOCOL_MD5;
                }
                else if (strstr(pValue,"SHA-1"))
                {
                    pCapabilitiesSipClient->TLSAuthenticationProtocols = TLS_AUTH_PROTOCOL_SHA_1;
                }
                else if (strstr(pValue,"SHA-2"))
                {
                    pCapabilitiesSipClient->TLSAuthenticationProtocols = TLS_AUTH_PROTOCOL_SHA_2;
                }
                else if (strstr(pValue,"AEAD"))
                {
                    pCapabilitiesSipClient->TLSAuthenticationProtocols = TLS_AUTH_PROTOCOL_AEAD;
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                }
            }
            else if( strstr(ParamName, "TLSAuthenticationKeySizes") )
            {
                //VoiceService.{i}.Capabilities.SIP.Client.TLSAuthenticationKeySizes
                strncpy(pCapabilitiesSipClient->TLSAuthenticationKeySizes, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "TLSEncryptionProtocols") )
            {
                //VoiceService.{i}.Capabilities.SIP.Client.TLSEncryptionProtocols
                //strncpy(pCapabilitiesSipClient->TLSEncryptionProtocols, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "TLSEncryptionKeySizes") )
            {
                //VoiceService.{i}.Capabilities.SIP.Client.TLSEncryptionKeySizes
                strncpy(pCapabilitiesSipClient->TLSEncryptionKeySizes, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "TLSKeyExchangeProtocols") )
            {
                //VoiceService.{i}.Capabilities.SIP.Client.TLSKeyExchangeProtocols
                //strncpy(pCapabilitiesSipClient->TLSKeyExchangeProtocols, pValue,strlen(pValue)+1);

            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "Registrar") )
        {
            PDML_VOICESERVICE_CAPABILITIES_SIP_REGISTRAR pCapabilitiesSipReg = &(pCapabilities->SIP.Registrar_Obj);
            //VoiceService.{i}.Capabilities.SIP.Registrar.
            if( strstr(ParamName, "Extensions") )
            {
                //VoiceService.{i}.Capabilities.SIP.Registrar.Extensions
                strncpy(pCapabilitiesSipReg->Extensions, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "URISchemes") )
            {
                //VoiceService.{i}.Capabilities.SIP.Registrar.URISchemes
                strncpy(pCapabilitiesSipReg->URISchemes, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "EventTypes") )
            {
                //VoiceService.{i}.Capabilities.SIP.Registrar.EventTypes
                strncpy(pCapabilitiesSipReg->EventTypes, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "TLSAuthenticationProtocols") )
            {
                //VoiceService.{i}.Capabilities.SIP.Registrar.TLSAuthenticationProtocols
                //strncpy(pCapabilitiesSipReg->TLSAuthenticationProtocols, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "TLSAuthenticationKeySizes") )
            {
                //VoiceService.{i}.Capabilities.SIP.Registrar.TLSAuthenticationKeySizes
                strncpy(pCapabilitiesSipReg->TLSAuthenticationKeySizes, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "TLSEncryptionProtocols") )
            {
                //VoiceService.{i}.Capabilities.SIP.Registrar.TLSEncryptionProtocols
                //strncpy(pCapabilitiesSipReg->TLSEncryptionProtocols, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "TLSEncryptionKeySizes") )
            {
                //VoiceService.{i}.Capabilities.SIP.Registrar.TLSEncryptionKeySizes
                strncpy(pCapabilitiesSipReg->TLSEncryptionKeySizes, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "TLSKeyExchangeProtocols") )
            {
                //VoiceService.{i}.Capabilities.SIP.Registrar.TLSKeyExchangeProtocols
                //strncpy(pCapabilitiesSipReg->TLSKeyExchangeProtocols, pValue,strlen(pValue)+1);

            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
    }
    else if( strstr(ParamName, "MGCP") )
    {
        PDML_VOICESERVICE_CAPABILITIES_MGCP pCapabilitiesMgcp = &(pCapabilities->MGCP);
        //VoiceService.{i}.Capabilities.MGCP.
        if( strstr(ParamName, "Extensions") )
        {
            //VoiceService.{i}.Capabilities.MGCP.Extensions
            strncpy(pCapabilitiesMgcp->Extensions, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "H323") )
    {
        PDML_VOICESERVICE_CAPABILITIES_H323 pCapabilitiesH323 = &(pCapabilities->H323);
        //VoiceService.{i}.Capabilities.H323.
        if( strstr(ParamName, "FastStart") )
        {
            //VoiceService.{i}.Capabilities.H323.FastStart
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCapabilitiesH323->FastStart = true;
            }
            else
            {
                 pCapabilitiesH323->FastStart = false;
            }
        }
        else if( strstr(ParamName, "H235AuthenticationMethods") )
        {
            //VoiceService.{i}.Capabilities.H323.H235AuthenticationMethods
            //strncpy(pCapabilitiesH323->H235AuthenticationMethods, pValue,strlen(pValue)+1);

        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "ISDN") )
    {
        PDML_VOICESERVICE_CAPABILITIES_ISDN pCapabilitiesIsdn = &(pCapabilities->ISDN);
        //VoiceService.{i}.Capabilities.ISDN.
        if( strstr(ParamName, "MSN") )
        {
            //VoiceService.{i}.Capabilities.ISDN.MSN
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCapabilitiesIsdn->MSN = true;
            }
            else
            {
                 pCapabilitiesIsdn->MSN = false;
            }
        }
        else if( strstr(ParamName, "DDI") )
        {
            //VoiceService.{i}.Capabilities.ISDN.DDI
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCapabilitiesIsdn->DDI = true;
            }
            else
            {
                 pCapabilitiesIsdn->DDI = false;
            }
        }
        else if( strstr(ParamName, "MCID") )
        {
            //VoiceService.{i}.Capabilities.ISDN.MCID
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCapabilitiesIsdn->MCID = true;
            }
            else
            {
                 pCapabilitiesIsdn->MCID = false;
            }
        }
        else if( strstr(ParamName, "MWI") )
        {
            //VoiceService.{i}.Capabilities.ISDN.MWI
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCapabilitiesIsdn->MWI = true;
            }
            else
            {
                 pCapabilitiesIsdn->MWI = false;
            }
        }
        else if( strstr(ParamName, "AoC") )
        {
            //VoiceService.{i}.Capabilities.ISDN.AoC
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCapabilitiesIsdn->AoC = true;
            }
            else
            {
                 pCapabilitiesIsdn->AoC = false;
            }
        }
        else if( strstr(ParamName, "ECT") )
        {
            //VoiceService.{i}.Capabilities.ISDN.ECT
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCapabilitiesIsdn->ECT = true;
            }
            else
            {
                 pCapabilitiesIsdn->ECT = false;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "POTS") )
    {
        PDML_VOICESERVICE_CAPABILITIES_POTS pCapabilitiesPots = &(pCapabilities->POTS);
        //VoiceService.{i}.Capabilities.POTS.
        if( strstr(ParamName, "DialType") )
        {
            //VoiceService.{i}.Capabilities.POTS.DialType
            //strncpy(pCapabilitiesPots->DialType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ClipGeneration") )
        {
            //VoiceService.{i}.Capabilities.POTS.ClipGeneration
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCapabilitiesPots->ClipGeneration = true;
            }
            else
            {
                 pCapabilitiesPots->ClipGeneration = false;
            }
        }
        else if( strstr(ParamName, "ChargingPulse") )
        {
            //VoiceService.{i}.Capabilities.POTS.ChargingPulse
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCapabilitiesPots->ChargingPulse = true;
            }
            else
            {
                 pCapabilitiesPots->ChargingPulse = false;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Codec") )
    {
        int hal_index = 0;
        int codec_index = 0;
        //VoiceService.{i}.Capabilities.Codec.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Capabilities_Codec_index(ParamName,DML_VOICESERVICE_CAPABILITIES_CODEC,&hal_index)) ||
        ( hal_index <= 0 ))
        {
            return ANSC_STATUS_FAILURE;
        }
        codec_index = hal_index - 1;

        PDML_VOICESERVICE_CAPABILITIES_CODEC_LIST_T pCapabilitiesCodecList = &(pCapabilities->Codec);

        PDML_VOICESERVICE_CAPABILITIES_CODEC_CTRL_T pCapabilitiesCodecData = pCapabilitiesCodecList->pdata[codec_index];

        if(pCapabilitiesCodecData == NULL)
        {
            TelcoVoiceMgrDmlAddCapabilitiesCodec(pCapabilitiesCodecList, codec_index);

            pCapabilitiesCodecData = pCapabilitiesCodecList->pdata[codec_index];

            if(pCapabilitiesCodecData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_VOICESERVICE_CAPABILITIES_CODEC pCapabilitiesCodec = &(pCapabilitiesCodecData->dml);

        pCapabilitiesCodec->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Codec") )
        {
            //VoiceService.{i}.Capabilities.Codec.{i}.Codec
            //strncpy(pCapabilitiesCodec->Codec, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "BitRate") )
        {
            //VoiceService.{i}.Capabilities.Codec.{i}.BitRate
            pCapabilitiesCodec->BitRate = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "PacketizationPeriod") )
        {
            //VoiceService.{i}.Capabilities.Codec.{i}.PacketizationPeriod
            strncpy(pCapabilitiesCodec->PacketizationPeriod, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SilenceSuppression") )
        {
            //VoiceService.{i}.Capabilities.Codec.{i}.SilenceSuppression
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                pCapabilitiesCodec->SilenceSuppression = true;
            }
            else
            {
                pCapabilitiesCodec->SilenceSuppression = false;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "QualityIndicator") )
    {
        PDML_VOICESERVICE_CAPABILITIES_QUALITYINDICATOR pCapabilitiesQualityInd = &(pCapabilities->QualityIndicator);
        //VoiceService.{i}.Capabilities.QualityIndicator.
        if( strstr(ParamName, "QIModelsSupported") )
        {
            //VoiceService.{i}.Capabilities.QualityIndicator.QIModelsSupported
            strncpy(pCapabilitiesQualityInd->QIModelsSupported, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "MaxQIValues") )
        {
            //VoiceService.{i}.Capabilities.QualityIndicator.MaxQIValues
            pCapabilitiesQualityInd->MaxQIValues = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "MaxWorstQIValues") )
        {
            //VoiceService.{i}.Capabilities.QualityIndicator.MaxWorstQIValues
            pCapabilitiesQualityInd->MaxWorstQIValues = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "MaxWorstQIValues") )
        {
            //VoiceService.{i}.Capabilities.QualityIndicator.MaxWorstQIValues
            pCapabilitiesQualityInd->MaxWorstQIValues = strtoul(pValue,&err, 10);
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

ANSC_STATUS Map_hal_dml_reservePorts(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_RESERVEDPORTS pReservedPorts = &(pVoiceService->ReservedPorts);

    if( strstr(ParamName, "WANPortRange") )
    {
        //VoiceService.{i}.ReservedPorts.WANPortRange
        strncpy(pReservedPorts->WANPortRange, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "LANPortRange") )
    {
        //VoiceService.{i}.ReservedPorts.LANPortRange
        strncpy(pReservedPorts->LANPortRange, pValue,strlen(pValue)+1);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, ParamName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_ISDN(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_ISDN pIsdn = &(pVoiceService->ISDN_obj);

    if( strstr(ParamName, "BRI") )
    {
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_ISDN_BRI_index(ParamName, DML_VOICESERVICE_ISDN_BRI, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_ISDN_BRI_LIST_T    pIsdnBRIList = &(pIsdn->BRI);

        PDML_ISDN_BRI_CTRL_T    pIsdnBRIData = pIsdnBRIList->pdata[hal_index - 1];

        if(pIsdnBRIData == NULL)
        {

            if( TelcoVoiceMgrDmlAddIsdnBRI(pIsdnBRIList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pIsdnBRIData = pIsdnBRIList->pdata[hal_index - 1];

            if(pIsdnBRIData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_ISDN_BRI pIsdnBri = &(pIsdnBRIData->dml);

        pIsdnBri->uInstanceNumber = hal_index;

        //VoiceService.{i}.ISDN.BRI.{i}.
        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pIsdnBri->Enable = true;
            }
            else
            {
                 pIsdnBri->Enable = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.Status
            //strncpy(pIsdnBri->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Name") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.Name
            strncpy(pIsdnBri->Name, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ToneEventProfile") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.ToneEventProfile
            strncpy(pIsdnBri->ToneEventProfile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ApplicationInterface") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.ApplicationInterface
            //strncpy(pIsdnBri->ApplicationInterface, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Protocol") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.Protocol
            //strncpy(pIsdnBri->Protocol, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ProtocolEmulation") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.ProtocolEmulation
            //strncpy(pIsdnBri->ProtocolEmulation, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "PermanentLayer2") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.PermanentLayer2
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pIsdnBri->PermanentLayer2 = true;
            }
            else
            {
                 pIsdnBri->PermanentLayer2 = false;
            }
        }
        else if( strstr(ParamName, "PermanentLayer1") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.PermanentLayer1
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pIsdnBri->PermanentLayer1 = true;
            }
            else
            {
                 pIsdnBri->PermanentLayer1 = false;
            }
        }
        else if( strstr(ParamName, "LapdDisconnectionTimeout") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.LapdDisconnectionTimeout
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pIsdnBri->LapdDisconnectionTimeout = true;
            }
            else
            {
                 pIsdnBri->LapdDisconnectionTimeout = false;
            }
        }
        else if( strstr(ParamName, "TEINegotiation") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.TEINegotiation
            //strncpy(pIsdnBri->TEINegotiation, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "StaticTEI") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.StaticTEI
            pIsdnBri->StaticTEI = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "LifeLineHold") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.LifeLineHold
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pIsdnBri->LifeLineHold = true;
            }
            else
            {
                 pIsdnBri->LifeLineHold = false;
            }
        }
        else if( strstr(ParamName, "PowerSourceOne") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.PowerSourceOne
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pIsdnBri->PowerSourceOne = true;
            }
            else
            {
                 pIsdnBri->PowerSourceOne = false;
            }
        }
        else if( strstr(ParamName, "MaxNumBChannels") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.MaxNumBChannels
            pIsdnBri->MaxNumBChannels = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "OutboundOnlyBChannels") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.OutboundOnlyBChannels
            strncpy(pIsdnBri->OutboundOnlyBChannels, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "InboundOnlyBChannels") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.InboundOnlyBChannels
            strncpy(pIsdnBri->InboundOnlyBChannels, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "BidirectionalBChannels") )
        {
            //VoiceService.{i}.ISDN.BRI.{i}.BidirectionalBChannels
            strncpy(pIsdnBri->BidirectionalBChannels, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "PRI") )
    {
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_ISDN_PRI_index(ParamName, DML_VOICESERVICE_ISDN_PRI, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_ISDN_PRI_LIST_T    pIsdnPRIList = &(pIsdn->PRI);

        PDML_ISDN_PRI_CTRL_T    pIsdnPRIData = pIsdnPRIList->pdata[hal_index - 1];

        if(pIsdnPRIData == NULL)
        {

            if( TelcoVoiceMgrDmlAddIsdnPRI(pIsdnPRIList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pIsdnPRIData = pIsdnPRIList->pdata[hal_index - 1];

            if(pIsdnPRIData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_ISDN_PRI pIsdnPri = &(pIsdnPRIData->dml);

        pIsdnPri->uInstanceNumber = hal_index;

        //VoiceService.{i}.ISDN.PRI.{i}.
        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pIsdnPri->Enable = true;
            }
            else
            {
                 pIsdnPri->Enable = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.Status
            //strncpy(pIsdnPri->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Name") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.Name
            strncpy(pIsdnPri->Name, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ToneEventProfile") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.ToneEventProfile
            strncpy(pIsdnPri->ToneEventProfile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ApplicationInterface") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.ApplicationInterface
            //strncpy(pIsdnPri->ApplicationInterface, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Protocol") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.Protocol
            //strncpy(pIsdnPri->Protocol, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ProtocolEmulation") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.ProtocolEmulation
            //strncpy(pIsdnPri->ProtocolEmulation, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "PermanentLayer2") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.PermanentLayer2
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pIsdnPri->PermanentLayer2 = true;
            }
            else
            {
                 pIsdnPri->PermanentLayer2 = false;
            }
        }
        else if( strstr(ParamName, "Linecode") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.Linecode
            //strncpy(pIsdnPri->LineCode, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ESF") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.ESF
            //strncpy(pIsdnPri->ESF, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "PhysicalInterfaceType") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.PhysicalInterfaceType
            //strncpy(pIsdnPri->PhysicalInterfaceType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "MaxNumBChannels") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.MaxNumBChannels
            pIsdnPri->MaxNumBChannels = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "OutboundOnlyBChannels") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.OutboundOnlyBChannels
            strncpy(pIsdnPri->OutboundOnlyBChannels, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "InboundOnlyBChannels") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.InboundOnlyBChannels
            strncpy(pIsdnPri->InboundOnlyBChannels, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "BidirectionalBChannels") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.BidirectionalBChannels
            strncpy(pIsdnPri->BidirectionalBChannels, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ClockMode") )
        {
            //VoiceService.{i}.ISDN.PRI.{i}.ClockMode
            //strncpy(pIsdnPri->ClockMode, pValue,strlen(pValue)+1);

        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS Map_hal_dml_POTS(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_POTS pPots = &(pVoiceService->POTS_obj);

    if( strstr(ParamName, "Region") )
    {
        //VoiceService.{i}.POTS.Region
        strncpy(pPots->Region, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "FXO") )
    {
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_POTS_FXO_index(ParamName, DML_VOICESERVICE_POTS_FXO, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_POTS_FXO_LIST_T    pPotsFXOList = &(pPots->FXO);

        PDML_POTS_FXO_CTRL_T    pPotsFXOData = pPotsFXOList->pdata[hal_index - 1];

        if(pPotsFXOData == NULL)
        {

            if( TelcoVoiceMgrDmlAddPotsFXO(pPotsFXOList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pPotsFXOData = pPotsFXOList->pdata[hal_index - 1];

            if(pPotsFXOData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_POTS_FXO pPotsFxo = &(pPotsFXOData->dml);

        pPotsFxo->uInstanceNumber = hal_index;

        //VoiceService.{i}.POTS.FXO.{i}.
        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pPotsFxo->Enable = true;
            }
            else
            {
                 pPotsFxo->Enable = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.Status
            //strncpy(pPotsFxo->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Name") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.Name
            strncpy(pPotsFxo->Name, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ToneEventProfile") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.ToneEventProfile
            strncpy(pPotsFxo->ToneEventProfile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SecondStepDialing") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.SecondStepDialing
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pPotsFxo->SecondStepDialing = true;
            }
            else
            {
                 pPotsFxo->SecondStepDialing = false;
            }
        }
        else if( strstr(ParamName, "TimeoutBeforeDialing") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.TimeoutBeforeDialing
            pPotsFxo->TimeoutBeforeDialing = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "RingingTimeout") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.RingingTimeout
            pPotsFxo->RingingTimeout = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "RingNumber") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.RingNumber
            pPotsFxo->RingNumber = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "OnHookMinDuration") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.OnHookMinDuration
            pPotsFxo->OnHookMinDuration = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "SignalingMode") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.SignalingMode
            //strncpy(pPotsFxo->SignalingMode, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "DTMFDialoutInterval") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.DTMFDialoutInterval
            pPotsFxo->DTMFDialoutInterval = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "CallerIdDetectionEnable") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.CallerIdDetectionEnable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pPotsFxo->CallerIdDetectionEnable = true;
            }
            else
            {
                 pPotsFxo->CallerIdDetectionEnable = false;
            }
        }
        else if( strstr(ParamName, "Active") )
        {
            //VoiceService.{i}.POTS.FXO.{i}.Active
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pPotsFxo->Active = true;
            }
            else
            {
                 pPotsFxo->Active = false;
            }
        }
        else if( strstr(ParamName, "DiagTests") )
        {
            PDML_POTS_FXO_DIAGTESTS pPotsFxoDiagTests = &(pPotsFxo->DiagTests);
            //VoiceService.{i}.POTS.FXO.{i}.DiagTests.
            if( strstr(ParamName, "DiagnosticsState") )
            {
                //VoiceService.{i}.POTS.FXO.{i}.DiagTests.DiagnosticsState
                //strncpy(pPotsFxoDiagTests->DiagnosticsState, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "TestSelector") )
            {
                //VoiceService.{i}.POTS.FXO.{i}.DiagTests.TestSelector
                //strncpy(pPotsFxoDiagTests->TestSelector, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "TestResult") )
            {
                //VoiceService.{i}.POTS.FXO.{i}.DiagTests.TestResult
                //strncpy(pPotsFxoDiagTests->TestResult, pValue,strlen(pValue)+1);

            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
               AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
               return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "FXS") )
    {
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_POTS_FXS_index(ParamName, DML_VOICESERVICE_POTS_FXS, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_POTS_FXS_LIST_T    pPotsFXSList = &(pPots->FXS);

        PDML_POTS_FXS_CTRL_T    pPotsFXSData = pPotsFXSList->pdata[hal_index - 1];

        if(pPotsFXSData == NULL)
        {

            if( TelcoVoiceMgrDmlAddPotsFXS(pPotsFXSList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pPotsFXSData = pPotsFXSList->pdata[hal_index - 1];

            if(pPotsFXSData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_POTS_FXS pPotsFxs = &(pPotsFXSData->dml);

        pPotsFxs->uInstanceNumber = hal_index;

        //VoiceService.{i}.POTS.FXS.{i}.
        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pPotsFxs->Enable = true;
            }
            else
            {
                 pPotsFxs->Enable = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.Status
            //strncpy(pPotsFxs->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Name") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.Name
            strncpy(pPotsFxs->Name, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ToneEventProfile") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.ToneEventProfile
            strncpy(pPotsFxs->ToneEventProfile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "FaxPassThrough") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.FaxPassThrough
            //strncpy(pPotsFxs->FaxPassThrough, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ModemPassThrough") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.ModemPassThrough
            //strncpy(pPotsFxs->ModemPassThrough, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "DialType") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.DialType
            //strncpy(pPotsFxs->DialType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ClipGeneration") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.ClipGeneration
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pPotsFxs->ClipGeneration = true;
            }
            else
            {
                 pPotsFxs->ClipGeneration = false;
            }
        }
        else if( strstr(ParamName, "ChargingPulse") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.ChargingPulse
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pPotsFxs->ChargingPulse = true;
            }
            else
            {
                 pPotsFxs->ChargingPulse = false;
            }
        }
        else if( strstr(ParamName, "Active") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.Active
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pPotsFxs->Active = true;
            }
            else
            {
                 pPotsFxs->Active = false;
            }
        }
        else if( strstr(ParamName, "TerminalType") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.TerminalType
            //strncpy(pPotsFxs->TerminalType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "VoiceProcessing") )
        {
            //VoiceService.{i}.POTS.FXS.{i}.VoiceProcessing.
            PDML_POTS_FXS_VOICEPROCESSING pPotsFxsVoiceProcess = &(pPotsFxs->VoiceProcessing);
            if( strstr(ParamName, "TransmitGain") )
            {
                //VoiceService.{i}.POTS.FXS.{i}.VoiceProcessing.TransmitGain
                pPotsFxsVoiceProcess->EchoCancellationTail = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "ReceiveGain") )
            {
                //VoiceService.{i}.POTS.FXS.{i}.VoiceProcessing.ReceiveGain
                pPotsFxsVoiceProcess->EchoCancellationTail = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "EchoCancellationEnable") )
            {
                //VoiceService.{i}.POTS.FXS.{i}.VoiceProcessing.EchoCancellationEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pPotsFxsVoiceProcess->EchoCancellationEnable = true;
                }
                else
                {
                     pPotsFxsVoiceProcess->EchoCancellationEnable = false;
                }
            }
            else if( strstr(ParamName, "EchoCancellationInUse") )
            {
                //VoiceService.{i}.POTS.FXS.{i}.VoiceProcessing.EchoCancellationInUse
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pPotsFxsVoiceProcess->EchoCancellationInUse = true;
                }
                else
                {
                     pPotsFxsVoiceProcess->EchoCancellationInUse = false;
                }
            }
            else if( strstr(ParamName, "EchoCancellationTail") )
            {
                //VoiceService.{i}.POTS.FXS.{i}.VoiceProcessing.EchoCancellationTail
                pPotsFxsVoiceProcess->EchoCancellationTail = strtoul(pValue,&err, 10);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "DiagTests") )
        {
            PDML_POTS_FXS_DIAGTESTS pPotsFxsDiagTests = &(pPotsFxs->DiagTests);
            //VoiceService.{i}.POTS.FXS.{i}.DiagTests.
            if( strstr(ParamName, "DiagnosticsState") )
            {
                //VoiceService.{i}.POTS.FXS.{i}.DiagTests.DiagnosticsState
                //strncpy(pPotsFxsDiagTests->DiagnosticsState, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "TestSelector") )
            {
                //VoiceService.{i}.POTS.FXS.{i}.DiagTests.TestSelector
                //strncpy(pPotsFxsDiagTests->TestSelector, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "TestResult") )
            {
                //VoiceService.{i}.POTS.FXS.{i}.DiagTests.TestResult
                //strncpy(pPotsFxsDiagTests->TestResult, pValue,strlen(pValue)+1);

            }
            else
            {
               AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
               return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Ringer") )
    {
        PDML_POTS_RINGER pPotsRinger = &(pPots->Ringer_Obj);
        //VoiceService.{i}.POTS.Ringer.
        if( strstr(ParamName, "Event") )
        {
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_POTS_Ringer_Event_index(ParamName, DML_VOICESERVICE_POTS_RINGER_EVT, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_POTS_RINGER_EVT_LIST_T   pPotsRingerEvtList = &(pPotsRinger->Event);

            PDML_POTS_RINGER_EVT_CTRL_T   pPotsRingerEvtData = pPotsRingerEvtList->pdata[hal_index - 1];

            if(pPotsRingerEvtData == NULL)
            {

                if( TelcoVoiceMgrDmlAddPotsRingerEvt(pPotsRingerEvtList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pPotsRingerEvtData = pPotsRingerEvtList->pdata[hal_index - 1];

                if(pPotsRingerEvtData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_POTS_RINGER_EVT pPotsRingerEvt = &(pPotsRingerEvtData->dml);

            pPotsRingerEvt->uInstanceNumber = hal_index;

            //VoiceService.{i}.POTS.Ringer.Event.{i}.
            if( strstr(ParamName, "Function") )
            {
                //VoiceService.{i}.POTS.Ringer.Event.{i}.Function
                //strncpy(pPotsRingerEvt->Function, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "Cadence") )
            {
                //VoiceService.{i}.POTS.Ringer.Event.{i}.Cadence
                strncpy(pPotsRingerEvt->Cadence, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
}
ANSC_STATUS Map_hal_dml_DECT(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_DECT pDect = &(pVoiceService->DECT_obj);

    if( strstr(ParamName, "Base") )
    {
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_DECT_Base_index(ParamName, DML_VOICESERVICE_DECT_BASE, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_DECT_BASE_LIST_T    pDectBaseList = &(pDect->Base);

        PDML_DECT_BASE_CTRL_T    pDectBaseData = pDectBaseList->pdata[hal_index - 1];

        if(pDectBaseData == NULL)
        {

            if( TelcoVoiceMgrDmlAddDectBase(pDectBaseList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pDectBaseData = pDectBaseList->pdata[hal_index - 1];

            if(pDectBaseData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_DECT_BASE pDectBase = &(pDectBaseData->dml);

        pDectBase->uInstanceNumber = hal_index;

        //VoiceService.{i}.DECT.Base.{i}.
        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.DECT.Base.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pDectBase->Enable = true;
            }
            else
            {
                 pDectBase->Enable = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.DECT.Base.{i}.Status
            //strncpy(pDectBase->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Name") )
        {
            //VoiceService.{i}.DECT.Base.{i}.Name
            //strncpy(pDectBase->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ToneEventProfile") )
        {
            //VoiceService.{i}.DECT.Base.{i}.ToneEventProfile
            strncpy(pDectBase->ToneEventProfile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Standard") )
        {
            //VoiceService.{i}.DECT.Base.{i}.Standard
            //strncpy(pDectBase->Standard, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "RFPI") )
        {
            //VoiceService.{i}.DECT.Base.{i}.RFPI
            strncpy(pDectBase->RFPI, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "MaxSupportedPP") )
        {
            //VoiceService.{i}.DECT.Base.{i}.MaxSupportedPP
            pDectBase->MaxSupportedPP = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "PIN") )
        {
            //VoiceService.{i}.DECT.Base.{i}.PIN
        }
        else if( strstr(ParamName, "RepeaterSupportEnabled") )
        {
            //VoiceService.{i}.DECT.Base.{i}.RepeaterSupportEnabled
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pDectBase->RepeaterSupportEnabled = true;
            }
            else
            {
                 pDectBase->RepeaterSupportEnabled = false;
            }
        }
        else if( strstr(ParamName, "NEMOEnable") )
        {
            //VoiceService.{i}.DECT.Base.{i}.NEMOEnable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pDectBase->NEMOEnable = true;
            }
            else
            {
                 pDectBase->NEMOEnable = false;
            }
        }
        else if( strstr(ParamName, "SubscriptionEnable") )
        {
            //VoiceService.{i}.DECT.Base.{i}.SubscriptionEnable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pDectBase->SubscriptionEnable = true;
            }
            else
            {
                 pDectBase->SubscriptionEnable = false;
            }
        }
        else if( strstr(ParamName, "CipheringEnable") )
        {
            //VoiceService.{i}.DECT.Base.{i}.CipheringEnable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pDectBase->CipheringEnable = true;
            }
            else
            {
                 pDectBase->CipheringEnable = false;
            }
        }
        else if( strstr(ParamName, "EncryptionType") )
        {
            //VoiceService.{i}.DECT.Base.{i}.EncryptionType
            //strncpy(pDectBase->EncryptionType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "RFPowerControl") )
        {
            //VoiceService.{i}.DECT.Base.{i}.RFPowerControl
            //strncpy(pDectBase->RFPowerControl, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "FirmwareVersion") )
        {
            //VoiceService.{i}.DECT.Base.{i}.FirmwareVersion
            strncpy(pDectBase->FirmwareVersion, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "EepromVersion") )
        {
            //VoiceService.{i}.DECT.Base.{i}.EepromVersion
            strncpy(pDectBase->EepromVersion, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "HardwareVersion") )
        {
            //VoiceService.{i}.DECT.Base.{i}.HardwareVersion
            strncpy(pDectBase->HardwareVersion, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Stats") )
        {
            PDML_DECT_BASE_STATS pDectBaseStats = &(pDectBase->Stats);
            //VoiceService.{i}.DECT.Base.{i}.Stats.
            if( strstr(ParamName, "Handovers") )
            {
                //VoiceService.{i}.DECT.Base.{i}.Stats.Handovers
                pDectBaseStats->Handovers = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "HandoverFailures") )
            {
                //VoiceService.{i}.DECT.Base.{i}.Stats.HandoverFailures
                pDectBaseStats->HandoverFailures = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "ControlFieldErrors") )
            {
                //VoiceService.{i}.DECT.Base.{i}.Stats.ControlFieldErrors
                pDectBaseStats->ControlFieldErrors = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "PayloadFieldErrors") )
            {
                //VoiceService.{i}.DECT.Base.{i}.Stats.PayloadFieldErrors
                pDectBaseStats->PayloadFieldErrors = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "SyncFailures") )
            {
                //VoiceService.{i}.DECT.Base.{i}.Stats.SyncFailures
                pDectBaseStats->SyncFailures = strtoul(pValue,&err, 10);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Portable") )
    {
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_DECT_Portable_index(ParamName, DML_VOICESERVICE_DECT_PORTABLE, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_DECT_PORTABLE_LIST_T    pDectPortableList = &(pDect->Portable);

        PDML_DECT_PORTABLE_CTRL_T    pDectPortableData = pDectPortableList->pdata[hal_index - 1];

        if(pDectPortableData == NULL)
        {

            if( TelcoVoiceMgrDmlAddDectPortable(pDectPortableList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pDectPortableData = pDectPortableList->pdata[hal_index - 1];

            if(pDectPortableData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_DECT_PORTABLE pDectPortable = &(pDectPortableData->dml);

        pDectPortable->uInstanceNumber = hal_index;

        //VoiceService.{i}.DECT.Portable.{i}.
        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pDectPortable->Enable = true;
            }
            else
            {
                 pDectPortable->Enable = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.Status
            //strncpy(pDectPortable->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "CodecList") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.CodecList
            strncpy(pDectPortable->CodecList, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "RegistrationStatus") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.RegistrationStatus
            //strncpy(pDectPortable->RegistrationStatus, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "IPUI") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.IPUI
            strncpy(pDectPortable->IPUI, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "IPUILength") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.IPUILength
            pDectPortable->IPUILength = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "IPEI") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.IPEI
            strncpy(pDectPortable->IPEI, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "PARK") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.PARK
            strncpy(pDectPortable->PARK, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "BaseAttachedTo") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.BaseAttachedTo
            strncpy(pDectPortable->BaseAttachedTo, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "PortableType") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.PortableType
            //strncpy(pDectPortable->PortableType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "SubscriptionTime") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.SubscriptionTime
            strncpy(pDectPortable->SubscriptionTime, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Control") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.Control
            //strncpy(pDectPortable->Control, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "HardwareVersion") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.HardwareVersion
            strncpy(pDectPortable->HardwareVersion, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SoftwareVersion") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.SoftwareVersion
            strncpy(pDectPortable->SoftwareVersion, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SoftwareUpgrade") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.SoftwareUpgrade
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pDectPortable->SoftwareUpgrade = true;
            }
            else
            {
                 pDectPortable->SoftwareUpgrade = false;
            }
        }
        else if( strstr(ParamName, "LastUpdateDateTime") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.LastUpdateDateTime
            strncpy(pDectPortable->LastUpdateDateTime, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "OperatorName") )
        {
            //VoiceService.{i}.DECT.Portable.{i}.OperatorName
            strncpy(pDectPortable->OperatorName, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
    }
}
ANSC_STATUS Map_hal_dml_SIP(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_SIP pSip = &(pVoiceService->SIP_obj);

    if( strstr(ParamName, "Client") )
    {
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Client_index(ParamName, DML_VOICESERVICE_SIP_CLIENT, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_SIP_CLIENT_LIST_T    pSipCliList = &(pSip->Client);

        PDML_SIP_CLIENT_CTRL_T    pSipCliData = pSipCliList->pdata[hal_index - 1];

        if(pSipCliData == NULL)
        {

            if( TelcoVoiceMgrDmlAddSipClient(pSipCliList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pSipCliData = pSipCliList->pdata[hal_index - 1];

            if(pSipCliData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_SIP_CLIENT pSipCli = &(pSipCliData->dml);

        pSipCli->uInstanceNumber = hal_index;

        //VoiceService.{i}.SIP.Client.{i}.
        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.SIP.Client.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipCli->Enable = true;
            }
            else
            {
                 pSipCli->Enable = false;
            }
        }
        else if( strstr(ParamName, "QuiescentMode") )
        {
            //VoiceService.{i}.SIP.Client.{i}.QuiescentMode
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipCli->QuiescentMode = true;
            }
            else
            {
                 pSipCli->QuiescentMode = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.SIP.Client.{i}.Status
            //strncpy(pSipCli->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Origin") )
        {
            //VoiceService.{i}.SIP.Client.{i}.Origin
            //strncpy(pSipCli->Origin, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "RegisterMode") )
        {
            //VoiceService.{i}.SIP.Client.{i}.RegisterMode
            //strncpy(pSipCli->RegisterMode, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "AuthUserName") )
        {
            //VoiceService.{i}.SIP.Client.{i}.AuthUserName
            strncpy(pSipCli->AuthUserName, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "AuthPassword") )
        {
            //VoiceService.{i}.SIP.Client.{i}.AuthPassword
            strncpy(pSipCli->AuthPassword, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Network") )
        {
            //VoiceService.{i}.SIP.Client.{i}.Network
            strncpy(pSipCli->Network, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "MaxSessions") )
        {
            //VoiceService.{i}.SIP.Client.{i}.MaxSessions
            pSipCli->MaxSessions = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "RegisterURI") )
        {
            //VoiceService.{i}.SIP.Client.{i}.RegisterURI
            strncpy(pSipCli->RegisterURI, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "E164Format") )
        {
            //VoiceService.{i}.SIP.Client.{i}.E164Format
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipCli->E164Format = true;
            }
            else
            {
                 pSipCli->E164Format = false;
            }
        }
        else if( strstr(ParamName, "T38Enable") )
        {
            //VoiceService.{i}.SIP.Client.{i}.T38Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipCli->T38Enable = true;
            }
            else
            {
                 pSipCli->T38Enable = false;
            }
        }
        else if( strstr(ParamName, "Contact") )
        {
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Client_Contact_index(ParamName, DML_VOICESERVICE_SIP_CLIENT_CONTACT, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_SIP_CLIENT_CONTACT_LIST_T    pSipCliContactList = &(pSipCli->Contact);

            PDML_SIP_CLIENT_CONTACT_CTRL_T    pSipCliContactData = pSipCliContactList->pdata[hal_index - 1];

            if(pSipCliContactData == NULL)
            {

                if( TelcoVoiceMgrDmlAddSipClientContact(pSipCliContactList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pSipCliContactData = pSipCliContactList->pdata[hal_index - 1];

                if(pSipCliContactData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_SIP_CLIENT_CONTACT pSipCliContact = &(pSipCliContactData->dml);

            pSipCliContact->uInstanceNumber = hal_index;

            //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.
            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pSipCliContact->Enable = true;
                }
                else
                {
                     pSipCliContact->Enable = false;
                }
            }
            else if( strstr(ParamName, "Status") )
            {
                //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.Status
                //strncpy(pSipCliContact->Status, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "Origin") )
            {
                //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.Origin
                //strncpy(pSipCliContact->Origin, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "IPAddress") )
            {
                //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.IPAddress
                strncpy(pSipCliContact->IPAddress, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "Port") )
            {
                //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.Port
                pSipCliContact->Port = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "ContactURI") )
            {
                //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.ContactURI
                strncpy(pSipCliContact->ContactURI, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "ExpireTime") )
            {
                //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.ExpireTime
                strncpy(pSipCliContact->ExpireTime, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "PingInterval") )
            {
                //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.PingInterval
                pSipCliContact->PingInterval = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "UserAgent") )
            {
                //VoiceService.{i}.SIP.Client.{i}.Contact.{i}.UserAgent
                strncpy(pSipCliContact->UserAgent, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "EventSubscribe") )
        {
            //VoiceService.{i}.SIP.Client.{i}.EventSubscribe.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Client_EventSubscribe_index(ParamName, DML_VOICESERVICE_SIP_CLIENT_EVTSUBSCRIBE, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_SIP_CLIENT_SIPEVENTSUB_LIST_T    pSipCliEvtSubscribeList = &(pSipCli->SIPEventSub);

            PDML_SIP_CLIENT_SIPEVENTSUB_CTRL_T    pSipCliEvtSubscribeData = pSipCliEvtSubscribeList->pdata[hal_index - 1];

            if(pSipCliEvtSubscribeData == NULL)
            {

                if( TelcoVoiceMgrDmlAddSipClientContactEvtSub(pSipCliEvtSubscribeList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pSipCliEvtSubscribeData = pSipCliEvtSubscribeList->pdata[hal_index - 1];

                if(pSipCliEvtSubscribeData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_SIP_CLIENT_SIPEVENTSUB pSipCliEvtSubscribe = &(pSipCliEvtSubscribeData->dml);

            pSipCliEvtSubscribe->uInstanceNumber = hal_index;

            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.SIP.Client.{i}.EventSubscribe.{i}.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pSipCliEvtSubscribe->Enable = true;
                }
                else
                {
                     pSipCliEvtSubscribe->Enable = false;
                }
            }
            else if( strstr(ParamName, "Event") )
            {
                //VoiceService.{i}.SIP.Client.{i}.EventSubscribe.{i}.Event
                strncpy(pSipCliEvtSubscribe->Event, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "AuthUserName") )
            {
                //VoiceService.{i}.SIP.Client.{i}.EventSubscribe.{i}.AuthUserName
                strncpy(pSipCliEvtSubscribe->AuthUserName, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "AuthPassword") )
            {
                //VoiceService.{i}.SIP.Client.{i}.EventSubscribe.{i}.AuthPassword
                strncpy(pSipCliEvtSubscribe->AuthPassword, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Network") )
    {
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Network_index(ParamName, DML_VOICESERVICE_SIP_NETWORK, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_SIP_NETWORK_LIST_T    pSipNetworkList = &(pSip->Network);

        PDML_SIP_NETWORK_CTRL_T    pSipNetworkData = pSipNetworkList->pdata[hal_index - 1];

        if(pSipNetworkData == NULL)
        {

            if( TelcoVoiceMgrDmlAddSipNetwork(pSipNetworkList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pSipNetworkData = pSipNetworkList->pdata[hal_index - 1];

            if(pSipNetworkData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_SIP_NETWORK pSipNetwork = &(pSipNetworkData->dml);

        pSipNetwork->uInstanceNumber = hal_index;

        //VoiceService.{i}.SIP.Network.{i}.
        if( strstr(ParamName, "X_RDK_Firewall_Rule_Data") )
        {
            //VoiceService.{i}.SIP.X_RDK_Firewall_Rule_Data
            strncpy(pSipNetwork->X_RDK_Firewall_Rule_Data, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.SIP.Network.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipNetwork->Enable = true;
            }
            else
            {
                 pSipNetwork->Enable = false;
            }
        }
        else if( strstr(ParamName, "QuiescentMode") )
        {
            //VoiceService.{i}.SIP.Network.{i}.QuiescentMode
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipNetwork->QuiescentMode = true;
            }
            else
            {
                 pSipNetwork->QuiescentMode = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.SIP.Network.{i}.Status
            //strncpy(pSipNetwork->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ProxyServer") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ProxyServer
            strncpy(pSipNetwork->ProxyServer, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ProxyServerPort") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ProxyServerPort
            pSipNetwork->ProxyServerPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "ProxyServerTransport") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ProxyServerTransport
            //strncpy(pSipNetwork->ProxyServerTransport, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "RegistrarServer") )
        {
            //VoiceService.{i}.SIP.Network.{i}.RegistrarServer
            strncpy(pSipNetwork->RegistrarServer, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "RegistrarServerPort") )
        {
            //VoiceService.{i}.SIP.Network.{i}.RegistrarServerPort
            pSipNetwork->RegistrarServerPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "RegistrarServerTransport") )
        {
            //VoiceService.{i}.SIP.Network.{i}.RegistrarServerTransport
            //strncpy(pSipNetwork->RegistrarServerTransport, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ServerDomain") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ServerDomain
            strncpy(pSipNetwork->ServerDomain, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ChosenDomain") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ChosenDomain
            strncpy(pSipNetwork->ChosenDomain, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ChosenIPAddress") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ChosenIPAddress
            strncpy(pSipNetwork->ChosenIPAddress, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ChosenPort") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ChosenPort
            pSipNetwork->ChosenPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "UserAgentDomain") )
        {
            //VoiceService.{i}.SIP.Network.{i}.UserAgentDomain
            strncpy(pSipNetwork->UserAgentDomain, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "UserAgentPort") )
        {
            //VoiceService.{i}.SIP.Network.{i}.UserAgentPort
            pSipNetwork->UserAgentPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "UserAgentTransport") )
        {
            //VoiceService.{i}.SIP.Network.{i}.UserAgentTransport
            //strncpy(pSipNetwork->UserAgentTransport, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "OutboundProxy") )
        {
            //VoiceService.{i}.SIP.Network.{i}.OutboundProxy
            strncpy(pSipNetwork->OutboundProxy, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "OutboundProxyResolvedAddress") )
        {
            //VoiceService.{i}.SIP.Network.{i}.OutboundProxyResolvedAddress
            strncpy(pSipNetwork->OutboundProxyResolvedAddress, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "OutboundProxyPrecedence") )
        {
            //VoiceService.{i}.SIP.Network.{i}.OutboundProxyPrecedence
            //strncpy(pSipNetwork->OutboundProxyPrecedence, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "OutboundProxyPort") )
        {
            //VoiceService.{i}.SIP.Network.{i}.OutboundProxyPort
            pSipNetwork->OutboundProxyPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "STUNEnable") )
        {
            //VoiceService.{i}.SIP.Network.{i}.STUNEnable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipNetwork->STUNEnable = true;
            }
            else
            {
                 pSipNetwork->STUNEnable = false;
            }
        }
        else if( strstr(ParamName, "STUNServer") )
        {
            //VoiceService.{i}.SIP.Network.{i}.STUNServer
            strncpy(pSipNetwork->STUNServer, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "NonVoiceBandwidthReservedUpstream") )
        {
            //VoiceService.{i}.SIP.Network.{i}.NonVoiceBandwidthReservedUpstream
            pSipNetwork->NonVoiceBandwidthReservedUpstream = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "NonVoiceBandwidthReservedDownstream") )
        {
            //VoiceService.{i}.SIP.Network.{i}.NonVoiceBandwidthReservedDownstream
            pSipNetwork->NonVoiceBandwidthReservedDownstream = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Organization") )
        {
            //VoiceService.{i}.SIP.Network.{i}.Organization
            strncpy(pSipNetwork->Organization, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "RegistrationPeriod") )
        {
            //VoiceService.{i}.SIP.Network.{i}.RegistrationPeriod
            pSipNetwork->RegistrationPeriod = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Realm") )
        {
            //VoiceService.{i}.SIP.Network.{i}.Realm
            strncpy(pSipNetwork->Realm, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "TimerT1") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerT1
            pSipNetwork->TimerT1 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerT2") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerT2
            pSipNetwork->TimerT2 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerT4") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerT4
            pSipNetwork->TimerT4 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerA") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerA
            pSipNetwork->TimerA = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerB") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerB
            pSipNetwork->TimerB = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerC") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerC
            pSipNetwork->TimerC = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerD") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerD
            pSipNetwork->TimerD = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerE") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerE
            pSipNetwork->TimerE = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerF") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerF
            pSipNetwork->TimerF = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerG") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerG
            pSipNetwork->TimerG = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerH") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerH
            pSipNetwork->TimerH = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerI") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerI
            pSipNetwork->TimerI = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerJ") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerJ
            pSipNetwork->TimerJ = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerK") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerK
            pSipNetwork->TimerK = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "InviteExpires") )
        {
            //VoiceService.{i}.SIP.Network.{i}.InviteExpires
            pSipNetwork->InviteExpires = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "ReInviteExpires") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ReInviteExpires
            pSipNetwork->ReInviteExpires = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "RegisterExpires") )
        {
            //VoiceService.{i}.SIP.Network.{i}.RegisterExpires
            pSipNetwork->RegisterExpires = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "RegisterRetryInterval") )
        {
            //VoiceService.{i}.SIP.Network.{i}.RegisterRetryIntrval
            pSipNetwork->RegisterRetryInterval = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "InboundAuth") )
        {
            //VoiceService.{i}.SIP.Network.{i}.InboundAuth
            //strncpy(pSipNetwork->InboundAuth, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "InboundAuthUsername") )
        {
            //VoiceService.{i}.SIP.Network.{i}.InboundAuthUsername
            strncpy(pSipNetwork->InboundAuthUsername, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "InboundAuthPassword") )
        {
            //VoiceService.{i}.SIP.Network.{i}.InboundAuthPassword
            strncpy(pSipNetwork->InboundAuthPassword, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "UseCodecPriorityInSDPResponse") )
        {
            //VoiceService.{i}.SIP.Network.{i}.UseCodecPriorityInSDPResponse
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipNetwork->UseCodecPriorityInSDPResponse = true;
            }
            else
            {
                 pSipNetwork->UseCodecPriorityInSDPResponse = false;
            }
        }
        else if( strstr(ParamName, "DSCPMark") )
        {
            //VoiceService.{i}.SIP.Network.{i}.DSCPMark
            pSipNetwork->DSCPMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "VLANIDMark") )
        {
            //VoiceService.{i}.SIP.Network.{i}.VLANIDMark
            pSipNetwork->VLANIDMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "EthernetPriorityMark") )
        {
            //VoiceService.{i}.SIP.Network.{i}.EthernetPriorityMark
            pSipNetwork->EthernetPriorityMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "ConferenceCallDomainURI") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ConferenceCallDomainURI
            strncpy(pSipNetwork->ConferenceCallDomainURI, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "TimerLoginRejected") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerLoginRejected
            pSipNetwork->TimerLoginRejected = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "NoLoginRetry") )
        {
            //VoiceService.{i}.SIP.Network.{i}.NoLoginRetry
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipNetwork->NoLoginRetry = true;
            }
            else
            {
                 pSipNetwork->NoLoginRetry = false;
            }
        }
        else if( strstr(ParamName, "TimerRegistrationFailed") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerRegistrationFailed
            pSipNetwork->TimerRegistrationFailed = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TimerSubscriptionFailed") )
        {
            //VoiceService.{i}.SIP.Network.{i}.TimerSubscriptionFailed
            pSipNetwork->TimerSubscriptionFailed = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "UnansweredRegistrationAttempts") )
        {
            //VoiceService.{i}.SIP.Network.{i}.UnansweredRegistrationAttempts
            pSipNetwork->UnansweredRegistrationAttempts = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "VoIPProfile") )
        {
            //VoiceService.{i}.SIP.Network.{i}.VoIPProfile
            strncpy(pSipNetwork->VoIPProfile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CodecList") )
        {
            //VoiceService.{i}.SIP.Network.{i}.CodecList
            strncpy(pSipNetwork->CodecList, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "MaxSessions") )
        {
            //VoiceService.{i}.SIP.Network.{i}.MaxSessions
            pSipNetwork->MaxSessions = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "X_RDK_SKBMark"))
        {
            //VoiceService.{i}.SIP.Network.{i}.X_RDK_SKBMark
            pSipNetwork->X_RDK_SKBMark = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "X_RDK-Central_COM_ConferencingURI"))
        {
            //VoiceService.{i}.SIP.Network.{i}.X_RDK-Central_COM_ConferencingURI
            strncpy(pSipNetwork->X_RDK_Central_COM_ConferencingURI, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "X_RDK-Central_COM_NetworkDisconnect"))
        {
            //VoiceService.{i}.SIP.Network.{i}.X_RDK-Central_COM_NetworkDisconnect
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pSipNetwork->X_RDK_Central_COM_NetworkDisconnect = true;
            }
            else
            {
                pSipNetwork->X_RDK_Central_COM_NetworkDisconnect = false;
            }
        }
        else if( strstr(ParamName, "X_RDK_PRACKRequired"))
        {
            //VoiceService.{i}.SIP.Network.{i}.X_RDK_PRACKRequired
            if(strstr(pValue, "true") || strstr(pValue, "1"))
            {
                pSipNetwork->X_RDK_PRACKRequired = true;
            }
            else
            {
                pSipNetwork->X_RDK_PRACKRequired = false;
            }
        }
        else if( strstr(ParamName, "FQDNServer") )
        {
            //VoiceService.{i}.SIP.Network.{i}.FQDNServer.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Network_FQDNServer_index(ParamName, DML_VOICESERVICE_SIP_NETWORK_FQDNSERVER, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_SIP_NETWORK_FQDNSERVER_LIST_T    pSipNetworkFqdnList = &(pSipNetwork->FQDNServer);

            PDML_SIP_NETWORK_FQDNSERVER_CTRL_T    pSipNetworkFqdnData = pSipNetworkFqdnList->pdata[hal_index - 1];

            if(pSipNetworkFqdnData == NULL)
            {

                if( TelcoVoiceMgrDmlAddSipNetworkFQDNServer(pSipNetworkFqdnList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pSipNetworkFqdnData = pSipNetworkFqdnList->pdata[hal_index - 1];

                if(pSipNetworkFqdnData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_SIP_NETWORK_FQDNSERVER pSipNetworkFqdnSrv = &(pSipNetworkFqdnData->dml);

            pSipNetworkFqdnSrv->uInstanceNumber = hal_index;

            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.SIP.Network.{i}.FQDNServer.{i}.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pSipNetworkFqdnSrv->Enable = true;
                }
                else
                {
                     pSipNetworkFqdnSrv->Enable = false;
                }
            }
            else if( strstr(ParamName, "Origin") )
            {
                //VoiceService.{i}.SIP.Network.{i}.FQDNServer.{i}.Origin
                //strncpy(pSipNetworkFqdnSrv->Origin, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "Domain") )
            {
                //VoiceService.{i}.SIP.Network.{i}.FQDNServer.{i}.Domain
                strncpy(pSipNetworkFqdnSrv->Domain, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "Weight") )
            {
                //VoiceService.{i}.SIP.Network.{i}.FQDNServer.{i}.Weight
                pSipNetworkFqdnSrv->Weight = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "Priority") )
            {
                //VoiceService.{i}.SIP.Network.{i}.FQDNServer.{i}.Priority
                pSipNetworkFqdnSrv->Priority = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "Port") )
            {
                //VoiceService.{i}.SIP.Network.{i}.FQDNServer.{i}.Port
                pSipNetworkFqdnSrv->Port = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "IPAddresses") )
            {
                //VoiceService.{i}.SIP.Network.{i}.FQDNServer.{i}.IPAddresses
                strncpy(pSipNetworkFqdnSrv->IPAddresses, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "EventSubscribe") )
        {
            //VoiceService.{i}.SIP.Network.{i}.EventSubscribe.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Network_EventSubscribe_index(ParamName, DML_VOICESERVICE_SIP_NETWORK_EVTSUBSCRIBE, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_SIP_NETWORK_EVENTSUBSCRIBE_LIST_T    pSipNetworkEvtSubscribeList = &(pSipNetwork->EventSubscribe);

            PDML_SIP_NETWORK_EVENTSUBSCRIBE_CTRL_T    pSipNetworkEvtSubscribeData = pSipNetworkEvtSubscribeList->pdata[hal_index - 1];

            if(pSipNetworkEvtSubscribeData == NULL)
            {

                if( TelcoVoiceMgrDmlAddSipNetworkEvtSub(pSipNetworkEvtSubscribeList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pSipNetworkEvtSubscribeData = pSipNetworkEvtSubscribeList->pdata[hal_index - 1];

                if(pSipNetworkEvtSubscribeData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_SIP_NETWORK_EVENTSUBSCRIBE pSipNetworkEvtSubscribe = &(pSipNetworkEvtSubscribeData->dml);

            pSipNetworkEvtSubscribe->uInstanceNumber = hal_index;

            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.SIP.Network.{i}.EventSubscribe.{i}.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pSipNetworkEvtSubscribe->Enable = true;
                }
                else
                {
                     pSipNetworkEvtSubscribe->Enable = false;
                }
            }
            else if( strstr(ParamName, "Event") )
            {
                //VoiceService.{i}.SIP.Network.{i}.EventSubscribe.{i}.Event
                strncpy(pSipNetworkEvtSubscribe->Event, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "Notifier") )
            {
                //VoiceService.{i}.SIP.Network.{i}.EventSubscribe.{i}.Notifier
                strncpy(pSipNetworkEvtSubscribe->Notifier, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "NotifierPort") )
            {
                //VoiceService.{i}.SIP.Network.{i}.EventSubscribe.{i}.NotifierPort
                pSipNetworkEvtSubscribe->NotifierPort = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "NotifierTransport") )
            {
                //VoiceService.{i}.SIP.Network.{i}.EventSubscribe.{i}.NotifierTransport
                //strncpy(pSipNetworkEvtSubscribe->NotifierTransport, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "ExpireTime") )
            {
                //VoiceService.{i}.SIP.Network.{i}.EventSubscribe.{i}.ExpireTime
                pSipNetworkEvtSubscribe->ExpireTime = strtoul(pValue,&err, 10);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "ResponseMap") )
        {
            //VoiceService.{i}.SIP.Network.{i}.ResponseMap.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Network_ResponseMap_index(ParamName, DML_VOICESERVICE_SIP_NETWORK_RESPMAP, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_SIP_NETWORK_RESPMAP_LIST_T    pSipNetworkRespMapList = &(pSipNetwork->ResponseMap);

            PDML_SIP_NETWORK_RESPMAP_CTRL_T    pSipNetworkRespMapData = pSipNetworkRespMapList->pdata[hal_index - 1];

            if(pSipNetworkRespMapData == NULL)
            {

                if( TelcoVoiceMgrDmlAddSipNetworkRespMap(pSipNetworkRespMapList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pSipNetworkRespMapData = pSipNetworkRespMapList->pdata[hal_index - 1];

                if(pSipNetworkRespMapData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_SIP_NETWORK_RESPMAP pSipNetworkRespMap = &(pSipNetworkRespMapData->dml);

            pSipNetworkRespMap->uInstanceNumber = hal_index;

            if( strstr(ParamName, "SIPResponseNumber") )
            {
                //VoiceService.{i}.SIP.Network.{i}.ResponseMap.{i}.SIPResponseNumber
            }
            else if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.SIP.Network.{i}.ResponseMap.{i}.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pSipNetworkRespMap->Enable = true;
                }
                else
                {
                     pSipNetworkRespMap->Enable = false;
                }
            }
            else if( strstr(ParamName, "TextMessage") )
            {
                //VoiceService.{i}.SIP.Network.{i}.ResponseMap.{i}.TextMessage
                strncpy(pSipNetworkRespMap->TextMessage, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "Tone") )
            {
                //VoiceService.{i}.SIP.Network.{i}.ResponseMap.{i}.Tone
                strncpy(pSipNetworkRespMap->Tone, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Proxy") )
    {
        //VoiceService.{i}.SIP.Proxy.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Proxy_index(ParamName, DML_VOICESERVICE_SIP_PROXY, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_SIP_PROXY_LIST_T    pSipProxyList = &(pSip->Proxy);

        PDML_SIP_PROXY_CTRL_T    pSipProxyData = pSipProxyList->pdata[hal_index - 1];

        if(pSipProxyData == NULL)
        {

            if( TelcoVoiceMgrDmlAddSipProxy(pSipProxyList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pSipProxyData = pSipProxyList->pdata[hal_index - 1];

            if(pSipProxyData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_SIP_PROXY pSipProxy = &(pSipProxyData->dml);

        pSipProxy->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.SIP.Proxy.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipProxy->Enable = true;
            }
            else
            {
                 pSipProxy->Enable = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.SIP.Proxy.{i}.Status
            //strncpy(pSipProxy->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Origin") )
        {
            //VoiceService.{i}.SIP.Proxy.{i}.Origin
            //strncpy(pSipProxy->Origin, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ProxyIPAddress") )
        {
            //VoiceService.{i}.SIP.Proxy.{i}.ProxyIPAddress
            strncpy(pSipProxy->ProxyIPAddress, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ProxyPort") )
        {
            //VoiceService.{i}.SIP.Proxy.{i}.ProxyPort
            pSipProxy->ProxyPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "ContactURI") )
        {
            //VoiceService.{i}.SIP.Proxy.{i}.ContactURI
            strncpy(pSipProxy->ContactURI, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "VoIPProfile") )
        {
            //VoiceService.{i}.SIP.Proxy.{i}.VoIPProfile
            strncpy(pSipProxy->VoIPProfile, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Registrar") )
    {
        //VoiceService.{i}.SIP.Registrar.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Registrar_index(ParamName, DML_VOICESERVICE_SIP_REGISTRAR, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_SIP_REGISTRAR_LIST_T    pSipRegistrarList = &(pSip->Registrar);

        PDML_SIP_REGISTRAR_CTRL_T    pSipRegistrarData = pSipRegistrarList->pdata[hal_index - 1];

        if(pSipRegistrarData == NULL)
        {

            if( TelcoVoiceMgrDmlAddSipRegistrar(pSipRegistrarList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pSipRegistrarData = pSipRegistrarList->pdata[hal_index - 1];

            if(pSipRegistrarData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_SIP_REGISTRAR pSipRegistrar = &(pSipRegistrarData->dml);

        pSipRegistrar->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipRegistrar->Enable = true;
            }
            else
            {
                 pSipRegistrar->Enable = false;
            }
        }
        else if( strstr(ParamName, "QuiescentMode") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.QuiescentMode
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pSipRegistrar->QuiescentMode = true;
            }
            else
            {
                 pSipRegistrar->QuiescentMode = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.Status
            //strncpy(pSipRegistrar->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Origin") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.Origin
            //strncpy(pSipRegistrar->Origin, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "RegistrarIPAddress") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.RegistrarIPAddress
            strncpy(pSipRegistrar->RegistrarIPAddress, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "RegistrarPort") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.RegistrarPort
            pSipRegistrar->RegistrarPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "RegisterExpires") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.RegisterExpires
            pSipRegistrar->RegisterExpires = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "PingInterval") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.PingInterval
            pSipRegistrar->PingInterval = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Organization") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.Organization
            strncpy(pSipRegistrar->Organization, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Realm") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.Realm
            strncpy(pSipRegistrar->Realm, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "VoIPProfile") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.VoIPProfile
            strncpy(pSipRegistrar->VoIPProfile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ContactURI") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.ContactURI
            strncpy(pSipRegistrar->ContactURI, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Account") )
        {
            //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Registrar_Account_index(ParamName, DML_VOICESERVICE_SIP_REGISTRAR_ACCOUNT, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_SIP_REGISTRAR_ACCOUNT_LIST_T    pSipRegistrarAccList = &(pSipRegistrar->Account);

            PDML_SIP_REGISTRAR_ACCOUNT_CTRL_T    pSipRegistrarAccData = pSipRegistrarAccList->pdata[hal_index - 1];

            if(pSipRegistrarAccData == NULL)
            {

                if( TelcoVoiceMgrDmlAddSipRegistrarAcc(pSipRegistrarAccList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pSipRegistrarAccData = pSipRegistrarAccList->pdata[hal_index - 1];

                if(pSipRegistrarAccData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_SIP_REGISTRAR_ACCOUNT pSipRegistrarAcc = &(pSipRegistrarAccData->dml);

            pSipRegistrarAcc->uInstanceNumber = hal_index;

            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Enable
                strncpy(pSipRegistrarAcc->Enable, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "QuiescentMode") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.QuiescentMode
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pSipRegistrarAcc->QuiescentMode = true;
                }
                else
                {
                     pSipRegistrarAcc->QuiescentMode = false;
                }
            }
            else if( strstr(ParamName, "Status") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Status
                //strncpy(pSipRegistrarAcc->Status, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "Origin") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Origin
                //strncpy(pSipRegistrarAcc->Origin, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "CallStatus") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.CallStatus
                //strncpy(pSipRegistrarAcc->CallStatus, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "AuthUserName") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.AuthUserName
                strncpy(pSipRegistrarAcc->AuthUserName, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "AuthPassword") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.AuthPassword
                strncpy(pSipRegistrarAcc->AuthPassword, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "RegistrationStatus") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.RegistrationStatus
                //strncpy(pSipRegistrarAcc->RegistrationStatus, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "URI") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.URI
                strncpy(pSipRegistrarAcc->URI, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "Domain") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Domain
                strncpy(pSipRegistrarAcc->Domain, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "CodecList") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.CodecList
                strncpy(pSipRegistrarAcc->CodecList, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "VoIPProfile") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.VoIPProfile
                strncpy(pSipRegistrarAcc->VoIPProfile, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "Contact") )
            {
                //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Contact.{i}.
                if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_SIP_Registrar_Account_Contact_index(ParamName, DML_VOICESERVICE_SIP_REGISTRAR_ACCOUNT_CONTACT, &hal_index)) ||
                    ( hal_index <= 0 ))
                {
                     AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }

                PDML_SIP_REGISTRAR_ACCOUNT_CONTACT_LIST_T    pSipRegistrarAccContactList = &(pSipRegistrarAcc->Contact);

                PDML_SIP_REGISTRAR_ACCOUNT_CONTACT_CTRL_T    pSipRegistrarAccContactData = pSipRegistrarAccContactList->pdata[hal_index - 1];

                if(pSipRegistrarAccContactData == NULL)
                {

                    if( TelcoVoiceMgrDmlAddSipRegistrarAccContact(pSipRegistrarAccContactList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pSipRegistrarAccContactData = pSipRegistrarAccContactList->pdata[hal_index - 1];

                    if(pSipRegistrarAccContactData == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                PDML_SIP_REGISTRAR_ACCOUNT_CONTACT pSipRegistrarAccContact = &(pSipRegistrarAccContactData->dml);

                pSipRegistrarAccContact->uInstanceNumber = hal_index;


                if( strstr(ParamName, "Enable") )
                {
                    //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Contact.{i}.Enable
                    if( strstr(pValue, "true") || strstr(pValue, "1") )
                    {
                         pSipRegistrarAccContact->Enable = true;
                    }
                    else
                    {
                         pSipRegistrarAccContact->Enable = false;
                    }
                }
                else if( strstr(ParamName, "Status") )
                {
                    //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Contact.{i}.Status
                    //strncpy(pSipRegistrarAccContact->Status, pValue,strlen(pValue)+1);

                }
                else if( strstr(ParamName, "Origin") )
                {
                    //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Contact.{i}.Origin
                    //strncpy(pSipRegistrarAccContact->Origin, pValue,strlen(pValue)+1);

                }
                else if( strstr(ParamName, "IPAddress") )
                {
                    //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Contact.{i}.IPAddress
                    strncpy(pSipRegistrarAccContact->IPAddress, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "Port") )
                {
                    //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Contact.{i}.Port
                    pSipRegistrarAccContact->Port = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "ContactURI") )
                {
                    //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Contact.{i}.ContactURI
                    strncpy(pSipRegistrarAccContact->ContactURI, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "ExpireTime") )
                {
                    //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Contact.{i}.ExpireTime
                    strncpy(pSipRegistrarAccContact->ExpireTime, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "UserAgent") )
                {
                    //VoiceService.{i}.SIP.Registrar.{i}.Account.{i}.Contact.{i}.UserAgent
                    strncpy(pSipRegistrarAccContact->UserAgent, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS Map_hal_dml_MGCP(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_MGCP pMGCP = &(pVoiceService->MGCP_obj);

    if( strstr(ParamName, "Client") )
    {
        //VoiceService.{i}.MGCP.Client.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_MGCP_Client_index(ParamName, DML_VOICESERVICE_MGCP_CLIENT, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_MGCP_CLIENT_LIST_T    pMGCP_ClientList = &(pMGCP->Client);

        PDML_MGCP_CLIENT_CTRL_T    pMGCP_ClientData = pMGCP_ClientList->pdata[hal_index - 1];

        if(pMGCP_ClientData == NULL)
        {

            if( TelcoVoiceMgrDmlAddMGCPClient(pMGCP_ClientList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pMGCP_ClientData = pMGCP_ClientList->pdata[hal_index - 1];

            if(pMGCP_ClientData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_MGCP_CLIENT pMGCP_Client = &(pMGCP_ClientData->dml);

        pMGCP_Client->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.MGCP.Client.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pMGCP_Client->Enable = true;
            }
            else
            {
                 pMGCP_Client->Enable = false;
            }
        }
        else if( strstr(ParamName, "QuiescentMode") )
        {
            //VoiceService.{i}.MGCP.Client.{i}.QuiescentMode
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pMGCP_Client->QuiescentMode = true;
            }
            else
            {
                 pMGCP_Client->QuiescentMode = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.MGCP.Client.{i}.Status
            //strncpy(pMGCP_Client->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "RegisterMode") )
        {
            //VoiceService.{i}.MGCP.Client.{i}.RegisterMode
            //strncpy(pMGCP_Client->RegisterMode, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "LocalPort") )
        {
            //VoiceService.{i}.MGCP.Client.{i}.LocalPort
            pMGCP_Client->LocalPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Domain") )
        {
            //VoiceService.{i}.MGCP.Client.{i}.Domain
            strncpy(pMGCP_Client->Domain, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "User") )
        {
            //VoiceService.{i}.MGCP.Client.{i}.User
            strncpy(pMGCP_Client->User, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Network") )
        {
            //VoiceService.{i}.MGCP.Client.{i}.Network
            strncpy(pMGCP_Client->Network, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "MaxSessions") )
        {
            //VoiceService.{i}.MGCP.Client.{i}.MaxSessions
            pMGCP_Client->MaxSessions = strtoul(pValue,&err, 10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Network") )
    {
        //VoiceService.{i}.MGCP.Network.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_MGCP_Network_index(ParamName, DML_VOICESERVICE_MGCP_NETWORK, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_MGCP_NETWORK_LIST_T    pMGCP_NetworkList = &(pMGCP->Network);

        PDML_MGCP_NETWORK_CTRL_T    pMGCP_NetworkData = pMGCP_NetworkList->pdata[hal_index - 1];

        if(pMGCP_NetworkData == NULL)
        {

            if( TelcoVoiceMgrDmlAddMGCPClient(pMGCP_NetworkList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pMGCP_NetworkData = pMGCP_NetworkList->pdata[hal_index - 1];

            if(pMGCP_NetworkData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_MGCP_NETWORK pMGCP_Network = &(pMGCP_NetworkData->dml);

        pMGCP_Network->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pMGCP_Network->Enable = true;
            }
            else
            {
                 pMGCP_Network->Enable = false;
            }
        }
        else if( strstr(ParamName, "QuiescentMode") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.QuiescentMode
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pMGCP_Network->QuiescentMode = true;
            }
            else
            {
                 pMGCP_Network->QuiescentMode = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.Status
        //    strncpy(pMGCP_Network->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "CallAgent1") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.CallAgent1
            strncpy(pMGCP_Network->CallAgent1, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CallAgentPort1") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.CallAgentPort1
            pMGCP_Network->CallAgentPort1 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "CallAgent2") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.CallAgent2
            strncpy(pMGCP_Network->CallAgent2, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CallAgentPort2") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.CallAgentPort2
            pMGCP_Network->CallAgentPort2 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "RetranIntervalTimer") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.RetranIntervalTimer
            pMGCP_Network->RetranIntervalTimer = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "MaxRetranCount") )
        {
            //VoiceService.{i}MGCP.Network.{i}.MaxRetranCount
            pMGCP_Network->MaxRetranCount = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "DSCPMark") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.DSCPMark
            pMGCP_Network->DSCPMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "VLANIDMark") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.VLANIDMark
            pMGCP_Network->VLANIDMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "EthernetPriorityMark") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.EthernetPriorityMark
            pMGCP_Network->EthernetPriorityMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "AllowPiggybackEvents") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.AllowPiggybackEvents
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pMGCP_Network->AllowPiggybackEvents = true;
            }
            else
            {
                 pMGCP_Network->AllowPiggybackEvents = false;
            }
        }
        else if( strstr(ParamName, "SendRSIPImmediately") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.SendRSIPImmediately
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pMGCP_Network->SendRSIPImmediately = true;
            }
            else
            {
                 pMGCP_Network->SendRSIPImmediately = false;
            }
        }
        else if( strstr(ParamName, "STUNEnable") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.STUNEnable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pMGCP_Network->STUNEnable = true;
            }
            else
            {
                 pMGCP_Network->STUNEnable = false;
            }
        }
        else if( strstr(ParamName, "STUNServer") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.STUNServer
            strncpy(pMGCP_Network->STUNServer, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "NonVoiceBandwidthReservedUpstream") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.NonVoiceBandwidthReservedUpstream
            pMGCP_Network->NonVoiceBandwidthReservedUpstream = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "NonVoiceBandwidthReservedDownstream") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.NonVoiceBandwidthReservedDownstream
            pMGCP_Network->NonVoiceBandwidthReservedDownstream = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "MaxSessions") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.MaxSessions
            pMGCP_Network->MaxSessions = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "VoIPProfile") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.VoIPProfile
            strncpy(pMGCP_Network->VoIPProfile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CodecList") )
        {
            //VoiceService.{i}.MGCP.Network.{i}.CodecList
            strncpy(pMGCP_Network->CodecList, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

}
ANSC_STATUS Map_hal_dml_H323(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_H323 pH323 = &(pVoiceService->H323_obj);

    if( strstr(ParamName, "Client") )
    {
        //VoiceService.{i}.H323.Client.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_H323_Client_index(ParamName, DML_VOICESERVICE_H323_CLIENT, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_H323_CLIENT_LIST_T    pH323CliList = &(pH323->Client);

        PDML_H323_CLIENT_CTRL_T    pH323CliData = pH323CliList->pdata[hal_index - 1];

        if(pH323CliData == NULL)
        {

            if( TelcoVoiceMgrDmlAddH323Client(pH323CliList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pH323CliData = pH323CliList->pdata[hal_index - 1];

            if(pH323CliData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_H323_CLIENT pH323Cli = &(pH323CliData->dml);

        pH323Cli->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.H323.Client.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pH323Cli->Enable = true;
            }
            else
            {
                 pH323Cli->Enable = false;
            }
        }
        else if( strstr(ParamName, "QuiescentMode") )
        {
            //VoiceService.{i}.H323.Client.{i}.QuiescentMode
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pH323Cli->QuiescentMode = true;
            }
            else
            {
                 pH323Cli->QuiescentMode = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.H323.Client.{i}.Status
            //strncpy(pH323Cli->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "H235Authentication") )
        {
            //VoiceService.{i}.H323.Client.{i}.H235Authentication
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pH323Cli->H235Authentication = true;
            }
            else
            {
                 pH323Cli->H235Authentication = false;
            }
        }
        else if( strstr(ParamName, "AuthPassword") )
        {
            //VoiceService.{i}.H323.Client.{i}.AuthPassword
            strncpy(pH323Cli->AuthPassword, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SendersID") )
        {
            //VoiceService.{i}.H323.Client.{i}.SendersID
            strncpy(pH323Cli->SendersID, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Network") )
        {
            //VoiceService.{i}.H323.Client.{i}.Network
            strncpy(pH323Cli->Network, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "H323ID") )
        {
            //VoiceService.{i}.H323.Client.{i}.H323ID
            strncpy(pH323Cli->H323ID, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "MaxSessions") )
        {
            //VoiceService.{i}.H323.Client.{i}.MaxSessions
            pH323Cli->MaxSessions = strtoul(pValue,&err, 10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Network") )
    {
        //VoiceService.{i}.H323.Network.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_H323_Network_index(ParamName, DML_VOICESERVICE_H323_NETWORK, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_H323_NETWORK_LIST_T    pH323NwkList = &(pH323->Network);

        PDML_H323_NETWORK_CTRL_T    pH323NwkData = pH323NwkList->pdata[hal_index - 1];

        if(pH323NwkData == NULL)
        {

            if( TelcoVoiceMgrDmlAddH323Network(pH323NwkList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pH323NwkData = pH323NwkList->pdata[hal_index - 1];

            if(pH323NwkData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_H323_NETWORK pH323Nwk = &(pH323NwkData->dml);

        pH323Nwk->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.H323.Network.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pH323Nwk->Enable = true;
            }
            else
            {
                 pH323Nwk->Enable = false;
            }
        }
        else if( strstr(ParamName, "QuiescentMode") )
        {
            //VoiceService.{i}.H323.Network.{i}.QuiescentMode
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pH323Nwk->QuiescentMode = true;
            }
            else
            {
                 pH323Nwk->QuiescentMode = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.H323.Network.{i}.Status
            //strncpy(pH323Nwk->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Gatekeeper") )
        {
            //VoiceService.{i}.H323.Network.{i}.Gatekeeper
            strncpy(pH323Nwk->Gatekeeper, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "GatekeeperPort") )
        {
            //VoiceService.{i}.H323.Network.{i}.GatekeeperPort
            pH323Nwk->GatekeeperPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "GatekeeperID") )
        {
            //VoiceService.{i}.H323.Network.{i}.GatekeeperID
            strncpy(pH323Nwk->GatekeeperID, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "TimeToLive") )
        {
            //VoiceService.{i}.H323.Network.{i}.TimeToLive
            pH323Nwk->TimeToLive = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "DSCPMark") )
        {
            //VoiceService.{i}.H323.Network.{i}.DSCPMark
            pH323Nwk->DSCPMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "VLANIDMark") )
        {
            //VoiceService.{i}.H323.Network.{i}.VLANIDMark
            pH323Nwk->VLANIDMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "EthernetPriorityMark") )
        {
            //VoiceService.{i}.H323.Network.{i}.EthernetPriorityMark
            pH323Nwk->EthernetPriorityMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "STUNEnable") )
        {
            //VoiceService.{i}.H323.Network.{i}.STUNEnable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pH323Nwk->STUNEnable = true;
            }
            else
            {
                 pH323Nwk->STUNEnable = false;
            }
        }
        else if( strstr(ParamName, "STUNServer") )
        {
            //VoiceService.{i}.H323.Network.{i}.STUNServer
            strncpy(pH323Nwk->STUNServer, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "NonVoiceBandwidthReservedUpstream") )
        {
            //VoiceService.{i}.H323.Network.{i}.NonVoiceBandwidthReservedUpstream
            pH323Nwk->NonVoiceBandwidthReservedUpstream = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "NonVoiceBandwidthReservedDownstream") )
        {
            //VoiceService.{i}.H323.Network.{i}.NonVoiceBandwidthReservedDownstream
            pH323Nwk->NonVoiceBandwidthReservedDownstream = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "MaxSessions") )
        {
            //VoiceService.{i}.H323.Network.{i}.MaxSessions
            pH323Nwk->MaxSessions = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "VoIPProfile") )
        {
            //VoiceService.{i}.H323.Network.{i}.VoIPProfile
            strncpy(pH323Nwk->VoIPProfile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CodecList") )
        {
            //VoiceService.{i}.H323.Network.{i}.CodecList
            strncpy(pH323Nwk->CodecList, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

}
ANSC_STATUS Map_hal_dml_Trunk(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Trunk_index(ParamName, DML_VOICESERVICE_TRUNK, &hal_index)) ||
        ( hal_index <= 0 ))
    {
         AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    PDML_TRUNK_LIST_T    pTrunkList = &(pVoiceService->Trunk);

    PDML_TRUNK_CTRL_T    pTrunkData = pTrunkList->pdata[hal_index - 1];

    if(pTrunkData == NULL)
    {

        if( TelcoVoiceMgrDmlAddTrunk(pTrunkList, hal_index - 1) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pTrunkData = pTrunkList->pdata[hal_index - 1];

        if(pTrunkData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    PDML_TRUNK pTrunk = &(pTrunkData->dml);

    pTrunk->uInstanceNumber = hal_index;

    if( strstr(ParamName, "Enable") )
    {
        //VoiceService.{i}.Trunk.{i}.Enable
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pTrunk->Enable = true;
        }
        else
        {
             pTrunk->Enable = false;
        }
    }
    else if( strstr(ParamName, "QuiescentMode") )
    {
        //VoiceService.{i}.Trunk.{i}.QuiescentMode
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pTrunk->QuiescentMode = true;
        }
        else
        {
             pTrunk->QuiescentMode = false;
        }
    }
    else if( strstr(ParamName, "Status") )
    {
        //VoiceService.{i}.Trunk.{i}.Status
       // strncpy(pTrunk->Status, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "Origin") )
    {
        //VoiceService.{i}.Trunk.{i}.Origin
        //strncpy(pTrunk->Origin, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "Name") )
    {
        //VoiceService.{i}.Trunk.{i}.Name
        strncpy(pTrunk->Name, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "DDIRange") )
    {
        //VoiceService.{i}.Trunk.{i}.DDIRange
        strncpy(pTrunk->DDIRange, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "LineObjectCreation") )
    {
        //VoiceService.{i}.Trunk.{i}.LineObjectCreation
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pTrunk->LineObjectCreation = true;
        }
        else
        {
             pTrunk->LineObjectCreation = false;
        }
    }
    else if( strstr(ParamName, "MaxChannels") )
    {
        //VoiceService.{i}.Trunk.{i}.MaxChannels
        pTrunk->MaxChannels = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "MaxOutboundChannelCount") )
    {
        //VoiceService.{i}.Trunk.{i}.MaxOutboundChannelCount
        pTrunk->MaxOutboundChannelCount = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "MaxInboundChannelCount") )
    {
        //VoiceService.{i}.Trunk.{i}.MaxInboundChannelCount
        pTrunk->MaxInboundChannelCount = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "CodecPriority") )
    {
        //VoiceService.{i}.Trunk.{i}.CodecPriority
        //strncpy(pTrunk->CodecPriority, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "Provider") )
    {
        //VoiceService.{i}.Trunk.{i}.Provider
        strncpy(pTrunk->Provider, pValue,strlen(pValue)+1);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS Map_hal_dml_CallControl(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_CALLCONTROL pCallCtrl = &(pVoiceService->CallControl_obj);

    if( strstr(ParamName, "MaxNumberOfLines") )
    {
        //VoiceService.{i}.CallControl.MaxNumberOfLines
        pCallCtrl->MaxNumberOfLines = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "MaxNumberOfExtensions") )
    {
        //VoiceService.{i}.CallControl.MaxNumberOfExtensions
        pCallCtrl->MaxNumberOfExtensions = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "Line") )
    {
        //VoiceService.{i}.CallControl.Line.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_Line_index(ParamName, DML_VOICESERVICE_CALLCONTROL_LINE, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_CALLCONTROL_LINE_LIST_T    pCallCtrlLineList = &(pCallCtrl->Line);

        PDML_CALLCONTROL_LINE_CTRL_T    pCallCtrlLineData = pCallCtrlLineList->pdata[hal_index - 1];

        if(pCallCtrlLineData == NULL)
        {

            if( TelcoVoiceMgrDmlAddCallCtrlLine(pCallCtrlLineList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pCallCtrlLineData = pCallCtrlLineList->pdata[hal_index - 1];

            if(pCallCtrlLineData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_CALLCONTROL_LINE pCallCtrlLine = &(pCallCtrlLineData->dml);

        pCallCtrlLine->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.CallControl.Line.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCallCtrlLine->Enable = true;
            }
            else
            {
                 pCallCtrlLine->Enable = false;
            }
        }
        else if( strstr(ParamName, "QuiescentMode") )
        {
            //VoiceService.{i}.CallControl.Line.{i}.QuiescentMode
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCallCtrlLine->QuiescentMode = true;
            }
            else
            {
                 pCallCtrlLine->QuiescentMode = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.CallControl.Line.{i}.Status
            //strncpy(pCallCtrlLine->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "CallStatus") )
        {
            //VoiceService.{i}.CallControl.Line.{i}.CallStatus
            //strncpy(pCallCtrlLine->CallStatus, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Origin") )
        {
            //VoiceService.{i}.CallControl.Line.{i}.Origin
            //strncpy(pCallCtrlLine->Origin, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "DirectoryNumber") )
        {
            //VoiceService.{i}.CallControl.Line.{i}.DirectoryNumber
            strncpy(pCallCtrlLine->DirectoryNumber, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Provider") )
        {
            //VoiceService.{i}.CallControl.Line.{i}.Provider
            strncpy(pCallCtrlLine->Provider, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CallingFeatures") )
        {
            //VoiceService.{i}.CallControl.Line.{i}.CallingFeatures
            strncpy(pCallCtrlLine->CallingFeatures, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Stats") )
        {
            //VoiceService.{i}.CallControl.Line.{i}.Stats.
            PDML_CALLCONTROL_LINE_STATS pCallCtrlLineStats = &(pCallCtrlLine->Stats);
            if( strstr(ParamName, "IncomingCalls") )
            {
                PDML_CALLCONTROL_STATS_INCALLS pCallCtrlLineStatsInCall = &(pCallCtrlLineStats->IncomingCalls);
                //VoiceService.{i}.CallControl.Line.{i}.Stats.IncomingCalls.
                if( strstr(ParamName, "CallsReceived") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.IncomingCalls.CallsReceived
                    pCallCtrlLineStatsInCall->CallsReceived = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsConnected") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.IncomingCalls.CallsConnected
                    pCallCtrlLineStatsInCall->CallsConnected = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsFailed") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.IncomingCalls.CallsFailed
                    pCallCtrlLineStatsInCall->CallsFailed = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsDropped") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.IncomingCalls.CallsDropped
                    pCallCtrlLineStatsInCall->CallsDropped = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "TotalCallTime") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.IncomingCalls.TotalCallTime
                    pCallCtrlLineStatsInCall->TotalCallTime = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "OutgoingCalls") )
            {
                PDML_CALLCONTROL_STATS_OUTCALLS pCallCtrlLineStatsOutCall = &(pCallCtrlLineStats->OutgoingCalls);
                //VoiceService.{i}.CallControl.Line.{i}.Stats.OutgoingCalls.
                if( strstr(ParamName, "CallsAttempted") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.OutgoingCalls.CallsAttempted
                    pCallCtrlLineStatsOutCall->CallsAttempted = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsConnected") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.OutgoingCalls.CallsConnected
                    pCallCtrlLineStatsOutCall->CallsConnected = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsFailed") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.OutgoingCalls.CallsFailed
                    pCallCtrlLineStatsOutCall->CallsFailed = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsDropped") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.OutgoingCalls.CallsDropped
                    pCallCtrlLineStatsOutCall->CallsDropped = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "TotalCallTime") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.OutgoingCalls.TotalCallTime
                    pCallCtrlLineStatsOutCall->TotalCallTime = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "RTP") )
            {
                PDML_CALLCONTROL_STATS_RTP pCallCtrlLineStatsRtp = &(pCallCtrlLineStats->RTP);
                //VoiceService.{i}.CallControl.Line.{i}.Stats.RTP.
                if( strstr(ParamName, "PacketsReceived") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.RTP.PacketsReceived
                    pCallCtrlLineStatsRtp->PacketsReceived = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsSent") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.RTP.PacketsSent
                    pCallCtrlLineStatsRtp->PacketsSent = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsLost") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.RTP.PacketsLost
                    pCallCtrlLineStatsRtp->PacketsLost = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BytesSent") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.RTP.BytesSent
                    pCallCtrlLineStatsRtp->BytesSent = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BytesReceived") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.RTP.BytesReceived
                    pCallCtrlLineStatsRtp->BytesReceived = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "DSP") )
            {
                PDML_CALLCONTROL_STATS_DSP pCallCtrlLineStatsDsp = &(pCallCtrlLineStats->DSP);
                //VoiceService.{i}.CallControl.Line.{i}.Stats.DSP.
                if( strstr(ParamName, "Overruns") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.DSP.Overruns
                    pCallCtrlLineStatsDsp->Overruns = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "Underruns") )
                {
                    //VoiceService.{i}.CallControl.Line.{i}.Stats.DSP.Underruns
                    pCallCtrlLineStatsDsp->Underruns = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Extension") )
    {
        //VoiceService.{i}.CallControl.Extension.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_Extension_index(ParamName, DML_VOICESERVICE_CALLCONTROL_EXTENSION, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_CALLCONTROL_EXTENSION_LIST_T    pCallCtrlExtList = &(pCallCtrl->Extension);

        PDML_CALLCONTROL_EXTENSION_CTRL_T    pCallCtrlExtData = pCallCtrlExtList->pdata[hal_index - 1];

        if(pCallCtrlExtData == NULL)
        {

            if( TelcoVoiceMgrDmlAddCallCtrlExt(pCallCtrlExtList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pCallCtrlExtData = pCallCtrlExtList->pdata[hal_index - 1];

            if(pCallCtrlExtData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_CALLCONTROL_EXTENSION pCallCtrlExt = &(pCallCtrlExtData->dml);

        pCallCtrlExt->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCallCtrlExt->Enable = true;
            }
            else
            {
                 pCallCtrlExt->Enable = false;
            }
        }
        else if( strstr(ParamName, "QuiescentMode") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.QuiescentMode
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCallCtrlExt->QuiescentMode = true;
            }
            else
            {
                 pCallCtrlExt->QuiescentMode = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.Status
            //strncpy(pCallCtrlExt->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "CallStatus") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.CallStatus
        //    strncpy(pCallCtrlExt->CallStatus, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Origin") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.Origin
            //strncpy(pCallCtrlExt->Origin, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Name") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.Name
            strncpy(pCallCtrlExt->Name, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ExtensionNumber") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.ExtensionNumber
            strncpy(pCallCtrlExt->ExtensionNumber, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Provider") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.Provider
            strncpy(pCallCtrlExt->Provider, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "NumberingPlan") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.NumberingPlan
            strncpy(pCallCtrlExt->NumberingPlan, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CallingFeatures") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.CallingFeatures
            strncpy(pCallCtrlExt->CallingFeatures, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "CallWaitingStatus") )
        {
            //VoiceServce.{i}.CallControl.Extension.{i}.CallWaitingStatus
            //strncpy(pCallCtrlExt->CallWaitingStatus, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ConferenceCallingStatus") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.ConferenceCallingStatus
            //strncpy(pCallCtrlExt->ConferenceCallingStatus, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "ConferenceCallingSessionCount") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.ConferenceCallingSessionCount
            pCallCtrlExt->ConferenceCallingSessionCount = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "VoiceMail") )
        {
            //VoiceService.{i}.CallControl.Extension.{i}.VoiceMail
            strncpy(pCallCtrlExt->VoiceMail, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Stats") )
        {
            PDML_CALLCONTROL_EXTENSION_STATS pCallCtrlExtStats = &(pCallCtrlExt->Stats);
            //VoiceService.{i}.CallControl.Extension.{i}.Stats.
            if( strstr(ParamName, "IncomingCalls") )
            {
                PDML_CALLCONTROL_STATS_INCALLS pCallCtrlExtStatsIC = &(pCallCtrlExtStats->IncomingCalls);
                //VoiceService.{i}.CallControl.Extension.{i}.Stats.IncomingCalls.
                if( strstr(ParamName, "CallsReceived") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.IncomingCalls.CallsReceived
                    pCallCtrlExtStatsIC->CallsReceived = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsConnected") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.IncomingCalls.CallsConnected
                    pCallCtrlExtStatsIC->CallsConnected = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsFailed") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.IncomingCalls.CallsFailed
                    pCallCtrlExtStatsIC->CallsFailed = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsDropped") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.IncomingCalls.CallsDropped
                    pCallCtrlExtStatsIC->CallsDropped = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "TotalCallTime") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.IncomingCalls.TotalCallTime
                    pCallCtrlExtStatsIC->TotalCallTime = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "OutgoingCalls") )
            {
                PDML_CALLCONTROL_STATS_OUTCALLS pCallCtrlExtStatsOutCall = &(pCallCtrlExtStats->OutgoingCalls);
                //VoiceService.{i}.CallControl.Extension.{i}.Stats.OutgoingCalls.
                if( strstr(ParamName, "CallsAttempted") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.OutgoingCalls.CallsAttempted
                    pCallCtrlExtStatsOutCall->CallsAttempted = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsConnected") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.OutgoingCalls.CallsConnected
                    pCallCtrlExtStatsOutCall->CallsConnected = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsFailed") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.OutgoingCalls.CallsFailed
                    pCallCtrlExtStatsOutCall->CallsFailed = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "CallsDropped") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.OutgoingCalls.CallsDropped
                    pCallCtrlExtStatsOutCall->CallsDropped = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "TotalCallTime") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.OutgoingCalls.TotalCallTime
                    pCallCtrlExtStatsOutCall->TotalCallTime = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "RTP") )
            {
                PDML_CALLCONTROL_STATS_RTP pCallCtrlExtStatsRtp = &(pCallCtrlExtStats->RTP);
                //VoiceService.{i}.CallControl.Extension.{i}.Stats.RTP.
                if( strstr(ParamName, "PacketsReceived") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.RTP.PacketsReceived
                    pCallCtrlExtStatsRtp->PacketsReceived = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsSent") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.RTP.PacketsSent
                    pCallCtrlExtStatsRtp->PacketsSent = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsLost") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.RTP.PacketsLost
                    pCallCtrlExtStatsRtp->PacketsLost = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BytesSent") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.RTP.BytesSent
                    pCallCtrlExtStatsRtp->BytesSent = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BytesReceived") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.RTP.BytesReceived
                    pCallCtrlExtStatsRtp->BytesReceived = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "DSP") )
            {
                PDML_CALLCONTROL_STATS_DSP pCallCtrlExtStatsDsp = &(pCallCtrlExtStats->DSP);
                //VoiceService.{i}.CallControl.Extension.{i}.Stats.DSP.
                if( strstr(ParamName, "Overruns") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.DSP.Overruns
                    pCallCtrlExtStatsDsp->Overruns = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "Underruns") )
                {
                    //VoiceService.{i}.CallControl.Extension.{i}.Stats.DSP.Underruns
                    pCallCtrlExtStatsDsp->Underruns = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Group") )
    {
        //VoiceService.{i}.CallControl.Group.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_Group_index(ParamName, DML_VOICESERVICE_CALLCONTROL_GROUP, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_CALLCONTROL_GROUP_LIST_T    pCallCtrlGrpList = &(pCallCtrl->Group);

        PDML_CALLCONTROL_GROUP_CTRL_T    pCallCtrlGrpData = pCallCtrlGrpList->pdata[hal_index - 1];

        if(pCallCtrlGrpData == NULL)
        {

            if( TelcoVoiceMgrDmlAddCallCtrlGrp(pCallCtrlGrpList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pCallCtrlGrpData = pCallCtrlGrpList->pdata[hal_index - 1];

            if(pCallCtrlGrpData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_CALLCONTROL_GROUP pCallCtrlGrp = &(pCallCtrlGrpData->dml);

        pCallCtrlGrp->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Extensions") )
        {
            //VoiceService.{i}.CallControl.Group.{i}.Extensions
            strncpy(pCallCtrlGrp->Extensions, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "RingType") )
        {
            //VoiceService.{i}.CallControl.Group.{i}.RingType
            //strncpy(pCallCtrlGrp->RingType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "RingTimeout") )
        {
            //VoiceService.{i}.CallControl.Group.{i}.RingTimeout
            pCallCtrlGrp->RingTimeout = strtoul(pValue,&err, 10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Mailbox") )
    {
        //VoiceService.{i}.CallControl.Mailbox.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_Mailbox_index(ParamName, DML_VOICESERVICE_CALLCONTROL_MAILBOX, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_CALLCONTROL_MAILBOX_LIST_T    pCallCtrlMailBoxList = &(pCallCtrl->Mailbox);

        PDML_CALLCONTROL_MAILBOX_CTRL_T    pCallCtrlMailBoxData = pCallCtrlMailBoxList->pdata[hal_index - 1];

        if(pCallCtrlMailBoxData == NULL)
        {

            if( TelcoVoiceMgrDmlAddCallCtrlMailBox(pCallCtrlMailBoxList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pCallCtrlMailBoxData = pCallCtrlMailBoxList->pdata[hal_index - 1];

            if(pCallCtrlMailBoxData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_CALLCONTROL_MAILBOX pCallCtrlMailBox = &(pCallCtrlMailBoxData->dml);

        pCallCtrlMailBox->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCallCtrlMailBox->Enable = true;
            }
            else
            {
                 pCallCtrlMailBox->Enable = false;
            }
        }
        else if( strstr(ParamName, "MaxMsg") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.MaxMsg
            pCallCtrlMailBox->MaxMsg = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "MaxMessageTime") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.MaxMessageTime
            pCallCtrlMailBox->MaxMessageTime = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "MinSize") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.MinSize
            pCallCtrlMailBox->MinSize = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "SMTPServerAddress") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.SMTPServerAddress
            strncpy(pCallCtrlMailBox->SMTPServerAddress, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SMTPServerPort") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.SMTPServerPort
            pCallCtrlMailBox->SMTPServerPort = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "SMTPUser") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.SMTPUser
            strncpy(pCallCtrlMailBox->SMTPUser, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SMTPPassword") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.SMTPPassword
            strncpy(pCallCtrlMailBox->SMTPPassword, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "SMTPAuthenticationType") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.SMTPAuthenticationType
            //strncpy(pCallCtrlMailBox->SMTPAuthenticationType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "SMTPFrom") )
        {
            //VoiceService.{i}.CallControl.Mailbox.{i}.SMTPFrom
            strncpy(pCallCtrlMailBox->SMTPFrom, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "IncomingMap") )
    {
        //VoiceService.{i}.CallControl.IncomingMap.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_IncomingMap_index(ParamName, DML_VOICESERVICE_CALLCONTROL_INCOMMINGMAP, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_CALLCONTROL_INCOMINGMAP_LIST_T    pCallCtrlInMapList = &(pCallCtrl->IncommingMap);

        PDML_CALLCONTROL_INCOMINGMAP_CTRL_T    pCallCtrlInMapData = pCallCtrlInMapList->pdata[hal_index - 1];

        if(pCallCtrlInMapData == NULL)
        {

            if( TelcoVoiceMgrDmlAddCallCtrlInMap(pCallCtrlInMapList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pCallCtrlInMapData = pCallCtrlInMapList->pdata[hal_index - 1];

            if(pCallCtrlInMapData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_CALLCONTROL_INCOMINGMAP pCallCtrlInMap = &(pCallCtrlInMapData->dml);

        pCallCtrlInMap->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.CallControl.IncomingMap.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCallCtrlInMap->Enable = true;
            }
            else
            {
                 pCallCtrlInMap->Enable = false;
            }
        }
        else if( strstr(ParamName, "Line") )
        {
            //VoiceService.{i}.CallControl.IncomingMap.{i}.Line
            strncpy(pCallCtrlInMap->Line, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Extension") )
        {
            //VoiceService.{i}.CallControl.IncomingMap.{i}.Extension
            strncpy(pCallCtrlInMap->Extension, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Order") )
        {
            //VoiceService.{i}.CallControl.IncomingMap.{i}.Order
            pCallCtrlInMap->Order = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Timeout") )
        {
            //VoiceService.{i}.CallControl.IncomingMap.{i}.Timeout
            pCallCtrlInMap->Timeout = strtoul(pValue,&err, 10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "OutgoingMap") )
    {
        //VoiceService.{i}.CallControl.OutgoingMap.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_OutgoingMap_index(ParamName, DML_VOICESERVICE_CALLCONTROL_OUTGOINGMAP, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_CALLCONTROL_OUTGOINGMAP_LIST_T    pCallCtrlOutMapList = &(pCallCtrl->OutgoingMap);

        PDML_CALLCONTROL_OUTGOINGMAP_CTRL_T    pCallCtrlOutMapData = pCallCtrlOutMapList->pdata[hal_index - 1];

        if(pCallCtrlOutMapData == NULL)
        {

            if( TelcoVoiceMgrDmlAddCallCtrlOutMap(pCallCtrlOutMapList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pCallCtrlOutMapData = pCallCtrlOutMapList->pdata[hal_index - 1];

            if(pCallCtrlOutMapData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_CALLCONTROL_OUTGOINGMAP pCallCtrlOutMap = &(pCallCtrlOutMapData->dml);

        pCallCtrlOutMap->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.CallControl.OutgoingMap.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pCallCtrlOutMap->Enable = true;
            }
            else
            {
                 pCallCtrlOutMap->Enable = false;
            }
        }
        else if( strstr(ParamName, "CLIPNoScreeningNumber") )
        {
            //VoiceService.{i}.CallControl.OutgoingMap.{i}.CLIPNoScreeningNumber
            strncpy(pCallCtrlOutMap->CLIPNoScreeningNumber, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Extension") )
        {
            //VoiceService.{i}.CallControl.OutgoingMap.{i}.Extension
            strncpy(pCallCtrlOutMap->Extension, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Line") )
        {
            //VoiceService.{i}.CallControl.OutgoingMap.{i}.Line
            strncpy(pCallCtrlOutMap->Line, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Order") )
        {
            //VoiceService.{i}.CallControl.OutgoingMap.{i}.Order
            pCallCtrlOutMap->Order = strtoul(pValue,&err, 10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "NumberingPlan") )
    {
        //VoiceService.{i}.CallControl.NumberingPlan.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_NumberingPlan_index(ParamName, DML_VOICESERVICE_CALLCONTROL_NUMPLAN, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_CALLCONTROL_NUMBERINGPLAN_LIST_T    pCallCtrlNumPlanList = &(pCallCtrl->NumberingPlan);

        PDML_CALLCONTROL_NUMBERINGPLAN_CTRL_T    pCallCtrlNumPlanData = pCallCtrlNumPlanList->pdata[hal_index - 1];

        if(pCallCtrlNumPlanData == NULL)
        {

            if( TelcoVoiceMgrDmlAddCallCtrlNumPlan(pCallCtrlNumPlanList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pCallCtrlNumPlanData = pCallCtrlNumPlanList->pdata[hal_index - 1];

            if(pCallCtrlNumPlanData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_CALLCONTROL_NUMBERINGPLAN pCallCtrlNumPlan = &(pCallCtrlNumPlanData->dml);

        pCallCtrlNumPlan->uInstanceNumber = hal_index;

        if( strstr(ParamName, "MinimumNumberOfDigits") )
        {
            //VoiceService.{i}.CallControl.NumberingPlan.{i}.MinimumNumberOfDigits
            pCallCtrlNumPlan->MinimumNumberOfDigits = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "MaximumNumberOfDigits") )
        {
            //VoiceService.{i}.CallControl.NumberingPlan.{i}.MaximumNumberOfDigits
            pCallCtrlNumPlan->MaximumNumberOfDigits = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "InterDigitTimerStd") )
        {
            //VoiceService.{i}.CallControl.NumberingPlan.{i}.InterDigitTimerStd
            pCallCtrlNumPlan->InterDigitTimerStd = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "InterDigitTimerOpen") )
        {
            //VoiceService.{i}.CallControl.NumberingPlan.{i}.InterDigitTimerOpen
            pCallCtrlNumPlan->InterDigitTimerOpen = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TerminationDigit") )
        {
            //VoiceService.{i}.CallControl.NumberingPlan.{i}.TerminationDigit
            strncpy(pCallCtrlNumPlan->TerminationDigit, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "InvalidNumberTone") )
        {
            //VoiceService.{i}.CallControl.NumberingPlan.{i}.InvalidNumberTone
            strncpy(pCallCtrlNumPlan->InvalidNumberTone, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "PrefixInfoMaxEntries") )
        {
            //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfoMaxEntries
            pCallCtrlNumPlan->PrefixInfoMaxEntries = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "PrefixInfo") )
        {
            //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_NumberingPlan_PrefixInfo_index(ParamName, DML_VOICESERVICE_CALLCONTROL_NUMPLAN_PREFIXINFO, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_LIST_T    pCallCtrlNumPlanPrefixInfoList = &(pCallCtrlNumPlan->PrefixInfo);

            PDML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_CTRL_T    pCallCtrlNumPlanPrefixInfoData = pCallCtrlNumPlanPrefixInfoList->pdata[hal_index - 1];

            if(pCallCtrlNumPlanPrefixInfoData == NULL)
            {

                if( TelcoVoiceMgrDmlAddCallCtrlNumPlanPrefixInfo(pCallCtrlNumPlanPrefixInfoList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pCallCtrlNumPlanPrefixInfoData = pCallCtrlNumPlanPrefixInfoList->pdata[hal_index - 1];

                if(pCallCtrlNumPlanPrefixInfoData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO pCallCtrlNumPlanPrefixInfo = &(pCallCtrlNumPlanPrefixInfoData->dml);

            pCallCtrlNumPlanPrefixInfo->uInstanceNumber = hal_index;

            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlNumPlanPrefixInfo->Enable = true;
                }
                else
                {
                     pCallCtrlNumPlanPrefixInfo->Enable = false;
                }
            }
            else if( strstr(ParamName, "PrefixRange") )
            {
                //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.PrefixRange
                strncpy(pCallCtrlNumPlanPrefixInfo->PrefixRange, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "PrefixMinNumberOfDigits") )
            {
                //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.PrefixMinNumberOfDigits
                pCallCtrlNumPlanPrefixInfo->PrefixMinNumberOfDigits = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "PrefixMaxNumberOfDigits") )
            {
                //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.PrefixMaxNumberOfDigits
                pCallCtrlNumPlanPrefixInfo->PrefixMaxNumberOfDigits = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "NumberOfDigitsToRemove") )
            {
                //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.NumberOfDigitsToRemove
                pCallCtrlNumPlanPrefixInfo->NumberOfDigitsToRemove = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "PosOfDigitsToRemove") )
            {
                //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.PosOfDigitsToRemove
                pCallCtrlNumPlanPrefixInfo->PosOfDigitsToRemove = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "DialTone") )
            {
                //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.DialTone
                strncpy(pCallCtrlNumPlanPrefixInfo->DialTone, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "FacilityAction") )
            {
                //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.FacilityAction
                strncpy(pCallCtrlNumPlanPrefixInfo->DialTone, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "FacilityActionArgument") )
            {
                //VoiceService.{i}.CallControl.NumberingPlan.{i}.PrefixInfo.{i}.FacilityActionArgument
                strncpy(pCallCtrlNumPlanPrefixInfo->FacilityActionArgument, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "CallingFeatures") )
    {
        PDML_CALLCONTROL_CALLINGFEATURES pCallCtrlCF = &(pCallCtrl->CallingFeatures);
        //VoiceService.{i}.CallControl.CallingFeatures.
        if( strstr(ParamName, "Set") )
        {
            //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_index(ParamName, DML_VOICESERVICE_CALLCONTROL_SET, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_CALLCONTROL_CALLINGFEATURES_SET_LIST_T    pCallCtrlCFSetList = &(pCallCtrlCF->Set);

            PDML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_T    pCallCtrlCFSetData = pCallCtrlCFSetList->pdata[hal_index - 1];

            if(pCallCtrlCFSetData == NULL)
            {

                if( TelcoVoiceMgrDmlAddCallCtrlCFSet(pCallCtrlCFSetList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pCallCtrlCFSetData = pCallCtrlCFSetList->pdata[hal_index - 1];

                if(pCallCtrlCFSetData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_CALLCONTROL_CALLINGFEATURES_SET pCallCtrlCFSet = &(pCallCtrlCFSetData->dml);

            pCallCtrlCFSet->uInstanceNumber = hal_index;

            if( strstr(ParamName, "CallerIDEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallerIDEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->CallerIDEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->CallerIDEnable = false;
                }
            }
            else if( strstr(ParamName, "CallerIDNameEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallerIDNameEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->CallerIDNameEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->CallerIDNameEnable = false;
                }
            }
            else if( strstr(ParamName, "CallWaitingEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallWaitingEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->CallWaitingEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->CallWaitingEnable = false;
                }
            }
            else if( strstr(ParamName, "CallForwardUnconditionalEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallForwardUnconditionalEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->CallForwardUnconditionalEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->CallForwardUnconditionalEnable = false;
                }
            }
            else if( strstr(ParamName, "CallForardUnconditionalNumber") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallForwardUnconditionalNumber
                strncpy(pCallCtrlCFSet->CallForwardUnconditionalNumber, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "CallForwardOnBusyEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallForwardOnBusyEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->CallForwardOnBusyEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->CallForwardOnBusyEnable = false;
                }
            }
            else if( strstr(ParamName, "CallForwardOnBusyNumber") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallForwardOnBusyNumber
                strncpy(pCallCtrlCFSet->CallForwardOnBusyNumber, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "CallForwardOnBusyRingTimeout") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallForwardOnBusyRingTimeout
                pCallCtrlCFSet->CallForwardOnBusyRingTimeout = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "CallForwardOnNoAnswerEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallForwardOnNoAnswerEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->CallForwardOnNoAnswerEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->CallForwardOnNoAnswerEnable = false;
                }
            }
            else if( strstr(ParamName, "CallForwardOnNoAnswerNumber") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallForwardOnNoAnswerNumber
                strncpy(pCallCtrlCFSet->CallForwardOnNoAnswerNumber, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "CallForwardOnNoAnswerRingTimeout") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallForwardOnNoAnswerRingTimeout
                pCallCtrlCFSet->CallForwardOnNoAnswerRingTimeout = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "CallTransferEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallTransferEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->CallTransferEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->CallTransferEnable = false;
                }
            }
            else if( strstr(ParamName, "MWIEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.MWIEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->MWIEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->MWIEnable = false;
                }
            }
            else if( strstr(ParamName, "VMWIEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.VMWIEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->VMWIEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->VMWIEnable = false;
                }
            }
            else if( strstr(ParamName, "LineMessagesWaiting") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.LineMessagesWaiting
                pCallCtrlCFSet->LineMessagesWaiting = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "AnonymousCallRejectionEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.AnonymousCallRejectionEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->AnonymousCallRejectionEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->AnonymousCallRejectionEnable = false;
                }
            }
            else if( strstr(ParamName, "AnonymousCallEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.AnonymousCallEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->AnonymousCallEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->AnonymousCallEnable = false;
                }
            }
            else if( strstr(ParamName, "DoNotDisturbEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.DoNotDisturbEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->DoNotDisturbEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->DoNotDisturbEnable = false;
                }
            }
            else if( strstr(ParamName, "RepeatDialEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.RepeatDialEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->RepeatDialEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->RepeatDialEnable = false;
                }
            }
            else if( strstr(ParamName, "VoiceMailEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.VoiceMailEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->VoiceMailEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->VoiceMailEnable = false;
                }
            }
            else if( strstr(ParamName, "CallPickUpEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CallPickUpEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->CallPickUpEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->CallPickUpEnable = false;
                }
            }
            else if( strstr(ParamName, "CCBSEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CCBSEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->CCBSEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->CCBSEnable = false;
                }
            }
            else if( strstr(ParamName, "IIFCEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.IIFCEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->IIFCEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->IIFCEnable = false;
                }
            }
            else if( strstr(ParamName, "BlockForwardEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.BlockForwardEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->BlockForwardEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->BlockForwardEnable = false;
                }
            }
            else if( strstr(ParamName, "SecretForwarderEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.SecretForwarderEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->SecretForwarderEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->SecretForwarderEnable = false;
                }
            }
            else if( strstr(ParamName, "FollowMeEnable") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.FollowMeEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pCallCtrlCFSet->FollowMeEnable = true;
                }
                else
                {
                     pCallCtrlCFSet->FollowMeEnable = false;
                }
            }
            else if( strstr(ParamName, "X_RDK-Central_COM_ConferenceCallingEnable"))
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.X_RDK-Central_COM_ConferenceCallingEnable
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pCallCtrlCFSet->X_RDK_Central_COM_ConferenceCallingEnable = true;
                }
                else
                {
                    pCallCtrlCFSet->X_RDK_Central_COM_ConferenceCallingEnable = false;
                }
            }
            else if( strstr(ParamName, "X_RDK-Central_COM_HoldEnable"))
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.X_RDK-Central_COM_HoldEnable
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pCallCtrlCFSet->X_RDK_Central_COM_HoldEnable = true;
                }
                else
                {
                    pCallCtrlCFSet->X_RDK_Central_COM_HoldEnable = false;
                }
            }
            else if( strstr(ParamName, "X_RDK-Central_COM_PhoneCallerIDEnable"))
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.X_RDK-Central_COM_PhoneCallerIDEnable
                if(strstr(pValue, "true") || strstr(pValue, "1"))
                {
                    pCallCtrlCFSet->X_RDK_Central_COM_PhoneCallerIDEnable = true;
                }
                else
                {
                    pCallCtrlCFSet->X_RDK_Central_COM_PhoneCallerIDEnable = false;
                }
            }
            else if( strstr(ParamName, "CFT") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CFT.{i}.
                if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_CFT_index(ParamName, DML_VOICESERVICE_CALLCONTROL_SET_CFT, &hal_index)) ||
                    ( hal_index <= 0 ))
                {
                     AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }

                PDML_CALLCONTROL_CALLINGFEATURES_SET_CFT_LIST_T    pCallCtrlCFSetCFTList = &(pCallCtrlCFSet->CFT);

                PDML_CALLCONTROL_CALLINGFEATURES_SET_CFT_CTRL_T    pCallCtrlCFSetCFTData = pCallCtrlCFSetCFTList->pdata[hal_index - 1];

                if(pCallCtrlCFSetCFTData == NULL)
                {

                    if( TelcoVoiceMgrDmlAddCallCtrlCFSetCFT(pCallCtrlCFSetCFTList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pCallCtrlCFSetCFTData = pCallCtrlCFSetCFTList->pdata[hal_index - 1];

                    if(pCallCtrlCFSetCFTData == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                PDML_CALLCONTROL_CALLINGFEATURES_SET_CFT pCallCtrlCFSetCFT = &(pCallCtrlCFSetCFTData->dml);

                pCallCtrlCFSetCFT->uInstanceNumber = hal_index;

                if( strstr(ParamName, "Enable") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CFT.{i}.Enable
                    if( strstr(pValue, "true") || strstr(pValue, "1") )
                    {
                         pCallCtrlCFSetCFT->Enable = true;
                    }
                    else
                    {
                         pCallCtrlCFSetCFT->Enable = false;
                    }
                }
                else if( strstr(ParamName, "Day") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CFT.{i}.Day
                    //strncpy(pCallCtrlCFSetCFT->Day, pValue,strlen(pValue)+1);

                }
                else if( strstr(ParamName, "StartTime") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CFT.{i}.StartTime
                    strncpy(pCallCtrlCFSetCFT->StartTime, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "EndTime") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CFT.{i}.EndTime
                    strncpy(pCallCtrlCFSetCFT->EndTime, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "ForwardedToNumber") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.CFT.{i}.ForwardedToNumber
                    strncpy(pCallCtrlCFSetCFT->ForwardedToNumber, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "SCF") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.SCF.{i}.
                if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_SCF_index(ParamName, DML_VOICESERVICE_CALLCONTROL_SET_SCF, &hal_index)) ||
                    ( hal_index <= 0 ))
                {
                     AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }

                PDML_CALLCONTROL_CALLINGFEATURES_SET_SCF_LIST_T    pCallCtrlCFSetSCFList = &(pCallCtrlCFSet->SCF);

                PDML_CALLCONTROL_CALLINGFEATURES_SET_SCF_CTRL_T    pCallCtrlCFSetSCFData = pCallCtrlCFSetSCFList->pdata[hal_index - 1];

                if(pCallCtrlCFSetSCFData == NULL)
                {

                    if( TelcoVoiceMgrDmlAddCallCtrlCFSetSCF(pCallCtrlCFSetSCFList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pCallCtrlCFSetSCFData = pCallCtrlCFSetSCFList->pdata[hal_index - 1];

                    if(pCallCtrlCFSetSCFData == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                PDML_CALLCONTROL_CALLINGFEATURES_SET_SCF pCallCtrlCFSetSCF = &(pCallCtrlCFSetSCFData->dml);

                pCallCtrlCFSetSCF->uInstanceNumber = hal_index;

                if( strstr(ParamName, "Enable") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.SCF.{i}.Enable
                    if( strstr(pValue, "true") || strstr(pValue, "1") )
                    {
                         pCallCtrlCFSetSCF->Enable = true;
                    }
                    else
                    {
                         pCallCtrlCFSetSCF->Enable = false;
                    }
                }
                else if( strstr(ParamName, "CallingNumber") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.SCF.{i}.CallingNumber
                    strncpy(pCallCtrlCFSetSCF->CallingNumber, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "ForwardedToNumber") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.SCF.{i}.ForwardedToNumber
                    strncpy(pCallCtrlCFSetSCF->ForwardedToNumber, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "SCREJ") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.SCREJ.{i}.
                if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_SCREJ_index(ParamName, DML_VOICESERVICE_CALLCONTROL_SET_SCREJ, &hal_index)) ||
                    ( hal_index <= 0 ))
                {
                     AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }

                PDML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_LIST_T    pCallCtrlCFSetSCREJList = &(pCallCtrlCFSet->SCREJ);

                PDML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_CTRL_T    pCallCtrlCFSetSCREJData = pCallCtrlCFSetSCREJList->pdata[hal_index - 1];

                if(pCallCtrlCFSetSCREJData == NULL)
                {

                    if( TelcoVoiceMgrDmlAddCallCtrlCFSetSCREJ(pCallCtrlCFSetSCREJList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pCallCtrlCFSetSCREJData = pCallCtrlCFSetSCREJList->pdata[hal_index - 1];

                    if(pCallCtrlCFSetSCREJData == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                PDML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ pCallCtrlCFSetSCREJ = &(pCallCtrlCFSetSCREJData->dml);

                pCallCtrlCFSetSCREJ->uInstanceNumber = hal_index;

                if( strstr(ParamName, "Enable") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.SCREJ.{i}.Enable
                    if( strstr(pValue, "true") || strstr(pValue, "1") )
                    {
                         pCallCtrlCFSetSCREJ->Enable = true;
                    }
                    else
                    {
                         pCallCtrlCFSetSCREJ->Enable = false;
                    }
                }
                else if( strstr(ParamName, "CallingNumber") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.SCREJ.{i}.CallingNumber
                    strncpy(pCallCtrlCFSetSCREJ->CallingNumber, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "FollowMe") )
            {
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.FollowMe.{i}.
                if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_FollowMe_index(ParamName, DML_VOICESERVICE_CALLCONTROL_SET_FOLLOWME, &hal_index)) ||
                    ( hal_index <= 0 ))
                {
                     AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }

                PDML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_LIST_T    pCallCtrlCFSetFollowMeList = &(pCallCtrlCFSet->FollowMe);

                PDML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_CTRL_T    pCallCtrlCFSetFollowMeData = pCallCtrlCFSetFollowMeList->pdata[hal_index - 1];

                if(pCallCtrlCFSetFollowMeData == NULL)
                {

                    if( TelcoVoiceMgrDmlAddCallCtrlCFSetFollowMe(pCallCtrlCFSetFollowMeList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                    {
                        AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pCallCtrlCFSetFollowMeData = pCallCtrlCFSetFollowMeList->pdata[hal_index - 1];

                    if(pCallCtrlCFSetFollowMeData == NULL)
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }

                PDML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME pCallCtrlCFSetFollowMe = &(pCallCtrlCFSetFollowMeData->dml);

                pCallCtrlCFSetFollowMe->uInstanceNumber = hal_index;

                if( strstr(ParamName, "Enable") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.FollowMe.{i}.Enable
                    if( strstr(pValue, "true") || strstr(pValue, "1") )
                    {
                         pCallCtrlCFSetFollowMe->Enable = true;
                    }
                    else
                    {
                         pCallCtrlCFSetFollowMe->Enable = false;
                    }
                }
                else if( strstr(ParamName, "Delay") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.FollowMe.{i}.Delay
                    pCallCtrlCFSetFollowMe->Delay = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "Number") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.FollowMe.{i}.Number
                    strncpy(pCallCtrlCFSetFollowMe->Number, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "Order") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.FollowMe.{i}.Order
                    pCallCtrlCFSetFollowMe->Order = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "Voice2Mail") )
            {
                PDML_CALLCONTROL_CALLINGFEATURES_SET_VOICE2MAIL pCallCtrlCFSetV2M = &(pCallCtrlCFSet->Voice2Mail);
                //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.Voice2Mail.
                if( strstr(ParamName, "Enable") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.Voice2Mail.Enable
                    if( strstr(pValue, "true") || strstr(pValue, "1") )
                    {
                         pCallCtrlCFSetV2M->Enable = true;
                    }
                    else
                    {
                         pCallCtrlCFSetV2M->Enable = false;
                    }
                }
                else if( strstr(ParamName, "MaxMessageLength") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.Voice2Mail.MaxMessageLength
                    pCallCtrlCFSetV2M->MaxMessageLength = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "Attach") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.Voice2Mail.Attach
                    if( strstr(pValue, "true") || strstr(pValue, "1") )
                    {
                         pCallCtrlCFSetV2M->Attach = true;
                    }
                    else
                    {
                         pCallCtrlCFSetV2M->Attach = false;
                    }
                }
                else if( strstr(ParamName, "KeepLocal") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.Voice2Mail.KeepLocal
                    if( strstr(pValue, "true") || strstr(pValue, "1") )
                    {
                         pCallCtrlCFSetV2M->KeepLocal = true;
                    }
                    else
                    {
                         pCallCtrlCFSetV2M->KeepLocal = false;
                    }
                }
                else if( strstr(ParamName, "EMailAddress") )
                {
                    //VoiceService.{i}.CallControl.CallingFeatures.Set.{i}.Voice2Mail.EMailAddress
                    strncpy(pCallCtrlCFSetV2M->EMailAddress, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS Map_hal_dml_Interwork(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Interwork_index(ParamName, DML_VOICESERVICE_INTERWORK, &hal_index)) ||
        ( hal_index <= 0 ))
    {
         AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    PDML_INTERWORK_LIST_T    pInterworkList = &(pVoiceService->Interwork);

    PDML_INTERWORK_CTRL_T    pInterworkData = pInterworkList->pdata[hal_index - 1];

    if(pInterworkData == NULL)
    {

        if( TelcoVoiceMgrDmlAddInterwork(pInterworkList, hal_index - 1) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pInterworkData = pInterworkList->pdata[hal_index - 1];

        if(pInterworkData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    PDML_INTERWORK pInterwork = &(pInterworkData->dml);

    pInterwork->uInstanceNumber = hal_index;

    if( strstr(ParamName, "Enable") )
    {
        //VoiceService.{i}.Interwork.{i}.Enable
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pInterwork->Enable = true;
        }
        else
        {
             pInterwork->Enable = false;
        }
    }
    else if( strstr(ParamName, "QuiescentMode") )
    {
        //VoiceService.{i}.Interwork.{i}.QuiescentMode
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pInterwork->QuiescentMode = true;
        }
        else
        {
             pInterwork->QuiescentMode = false;
        }
    }
    else if( strstr(ParamName, "Status") )
    {
        //VoiceService.{i}.Interwork.{i}.Status
        //strncpy(pInterwork->Status, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "OperationalStatus") )
    {
        //VoiceService.{i}.Interwork.{i}.OperationalStatus
        //strncpy(pInterwork->OperationalStatus, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "OperationalStatusReason") )
    {
        //VoiceService.{i}.Interwork.{i}.OperationalStatusReason
        strncpy(pInterwork->OperationalStatusReason, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "NetworkConnectionMode") )
    {
        //VoiceService.{i}.Interwork.{i}.NetworkConnectionMode
        //strncpy(pInterwork->NetworkConnectionMode, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "UserConnectionMode") )
    {
        //VoiceService.{i}.Interwork.{i}.UserConnectionMode
        //strncpy(pInterwork->UserConnectionMode, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "E164Mode") )
    {
        //VoiceService.{i}.Interwork.{i}.E164Mode
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pInterwork->E164Mode = true;
        }
        else
        {
             pInterwork->E164Mode = false;
        }
    }
    else if( strstr(ParamName, "NetworkAuthenticationChallengeMode") )
    {
        //VoiceService.{i}.Interwork.{i}.NetworkAuthenticationChallengeMode
        //strncpy(pInterwork->NetworkAuthChallengeMode, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "NetworkIPAddress") )
    {
        //VoiceService.{i}.Interwork.{i}.NetworkIPAddress
        strncpy(pInterwork->NetworkIPAddress, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "InterworkingRuleSetURI") )
    {
        //VoiceService.{i}.Interwork.{i}.InterworkingRuleSetURI
        strncpy(pInterwork->InterworkingRuleSetURI, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "InterworkingRuleSetInterval") )
    {
        //VoiceService.{i}.Interwork.{i}.InterworkingRuleSetInterval
        pInterwork->InterworkingRuleSetInterval = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "InterworkingRuleSetTime") )
    {
        //VoiceService.{i}.Interwork.{i}.InterworkingRuleSetTime
        strncpy(pInterwork->InterworkingRuleSetTime, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "FirewallRuleSetURI") )
    {
        //VoiceService.{i}.Interwork.{i}.FirewallRuleSetURI
        strncpy(pInterwork->FirewallRuleSetURI, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "FirewallRuleSetInterval") )
    {
        //VoiceService.{i}.Interwork.{i}.FirewallRuleSetInterval
        pInterwork->FirewallRuleSetInterval = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "FirewallRuleSetTime") )
    {
        //VoiceService.{i}.Interwork.{i}.FirewallRuleSetTime
        //pInterwork->FirewallRuleSetTime = strtoul(pValue,&err, 10);
        strncpy(pInterwork->FirewallRuleSetTime, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "InterworkName") )
    {
        //VoiceService.{i}.Interwork.{i}.InterworkName
        strncpy(pInterwork->InterworkName, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "ProxyServer") )
    {
        //VoiceService.{i}.Interwork.{i}.ProxyServer
        strncpy(pInterwork->ProxyServer, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Networks") )
    {
        //VoiceService.{i}.Interwork.{i}.Networks
        strncpy(pInterwork->Networks, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "E164Client") )
    {
        //VoiceService.{i}.Interwork.{i}.E164Client
        strncpy(pInterwork->E164Client, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "UserInterface") )
    {
        //VoiceService.{i}.Interwork.{i}.UserInterface.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Interwork_UserInterface_index(ParamName, DML_VOICESERVICE_INTERWORK_USERINTERFACE, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_INTERWORK_USERINTERFACE_LIST_T    pInterworkUIList = &(pInterwork->UserInterface);

        PDML_INTERWORK_USERINTERFACE_CTRL_T    pInterworkUIData = pInterworkUIList->pdata[hal_index - 1];

        if(pInterworkUIData == NULL)
        {

            if( TelcoVoiceMgrDmlAddInterworkUI(pInterworkUIList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pInterworkUIData = pInterworkUIList->pdata[hal_index - 1];

            if(pInterworkUIData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_INTERWORK_USERINTERFACE pInterworkUI = &(pInterworkUIData->dml);

        pInterworkUI->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.Interwork.{i}.UserInterface.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pInterworkUI->Enable = true;
            }
            else
            {
                 pInterworkUI->Enable = false;
            }
        }
        else if( strstr(ParamName, "Registrar") )
        {
            //VoiceService.{i}.Interwork.{i}.UserInterface.{i}.Registrar
            strncpy(pInterworkUI->Registrar, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Network") )
        {
            //VoiceService.{i}.Interwork.{i}.UserInterface.{i}.Network
            strncpy(pInterworkUI->Network, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Map") )
    {
        //VoiceService.{i}.Interwork.{i}.Map.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Interwork_Map_index(ParamName, DML_VOICESERVICE_INTERWORK_MAP, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_INTERWORK_MAP_LIST_T    pInterworkMapList = &(pInterwork->Map);

        PDML_INTERWORK_MAP_CTRL_T    pInterworkMapData = pInterworkMapList->pdata[hal_index - 1];

        if(pInterworkMapData == NULL)
        {

            if( TelcoVoiceMgrDmlAddInterworkMap(pInterworkMapList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pInterworkMapData = pInterworkMapList->pdata[hal_index - 1];

            if(pInterworkMapData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_INTERWORK_MAP pInterworkMap = &(pInterworkMapData->dml);

        pInterworkMap->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pInterworkMap->Enable = true;
            }
            else
            {
                 pInterworkMap->Enable = false;
            }
        }
        else if( strstr(ParamName, "Status") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.Status
            //strncpy(pInterworkMap->Status, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "StatusDescription") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.StatusDescription
            strncpy(pInterworkMap->StatusDescription, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "LastTime") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.LastTime
            strncpy(pInterworkMap->LastTime, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Origin") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.Origin
            //strncpy(pInterworkMap->Origin, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "NetworkConnection") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.NetworkConnection
            strncpy(pInterworkMap->NetworkConnection, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "UserConnection") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.UserConnection
            strncpy(pInterworkMap->UserConnection, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "DigitMap") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.DigitMap
            strncpy(pInterworkMap->DigitMap, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "DigitMapEnable") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.DigitMapEnable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pInterworkMap->DigitMapEnable = true;
            }
            else
            {
                 pInterworkMap->DigitMapEnable = false;
            }
        }
        else if( strstr(ParamName, "Priority") )
        {
            //VoiceService.{i}.Interwork.{i}.Map.{i}.Priority
            pInterworkMap->Priority = strtoul(pValue,&err, 10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
}


ANSC_STATUS Map_hal_dml_CallLog(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallLog_index(ParamName, DML_VOICESERVICE_CALLLOG, &hal_index)) ||
        ( hal_index <= 0 ))
    {
         AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    PDML_CALLLOG_LIST_T    pCallLogList = &(pVoiceService->CallLog);

    PDML_CALLLOG_CTRL_T    pCallLogData = pCallLogList->pdata[hal_index - 1];

    if(pCallLogData == NULL)
    {

        if( TelcoVoiceMgrDmlAddCallLog(pCallLogList, hal_index - 1) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pCallLogData = pCallLogList->pdata[hal_index - 1];
        if(pCallLogData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    PDML_CALLLOG pCallLog = &(pCallLogData->dml);

    pCallLog->uInstanceNumber = hal_index;

    if( strstr(ParamName, "CallingPartyNumber") )
    {
        //VoiceService.{i}.CallLog.{i}.CallingPartyNumber
        strncpy(pCallLog->CallingPartyNumber, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "CalledPartyNumber") )
    {
        //VoiceService.{i}.CallLog.{i}.CalledPartyNumber
        strncpy(pCallLog->CalledPartyNumber, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Source") )
    {
        //VoiceService.{i}.CallLog.{i}.Source
        strncpy(pCallLog->Source, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Destination") )
    {
        //VoceService.{i}.CallLog.{i}.Destination
        strncpy(pCallLog->Destination, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "RemoteParty") )
    {
        //VoiceService.{i}.CallLog.{i}.RemoteParty
    }
    else if( strstr(ParamName, "UsedLine") )
    {
        //VoiceService.{i}.CallLog.{i}.UsedLine
        strncpy(pCallLog->UsedLine, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "UsedExtensions") )
    {
        //VoiceService.{i}.CallLog.{i}.UsedExtensions
        strncpy(pCallLog->UsedExtensions, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Direction") )
    {
        //VoiceService.{i}.CallLog.{i}.Direction
        //strncpy(pCallLog->Direction, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "Start") )
    {
        //VoiceService.{i}.CallLog.{i}.Start
        strncpy(pCallLog->Start, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Duration") )
    {
        //VoiceService.{i}.CallLog.{i}.Duration
        pCallLog->Duration = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "CallTerminationCause") )
    {
        //VoiceService.{i}.CallLog.{i}.CallTerminationCause
        //strncpy(pCallLog->CallTerminationCause, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "SignalingPerformance") )
    {
        //VoiceService.{i}.CallLog.{i}.SignalingPerformance.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallLog_SignalingPerformance_index(ParamName, DML_VOICESERVICE_CALLLOG_SIGNALINGPERFORMANCE, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_CALLLOG_SIGNALINGPERF_LIST_T    pCallLogSignPerfList = &(pCallLog->SignalingPerformance);

        PDML_CALLLOG_SIGNALINGPERF_CTRL_T    pCallLogSignPerfData = pCallLogSignPerfList->pdata[hal_index - 1];

        if(pCallLogSignPerfData == NULL)
        {

            if( TelcoVoiceMgrDmlAddCallLogSignalPerf(pCallLogSignPerfList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pCallLogSignPerfData = pCallLogSignPerfList->pdata[hal_index - 1];

            if(pCallLogSignPerfData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_CALLLOG_SIGNALINGPERF pCallLogSignPerf = &(pCallLogSignPerfData->dml);

        pCallLogSignPerf->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Protocol") )
        {
            //VoiceService.{i}.CallLog.{i}.SignalingPerformance.{i}.Protocol
            //strncpy(pCallLogSignPerf->Protocol, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "CallSetupDelay") )
        {
            //VoiceService.{i}.CallLog.{i}.SignalingPerformance.{i}.CallSetupDelay
            pCallLogSignPerf->CallSetupDelay = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "OutgoingMediaEstablishDelay") )
        {
            //VoiceService.{i}.CallLog.{i}.SignalingPerformance.{i}.OutgoingMediaEstablishDelay
            pCallLogSignPerf->OutgoingMediaEstablishDelay = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "IncomingMediaEstablishDelay") )
        {
            //VoiceService.{i}.CallLog.{i}.SignalingPerformance.{i}.IncomingMediaEstablishDelay
            pCallLogSignPerf->IncomingMediaEstablishDelay = strtoul(pValue,&err, 10);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Session") )
    {
        //VoiceService.{i}.CallLog.{i}.Session.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CallLog_Session_index(ParamName, DML_VOICESERVICE_CALLLOG_SESSION, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_CALLLOG_SESSION_LIST_T    pCallLogSessionList = &(pCallLog->Session);

        PDML_CALLLOG_SESSION_CTRL_T    pCallLogSessionData = pCallLogSessionList->pdata[hal_index - 1];

        if(pCallLogSessionData == NULL)
        {

            if( TelcoVoiceMgrDmlAddCallLogSession(pCallLogSessionList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pCallLogSessionData = pCallLogSessionList->pdata[hal_index - 1];

            if(pCallLogSessionData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_CALLLOG_SESSION pCallLogSession = &(pCallLogSessionData->dml);

        pCallLogSession->uInstanceNumber = hal_index;

        if( strstr(ParamName, "StreamType") )
        {
            //VoiceService.{i}.CallLog.{i}.Session.{i}.StreamType
            //strncpy(pCallLogSession->StreamType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "Start") )
        {
            //VoiceService.{i}.CallLog.{i}.Session.{i}.Start
            strncpy(pCallLogSession->Start, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Duration") )
        {
            //VoiceService.{i}.CallLog.{i}.Session.{i}.Duration
            pCallLogSession->Duration = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "SessionID") )
        {
            //VoiceService.{i}.CallLog.{i}.Session.{i}.SessionID
            strncpy(pCallLogSession->SessionID, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "Source") )
        {
            PDML_CALLLOG_SESSION_SOURCE pCallLogSessionSrc = &(pCallLogSession->Source);
            //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.
            if( strstr(ParamName, "RTP") )
            {
                PDML_CALLLOG_SESSION_RTP pCallLogSessSrcRtp = &(pCallLogSessionSrc->RTP);
                //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.
                if( strstr(ParamName, "FarEndIPAddress") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.FarEndIPAddress
                    strncpy(pCallLogSessSrcRtp->FarEndIPAddress, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "FarEndUDPPort") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.FarEndUDPPort
                    pCallLogSessSrcRtp->FarEndUDPPort = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "LocalUDPPort") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.LocalUDPPort
                    pCallLogSessSrcRtp->LocalUDPPort = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "MinJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.MinJitter
                    pCallLogSessSrcRtp->MinJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "MaxJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.MaxJitter
                    pCallLogSessSrcRtp->MaxJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "MeanJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.MeanJitter
                    pCallLogSessSrcRtp->MeanJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketDelayVariation") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.PacketDelayVariation
                    pCallLogSessSrcRtp->PacketDelayVariation = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BufferDelay") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.BufferDelay
                    pCallLogSessSrcRtp->BufferDelay = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BurstCount") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.BurstCount
                    pCallLogSessSrcRtp->BurstCount = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsReceived") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.PacketsReceived
                    pCallLogSessSrcRtp->PacketsReceived = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsSent") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.PacketsSent
                    pCallLogSessSrcRtp->PacketsSent = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsLost") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.PacketsLost
                    pCallLogSessSrcRtp->PacketsLost = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsDiscarded") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.PacketsDiscarded
                    pCallLogSessSrcRtp->PacketsDiscarded = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BytesReceived") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.BytesReceived
                }
                else if( strstr(ParamName, "BytesSent") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.BytesSent
                }
                else if( strstr(ParamName, "ReceivePacketLossRate") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.ReceivePacketLossRate
                }
                else if( strstr(ParamName, "FarEndPacketLossRate") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.FarEndPacketLossRate
                }
                else if( strstr(ParamName, "ReceiveInterarrivalJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.ReceiveInterarrivalJitter
                    pCallLogSessSrcRtp->ReceiveInterarrivalJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "FarEndInterarrivalJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.FarEndInterarrivalJitter
                    pCallLogSessSrcRtp->FarEndInterarrivalJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "AverageReceiveInterarrivalJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.AverageReceiveInterarrivalJitter
                    pCallLogSessSrcRtp->AverageReceiveInterarrivalJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "AverageFarEndInterarrivalJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.AverageFarEndInterarrivalJitter
                    pCallLogSessSrcRtp->AverageFarEndInterarrivalJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "RoundTripDelay") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.RoundTripDelay
                    pCallLogSessSrcRtp->RoundTripDelay = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "AverageRoundTripDelay") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.AverageRoundTripDelay
                    pCallLogSessSrcRtp->AverageRoundTripDelay = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "SamplingFrequency") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.RTP.SamplingFrequency
                    pCallLogSessSrcRtp->SamplingFrequency = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "DSP") )
            {
                PDML_CALLLOG_SESSION_DSP pCallLogSessionSrcDsp = &(pCallLogSessionSrc->DSP);
                //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.
                if( strstr(ParamName, "ReceiveCodec") )
                {
                    PDML_CALLLOG_SESSION_DSP_RXCODEC pCallLogSessDspRevCodec = &(pCallLogSessionSrcDsp->ReceiveCodec);
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.ReceiveCodec.
                    if( strstr(ParamName, "Codec") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.ReceiveCodec.Codec
                        strncpy(pCallLogSessDspRevCodec->Codec, pValue,strlen(pValue)+1);
                    }
                    else if( strstr(ParamName, "SilenceSuppression") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.ReceiveCodec.SilenceSuppression
                        if( strstr(pValue, "true") || strstr(pValue, "1") )
                        {
                             pCallLogSessDspRevCodec->SilenceSuppression = true;
                        }
                        else
                        {
                             pCallLogSessDspRevCodec->SilenceSuppression = false;
                        }
                    }
                    else if( strstr(ParamName, "Overruns") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.ReceiveCodec.Overruns
                        pCallLogSessDspRevCodec->Overruns = strtoul(pValue,&err, 10);
                    }
                    else if( strstr(ParamName, "Underruns") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.ReceiveCodec.Underruns
                        pCallLogSessDspRevCodec->Underruns = strtoul(pValue,&err, 10);
                    }
                    else
                    {
                        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                        return ANSC_STATUS_FAILURE;
                    }
                }
                else if( strstr(ParamName, "TransmitCodec") )
                {
                    PDML_CALLLOG_SESSION_DSP_TXCODEC pCallLogSessDspTXCodec = &(pCallLogSessionSrcDsp->TransmitCodec);
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.TransmitCodec.
                    if( strstr(ParamName, "Codec") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.TransmitCodec.Codec
                        strncpy(pCallLogSessDspTXCodec->Codec, pValue,strlen(pValue)+1);
                    }
                    else if( strstr(ParamName, "SilenceSuppression") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.TransmitCodec.SilenceSuppression
                        if( strstr(pValue, "true") || strstr(pValue, "1") )
                        {
                             pCallLogSessDspTXCodec->SilenceSuppression = true;
                        }
                        else
                        {
                             pCallLogSessDspTXCodec->SilenceSuppression = false;
                        }
                    }
                    else if( strstr(ParamName, "PacketizationPeriod") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.TransmitCodec.PacketizationPeriod
                        pCallLogSessDspTXCodec->PacketizationPeriod = strtoul(pValue,&err, 10);
                    }
                    else if( strstr(ParamName, "Overruns") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.TransmitCodec.Overruns
                        pCallLogSessDspTXCodec->Overruns = strtoul(pValue,&err, 10);
                    }
                    else if( strstr(ParamName, "Underruns") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.DSP.TransmitCodec.Underruns
                        pCallLogSessDspTXCodec->Underruns = strtoul(pValue,&err, 10);
                    }
                    else
                    {
                        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                        return ANSC_STATUS_FAILURE;
                    }
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "VoiceQuality") )
            {
                PDML_CALLLOG_SESSION_VOICEQUALITY pCallLogSessSrcVoipVQ = &(pCallLogSessionSrc->VoiceQuality);
                //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.VoiceQuality.
                if( strstr(ParamName, "VoIPQualityIndicator") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.VoiceQuality.VoIPQualityIndicator
                    strncpy(pCallLogSessSrcVoipVQ->VoIPQualityIndicator, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "WorstVoIPQualityIndicatorsValues") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.VoiceQuality.WorstVoIPQualityIndicatorsValues
                    strncpy(pCallLogSessSrcVoipVQ->WorstVoIPQualityIndicatorsValues, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "WorstVoIPQualityIndicatorTimestamps") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Source.VoiceQuality.WorstVoIPQualityIndicatorTimestamps
                    strncpy(pCallLogSessSrcVoipVQ->WorstVoIPQualityIndicatorTimestamps, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "Destination") )
        {
            PDML_CALLLOG_SESSION_DESTINATION pCallLogSessDst = &(pCallLogSession->Destination);
            //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.
            if( strstr(ParamName, "RTP") )
            {
                PDML_CALLLOG_SESSION_RTP pCallLogSessDstRtp = &(pCallLogSessDst->RTP);
                //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.
                if( strstr(ParamName, "FarEndIPAddress") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.FarEndIPAddress
                    strncpy(pCallLogSessDstRtp->FarEndIPAddress, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "FarEndUDPPort") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.FarEndUDPPort
                    pCallLogSessDstRtp->FarEndUDPPort = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "LocalUDPPort") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.LocalUDPPort
                    pCallLogSessDstRtp->LocalUDPPort = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "MinJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.MinJitter
                    pCallLogSessDstRtp->MinJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "MaxJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.MaxJitter
                    pCallLogSessDstRtp->MaxJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "MeanJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.MeanJitter
                    pCallLogSessDstRtp->MeanJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketDelayVariation") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.PacketDelayVariation
                    pCallLogSessDstRtp->PacketDelayVariation = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BufferDelay") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.BufferDelay
                    pCallLogSessDstRtp->BufferDelay = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BurstCount") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.BurstCount
                    pCallLogSessDstRtp->BurstCount = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsReceived") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.PacketsReceived
                    pCallLogSessDstRtp->PacketsReceived = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsSent") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.PacketsSent
                    pCallLogSessDstRtp->PacketsSent = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsLost") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.PacketsLost
                    pCallLogSessDstRtp->PacketsLost = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "PacketsDicarded") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.PacketsDiscarded
                    pCallLogSessDstRtp->PacketsDiscarded = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "BytesReceived") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.BytesReceived
                }
                else if( strstr(ParamName, "BytesSent") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.BytesSent
                }
                else if( strstr(ParamName, "ReceivePacketLossRate") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.ReceivePacketLossRate
                }
                else if( strstr(ParamName, "FarEndPacketLossRate") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.FarEndPacketLossRate
                }
                else if( strstr(ParamName, "ReceiveInterarrivalJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.ReceiveInterarrivalJitter
                    pCallLogSessDstRtp->ReceiveInterarrivalJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "FarEndInterarrivalJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.FarEndInterarrivalJitter
                    pCallLogSessDstRtp->FarEndInterarrivalJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "AverageReceiveInterarrivalJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.AverageReceiveInterarrivalJitter
                    pCallLogSessDstRtp->AverageReceiveInterarrivalJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "AverageFarEndInterarrivalJitter") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.AverageFarEndInterarrivalJitter
                    pCallLogSessDstRtp->AverageFarEndInterarrivalJitter = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "RoundTripDelay") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.RoundTripDelay
                    pCallLogSessDstRtp->RoundTripDelay = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "AverageRoundTripDelay") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.AverageRoundTripDelay
                    pCallLogSessDstRtp->AverageRoundTripDelay = strtoul(pValue,&err, 10);
                }
                else if( strstr(ParamName, "SamplingFrequency") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.RTP.SamplingFrequency
                    pCallLogSessDstRtp->SamplingFrequency = strtoul(pValue,&err, 10);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "DSP") )
            {
                PDML_CALLLOG_SESSION_DSP pCallLogSessDstDsp = &(pCallLogSessDst->DSP);
                //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.
                if( strstr(ParamName, "ReceiveCodec") )
                {
                    PDML_CALLLOG_SESSION_DSP_RXCODEC pCallLogSessDstDspRXCodec = &(pCallLogSessDstDsp->ReceiveCodec);
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.ReceiveCodec.
                    if( strstr(ParamName, "Codec") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.ReceiveCodec.Codec
                        strncpy(pCallLogSessDstDspRXCodec->Codec, pValue,strlen(pValue)+1);
                    }
                    else if( strstr(ParamName, "SilenceSuppression") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.ReceiveCodec.SilenceSuppression
                        if( strstr(pValue, "true") || strstr(pValue, "1") )
                        {
                             pCallLogSessDstDspRXCodec->SilenceSuppression = true;
                        }
                        else
                        {
                             pCallLogSessDstDspRXCodec->SilenceSuppression = false;
                        }
                    }
                    else if( strstr(ParamName, "Overruns") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.ReceiveCodec.Overruns
                        pCallLogSessDstDspRXCodec->Overruns = strtoul(pValue,&err, 10);
                    }
                    else if( strstr(ParamName, "Underruns") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.ReceiveCodec.Underruns
                        pCallLogSessDstDspRXCodec->Underruns = strtoul(pValue,&err, 10);
                    }
                    else
                    {
                        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                        return ANSC_STATUS_FAILURE;
                    }
                }
                else if( strstr(ParamName, "TransmitCodec") )
                {
                    PDML_CALLLOG_SESSION_DSP_TXCODEC pCallLogSessDstDspTXCodec = &(pCallLogSessDstDsp->TransmitCodec);
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.TransmitCodec.
                    if( strstr(ParamName, "Codec") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.TransmitCodec.Codec
                        strncpy(pCallLogSessDstDspTXCodec->Codec, pValue,strlen(pValue)+1);
                    }
                    else if( strstr(ParamName, "SilenceSuppression") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.TransmitCodec.SilenceSuppression
                        if( strstr(pValue, "true") || strstr(pValue, "1") )
                        {
                             pCallLogSessDstDspTXCodec->SilenceSuppression = true;
                        }
                        else
                        {
                             pCallLogSessDstDspTXCodec->SilenceSuppression = false;
                        }
                    }
                    else if( strstr(ParamName, "PacketizationPeriod") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.TransmitCodec.PacketizationPeriod
                        pCallLogSessDstDspTXCodec->PacketizationPeriod = strtoul(pValue,&err, 10);
                    }
                    else if( strstr(ParamName, "Overruns") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.TransmitCodec.Overruns
                        pCallLogSessDstDspTXCodec->Overruns = strtoul(pValue,&err, 10);
                    }
                    else if( strstr(ParamName, "Underruns") )
                    {
                        //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.DSP.TransmitCodec.Underruns
                        pCallLogSessDstDspTXCodec->Underruns = strtoul(pValue,&err, 10);
                    }
                    else
                    {
                        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                        return ANSC_STATUS_FAILURE;
                    }
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else if( strstr(ParamName, "VoiceQuality") )
            {
                PDML_CALLLOG_SESSION_VOICEQUALITY pCallLogSessDstDspVQVoipQI = &(pCallLogSessDst->VoiceQuality);
                //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.VoiceQuality.
                if( strstr(ParamName, "VoIPQualityIndicator") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.VoiceQuality.VoIPQualityIndicator
                    strncpy(pCallLogSessDstDspVQVoipQI->VoIPQualityIndicator, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "WorstVoIPQualityIndicatorsValues") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.VoiceQuality.WorstVoIPQualityIndicatorsValues
                    strncpy(pCallLogSessDstDspVQVoipQI->WorstVoIPQualityIndicatorsValues, pValue,strlen(pValue)+1);
                }
                else if( strstr(ParamName, "WorstVoIPQualityIndicatorTimestamps") )
                {
                    //VoiceService.{i}.CallLog.{i}.Session.{i}.Destination.VoiceQuality.WorstVoIPQualityIndicatorTimestamps
                    strncpy(pCallLogSessDstDspVQVoipQI->WorstVoIPQualityIndicatorTimestamps, pValue,strlen(pValue)+1);
                }
                else
                {
                    AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
}

ANSC_STATUS Map_hal_dml_VoipProfile(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }
    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_VoIPProfile_index(ParamName, DML_VOICESERVICE_VOIPPROF, &hal_index)) ||
        ( hal_index <= 0 ))
    {
         AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    PDML_VOIPPROFILE_LIST_T    pVoipProfileList = &(pVoiceService->VoIPProfile);

    PDML_VOIPPROFILE_CTRL_T    pVoipProfileData = pVoipProfileList->pdata[hal_index - 1];

    if(pVoipProfileData == NULL)
    {

        if( TelcoVoiceMgrDmlAddVoipProfile(pVoipProfileList, hal_index - 1) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pVoipProfileData = pVoipProfileList->pdata[hal_index - 1];
        if(pVoipProfileData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    PDML_VOIPPROFILE pVoipProfile = &(pVoipProfileData->dml);

    pVoipProfile->uInstanceNumber = hal_index;

    if( strstr(ParamName, "Enable") )
    {
        //VoiceService.{i}.VoIPProfile.{i}.Enable
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pVoipProfile->Enable = true;
        }
        else
        {
             pVoipProfile->Enable = false;
        }
    }
    else if( strstr(ParamName, "QuiescentMode") )
    {
        //VoiceService.{i}.VoIPProfile.{i}.QuiescentMode
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pVoipProfile->QuiescentMode = true;
        }
        else
        {
             pVoipProfile->QuiescentMode = false;
        }
    }
    else if( strstr(ParamName, "Name") )
    {
        //VoiceService.{i}.VoIPProfile.{i}.Name
        strncpy(pVoipProfile->Name, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "DTMFMethodG711") )
    {
        //VoiceService.{i}.VoIPProfile.{i}.DTMFMethodG711
        //strncpy(pVoipProfile->DTMFMethodG711, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "DTMFMethod") )
    {
        //VoiceService.{i}.VoIPProfile.{i}.DTMFMethod
        //strncpy(pVoipProfile->DTMFMethod, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "QIModelUsed") )
    {
        //VoiceService.{i}.VoIPProfile.{i}.QIModelUsed
        strncpy(pVoipProfile->QIModelUsed, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "QICalculationInterval") )
    {
        //VoiceService.{i}.VoIPProfile.{i}.QICalculationInterval
        pVoipProfile->QICalculationInterval = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "NumberOfWorstQIValues") )
    {
        //VoiceService.{i}.VoIPProfile.{i}.NumberOfWorstQIValues
        pVoipProfile->NumberOfWorstQIValues = strtoul(pValue,&err, 10);
    }
    else if( strstr(ParamName, "X_RDK-Central_COM_DigitMap"))
    {
        //VoiceService.{i}.VoIPProfile.{i}.X_RDK-Central_COM_DigitMap
        strncpy(pVoipProfile->X_RDK_Central_COM_DigitMap, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "X_RDK-Central_COM_EmergencyDigitMap"))
    {
        //VoiceService.{i}.VoIPProfile.{i}.X_RDK-Central_COM_EmergencyDigitMap
        strncpy(pVoipProfile->X_RDK_Central_COM_EmergencyDigitMap, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "X_RDK-Central_COM_SDigitTimer"))
    {
        //VoiceService.{i}.VoIPProfile.{i}.X_RDK-Central_COM_SDigitTimer
        pVoipProfile->X_RDK_Central_COM_SDigitTimer = strtoul(pValue,&err,10);
    }
    else if( strstr(ParamName, "X_RDK-Central_COM_ZDigitTimer"))
    {
        //VoiceService.{i}.VoIPProfile.{i}.X_RDK-Central_COM_ZDigitTimer
        pVoipProfile->X_RDK_Central_COM_ZDigitTimer = strtoul(pValue,&err,10);
    }
    else if( strstr(ParamName, "RTP") )
    {
        PDML_VOIPPROFILE_RTP pRTP = &(pVoipProfile->RTP);
        //VoiceService.{i}.VoIPProfile.{i}.RTP.
        if( strstr(ParamName, "LocalPortMin") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.LocalPortMin
            pRTP->LocalPortMin = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "LocalPortMax") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.LocalPortMax
            pRTP->LocalPortMax = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Gmin") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.Gmin
            pRTP->Gmin = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "DSCPMark") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.DSCPMark
            pRTP->DSCPMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "VLANIDMark") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.VLANIDMark
            pRTP->VLANIDMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "EthernetPriorityMark") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.EthernetPriorityMark
            pRTP->EthernetPriorityMark = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TelephoneEventPayloadType") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.TelephoneEventPayloadType
            pRTP->TelephoneEventPayloadType = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "JitterBufferType") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.JitterBufferType
            //strncpy(pRTP->JitterBufferType, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "JitterBufferMaxSize") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.JitterBufferMaxSize
            pRTP->JitterBufferMaxSize = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "JitterBufferMinSize") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.JitterBufferMinSize
            pRTP->JitterBufferMinSize = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "JitterBufferActualSize") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.JitterBufferActualSize
            pRTP->JitterBufferActualSize = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "X_RDK_Firewall_Rule_Data"))
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.X_RDK_Firewall_Rule_Data
            strncpy(pRTP->X_RDK_Firewall_Rule_Data, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "X_RDK_SKBMark"))
        {
            //VoiceService.{i}.VoIPProfile.{i}.RTP.X_RDK_SKBMark
            pRTP->X_RDK_SKBMark = strtoul(pValue,&err,10);
        }
        else if( strstr(ParamName, "RTCP") )
        {
            PDML_VOIPPROFILE_RTP_RTCP pRTPRTCP = &(pVoipProfile->RTP.RTCP);
            //VoiceService.{i}.VoIPProfile.{i}.RTP.RTCP.
            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.RTCP.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pRTPRTCP->Enable = true;
                }
                else
                {
                     pRTPRTCP->Enable = false;
                }
            }
            else if( strstr(ParamName, "TxRepeatInterval") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.RTCP.TxRepeatInterval
                pRTPRTCP->TxRepeatInterval = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "LocalCName") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.RTCP.LocalCName
                strncpy(pRTPRTCP->LocalCName, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "SRTP") )
        {
            PDML_VOIPPROFILE_RTP_SRTP pRTP_SRTP = &(pVoipProfile->RTP.SRTP);
            //VoiceService.{i}.VoIPProfile.{i}.RTP.SRTP.
            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.SRTP.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pRTP_SRTP->Enable = true;
                }
                else
                {
                     pRTP_SRTP->Enable = false;
                }
            }
            else if( strstr(ParamName, "KeyingMethods") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.SRTP.KeyingMethods
                //strncpy(pRTP_SRTP->KeyingMethods, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "EncryptionKeySizes") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.SRTP.EncryptionKeySizes
                strncpy(pRTP_SRTP->EncryptionKeySizes, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else if( strstr(ParamName, "Redundancy") )
        {
            PDML_VOIPPROFILE_RTP_REDUNDANCY pRTPRedundancy = &(pVoipProfile->RTP.Redundancy);
            //VoiceService.{i}.VoIPProfile.{i}.RTP.Redundancy.
            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.Redundancy.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pRTPRedundancy->Enable = true;
                }
                else
                {
                     pRTPRedundancy->Enable = false;
                }
            }
            else if( strstr(ParamName, "PayloadType") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.Redundancy.PayloadType
                pRTPRedundancy->PayloadType = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "BlockPayloadType") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.Redundancy.BlockPayloadType
                pRTPRedundancy->BlockPayloadType = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "FaxAndModemRedundancy") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.Redundancy.FaxAndModemRedundancy
                pRTPRedundancy->FaxAndModemRedundancy = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "ModemRedundancy") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.Redundancy.ModemRedundancy
                pRTPRedundancy->ModemRedundancy = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "DTMFRedundancy") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.Redundancy.DTMFRedundancy
                pRTPRedundancy->DTMFRedundancy = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "VoiceRedundancy") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.Redundancy.VoiceRedundancy
                pRTPRedundancy->VoiceRedundancy = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "MaxSessionsUsingRedundancy") )
            {
                //VoiceService.{i}.VoIPProfile.{i}.RTP.Redundancy.MaxSessionsUsingRedundancy
                pRTPRedundancy->MaxSessionsUsingRedundancy = strtoul(pValue,&err, 10);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "FaxT38") )
    {
        PDML_VOIPPROFILE_FAXT38 pFaxT38 = &(pVoipProfile->FaxT38);
        //VoiceService.{i}.VoIPProfile.{i}.FaxT38.
        if( strstr(ParamName, "MaxBitRate") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.FaxT38.MaxBitRate
            pFaxT38->MaxBitRate = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "HighSpeedRedundancy") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.FaxT38.HighSpeedRedundancy
            pFaxT38->HighSpeedRedundancy = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "LowSpeedRedundancy") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.FaxT38.LowSpeedRedundancy
            pFaxT38->LowSpeedRedundancy = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "TCFMethod") )
        {
            //VoiceService.{i}.VoIPProfile.{i}.FaxT38.TCFMethod
            //strncpy(pFaxT38->TCFMethod, pValue,strlen(pValue)+1);

        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
}
ANSC_STATUS Map_hal_dml_codecProfile(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_CodecProfile_index(ParamName, DML_VOICESERVICE_CODECPROF, &hal_index)) ||
        ( hal_index <= 0 ))
    {
         AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    PDML_CODECPROFILE_LIST_T    pCodecProfileList = &(pVoiceService->CodecProfile);

    PDML_CODECPROFILE_CTRL_T    pCodecProfileData = pCodecProfileList->pdata[hal_index - 1];

    if(pCodecProfileData == NULL)
    {

        if( TelcoVoiceMgrDmlAddCodecProfile(pCodecProfileList, hal_index - 1) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pCodecProfileData = pCodecProfileList->pdata[hal_index - 1];
        if(pCodecProfileData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    PDML_CODECPROFILE pCodecProfile = &(pCodecProfileData->dml);

    pCodecProfile->uInstanceNumber = hal_index;

    if( strstr(ParamName, "Enable") )
    {
        //VoiceService.{i}.CodecProfile.{i}.Enable
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pCodecProfile->Enable = true;
        }
        else
        {
             pCodecProfile->Enable = false;
        }
    }
    else if( strstr(ParamName, "Codec") )
    {
        //VoiceService.{i}.CodecProfile.{i}.Codec
        strncpy(pCodecProfile->Codec, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "PacketizationPeriod") )
    {
        //VoiceService.{i}.CodecProfile.{i}.PacketizationPeriod
        strncpy(pCodecProfile->PacketizationPeriod, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "SilenceSuppression") )
    {
        //VoiceService.{i}.CodecProfile.{i}.SilenceSuppression
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pCodecProfile->SilenceSuppression = true;
        }
        else
        {
             pCodecProfile->SilenceSuppression = false;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
}
ANSC_STATUS Map_hal_dml_Tone(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_TONE pTone = &(pVoiceService->Tone_obj);

    if( strstr(ParamName, "DefautEventProfile") )
    {
        //VoiceService.{i}.Tone.DefautEventProfile
        strncpy(pTone->DefautEventProfile, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Description") )
    {
        //VoiceService.{i}.Tone.Description.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Tone_Description_index(ParamName, DML_VOICESERVICE_TONE_DESC, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_TONE_DESCRIPTION_LIST_T    pToneDescList = &(pTone->Description);

        PDML_TONE_DESCRIPTION_CTRL_T    pToneDescData = pToneDescList->pdata[hal_index - 1];

        if(pToneDescData == NULL)
        {

            if( TelcoVoiceMgrDmlAddToneDesc(pToneDescList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pToneDescData = pToneDescList->pdata[hal_index - 1];

            if(pToneDescData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_TONE_DESCRIPTION pToneDesc = &(pToneDescData->dml);

        pToneDesc->uInstanceNumber = hal_index;

        if( strstr(ParamName, "ToneEnable") )
        {
            //VoiceService.{i}.Tone.Description.{i}.ToneEnable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pToneDesc->ToneEnable = true;
            }
            else
            {
                 pToneDesc->ToneEnable = false;
            }
        }
        else if( strstr(ParamName, "ToneName") )
        {
            //VoiceService.{i}.Tone.Description.{i}.ToneName
            strncpy(pToneDesc->ToneName, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "TonePattern") )
        {
            //VoiceService.{i}.Tone.Description.{i}.TonePattern
            strncpy(pToneDesc->TonePattern, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ToneFile") )
        {
            //VoiceService.{i}.Tone.Description.{i}.ToneFile
            strncpy(pToneDesc->ToneFile, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "ToneRepetitions") )
        {
            //VoiceService.{i}.Tone.Description.{i}.ToneRepetitions
            pToneDesc->ToneRepetitions = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "ToneText") )
        {
            //VoiceService.{i}.Tone.Description.{i}.ToneText
            strncpy(pToneDesc->ToneText, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Pattern") )
    {
        //VoiceService.{i}.Tone.Pattern.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Tone_Pattern_index(ParamName, DML_VOICESERVICE_TONE_PATTERN, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_TONE_PATTERN_LIST_T    pTonePatternList = &(pTone->Pattern);

        PDML_TONE_PATTERN_CTRL_T    pTonePatternData = pTonePatternList->pdata[hal_index - 1];

        if(pTonePatternData == NULL)
        {

            if( TelcoVoiceMgrDmlAddTonePattern(pTonePatternList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pTonePatternData = pTonePatternList->pdata[hal_index - 1];

            if(pTonePatternData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_TONE_PATTERN pTonePattern = &(pTonePatternData->dml);

        pTonePattern->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pTonePattern->Enable = true;
            }
            else
            {
                 pTonePattern->Enable = false;
            }
        }
        else if( strstr(ParamName, "ToneOn") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.ToneOn
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pTonePattern->ToneOn = true;
            }
            else
            {
                 pTonePattern->ToneOn = false;
            }
        }
        else if( strstr(ParamName, "Frequency1") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Frequency1
            pTonePattern->Frequency1 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Power1") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Power1
            pTonePattern->Power1 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Frequency2") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Frequency2
            pTonePattern->Frequency2 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Power2") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Power2
            pTonePattern->Power2 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Frequency3") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Frequency3
            pTonePattern->Frequency3 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Power3") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Power3
            pTonePattern->Power3 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Frequency4") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Frequency4
            pTonePattern->Frequency4 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Power4") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Power4
            pTonePattern->Power4 = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "ModulationFrequency") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.ModulationFrequency
            pTonePattern->ModulationFrequency = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "ModulationPower") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.ModulationPower
            pTonePattern->ModulationPower = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Duration") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.Duration
            pTonePattern->Duration = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "NextPattern") )
        {
            //VoiceService.{i}.Tone.Pattern.{i}.NextPattern
            strncpy(pTonePattern->NextPattern, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "EventProfile") )
    {
        //VoiceService.{i}.Tone.EventProfile.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Tone_EventProfile_index(ParamName, DML_VOICESERVICE_TONE_EVTPROF, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_TONE_EVENTPROFILE_LIST_T    pToneEvtProfList = &(pTone->EventProfile);

        PDML_TONE_EVENTPROFILE_CTRL_T    pToneEvtProfData = pToneEvtProfList->pdata[hal_index - 1];

        if(pToneEvtProfData == NULL)
        {

            if( TelcoVoiceMgrDmlAddToneEvtProf(pToneEvtProfList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pToneEvtProfData = pToneEvtProfList->pdata[hal_index - 1];

            if(pToneEvtProfData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_TONE_EVENTPROFILE pToneEvtProf = &(pToneEvtProfData->dml);

        pToneEvtProf->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Event") )
        {
            //VoiceService.{i}.Tone.EventProfile.{i}.Event.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Tone_EventProfile_Event_index(ParamName, DML_VOICESERVICE_TONE_EVTPROF_EVT, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_TONE_EVENTPROFILE_EVENT_LIST_T    pToneEvtProfEvtList = &(pToneEvtProf->Event);

            PDML_TONE_EVENTPROFILE_EVENT_CTRL_T    pToneEvtProfEvtData = pToneEvtProfEvtList->pdata[hal_index - 1];

            if(pToneEvtProfEvtData == NULL)
            {

                if( TelcoVoiceMgrDmlAddToneEvtProfEvt(pToneEvtProfEvtList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pToneEvtProfEvtData = pToneEvtProfEvtList->pdata[hal_index - 1];

                if(pToneEvtProfEvtData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_TONE_EVENTPROFILE_EVENT pToneEvtProfEvt = &(pToneEvtProfEvtData->dml);

            pToneEvtProfEvt->uInstanceNumber = hal_index;

            if( strstr(ParamName, "Function") )
            {
                //VoiceService.{i}.Tone.EventProfile.{i}.Event.{i}.Function
                //strncpy(pToneEvtProfEvt->Function, pValue,strlen(pValue)+1);

            }
            else if( strstr(ParamName, "Tone") )
            {
                //VoiceService.{i}.Tone.EventProfile.{i}.Event.{i}.Tone
                strncpy(pToneEvtProfEvt->Tone, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
}
ANSC_STATUS Map_hal_dml_Terminal(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Terminal_index(ParamName, DML_VOICESERVICE_TERMINAL, &hal_index)) ||
        ( hal_index <= 0 ))
    {
         AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }

    PDML_VOICESERVICE_TERMINAL_LIST_T    pTerminalList = &(pVoiceService->Terminal);

    PDML_VOICESERVICE_TERMINAL_CTRL_T    pTerminalData = pTerminalList->pdata[hal_index - 1];

    if(pTerminalData == NULL)
    {

        if( TelcoVoiceMgrDmlAddTerminal(pTerminalList, hal_index - 1) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pTerminalData = pTerminalList->pdata[hal_index - 1];
        if(pTerminalData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    PDML_VOICESERVICE_TERMINAL pTerminal = &(pTerminalData->dml);

    pTerminal->uInstanceNumber = hal_index;

    //VoiceService.{i}.Terminal.{i}.
    if( strstr(ParamName, "Enable") )
    {
        //VoiceService.{i}.Terminal.{i}.Enable
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pTerminal->Enable = true;
        }
        else
        {
             pTerminal->Enable = false;
        }
    }
    else if( strstr(ParamName, "QuiescentMode") )
    {
        //VoiceService.{i}.Terminal.{i}.QuiescentMode
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pTerminal->QuiescentMode = true;
        }
        else
        {
             pTerminal->QuiescentMode = false;
        }
    }
    else if( strstr(ParamName, "Status") )
    {
        //VoiceService.{i}.Terminal.{i}.Status
        //strncpy(pTerminal->Status, pValue,strlen(pValue)+1);

    }
    else if( strstr(ParamName, "ToneEventProfile") )
    {
        //VoiceService.{i}.Terminal.{i}.ToneEventProfile
        strncpy(pTerminal->ToneEventProfile, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Audio") )
    {
        //VoiceService.{i}.Terminal.{i}.Audio.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Terminal_Audio_index(ParamName, DML_VOICESERVICE_TERMINAL_AUDIO, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_TERMINAL_AUDIO_LIST_T    pTerminalAudioList = &(pTerminal->Audio);

        PDML_TERMINAL_AUDIO_CTRL_T    pTerminalAudioData = pTerminalAudioList->pdata[hal_index - 1];

        if(pTerminalAudioData == NULL)
        {

            if( TelcoVoiceMgrDmlAddTerminalAudio(pTerminalAudioList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pTerminalAudioData = pTerminalAudioList->pdata[hal_index - 1];

            if(pTerminalAudioData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_TERMINAL_AUDIO pTerminalAudio = &(pTerminalAudioData->dml);

        pTerminalAudio->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Name") )
        {
            //VoiceService.{i}.Terminal.{i}.Audio.{i}.Name
            strncpy(pTerminalAudio->Name, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "VoiceProcessing") )
        {
            PDML_TERMINAL_AUDIO_VOICEPROCESSING pTerminalAudioVoiceProcessing = &(pTerminalAudio->VoiceProcessing);
            //VoiceService.{i}.Terminal.{i}.Audio.{i}.VoiceProcessing.
            if( strstr(ParamName, "TransmitGain") )
            {
                //VoiceService.{i}.Terminal.{i}.Audio.{i}.VoiceProcessing.TransmitGain
                pTerminalAudioVoiceProcessing->TransmitGain = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "ReceiveGain") )
            {
                //VoiceService.{i}.Terminal.{i}.Audio.{i}.VoiceProcessing.ReceiveGain
                pTerminalAudioVoiceProcessing->ReceiveGain = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "EchoCancellationEnable") )
            {
                //VoiceService.{i}.Terminal.{i}.Audio.{i}.VoiceProcessing.EchoCancellationEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pTerminalAudioVoiceProcessing->EchoCancellationEnable = true;
                }
                else
                {
                     pTerminalAudioVoiceProcessing->EchoCancellationEnable = false;
                }
            }
            else if( strstr(ParamName, "EchoCancellationInUse") )
            {
                //VoiceService.{i}.Terminal.{i}.Audio.{i}.VoiceProcessing.EchoCancellationInUse
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pTerminalAudioVoiceProcessing->EchoCancellationInUse = true;
                }
                else
                {
                     pTerminalAudioVoiceProcessing->EchoCancellationInUse = false;
                }
            }
            else if( strstr(ParamName, "EchoCancellationTail") )
            {
                //VoiceService.{i}.Terminal.{i}.Audio.{i}.VoiceProcessing.EchoCancellationTail
                pTerminalAudioVoiceProcessing->EchoCancellationTail = strtoul(pValue,&err, 10);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "ButtonMap") )
    {
        PDML_TERMINAL_BUTTONMAP pTermButtonMap = &(pTerminal->ButtonMap);
        //VoiceService.{i}.Terminal.{i}.ButtonMap.
        if( strstr(ParamName,"Button") )
        {
            //VoiceService.{i}.Terminal.{i}.ButtonMap.Button.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Terminal_ButtonMap_Button_index(ParamName, DML_VOICESERVICE_TERMINAL_BUTTONMAP_BUTTON, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_TERMINAL_BUTTONMAP_BUTTON_LIST_T    pTermButtonMapButtonList = &(pTermButtonMap->Button);

            PDML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T    pTermButtonMapButtonData = pTermButtonMapButtonList->pdata[hal_index - 1];

            if(pTermButtonMapButtonData == NULL)
            {

                if( TelcoVoiceMgrDmlAddTermButtonMapButton(pTermButtonMapButtonList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pTermButtonMapButtonData = pTermButtonMapButtonList->pdata[hal_index - 1];

                if(pTermButtonMapButtonData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_TERMINAL_BUTTONMAP_BUTTON pTermButtonMapButton = &(pTermButtonMapButtonData->dml);

            pTermButtonMapButton->uInstanceNumber = hal_index;

            if( strstr(ParamName, "ButtonName") )
            {
                //VoiceService.{i}.Terminal.{i}.ButtonMap.Button.{i}.ButtonName
                strncpy(pTermButtonMapButton->ButtonName, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "FacilityAction") )
            {
                //VoiceService.{i}.Terminal.{i}.ButtonMap.Button.{i}.FacilityAction
                strncpy(pTermButtonMapButton->FacilityAction, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "FacilityActionArgument") )
            {
                //VoiceService.{i}.Terminal.{i}.ButtonMap.Button.{i}.FacilityActionArument
                strncpy(pTermButtonMapButton->FacilityActionArgument, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "QuickDialNumber") )
            {
                //VoiceService.{i}.Terminal.{i}.ButtonMap.Button.{i}.QuickDialNumber
                strncpy(pTermButtonMapButton->QuickDialNumber, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "ButtonMessage") )
            {
                //VoiceService.{i}.Terminal.{i}.ButtonMap.Button.{i}.ButtonMessage
                strncpy(pTermButtonMapButton->ButtonMessage, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "UserAccess") )
            {
                //VoiceService.{i}.Terminal.{i}.ButtonMap.Button.{i}.UserAccess
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pTermButtonMapButton->UserAccess = true;
                }
                else
                {
                     pTermButtonMapButton->UserAccess = false;
                }
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "Ringer") )
    {
        PDML_TERMINAL_RINGER pTermRinger = &(pTerminal->Ringer);
        //VoiceService.{i}.Terminal.{i}.Ringer.
        if( strstr(ParamName, "Description") )
        {
            //VoiceService.{i}.Terminal.{i}.Ringer.Description.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Terminal_Ringer_Description_index(ParamName, DML_VOICESERVICE_TERMINAL_RINGER_DESC, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_TERMINAL_RINGER_DESCRIPTION_LIST_T    pTermRingerDescList = &(pTermRinger->Description);

            PDML_TERMINAL_RINGER_DESCRIPTION_CTRL_T    pTermRingerDescData = pTermRingerDescList->pdata[hal_index - 1];

            if(pTermRingerDescData == NULL)
            {

                if( TelcoVoiceMgrDmlAddTermRingerDesc(pTermRingerDescList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pTermRingerDescData = pTermRingerDescList->pdata[hal_index - 1];

                if(pTermRingerDescData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_TERMINAL_RINGER_DESCRIPTION pTermRingerDesc = &(pTermRingerDescData->dml);

            pTermRingerDesc->uInstanceNumber = hal_index;

            if( strstr(ParamName, "RingEnable") )
            {
                //VoiceService.{i}.Terminal.{i}.Ringer.Description.{i}.RingEnable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pTermRingerDesc->RingEnable = true;
                }
                else
                {
                     pTermRingerDesc->RingEnable = false;
                }
            }
            else if( strstr(ParamName, "RingName") )
            {
                //VoiceService.{i}.Terminal.{i}.Ringer.Description.{i}.RingName
            }
            else if( strstr(ParamName, "RingPattern") )
            {
                //VoiceService.{i}.Terminal.{i}.Ringer.Description.{i}.RingPattern
                strncpy(pTermRingerDesc->RingPattern, pValue,strlen(pValue)+1);
            }
            else if( strstr(ParamName, "RingFile") )
            {
                //VoiceService.{i}.Terminal.{i}.Ringer.Description.{i}.RingFile
                strncpy(pTermRingerDesc->RingFile, pValue,strlen(pValue)+1);
            }
        }
        else if( strstr(ParamName, "Pattern") )
        {
            //VoiceService.{i}.Terminal.{i}.Ringer.Pattern.{i}.
            if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_Terminal_Ringer_Pattern_index(ParamName, DML_VOICESERVICE_TERMINAL_RINGER_PATTERN, &hal_index)) ||
                ( hal_index <= 0 ))
            {
                 AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }

            PDML_TERMINAL_RINGER_PATTERN_LIST_T    pTermRingerPatternList = &(pTermRinger->Pattern);

            PDML_TERMINAL_RINGER_PATTERN_CTRL_T    pTermRingerPatternData = pTermRingerPatternList->pdata[hal_index - 1];

            if(pTermRingerPatternData == NULL)
            {

                if( TelcoVoiceMgrDmlAddTermRingerPattern(pTermRingerPatternList, hal_index - 1) != ANSC_STATUS_SUCCESS)
                {
                    AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                pTermRingerPatternData = pTermRingerPatternList->pdata[hal_index - 1];

                if(pTermRingerPatternData == NULL)
                {
                    return ANSC_STATUS_FAILURE;
                }
            }

            PDML_TERMINAL_RINGER_PATTERN pTermRingerPattern = &(pTermRingerPatternData->dml);

            pTermRingerPattern->uInstanceNumber = hal_index;

            if( strstr(ParamName, "Enable") )
            {
                //VoiceService.{i}.Terminal.{i}.Ringer.Pattern.{i}.Enable
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pTermRingerPattern->Enable = true;
                }
                else
                {
                     pTermRingerPattern->Enable = false;
                }
            }
            else if( strstr(ParamName, "RingerOn") )
            {
                //VoiceService.{i}.Terminal.{i}.Ringer.Pattern.{i}.RingerOn
                if( strstr(pValue, "true") || strstr(pValue, "1") )
                {
                     pTermRingerPattern->RingerOn = true;
                }
                else
                {
                     pTermRingerPattern->RingerOn = false;
                }
            }
            else if( strstr(ParamName, "Duration") )
            {
                //VoiceService.{i}.Terminal.{i}.Ringer.Pattern.{i}.Duration
                pTermRingerPattern->Duration = strtoul(pValue,&err, 10);
            }
            else if( strstr(ParamName, "NextPattern") )
            {
                //VoiceService.{i}.Terminal.{i}.Ringer.Pattern.{i}.NextPattern
                strncpy(pTermRingerPattern->NextPattern, pValue,strlen(pValue)+1);
            }
            else
            {
                AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( strstr(ParamName, "DiagTests") )
    {
        //VoiceService.{i}.Terminal.{i}.DiagTests.
        PDML_TERMINAL_DIAGTESTS pTerminalDiagTests = &(pTerminal->DiagTests);

        if( strstr(ParamName, "DiagnosticsState") )
        {
            //VoiceService.{i}.Terminal.{i}.DiagTests.DiagnosticsState
            //strncpy(pTerminalDiagTests->DiagnosticsState, pValue,strlen(pValue)+1);

        }
        else if( strstr(ParamName, "TestSelector") )
        {
            //VoiceService.{i}.Terminal.{i}.DiagTests.TestSelector
            strncpy(pTerminalDiagTests->TestSelector, pValue,strlen(pValue)+1);
        }
        else if( strstr(ParamName, "PhoneRinging") )
        {
            //VoiceService.{i}.Terminal.{i}.DiagTests.PhoneRinging
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pTerminalDiagTests->PhoneRinging = true;
            }
            else
            {
                 pTerminalDiagTests->PhoneRinging = false;
            }
        }
        else if( strstr(ParamName, "X_RDK_TestResult"))
        {
            //VoiceService.{i}.Terminal.{i}.DiagTests.X_RDK_TestResult
            strncpy(pTerminalDiagTests->X_RDK_TestResult, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS Map_hal_dml_clockSync(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue)
{
    char *err;
    int hal_index = 0;

    if(pVoiceServiceList == NULL || ParamName == NULL || pValue == NULL)
    {
        AnscTraceError(("%s:%d:: Invalid Input Parameter[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
   
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = getVoiceService(pVoiceServiceList, ParamName);

    if (pVoiceService == NULL )
    {
        AnscTraceError(("%s:%d:: Invalid pVoiceService \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    PDML_VOICESERVICE_CLOCKSYNC pClkSync = &(pVoiceService->ClockSynchronization_obj);

    //VoiceService.{i}.ClockSynchronization.
    if( strstr(ParamName, "CurrentSource") )
    {
        //VoiceService.{i}.ClockSynchronization.CurrentSource
        strncpy(pClkSync->CurrentSource, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "Description") )
    {
        //VoiceService.{i}.ClockSynchronization.Description
        strncpy(pClkSync->Description, pValue,strlen(pValue)+1);
    }
    else if( strstr(ParamName, "AutoRevert") )
    {
        //VoiceService.{i}.ClockSynchronization.AutoRevert
        if( strstr(pValue, "true") || strstr(pValue, "1") )
        {
             pClkSync->AutoRevert = true;
        }
        else
        {
             pClkSync->AutoRevert = false;
        }
    }
    else if( strstr(ParamName, "ClockSource") )
    {
        //VoiceService.{i}.ClockSynchronization.ClockSource.{i}.
        if( (ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_ClockSynchronization_ClockSource_index(ParamName, DML_VOICESERVICE_CLKSRC, &hal_index)) ||
            ( hal_index <= 0 ))
        {
             AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }

        PDML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_LIST_T    pClkSyncSrcList = &(pClkSync->ClockSource);

        PDML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_CTRL_T    pClkSyncSrcData = pClkSyncSrcList->pdata[hal_index - 1];

        if(pClkSyncSrcData == NULL)
        {

            if( TelcoVoiceMgrDmlAddClockSource(pClkSyncSrcList, hal_index - 1) != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
                return ANSC_STATUS_FAILURE;
            }

            pClkSyncSrcData = pClkSyncSrcList->pdata[hal_index - 1];

            if(pClkSyncSrcData == NULL)
            {
                return ANSC_STATUS_FAILURE;
            }
        }

        PDML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE pClkSyncSrc = &(pClkSyncSrcData->dml);

        pClkSyncSrc->uInstanceNumber = hal_index;

        if( strstr(ParamName, "Enable") )
        {
            //VoiceService.{i}.ClockSynchronization.ClockSource.{i}.Enable
            if( strstr(pValue, "true") || strstr(pValue, "1") )
            {
                 pClkSyncSrc->Enable = true;
            }
            else
            {
                 pClkSyncSrc->Enable = false;
            }
        }
        else if( strstr(ParamName, "Order") )
        {
            //VoiceService.{i}.ClockSynchronization.ClockSource.{i}.Order
            pClkSyncSrc->Order = strtoul(pValue,&err, 10);
        }
        else if( strstr(ParamName, "Interface") )
        {
            //VoiceService.{i}.ClockSynchronization.ClockSource.{i}.Interface
            strncpy(pClkSyncSrc->Interface, pValue,strlen(pValue)+1);
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, ParamName));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
