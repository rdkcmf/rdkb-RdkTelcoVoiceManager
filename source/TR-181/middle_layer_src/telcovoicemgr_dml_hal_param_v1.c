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
#include "json_hal_client.h"
#include "ansc_platform.h"
#include "ccsp_trace.h"

#include "telcovoicemgr_dml_hal_param_v1.h"

#define STRNCPY(dest, source) strncpy(dest, source, sizeof(dest))
#define STRNCMP(str1, str2) !strncmp(str1, str2, sizeof(str2))

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

    if( *hal_index <= 0 )
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

void select_child(char *pathStart, char *ptrEnd,  char **childName)
{
    if(!(pathStart && ptrEnd && childName ) ||
                                   pathStart >= ptrEnd)
    {
        AnscTraceError(("\n%s:%d:: Invalid arguments..\n", __FUNCTION__, __LINE__));
        return;
    }
    *childName = strtok(pathStart,".");
    if ((*childName + strlen(*childName)) > ptrEnd)
    {
        *childName = NULL;
    }
}

int get_index_and_select_child(char *pathStart, char *tableName, char *ptrEnd,  char **childName)
{
    int uHalIndex = -1;
    char *strIndexVal = NULL;
    char *endVal = NULL;
    if(!(pathStart && tableName && ptrEnd && childName) ||
                                            pathStart >= ptrEnd)
    {
        AnscTraceError(("\n%s:%d:: Invalid arguments..\n", __FUNCTION__, __LINE__));
        return -1;
    }
    pathStart = strstr(pathStart, tableName);
    *childName = NULL;
    pathStart += strlen(tableName) + 1; // Move the ptr to the location of index
    if (pathStart < ptrEnd)
    {
        strIndexVal = strtok(pathStart, ".");
        if (strIndexVal && (strIndexVal+strlen(strIndexVal)) < ptrEnd) {
            uHalIndex = strtol(strIndexVal, &endVal, 0);
            if (uHalIndex > 0)
            {
                pathStart += strlen(strIndexVal) + 1; // Move the ptr to the location after "index."
                select_child(pathStart, ptrEnd, childName);
                if (childName)
                {
                    return uHalIndex - 1;
                }
            }
        }
    }
    return -1;
}

ANSC_STATUS Map_hal_dml_voiceProfile_ServiceProviderInfo(
                PTELCOVOICEMGR_DML_SERVICE_PROVIDER_INFO pVoiceProf_ServiceProviderInfo, char* childName,
                                                           char *pathEnd, char* pValue)
{
    if(!(pVoiceProf_ServiceProviderInfo && childName && pathEnd && pValue) ||
                                         pathEnd <= childName)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(childName, "Name"))
    {
        STRNCPY(pVoiceProf_ServiceProviderInfo->Name, pValue);
    }
    else if( STRNCMP(childName, "URL"))
    {
        STRNCPY(pVoiceProf_ServiceProviderInfo->URL, pValue);
    }
    else if( STRNCMP(childName, "ContactPhoneNumber"))
    {
        STRNCPY(pVoiceProf_ServiceProviderInfo->ContactPhoneNumber, pValue);
    }
    else if( STRNCMP(childName, "EmailAddress"))
    {
        STRNCPY(pVoiceProf_ServiceProviderInfo->EmailAddress, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, childName, pValue));
        return ANSC_STATUS_FAILURE;
    }
}

ANSC_STATUS Map_hal_dml_voiceProfile_SIP_EventSubscribe(
               DML_SIP_EVENTSUBSCRIBE_LIST_T* psipEvtSubscribeList, char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(psipEvtSubscribeList && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "EventSubscribe", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }

    DML_SIP_EVENTSUBSCRIBE_CTRL_T* psipEvtSubscribeData = psipEvtSubscribeList->pdata[uTableIndex];

    if(psipEvtSubscribeData == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileSIPEventSubscribe(psipEvtSubscribeList, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        psipEvtSubscribeData = psipEvtSubscribeList->pdata[uTableIndex];
        if(psipEvtSubscribeData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE* pVoiceProfile_SIP_EventSubscribe = &(psipEvtSubscribeData->dml);

    pVoiceProfile_SIP_EventSubscribe->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "Event"))
    {
        STRNCPY(pVoiceProfile_SIP_EventSubscribe->Event, pValue);
    }
    else if( STRNCMP(strChildName, "NotifierPort"))
    {
        pVoiceProfile_SIP_EventSubscribe->NotifierPort = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "NotifierTransport"))
    {
        STRNCPY(pVoiceProfile_SIP_EventSubscribe->NotifierTransport, pValue);
    }
    else if( STRNCMP(strChildName, "Notifier"))
    {
        STRNCPY(pVoiceProfile_SIP_EventSubscribe->Notifier, pValue);
    }
    else if( STRNCMP(strChildName, "ExpireTime"))
    {
        pVoiceProfile_SIP_EventSubscribe->ExpireTime = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_SIP_ResponseMap(
               DML_SIP_RESPONSEMAP_LIST_T* pSIPResponseMapList, char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pSIPResponseMapList && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "ResponseMap", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_SIP_RESPONSEMAP_CTRL_T* pSIPResponseMapData = pSIPResponseMapList->pdata[uTableIndex];

    if(pSIPResponseMapData == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileSIPRespMap(pSIPResponseMapList, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pSIPResponseMapData = pSIPResponseMapList->pdata[uTableIndex];
        if(pSIPResponseMapData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_SIP_RESPONSEMAP* pVoiceProfile_SIP_RespMap = &(pSIPResponseMapData->dml);

    pVoiceProfile_SIP_RespMap->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "SIPResponseNumber"))
    {
        pVoiceProfile_SIP_RespMap->SIPResponseNumber = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TextMessage"))
    {
        STRNCPY( pVoiceProfile_SIP_RespMap->TextMessage, pValue);
    }
    else if( STRNCMP(strChildName, "Tone"))
    {
        pVoiceProfile_SIP_RespMap->Tone = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_SIP(PTELCOVOICEMGR_DML_SIP pVoiceProf_SIP,
                                          char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;
    ANSC_STATUS retStatus = ANSC_STATUS_FAILURE;

    if(!(pVoiceProf_SIP && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(strChildName, "ProxyServerPort"))
    {
        pVoiceProf_SIP->ProxyServerPort = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ProxyServerTransport"))
    {
        STRNCPY(pVoiceProf_SIP->ProxyServerTransport, pValue);
    }
    else if( STRNCMP(strChildName, "ProxyServer"))
    {
        STRNCPY(pVoiceProf_SIP->ProxyServer, pValue);
    }
    else if( STRNCMP(strChildName, "RegistrarServerPort"))
    {
        pVoiceProf_SIP->RegistrarServerPort = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RegistrarServerTransport"))
    {
        STRNCPY(pVoiceProf_SIP->RegistrarServerTransport, pValue);
    }
    else if( STRNCMP(strChildName, "RegistrarServer"))
    {
        STRNCPY(pVoiceProf_SIP->RegistrarServer, pValue);
    }
    else if( STRNCMP(strChildName, "UserAgentDomain"))
    {
        STRNCPY(pVoiceProf_SIP->UserAgentDomain, pValue);
    }
    else if( STRNCMP(strChildName, "UserAgentPort"))
    {
        pVoiceProf_SIP->UserAgentPort = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "UserAgentTransport"))
    {
        STRNCPY(pVoiceProf_SIP->UserAgentTransport, pValue);
    }
    else if( STRNCMP(strChildName, "OutboundProxyPort"))
    {
        pVoiceProf_SIP->OutboundProxyPort = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "OutboundProxy"))
    {
        STRNCPY(pVoiceProf_SIP->OutboundProxy, pValue);
    }
    else if( STRNCMP(strChildName, "Organization"))
    {
        STRNCPY(pVoiceProf_SIP->Organization, pValue);
    }
    else if( STRNCMP(strChildName, "RegistrationPeriod"))
    {
        pVoiceProf_SIP->RegistrationPeriod = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerT1"))
    {
        pVoiceProf_SIP->TimerT1 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerT2"))
    {
        pVoiceProf_SIP->TimerT2 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerT4"))
    {
        pVoiceProf_SIP->TimerT4 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerA"))
    {
        pVoiceProf_SIP->TimerA = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerB"))
    {
        pVoiceProf_SIP->TimerB = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerC"))
    {
        pVoiceProf_SIP->TimerC = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerD"))
    {
        pVoiceProf_SIP->TimerD = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerE"))
    {
        pVoiceProf_SIP->TimerE = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerF"))
    {
        pVoiceProf_SIP->TimerF = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerG"))
    {
        pVoiceProf_SIP->TimerG = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerH"))
    {
        pVoiceProf_SIP->TimerH = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerI"))
    {
        pVoiceProf_SIP->TimerI = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerJ"))
    {
        pVoiceProf_SIP->TimerJ = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TimerK"))
    {
        pVoiceProf_SIP->TimerK = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "InviteExpires"))
    {
        pVoiceProf_SIP->InviteExpires = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ReInviteExpires"))
    {
        pVoiceProf_SIP->ReInviteExpires = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RegisterExpires"))
    {
        pVoiceProf_SIP->RegisterExpires = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RegistersMinExpires"))
    {
        pVoiceProf_SIP->RegistersMinExpires = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RegisterRetryInterval"))
    {
        pVoiceProf_SIP->RegisterRetryInterval = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "InboundAuthUsername"))
    {
        STRNCPY(pVoiceProf_SIP->InboundAuthUsername, pValue);
    }
    else if( STRNCMP(strChildName, "InboundAuthPassword"))
    {
        STRNCPY(pVoiceProf_SIP->InboundAuthPassword, pValue);
    }
    else if( STRNCMP(strChildName, "InboundAuth"))
    {
        if(STRNCMP(pValue, "None"))
        {
            pVoiceProf_SIP->InboundAuth = SIP_INBOUNDAUTH_NONE;
        }
        else if(STRNCMP(pValue, "Digest"))
        {
            pVoiceProf_SIP->InboundAuth = SIP_INBOUNDAUTH_DIGEST;
        }
    }
    else if( STRNCMP(strChildName, "UseCodecPriorityInSDPResponse"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProf_SIP->UseCodecPriorityInSDPResponse = true;
        }
        else
        {
            pVoiceProf_SIP->UseCodecPriorityInSDPResponse = false;
        }
    }
    else if( STRNCMP(strChildName, "DSCPMark"))
    {
        pVoiceProf_SIP->DSCPMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "VLANIDMark"))
    {
        pVoiceProf_SIP->VLANIDMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "EthernetPriorityMark"))
    {
        pVoiceProf_SIP->EthernetPriorityMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "X_RDK_SKBMark"))
    {
        pVoiceProf_SIP->X_RDK_SKBMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "X_RDK-Central_COM_ConferencingURI"))
    {
        STRNCPY(pVoiceProf_SIP->ConferencingURI, pValue);
    }
    else if( STRNCMP(strChildName, "X_RDK-Central_COM_NetworkDisconnect"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProf_SIP->NetworkDisconnect = true;
        }
        else
        {
            pVoiceProf_SIP->NetworkDisconnect = false;
        }
    }
    else if( STRNCMP(strChildName, "X_RDK_PRACKRequired"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProf_SIP->X_RDK_PRACKRequired = true;
        }
        else
        {
            pVoiceProf_SIP->X_RDK_PRACKRequired = false;
        }
    } 
    else if( STRNCMP(strChildName, "EventSubscribe"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.SIP.EventSubscribe.{i}.
        DML_SIP_EVENTSUBSCRIBE_LIST_T* psipEvtSubscribeList = &(pVoiceProf_SIP->SipEventSubscribeList);
        retStatus = Map_hal_dml_voiceProfile_SIP_EventSubscribe(
                       &(pVoiceProf_SIP->SipEventSubscribeList), strChildName, pathEnd, pValue);
        if( retStatus != ANSC_STATUS_SUCCESS) 
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

    }
    else if( STRNCMP(strChildName, "ResponseMap"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.SIP.ResponseMap.{i}.
        DML_SIP_RESPONSEMAP_LIST_T* pSIPResponseMapList = &(pVoiceProf_SIP->SipResponseMapList);
        retStatus = Map_hal_dml_voiceProfile_SIP_ResponseMap(
                        &(pVoiceProf_SIP->SipResponseMapList), strChildName, pathEnd, pValue);
        if( retStatus != ANSC_STATUS_SUCCESS) 
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_SIP_EventSubscribe(
               DML_LINE_SIP_EVENTSUBSCRIBE_LIST_T* pVoiceProfile_Line_SIP_EvtSubscribe_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
     int uTableIndex = 0;
     char *strChildName = NULL;

     if(!(pVoiceProfile_Line_SIP_EvtSubscribe_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "EventSubscribe", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }

    DML_LINE_SIP_EVENTSUBSCRIBE_CTRL_T* pVoiceProfile_Line_SIP_EvtSubscribe_Data =
                                         pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[uTableIndex];

    if(pVoiceProfile_Line_SIP_EvtSubscribe_Data == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileLineSIPEvtSubscribe(pVoiceProfile_Line_SIP_EvtSubscribe_List,
                                                                       uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pVoiceProfile_Line_SIP_EvtSubscribe_Data = pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[uTableIndex];
        if(pVoiceProfile_Line_SIP_EvtSubscribe_Data == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_LINE_SIP_EVENTSUBSCRIBE* pVoiceProfile_Line_SIP_EventSubscribe = &(pVoiceProfile_Line_SIP_EvtSubscribe_Data->dml);

    pVoiceProfile_Line_SIP_EventSubscribe->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "Event"))
    {
        STRNCPY(pVoiceProfile_Line_SIP_EventSubscribe->Event, pValue);
    }
    else if( STRNCMP(strChildName, "AuthUserName"))
    {
        STRNCPY(pVoiceProfile_Line_SIP_EventSubscribe->AuthUserName, pValue);
    }
    else if( STRNCMP(strChildName, "AuthPassword"))
    {
        STRNCPY(pVoiceProfile_Line_SIP_EventSubscribe->AuthPassword, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }

}


ANSC_STATUS Map_hal_dml_voiceProfileLine_SIP(TELCOVOICEMGR_DML_LINE_SIP* pVoiceProfile_Line_SIP,
                                                  char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    ANSC_STATUS retStatus = ANSC_STATUS_FAILURE;

    if(!(pVoiceProfile_Line_SIP && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "AuthUserName"))
    {
        STRNCPY(pVoiceProfile_Line_SIP->AuthUserName, pValue);
    }
    else if( STRNCMP(strChildName, "AuthPassword"))
    {
        STRNCPY(pVoiceProfile_Line_SIP->AuthPassword, pValue);
    }
    else if( STRNCMP(strChildName, "URI"))
    {
        STRNCPY(pVoiceProfile_Line_SIP->URI, pValue);
    }
    else if( STRNCMP(strChildName, "EventSubscribe"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP.EventSubscribe.{i}.
        return Map_hal_dml_voiceProfileLine_SIP_EventSubscribe( &(pVoiceProfile_Line_SIP->LineSipEventSubscribeList),
                                                                             strChildName, pathEnd, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_MGCP(TELCOVOICEMGR_DML_LINE_MGCP* pVoiceProfile_Line_MGCP,
                                                  char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;

    if(!(pVoiceProfile_Line_MGCP && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
    }
    else if( STRNCMP(strChildName, "LineName"))
    {
        STRNCPY(pVoiceProfile_Line_MGCP->LineName, pValue);
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
    }
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_H323(TELCOVOICEMGR_DML_LINE_H323* pVoiceProfile_Line_H323,
                                                  char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;

    if(!(pVoiceProfile_Line_H323 && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
    }
    else if( STRNCMP(strChildName, "H323ID"))
    {
        STRNCPY(pVoiceProfile_Line_H323->H323ID, pValue);
        return ANSC_STATUS_SUCCESS;
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
    }
    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_Ringer_Event(
                DML_LINE_RINGER_EVENT_LIST_T* pVoiceProfile_Line_Ringer_Event_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Line_Ringer_Event_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Event", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_LINE_RINGER_PATTERN_CTRL_T* pVoiceProfile_Line_Ringer_Event_Data = pVoiceProfile_Line_Ringer_Event_List->pdata[uTableIndex];

    if(pVoiceProfile_Line_Ringer_Event_Data == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileLineRingerEvent(pVoiceProfile_Line_Ringer_Event_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pVoiceProfile_Line_Ringer_Event_Data = pVoiceProfile_Line_Ringer_Event_List->pdata[uTableIndex];
        if(pVoiceProfile_Line_Ringer_Event_Data == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_LINE_RINGER_EVENT* pVoiceProfile_Line_Ringer_Event = &(pVoiceProfile_Line_Ringer_Event_Data->dml);

    pVoiceProfile_Line_Ringer_Event->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "Function"))
    {
        STRNCPY(pVoiceProfile_Line_Ringer_Event->Function, pValue);
    }
    else if( STRNCMP(strChildName, "RingID"))
    {
        pVoiceProfile_Line_Ringer_Event->RingID = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS Map_hal_dml_voiceProfileLine_Ringer_Desc(
                DML_LINE_RINGER_DESCRIPTION_LIST_T* pVoiceProfile_Line_Ringer_Desc_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Line_Ringer_Desc_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Description", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_LINE_RINGER_DESCRIPTION_CTRL_T* pVoiceProfile_Line_Ringer_Desc_Data = pVoiceProfile_Line_Ringer_Desc_List->pdata[uTableIndex];

    if(pVoiceProfile_Line_Ringer_Desc_Data == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileLineRingerDesc(pVoiceProfile_Line_Ringer_Desc_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;   
        }
        pVoiceProfile_Line_Ringer_Desc_Data = pVoiceProfile_Line_Ringer_Desc_List->pdata[uTableIndex];
        if(pVoiceProfile_Line_Ringer_Desc_Data == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_LINE_RINGER_DESCRIPTION* pVoiceProfile_Line_Ringer_Desc = &(pVoiceProfile_Line_Ringer_Desc_Data->dml);

    pVoiceProfile_Line_Ringer_Desc->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "EntryID"))
    {
        pVoiceProfile_Line_Ringer_Desc->EntryID = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RingEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_Ringer_Desc->RingEnable = true;
        }
        else
        {
            pVoiceProfile_Line_Ringer_Desc->RingEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "RingName"))
    {
        STRNCPY(pVoiceProfile_Line_Ringer_Desc->RingName, pValue);
    }
    else if( STRNCMP(strChildName, "RingPattern"))
    {
        pVoiceProfile_Line_Ringer_Desc->RingPattern = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RingFile"))
    {
        STRNCPY(pVoiceProfile_Line_Ringer_Desc->RingFile, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_Ringer_Pattern(
                DML_LINE_RINGER_PATTERN_LIST_T* pVoiceProfile_Line_Ringer_Pattern_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Line_Ringer_Pattern_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Pattern", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_LINE_RINGER_PATTERN_CTRL_T* pVoiceProfile_Line_Ringer_Pattern_Data = pVoiceProfile_Line_Ringer_Pattern_List->pdata[uTableIndex];

    if(pVoiceProfile_Line_Ringer_Pattern_Data == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileLineRingerPattern(pVoiceProfile_Line_Ringer_Pattern_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;   
        }
        pVoiceProfile_Line_Ringer_Pattern_Data = pVoiceProfile_Line_Ringer_Pattern_List->pdata[uTableIndex];
        if(pVoiceProfile_Line_Ringer_Pattern_Data == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_LINE_RINGER_PATTERN* pVoiceProfile_Line_Ringer_Pattern = &(pVoiceProfile_Line_Ringer_Pattern_Data->dml);

    pVoiceProfile_Line_Ringer_Pattern->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "EntryID"))
    {
        pVoiceProfile_Line_Ringer_Pattern->EntryID = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RingerOn"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_Ringer_Pattern->RingerOn = true;
        }
        else
        {
            pVoiceProfile_Line_Ringer_Pattern->RingerOn = false;
        }
    }
    else if( STRNCMP(strChildName, "Duration"))
    {
        pVoiceProfile_Line_Ringer_Pattern->Duration = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "NextEntryID"))
    {
        pVoiceProfile_Line_Ringer_Pattern->NextEntryID = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_Ringer(TELCOVOICEMGR_DML_LINE_RINGER* pVoiceProfile_Line_Ringer,
                                                  char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;

    if(!(pVoiceProfile_Line_Ringer && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(strChildName, "Event"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.Event.{i}
        return Map_hal_dml_voiceProfileLine_Ringer_Event( &(pVoiceProfile_Line_Ringer->LineRingerEventList),
                                                                               strChildName, pathEnd, pValue);
    }
    else if( STRNCMP(strChildName, "Description"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.Description.{i}
        return Map_hal_dml_voiceProfileLine_Ringer_Desc( &(pVoiceProfile_Line_Ringer->LineRingerDescriptionList),
                                                                                  strChildName, pathEnd, pValue);
    }
    else if( STRNCMP(strChildName, "Pattern"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.Pattern.{i}
        return Map_hal_dml_voiceProfileLine_Ringer_Pattern( &(pVoiceProfile_Line_Ringer->LineRingerPatternList),
                                                                                  strChildName, pathEnd, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }

}

ANSC_STATUS Map_hal_dml_voiceProfileLine_CallingFeatures(
                 TELCOVOICEMGR_DML_LINE_CALLINGFEATURES* pVoiceProfile_Line_CallingFeatures,
                                                  char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Line_CallingFeatures && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(strChildName, "CallerIDEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->CallerIDEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->CallerIDEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "CallerIDNameEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->CallerIDNameEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->CallerIDNameEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "CallerIDName"))
    {
        STRNCPY(pVoiceProfile_Line_CallingFeatures->CallerIDName, pValue);
    }
    else if( STRNCMP(strChildName, "X_RDK-Central_COM_ConferenceCallingEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->ConferenceCallingEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->ConferenceCallingEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "X_RDK-Central_COM_HoldEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->HoldEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->HoldEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "X_RDK-Central_COM_PhoneCallerIDEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->PhoneCallerIDEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->PhoneCallerIDEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "CallWaitingEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->CallWaitingEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->CallWaitingEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "CallWaitingStatus"))
    {
        if(STRNCMP(pValue, "Disabled"))
        {
            pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_DISABLED;
        }
        else if(STRNCMP(pValue, "Idle"))
        {
            pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_IDLE;
        }
        else if(STRNCMP(pValue, "SecondaryRinging"))
        {
            pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_SECONDARY_RINGING;
        }
        else if(STRNCMP(pValue, "SecondaryConnecting"))
        {
            pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_SECONDARY_CONNECTING;
        }
        else if(STRNCMP(pValue, "SecondaryConnected"))
        {
            pVoiceProfile_Line_CallingFeatures->CallWaitingStatus = VOICE_CW_STATE_SECONDARY_CONNECTED;
        }
    }
    else if( STRNCMP(strChildName, "MaxSessions"))
    {
        pVoiceProfile_Line_CallingFeatures->MaxSessions = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ConferenceCallingStatus"))
    {
        if(STRNCMP(pValue, "Disabled"))
        {
            pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_DISABLED;
        }
        else if(STRNCMP(pValue, "Idle"))
        {
            pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_IDLE;
        }
        else if(STRNCMP(pValue, "SecondaryCalling"))
        {
            pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_SECONDARY_RINGING;
        }
        else if(STRNCMP(pValue, "SecondaryConnecting"))
        {
            pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_SECONDARY_CONNECTING;
        }
        else if(STRNCMP(pValue, "SecondaryConnected"))
        {
            pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_SECONDARY_CONNECTED;
        }
        else if(STRNCMP(pValue, "InConferenceCall"))
        {
            pVoiceProfile_Line_CallingFeatures->ConferenceCallingStatus = VOICE_CONFERENCE_CALLING_STATE_IN_CONFERENCE_CALL;
        }
    }
    else if( STRNCMP(strChildName, "ConferenceCallingSessionCount"))
    {
        pVoiceProfile_Line_CallingFeatures->ConferenceCallingSessionCount = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "CallForwardUnconditionalEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->CallForwardUnconditionalEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->CallForwardUnconditionalEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "CallForwardUnconditionalNumber"))
    {
        STRNCPY(pVoiceProfile_Line_CallingFeatures->CallForwardUnconditionalNumber, pValue);
    }
    else if( STRNCMP(strChildName, "CallForwardOnBusyEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->CallForwardOnBusyEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->CallForwardOnBusyEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "CallForwardOnBusyNumber"))
    {
        STRNCPY(pVoiceProfile_Line_CallingFeatures->CallForwardOnBusyNumber, pValue);
    }
    else if( STRNCMP(strChildName, "CallForwardOnNoAnswerEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->CallForwardOnNoAnswerEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->CallForwardOnNoAnswerEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "CallForwardOnNoAnswerNumber"))
    {
        STRNCPY(pVoiceProfile_Line_CallingFeatures->CallForwardOnNoAnswerNumber, pValue);
    }
    else if( STRNCMP(strChildName, "CallForwardOnNoAnswerRingCount"))
    {
        pVoiceProfile_Line_CallingFeatures->CallForwardOnNoAnswerRingCount = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "CallTransferEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->CallTransferEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->CallTransferEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "MWIEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->MWIEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->MWIEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "MessageWaiting"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->MessageWaiting = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->MessageWaiting = false;
        }
    }
    else if( STRNCMP(strChildName, "AnonymousCallBlockEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->AnonymousCallBlockEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->AnonymousCallBlockEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "AnonymousCalEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->AnonymousCalEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->AnonymousCalEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "DoNotDisturbEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->DoNotDisturbEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->DoNotDisturbEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "CallReturnEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_CallingFeatures->CallReturnEnable = true;
        }
        else
        {
            pVoiceProfile_Line_CallingFeatures->CallReturnEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "RepeatDialEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
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
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_VoiceProcessing(
                 TELCOVOICEMGR_DML_LINE_VOICEPROCESSING* pVoiceProfile_Line_VoiceProcessing,
                                                  char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Line_VoiceProcessing && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(strChildName, "TransmitGain"))
    {
        pVoiceProfile_Line_VoiceProcessing->TransmitGain = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ReceiveGain"))
    {
        pVoiceProfile_Line_VoiceProcessing->ReceiveGain = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "EchoCancellationEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_VoiceProcessing->EchoCancellationEnable = true;
        }
        else
        {
            pVoiceProfile_Line_VoiceProcessing->EchoCancellationEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "EchoCancellationInUse"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_VoiceProcessing->EchoCancellationInUse = true;
        }
        else
        {
            pVoiceProfile_Line_VoiceProcessing->EchoCancellationInUse = false;
        }
    }
    else if( STRNCMP(strChildName, "EchoCancellationTail"))
    {
        pVoiceProfile_Line_VoiceProcessing->EchoCancellationTail = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_Stats(
                      TELCOVOICEMGR_DML_LINE_STATS* pVoiceProfile_Line_Stats,
                                                  char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Line_Stats && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "ResetStatistics"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_Stats->ResetStatistics = true;
        }
        else
        {
            pVoiceProfile_Line_Stats->ResetStatistics = false;
        }
    }
    else if( STRNCMP(strChildName, "PacketsSent"))
    {
        pVoiceProfile_Line_Stats->PacketsSent = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "PacketsReceived"))
    {
        pVoiceProfile_Line_Stats->PacketsReceived = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "BytesSent"))
    {
        pVoiceProfile_Line_Stats->BytesSent = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "BytesReceived"))
    {
        pVoiceProfile_Line_Stats->BytesReceived = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "PacketsLost"))
    {
        pVoiceProfile_Line_Stats->PacketsLost = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Overruns"))
    {
        pVoiceProfile_Line_Stats->Overruns = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Underruns"))
    {
        pVoiceProfile_Line_Stats->Underruns = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "IncomingCallsReceived"))
    {
        pVoiceProfile_Line_Stats->IncomingCallsReceived = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "IncomingCallsAnswered"))
    {
        pVoiceProfile_Line_Stats->IncomingCallsAnswered = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "IncomingCallsConnected"))
    {
        pVoiceProfile_Line_Stats->IncomingCallsConnected = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "IncomingCallsFailed"))
    {
        pVoiceProfile_Line_Stats->IncomingCallsFailed = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "OutgoingCallsAttempted"))
    {
        pVoiceProfile_Line_Stats->OutgoingCallsAttempted = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "OutgoingCallsAnswered"))
    {
        pVoiceProfile_Line_Stats->OutgoingCallsAnswered = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "OutgoingCallsConnected"))
    {
        pVoiceProfile_Line_Stats->OutgoingCallsConnected = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "OutgoingCallsFailed"))
    {
        pVoiceProfile_Line_Stats->OutgoingCallsFailed = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "CallsDropped"))
    {
        pVoiceProfile_Line_Stats->CallsDropped = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TotalCallTime"))
    {
        pVoiceProfile_Line_Stats->TotalCallTime = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ServerDownTime"))
    {
        pVoiceProfile_Line_Stats->ServerDownTime = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ReceivePacketLossRate"))
    {
        pVoiceProfile_Line_Stats->ReceivePacketLossRate = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "FarEndPacketLossRate"))
    {
        pVoiceProfile_Line_Stats->FarEndPacketLossRate = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ReceiveInterarrivalJitter"))
    {
        pVoiceProfile_Line_Stats->ReceiveInterarrivalJitter = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "FarEndInterarrivalJitter"))
    {
        pVoiceProfile_Line_Stats->FarEndInterarrivalJitter = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RoundTripDelay"))
    {
        pVoiceProfile_Line_Stats->RoundTripDelay = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "AverageReceiveInterarrivalJitter"))
    {
        pVoiceProfile_Line_Stats->AverageReceiveInterarrivalJitter = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "AverageFarEndInterarrivalJitter"))
    {
        pVoiceProfile_Line_Stats->AverageFarEndInterarrivalJitter = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "AverageRoundTripDelay"))
    {
        pVoiceProfile_Line_Stats->AverageRoundTripDelay = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS Map_hal_dml_voiceProfileLine_Session(
                DML_LINE_SESSION_LIST_T* pVoiceProfile_Line_Session_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Line_Session_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Session", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_LINE_SESSION_CTRL_T* pVoiceProfile_Line_Session_Data = pVoiceProfile_Line_Session_List->pdata[uTableIndex];

    if(pVoiceProfile_Line_Session_Data == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileLineSession(pVoiceProfile_Line_Session_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pVoiceProfile_Line_Session_Data = pVoiceProfile_Line_Session_List->pdata[uTableIndex];
        if(pVoiceProfile_Line_Session_Data == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_LINE_SESSION* pVoiceProfile_Line_Session = &(pVoiceProfile_Line_Session_Data->dml);

    pVoiceProfile_Line_Session->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "SessionStartTime"))
    {
        //dateTime
        STRNCPY(pVoiceProfile_Line_Session->SessionStartTime, pValue);
    }
    else if( STRNCMP(strChildName, "SessionDuration"))
    {
        pVoiceProfile_Line_Session->SessionDuration = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "FarEndIPAddress"))
    {
        STRNCPY(pVoiceProfile_Line_Session->FarEndIPAddress, pValue);
    }
    else if( STRNCMP(strChildName, "FarEndUDPPort"))
    {
        pVoiceProfile_Line_Session->FarEndUDPPort = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "LocalUDPPort"))
    {
        pVoiceProfile_Line_Session->LocalUDPPort = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_CodecList(
                 DML_LINE_CODECLIST_LIST_T* pVoiceProfile_Line_CodecList_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Line_CodecList_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "List", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_LINE_CODECLIST_CTRL_T* pVoiceProfile_Line_CodecList_Data = pVoiceProfile_Line_CodecList_List->pdata[uTableIndex];

    if(pVoiceProfile_Line_CodecList_Data == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileLineCodecList(pVoiceProfile_Line_CodecList_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pVoiceProfile_Line_CodecList_Data = pVoiceProfile_Line_CodecList_List->pdata[uTableIndex];
        if(pVoiceProfile_Line_CodecList_Data == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_LINE_CODECLIST* pVoiceProfile_Line_Codec_List = &(pVoiceProfile_Line_CodecList_Data->dml);

    pVoiceProfile_Line_Codec_List->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "EntryID"))
    {
        pVoiceProfile_Line_Codec_List->EntryID = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Codec"))
    {
        STRNCPY(pVoiceProfile_Line_Codec_List->Codec, pValue);
    }
    else if( STRNCMP(strChildName, "BitRate"))
    {
        pVoiceProfile_Line_Codec_List->BitRate = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "PacketizationPeriod"))
    {
        STRNCPY(pVoiceProfile_Line_Codec_List->PacketizationPeriod, pValue);
    }
    else if( STRNCMP(strChildName, "SilenceSuppression"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_Codec_List->SilenceSuppression = true;
        }
        else
        {
            pVoiceProfile_Line_Codec_List->SilenceSuppression = false;
        }
    }
    else if( STRNCMP(strChildName, "Enable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_Codec_List->Enable = true;
        }
        else
        {
            pVoiceProfile_Line_Codec_List->Enable = false;
        }
    }
    else if( STRNCMP(strChildName, "Priority"))
    {
        pVoiceProfile_Line_Codec_List->Priority = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfileLine_Codec(
                TELCOVOICEMGR_DML_LINE_CODEC* pVoiceProfile_Line_Codec,
                                     char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Line_Codec && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(strChildName, "TransmitCodec"))
    {
        STRNCPY(pVoiceProfile_Line_Codec->TransmitCodec, pValue);
    }
    else if( STRNCMP(strChildName, "ReceiveCodec"))
    {
        STRNCPY(pVoiceProfile_Line_Codec->ReceiveCodec, pValue);
    }
    else if( STRNCMP(strChildName, "TransmitBitRate"))
    {
        pVoiceProfile_Line_Codec->TransmitBitRate = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ReceiveBitRate"))
    {
        pVoiceProfile_Line_Codec->ReceiveBitRate = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TransmitSilenceSuppression"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_Codec->TransmitSilenceSuppression = true;
        }
        else
        {
            pVoiceProfile_Line_Codec->TransmitSilenceSuppression = false;
        }
    }
    else if( STRNCMP(strChildName, "ReceiveSilenceSuppression"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line_Codec->ReceiveSilenceSuppression = true;
        }
        else
        {
            pVoiceProfile_Line_Codec->ReceiveSilenceSuppression = false;
        }
    }
    else if( STRNCMP(strChildName, "TransmitPacketizationPeriod"))
    {
        pVoiceProfile_Line_Codec->TransmitPacketizationPeriod = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "List"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Codec.List.{i}.
        return Map_hal_dml_voiceProfileLine_CodecList(
                            &(pVoiceProfile_Line_Codec->LineCodecList),
                                                       strChildName, pathEnd, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS Map_hal_dml_voiceProfileLine(DML_LINE_LIST_T* pVoiceProfile_Line_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.
    if(!(pVoiceProfile_Line_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Line", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }

    DML_LINE_CTRL_T* pVoiceProfile_Line_Data = pVoiceProfile_Line_List->pdata[uTableIndex];

    if(pVoiceProfile_Line_Data == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddVoiceProfileLine(pVoiceProfile_Line_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pVoiceProfile_Line_Data = pVoiceProfile_Line_List->pdata[uTableIndex];
        if(pVoiceProfile_Line_Data == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_LINE* pVoiceProfile_Line = &(pVoiceProfile_Line_Data->dml);

    pVoiceProfile_Line->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "Enable"))
    {
        if(STRNCMP(pValue, "Disabled"))
        {
            pVoiceProfile_Line->Enable = DISABLED;
        }
        else if(STRNCMP(pValue, "Quiescent"))
        {
            pVoiceProfile_Line->Enable = QUIESCENT;
        }
        else if(STRNCMP(pValue, "Enabled"))
        {
            pVoiceProfile_Line->Enable = ENABLED;
        }
    }
    else if( STRNCMP(strChildName, "DirectoryNumber"))
    {
        STRNCPY(pVoiceProfile_Line->DirectoryNumber, pValue);
    }
    else if( STRNCMP(strChildName, "Status"))
    {
        if(STRNCMP(pValue, "Up"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_UP;
        }
        else if(STRNCMP(pValue, "Initializing"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_INITIALIZING;
        }
        else if(STRNCMP(pValue, "Registering"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_REGISTERING;
        }
        else if(STRNCMP(pValue, "Unregistering"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_UNREGISTERING;
        }
        else if(STRNCMP(pValue, "Error"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_ERROR;
        }
        else if(STRNCMP(pValue, "Testing"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_TESTING;
        }
        else if(STRNCMP(pValue, "Quiescent"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_QUIESCENT;
        }
        else if(STRNCMP(pValue, "Disabled"))
        {
            pVoiceProfile_Line->Status = VOICE_LINE_STATE_DISABLED;
        }
    }
    else if( STRNCMP(strChildName, "CallState"))
    {
        if(STRNCMP(pValue, "Idle"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_IDLE;
        }
        else if(STRNCMP(pValue, "Calling"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_CALLING;
        }
        else if(STRNCMP(pValue, "Ringing"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_RINGING;
        }
        else if(STRNCMP(pValue, "Connecting"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_CONNECTING;
        }
        else if(STRNCMP(pValue, "InCall"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_INCALL;
        }
        else if(STRNCMP(pValue, "Hold"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_HOLD;
        }
        else if(STRNCMP(pValue, "Disconnecting"))
        {
            pVoiceProfile_Line->CallState = VOICE_CALL_STATE_DISCONNECTING;
        }
    }
    else if( STRNCMP(strChildName, "PhyReferenceList"))
    {
        STRNCPY(pVoiceProfile_Line->PhyReferenceList, pValue);
    }
    else if( STRNCMP(strChildName, "RingMuteStatus"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Line->RingMuteStatus = true;
        }
        else
        {
            pVoiceProfile_Line->RingMuteStatus = false;
        }
    }
    else if( STRNCMP(strChildName, "RingVolumeStatus"))
    {
        pVoiceProfile_Line->RingVolumeStatus = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "X_RDK_OutboundProxyAddresses"))
    {
        STRNCPY(pVoiceProfile_Line->X_RDK_OutboundProxyAddresses, pValue);
    }
    else if( STRNCMP(strChildName, "SIP"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP.
        return Map_hal_dml_voiceProfileLine_SIP( &(pVoiceProfile_Line->LineSipObj), 
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
    }
    else if( STRNCMP(strChildName, "MGCP"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.MGCP.
        return Map_hal_dml_voiceProfileLine_MGCP( &(pVoiceProfile_Line->LineMGCPObj), 
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
    }
    else if( STRNCMP(strChildName, "H323"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.H323.
        return Map_hal_dml_voiceProfileLine_MGCP( &(pVoiceProfile_Line->LineH323Obj), 
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
    }
    else if( STRNCMP(strChildName, "Ringer"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Ringer.
        return Map_hal_dml_voiceProfileLine_Ringer( &(pVoiceProfile_Line->LineRingerObj), 
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
    }
    else if( STRNCMP(strChildName, "CallingFeatures"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.CallingFeatures.
        return Map_hal_dml_voiceProfileLine_CallingFeatures( &(pVoiceProfile_Line->LineCallingFeaturesObj), 
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
    }
    else if( STRNCMP(strChildName, "VoiceProcessing"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.VoiceProcessing.
        return Map_hal_dml_voiceProfileLine_VoiceProcessing( &(pVoiceProfile_Line->LineVoiceProcessingObj), 
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);

    }
    else if( STRNCMP(strChildName, "Codec"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Codec.
        TELCOVOICEMGR_DML_LINE_CODEC* pVoiceProfile_Line_Codec = &(pVoiceProfile_Line->LineCodecObj);

        return Map_hal_dml_voiceProfileLine_Codec( &(pVoiceProfile_Line->LineCodecObj),
                                     strChildName + (strlen(strChildName) + 1), ptrEnd, pValue);
    }
    else if( STRNCMP(strChildName, "Session"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Session.{i}.
        return Map_hal_dml_voiceProfileLine_Session( &(pVoiceProfile_Line->LineSessionList),
                                                                     strChildName, ptrEnd, pValue);
    }
    else if( STRNCMP(strChildName, "Stats"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Stats.
        return Map_hal_dml_voiceProfileLine_Stats( &(pVoiceProfile_Line->LineStatsObj), 
                               strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, paramName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_MGCP(TELCOVOICEMGR_DML_MGCP* pVoiceProfile_MGCP,
                                          char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_MGCP && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "CallAgent1"))
    {
        STRNCPY(pVoiceProfile_MGCP->CallAgent1, pValue);
    }
    else if( STRNCMP(strChildName, "CallAgentPort1"))
    {
        pVoiceProfile_MGCP->CallAgentPort1 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "CallAgent2"))
    {
        STRNCPY(pVoiceProfile_MGCP->CallAgent2, pValue);
    }
    else if( STRNCMP(strChildName, "CallAgentPort2"))
    {
        pVoiceProfile_MGCP->CallAgentPort2 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RetranIntervalTimer"))
    {
        pVoiceProfile_MGCP->RetranIntervalTimer = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "MaxRetranCount"))
    {
        pVoiceProfile_MGCP->MaxRetranCount = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RegisterMode"))
    {
        if(STRNCMP(pValue, "WildCard"))
        {
            pVoiceProfile_MGCP->RegisterMode = MGCP_REGISTERMODE_WILDCARD;
        }
        else if(STRNCMP(pValue, "Individual"))
        {
            pVoiceProfile_MGCP->RegisterMode = MGCP_REGISTERMODE_INDIVIDUAL;
        }
    }
    else if( STRNCMP(strChildName, "LocalPort"))
    {
        pVoiceProfile_MGCP->LocalPort = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Domain"))
    {
        STRNCPY(pVoiceProfile_MGCP->Domain, pValue);
    }
    else if( STRNCMP(strChildName, "User"))
    {
        STRNCPY(pVoiceProfile_MGCP->User, pValue);
    }
    else if( STRNCMP(strChildName, "DSCPMark"))
    {
        pVoiceProfile_MGCP->DSCPMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "VLANIDMark"))
    {
        pVoiceProfile_MGCP->VLANIDMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "EthernetPriorityMark"))
    {
        pVoiceProfile_MGCP->EthernetPriorityMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "AllowPiggybackEvents"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_MGCP->AllowPiggybackEvents = true;
        }
        else
        {
            pVoiceProfile_MGCP->AllowPiggybackEvents = false;
        }
    }
    else if( STRNCMP(strChildName, "SendRSIPImmediately"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
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
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_H323(TELCOVOICEMGR_DML_H323* pVoiceProfile_H323,
                                          char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_H323 && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "Gatekeeper"))
    {
        STRNCPY(pVoiceProfile_H323->Gatekeeper, pValue);
    }
    else if( STRNCMP(strChildName, "GatekeeperPort"))
    {
        pVoiceProfile_H323->GatekeeperPort = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "GatekeeperID"))
    {
        STRNCPY(pVoiceProfile_H323->GatekeeperID, pValue);
    }
    else if( STRNCMP(strChildName, "TimeToLive"))
    {
        pVoiceProfile_H323->TimeToLive = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "H235Authentication"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_H323->H235Authentication = true;
        }
        else
        {
            pVoiceProfile_H323->H235Authentication = false;
        }
    }
    else if( STRNCMP(strChildName, "AuthPassword"))
    {
    STRNCPY(pVoiceProfile_H323->AuthPassword, pValue);
    }
    else if( STRNCMP(strChildName, "SendersID"))
    {
    STRNCPY(pVoiceProfile_H323->SendersID, pValue);
    }
    else if( STRNCMP(strChildName, "DSCPMark"))
    {
        pVoiceProfile_H323->DSCPMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "VLANIDMark"))
    {
        pVoiceProfile_H323->VLANIDMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "EthernetPriorityMark"))
    {
        pVoiceProfile_H323->EthernetPriorityMark = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("\n%s:%d:: \nInvalid ParamName[%s] \nparamValue[%s].", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_RTP_RTCP(TELCOVOICEMGR_DML_RTP_RTCP* pVoiceProfile_RTP_RTCP,
                                          char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_RTP_RTCP && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "Enable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_RTP_RTCP->Enable = true;
        }
        else
        {
            pVoiceProfile_RTP_RTCP->Enable = false;
        }
    }
    else if( STRNCMP(strChildName, "TxRepeatInterval"))
    {
        pVoiceProfile_RTP_RTCP->TxRepeatInterval = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "LocalCName"))
    {
        STRNCPY(pVoiceProfile_RTP_RTCP->LocalCName, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_RTP_SRTP(TELCOVOICEMGR_DML_RTP_SRTP* pVoiceProfile_RTP_SRTP,
                                          char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_RTP_SRTP && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "Enable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_RTP_SRTP->Enable = true;
        }
        else
        {
            pVoiceProfile_RTP_SRTP->Enable = false;
        }
    }
    else if( STRNCMP(strChildName, "KeyingMethods"))
    {
        STRNCPY(pVoiceProfile_RTP_SRTP->KeyingMethods, pValue);
    }
    else if( STRNCMP(strChildName, "EncryptionKeySizes"))
    {
        STRNCPY(pVoiceProfile_RTP_SRTP->EncryptionKeySizes, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_RTP_Redundancy(
                         TELCOVOICEMGR_DML_RTP_REDUNDANCY* pVoiceProfile_RTP_Redundancy,
                                          char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_RTP_Redundancy && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "Enable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_RTP_Redundancy->Enable = true;
        }
        else
        {
            pVoiceProfile_RTP_Redundancy->Enable  = false;
        }
    }
    else if( STRNCMP(strChildName, "BlockPayloadType"))
    {
        pVoiceProfile_RTP_Redundancy->BlockPayloadType = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "PayloadType"))
    {
        pVoiceProfile_RTP_Redundancy->PayloadType = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "FaxAndModemRedundancy"))
    {
        pVoiceProfile_RTP_Redundancy->FaxAndModemRedundancy = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ModemRedundancy"))
    {
        pVoiceProfile_RTP_Redundancy->ModemRedundancy = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "DTMFRedundancy"))
    {
        pVoiceProfile_RTP_Redundancy->DTMFRedundancy = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "VoiceRedundancy"))
    {
        pVoiceProfile_RTP_Redundancy->VoiceRedundancy = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "MaxSessionsUsingRedundancy"))
    {
        pVoiceProfile_RTP_Redundancy->MaxSessionsUsingRedundancy = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_RTP(TELCOVOICEMGR_DML_RTP* pVoiceProfile_RTP,
                                          char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_RTP && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "LocalPortMin"))
    {
        pVoiceProfile_RTP->LocalPortMin = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "LocalPortMax"))
    {
        pVoiceProfile_RTP->LocalPortMax = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "DSCPMark"))
    {
        pVoiceProfile_RTP->DSCPMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "VLANIDMark"))
    {
        pVoiceProfile_RTP->VLANIDMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "EthernetPriorityMark"))
    {
        pVoiceProfile_RTP->EthernetPriorityMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TelephoneEventPayloadType"))
    {
        pVoiceProfile_RTP->TelephoneEventPayloadType = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "X_RDK_SKBMark"))
    {
        pVoiceProfile_RTP->X_RDK_SKBMark = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "RTCP"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.RTP.RTCP.
        return Map_hal_dml_voiceProfile_RTP_RTCP( &(pVoiceProfile_RTP->RTCPObj),
                           strChildName + (strlen(strChildName) + 1), pathEnd,  pValue);
    }
    else if( STRNCMP(strChildName, "SRTP"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.RTP.SRTP.
        return Map_hal_dml_voiceProfile_RTP_SRTP( &(pVoiceProfile_RTP->SRTPObj),
                           strChildName + (strlen(strChildName) + 1), pathEnd,  pValue);

    }
    else if( STRNCMP(strChildName, "Redundancy"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.RTP.Redundancy.
        return Map_hal_dml_voiceProfile_RTP_Redundancy( &(pVoiceProfile_RTP->RedundancyObj),
                           strChildName + (strlen(strChildName) + 1), pathEnd,  pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS Map_hal_dml_voiceProfile_NumberingPlan_PrifInfo(
                   DML_NUMBERINGPLAN_PREFIXINFO_LIST_T* pVoiceProfile_Numberingplan_PrefixInfo_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    //VoiceService.{i}.VoiceProfile.{i}.Line.{i}.
    if(!(pVoiceProfile_Numberingplan_PrefixInfo_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "PrefixInfo", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T* pVoiceProfile_Numberingplan_PrefixInfo_Data = pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[uTableIndex];

    if(pVoiceProfile_Numberingplan_PrefixInfo_Data == NULL)
    {
        //create new Numberingplan PrefixInfo
        if( TelcoVoiceMgrDmlAddNumberingplanPrefixInfo(pVoiceProfile_Numberingplan_PrefixInfo_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pVoiceProfile_Numberingplan_PrefixInfo_Data = pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[uTableIndex];
        if(pVoiceProfile_Numberingplan_PrefixInfo_Data == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO* pVoiceProfile_NumberingPlan_PrefixInfo = &(pVoiceProfile_Numberingplan_PrefixInfo_Data->dml);

    pVoiceProfile_NumberingPlan_PrefixInfo->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "PrefixRange"))
    {
        STRNCPY(pVoiceProfile_NumberingPlan_PrefixInfo->PrefixRange, pValue);
    }
    else if( STRNCMP(strChildName, "PrefixMinNumberOfDigits"))
    {
        pVoiceProfile_NumberingPlan_PrefixInfo->PrefixMinNumberOfDigits = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "PrefixMaxNumberOfDigits"))
    {
        pVoiceProfile_NumberingPlan_PrefixInfo->PrefixMaxNumberOfDigits = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "NumberOfDigitsToRemove"))
    {
        pVoiceProfile_NumberingPlan_PrefixInfo->NumberOfDigitsToRemove = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "PosOfDigitsToRemove"))
    {
        pVoiceProfile_NumberingPlan_PrefixInfo->PosOfDigitsToRemove = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "DialTone"))
    {
        pVoiceProfile_NumberingPlan_PrefixInfo->DialTone = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "FacilityAction"))
    {
        STRNCPY(pVoiceProfile_NumberingPlan_PrefixInfo->FacilityAction, pValue);
    }
    else if( STRNCMP(strChildName, "FacilityActionArgument"))
    {
        STRNCPY(pVoiceProfile_NumberingPlan_PrefixInfo->FacilityActionArgument, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_NumberingPlan(TELCOVOICEMGR_DML_NUMBERINGPLAN* pVoiceProfile_NumberingPlan,
                                          char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_NumberingPlan && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "MinimumNumberOfDigits"))
    {
        pVoiceProfile_NumberingPlan->MinimumNumberOfDigits = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "MaximumNumberOfDigits"))
    {
        pVoiceProfile_NumberingPlan->MaximumNumberOfDigits = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "InterDigitTimerStd"))
    {
        pVoiceProfile_NumberingPlan->InterDigitTimerStd = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "InterDigitTimerOpen"))
    {
        pVoiceProfile_NumberingPlan->InterDigitTimerOpen = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "InvalidNumberTone"))
    {
        pVoiceProfile_NumberingPlan->InvalidNumberTone = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "PrefixInfoMaxEntries"))
    {
        pVoiceProfile_NumberingPlan->PrefixInfoMaxEntries = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "PrefixInfo"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.NumberingPlan.PrefixInfo.{i}.
        return Map_hal_dml_voiceProfile_NumberingPlan_PrifInfo(
                              &(pVoiceProfile_NumberingPlan->PrefixInfoList),
                                                  strChildName, pathEnd, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return  ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_Tone_Event(
                   DML_TONE_EVENT_LIST_T* pVoiceProfile_Tone_Event_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Tone_Event_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Event", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }

    DML_TONE_EVENT_CTRL_T* pVoiceProfile_Tone_Event_Data = pVoiceProfile_Tone_Event_List->pdata[uTableIndex];

    if(pVoiceProfile_Tone_Event_Data == NULL)
    {
        //create new pVoiceProfile_Tone_Event
        if( TelcoVoiceMgrDmlAddVoiceProfileToneEvent(pVoiceProfile_Tone_Event_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pVoiceProfile_Tone_Event_Data = pVoiceProfile_Tone_Event_List->pdata[uTableIndex];
        if(pVoiceProfile_Tone_Event_Data == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_TONE_EVENT* pVoiceProfile_Tone_Event = &(pVoiceProfile_Tone_Event_Data->dml);

    pVoiceProfile_Tone_Event->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "Function"))
    {
        STRNCPY(pVoiceProfile_Tone_Event->Function, pValue);
    }
    else if( STRNCMP(strChildName, "ToneID"))
    {
        pVoiceProfile_Tone_Event->ToneID = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_Tone_Desc(
                   DML_TONE_DESCRIPTION_LIST_T* pVoiceProfile_Tone_Desc_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Tone_Desc_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Description", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_TONE_DESCRIPTION_CTRL_T* pVoiceProfile_Tone_Desc_Data = pVoiceProfile_Tone_Desc_List->pdata[uTableIndex];

    if(pVoiceProfile_Tone_Desc_Data == NULL)
    {
        //create new Profile Tone Desc
        if( TelcoVoiceMgrDmlAddVoiceProfileToneDesc(pVoiceProfile_Tone_Desc_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pVoiceProfile_Tone_Desc_Data = pVoiceProfile_Tone_Desc_List->pdata[uTableIndex];
        if(pVoiceProfile_Tone_Desc_Data == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_TONE_DESCRIPTION* pVoiceProfile_Tone_Desc = &(pVoiceProfile_Tone_Desc_Data->dml);

    pVoiceProfile_Tone_Desc->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "EntryID"))
    {
        pVoiceProfile_Tone_Desc->EntryID = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ToneEnable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Tone_Desc->ToneEnable = true;
        }
        else
        {
            pVoiceProfile_Tone_Desc->ToneEnable = false;
        }
    }
    else if( STRNCMP(strChildName, "ToneName"))
    {
        STRNCPY(pVoiceProfile_Tone_Desc->ToneName, pValue);
    }
    else if( STRNCMP(strChildName, "TonePattern"))
    {
        pVoiceProfile_Tone_Desc->TonePattern = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ToneFile"))
    {
        STRNCPY(pVoiceProfile_Tone_Desc->ToneFile, pValue);
    }
    else if( STRNCMP(strChildName, "ToneRepetitions"))
    {
        pVoiceProfile_Tone_Desc->ToneRepetitions = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ToneText"))
    {
        STRNCPY(pVoiceProfile_Tone_Desc->ToneText, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_Tone_Pattern(
                   DML_TONE_PATTERN_LIST_T* pVoiceProfile_Tone_Pattern_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Tone_Pattern_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Pattern", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_TONE_PATTERN_CTRL_T* pVoiceProfile_Tone_Pattern_Data = pVoiceProfile_Tone_Pattern_List->pdata[uTableIndex];

    if(pVoiceProfile_Tone_Pattern_Data == NULL)
    {
        //create new Veip
        if( TelcoVoiceMgrDmlAddVoiceProfileTonePattern(pVoiceProfile_Tone_Pattern_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pVoiceProfile_Tone_Pattern_Data = pVoiceProfile_Tone_Pattern_List->pdata[uTableIndex];
        if(pVoiceProfile_Tone_Pattern_Data == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_TONE_PATTERN* pVoiceProfile_Tone_Pattern = &(pVoiceProfile_Tone_Pattern_Data->dml);

    pVoiceProfile_Tone_Pattern->InstanceNumber = uTableIndex + 1;
    if( STRNCMP(strChildName, "EntryID"))
    {
        pVoiceProfile_Tone_Pattern->EntryID = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ToneOn"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_Tone_Pattern->ToneOn = true;
        }
        else
        {
            pVoiceProfile_Tone_Pattern->ToneOn = false;
        }
    }
    else if( STRNCMP(strChildName, "Frequency1"))
    {
        pVoiceProfile_Tone_Pattern->Frequency1 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Power1"))
    {
        pVoiceProfile_Tone_Pattern->Power1 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Frequency2"))
    {
        pVoiceProfile_Tone_Pattern->Frequency2 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Power2"))
    {
        pVoiceProfile_Tone_Pattern->Power2 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Frequency3"))
    {
        pVoiceProfile_Tone_Pattern->Frequency3 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Power3"))
    {
        pVoiceProfile_Tone_Pattern->Power3 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Frequency4"))
    {
        pVoiceProfile_Tone_Pattern->Frequency4 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Power4"))
    {
        pVoiceProfile_Tone_Pattern->Power4 = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ModulationFrequency"))
    {
        pVoiceProfile_Tone_Pattern->ModulationFrequency = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "ModulationPower"))
    {
        pVoiceProfile_Tone_Pattern->ModulationPower = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Duration"))
    {
        pVoiceProfile_Tone_Pattern->Duration = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "NextEntryID"))
    {
        pVoiceProfile_Tone_Pattern->NextEntryID = strtoul(pValue,&err,10);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_Tone( TELCOVOICEMGR_DML_TONE* pVoiceProfile_Tone,
                                     char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_Tone && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(strChildName, "Event"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Tone.Event.{i}.
        return Map_hal_dml_voiceProfile_Tone_Event( &(pVoiceProfile_Tone->ToneEventList),
                                                           strChildName, pathEnd, pValue);
    }
    else if( STRNCMP(strChildName, "Description"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Tone.Description.{i}.
        return Map_hal_dml_voiceProfile_Tone_Desc( &(pVoiceProfile_Tone->ToneDescriptionList),
                                                              strChildName, pathEnd, pValue);
    }
    else if( STRNCMP(strChildName, "Pattern"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.Tone.Pattern.{i}.
        return Map_hal_dml_voiceProfile_Tone_Pattern( &(pVoiceProfile_Tone->TonePatternList),
                                                              strChildName, pathEnd, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_ButtonMap_Button(DML_BUTTON_LIST_T* pVoiceProfile_ButtonMap_Button_List,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_ButtonMap_Button_List && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Button", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }
    DML_BUTTON_CTRL_T* pVoiceProfile_ButtonMap_Button_Data = pVoiceProfile_ButtonMap_Button_List->pdata[uTableIndex];

    if(pVoiceProfile_ButtonMap_Button_Data == NULL)
    {
        //create new 
        if( TelcoVoiceMgrDmlAddVoiceProfileButtonMapButton(pVoiceProfile_ButtonMap_Button_List, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pVoiceProfile_ButtonMap_Button_Data = pVoiceProfile_ButtonMap_Button_List->pdata[uTableIndex];
        if(pVoiceProfile_ButtonMap_Button_Data == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_BUTTON* pVoiceProfile_ButtonMap_Button = &(pVoiceProfile_ButtonMap_Button_Data->dml);

    pVoiceProfile_ButtonMap_Button->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "ButtonName"))
    {
        STRNCPY(pVoiceProfile_ButtonMap_Button->ButtonName, pValue);
    }
    else if( STRNCMP(strChildName, "FacilityAction"))
    {
        STRNCPY(pVoiceProfile_ButtonMap_Button->FacilityAction, pValue);
    }
    else if( STRNCMP(strChildName, "FacilityActionArgument"))
    {
        STRNCPY(pVoiceProfile_ButtonMap_Button->FacilityActionArgument, pValue);
    }
    else if( STRNCMP(strChildName, "QuickDialNumber"))
    {
        STRNCPY(pVoiceProfile_ButtonMap_Button->QuickDialNumber, pValue);
    }
    else if( STRNCMP(strChildName, "ButtonMessage"))
    {
        STRNCPY(pVoiceProfile_ButtonMap_Button->ButtonMessage, pValue);
    }
    else if( STRNCMP(strChildName, "UserAccess"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
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
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_ButtonMap(
           TELCOVOICEMGR_DML_BUTTONMAP* pVoiceProfile_ButtonMap,
                                     char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_ButtonMap && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(strChildName, "NumberOfButtons"))
    {
        pVoiceProfile_ButtonMap->NumberOfButtons = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Button"))
    {
        //VoiceService.{i}.VoiceProfile.{i}.ButtonMap.Button.{i}.
        return Map_hal_dml_voiceProfile_ButtonMap_Button(&(pVoiceProfile_ButtonMap->ButtonList),
                                                  strChildName, pathEnd, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceProfile_FaxT38(
           TELCOVOICEMGR_DML_FAXT38* pVoiceProfile_FaxT38,
                                     char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pVoiceProfile_FaxT38 && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if( STRNCMP(strChildName, "Enable"))
    {
        if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
        {
            pVoiceProfile_FaxT38->Enable = true;
        }
        else
        {
            pVoiceProfile_FaxT38->Enable = false;
        }
    }
    else if( STRNCMP(strChildName, "BitRate"))
    {
        pVoiceProfile_FaxT38->BitRate = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "HighSpeedPacketRate"))
    {
        pVoiceProfile_FaxT38->HighSpeedPacketRate = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "HighSpeedRedundancy"))
    {
        pVoiceProfile_FaxT38->HighSpeedRedundancy = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "LowSpeedRedundancy"))
    {
        pVoiceProfile_FaxT38->LowSpeedRedundancy = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "TCFMethod"))
    {
        if(STRNCMP(pValue, "Local"))
        {
            pVoiceProfile_FaxT38->TCFMethod = TCFMETHOD_LOCAL;
        }
        else if( STRNCMP(strChildName, "Network"))
        {
            pVoiceProfile_FaxT38->TCFMethod = TCFMETHOD_NETWORK;
        }
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid strChildName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
ANSC_STATUS Map_hal_dml_voiceProfile(DML_PROFILE_LIST_T* pVoiceProfileList, char* paramName, char* ptrEnd, char* pValue)
{
    char *err;
    int uTableIndex = 0;

    char *strChildName = NULL;
    ANSC_STATUS retStatus = ANSC_STATUS_SUCCESS;

    if(!(pVoiceProfileList && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "VoiceProfile", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }

    DML_PROFILE_CTRL_T* pVoiceProfile = pVoiceProfileList->pdata[uTableIndex];

    if(pVoiceProfile == NULL)
    {
        //create new VoiceProfile
        if( TelcoVoiceMgrDmlAddVoiceProfile(pVoiceProfileList, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pVoiceProfile = pVoiceProfileList->pdata[uTableIndex];
        if(pVoiceProfile == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_VOICEPROFILE* pVoiceProf = &(pVoiceProfile->dml);
    if (pVoiceProf)
    {
        pVoiceProf->InstanceNumber = uTableIndex + 1;
        //VoiceService.{i}.VoiceProfile.{i}.
        if( STRNCMP(strChildName, "Line"))
        { 
            //VoiceService.{i}.VoiceProfile{i}.Line.{i}.
            retStatus = Map_hal_dml_voiceProfileLine(&(pVoiceProf->LineList), strChildName, ptrEnd, pValue);
        }
        else if( STRNCMP(strChildName, "ServiceProviderInfo"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.ServiceProviderInfo.
            retStatus = Map_hal_dml_voiceProfile_ServiceProviderInfo( &(pVoiceProf->ServiceProviderInfoObj), 
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
        }
        else if( STRNCMP(strChildName, "SIP"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.SIP.
            retStatus = Map_hal_dml_voiceProfile_SIP( &(pVoiceProf->SIPObj), 
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
        }  // END_OF_SIP
        else if( STRNCMP(strChildName, "MGCP"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.MGCP.
            retStatus = Map_hal_dml_voiceProfile_MGCP(&(pVoiceProf->MGCPObj),
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
        }
        else if( STRNCMP(strChildName, "H323"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.H323.
            retStatus = Map_hal_dml_voiceProfile_H323(&(pVoiceProf->H323Obj),
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
        }
        else if( STRNCMP(strChildName, "RTP"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.RTP.
            retStatus = Map_hal_dml_voiceProfile_RTP( &(pVoiceProf->RTPObj),
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);

        } // end of RTP
        else if( STRNCMP(strChildName, "NumberingPlan"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.NumberingPlan.
            retStatus = Map_hal_dml_voiceProfile_NumberingPlan( &(pVoiceProf->NumberingPlanObj),
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
        }
        else if( STRNCMP(strChildName, "Tone"))
        {
            // VoiceService.{i}.VoiceProfile.{i}.Tone.
            retStatus = Map_hal_dml_voiceProfile_Tone( &(pVoiceProf->ToneObj),
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
        }
        else if( STRNCMP(strChildName, "ButtonMap"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.ButtonMap.
            retStatus = Map_hal_dml_voiceProfile_ButtonMap(  &(pVoiceProf->ButtonMapObj),
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
        }
        else if( STRNCMP(strChildName, "FaxT38"))
        {
            //VoiceService.{i}.VoiceProfile.{i}.FaxT38.
            retStatus = Map_hal_dml_voiceProfile_FaxT38(  &(pVoiceProf->Fax38Obj),
                           strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
        }
        else if( STRNCMP(strChildName, "Enable"))
        {
            pVoiceProf->Enable = strtoul(pValue,&err,10);
        }
        else if( STRNCMP(strChildName, "Reset"))
        {
            if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
            {
                pVoiceProf->Reset = true;
            }
            else
            {
                pVoiceProf->Reset = false;
            }
        }
        else if( STRNCMP(strChildName, "NumberOfLines"))
        {
            pVoiceProf->NumberOfLines = strtoul(pValue,&err,10);
        }
        else if( STRNCMP(strChildName, "Name"))
        {
            STRNCPY(pVoiceProf->Name, pValue);
        }
        else if( STRNCMP(strChildName, "SignalingProtocol"))
        {
            STRNCPY(pVoiceProf->SignalingProtocol, pValue);
        }
        else if( STRNCMP(strChildName, "MaxSessions"))
        {
            pVoiceProf->MaxSessions = strtoul(pValue,&err,10);
        }
        else if( STRNCMP(strChildName, "DTMFMethodG711"))
        {
            if (strcmp(pValue,"InBand") == 0)
            {
                pVoiceProf->DTMFMethodG711 = VOICE_PROFILE_DTMF_METHODG711_INBAND;
            }
            else if (strcmp(pValue,"RFC4733") == 0)
            {
                pVoiceProf->DTMFMethodG711 = VOICE_PROFILE_DTMF_METHODG711_RFC2833;
            }
            else if (strcmp(pValue,"SIPInfo") == 0)
            {
                pVoiceProf->DTMFMethodG711 = VOICE_PROFILE_DTMF_METHODG711_SIPINFO;
            }
        }
        else if( STRNCMP(strChildName, "DTMFMethod"))
        {
            if (strcmp(pValue,"InBand") == 0)
            {
                pVoiceProf->DTMFMethod = VOICE_PROFILE_DTMF_METHOD_INBAND;
            }
            else if (strcmp(pValue,"RFC4733") == 0)
            {
                pVoiceProf->DTMFMethod = VOICE_PROFILE_DTMF_METHOD_RFC2833;
            }
            else if (strcmp(pValue,"SIPInfo") == 0)
            {
                pVoiceProf->DTMFMethod = VOICE_PROFILE_DTMF_METHOD_SIPINFO;
            }
        }
        else if( STRNCMP(strChildName, "Region"))
        {
            STRNCPY(pVoiceProf->Region, pValue);
        }
        else if( STRNCMP(strChildName, "X_RDK-Central_COM_DigitMap"))
        {
            STRNCPY(pVoiceProf->X_RDK_DigitMap, pValue);
        }
        else if( STRNCMP(strChildName, "X_RDK-Central_COM_EmergencyDigitMap"))
        {
            STRNCPY(pVoiceProf->EmergencyDigitMap, pValue);
        }
        else if( STRNCMP(strChildName, "X_RDK-Central_COM_SDigitTimer"))
        {
            pVoiceProf->SDigitTimer = strtoul(pValue,&err,10);
        }
        else if( STRNCMP(strChildName, "X_RDK-Central_COM_ZDigitTimer"))
        {
            pVoiceProf->ZDigitTimer = strtoul(pValue,&err,10);
        }
        else if( STRNCMP(strChildName, "DigitMapEnable"))
        {
            if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
            {
                pVoiceProf->DigitMapEnable = true;
            }
            else
            {
                pVoiceProf->DigitMapEnable = false;
            }
        }
        else if( STRNCMP(strChildName, "DigitMap"))
        {
            STRNCPY(pVoiceProf->DigitMap, pValue);
        }
        else if( STRNCMP(strChildName, "STUNEnable"))
        {
            if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
            {
                pVoiceProf->STUNEnable = true;
            }
            else
            {
                pVoiceProf->STUNEnable = false;
            }
        }
        else if( STRNCMP(strChildName, "STUNServer"))
        {
            STRNCPY(pVoiceProf->STUNServer, pValue);
        }
        else if( STRNCMP(strChildName, "NonVoiceBandwidthReservedUpstream"))
        {
            pVoiceProf->NonVoiceBandwidthReservedUpstream = strtoul(pValue,&err,10);
        }
        else if( STRNCMP(strChildName, "NonVoiceBandwidthReservedDownstream"))
        {
            pVoiceProf->NonVoiceBandwidthReservedDownstream = strtoul(pValue,&err,10);
        }
        else if( STRNCMP(strChildName, "PSTNFailOver"))
        {
            if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
            {
                pVoiceProf->PSTNFailOver = true;
            }
            else
            {
                pVoiceProf->PSTNFailOver = false;
            }
        }
        else if( STRNCMP(strChildName, "FaxPassThrough"))
        {
            if(STRNCMP(pValue, "Disable"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_FAXPASSTHROUGH_DISABLE;
            }
            else if(STRNCMP(pValue, "Auto"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_FAXPASSTHROUGH_AUTO;
            }
            else if(STRNCMP(pValue, "Force"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_FAXPASSTHROUGH_FORCE;
            }
        }
        else if( STRNCMP(strChildName, "ModemPassThrough"))
        {
            if(STRNCMP(pValue, "Disable"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_MODEMPASSTHROUGH_DISABLE;
            }
            else if(STRNCMP(pValue, "Auto"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_MODEMPASSTHROUGH_AUTO;
            }
            else if(STRNCMP(pValue, "Force"))
            {
                pVoiceProf->FaxPassThrough = VOICE_PROFILE_MODEMPASSTHROUGH_FORCE;
            }
        }
    }
    if( retStatus != ANSC_STATUS_SUCCESS)
    {
        AnscTraceError(("%s:%d:: Mapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,strChildName));
        return ANSC_STATUS_FAILURE;
    }
    pVoiceProfile->updated = true;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceService_Debug(
                       PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService, char* paramName,
                                                           char *pathEnd, char* pValue)
{
    char *err;
    if(!(pVoiceService && paramName && pathEnd && pValue) ||
                                         pathEnd <= paramName)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(paramName, "LogServerPort"))
    {
        pVoiceService->X_RDK_DebugObj.LogServerPort = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(paramName, "LogServer"))
    {
        STRNCPY(pVoiceService->X_RDK_DebugObj.LogServer, pValue);
    }
    else if( STRNCMP(paramName, "CCTKTraceGroup"))
    {
        STRNCPY(pVoiceService->X_RDK_DebugObj.CCTKTraceGroup, pValue);
    }
    else if( STRNCMP(paramName, "CCTKTraceLevel"))
    {
        STRNCPY(pVoiceService->X_RDK_DebugObj.CCTKTraceLevel, pValue);
    }
    else if( STRNCMP(paramName, "ModuleLogLevels"))
    {
        STRNCPY(pVoiceService->X_RDK_DebugObj.ModuleLogLevels, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid Parameter [%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_phyInterface_Tests(
                PTELCOVOICEMGR_DML_PHYINTERFACE_TESTS pPhyInterface_Tests,
                                     char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;

    if(!(pPhyInterface_Tests && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "TestState"))
    {
        if(STRNCMP(pValue, "None"))
        {
            pPhyInterface_Tests->TestState = PHYINTERFACE_TESTSTATE_NONE;
        }
        else if(STRNCMP(pValue, "Requested"))
        {
            pPhyInterface_Tests->TestState = PHYINTERFACE_TESTSTATE_REQUESTED;
        }
        else if(STRNCMP(pValue, "Complete"))
        {
            pPhyInterface_Tests->TestState = PHYINTERFACE_TESTSTATE_COMPLETE;
        }
        else if(STRNCMP(pValue, "Error_TestNotSupported"))
        {
            pPhyInterface_Tests->TestState = PHYINTERFACE_TESTSTATE_ERROR_TESTNOTSUPPORTED;
        }
    }
    else if( STRNCMP(strChildName, "TestSelector"))
    {
        if(STRNCMP(pValue, "PhoneConnectivityTest"))
        {
            pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_PHONE_CONNECTIVITY_TEST;
        }
        else if(STRNCMP(pValue, "Hazard Potential"))
        {
            pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_HAZARD_POTENTIAL;
        }
        else if(STRNCMP(pValue, "Foreign Voltage"))
        {
            pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_FOREIGN_VOLTAGE;
        }
        else if(STRNCMP(pValue, "Resistive Faults"))
        {
            pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_RESISTIVE_FAULTS;
        }
        else if(STRNCMP(pValue, "Off-hook"))
        {
            pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_OFF_HOOK;
        }
        else if(STRNCMP(pValue, "REN"))
        {
            pPhyInterface_Tests->TestSelector = PHYINTERFACE_TESTSELECTOR_REN;
        }
    }
    else if( STRNCMP(strChildName, "PhoneConnectivity"))
    {
        if(STRNCMP(pValue, "1"))
        {
            pPhyInterface_Tests->PhoneConnectivity = true;
        }
        else
        {
            pPhyInterface_Tests->PhoneConnectivity = false;
        }
    }
    else if( STRNCMP(strChildName, "X_RDK_TestResult"))
    {
        STRNCPY(pPhyInterface_Tests->X_RDK_TestResult, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_phyInterface(DML_PHYINTERFACE_LIST_T* pPhyInterfaceList, char* paramName, char* ptrEnd, char* pValue)
{
    char *strChildName = NULL;
    char *err;
    int uTableIndex = 0;
    ANSC_STATUS retStatus = ANSC_STATUS_FAILURE;

    if(!(pPhyInterfaceList && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "PhyInterface", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }

    DML_PHYINTERFACE_CTRL_T* pPhyInterfaceData = pPhyInterfaceList->pdata[uTableIndex];

    if(pPhyInterfaceData == NULL)
    {
        //create new PhyInterface
        if( TelcoVoiceMgrDmlAddPhyInterface(pPhyInterfaceList, uTableIndex-1) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }

        pPhyInterfaceData = pPhyInterfaceList->pdata[uTableIndex];
        if(pPhyInterfaceData == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_PHYINTERFACE* pPhyInterface = &(pPhyInterfaceData->dml);
    if(pPhyInterface)
    {
        pPhyInterface->InstanceNumber = uTableIndex + 1;

        if( STRNCMP(strChildName, "PhyPort"))
        {
            STRNCPY(pPhyInterface->PhyPort, pValue);
        }
        else if( STRNCMP(strChildName, "InterfaceID"))
        {
            pPhyInterface->InterfaceID = strtoul(pValue,&err,10);
        }
        else if( STRNCMP(strChildName, "Description"))
        {
            STRNCPY(pPhyInterface->Description, pValue);
        }
        else if( STRNCMP(strChildName, "Tests"))
        {
            //VoiceService.{i}.PhyInterface.{i}.Tests.
            retStatus = Map_hal_dml_phyInterface_Tests( &(pPhyInterface->PhyInterfaceTestsObj),
                                                strChildName + (strlen(strChildName) + 1), ptrEnd,  pValue);
            if(retStatus != ANSC_STATUS_SUCCESS)
            {
                AnscTraceError(("%s:%d:: Failed to set ParamName[%s] paramValue[%s].\n",
                                                              __FUNCTION__, __LINE__, strChildName, pValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        else
        {
            AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
            return ANSC_STATUS_FAILURE;
        }
    }
    pPhyInterfaceData->updated = true;
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_capabilitiesCodecs(DML_CAPABILITIES_CODECS_LIST_T* pCapabilitiesCodecList,
                                                  char* paramName, char* ptrEnd, char* pValue)
{
    int uTableIndex = 0;
    char *strChildName = NULL;
    char *err;

    if(!(pCapabilitiesCodecList && paramName && ptrEnd && pValue))
    {
        AnscTraceError(("\n%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    uTableIndex = get_index_and_select_child(paramName, "Codecs", ptrEnd,  &strChildName);

    if(uTableIndex < 0 )
    {
        AnscTraceError(("\n%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, paramName));
        return ANSC_STATUS_FAILURE;
    }

    DML_CAPABILITIES_CODECS_CTRL_T* pCapabilitiesCodecData = pCapabilitiesCodecList->pdata[uTableIndex];

    if(pCapabilitiesCodecData == NULL)
    {
        //create new
        if( TelcoVoiceMgrDmlAddCapabilitiesCodec(pCapabilitiesCodecList, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pCapabilitiesCodecData = pCapabilitiesCodecList->pdata[uTableIndex];
        if(pCapabilitiesCodecData == NULL)
        {
            AnscTraceError(("%s:%d:: Could not find table entry for parameter %s..\n", __FUNCTION__, __LINE__, strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }

    TELCOVOICEMGR_DML_CAPABILITIES_CODECS* pCapabilities_Codecs = &(pCapabilitiesCodecData->dml);

    pCapabilities_Codecs->InstanceNumber = uTableIndex + 1;

    if( STRNCMP(strChildName, "EntryID"))
    {
        pCapabilities_Codecs->EntryID = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "Codec"))
    {
        STRNCPY(pCapabilities_Codecs->Codec, pValue);
    }
    else if( STRNCMP(strChildName, "BitRate"))
    {
        pCapabilities_Codecs->BitRate = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "PacketizationPeriod"))
    {
        STRNCPY(pCapabilities_Codecs->PacketizationPeriod, pValue);
    }
    else if( STRNCMP(strChildName, "SilenceSuppression"))
    {
        if(STRNCMP(pValue,"1"))
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
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_capabilities_H323(
                PTELCOVOICEMGR_DML_CAPABILITIES_H323 pCapabilities_H323,
                                     char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;

    if(!(pCapabilities_H323 && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "FastStart"))
    {
        if(STRNCMP(pValue, "1"))
        {
            pCapabilities_H323->FastStart = true;
        }
        else
        {
            pCapabilities_H323->FastStart = false;
        }
    }
    else if( STRNCMP(strChildName, "H235AuthenticationMethods"))
    {
        STRNCPY(pCapabilities_H323->H235AuthenticationMethods, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_capabilities_MGCP(
                PTELCOVOICEMGR_DML_CAPABILITIES_MGCP pCapabilities_MGCP,
                                     char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;

    if(!(pCapabilities_MGCP && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(strChildName, "Extensions"))
    {
        STRNCPY(pCapabilities_MGCP->Extensions, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_capabilities_SIP(
                PTELCOVOICEMGR_DML_CAPABILITIES_SIP pCapabilities_SIP,
                                     char* childNameStart, char *pathEnd, char* pValue)
{
    char *strChildName = NULL;

    if(!(pCapabilities_SIP && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    if( STRNCMP(strChildName, "Role"))
    {
        if(STRNCMP(pValue, "UserAgent"))
        {
            pCapabilities_SIP->Role = SIP_ROLE_USER_AGENT;
        }
        else if(STRNCMP(pValue, "BackToBackUserAgents"))
        {
            pCapabilities_SIP->Role = SIP_ROLE_B2B_USER_AGENT;
        }
        else if(STRNCMP(pValue, "OutboundProxy"))
        {
            pCapabilities_SIP->Role = SIP_ROLE_OUTBOUND_PROXY;
        }
    }
    else if( STRNCMP(strChildName, "Extensions"))
    {
        STRNCPY(pCapabilities_SIP->Extensions, pValue);
    }
    else if( STRNCMP(strChildName, "Transports"))
    {
        STRNCPY(pCapabilities_SIP->Transports, pValue);
    }
    else if( STRNCMP(strChildName, "URISchemes"))
    {
        STRNCPY(pCapabilities_SIP->URISchemes, pValue);
    }
    else if( STRNCMP(strChildName, "EventSubscription"))
    {
        if(STRNCMP(pValue, "1"))
        {
            pCapabilities_SIP->EventSubscription = true;
        }
        else
        {
            pCapabilities_SIP->EventSubscription = false;
        }
    }
    else if( STRNCMP(strChildName, "ResponseMap"))
    {
        if(STRNCMP(pValue, "1"))
        {
            pCapabilities_SIP->ResponseMap = true;
        }
        else
        {
            pCapabilities_SIP->ResponseMap = false;
        }
    }
    else if( STRNCMP(strChildName, "TLSAuthenticationProtocols"))
    {
        STRNCPY(pCapabilities_SIP->TLSAuthenticationProtocols, pValue);
    }
    else if( STRNCMP(strChildName, "TLSAuthenticationKeySizes"))
    {
        STRNCPY(pCapabilities_SIP->TLSAuthenticationKeySizes, pValue);
    }
    else if( STRNCMP(strChildName, "TLSEncryptionProtocols"))
    {
        STRNCPY(pCapabilities_SIP->TLSEncryptionProtocols, pValue);
    }
    else if( STRNCMP(strChildName, "TLSEncryptionKeySizes"))
    {
        STRNCPY(pCapabilities_SIP->TLSEncryptionKeySizes, pValue);
    }
    else if( STRNCMP(strChildName, "TLSKeyExchangeProtocols"))
    {
        STRNCPY(pCapabilities_SIP->TLSKeyExchangeProtocols, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_capabilities(PTELCOVOICEMGR_DML_CAPABILITIES pCapabilities,
                                     char* childNameStart, char *pathEnd, char* pValue)
{
    ANSC_STATUS retStatus = ANSC_STATUS_SUCCESS;
    char *err;

    char *strChildName = NULL;

    if(!(pCapabilities && childNameStart && pathEnd && pValue) || 
                                         pathEnd <= childNameStart)
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    select_child(childNameStart, pathEnd, &strChildName);
    if (!strChildName)
    {
        AnscTraceError(("%s:%d:: Incomplete path\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if( STRNCMP(strChildName, "MaxProfileCount"))
    {
       pCapabilities->MaxProfileCount = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "MaxLineCount"))
    {
       pCapabilities->MaxLineCount = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "MaxSessionsPerLine"))
    {
       pCapabilities->MaxSessionsPerLine = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "MaxSessionCount"))
    {
       pCapabilities->MaxSessionCount = strtoul(pValue,&err,10);
    }
    else if( STRNCMP(strChildName, "SignalingProtocols"))
    {
       STRNCPY(pCapabilities->SignalingProtocols, pValue);
    }
    else if( STRNCMP(strChildName, "Regions"))
    {
       STRNCPY(pCapabilities->Regions, pValue);
    }
    else if( STRNCMP(strChildName, "RTCP"))
    {
       if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
       {
           pCapabilities->RTCP = true;
       }
       else
       {
           pCapabilities->RTCP = false;
       }
    }
    else if( STRNCMP(strChildName, "SRTPKeyingMethods"))
    {
       STRNCPY(pCapabilities->SRTPKeyingMethods, pValue);
    }
    else if( STRNCMP(strChildName, "SRTPEncryptionKeySizes"))
    {
       STRNCPY(pCapabilities->SRTPEncryptionKeySizes, pValue);
    }
    else if( STRNCMP(strChildName, "SRTP"))
    {
       if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
       {
           pCapabilities->SRTP = true;
       }
       else
       {
           pCapabilities->SRTP = false;
       }
    }
    else if( STRNCMP(strChildName, "RTPRedundancy"))
    {
       if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
       {
           pCapabilities->RTPRedundancy = true;
       }
       else
       {
           pCapabilities->RTPRedundancy = false;
       }
    }
    else if( STRNCMP(strChildName, "DSCPCoupled"))
    {
       if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
       {
           pCapabilities->DSCPCoupled = true;
       }
       else
       {
           pCapabilities->DSCPCoupled = false;
       }
    }
    else if( STRNCMP(strChildName, "EthernetTaggingCoupled"))
    {
       if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
       {
           pCapabilities->EthernetTaggingCoupled = true;
       }
       else
       {
           pCapabilities->EthernetTaggingCoupled = false;
       }
    }
    else if( STRNCMP(strChildName, "PSTNSoftSwitchOver"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->PSTNSoftSwitchOver = true;
       }
       else
       {
           pCapabilities->PSTNSoftSwitchOver = false;
       }
    }
    else if( STRNCMP(strChildName, "FaxT38"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->FaxT38 = true;
       }
       else
       {
           pCapabilities->FaxT38 = false;
       }
    }
    else if( STRNCMP(strChildName, "FaxPassThrough"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->FaxPassThrough = true;
       }
       else
       {
           pCapabilities->FaxPassThrough = false;
       }
    }
    else if( STRNCMP(strChildName, "ModemPassThrough"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->ModemPassThrough = true;
       }
       else
       {
           pCapabilities->ModemPassThrough = false;
       }
    }
    else if( STRNCMP(strChildName, "ToneDescriptionsEditable"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->ToneDescriptionsEditable = true;
       }
       else
       {
           pCapabilities->ToneDescriptionsEditable = false;
       }

    }
    else if( STRNCMP(strChildName, "PatternBasedToneGeneration"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->PatternBasedToneGeneration = true;
       }
       else
       {
           pCapabilities->PatternBasedToneGeneration = false;
       }

    }
    else if( STRNCMP(strChildName, "FileBasedToneGeneration"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->FileBasedToneGeneration = true;
       }
       else
       {
           pCapabilities->FileBasedToneGeneration = false;
       }

    }
    else if( STRNCMP(strChildName, "ToneGeneration"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->ToneGeneration = true;
       }
       else
       {
           pCapabilities->ToneGeneration = false;
       }
    }
    else if( STRNCMP(strChildName, "ToneFileFormats"))
    {

       STRNCPY(pCapabilities->ToneFileFormats, pValue);
    }
    else if( STRNCMP(strChildName, "RingDescriptionsEditable"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->RingDescriptionsEditable = true;
       }
       else
       {
           pCapabilities->RingDescriptionsEditable = false;
       }

    }
    else if( STRNCMP(strChildName, "PatternBasedRingGeneration"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->PatternBasedRingGeneration = true;
       }
       else
       {
           pCapabilities->PatternBasedRingGeneration = false;
       }

    }
    else if( STRNCMP(strChildName, "RingPatternEditable"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->RingPatternEditable = true;
       }
       else
       {
           pCapabilities->RingPatternEditable = false;
       }

    }
    else if( STRNCMP(strChildName, "FileBasedRingGeneration"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->FileBasedRingGeneration = true;
       }
       else
       {
           pCapabilities->FileBasedRingGeneration = false;
       }

    }
    else if( STRNCMP(strChildName, "RingGeneration"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->RingGeneration = true;
       }
       else
       {
           pCapabilities->RingGeneration = false;
       }

    }
    else if( STRNCMP(strChildName, "RingFileFormats"))
    {

       STRNCPY(pCapabilities->RingFileFormats, pValue);
    }
    else if( STRNCMP(strChildName, "DigitMap"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->DigitMap = true;
       }
       else
       {
           pCapabilities->DigitMap = false;
       }

    }
    else if( STRNCMP(strChildName, "NumberingPlan"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->NumberingPlan = true;
       }
       else
       {
           pCapabilities->NumberingPlan = false;
       }

    }
    else if( STRNCMP(strChildName, "ButtonMap"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->ButtonMap = true;
       }
       else
       {
           pCapabilities->ButtonMap = false;
       }

    }
    else if( STRNCMP(strChildName, "VoicePortTests"))
    {
       if(STRNCMP(pValue, "1"))
       {
           pCapabilities->VoicePortTests = true;
       }
       else
       {
           pCapabilities->VoicePortTests = false;
       }
    }
    else if( STRNCMP(strChildName, "SIP"))
    {
        //VoiceService.{i}.Capabilities.SIP.
        return Map_hal_dml_capabilities_SIP( &(pCapabilities->CapabilitiesSIPObj),
                                     strChildName + (strlen(strChildName) + 1), pathEnd,  pValue);
    }
    else if( STRNCMP(strChildName, "MGCP"))
    {
        //VoiceService.{i}.Capabilities.MGCP.
        return Map_hal_dml_capabilities_MGCP(&(pCapabilities->CapabilitiesMGCPObj),
                                     strChildName + (strlen(strChildName) + 1), pathEnd,  pValue);
    }
    else if( STRNCMP(strChildName, "H323"))
    {
        //VoiceService.{i}.Capabilities.H323.
        return Map_hal_dml_capabilities_H323(&(pCapabilities->CapabilitiesH323Obj),
                                     strChildName + (strlen(strChildName) + 1), pathEnd,  pValue);
    }
    else if( STRNCMP(strChildName, "Codecs"))
    {
        //VoiceService.{i}.Capabilities.Codecs.{i}.
        return Map_hal_dml_capabilitiesCodecs(&(pCapabilities->CapabilitiesCodecList),
                                                  strChildName, pathEnd, pValue);
    }
    else
    {
        AnscTraceError(("%s:%d:: Invalid ParamName[%s] paramValue[%s].\n", __FUNCTION__, __LINE__, strChildName, pValue));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS Map_hal_dml_voiceService(DML_VOICE_SERVICE_LIST_T* pVoiceServiceList, char* pathStart, char* pValue)
{
    ANSC_STATUS retStatus = ANSC_STATUS_FAILURE;
    int uTableIndex = 0;
    char *err;
    char *pathEnd = NULL;
    char *strChildName = NULL;
    char tmpPath[BUF_LEN_1024] = {};

    if(!(pVoiceServiceList && pathStart && pValue))
    {
        AnscTraceError(("%s:%d:: Invalid Arguments\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    STRNCPY(tmpPath, pathStart);
    pathEnd = tmpPath + strlen(tmpPath);
    uTableIndex =  get_index_and_select_child(tmpPath,
                                             "VoiceService", pathEnd,  &strChildName);
    if( uTableIndex < 0 )
    {
        AnscTraceError(("%s:%d:: Invalid index ParamName[%s]\n", __FUNCTION__, __LINE__, pathStart));
        return ANSC_STATUS_FAILURE;
    }

    DML_VOICE_SERVICE_CTRL_T* pVoiceServiceData = pVoiceServiceList->pdata[uTableIndex];

    if(pVoiceServiceData == NULL)
    {
        //create new Voice Service
        if( TelcoVoiceMgrDmlAddVoiceService(pVoiceServiceList, uTableIndex) != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
        pVoiceServiceData = pVoiceServiceList->pdata[uTableIndex];
        if(pVoiceServiceData == NULL)
        {
            return ANSC_STATUS_FAILURE;
        }
    }

    PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService = &(pVoiceServiceData->dml);

    pVoiceService->InstanceNumber = uTableIndex + 1;
    if( STRNCMP(strChildName, "X_RDK_DisableLoopCurrentUntilRegistered"))
    {
       if(STRNCMP(pValue, "true") || STRNCMP(pValue, "1"))
       {
           pVoiceService->X_RDK_DisableLoopCurrentUntilRegistered = true;
       }
       else
       {
           pVoiceService->X_RDK_DisableLoopCurrentUntilRegistered = false;
       }
    }
    else if( STRNCMP(strChildName, "X_RDK_Enable"))
    {
       /*Enable value not stored in hal, handled in manager*/
    }
    else if( STRNCMP(strChildName, "X_RDK_FactoryReset"))
    {
       /*Factory Reset handled in manager*/
    }
    else if( STRNCMP(strChildName, "X_RDK_BoundIfName"))
    {
       STRNCPY(pVoiceService->X_RDK_BoundIfName, pValue);
    }
    else if( STRNCMP(strChildName, "X_RDK_IpAddressFamily"))
    {
       STRNCPY(pVoiceService->X_RDK_IpAddressFamily, pValue);
    }
    else if( STRNCMP(strChildName, "X_RDK_BoundIpAddr"))
    {
       STRNCPY(pVoiceService->X_RDK_BoundIpAddr, pValue);
    }
    else if( STRNCMP(strChildName, "X_RDK_Firewall_Rule_Data") )
    {
        STRNCPY(pVoiceService->X_RDK_Firewall_Rule_Data, pValue);
    }
    else if( STRNCMP(strChildName, "X_RDK_Status"))
    {
       if(STRNCMP(pValue, "Stopped"))
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_STOPPED;
       }
       else if(STRNCMP(pValue, "Starting"))
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_STARTING;
       }
       else if(STRNCMP(pValue, "Started"))
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_STARTED;
       }
       else if(STRNCMP(pValue, "Stopping"))
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_STOPPING;
       }
       else if(STRNCMP(pValue, "Error"))
       {
           pVoiceService->X_RDK_Status = VOICE_PROCESS_STATUS_ERROR;
       }
    }
    else if( STRNCMP(strChildName, "X_RDK_Debug"))
    {
        retStatus = Map_hal_dml_voiceService_Debug( pVoiceService, 
                           strChildName + (strlen(strChildName) + 1), pathEnd,  pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Mapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( STRNCMP(strChildName,"VoiceProfile"))
    {
        //VoiceService.{i}.VoiceProfile{i}.
        retStatus = Map_hal_dml_voiceProfile(&(pVoiceService->VoiceProfileList),
                                                           strChildName, pathEnd, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Mapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( STRNCMP(strChildName,"PhyInterface"))
    {
        //VoiceService.{i}.PhyInterface{i}.
        retStatus = Map_hal_dml_phyInterface(&(pVoiceService->PhyInterfaceList), strChildName, pathEnd, pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTraceError(("%s:%d:: Mapping failed for ParamName[%s]\n", __FUNCTION__, __LINE__,strChildName));
            return ANSC_STATUS_FAILURE;
        }
    }
    else if( STRNCMP(strChildName,"Capabilities"))
    {
        //VoiceService.{i}.Capabilities.
        retStatus = Map_hal_dml_capabilities(&(pVoiceService->CapabilitiesObj),
                                   strChildName + (strlen(strChildName) + 1), pathEnd,  pValue);
        if(retStatus != ANSC_STATUS_SUCCESS)
        {
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        fprintf(stderr, "\nUnknown ParamName[%s]\nParamValue[%s]", strChildName, pValue);
        return ANSC_STATUS_FAILURE;
    }

    pVoiceServiceData->updated = true;
    return ANSC_STATUS_SUCCESS;
}
