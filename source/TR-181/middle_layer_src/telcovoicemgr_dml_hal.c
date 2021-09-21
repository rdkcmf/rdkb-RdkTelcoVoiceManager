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
#include "telcovoicemgr_dml_hal.h"
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_services_apis_v2.h"
#else
#include "telcovoicemgr_services_apis_v1.h"
#endif

static ANSC_STATUS voice_process_get_info(hal_param_t *get_param);
static ANSC_STATUS get_voice_line_stats(const json_object *reply_msg, TELCOVOICEMGR_DML_VOICESERVICE_STATS *stVoiceStats);

#define HALINIT  "Devices.Services.VoiceHalInit"
/*******************************************************************************
 *                        LOCAL FUNCTION TYPES
 *******************************************************************************/

/**
 * @brief Utility API to create json request message to send to the interface manager
 *
 * @param request_type (IN)   - Type of request message (GET/SET)
 * @param param_name   (IN)   - String holds the parameter name. This contains the absolute DML path for the parameter.
 * @param type         (IN)   - Type of the parameter needs to be passed to lower layer.
 * @param param_val    (IN)   - String holds the value of the parameter needs to set. This string contains the data based on the type.
 * @return Status of the operation
 * @retval ANSC_STATUS_SUCCESS if successful
 * @retval ANSC_STATUS_FAILURE if any error is detected
 */

/**************************************************************************************************
 * PUBLIC APIS
 **************************************************************************************************/
ANSC_STATUS TelcoVoiceMgrHal_Init()
{
    bool bStatus = 1;
    if (json_hal_client_init(TELCOVOICEMGR_CONF_FILE) != RETURN_OK)
    {
        CcspTraceError(("%s - %d Failed to initialise json hal client library \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if (json_hal_client_run() != RETURN_OK)
    {
        CcspTraceError(("%s - %d Failed to start the json hal client module \n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    /**
     * Make sure HAL client connected to server.
     * Here waits for 10seconds time to check whether connection established or not.
     * If connection not established within this time, returned error.
     */
    int retry_count = 0;
    int is_client_connected = 0;
    while (retry_count < HAL_CONNECTION_RETRY_MAX_COUNT)
    {
        if (!json_hal_is_client_connected())
        {
            sleep(1);
            retry_count++;
        }
        else
        {
            CcspTraceInfo(("Hal-client connected to the hal server \n"));
            is_client_connected = TRUE;
            break;
        }
    }

    if (is_client_connected != TRUE)
    {
        CcspTraceInfo(("Failed to connect to the hal server. \n"));
        return ANSC_STATUS_FAILURE;
    }
    if(TelcoVoiceMgrHal_InitData(bStatus) !=  ANSC_STATUS_SUCCESS)
    {
        CcspTraceInfo(("Failed to initialise data\n"));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;

}

ANSC_STATUS TelcoVoiceMgrHal_InitData(bool bStatus)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE, "%s", HALINIT);
    if(bStatus)
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","true");
    }
    else
    {
        snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","false");
    }
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_BOOLEAN,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

static json_object *create_json_request_message(eActionType request_type, const CHAR *param_name, eParamType type, CHAR *param_val)
{
    CcspTraceInfo(("%s - %d [VAV] Enter \n", __FUNCTION__, __LINE__));

    json_object *jrequest = NULL;
    json_object *jparam_arr = NULL;
    json_object *jparam_obj = NULL;
    hal_param_t stParam;
    memset(&stParam, 0, sizeof(stParam));
    json_bool val;
    switch (request_type)
    {
        case SET_REQUEST_MESSAGE:
            jrequest = json_hal_client_get_request_header(SET_PARAMETER_METHOD);
            strncpy(stParam.name, param_name, sizeof(stParam.name)-1);
            stParam.type = type;
            switch (type)
            {
                case PARAM_BOOLEAN:
                case PARAM_INTEGER:
                case PARAM_UNSIGNED_INTEGER:
                case PARAM_STRING:
                {
                    strncpy(stParam.value,param_val,sizeof(stParam.value)-1);
                    json_hal_add_param(jrequest, SET_REQUEST_MESSAGE, &stParam);
                    break;
                }
                default:
                {
                    fprintf(stderr,"Invalid input\n");
                    break;
                }
            }
        break;
        case GET_REQUEST_MESSAGE:
            jrequest = json_hal_client_get_request_header(GET_PARAMETER_METHOD);
            strncpy(stParam.name, param_name, sizeof(stParam.name)-1);
            json_hal_add_param(jrequest, GET_REQUEST_MESSAGE, &stParam);
            break;
    }
    return jrequest;
}

ANSC_STATUS TelcoVoiceMgrHal_GetInitData()
{
    ANSC_STATUS retStatus = ANSC_STATUS_FAILURE;

    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrData = TelcoVoiceMgrDmlGetDataLocked();

    if(pTelcoVoiceMgrData != NULL)
    {
        retStatus = TelcoVoiceMgrHal_GetVoiceServices(&(pTelcoVoiceMgrData->Service.VoiceService));
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
    }
    return retStatus;
}

ANSC_STATUS TelcoVoiceHal_GetSingleParameter(hal_param_t *get_param)
{
    CHECK(get_param != NULL);

    /**
     * Telcovoice manager mainly required the following params:
     * 1. Enable
     * 2. Line Status
     * 3. Call Status
     */
    ANSC_STATUS rc = ANSC_STATUS_SUCCESS;

    rc = voice_process_get_info(get_param);
    if (rc != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("%s - %d Failed to get value for param [%s]\n", __FUNCTION__, __LINE__, get_param->name));
    }

    return rc;
}

static ANSC_STATUS voice_process_get_info(hal_param_t *get_param)
{
    CHECK(get_param != NULL);

    json_object *jreply_msg;
    json_object *jrequest;
    hal_param_t resp_param;
    int rc = ANSC_STATUS_FAILURE;

    jrequest = create_json_request_message(GET_REQUEST_MESSAGE, get_param->name, NULL_TYPE , NULL);
    CHECK(jrequest != NULL);

    CcspTraceInfo(("%s - %d Json request message = %s \n", __FUNCTION__, __LINE__, json_object_to_json_string_ext(jrequest, JSON_C_TO_STRING_PRETTY)));

    if (json_hal_client_send_and_get_reply(jrequest, &jreply_msg) != RETURN_OK)
    {
        CcspTraceError(("%s - %d Failed to get reply for the json request \n", __FUNCTION__, __LINE__));
        FREE_JSON_OBJECT(jrequest);
        FREE_JSON_OBJECT(jreply_msg);
        return rc;
    }

    CHECK(jreply_msg != NULL);
    CcspTraceInfo(("Got Json response \n = %s \n", json_object_to_json_string_ext(jreply_msg, JSON_C_TO_STRING_PRETTY)));

    if (json_hal_get_param(jreply_msg, JSON_RPC_PARAM_ARR_INDEX, GET_RESPONSE_MESSAGE, &resp_param) != RETURN_OK)
    {
        CcspTraceError(("%s - %d Failed to get required params from the response message \n", __FUNCTION__, __LINE__));
        return rc;
    }

    strncpy(get_param->value, resp_param.value, sizeof(resp_param.value));

    // Free json objects.
    FREE_JSON_OBJECT(jrequest);
    FREE_JSON_OBJECT(jreply_msg);
    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS TelcoVoiceHal_GetLineStats(const char *param_name, TELCOVOICEMGR_DML_VOICESERVICE_STATS *pLineStats)
{
    CHECK(param_name != NULL);
    CHECK(pLineStats != NULL);

    ANSC_STATUS rc = ANSC_STATUS_SUCCESS;

    json_object *jreply_msg = NULL;
    json_object *jrequest = create_json_request_message(GET_REQUEST_MESSAGE, param_name, NULL_TYPE , NULL);
    CHECK(jrequest != NULL);

    if (json_hal_client_send_and_get_reply(jrequest, &jreply_msg) != RETURN_OK)
    {
        CcspTraceError(("%s - %d Failed to get reply for the json request \n", __FUNCTION__, __LINE__));
        FREE_JSON_OBJECT(jrequest);
        FREE_JSON_OBJECT(jreply_msg);
        return ANSC_STATUS_FAILURE;
    }

    rc = get_voice_line_stats(jreply_msg, pLineStats);
    if (rc != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("%s - %d Failed to get statistics data  \n", __FUNCTION__, __LINE__));
    }

    // Free json objects.
    FREE_JSON_OBJECT(jrequest);
    FREE_JSON_OBJECT(jreply_msg);
    return rc;
}


static ANSC_STATUS get_voice_line_stats(const json_object *reply_msg, TELCOVOICEMGR_DML_VOICESERVICE_STATS *stVoiceStats)
{
    ANSC_STATUS rc = ANSC_STATUS_SUCCESS;
    int total_param_count = 0;

    total_param_count = json_hal_get_total_param_count(reply_msg);
    hal_param_t resp_param;

    /**
     * Traverse through each index and retrieve value.
     */
    for (int i = 0; i < total_param_count; ++i)
    {
        if (json_hal_get_param(reply_msg, i, GET_RESPONSE_MESSAGE, &resp_param) != RETURN_OK)
        {
            CcspTraceError(("%s - %d Failed to get the param from response message [index = %d] \n", __FUNCTION__, __LINE__, i));
            continue;
        }
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        if( strstr(resp_param.name, "RTP.BytesReceived") )
        {
            stVoiceStats->BytesReceived = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "RTP.BytesSent") )
        {
            stVoiceStats->BytesSent = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "IncomingCalls.CallsConnected") )
        {
            stVoiceStats->IncomingCallsConnected = atol(resp_param.value);
            stVoiceStats->IncomingCallsAnswered= atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "IncomingCalls.CallsFailed") )
        {
            stVoiceStats->IncomingCallsFailed = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "IncomingCalls.CallsReceived") )
        {
            stVoiceStats->IncomingCallsReceived= atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "OutgoingCalls.CallsAttempted") )
        {
            stVoiceStats->OutgoingCallsAttempted = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "OutgoingCalls.CallsConnected") )
        {
            stVoiceStats->OutgoingCallsConnected = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "OutgoingCalls.CallsFailed") )
        {
            stVoiceStats->OutgoingCallsFailed = atol(resp_param.value);
        }
#else
        if( strstr(resp_param.name, "AverageFarEndInterarrivalJitter") )
        {
            stVoiceStats->AverageFarEndInterarrivalJitter = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "AverageReceiveInterarrivalJitter") )
        {
            stVoiceStats->AverageReceiveInterarrivalJitter = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "AverageRoundTripDelay") )
        {
            stVoiceStats->AverageRoundTripDelay = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "BytesReceived") )
        {
            stVoiceStats->BytesReceived = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "BytesSent") )
        {
            stVoiceStats->BytesSent = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "FarEndInterarrivalJitter") )
        {
            stVoiceStats->FarEndInterarrivalJitter = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "FarEndPacketLossRate") )
        {
            stVoiceStats->FarEndPacketLossRate = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "IncomingCallsAnswered") )
        {
            stVoiceStats->IncomingCallsAnswered= atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "IncomingCallsConnected") )
        {
            stVoiceStats->IncomingCallsConnected = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "IncomingCallsFailed") )
        {
            stVoiceStats->IncomingCallsFailed = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "IncomingCallsReceived") )
        {
            stVoiceStats->IncomingCallsReceived= atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "OutgoingCallsAnswered") )
        {
            stVoiceStats->OutgoingCallsAnswered = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "OutgoingCallsAttempted") )
        {
            stVoiceStats->OutgoingCallsAttempted = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "OutgoingCallsConnected") )
        {
            stVoiceStats->OutgoingCallsConnected = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "OutgoingCallsFailed") )
        {
            stVoiceStats->OutgoingCallsFailed = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "PacketsLost") )
        {
            stVoiceStats->PacketsLost = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "PacketsReceived") )
        {
            stVoiceStats->PacketsReceived = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "PacketsSent") )
        {
            stVoiceStats->PacketsSent = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "ReceiveInterarrivalJitter") )
        {
            stVoiceStats->ReceiveInterarrivalJitter = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "ReceivePacketLossRate") )
        {
            stVoiceStats->ReceivePacketLossRate = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "RoundTripDelay") )
        {
            stVoiceStats->RoundTripDelay = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "Overruns") )
        {
            stVoiceStats->Overruns = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "Underruns") )
        {
            stVoiceStats->Underruns = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "CallsDropped") )
        {
            stVoiceStats->CallsDropped = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "TotalCallTime") )
        {
            stVoiceStats->TotalCallTime = atol(resp_param.value);
        }
        else if( strstr(resp_param.name, "ServerDownTime") )
        {
            stVoiceStats->ServerDownTime = atol(resp_param.value);
        }
#endif //FEATURE_RDKB_VOICE_DM_TR104_V2
        else
        {
            CcspTraceWarning(("%s::Unknown ParamName:%s\n", __FUNCTION__, resp_param.name));
        }
    }

    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrHal_SendJsonRequest: */
/**
* @description Function to send json hal set request.
* Prepare the json message and pass it here as a function parameter.
*
* json_object *jmsg - Json set request message.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrHal_SendJsonRequest(json_object *jmsg)
{
    ANSC_STATUS rc = ANSC_STATUS_SUCCESS;
    json_object *jreply_msg = NULL;
    json_bool status = FALSE;

    CcspTraceInfo(("JSON Request message = %s \n", json_object_to_json_string_ext(jmsg, JSON_C_TO_STRING_PRETTY)));
    if( json_hal_client_send_and_get_reply(jmsg, &jreply_msg) != RETURN_OK)
    {
        CcspTraceError(("[%s][%d] RPC message failed \n", __FUNCTION__, __LINE__));
        FREE_JSON_OBJECT(jmsg);
        FREE_JSON_OBJECT(jreply_msg);
        return RETURN_ERR;
    }
    CHECK(jreply_msg);
    
    if (json_hal_get_result_status(jreply_msg, &status) == RETURN_OK)
    {
        if (status)
        {
            CcspTraceInfo(("%s - %d Set request is successful ", __FUNCTION__, __LINE__));
            rc = RETURN_OK;
        }
        else
        {
            CcspTraceError(("%s - %d - Set request is failed \n", __FUNCTION__, __LINE__));
            rc = RETURN_ERR;
        }
    }
    else
    {
        CcspTraceError(("%s - %d Failed to get result status from json response, something wrong happened!!! \n", __FUNCTION__, __LINE__));
        rc = RETURN_ERR;
    }
    
    // Free json objects.
    FREE_JSON_OBJECT(jmsg);
    FREE_JSON_OBJECT(jreply_msg);
    
    return rc;
}

ANSC_STATUS TelcoVoiceMgrHal_SetLinkUp(ULONG uVsIndex, char *dns_server_address, char *ipAddrFamily, char *wanIpAddress)
{
    ANSC_STATUS rc = ANSC_STATUS_SUCCESS;
    json_object *jmsg = NULL;
    hal_param_t param;

    if((dns_server_address == NULL) || (ipAddrFamily == NULL) || (wanIpAddress == NULL))
    {
        CcspTraceError(("Error: Invalid arguement \n"));
        return ANSC_STATUS_FAILURE;      
    }
    jmsg = json_hal_client_get_request_header(SET_PARAMETER_METHOD);
    CHECK(jmsg);

    memset(&param, 0, sizeof(param));
    snprintf(param.name, sizeof(param), VOICE_SERVICE_TABLE_NAME"%s", uVsIndex, "X_RDK_Enable");
    snprintf(param.value, sizeof(param.value), "%lu", VOICE_SERVICE_ENABLE);
    param.type = PARAM_UNSIGNED_INTEGER;
    json_hal_add_param(jmsg, SET_REQUEST_MESSAGE, &param);

    memset(&param, 0, sizeof(param));
    snprintf(param.name, sizeof(param), VOICE_SERVICE_TABLE_NAME"%s", uVsIndex, "X_RDK_DnsServers");
    snprintf(param.value, sizeof(param.value), "%s", dns_server_address);
    param.type = PARAM_STRING;
    json_hal_add_param(jmsg, SET_REQUEST_MESSAGE, &param);

    memset(&param, 0, sizeof(param));
    snprintf(param.name, sizeof(param), VOICE_SERVICE_TABLE_NAME"%s", uVsIndex, "X_RDK_BoundIpAddr");
    snprintf(param.value, sizeof(param.value), "%s", wanIpAddress);
    param.type = PARAM_STRING;
    json_hal_add_param(jmsg, SET_REQUEST_MESSAGE, &param);

    memset(&param, 0, sizeof(param));
    snprintf(param.name, sizeof(param), VOICE_SERVICE_TABLE_NAME"%s", uVsIndex, "X_RDK_IpAddressFamily ");
    snprintf(param.value, sizeof(param.value), "%s", ipAddrFamily);
    param.type = PARAM_STRING;
    json_hal_add_param(jmsg, SET_REQUEST_MESSAGE, &param);

    CcspTraceInfo(("JSON Request message = %s \n", json_object_to_json_string_ext(jmsg, JSON_C_TO_STRING_PRETTY)));

    rc = TelcoVoiceMgrHal_SendJsonRequest(jmsg);

    return rc;
}

ANSC_STATUS TelcoVoiceMgrHal_GetVoiceServices(DML_VOICE_SERVICE_LIST_T* pVoiceServiceList)
{
    if (pVoiceServiceList == NULL)
    {
        fprintf(stderr,"%s - %d Invalid argument \n", __FUNCTION__, __LINE__);
        return ANSC_STATUS_FAILURE;
    }

    int total_param_count = 0;
    hal_param_t resp_param;
    json_object *jreply_msg = NULL;
    memset(&resp_param, 0, sizeof(resp_param));

    json_object *jrequest = create_json_request_message(GET_REQUEST_MESSAGE, TELCOVOICE_QUERY_VOICESERVICE, NULL_TYPE , NULL);

    CHECK(jrequest != NULL);

    if (json_hal_client_send_and_get_reply(jrequest, &jreply_msg) != RETURN_OK)
    {
        fprintf(stderr,"%s - %d Failed to get reply for the json request \n", __FUNCTION__, __LINE__);
        // Free json objects.
        FREE_JSON_OBJECT(jrequest);
        FREE_JSON_OBJECT(jreply_msg);
        return ANSC_STATUS_FAILURE;
    }

    total_param_count = json_hal_get_total_param_count(jreply_msg);

    for (int i = 0; i < total_param_count; ++i)
    {
        if (json_hal_get_param(jreply_msg, i, GET_RESPONSE_MESSAGE, &resp_param) != RETURN_OK)
        {
            fprintf(stderr,"%s - %d Failed to get the param from response message [index = %d] \n", __FUNCTION__, __LINE__, i);
            continue;
        }
        Map_hal_dml_voiceService(pVoiceServiceList,resp_param.name,resp_param.value);
    }

    // Free json objects.
    FREE_JSON_OBJECT(jrequest);
    FREE_JSON_OBJECT(jreply_msg);

    return ANSC_STATUS_SUCCESS;
}

#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
ANSC_STATUS TelcoVoiceMgrHal_GetCapabilities(PTELCOVOICEMGR_DML_CAPABILITIES pCapabilities)
{

    ANSC_STATUS rc = ANSC_STATUS_SUCCESS;

    if (pCapabilities == NULL)
    {
        fprintf(stderr,"%s - %d Invalid argument \n", __FUNCTION__, __LINE__);
        return ANSC_STATUS_FAILURE;
    }

    int total_param_count = 0;
    hal_param_t resp_param;
    json_object *jreply_msg = NULL;
    memset(&resp_param, 0, sizeof(resp_param));

    json_object *jrequest = create_json_request_message(GET_REQUEST_MESSAGE,  TELCOVOICE_QUERY_CAPABILITIES, NULL_TYPE , NULL);

    CHECK(jrequest != NULL);

    if (json_hal_client_send_and_get_reply(jrequest, &jreply_msg) != RETURN_OK)
    {
        fprintf(stderr,"%s - %d Failed to get reply for the json request \n", __FUNCTION__, __LINE__);
        // Free json objects.
        FREE_JSON_OBJECT(jrequest);
        FREE_JSON_OBJECT(jreply_msg);
        return ANSC_STATUS_FAILURE;
    }

    total_param_count = json_hal_get_total_param_count(jreply_msg);

    for (int i = 0; i < total_param_count; ++i)
    {
        if (json_hal_get_param(jreply_msg, i, GET_RESPONSE_MESSAGE, &resp_param) != RETURN_OK)
        {
            fprintf(stderr,"%s - %d Failed to get the param from response message [index = %d] \n", __FUNCTION__, __LINE__, i);
            continue;
        }
        rc = Map_hal_dml_capabilities(pCapabilities,resp_param.name,resp_param.value);
    }
    if (rc != ANSC_STATUS_SUCCESS)
    {
        fprintf(stderr,"%s - %d Failed to get data  \n", __FUNCTION__, __LINE__);
    }

    // Free json objects.
    FREE_JSON_OBJECT(jrequest);
    FREE_JSON_OBJECT(jreply_msg);

    return rc;
}

ANSC_STATUS TelcoVoiceMgrHal_GetVoiceProfile(DML_PROFILE_LIST_T* pVoiceProfileList, int vsIndex)
{
    ANSC_STATUS rc = ANSC_STATUS_SUCCESS;
    int total_param_count = 0;
    hal_param_t resp_param;
    char paramName[JSON_MAX_STR_ARR_SIZE] = { 0 };
    json_object *jreply_msg = NULL;
    memset(&resp_param, 0, sizeof(resp_param));

    if ((pVoiceProfileList == NULL ) || (vsIndex <=0))
    {
        fprintf(stderr,"%s - %d Invalid argument \n", __FUNCTION__, __LINE__);
        return ANSC_STATUS_FAILURE;
    }

    snprintf(paramName, JSON_MAX_STR_ARR_SIZE, TELCOVOICE_QUERY_VOICEPROFILE, vsIndex);
    json_object *jrequest = create_json_request_message(GET_REQUEST_MESSAGE, paramName, NULL_TYPE , NULL);

    CHECK(jrequest != NULL);

    if (json_hal_client_send_and_get_reply(jrequest, &jreply_msg) != RETURN_OK)
    {
        fprintf(stderr,"%s - %d Failed to get reply for the json request \n", __FUNCTION__, __LINE__);
        // Free json objects.
        FREE_JSON_OBJECT(jrequest);
        FREE_JSON_OBJECT(jreply_msg);
        return ANSC_STATUS_FAILURE;
    }


    total_param_count = json_hal_get_total_param_count(jreply_msg);


    for (int i = 0; i < total_param_count; ++i)
    {
        if (json_hal_get_param(jreply_msg, i, GET_RESPONSE_MESSAGE, &resp_param) != RETURN_OK)
        {
            fprintf(stderr,"%s - %d Failed to get the param from response message [index = %d] \n", __FUNCTION__, __LINE__, i);
            continue;
        }
        Map_hal_dml_voiceProfile(pVoiceProfileList, resp_param.name, resp_param.value);
    }



    // Free json objects.
    FREE_JSON_OBJECT(jrequest);
    FREE_JSON_OBJECT(jreply_msg);

    return rc;
}

ANSC_STATUS TelcoVoiceMgrHal_GetPhyInterface(DML_PHYINTERFACE_LIST_T* pPhyInterfaceList, int vsIndex)
{
    ANSC_STATUS rc = ANSC_STATUS_SUCCESS;
    int total_param_count = 0;
    char paramName[JSON_MAX_STR_ARR_SIZE] = { 0 };
    hal_param_t resp_param;
    json_object *jreply_msg = NULL;
    memset(&resp_param, 0, sizeof(resp_param));

    if ((pPhyInterfaceList == NULL) || (vsIndex <=0))
    {
        fprintf(stderr,"%s - %d Invalid argument \n", __FUNCTION__, __LINE__);
        return ANSC_STATUS_FAILURE;
    }

    snprintf(paramName, JSON_MAX_STR_ARR_SIZE, TELCOVOICE_QUERY_PHYINTERFACE, vsIndex);
    json_object *jrequest = create_json_request_message(GET_REQUEST_MESSAGE, paramName, NULL_TYPE , NULL);

    CHECK(jrequest != NULL);

    if (json_hal_client_send_and_get_reply(jrequest, &jreply_msg) != RETURN_OK)
    {
        fprintf(stderr,"%s - %d Failed to get reply for the json request \n", __FUNCTION__, __LINE__);
        // Free json objects.
        FREE_JSON_OBJECT(jrequest);
        FREE_JSON_OBJECT(jreply_msg);
        return ANSC_STATUS_FAILURE;
    }


    //check all messages
    total_param_count = json_hal_get_total_param_count(jreply_msg);

    for (int i = 0; i < total_param_count; ++i)
    {
        if (json_hal_get_param(jreply_msg, i, GET_RESPONSE_MESSAGE, &resp_param) != RETURN_OK)
        {
            fprintf(stderr,"%s - %d Failed to get the param from response message [index = %d] \n", __FUNCTION__, __LINE__, i);
            continue;
        }
        Map_hal_dml_phyInterface(pPhyInterfaceList, resp_param.name, resp_param.value);
    }


    // Free json objects.
    FREE_JSON_OBJECT(jrequest);
    FREE_JSON_OBJECT(jreply_msg);

    return rc;
}
#endif

ANSC_STATUS TelcoVoiceMgrHal_SetParam(char *pName, eParamType pType, char *pValue)
{

    json_object *jrequest;
    int rc = ANSC_STATUS_FAILURE;
    jrequest = create_json_request_message(SET_REQUEST_MESSAGE, pName, pType, pValue);
    CHECK(jrequest != NULL);

    rc = TelcoVoiceMgrHal_SendJsonRequest(jrequest);

    return rc;
}

ANSC_STATUS get_event_param(const char* msg, const int len, char* event_name, char* event_val)
{
    json_object* msg_param = NULL;
    json_object* msg_param_val = NULL;

    if(msg == NULL) {
        return ANSC_STATUS_FAILURE;
    }
    /* Parse message and find event received. */
    json_object *jobj = json_tokener_parse(msg);
    CHECK(jobj);

    if (json_object_object_get_ex(jobj, JSON_RPC_FIELD_PARAMS, &msg_param))
    {
        json_object *jsubs_param_array = json_object_array_get_idx(msg_param, 0);
        if (jsubs_param_array == NULL)
        {
            CcspTraceError(("Failed to get params data from subscription json message \n"));
            FREE_JSON_OBJECT(jobj);
            return ANSC_STATUS_FAILURE;
        }

        if (json_object_object_get_ex(jsubs_param_array, JSON_RPC_FIELD_NAME, &msg_param_val))
        {
            strncpy(event_name, json_object_get_string(msg_param_val),255);
            CcspTraceInfo(("Event name = %s \n", event_name));
        }
        else
        {
            CcspTraceError(("Failed to get event name data from subscription json message \n"));
            FREE_JSON_OBJECT(jobj);
            return ANSC_STATUS_FAILURE;
        }
        if (json_object_object_get_ex(jsubs_param_array, JSON_RPC_FIELD_VALUE, &msg_param_val))
        {
            strncpy(event_val, json_object_get_string(msg_param_val),255);
            CcspTraceInfo(("Event value = %s \n", event_val));
        }
        else
        {
            CcspTraceError(("Failed to get event value data from subscription json message \n"));
            FREE_JSON_OBJECT(jobj);
            return ANSC_STATUS_FAILURE;
        }
    }

    FREE_JSON_OBJECT(jobj);

    CcspTraceInfo(("Event got for %s and its value =%s \n", event_name, event_val));

    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS TelcoVoiceMgrHal_SetParamBool(char* HalName, BOOL bValue)
{
    char strValue[8] = {0};
    if (bValue == TRUE)
    {
        strncpy(strValue, JSON_STR_TRUE, strlen(JSON_STR_TRUE)+1);
    }
    else
    {
        strncpy(strValue, JSON_STR_FALSE, strlen(JSON_STR_FALSE)+1);
    }
    if (TelcoVoiceMgrHal_SetParam(HalName, PARAM_BOOLEAN, strValue) == ANSC_STATUS_SUCCESS)
    {
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS TelcoVoiceMgrHal_SetParamInt(char* HalName, INT iValue)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};

    snprintf(strValue, JSON_MAX_VAL_ARR_SIZE, "%d", iValue);

    if (TelcoVoiceMgrHal_SetParam(HalName, PARAM_INTEGER, strValue) == ANSC_STATUS_SUCCESS)
    {
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS TelcoVoiceMgrHal_SetParamULong(char* HalName, ULONG uValue)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};

    snprintf(strValue, JSON_MAX_VAL_ARR_SIZE, "%ld", uValue);

    if (TelcoVoiceMgrHal_SetParam(HalName, PARAM_UNSIGNED_INTEGER, strValue) == ANSC_STATUS_SUCCESS)
    {
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_FAILURE;
}

ANSC_STATUS TelcoVoiceMgrHal_SetParamString(char* HalName, char* sValue)
{

    if (TelcoVoiceMgrHal_SetParam(HalName, PARAM_STRING, sValue) == ANSC_STATUS_SUCCESS)
    {
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_FAILURE;
}

void eventcb_FirewallRuleData(const char *msg, const int len)
{
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    char event_name[256] = {'\0'};
    char event_val[256] = {'\0'};
    int  uVsIndex            = 0;
    int  uVpIndex            = 0;
    int  uIndex              = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;

    ret = get_event_param(msg, len, event_name, event_val);
    if(ret == ANSC_STATUS_SUCCESS)
    {
        if (strstr(event_name, FIREWALL_RULE_DATA_EVENT))
        {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            if(ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_VoiceService_index(event_name, VOICE_SERVICE_TABLE_NAME, &uVsIndex))
#else
            if(ANSC_STATUS_FAILURE == telcovoicemgr_hal_get_voiceService_index(event_name, VOICE_SERVICE_TABLE_NAME, &uVsIndex))
#endif
            {
                return ANSC_STATUS_FAILURE;
            }
            if( uVsIndex <= 0 )
            {
                AnscTraceError(("\n%s:%d:: \nInvalid index ParamName[%s]", __FUNCTION__, __LINE__, event_name));
                return ANSC_STATUS_FAILURE;
            }
            TELCOVOICEMGR_DML_DATA*  pTelcoVoiceMgrData = TelcoVoiceMgrDmlGetDataLocked();
            if(pTelcoVoiceMgrData != NULL)
            {
                DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex - 1];
                pDmlVoiceService = &(pVoiceService->dml);
                if(pDmlVoiceService == NULL)
                {
                    //create new VoiceService
                    TelcoVoiceMgrDmlAddVoiceService(&(pTelcoVoiceMgrData->Service.VoiceService), uVsIndex - 1);

                    pDmlVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex - 1];
                    if(pDmlVoiceService == NULL)
                    {
                        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
                        return ANSC_STATUS_FAILURE;
                    }
                }
                ret = Map_hal_dml_voiceService(&(pTelcoVoiceMgrData->Service.VoiceService), event_name, event_val);
                if(ret == ANSC_STATUS_SUCCESS)
                {
                    TelcoVoiceMgrDmlSetX_RDK_FirewallRuleData(pDmlVoiceService);
                }
                else
                {
                    CcspTraceWarning(("[%s: %d] Sysevent set for firewall rules failed\n", __FUNCTION__, __LINE__));
                }
                TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
            }
        }
    }
}

ANSC_STATUS TelcoVoiceMgrHal_EventSubscribe(event_callback callback, const char* event_name, const char* event_notification_type)
{
    if(json_hal_client_subscribe_event(callback, event_name, event_notification_type) == 0)
    {
        return ANSC_STATUS_SUCCESS;
    }
    return ANSC_STATUS_FAILURE;
}

