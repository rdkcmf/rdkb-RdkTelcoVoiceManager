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

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
/* TR104V2 functionalities */
static ANSC_STATUS TelcoVoiceMgr_subscribe_hal_events(void)
{
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    char strName[BUF_LEN_256]={0};
    ULONG        uVsIndex            = 0;
    ULONG        uVpIndex            = 0;
    ULONG        uIndex              = 0;
    PDML_SIP     pDmlSipObj          = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE   pDmlVoiceService    = NULL;
    PDML_VOIPPROFILE   pDmlVoipProfile   =  NULL;

    TELCOVOICEMGR_DML_DATA*  pTelcoVoiceMgrData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrData != NULL)
    {
        for( uVsIndex = 0; uVsIndex < pTelcoVoiceMgrData->Service.VoiceService.ulQuantity; uVsIndex++)
        {
            DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex];
            pDmlVoiceService = &(pVoiceService->dml);
            if ( pDmlVoiceService )
            {
                pDmlSipObj = &(pDmlVoiceService->SIP_obj);
                if(pDmlSipObj)
                {
                    for (uIndex = 0; uIndex < pDmlSipObj->Network.ulQuantity; uIndex++)
                    {
                        PDML_SIP_NETWORK_CTRL_T    pSipNetworkData = pDmlSipObj->Network.pdata[uIndex];
                        PDML_SIP_NETWORK pDmlSipNetwork = &(pSipNetworkData->dml);
                        //SIP
                        snprintf(strName, BUF_LEN_256, SIP_FIREWALL_RULE_DATA, pDmlVoiceService->InstanceNumber, pDmlSipNetwork->uInstanceNumber);
                        ret = TelcoVoiceMgrHal_EventSubscribe(&eventcb_FirewallRuleData, strName, JSON_SUBSCRIBE_ON_CHANGE);
                        if(ret != ANSC_STATUS_SUCCESS) break;
                    }
                }

                for (uVpIndex = 0; uVpIndex < pDmlVoiceService->VoIPProfile->ulQuantity; uVpIndex++)
                {
                    DML_VOIPPROFILE_CTRL_T* pVoipProfileData = pDmlVoiceService->VoIPProfile->pdata[uVpIndex];
                    pDmlVoipProfile = &(pVoipProfileData->dml);
                    if(pDmlVoipProfile)
                    {
                        //RTP
                        snprintf(strName, BUF_LEN_256, RTP_FIREWALL_RULE_DATA, pDmlVoiceService->InstanceNumber, pDmlVoipProfile->uInstanceNumber);
                        ret = TelcoVoiceMgrHal_EventSubscribe(&eventcb_FirewallRuleData, strName, JSON_SUBSCRIBE_ON_CHANGE);
                        if(ret != ANSC_STATUS_SUCCESS) break;
                    }
                }
            }
        } 
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
    }
    return ret;
}
#else
static ANSC_STATUS TelcoVoiceMgr_subscribe_hal_events(void)
{
    ANSC_STATUS ret = ANSC_STATUS_FAILURE;
    char strName[BUF_LEN_256]={0};
    ULONG        uVsIndex            = 0;
    ULONG        uPhyIfIndex         = 0;
    ULONG        uVpIndex            = 0;
    PTELCOVOICEMGR_DML_PROFILE            pDmlVoiceProfile    = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;

    TELCOVOICEMGR_DML_DATA*  pTelcoVoiceMgrData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrData != NULL)
    {
        for( uVsIndex = 0; uVsIndex < pTelcoVoiceMgrData->Service.VoiceService.ulQuantity; uVsIndex++)
        {
            DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex];
            pDmlVoiceService = &(pVoiceService->dml);
            if ( pDmlVoiceService )
            {
               for (uVpIndex = 0; uVpIndex < pDmlVoiceService->VoiceProfileList.ulQuantity; uVpIndex++)
               {
                    DML_PROFILE_CTRL_T* pVoiceProfile = pDmlVoiceService->VoiceProfileList.pdata[uVpIndex];
                    pDmlVoiceProfile = &(pVoiceProfile->dml);
                    if(pDmlVoiceProfile)
                    {
                        //SIP
                        snprintf(strName, BUF_LEN_256, SIP_FIREWALL_RULE_DATA, pDmlVoiceService->InstanceNumber, pDmlVoiceProfile->InstanceNumber);
                        ret = TelcoVoiceMgrHal_EventSubscribe(&eventcb_FirewallRuleData, strName, JSON_SUBSCRIBE_ON_CHANGE);
                        if(ret != ANSC_STATUS_SUCCESS) break;

                        //RTP
                        snprintf(strName, BUF_LEN_256, RTP_FIREWALL_RULE_DATA, pDmlVoiceService->InstanceNumber, pDmlVoiceProfile->InstanceNumber);
                        ret = TelcoVoiceMgrHal_EventSubscribe(&eventcb_FirewallRuleData, strName, JSON_SUBSCRIBE_ON_CHANGE);
                        if(ret != ANSC_STATUS_SUCCESS) break;

                    }
               }
            }
        }
        
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
    }
    return ret;
}
#endif

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
