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

#include "telcovoicemgr_services_apis_v1.h"

ANSC_STATUS TelcoVoiceMgrDmlDataDelete(void)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    TELCOVOICEMGR_DML_DATA*  pTelcoVoiceMgrData = NULL;
    ULONG                        uVsIndex            = 0;
    ULONG                        uVpIndex            = 0;
    ULONG                        uIndex              = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;
    PTELCOVOICEMGR_DML_PROFILE            pDmlVoiceProfile    = NULL;
    PTELCOVOICEMGR_DML_PHYINTERFACE       pDmlPhyInterface    = NULL;
    PTELCOVOICEMGR_DML_SIP                pDmlSIP             = NULL;
    PTELCOVOICEMGR_DML_LINE               pDmlLine            = NULL;


    pTelcoVoiceMgrData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrData != NULL)
    {
        for( uVsIndex = 0; uVsIndex < pTelcoVoiceMgrData->Service.VoiceService.ulQuantity; uVsIndex++)
        {
            if(pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex] != NULL)
            {
                DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex];
                pDmlVoiceService = &(pVoiceService->dml);

                for (uIndex = 0; uIndex < pDmlVoiceService->PhyInterfaceList.ulQuantity; uIndex++)
                {
                    if(pDmlVoiceService->PhyInterfaceList.pdata[uIndex] != NULL)
                    {
                        AnscFreeMemory((ANSC_HANDLE)pDmlVoiceService->PhyInterfaceList.pdata[uIndex]);
                        pDmlVoiceService->PhyInterfaceList.pdata[uIndex] = NULL;
                    }
                }

                for( uVpIndex = 0; uVpIndex < pDmlVoiceService->VoiceProfileList.ulQuantity; uVpIndex++)
                {
                    if(pDmlVoiceService->VoiceProfileList.pdata[uVpIndex] != NULL)
                    {
                        DML_PROFILE_CTRL_T* pVoiceProfile = pDmlVoiceService->VoiceProfileList.pdata[uVpIndex];
                        pDmlVoiceProfile = &(pVoiceProfile->dml);
                        for (uIndex = 0; uIndex < pDmlVoiceProfile->LineList.ulQuantity; uIndex++)
                        {
                            if(pDmlVoiceProfile->LineList.pdata[uIndex] != NULL)
                            {
                                AnscFreeMemory((ANSC_HANDLE)pDmlVoiceProfile->LineList.pdata[uIndex]);
                                pDmlVoiceProfile->LineList.pdata[uIndex] = NULL;
                            }
                        }
                        AnscFreeMemory((ANSC_HANDLE)pDmlVoiceService->VoiceProfileList.pdata[uVpIndex]);
                        pDmlVoiceService->VoiceProfileList.pdata[uVpIndex] = NULL;
                    }

                }
                AnscFreeMemory((ANSC_HANDLE)pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex]);
                pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex] = NULL;
            }
        }
        
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
        
        //destroy the mutex        
        pthread_mutex_destroy(&(pTelcoVoiceMgrData->mDataMutex));

        //delete data
        AnscFreeMemory((ANSC_HANDLE)pTelcoVoiceMgrData);
        pTelcoVoiceMgrData = NULL;
        
        result = ANSC_STATUS_SUCCESS;
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlSetDefaultData(TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrData)
{
    ANSC_STATUS                  returnStatus        = ANSC_STATUS_SUCCESS;
    ULONG                        uLineIndex          = 0;
    ULONG                        uVsIndex            = 0;
    ULONG                        uPhyIfIndex         = 0;
    ULONG                        uVpIndex            = 0;
    ULONG                        uIndex              = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;
    PTELCOVOICEMGR_DML_LINE               pDmlLine            = NULL;
    PTELCOVOICEMGR_DML_PROFILE            pDmlVoiceProfile    = NULL;
    PTELCOVOICEMGR_DML_PHYINTERFACE       pDmlPhyInterface    = NULL;

    if(pTelcoVoiceMgrData != NULL)
    {
        pTelcoVoiceMgrData->Service.VoiceService.ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES;
        for( uVsIndex = 0; uVsIndex < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES; uVsIndex++)
        {
            pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex] = (DML_VOICE_SERVICE_CTRL_T*) AnscAllocateMemory(sizeof(DML_VOICE_SERVICE_CTRL_T));
            DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex];
            pDmlVoiceService = &(pVoiceService->dml);

            if ( !pDmlVoiceService )
            {
                returnStatus = ANSC_STATUS_RESOURCES;
                CcspTraceError(("%s - Failed pDmlVoiceService : NULL\n", __FUNCTION__));
                goto EXIT;
            }
            pDmlVoiceService->InstanceNumber = uVsIndex + 1;
            pDmlVoiceService->PhyInterfaceList.ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE;

            if ( pDmlVoiceService->PhyInterfaceList.ulQuantity != 0 )
            {
                for (uIndex = 0; uIndex < TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE; uIndex++)
                {
                    pDmlVoiceService->PhyInterfaceList.pdata[uIndex] = (DML_PHYINTERFACE_CTRL_T*) AnscAllocateMemory(sizeof(DML_PHYINTERFACE_CTRL_T));
                    DML_PHYINTERFACE_CTRL_T* pPhyInterface = pDmlVoiceService->PhyInterfaceList.pdata[uIndex];
                    pDmlPhyInterface = &(pPhyInterface->dml);

                    if ( !pDmlPhyInterface )
                    {
                        returnStatus = ANSC_STATUS_RESOURCES;
                        CcspTraceError(("%s - Failed pDmlPhyInterface : NULL\n", __FUNCTION__));
                        goto EXIT;
                    }
                    pDmlPhyInterface->InstanceNumber = uIndex + 1;
                    pDmlPhyInterface->pParentVoiceService = pDmlVoiceService;
                }
            }

            pDmlVoiceService->VoiceProfileList.ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE;

            if ( pDmlVoiceService->VoiceProfileList.ulQuantity != 0 )
            {
                for (uVpIndex = 0; uVpIndex < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE; uVpIndex++)
                {
                    pDmlVoiceService->VoiceProfileList.pdata[uVpIndex] = (DML_PROFILE_CTRL_T*) AnscAllocateMemory(sizeof(DML_PROFILE_CTRL_T));
                    DML_PROFILE_CTRL_T* pVoiceProfile = pDmlVoiceService->VoiceProfileList.pdata[uVpIndex];
                    pDmlVoiceProfile = &(pVoiceProfile->dml);
                    if ( !pDmlVoiceProfile )
                    {
                        returnStatus = ANSC_STATUS_RESOURCES;
                        CcspTraceError(("%s - Failed pDmlVoiceProfile : NULL\n", __FUNCTION__));
                        goto EXIT;
                    }
                    pDmlVoiceProfile->InstanceNumber = uVpIndex + 1;
                    pDmlVoiceProfile->pParentVoiceService = pDmlVoiceService;
                    pDmlVoiceProfile->Enable = ENABLED;
                    pDmlVoiceProfile->LineList.ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_LINE;

                    if (  pDmlVoiceProfile->LineList.ulQuantity != 0 )
                    {
                        for (uLineIndex = 0; uLineIndex < TELCOVOICEMGR_DML_NUMBER_OF_LINE; uLineIndex++)
                        {
                            pDmlVoiceProfile->LineList.pdata[uLineIndex] = (DML_LINE_CTRL_T*) AnscAllocateMemory(sizeof(DML_LINE_CTRL_T));
                            DML_LINE_CTRL_T* pLine = pDmlVoiceProfile->LineList.pdata[uLineIndex];
                            pDmlLine = (PTELCOVOICEMGR_DML_LINE)AnscAllocateMemory( sizeof(TELCOVOICEMGR_DML_LINE));
                            pDmlLine = &(pLine->dml);
                            if ( !pDmlLine )
                            {
                                returnStatus = ANSC_STATUS_RESOURCES;
                                CcspTraceError(("%s - Failed pDmlLine : NULL\n", __FUNCTION__));
                                goto EXIT;
                            }
                            pDmlLine->InstanceNumber = uLineIndex + 1;
                            pDmlLine->pParentVoiceProfile = pDmlVoiceProfile;
                        }
                    }
                }
            }
        }
    }
EXIT:
    return returnStatus;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceService(DML_VOICE_SERVICE_LIST_T* pVoiceServiceList, int vs_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_VOICESERVICE    pDmlVoiceService    = NULL;

    if(pVoiceServiceList != NULL)
    {
        if(vs_index < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES)
        {
            //delete old
            if(pVoiceServiceList->pdata[vs_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceServiceList->pdata[vs_index]);
                pVoiceServiceList->pdata[vs_index] = NULL;
                pVoiceServiceList->ulQuantity--;
            }

            //create new
            pVoiceServiceList->pdata[vs_index] = (DML_VOICE_SERVICE_CTRL_T*) AnscAllocateMemory(sizeof(DML_VOICE_SERVICE_CTRL_T));

            //set default
            if(pVoiceServiceList->pdata[vs_index] != NULL)
            {
                DML_VOICE_SERVICE_CTRL_T* pVoiceService = pVoiceServiceList->pdata[vs_index];
                pVoiceService->updated = false;
                pDmlVoiceService = &(pVoiceService->dml);
                if ( !pDmlVoiceService )
                {
                    memset( pDmlVoiceService, 0, sizeof( TELCOVOICEMGR_DML_VOICESERVICE ) );
                    pDmlVoiceService->InstanceNumber = vs_index + 1;
                }
                pVoiceServiceList->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }
    return result;
}
ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfile(DML_PROFILE_LIST_T* pVoiceProfileList, int vp_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_PROFILE    pDmlVoiceProfile    = NULL;

    if(pVoiceProfileList != NULL)
    {
        if(vp_index < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE)
        {
            //delete old
            if(pVoiceProfileList->pdata[vp_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfileList->pdata[vp_index]);
                pVoiceProfileList->pdata[vp_index] = NULL;
                pVoiceProfileList->ulQuantity--;
            }
 
            //create new
            pVoiceProfileList->pdata[vp_index] = (DML_PROFILE_CTRL_T*) AnscAllocateMemory(sizeof(DML_PROFILE_CTRL_T));

            //set default
            if(pVoiceProfileList->pdata[vp_index] != NULL)
            {
                DML_PROFILE_CTRL_T* pVoiceProfile = pVoiceProfileList->pdata[vp_index];
                pVoiceProfile->updated = false;
                pDmlVoiceProfile = &(pVoiceProfile->dml);
                if ( !pDmlVoiceProfile )
                {   
                    memset( pDmlVoiceProfile, 0, sizeof( TELCOVOICEMGR_DML_VOICEPROFILE ) );
                    pDmlVoiceProfile->InstanceNumber = vp_index + 1;
                }
                pVoiceProfileList->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }
    return result;
}


ANSC_STATUS TelcoVoiceMgrDmlAddPhyInterface(DML_PHYINTERFACE_LIST_T* pPhyInterfaceList, int pi_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_PHYINTERFACE       pDmlPhyInterface    = NULL;

    if(pPhyInterfaceList != NULL)
    {
        if(pi_index < TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE)
        {
            //delete old
            if(pPhyInterfaceList->pdata[pi_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pPhyInterfaceList->pdata[pi_index]);
                pPhyInterfaceList->pdata[pi_index] = NULL;
                pPhyInterfaceList->ulQuantity--;
            }

            //create new
            pPhyInterfaceList->pdata[pi_index] = (DML_PHYINTERFACE_CTRL_T*) AnscAllocateMemory(sizeof(DML_PHYINTERFACE_CTRL_T));
         
            //set default
            if(pPhyInterfaceList->pdata[pi_index] != NULL)
            {
                DML_PHYINTERFACE_CTRL_T* pPhyInterface = pPhyInterfaceList->pdata[pi_index];
                pPhyInterface->updated = false;
                pDmlPhyInterface = &(pPhyInterface->dml);
                if ( !pDmlPhyInterface )
                {
                    memset( pDmlPhyInterface, 0, sizeof( TELCOVOICEMGR_DML_PHYINTERFACE ) );
                    pDmlPhyInterface->InstanceNumber = pi_index + 1;
                }
                pPhyInterfaceList->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}


ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileSIPEventSubscribe(DML_SIP_EVENTSUBSCRIBE_LIST_T* psipEvtSubscribeList, int sipEvtSubscribe_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE       pVoiceProfile_SIP_EventSubscribe    = NULL;

    if(psipEvtSubscribeList != NULL)
    {
        if(sipEvtSubscribe_index < TELCOVOICEMGR_DATA_MAX_DML_SIP_EVENTSUBSCRIBE)
        {
            //delete old
            if(psipEvtSubscribeList->pdata[sipEvtSubscribe_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) psipEvtSubscribeList->pdata[sipEvtSubscribe_index]);
                psipEvtSubscribeList->pdata[sipEvtSubscribe_index] = NULL;
                psipEvtSubscribeList->ulQuantity--;
            }

            //create new
            psipEvtSubscribeList->pdata[sipEvtSubscribe_index] = (DML_SIP_EVENTSUBSCRIBE_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_EVENTSUBSCRIBE_CTRL_T));

            //set default
            if(psipEvtSubscribeList->pdata[sipEvtSubscribe_index] != NULL)
            {
                DML_SIP_EVENTSUBSCRIBE_CTRL_T* psipEvtSubscribeData = psipEvtSubscribeList->pdata[sipEvtSubscribe_index];
                psipEvtSubscribeData->updated = false;
                pVoiceProfile_SIP_EventSubscribe = &(psipEvtSubscribeData->dml);
                if ( !pVoiceProfile_SIP_EventSubscribe )
                {
                    memset( pVoiceProfile_SIP_EventSubscribe, 0, sizeof( TELCOVOICEMGR_DML_SIP_EVENTSUBSCRIBE ) );
                    pVoiceProfile_SIP_EventSubscribe->InstanceNumber = sipEvtSubscribe_index + 1;
                }
                psipEvtSubscribeList->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileSIPRespMap(DML_SIP_RESPONSEMAP_LIST_T* pSIPResponseMapList, int SipRespMap_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_SIP_RESPONSEMAP pVoiceProfile_SIP_RespMap    = NULL;
 
    if(pSIPResponseMapList != NULL)
    {
        if(SipRespMap_index < TELCOVOICEMGR_DATA_MAX_DML_SIP_RESPONSEMAP)
        {
            //delete old
            if(pSIPResponseMapList->pdata[SipRespMap_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSIPResponseMapList->pdata[SipRespMap_index]);
                pSIPResponseMapList->pdata[SipRespMap_index] = NULL;
                pSIPResponseMapList->ulQuantity--;
            }

            //create new
            pSIPResponseMapList->pdata[SipRespMap_index] = (DML_SIP_RESPONSEMAP_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_RESPONSEMAP_CTRL_T));

            //set default
            if(pSIPResponseMapList->pdata[SipRespMap_index] != NULL)
            {
                DML_SIP_RESPONSEMAP_CTRL_T* pSIPResponseMapData = pSIPResponseMapList->pdata[SipRespMap_index];
                pSIPResponseMapData->updated = false;
                pVoiceProfile_SIP_RespMap = &(pSIPResponseMapData->dml);
                if ( !pVoiceProfile_SIP_RespMap )
                {   
                    memset( pVoiceProfile_SIP_RespMap, 0, sizeof( TELCOVOICEMGR_DML_SIP_RESPONSEMAP ) );
                    pVoiceProfile_SIP_RespMap->InstanceNumber = SipRespMap_index + 1;

                }
                pSIPResponseMapList->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddNumberingplanPrefixInfo(DML_NUMBERINGPLAN_PREFIXINFO_LIST_T* pVoiceProfile_Numberingplan_PrefixInfo_List, int PrefixInfo_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO       pVoiceProfile_NumberingPlan_PrefixInfo    = NULL;

    if(pVoiceProfile_Numberingplan_PrefixInfo_List != NULL)
    {
        if(PrefixInfo_index < TELCOVOICEMGR_DATA_MAX_NUMBERINGPLAN_PREFIXINFO)
        {
            //delete old
            if(pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[PrefixInfo_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[PrefixInfo_index]);
                pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[PrefixInfo_index] = NULL;
                pVoiceProfile_Numberingplan_PrefixInfo_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[PrefixInfo_index] = (DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T*) AnscAllocateMemory(sizeof(DML_PHYINTERFACE_CTRL_T));

            //set default
            if(pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[PrefixInfo_index] != NULL)
            {
                DML_NUMBERINGPLAN_PREFIXINFO_CTRL_T* pVoiceProfile_Numberingplan_PrefixInfo_Data = pVoiceProfile_Numberingplan_PrefixInfo_List->pdata[PrefixInfo_index];
                pVoiceProfile_Numberingplan_PrefixInfo_Data->updated = false;
                pVoiceProfile_NumberingPlan_PrefixInfo = &(pVoiceProfile_Numberingplan_PrefixInfo_Data->dml);
                if ( !pVoiceProfile_NumberingPlan_PrefixInfo )
                {   
                    memset( pVoiceProfile_NumberingPlan_PrefixInfo, 0, sizeof( TELCOVOICEMGR_DML_NUMBERINGPLAN_PREFIXINFO ) );
                    pVoiceProfile_NumberingPlan_PrefixInfo->InstanceNumber = PrefixInfo_index + 1;
                }
                pVoiceProfile_Numberingplan_PrefixInfo_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}


ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileToneEvent(DML_TONE_EVENT_LIST_T* pVoiceProfile_Tone_Event_List, int ToneEvent_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_TONE_EVENT       pVoiceProfile_Tone_Event    = NULL;

    if(pVoiceProfile_Tone_Event_List != NULL)
    {
        if(ToneEvent_index < TELCOVOICEMGR_DATA_MAX_TONE_EVENT)
        {
            //delete old
            if(pVoiceProfile_Tone_Event_List->pdata[ToneEvent_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Tone_Event_List->pdata[ToneEvent_index]);
                pVoiceProfile_Tone_Event_List->pdata[ToneEvent_index] = NULL;
                pVoiceProfile_Tone_Event_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Tone_Event_List->pdata[ToneEvent_index] = (DML_TONE_EVENT_CTRL_T*) AnscAllocateMemory(sizeof(DML_TONE_EVENT_CTRL_T));

            //set default
            if(pVoiceProfile_Tone_Event_List->pdata[ToneEvent_index] != NULL)
            {
                DML_TONE_EVENT_CTRL_T* pVoiceProfile_Tone_Event_Data = pVoiceProfile_Tone_Event_List->pdata[ToneEvent_index];
                pVoiceProfile_Tone_Event_Data->updated = false;
                pVoiceProfile_Tone_Event = &(pVoiceProfile_Tone_Event_Data->dml);
                if ( !pVoiceProfile_Tone_Event )
                {
                    memset( pVoiceProfile_Tone_Event, 0, sizeof( TELCOVOICEMGR_DML_TONE_EVENT ) );
                    pVoiceProfile_Tone_Event->InstanceNumber = ToneEvent_index + 1;
                }
                pVoiceProfile_Tone_Event_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileToneDesc(DML_TONE_DESCRIPTION_LIST_T* pVoiceProfile_Tone_Desc_List, int ToneDesc_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_TONE_DESCRIPTION       pVoiceProfile_Tone_Desc    = NULL;

    if(pVoiceProfile_Tone_Desc_List != NULL)
    {
        if(ToneDesc_index < TELCOVOICEMGR_DATA_MAX_TONE_DESCRIPTION)
        {
            //delete old
            if(pVoiceProfile_Tone_Desc_List->pdata[ToneDesc_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Tone_Desc_List->pdata[ToneDesc_index]);
                pVoiceProfile_Tone_Desc_List->pdata[ToneDesc_index] = NULL;
                pVoiceProfile_Tone_Desc_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Tone_Desc_List->pdata[ToneDesc_index] = (DML_TONE_DESCRIPTION_CTRL_T*) AnscAllocateMemory(sizeof(DML_TONE_DESCRIPTION_CTRL_T));

            //set default
            if(pVoiceProfile_Tone_Desc_List->pdata[ToneDesc_index] != NULL)
            {
                DML_TONE_DESCRIPTION_CTRL_T* pVoiceProfile_Tone_Desc_Data = pVoiceProfile_Tone_Desc_List->pdata[ToneDesc_index];
                pVoiceProfile_Tone_Desc_Data->updated = false;
                pVoiceProfile_Tone_Desc = &(pVoiceProfile_Tone_Desc_Data->dml);
                if ( !pVoiceProfile_Tone_Desc )
                {
                    memset( pVoiceProfile_Tone_Desc, 0, sizeof( TELCOVOICEMGR_DML_TONE_DESCRIPTION ) );
                    pVoiceProfile_Tone_Desc->InstanceNumber = ToneDesc_index + 1;
                }
                pVoiceProfile_Tone_Desc_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileTonePattern(DML_TONE_PATTERN_LIST_T* pVoiceProfile_Tone_Pattern_List, int TonePattern_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_TONE_PATTERN       pVoiceProfile_Tone_Pattern    = NULL;

    if(pVoiceProfile_Tone_Pattern_List != NULL)
    {
        if(TonePattern_index < TELCOVOICEMGR_DATA_MAX_DML_TONE_PATTERN)
        {
            //delete old
            if(pVoiceProfile_Tone_Pattern_List->pdata[TonePattern_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Tone_Pattern_List->pdata[TonePattern_index]);
                pVoiceProfile_Tone_Pattern_List->pdata[TonePattern_index] = NULL;
                pVoiceProfile_Tone_Pattern_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Tone_Pattern_List->pdata[TonePattern_index] = (DML_TONE_PATTERN_CTRL_T*) AnscAllocateMemory(sizeof(DML_TONE_PATTERN_CTRL_T));

            //set default
            if(pVoiceProfile_Tone_Pattern_List->pdata[TonePattern_index] != NULL)
            {
                DML_TONE_PATTERN_CTRL_T* pVoiceProfile_Tone_Pattern_Data = pVoiceProfile_Tone_Pattern_List->pdata[TonePattern_index];
                pVoiceProfile_Tone_Pattern_Data->updated = false;
                pVoiceProfile_Tone_Pattern = &(pVoiceProfile_Tone_Pattern_Data->dml);
                if ( !pVoiceProfile_Tone_Pattern )
                {
                    memset( pVoiceProfile_Tone_Pattern, 0, sizeof( TELCOVOICEMGR_DML_TONE_PATTERN ) );
                    pVoiceProfile_Tone_Pattern->InstanceNumber = TonePattern_index + 1;
                }
                pVoiceProfile_Tone_Pattern_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS  TelcoVoiceMgrDmlAddVoiceProfileButtonMapButton(DML_BUTTON_LIST_T* pVoiceProfile_ButtonMap_Button_List, int Button_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_BUTTON       pVoiceProfile_ButtonMap_Button    = NULL;

    if(pVoiceProfile_ButtonMap_Button_List != NULL)
    {
        if(Button_index < TELCOVOICEMGR_DATA_MAX_BUTTON)
        {
            //delete old
            if(pVoiceProfile_ButtonMap_Button_List->pdata[Button_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_ButtonMap_Button_List->pdata[Button_index]);
                pVoiceProfile_ButtonMap_Button_List->pdata[Button_index] = NULL;
                pVoiceProfile_ButtonMap_Button_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_ButtonMap_Button_List->pdata[Button_index] = (DML_BUTTON_CTRL_T*) AnscAllocateMemory(sizeof(DML_BUTTON_CTRL_T));

            //set default
            if(pVoiceProfile_ButtonMap_Button_List->pdata[Button_index] != NULL)
            {
                DML_BUTTON_CTRL_T* pVoiceProfile_ButtonMap_Button_Data = pVoiceProfile_ButtonMap_Button_List->pdata[Button_index];
                pVoiceProfile_ButtonMap_Button_Data->updated = false;
                pVoiceProfile_ButtonMap_Button = &(pVoiceProfile_ButtonMap_Button_Data->dml);
                if ( !pVoiceProfile_ButtonMap_Button )
                {
                    memset( pVoiceProfile_ButtonMap_Button, 0, sizeof( TELCOVOICEMGR_DML_BUTTON ) );
                    pVoiceProfile_ButtonMap_Button->InstanceNumber = Button_index + 1;
                }
                pVoiceProfile_ButtonMap_Button_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLine(DML_LINE_LIST_T* pVoiceProfile_Line_List, int Line_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_LINE       pVoiceProfile_Line    = NULL;

    if(pVoiceProfile_Line_List != NULL)
    {
        if(Line_index < TELCOVOICEMGR_DATA_MAX_LINE)
        {
            //delete old
            if(pVoiceProfile_Line_List->pdata[Line_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Line_List->pdata[Line_index]);
                pVoiceProfile_Line_List->pdata[Line_index] = NULL;
                pVoiceProfile_Line_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Line_List->pdata[Line_index] = (DML_LINE_CTRL_T*) AnscAllocateMemory(sizeof(DML_LINE_CTRL_T));

            //set default
            if(pVoiceProfile_Line_List->pdata[Line_index] != NULL)
            {
                DML_LINE_CTRL_T* pVoiceProfile_Line_Data = pVoiceProfile_Line_List->pdata[Line_index];
                pVoiceProfile_Line_Data->updated = false;
                pVoiceProfile_Line = &(pVoiceProfile_Line_Data->dml);
                if ( !pVoiceProfile_Line )
                {
                    memset( pVoiceProfile_Line, 0, sizeof( TELCOVOICEMGR_DML_LINE ) );
                    pVoiceProfile_Line->InstanceNumber = Line_index + 1;
                }
                pVoiceProfile_Line_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineSIPEvtSubscribe(DML_LINE_SIP_EVENTSUBSCRIBE_LIST_T* pVoiceProfile_Line_SIP_EvtSubscribe_List, int Evtsubscribe_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_LINE_SIP_EVENTSUBSCRIBE       pVoiceProfile_Line_SIP_EventSubscribe    = NULL;

    if(pVoiceProfile_Line_SIP_EvtSubscribe_List != NULL)
    {
        if(Evtsubscribe_index < TELCOVOICEMGR_DATA_MAX_DML_LINE_SIP_EVENTSUBSCRIBE)
        {
            //delete old
            if(pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[Evtsubscribe_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[Evtsubscribe_index]);
                pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[Evtsubscribe_index] = NULL;
                pVoiceProfile_Line_SIP_EvtSubscribe_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[Evtsubscribe_index] = (DML_LINE_SIP_EVENTSUBSCRIBE_CTRL_T*) AnscAllocateMemory(sizeof(DML_LINE_SIP_EVENTSUBSCRIBE_CTRL_T));

            //set default
            if(pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[Evtsubscribe_index] != NULL)
            {
                DML_LINE_SIP_EVENTSUBSCRIBE_CTRL_T* pVoiceProfile_Line_SIP_EvtSubscribe_Data = pVoiceProfile_Line_SIP_EvtSubscribe_List->pdata[Evtsubscribe_index];
                pVoiceProfile_Line_SIP_EvtSubscribe_Data->updated = false;
                pVoiceProfile_Line_SIP_EventSubscribe = &(pVoiceProfile_Line_SIP_EvtSubscribe_Data->dml);
                if ( !pVoiceProfile_Line_SIP_EventSubscribe )
                {
                    memset( pVoiceProfile_Line_SIP_EventSubscribe, 0, sizeof( TELCOVOICEMGR_DML_LINE_SIP_EVENTSUBSCRIBE ) );
                    pVoiceProfile_Line_SIP_EventSubscribe->InstanceNumber = Evtsubscribe_index + 1;
                }
                pVoiceProfile_Line_SIP_EvtSubscribe_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineRingerEvent(DML_LINE_RINGER_EVENT_LIST_T* pVoiceProfile_Line_Ringer_Event_List, int Event_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_LINE_RINGER_EVENT       pVoiceProfile_Line_Ringer_Event    = NULL;

    if(pVoiceProfile_Line_Ringer_Event_List != NULL)
    {
        if(Event_index < TELCOVOICEMGR_DATA_MAX_DML_LINE_RINGER_EVENT)
        {
            //delete old
            if(pVoiceProfile_Line_Ringer_Event_List->pdata[Event_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Line_Ringer_Event_List->pdata[Event_index]);
                pVoiceProfile_Line_Ringer_Event_List->pdata[Event_index] = NULL;
                pVoiceProfile_Line_Ringer_Event_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Line_Ringer_Event_List->pdata[Event_index] = (DML_LINE_RINGER_PATTERN_CTRL_T*) AnscAllocateMemory(sizeof(DML_LINE_RINGER_PATTERN_CTRL_T));

            //set default
            if(pVoiceProfile_Line_Ringer_Event_List->pdata[Event_index] != NULL)
            {
                DML_LINE_RINGER_PATTERN_CTRL_T* pVoiceProfile_Line_Ringer_Event_Data = pVoiceProfile_Line_Ringer_Event_List->pdata[Event_index];
                pVoiceProfile_Line_Ringer_Event_Data->updated = false;
                pVoiceProfile_Line_Ringer_Event = &(pVoiceProfile_Line_Ringer_Event_Data->dml);
                if ( !pVoiceProfile_Line_Ringer_Event )
                {
                    memset( pVoiceProfile_Line_Ringer_Event, 0, sizeof( TELCOVOICEMGR_DML_LINE_RINGER_EVENT ) );
                    pVoiceProfile_Line_Ringer_Event->InstanceNumber = Event_index + 1;
                }
                pVoiceProfile_Line_Ringer_Event_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineRingerDesc(DML_LINE_RINGER_DESCRIPTION_LIST_T* pVoiceProfile_Line_Ringer_Desc_List, int RingerDesc_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_LINE_RINGER_DESCRIPTION       pVoiceProfile_Line_Ringer_Desc    = NULL;

    if(pVoiceProfile_Line_Ringer_Desc_List != NULL)
    {
        if(RingerDesc_index < TELCOVOICEMGR_DATA_MAX_DML_LINE_RINGER_DESCRIPTION)
        {
            //delete old
            if(pVoiceProfile_Line_Ringer_Desc_List->pdata[RingerDesc_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Line_Ringer_Desc_List->pdata[RingerDesc_index]);
                pVoiceProfile_Line_Ringer_Desc_List->pdata[RingerDesc_index] = NULL;
                pVoiceProfile_Line_Ringer_Desc_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Line_Ringer_Desc_List->pdata[RingerDesc_index] = (DML_LINE_RINGER_DESCRIPTION_CTRL_T*) AnscAllocateMemory(sizeof(DML_LINE_RINGER_DESCRIPTION_CTRL_T));

            //set default
            if(pVoiceProfile_Line_Ringer_Desc_List->pdata[RingerDesc_index] != NULL)
            {
                DML_LINE_RINGER_DESCRIPTION_CTRL_T* pVoiceProfile_Line_Ringer_Desc_Data = pVoiceProfile_Line_Ringer_Desc_List->pdata[RingerDesc_index];
                pVoiceProfile_Line_Ringer_Desc_Data->updated = false;
                pVoiceProfile_Line_Ringer_Desc = &(pVoiceProfile_Line_Ringer_Desc_Data->dml);
                if ( !pVoiceProfile_Line_Ringer_Desc )
                {
                    memset( pVoiceProfile_Line_Ringer_Desc, 0, sizeof( TELCOVOICEMGR_DML_LINE_RINGER_DESCRIPTION ) );
                    pVoiceProfile_Line_Ringer_Desc->InstanceNumber = RingerDesc_index + 1;
                }
                pVoiceProfile_Line_Ringer_Desc_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}


ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineRingerPattern(DML_LINE_RINGER_PATTERN_LIST_T* pVoiceProfile_Line_Ringer_Pattern_List, int RingerPattern_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_LINE_RINGER_PATTERN       pVoiceProfile_Line_Ringer_Pattern    = NULL;

    if(pVoiceProfile_Line_Ringer_Pattern_List != NULL)
    {
        if(RingerPattern_index < TELCOVOICEMGR_DATA_MAX_DML_LINE_RINGER_PATTERN)
        {
            //delete old
            if(pVoiceProfile_Line_Ringer_Pattern_List->pdata[RingerPattern_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Line_Ringer_Pattern_List->pdata[RingerPattern_index]);
                pVoiceProfile_Line_Ringer_Pattern_List->pdata[RingerPattern_index] = NULL;
                pVoiceProfile_Line_Ringer_Pattern_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Line_Ringer_Pattern_List->pdata[RingerPattern_index] = (DML_LINE_RINGER_PATTERN_CTRL_T*) AnscAllocateMemory(sizeof(DML_LINE_RINGER_PATTERN_CTRL_T));

            //set default
            if(pVoiceProfile_Line_Ringer_Pattern_List->pdata[RingerPattern_index] != NULL)
            {
                DML_LINE_RINGER_PATTERN_CTRL_T* pVoiceProfile_Line_Ringer_Pattern_Data = pVoiceProfile_Line_Ringer_Pattern_List->pdata[RingerPattern_index];
                pVoiceProfile_Line_Ringer_Pattern_Data->updated = false;
                pVoiceProfile_Line_Ringer_Pattern = &(pVoiceProfile_Line_Ringer_Pattern_Data->dml);
                if ( !pVoiceProfile_Line_Ringer_Pattern )
                {
                    memset( pVoiceProfile_Line_Ringer_Pattern, 0, sizeof( TELCOVOICEMGR_DML_LINE_RINGER_PATTERN ) );
                    pVoiceProfile_Line_Ringer_Pattern->InstanceNumber = RingerPattern_index + 1;
                }
                pVoiceProfile_Line_Ringer_Pattern_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}


ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineCodecList(DML_LINE_CODECLIST_LIST_T* pVoiceProfile_Line_CodecList_List, int CodecList_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_LINE_CODECLIST       pVoiceProfile_Line_Codec_List    = NULL;

    if(pVoiceProfile_Line_CodecList_List != NULL)
    {
        if(CodecList_index < TELCOVOICEMGR_DATA_MAX_DML_LINE_CODECLIST)
        {
            //delete old
            if(pVoiceProfile_Line_CodecList_List->pdata[CodecList_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Line_CodecList_List->pdata[CodecList_index]);
                pVoiceProfile_Line_CodecList_List->pdata[CodecList_index] = NULL;
                pVoiceProfile_Line_CodecList_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Line_CodecList_List->pdata[CodecList_index] = (DML_LINE_CODECLIST_CTRL_T*) AnscAllocateMemory(sizeof(DML_LINE_CODECLIST_CTRL_T));

            //set default
            if(pVoiceProfile_Line_CodecList_List->pdata[CodecList_index] != NULL)
            {
                DML_LINE_CODECLIST_CTRL_T* pVoiceProfile_Line_CodecList_Data = pVoiceProfile_Line_CodecList_List->pdata[CodecList_index];
                pVoiceProfile_Line_CodecList_Data->updated = false;
                pVoiceProfile_Line_Codec_List = &(pVoiceProfile_Line_CodecList_Data->dml);
                if ( !pVoiceProfile_Line_Codec_List )
                {
                    memset( pVoiceProfile_Line_Codec_List, 0, sizeof( TELCOVOICEMGR_DML_LINE_CODECLIST ) );
                    pVoiceProfile_Line_Codec_List->InstanceNumber = CodecList_index + 1;
                }
                pVoiceProfile_Line_CodecList_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceProfileLineSession(DML_LINE_SESSION_LIST_T* pVoiceProfile_Line_Session_List, int Session_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_LINE_SESSION      pVoiceProfile_Line_Session    = NULL;

    if(pVoiceProfile_Line_Session_List != NULL)
    {
        if(Session_index < TELCOVOICEMGR_DATA_MAX_DML_LINE_SESSION)
        {
            //delete old
            if(pVoiceProfile_Line_Session_List->pdata[Session_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceProfile_Line_Session_List->pdata[Session_index]);
                pVoiceProfile_Line_Session_List->pdata[Session_index] = NULL;
                pVoiceProfile_Line_Session_List->ulQuantity--;
            }

            //create new
            pVoiceProfile_Line_Session_List->pdata[Session_index] = (DML_LINE_SESSION_CTRL_T*) AnscAllocateMemory(sizeof(DML_LINE_SESSION_CTRL_T));

            //set default
            if(pVoiceProfile_Line_Session_List->pdata[Session_index] != NULL)
            {
                DML_LINE_SESSION_CTRL_T* pVoiceProfile_Line_Session_Data = pVoiceProfile_Line_Session_List->pdata[Session_index];
                pVoiceProfile_Line_Session_Data->updated = false;
                pVoiceProfile_Line_Session = &(pVoiceProfile_Line_Session_Data->dml);
                if ( !pVoiceProfile_Line_Session )
                {
                    memset( pVoiceProfile_Line_Session, 0, sizeof( TELCOVOICEMGR_DML_LINE_SESSION ) );
                    pVoiceProfile_Line_Session->InstanceNumber = Session_index + 1;
                }
                pVoiceProfile_Line_Session_List->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddCapabilitiesCodec(DML_CAPABILITIES_CODECS_LIST_T* pCapabilitiesCodecList, int codec_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_CAPABILITIES_CODECS      pCapabilities_Codecs    = NULL;

    if(pCapabilitiesCodecList != NULL)
    {
        if(codec_index < TELCOVOICEMGR_DATA_MAX_VOICE_CAPABILITIES_CODECS)
        {
            //delete old
            if(pCapabilitiesCodecList->pdata[codec_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCapabilitiesCodecList->pdata[codec_index]);
                pCapabilitiesCodecList->pdata[codec_index] = NULL;
                pCapabilitiesCodecList->ulQuantity--;
            }

            //create new
            pCapabilitiesCodecList->pdata[codec_index] = (DML_CAPABILITIES_CODECS_CTRL_T*) AnscAllocateMemory(sizeof(DML_CAPABILITIES_CODECS_CTRL_T));

            //set default
            if(pCapabilitiesCodecList->pdata[codec_index] != NULL)
            {
                DML_CAPABILITIES_CODECS_CTRL_T* pCapabilitiesCodecData = pCapabilitiesCodecList->pdata[codec_index];
                pCapabilitiesCodecData->updated = false;
                pCapabilities_Codecs = &(pCapabilitiesCodecData->dml);
                if ( !pCapabilities_Codecs )
                {
                    memset( pCapabilities_Codecs, 0, sizeof( TELCOVOICEMGR_DML_CAPABILITIES_CODECS ) );
                    pCapabilities_Codecs->InstanceNumber = codec_index + 1;
                }
                pCapabilitiesCodecList->ulQuantity++;
                result = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }

    return result;
}
