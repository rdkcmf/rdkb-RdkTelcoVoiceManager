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


#include "telcovoicemgr_controller.h"
#include "telcovoicemgr_dml_hal.h"

static ANSC_STATUS TelcoVoiceMgr_subscribe_hal_events(void)
{
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    char strName[BUF_LEN_256]={0};
    ULONG        uVsIndex            = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE   pDmlVoiceService    = NULL;

    TELCOVOICEMGR_DML_DATA*  pTelcoVoiceMgrData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrData != NULL)
    {
        for( uVsIndex = 0; uVsIndex < pTelcoVoiceMgrData->Service.VoiceService.ulQuantity; uVsIndex++)
        {
            DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex];
            pDmlVoiceService = &(pVoiceService->dml);
            if ( pDmlVoiceService )
            {
                snprintf(strName, BUF_LEN_256, FIREWALL_RULE_DATA, pDmlVoiceService->InstanceNumber);
                ret = TelcoVoiceMgrHal_EventSubscribe(&eventcb_FirewallRuleData, strName, JSON_SUBSCRIBE_ON_CHANGE);
                if(ret != ANSC_STATUS_SUCCESS) break;
            }
        } 
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
    }
    return ret;
}

ANSC_STATUS TelcoVoiceMgr_Controller_Init()
{
    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;

    //subscribe to events
    returnStatus = TelcoVoiceMgr_subscribe_hal_events();

    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        CcspTraceInfo(("%s %d - Error in HAL subscribe events. \n", __FUNCTION__, __LINE__ ));
        return returnStatus;
    }

    return returnStatus;
}
