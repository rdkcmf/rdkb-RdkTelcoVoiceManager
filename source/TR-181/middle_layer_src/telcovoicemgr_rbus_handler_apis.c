/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2021 RDK Management
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
/*
 * Copyright [2014] [Cisco Systems, Inc.]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     LICENSE-2.0" target="_blank" rel="nofollow">http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifdef RBUS_BUILD_FLAG_ENABLE
#include <rbus.h>
#include "telcovoicemgr_rbus_handler_apis.h"
#include "telcovoicemgr_dml_apis.h"
static rbusHandle_t rbusHandle;

char componentName[32] = "TELCOVOICEMANAGER";

/***********************************************************************

  Data Elements declaration:

 ***********************************************************************/
ANSC_STATUS TelcoVoiceMgr_Rbus_String_getParamValue (char * param, char * value)
{
    if ((param == NULL) || (value == NULL))
    {
        CcspTraceError(("%s %d: invalid args\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }

    if (rbus_getStr(rbusHandle, param, value) != RBUS_ERROR_SUCCESS)
    {
        CcspTraceError(("%s %d: unbale to get value of param %s\n", __FUNCTION__, __LINE__, param));
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

static void TelcoVoiceMgr_Rbus_EventReceiveHandler(rbusHandle_t handle, rbusEvent_t const* event, rbusEventSubscription_t* subscription)
{
    (void)handle;
    (void)subscription;

    const char* eventName = event->name;
    rbusValue_t valBuff = rbusObject_GetValue(event->data, NULL );

    if((valBuff == NULL) || (eventName == NULL))
    {
        CcspTraceError(("%s : FAILED , value is NULL\n",__FUNCTION__));
        return;
    }
    if (strcmp(eventName, WANMGR_CONFIG_WAN_INTERFACEAVAILABLESTATUS) == 0)
    {
        const char* newValue ;
        newValue = rbusValue_GetString(valBuff,NULL);
        CcspTraceInfo(("%s:%d Received [%s:%s]\n",__FUNCTION__, __LINE__,eventName, newValue));
    }
    else if (strcmp(eventName, WANMGR_CONFIG_WAN_INTERFACEACTIVESTATUS) == 0)
    {
        const char* newValue ;
        newValue = rbusValue_GetString(valBuff,NULL);
        if(strstr(newValue,REMOTE_LTE_ENABLED))
        {
         CcspTraceInfo(("%s:%d ***Disable Voice Stack*** Remote-LTE Received [%s:%s]\n",__FUNCTION__, __LINE__,eventName, newValue));
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
         TelcoVoiceMgr_setCallControlLineEnable(false);
#endif
        }
        else
        {
         CcspTraceInfo(("%s:%d ***Enable Voice Stack*** No Remote-LTE Received [%s:%s]\n",__FUNCTION__, __LINE__,eventName, newValue));
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
         TelcoVoiceMgr_setCallControlLineEnable(true);
#endif
        }
        CcspTraceInfo(("%s:%d Received [%s:%s]\n",__FUNCTION__, __LINE__,eventName, newValue));
    }
    else
    {
        CcspTraceError(("%s:%d Unexpected Event Received [%s]\n",__FUNCTION__, __LINE__,eventName));
    }
}


void TelcoVoiceMgr_Rbus_SubscribeDML(void)
{
    rbusError_t ret = RBUS_ERROR_SUCCESS;

    ret = rbusEvent_Subscribe(rbusHandle, WANMGR_CONFIG_WAN_INTERFACEAVAILABLESTATUS, TelcoVoiceMgr_Rbus_EventReceiveHandler, NULL, 0);
    if(ret != RBUS_ERROR_SUCCESS)
    {
        CcspTraceError(("%s %d - Failed to Subscribe %s, Error=%s \n", __FUNCTION__, __LINE__, rbusError_ToString(ret), WANMGR_CONFIG_WAN_INTERFACEAVAILABLESTATUS));
    }

    ret = rbusEvent_Subscribe(rbusHandle, WANMGR_CONFIG_WAN_INTERFACEACTIVESTATUS, TelcoVoiceMgr_Rbus_EventReceiveHandler, NULL, 0);
    if(ret != RBUS_ERROR_SUCCESS)
    {
        CcspTraceError(("%s %d - Failed to Subscribe %s, Error=%s \n", __FUNCTION__, __LINE__, rbusError_ToString(ret), WANMGR_CONFIG_WAN_INTERFACEACTIVESTATUS));
    }

    CcspTraceInfo(("TelcoVoiceMgr_Rbus_SubscribeDML done\n"));
}

void TelcoVoiceMgr_Rbus_UnSubscribeDML(void)
{
    rbusError_t ret = RBUS_ERROR_SUCCESS;

    ret = rbusEvent_Unsubscribe(rbusHandle, WANMGR_CONFIG_WAN_INTERFACEAVAILABLESTATUS);
    if(ret != RBUS_ERROR_SUCCESS)
    {
        CcspTraceError(("%s %d - Failed to Subscribe %s, Error=%s \n", __FUNCTION__, __LINE__, WANMGR_CONFIG_WAN_INTERFACEAVAILABLESTATUS, rbusError_ToString(ret)));
    }

    ret = rbusEvent_Unsubscribe(rbusHandle, WANMGR_CONFIG_WAN_INTERFACEACTIVESTATUS);
    if(ret != RBUS_ERROR_SUCCESS)
    {
        CcspTraceError(("%s %d - Failed to Subscribe %s, Error=%s \n", __FUNCTION__, __LINE__, WANMGR_CONFIG_WAN_INTERFACEACTIVESTATUS, rbusError_ToString(ret)));
    }

    CcspTraceInfo(("TelcoVoiceMgr_Rbus_UnSubscribeDML done\n"));
}

/***********************************************************************
  TelcoVoiceMgr_Rbus_Init(): Initialize Rbus and data elements
 ***********************************************************************/
ANSC_STATUS TelcoVoiceMgr_Rbus_Init()
{
    int rc = ANSC_STATUS_FAILURE;

    rc = rbus_open(&rbusHandle, componentName);
    if (rc != RBUS_ERROR_SUCCESS)
    {
        CcspTraceError(("TelcoVoiceMgr_Rbus_Init rbus initialization failed\n"));
        return rc;
    }

    return ANSC_STATUS_SUCCESS;
}

/*******************************************************************************
  TelcoVoiceMgr_RbusExit(): Unreg data elements and Exit
 ********************************************************************************/
ANSC_STATUS TelcoVoiceMgr_RbusExit()
{
    CcspTraceInfo(("%s %d - TelcoVoiceMgr_RbusExit called\n", __FUNCTION__, __LINE__ ));

    TelcoVoiceMgr_Rbus_UnSubscribeDML();

    rbus_close(rbusHandle);
    return ANSC_STATUS_SUCCESS;
}

#endif //RBUS_BUILD_FLAG_ENABLE
