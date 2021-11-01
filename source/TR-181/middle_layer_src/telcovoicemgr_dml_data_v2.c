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

#include "telcovoicemgr_services_apis_v2.h"

ANSC_STATUS TelcoVoiceMgrDmlDataDelete(void)
{
    ANSC_STATUS                         result               = ANSC_STATUS_FAILURE;
    TELCOVOICEMGR_DML_DATA*             pTelcoVoiceMgrData   = NULL;
    ULONG                               uVsIndex             = 0;
    ULONG                               uIndex               = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE     pDmlVoiceService     = NULL;
    PDML_SIP                            pDmlSipObj           = NULL;
    PDML_CALLCONTROL                    pDmlCallControlObj   = NULL;
    PDML_VOIPPROFILE                    pDmlVoipProfile      = NULL;
    PDML_CALLCONTROL_LINE               pDmlCallControlLine  = NULL;


    pTelcoVoiceMgrData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrData != NULL)
    {
        for( uVsIndex = 0; uVsIndex < pTelcoVoiceMgrData->Service.VoiceService.ulQuantity; uVsIndex++)
        {
            if(pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex] != NULL)
            {
                DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex];
                pDmlVoiceService = &(pVoiceService->dml);
                if ( pDmlVoiceService )
                {
                    pDmlSipObj = &(pDmlVoiceService->SIP_obj);

                    if( pDmlSipObj )
                    {
                        //Delete Network Table
                        for (uIndex = 0; uIndex < pDmlSipObj->Network.ulQuantity; uIndex++)
                        {
                            if(pDmlSipObj->Network.pdata[uIndex] != NULL)
                            {
                                AnscFreeMemory((ANSC_HANDLE)pDmlSipObj->Network.pdata[uIndex]);
                                pDmlSipObj->Network.pdata[uIndex] = NULL;
                            }
                        }
                        //Delete Client Table
                        for (uIndex = 0; uIndex < pDmlSipObj->Client.ulQuantity; uIndex++)
                        {
                            if(pDmlSipObj->Client.pdata[uIndex] != NULL)
                            {
                                AnscFreeMemory((ANSC_HANDLE)pDmlSipObj->Client.pdata[uIndex]);
                                pDmlSipObj->Client.pdata[uIndex] = NULL;
                            }
                        }
                    }
                    //Delete VoIP Profile Table
                    for (uIndex = 0; uIndex < pDmlVoiceService->VoIPProfile->ulQuantity; uIndex++)
                    {
                        if(pDmlVoiceService->VoIPProfile->pdata[uIndex] != NULL)
                        {
                            AnscFreeMemory((ANSC_HANDLE)pDmlVoiceService->VoIPProfile->pdata[uIndex]);
                            pDmlVoiceService->VoIPProfile->pdata[uIndex] = NULL;
                        }
                    }
                    //Delete Line Table
                    pDmlCallControlObj = &(pDmlVoiceService->CallControl_obj);
                    if(pDmlCallControlObj)
                    {
                        for (uIndex = 0; uIndex < pDmlCallControlObj->Line.ulQuantity; uIndex++)
                        {
                            if(pDmlCallControlObj->Line.pdata[uIndex] != NULL)
                            {
                                AnscFreeMemory((ANSC_HANDLE)pDmlCallControlObj->Line.pdata[uIndex]);
                                pDmlCallControlObj->Line.pdata[uIndex] = NULL;
                            }
                        }
                    }
                    //Delete Voice Service
                    AnscFreeMemory((ANSC_HANDLE)pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex]);
                    pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex] = NULL;
                }
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
    ANSC_STATUS                       returnStatus        = ANSC_STATUS_SUCCESS;
    ULONG                             uVsIndex            = 0;
    ULONG                             uIndex              = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE   pDmlVoiceService    = NULL;
    PDML_SIP                          pDmlSipObj          = NULL;
    PDML_CALLCONTROL                  pDmlCallControlObj  = NULL;
    PDML_SIP_NETWORK                  pDmlSipNetwork      = NULL;
    PDML_SIP_CLIENT                   pDmlSipClient       = NULL;
    PDML_VOIPPROFILE                  pDmlVoipProfile     = NULL;
    PDML_CALLCONTROL_LINE             pDmlCallControlLine = NULL;
    PDML_VOICESERVICE_TERMINAL        pDmlTerminal        = NULL;

    if(pTelcoVoiceMgrData != NULL)
    {
        pTelcoVoiceMgrData->Service.VoiceService.ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES;
        for( uVsIndex = 0; uVsIndex < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES; uVsIndex++)
        {
            pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex] = (DML_VOICE_SERVICE_CTRL_T*) AnscAllocateMemory(sizeof(DML_VOICE_SERVICE_CTRL_T));
            DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uVsIndex];
            pDmlVoiceService = &(pVoiceService->dml);

            if ( pDmlVoiceService == NULL )
            {
                returnStatus = ANSC_STATUS_RESOURCES;
                CcspTraceError(("%s - Failed pDmlVoiceService : NULL\n", __FUNCTION__));
                goto EXIT;
            }
            pDmlVoiceService->InstanceNumber = uVsIndex + 1;
            pDmlSipObj = &(pDmlVoiceService->SIP_obj);

            if( pDmlSipObj )
            {
                pDmlSipObj->Network.ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_SIP_NETWORK;

                if ( pDmlSipObj->Network.ulQuantity != 0 )
                {
                    for (uIndex = 0; uIndex < TELCOVOICEMGR_DML_NUMBER_OF_SIP_NETWORK; uIndex++)
                    {
                        pDmlSipObj->Network.pdata[uIndex] = (DML_SIP_NETWORK_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_NETWORK_CTRL_T));
                        DML_SIP_NETWORK_CTRL_T* pSipNetwork = pDmlSipObj->Network.pdata[uIndex];
                        pDmlSipNetwork = &(pSipNetwork->dml);

                        if ( pDmlSipNetwork == NULL )
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;
                            CcspTraceError(("%s - Failed pDmlSipNetwork : NULL\n", __FUNCTION__));
                            goto EXIT;
                        }
                        pDmlSipNetwork->uInstanceNumber = uIndex + 1;
                        pDmlSipNetwork->pParentVoiceService = pDmlVoiceService;
                    }
                }

                pDmlSipObj->Client.ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_SIP_CLIENTS;

                if ( pDmlSipObj->Client.ulQuantity != 0 )
                {
                    for (uIndex = 0; uIndex < TELCOVOICEMGR_DML_NUMBER_OF_SIP_CLIENTS; uIndex++)
                    {
                        pDmlSipObj->Client.pdata[uIndex] = (DML_SIP_CLIENT_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_CLIENT_CTRL_T));
                        DML_SIP_CLIENT_CTRL_T* pSipClient = pDmlSipObj->Client.pdata[uIndex];
                        pDmlSipClient = &(pSipClient->dml);

                        if ( pDmlSipClient == NULL )
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;
                            CcspTraceError(("%s - Failed pDmlSipClient : NULL\n", __FUNCTION__));
                            goto EXIT;
                        }
                        pDmlSipClient->uInstanceNumber = uIndex + 1;
                        pDmlSipClient->pParentVoiceService = pDmlVoiceService;
                    }
                }
            }
            pDmlVoiceService->VoIPProfile = (DML_VOIPPROFILE_LIST_T*) AnscAllocateMemory(sizeof(DML_VOIPPROFILE_LIST_T));
            pDmlVoiceService->VoIPProfile->ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_VOIP_PROFILE;

            if ( pDmlVoiceService->VoIPProfile->ulQuantity != 0 )
            {
                for (uIndex = 0; uIndex < TELCOVOICEMGR_DML_NUMBER_OF_VOIP_PROFILE; uIndex++)
                {
                    pDmlVoiceService->VoIPProfile->pdata[uIndex] = (DML_VOIPPROFILE_CTRL_T*) AnscAllocateMemory(sizeof(DML_VOIPPROFILE_CTRL_T));
                    DML_VOIPPROFILE_CTRL_T* pVoipProfile = pDmlVoiceService->VoIPProfile->pdata[uIndex];
                    pDmlVoipProfile = &(pVoipProfile->dml);

                    if ( !pDmlVoipProfile )
                    {
                        returnStatus = ANSC_STATUS_RESOURCES;
                        CcspTraceError(("%s - Failed pDmlVoipProfile : NULL\n", __FUNCTION__));
                        goto EXIT;
                    }
                    pDmlVoipProfile->uInstanceNumber = uIndex + 1;
                    pDmlVoipProfile->pParentVoiceService = pDmlVoiceService;
                    pDmlVoipProfile->Enable = 1;
                }
            }

            pDmlVoiceService->Terminal = (DML_VOICESERVICE_TERMINAL_LIST_T*) AnscAllocateMemory(sizeof(DML_VOICESERVICE_TERMINAL_LIST_T));
            pDmlVoiceService->Terminal->ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_TERMINALS;

            if ( pDmlVoiceService->Terminal->ulQuantity != 0 )
            {
                for (uIndex = 0; uIndex < TELCOVOICEMGR_DML_NUMBER_OF_TERMINALS; uIndex++)
                {
                    pDmlVoiceService->Terminal->pdata[uIndex] = (DML_VOICESERVICE_TERMINAL_LIST_T*) AnscAllocateMemory(sizeof(DML_VOICESERVICE_TERMINAL_LIST_T));
                    DML_VOICESERVICE_TERMINAL_CTRL_T* pTerminal = pDmlVoiceService->Terminal->pdata[uIndex];
                    pDmlTerminal = &(pTerminal->dml);

                    if ( !pDmlTerminal )
                    {
                        returnStatus = ANSC_STATUS_RESOURCES;
                        CcspTraceError(("%s - Failed pDmlTerminal : NULL\n", __FUNCTION__));
                        goto EXIT;
                    }
                    pDmlTerminal->uInstanceNumber = uIndex + 1;
                    pDmlTerminal->pParentVoiceService = pDmlVoiceService;
                    pDmlTerminal->Enable = 1;
                    AnscCopyString(pDmlTerminal->Alias,"VoiceTerminal");
                    pDmlTerminal->Audio.ulQuantity = 0;
                    PDML_TERMINAL_BUTTONMAP pDmlButtonMap = &(pDmlTerminal->ButtonMap);
                    pDmlButtonMap->Button.ulQuantity = 0;
                    PDML_TERMINAL_RINGER pDmlRinger= &(pDmlTerminal->Ringer);
                    pDmlRinger->Pattern.ulQuantity = 0;
                    pDmlRinger->Description.ulQuantity = 0;
                }
            }
            pDmlCallControlObj = &(pDmlVoiceService->CallControl_obj);
            if(pDmlCallControlObj)
            {
                pDmlCallControlObj->Line.ulQuantity = TELCOVOICEMGR_DML_NUMBER_OF_CALLCONTROL_LINES;
                if ( pDmlCallControlObj->Line.ulQuantity != 0 )
                {
                    for (uIndex = 0; uIndex < TELCOVOICEMGR_DML_NUMBER_OF_CALLCONTROL_LINES; uIndex++)
                    {
                        pDmlCallControlObj->Line.pdata[uIndex] = (DML_CALLCONTROL_LINE_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_LINE_CTRL_T));
                        DML_CALLCONTROL_LINE_CTRL_T* pCallControlLine = pDmlCallControlObj->Line.pdata[uIndex];
                        pDmlCallControlLine = &(pCallControlLine->dml);

                        if ( !pDmlCallControlLine )
                        {
                            returnStatus = ANSC_STATUS_RESOURCES;
                            CcspTraceError(("%s - Failed pDmlCallControlLine : NULL\n", __FUNCTION__));
                            goto EXIT;
                        }
                        pDmlCallControlLine->uInstanceNumber = uIndex + 1;
                        pDmlCallControlLine->pParentVoiceService = pDmlVoiceService;
                    }
                }
            }
        }
    }
    else
    {
        returnStatus = ANSC_STATUS_FAILURE;
        CcspTraceError(("%s - Failed pTelcoVoiceMgrData : NULL\n", __FUNCTION__));
    }
EXIT:
    return returnStatus;
}

ANSC_STATUS TelcoVoiceMgrDmlAddVoiceService(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_VOICESERVICE    pDmlVoiceService    = NULL;

    if(pVoiceServiceList != NULL)
    {
        if(index < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES)
        {
            //delete old
            if(pVoiceServiceList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoiceServiceList->pdata[index]);
                pVoiceServiceList->pdata[index] = NULL;
                pVoiceServiceList->ulQuantity--;
            }

            //create new
            pVoiceServiceList->pdata[index] = (DML_VOICE_SERVICE_CTRL_T*) AnscAllocateMemory(sizeof(DML_VOICE_SERVICE_CTRL_T));

            //set default
            if(pVoiceServiceList->pdata[index] != NULL)
            {
                PDML_VOICE_SERVICE_CTRL_T pVoiceServiceData = pVoiceServiceList->pdata[index];
                pVoiceServiceData->updated = false;
                pDmlVoiceService = &(pVoiceServiceData->dml);
                if ( pDmlVoiceService == NULL )
                {
                    memset( pDmlVoiceService, 0, sizeof( TELCOVOICEMGR_DML_VOICESERVICE ) );
                    pDmlVoiceService->InstanceNumber = index + 1;
                }
                pVoiceServiceList->ulQuantity++;
                result  = ANSC_STATUS_SUCCESS;
            }
            else
            {
                CcspTraceError(("%s - Failed \n", __FUNCTION__));
            }
        }
    }
    return result;

}

ANSC_STATUS TelcoVoiceMgrDmlAddCapabilitiesCodec(PDML_VOICESERVICE_CAPABILITIES_CODEC_LIST_T pCapabilitiesCodecList, int codec_index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_VOICESERVICE_CAPABILITIES_CODEC   pCapabilities_Codecs    = NULL;

    if(pCapabilitiesCodecList != NULL)
    {
        if(codec_index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCapabilitiesCodecList->pdata[codec_index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCapabilitiesCodecList->pdata[codec_index]);
                pCapabilitiesCodecList->pdata[codec_index] = NULL;
                pCapabilitiesCodecList->ulQuantity--;
            }

            //create new
            pCapabilitiesCodecList->pdata[codec_index] = (DML_VOICESERVICE_CAPABILITIES_CODEC_CTRL_T*) AnscAllocateMemory(sizeof(DML_VOICESERVICE_CAPABILITIES_CODEC_CTRL_T));

            //set default
            if(pCapabilitiesCodecList->pdata[codec_index] != NULL)
            {
                DML_VOICESERVICE_CAPABILITIES_CODEC_CTRL_T* pCapabilitiesCodecData = pCapabilitiesCodecList->pdata[codec_index];
                pCapabilitiesCodecData->updated = false;
                pCapabilities_Codecs = &(pCapabilitiesCodecData->dml);
                if ( !pCapabilities_Codecs )
                {
                    memset( pCapabilities_Codecs, 0, sizeof( DML_VOICESERVICE_CAPABILITIES_CODEC ) );
                    pCapabilities_Codecs->uInstanceNumber = codec_index + 1;
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

ANSC_STATUS TelcoVoiceMgrDmlAddIsdnBRI(PDML_ISDN_BRI_LIST_T pIsdnBRIList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_ISDN_BRI   pDmlIsdnBRI    = NULL;

    if(pIsdnBRIList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pIsdnBRIList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pIsdnBRIList->pdata[index]);
                pIsdnBRIList->pdata[index] = NULL;
                pIsdnBRIList->ulQuantity--;
            }

            //create new
            pIsdnBRIList->pdata[index] = (DML_ISDN_BRI_CTRL_T*) AnscAllocateMemory(sizeof(DML_ISDN_BRI_CTRL_T));

            //set default
            if(pIsdnBRIList->pdata[index] != NULL)
            {
                DML_ISDN_BRI_CTRL_T* pDmlIsdnBRIData = pIsdnBRIList->pdata[index];
                pDmlIsdnBRIData->updated = false;
                pDmlIsdnBRI = &(pDmlIsdnBRIData->dml);
                if ( !pDmlIsdnBRI )
                {
                    memset( pDmlIsdnBRI, 0, sizeof( DML_ISDN_BRI ) );
                    pDmlIsdnBRI->uInstanceNumber = index + 1;
                }
                pIsdnBRIList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddIsdnPRI(PDML_ISDN_PRI_LIST_T pIsdnPRIList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_ISDN_PRI   pDmlIsdnPRI    = NULL;

    if(pIsdnPRIList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pIsdnPRIList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pIsdnPRIList->pdata[index]);
                pIsdnPRIList->pdata[index] = NULL;
                pIsdnPRIList->ulQuantity--;
            }

            //create new
            pIsdnPRIList->pdata[index] = (DML_ISDN_PRI_CTRL_T*) AnscAllocateMemory(sizeof(DML_ISDN_PRI_CTRL_T));

            //set default
            if(pIsdnPRIList->pdata[index] != NULL)
            {
                DML_ISDN_PRI_CTRL_T* pDmlIsdnPRIData = pIsdnPRIList->pdata[index];
                pDmlIsdnPRIData->updated = false;
                pDmlIsdnPRI = &(pDmlIsdnPRIData->dml);
                if ( !pDmlIsdnPRI )
                {
                    memset( pDmlIsdnPRI, 0, sizeof( DML_ISDN_PRI ) );
                    pDmlIsdnPRI->uInstanceNumber = index + 1;
                }
                pIsdnPRIList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddPotsFXO(PDML_POTS_FXO_LIST_T pPotsFXOList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_POTS_FXO   pDmlPotsFXO    = NULL;

    if(pPotsFXOList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pPotsFXOList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pPotsFXOList->pdata[index]);
                pPotsFXOList->pdata[index] = NULL;
                pPotsFXOList->ulQuantity--;
            }

            //create new
            pPotsFXOList->pdata[index] = (DML_POTS_FXO_CTRL_T*) AnscAllocateMemory(sizeof(DML_POTS_FXO_CTRL_T));

            //set default
            if(pPotsFXOList->pdata[index] != NULL)
            {
                DML_POTS_FXO_CTRL_T* pDmlPotsFXOData = pPotsFXOList->pdata[index];
                pDmlPotsFXOData->updated = false;
                pDmlPotsFXO = &(pDmlPotsFXOData->dml);
                if ( !pDmlPotsFXO )
                {
                    memset( pDmlPotsFXO, 0, sizeof( DML_POTS_FXO ) );
                    pDmlPotsFXO->uInstanceNumber = index + 1;
                }
                pPotsFXOList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddPotsFXS(PDML_POTS_FXS_LIST_T pPotsFXSList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_POTS_FXS   pDmlPotsFXS    = NULL;

    if(pPotsFXSList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pPotsFXSList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pPotsFXSList->pdata[index]);
                pPotsFXSList->pdata[index] = NULL;
                pPotsFXSList->ulQuantity--;
            }

            //create new
            pPotsFXSList->pdata[index] = (DML_POTS_FXS_CTRL_T*) AnscAllocateMemory(sizeof(DML_POTS_FXS_CTRL_T));

            //set default
            if(pPotsFXSList->pdata[index] != NULL)
            {
                DML_POTS_FXS_CTRL_T* pDmlPotsFXSData = pPotsFXSList->pdata[index];
                pDmlPotsFXSData->updated = false;
                pDmlPotsFXS = &(pDmlPotsFXSData->dml);
                if ( !pDmlPotsFXS )
                {
                    memset( pDmlPotsFXS, 0, sizeof( DML_POTS_FXS ) );
                    pDmlPotsFXS->uInstanceNumber = index + 1;
                }
                pPotsFXSList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddPotsRingerEvt(PDML_POTS_RINGER_EVT_LIST_T pPotsRingerEvtList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_POTS_RINGER_EVT  pDmlPotsRingerEvt    = NULL;

    if(pPotsRingerEvtList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pPotsRingerEvtList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pPotsRingerEvtList->pdata[index]);
                pPotsRingerEvtList->pdata[index] = NULL;
                pPotsRingerEvtList->ulQuantity--;
            }

            //create new
            pPotsRingerEvtList->pdata[index] = (DML_POTS_RINGER_EVT_CTRL_T*) AnscAllocateMemory(sizeof(DML_POTS_RINGER_EVT_CTRL_T));

            //set default
            if(pPotsRingerEvtList->pdata[index] != NULL)
            {
                DML_POTS_RINGER_EVT_CTRL_T* pDmlPotsRingerEvtData = pPotsRingerEvtList->pdata[index];
                pDmlPotsRingerEvtData->updated = false;
                pDmlPotsRingerEvt = &(pDmlPotsRingerEvtData->dml);
                if ( !pDmlPotsRingerEvt )
                {
                    memset( pDmlPotsRingerEvt, 0, sizeof( DML_POTS_RINGER_EVT ) );
                    pDmlPotsRingerEvt->uInstanceNumber = index + 1;
                }
                pPotsRingerEvtList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddDectBase(PDML_DECT_BASE_LIST_T pDectBaseList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_DECT_BASE   pDmlDectBase   = NULL;

    if(pDectBaseList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pDectBaseList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pDectBaseList->pdata[index]);
                pDectBaseList->pdata[index] = NULL;
                pDectBaseList->ulQuantity--;
            }

            //create new
            pDectBaseList->pdata[index] = (DML_DECT_BASE_CTRL_T*) AnscAllocateMemory(sizeof(DML_DECT_BASE_CTRL_T));

            //set default
            if(pDectBaseList->pdata[index] != NULL)
            {
                DML_DECT_BASE_CTRL_T* pDmlDectBaseData = pDectBaseList->pdata[index];
                pDmlDectBaseData->updated = false;
                pDmlDectBase = &(pDmlDectBaseData->dml);
                if ( !pDmlDectBase )
                {
                    memset( pDmlDectBase, 0, sizeof( DML_DECT_BASE ) );
                    pDmlDectBase->uInstanceNumber = index + 1;
                }
                pDectBaseList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddDectPortable(PDML_DECT_PORTABLE_LIST_T pDectPortableList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_DECT_PORTABLE   pDmlDectPortable    = NULL;

    if(pDectPortableList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pDectPortableList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pDectPortableList->pdata[index]);
                pDectPortableList->pdata[index] = NULL;
                pDectPortableList->ulQuantity--;
            }

            //create new
            pDectPortableList->pdata[index] = (DML_DECT_PORTABLE_CTRL_T*) AnscAllocateMemory(sizeof(DML_DECT_PORTABLE_CTRL_T));

            //set default
            if(pDectPortableList->pdata[index] != NULL)
            {
                DML_DECT_PORTABLE_CTRL_T* pDmlDectPortableData = pDectPortableList->pdata[index];
                pDmlDectPortableData->updated = false;
                pDmlDectPortable = &(pDmlDectPortableData->dml);
                if ( !pDmlDectPortable )
                {
                    memset( pDmlDectPortable, 0, sizeof( DML_DECT_PORTABLE ) );
                    pDmlDectPortable->uInstanceNumber = index + 1;
                }
                pDectPortableList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddSipClient(PDML_SIP_CLIENT_LIST_T pSipCliList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_CLIENT   pDmlSipCli    = NULL;

    if(pSipCliList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipCliList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipCliList->pdata[index]);
                pSipCliList->pdata[index] = NULL;
                pSipCliList->ulQuantity--;
            }

            //create new
            pSipCliList->pdata[index] = (DML_SIP_CLIENT_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_CLIENT_CTRL_T));

            //set default
            if(pSipCliList->pdata[index] != NULL)
            {
                DML_SIP_CLIENT_CTRL_T* pDmlSipCliData = pSipCliList->pdata[index];
                pDmlSipCliData->updated = false;
                pDmlSipCli = &(pDmlSipCliData->dml);
                if ( !pDmlSipCli )
                {
                    memset( pDmlSipCli, 0, sizeof( DML_SIP_CLIENT ) );
                    pDmlSipCli->uInstanceNumber = index + 1;
                }
                pSipCliList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddSipClientContact(PDML_SIP_CLIENT_CONTACT_LIST_T pSipCliContactList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_CLIENT_CONTACT   pDmlSipCliContact    = NULL;

    if(pSipCliContactList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipCliContactList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipCliContactList->pdata[index]);
                pSipCliContactList->pdata[index] = NULL;
                pSipCliContactList->ulQuantity--;
            }

            //create new
            pSipCliContactList->pdata[index] = (DML_SIP_CLIENT_CONTACT_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_CLIENT_CONTACT_CTRL_T));

            //set default
            if(pSipCliContactList->pdata[index] != NULL)
            {
                DML_SIP_CLIENT_CONTACT_CTRL_T* pDmlSipCliContactData = pSipCliContactList->pdata[index];
                pDmlSipCliContactData->updated = false;
                pDmlSipCliContact = &(pDmlSipCliContactData->dml);
                if ( !pDmlSipCliContact )
                {
                    memset( pDmlSipCliContact, 0, sizeof( DML_SIP_CLIENT_CONTACT ) );
                    pDmlSipCliContact->uInstanceNumber = index + 1;
                }
                pSipCliContactList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddSipClientContactEvtSub(PDML_SIP_CLIENT_SIPEVENTSUB_LIST_T pSipCliEvtSubscribeList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_CLIENT_SIPEVENTSUB   pDmlSipCliEvtSubscribe    = NULL;

    if(pSipCliEvtSubscribeList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipCliEvtSubscribeList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipCliEvtSubscribeList->pdata[index]);
                pSipCliEvtSubscribeList->pdata[index] = NULL;
                pSipCliEvtSubscribeList->ulQuantity--;
            }

            //create new
            pSipCliEvtSubscribeList->pdata[index] = (DML_SIP_CLIENT_SIPEVENTSUB_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_CLIENT_SIPEVENTSUB_CTRL_T));

            //set default
            if(pSipCliEvtSubscribeList->pdata[index] != NULL)
            {
                DML_SIP_CLIENT_SIPEVENTSUB_CTRL_T* pDmlSipCliEvtSubscribeData = pSipCliEvtSubscribeList->pdata[index];
                pDmlSipCliEvtSubscribeData->updated = false;
                pDmlSipCliEvtSubscribe = &(pDmlSipCliEvtSubscribeData->dml);
                if ( !pDmlSipCliEvtSubscribe )
                {
                    memset( pDmlSipCliEvtSubscribe, 0, sizeof( DML_SIP_CLIENT_SIPEVENTSUB ) );
                    pDmlSipCliEvtSubscribe->uInstanceNumber = index + 1;
                }
                pSipCliEvtSubscribeList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddSipNetwork(PDML_SIP_NETWORK_LIST_T pSipNetworkList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_NETWORK    pDmlSipNetwork    = NULL;

    if(pSipNetworkList != NULL)
    {
        if(index < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES)
        {
            //delete old
            if(pSipNetworkList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipNetworkList->pdata[index]);
                pSipNetworkList->pdata[index] = NULL;
                pSipNetworkList->ulQuantity--;
            }

            //create new
            pSipNetworkList->pdata[index] = (DML_SIP_NETWORK_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_NETWORK_CTRL_T));

            //set default
            if(pSipNetworkList->pdata[index] != NULL)
            {
                PDML_SIP_NETWORK_CTRL_T pSipNetworkData = pSipNetworkList->pdata[index];
                pSipNetworkData->updated = false;
                pDmlSipNetwork = &(pSipNetworkData->dml);
                if ( pDmlSipNetwork == NULL )
                {
                    memset( pDmlSipNetwork, 0, sizeof( DML_SIP_NETWORK ) );
                    pDmlSipNetwork->uInstanceNumber = index + 1;
                }
                pSipNetworkList->ulQuantity++;
            }
            result  = ANSC_STATUS_SUCCESS;
        }
    }
    return result;
}

ANSC_STATUS TelcoVoiceMgrDmlAddSipNetworkFQDNServer(PDML_SIP_NETWORK_FQDNSERVER_LIST_T pSipNetworkFqdnList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_NETWORK_FQDNSERVER   pDmlSipNetworkFqdn = NULL;

    if(pSipNetworkFqdnList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipNetworkFqdnList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipNetworkFqdnList->pdata[index]);
                pSipNetworkFqdnList->pdata[index] = NULL;
                pSipNetworkFqdnList->ulQuantity--;
            }

            //create new
            pSipNetworkFqdnList->pdata[index] = (DML_SIP_NETWORK_FQDNSERVER_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_NETWORK_FQDNSERVER_CTRL_T));

            //set default
            if(pSipNetworkFqdnList->pdata[index] != NULL)
            {
                DML_SIP_NETWORK_FQDNSERVER_CTRL_T* pDmlSipNetworkFqdnData = pSipNetworkFqdnList->pdata[index];
                pDmlSipNetworkFqdnData->updated = false;
                pDmlSipNetworkFqdn = &(pDmlSipNetworkFqdnData->dml);
                if ( pDmlSipNetworkFqdn == NULL )
                {
                    memset( pDmlSipNetworkFqdn, 0, sizeof( DML_SIP_NETWORK_FQDNSERVER ) );
                    pDmlSipNetworkFqdn->uInstanceNumber = index + 1;
                }
                pSipNetworkFqdnList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddSipNetworkEvtSub(PDML_SIP_NETWORK_EVENTSUBSCRIBE_LIST_T pSipNetworkEvtSubscribeList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_NETWORK_EVENTSUBSCRIBE   pDmlSipNetworkEvtSubscribe    = NULL;

    if(pSipNetworkEvtSubscribeList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipNetworkEvtSubscribeList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipNetworkEvtSubscribeList->pdata[index]);
                pSipNetworkEvtSubscribeList->pdata[index] = NULL;
                pSipNetworkEvtSubscribeList->ulQuantity--;
            }

            //create new
            pSipNetworkEvtSubscribeList->pdata[index] = (DML_SIP_NETWORK_EVENTSUBSCRIBE_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_NETWORK_EVENTSUBSCRIBE_CTRL_T));

            //set default
            if(pSipNetworkEvtSubscribeList->pdata[index] != NULL)
            {
                DML_SIP_NETWORK_EVENTSUBSCRIBE_CTRL_T* pDmlSipNetworkEvtSubscribeData = pSipNetworkEvtSubscribeList->pdata[index];
                pDmlSipNetworkEvtSubscribeData->updated = false;
                pDmlSipNetworkEvtSubscribe = &(pDmlSipNetworkEvtSubscribeData->dml);
                if ( pDmlSipNetworkEvtSubscribe == NULL )
                {
                    memset( pDmlSipNetworkEvtSubscribe, 0, sizeof( DML_SIP_NETWORK_EVENTSUBSCRIBE ) );
                    pDmlSipNetworkEvtSubscribe->uInstanceNumber = index + 1;
                }
                pSipNetworkEvtSubscribeList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddSipNetworkRespMap(PDML_SIP_NETWORK_RESPMAP_LIST_T pSipNetworkRespMapList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_NETWORK_RESPMAP   pDmlSipNetworkRespMap    = NULL;

    if(pSipNetworkRespMapList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipNetworkRespMapList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipNetworkRespMapList->pdata[index]);
                pSipNetworkRespMapList->pdata[index] = NULL;
                pSipNetworkRespMapList->ulQuantity--;
            }

            //create new
            pSipNetworkRespMapList->pdata[index] = (DML_SIP_NETWORK_RESPMAP_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_NETWORK_RESPMAP_CTRL_T));

            //set default
            if(pSipNetworkRespMapList->pdata[index] != NULL)
            {
                DML_SIP_NETWORK_RESPMAP_CTRL_T* pDmlSipNetworkRespMapData = pSipNetworkRespMapList->pdata[index];
                pDmlSipNetworkRespMapData->updated = false;
                pDmlSipNetworkRespMap = &(pDmlSipNetworkRespMapData->dml);
                if ( pDmlSipNetworkRespMap == NULL )
                {
                    memset( pDmlSipNetworkRespMap, 0, sizeof( DML_SIP_NETWORK_RESPMAP ) );
                    pDmlSipNetworkRespMap->uInstanceNumber = index + 1;
                }
                pSipNetworkRespMapList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddSipProxy(PDML_SIP_PROXY_LIST_T pSipProxyList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_PROXY   pDmlSipProxy    = NULL;

    if(pSipProxyList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipProxyList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipProxyList->pdata[index]);
                pSipProxyList->pdata[index] = NULL;
                pSipProxyList->ulQuantity--;
            }

            //create new
            pSipProxyList->pdata[index] = (DML_SIP_PROXY_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_PROXY_CTRL_T));

            //set default
            if(pSipProxyList->pdata[index] != NULL)
            {
                DML_SIP_PROXY_CTRL_T* pDmlSipProxyData = pSipProxyList->pdata[index];
                pDmlSipProxyData->updated = false;
                pDmlSipProxy = &(pDmlSipProxyData->dml);
                if ( !pDmlSipProxy )
                {
                    memset( pDmlSipProxy, 0, sizeof( DML_SIP_PROXY ) );
                    pDmlSipProxy->uInstanceNumber = index + 1;
                }
                pSipProxyList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddSipRegistrar(PDML_SIP_REGISTRAR_LIST_T pSipRegistrarList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_REGISTRAR   pDmlSipRegistrar    = NULL;

    if(pSipRegistrarList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipRegistrarList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipRegistrarList->pdata[index]);
                pSipRegistrarList->pdata[index] = NULL;
                pSipRegistrarList->ulQuantity--;
            }

            //create new
            pSipRegistrarList->pdata[index] = (DML_SIP_REGISTRAR_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_REGISTRAR_CTRL_T));

            //set default
            if(pSipRegistrarList->pdata[index] != NULL)
            {
                DML_SIP_REGISTRAR_CTRL_T* pDmlSipRegistrarData = pSipRegistrarList->pdata[index];
                pDmlSipRegistrarData->updated = false;
                pDmlSipRegistrar = &(pDmlSipRegistrarData->dml);
                if ( pDmlSipRegistrar == NULL )
                {
                    memset( pDmlSipRegistrar, 0, sizeof( DML_SIP_REGISTRAR ) );
                    pDmlSipRegistrar->uInstanceNumber = index + 1;
                }
                pSipRegistrarList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddSipRegistrarAcc(PDML_SIP_REGISTRAR_ACCOUNT_LIST_T pSipRegistrarAccList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_REGISTRAR   pDmlSipRegistrarAcc    = NULL;

    if(pSipRegistrarAccList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipRegistrarAccList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipRegistrarAccList->pdata[index]);
                pSipRegistrarAccList->pdata[index] = NULL;
                pSipRegistrarAccList->ulQuantity--;
            }

            //create new
            pSipRegistrarAccList->pdata[index] = (DML_SIP_REGISTRAR_ACCOUNT_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_REGISTRAR_ACCOUNT_CTRL_T));

            //set default
            if(pSipRegistrarAccList->pdata[index] != NULL)
            {
                DML_SIP_REGISTRAR_ACCOUNT_CTRL_T* pDmlSipRegistrarAccData = pSipRegistrarAccList->pdata[index];
                pDmlSipRegistrarAccData->updated = false;
                pDmlSipRegistrarAcc = &(pDmlSipRegistrarAccData->dml);
                if ( pDmlSipRegistrarAcc == NULL )
                {
                    memset( pDmlSipRegistrarAcc, 0, sizeof( DML_SIP_REGISTRAR ) );
                    pDmlSipRegistrarAcc->uInstanceNumber = index + 1;
                }
                pSipRegistrarAccList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddSipRegistrarAccContact(PDML_SIP_REGISTRAR_ACCOUNT_CONTACT_LIST_T pSipRegistrarAccContactList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_SIP_REGISTRAR_ACCOUNT_CONTACT   pDmlSipRegistrarAccContact    = NULL;

    if(pSipRegistrarAccContactList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pSipRegistrarAccContactList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pSipRegistrarAccContactList->pdata[index]);
                pSipRegistrarAccContactList->pdata[index] = NULL;
                pSipRegistrarAccContactList->ulQuantity--;
            }

            //create new
            pSipRegistrarAccContactList->pdata[index] = (DML_SIP_REGISTRAR_ACCOUNT_CONTACT_CTRL_T*) AnscAllocateMemory(sizeof(DML_SIP_REGISTRAR_ACCOUNT_CONTACT_CTRL_T));

            //set default
            if(pSipRegistrarAccContactList->pdata[index] != NULL)
            {
                DML_SIP_REGISTRAR_ACCOUNT_CONTACT_CTRL_T* pDmlSipRegistrarAccContactData = pSipRegistrarAccContactList->pdata[index];
                pDmlSipRegistrarAccContactData->updated = false;
                pDmlSipRegistrarAccContact = &(pDmlSipRegistrarAccContactData->dml);
                if ( pDmlSipRegistrarAccContact == NULL )
                {
                    memset( pDmlSipRegistrarAccContact, 0, sizeof( DML_SIP_REGISTRAR_ACCOUNT_CONTACT ) );
                    pDmlSipRegistrarAccContact->uInstanceNumber = index + 1;
                }
                pSipRegistrarAccContactList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddMGCPClient(PDML_MGCP_CLIENT_LIST_T pMGCP_ClientList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_MGCP_CLIENT   pDmlMGCP_Client    = NULL;

    if(pMGCP_ClientList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pMGCP_ClientList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pMGCP_ClientList->pdata[index]);
                pMGCP_ClientList->pdata[index] = NULL;
                pMGCP_ClientList->ulQuantity--;
            }

            //create new
            pMGCP_ClientList->pdata[index] = (DML_MGCP_CLIENT_CTRL_T*) AnscAllocateMemory(sizeof(DML_MGCP_CLIENT_CTRL_T));

            //set default
            if(pMGCP_ClientList->pdata[index] != NULL)
            {
                DML_MGCP_CLIENT_CTRL_T* pDmlMGCP_ClientData = pMGCP_ClientList->pdata[index];
                pDmlMGCP_ClientData->updated = false;
                pDmlMGCP_Client = &(pDmlMGCP_ClientData->dml);
                if ( !pDmlMGCP_Client )
                {
                    memset( pDmlMGCP_Client, 0, sizeof( DML_MGCP_CLIENT ) );
                    pDmlMGCP_Client->uInstanceNumber = index + 1;
                }
                pMGCP_ClientList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddMGCPNetwork(PDML_MGCP_NETWORK_LIST_T pMGCP_NetworkList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_MGCP_NETWORK   pDmlMGCP_Network    = NULL;

    if(pMGCP_NetworkList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pMGCP_NetworkList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pMGCP_NetworkList->pdata[index]);
                pMGCP_NetworkList->pdata[index] = NULL;
                pMGCP_NetworkList->ulQuantity--;
            }

            //create new
            pMGCP_NetworkList->pdata[index] = (DML_MGCP_NETWORK_CTRL_T*) AnscAllocateMemory(sizeof(DML_MGCP_NETWORK_CTRL_T));

            //set default
            if(pMGCP_NetworkList->pdata[index] != NULL)
            {
                DML_MGCP_NETWORK_CTRL_T* pDmlMGCP_NetworkData = pMGCP_NetworkList->pdata[index];
                pDmlMGCP_NetworkData->updated = false;
                pDmlMGCP_Network = &(pDmlMGCP_NetworkData->dml);
                if ( !pDmlMGCP_Network )
                {
                    memset( pDmlMGCP_Network, 0, sizeof( DML_MGCP_NETWORK ) );
                    pDmlMGCP_Network->uInstanceNumber = index + 1;
                }
                pMGCP_NetworkList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddH323Client(PDML_H323_CLIENT_LIST_T pH323CliList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_H323_CLIENT  pDmlH323Cli    = NULL;

    if(pH323CliList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pH323CliList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pH323CliList->pdata[index]);
                pH323CliList->pdata[index] = NULL;
                pH323CliList->ulQuantity--;
            }

            //create new
            pH323CliList->pdata[index] = (DML_H323_CLIENT_CTRL_T*) AnscAllocateMemory(sizeof(DML_H323_CLIENT_CTRL_T));

            //set default
            if(pH323CliList->pdata[index] != NULL)
            {
                DML_H323_CLIENT_CTRL_T* pDmlH323CliData = pH323CliList->pdata[index];
                pDmlH323CliData->updated = false;
                pDmlH323Cli = &(pDmlH323CliData->dml);
                if ( !pDmlH323Cli )
                {
                    memset( pDmlH323Cli, 0, sizeof( DML_H323_CLIENT ) );
                    pDmlH323Cli->uInstanceNumber = index + 1;
                }
                pH323CliList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddH323Network(PDML_H323_NETWORK_LIST_T pH323NwkList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_H323_NETWORK  pDmlH323Nwk    = NULL;

    if(pH323NwkList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pH323NwkList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pH323NwkList->pdata[index]);
                pH323NwkList->pdata[index] = NULL;
                pH323NwkList->ulQuantity--;
            }

            //create new
            pH323NwkList->pdata[index] = (DML_H323_NETWORK_CTRL_T*) AnscAllocateMemory(sizeof(DML_H323_NETWORK_CTRL_T));

            //set default
            if(pH323NwkList->pdata[index] != NULL)
            {
                DML_H323_NETWORK_CTRL_T* pDmlH323NwkData = pH323NwkList->pdata[index];
                pDmlH323NwkData->updated = false;
                pDmlH323Nwk = &(pDmlH323NwkData->dml);
                if ( !pDmlH323Nwk )
                {
                    memset( pDmlH323Nwk, 0, sizeof( DML_H323_NETWORK ) );
                    pDmlH323Nwk->uInstanceNumber = index + 1;
                }
                pH323NwkList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddTrunk(PDML_TRUNK_LIST_T pTrunkList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_TRUNK   pDmlTrunk    = NULL;

    if(pTrunkList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pTrunkList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pTrunkList->pdata[index]);
                pTrunkList->pdata[index] = NULL;
                pTrunkList->ulQuantity--;
            }

            //create new
            pTrunkList->pdata[index] = (DML_TRUNK_CTRL_T*) AnscAllocateMemory(sizeof(DML_TRUNK_CTRL_T));

            //set default
            if(pTrunkList->pdata[index] != NULL)
            {
                DML_TRUNK_CTRL_T* pDmlTrunkData = pTrunkList->pdata[index];
                pDmlTrunkData->updated = false;
                pDmlTrunk = &(pDmlTrunkData->dml);
                if ( !pDmlTrunk )
                {
                    memset( pDmlTrunk, 0, sizeof( DML_TRUNK ) );
                    pDmlTrunk->uInstanceNumber = index + 1;
                }
                pTrunkList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlLine(PDML_CALLCONTROL_LINE_LIST_T pCallCtrlLineList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_LINE   pDmlCallCtrlLine    = NULL;

    if(pCallCtrlLineList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlLineList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlLineList->pdata[index]);
                pCallCtrlLineList->pdata[index] = NULL;
                pCallCtrlLineList->ulQuantity--;
            }

            //create new
            pCallCtrlLineList->pdata[index] = (DML_CALLCONTROL_LINE_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_LINE_CTRL_T));

            //set default
            if(pCallCtrlLineList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_LINE_CTRL_T* pDmlCallCtrlLineData = pCallCtrlLineList->pdata[index];
                pDmlCallCtrlLineData->updated = false;
                pDmlCallCtrlLine = &(pDmlCallCtrlLineData->dml);
                if ( !pDmlCallCtrlLine )
                {
                    memset( pDmlCallCtrlLine, 0, sizeof( DML_CALLCONTROL_LINE ) );
                    pDmlCallCtrlLine->uInstanceNumber = index + 1;
                }
                pCallCtrlLineList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlExt(PDML_CALLCONTROL_EXTENSION_LIST_T pCallCtrlExtList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_EXTENSION   pDmlCallCtrlExt    = NULL;

    if(pCallCtrlExtList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlExtList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlExtList->pdata[index]);
                pCallCtrlExtList->pdata[index] = NULL;
                pCallCtrlExtList->ulQuantity--;
            }

            //create new
            pCallCtrlExtList->pdata[index] = (DML_CALLCONTROL_EXTENSION_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_EXTENSION_CTRL_T));

            //set default
            if(pCallCtrlExtList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_EXTENSION_CTRL_T* pDmlCallCtrlExtData = pCallCtrlExtList->pdata[index];
                pDmlCallCtrlExtData->updated = false;
                pDmlCallCtrlExt = &(pDmlCallCtrlExtData->dml);
                if ( !pDmlCallCtrlExt )
                {
                    memset( pDmlCallCtrlExt, 0, sizeof( DML_CALLCONTROL_EXTENSION ) );
                    pDmlCallCtrlExt->uInstanceNumber = index + 1;
                }
                pCallCtrlExtList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlGrp(PDML_CALLCONTROL_GROUP_LIST_T pCallCtrlGrpList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_GROUP   pDmlCallCtrlGrp    = NULL;

    if(pCallCtrlGrpList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlGrpList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlGrpList->pdata[index]);
                pCallCtrlGrpList->pdata[index] = NULL;
                pCallCtrlGrpList->ulQuantity--;
            }

            //create new
            pCallCtrlGrpList->pdata[index] = (DML_CALLCONTROL_GROUP_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_GROUP_CTRL_T));

            //set default
            if(pCallCtrlGrpList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_GROUP_CTRL_T* pDmlCallCtrlGrpData = pCallCtrlGrpList->pdata[index];
                pDmlCallCtrlGrpData->updated = false;
                pDmlCallCtrlGrp = &(pDmlCallCtrlGrpData->dml);
                if ( !pDmlCallCtrlGrp )
                {
                    memset( pDmlCallCtrlGrp, 0, sizeof( DML_CALLCONTROL_GROUP ) );
                    pDmlCallCtrlGrp->uInstanceNumber = index + 1;
                }
                pCallCtrlGrpList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlMailBox(PDML_CALLCONTROL_MAILBOX_LIST_T pCallCtrlMailBoxList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_MAILBOX   pDmlCallCtrlMailBox    = NULL;

    if(pCallCtrlMailBoxList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlMailBoxList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlMailBoxList->pdata[index]);
                pCallCtrlMailBoxList->pdata[index] = NULL;
                pCallCtrlMailBoxList->ulQuantity--;
            }

            //create new
            pCallCtrlMailBoxList->pdata[index] = (DML_CALLCONTROL_MAILBOX_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_MAILBOX_CTRL_T));

            //set default
            if(pCallCtrlMailBoxList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_MAILBOX_CTRL_T* pDmlCallCtrlMailBoxData = pCallCtrlMailBoxList->pdata[index];
                pDmlCallCtrlMailBoxData->updated = false;
                pDmlCallCtrlMailBox = &(pDmlCallCtrlMailBoxData->dml);
                if ( !pDmlCallCtrlMailBox )
                {
                    memset( pDmlCallCtrlMailBox, 0, sizeof( DML_CALLCONTROL_MAILBOX ) );
                    pDmlCallCtrlMailBox->uInstanceNumber = index + 1;
                }
                pCallCtrlMailBoxList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlInMap(PDML_CALLCONTROL_INCOMINGMAP_LIST_T pCallCtrlInMapList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_INCOMINGMAP   pDmlCallCtrlInMap    = NULL;

    if(pCallCtrlInMapList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlInMapList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlInMapList->pdata[index]);
                pCallCtrlInMapList->pdata[index] = NULL;
                pCallCtrlInMapList->ulQuantity--;
            }

            //create new
            pCallCtrlInMapList->pdata[index] = (DML_CALLCONTROL_INCOMINGMAP_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_INCOMINGMAP_CTRL_T));

            //set default
            if(pCallCtrlInMapList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_INCOMINGMAP_CTRL_T* pDmlCallCtrlInMapData = pCallCtrlInMapList->pdata[index];
                pDmlCallCtrlInMapData->updated = false;
                pDmlCallCtrlInMap = &(pDmlCallCtrlInMapData->dml);
                if ( !pDmlCallCtrlInMap )
                {
                    memset( pDmlCallCtrlInMap, 0, sizeof( DML_CALLCONTROL_INCOMINGMAP ) );
                    pDmlCallCtrlInMap->uInstanceNumber = index + 1;
                }
                pCallCtrlInMapList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlOutMap(PDML_CALLCONTROL_OUTGOINGMAP_LIST_T pCallCtrlOutMapList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_OUTGOINGMAP   pDmlCallCtrlOutMap    = NULL;

    if(pCallCtrlOutMapList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlOutMapList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlOutMapList->pdata[index]);
                pCallCtrlOutMapList->pdata[index] = NULL;
                pCallCtrlOutMapList->ulQuantity--;
            }

            //create new
            pCallCtrlOutMapList->pdata[index] = (DML_CALLCONTROL_OUTGOINGMAP_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_OUTGOINGMAP_CTRL_T));

            //set default
            if(pCallCtrlOutMapList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_OUTGOINGMAP_CTRL_T* pDmlCallCtrlOutMapData = pCallCtrlOutMapList->pdata[index];
                pDmlCallCtrlOutMapData->updated = false;
                pDmlCallCtrlOutMap = &(pDmlCallCtrlOutMapData->dml);
                if ( !pDmlCallCtrlOutMap )
                {
                    memset( pDmlCallCtrlOutMap, 0, sizeof( DML_CALLCONTROL_OUTGOINGMAP ) );
                    pDmlCallCtrlOutMap->uInstanceNumber = index + 1;
                }
                pCallCtrlOutMapList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlNumPlan(PDML_CALLCONTROL_NUMBERINGPLAN_LIST_T pCallCtrlNumPlanList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_NUMBERINGPLAN   pDmlCallCtrlNumPlan    = NULL;

    if(pCallCtrlNumPlanList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlNumPlanList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlNumPlanList->pdata[index]);
                pCallCtrlNumPlanList->pdata[index] = NULL;
                pCallCtrlNumPlanList->ulQuantity--;
            }

            //create new
            pCallCtrlNumPlanList->pdata[index] = (DML_CALLCONTROL_NUMBERINGPLAN_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_NUMBERINGPLAN_CTRL_T));

            //set default
            if(pCallCtrlNumPlanList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_NUMBERINGPLAN_CTRL_T* pDmlCallCtrlNumPlanData = pCallCtrlNumPlanList->pdata[index];
                pDmlCallCtrlNumPlanData->updated = false;
                pDmlCallCtrlNumPlan = &(pDmlCallCtrlNumPlanData->dml);
                if ( !pDmlCallCtrlNumPlan )
                {
                    memset( pDmlCallCtrlNumPlan, 0, sizeof( DML_CALLCONTROL_NUMBERINGPLAN ) );
                    pDmlCallCtrlNumPlan->uInstanceNumber = index + 1;
                }
                pCallCtrlNumPlanList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlNumPlanPrefixInfo(PDML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_LIST_T pCallCtrlNumPlanPrefixInfoList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO   pDmlCallCtrlNumPlanPrefixInfo    = NULL;

    if(pCallCtrlNumPlanPrefixInfoList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlNumPlanPrefixInfoList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlNumPlanPrefixInfoList->pdata[index]);
                pCallCtrlNumPlanPrefixInfoList->pdata[index] = NULL;
                pCallCtrlNumPlanPrefixInfoList->ulQuantity--;
            }

            //create new
            pCallCtrlNumPlanPrefixInfoList->pdata[index] = (DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_CTRL_T));

            //set default
            if(pCallCtrlNumPlanPrefixInfoList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO_CTRL_T* pDmlCallCtrlNumPlanPrefixInfoData = pCallCtrlNumPlanPrefixInfoList->pdata[index];
                pDmlCallCtrlNumPlanPrefixInfoData->updated = false;
                pDmlCallCtrlNumPlanPrefixInfo = &(pDmlCallCtrlNumPlanPrefixInfoData->dml);
                if ( !pDmlCallCtrlNumPlanPrefixInfo )
                {
                    memset( pDmlCallCtrlNumPlanPrefixInfo, 0, sizeof( DML_CALLCONTROL_NUMBERINGPLAN_PREFIXINFO ) );
                    pDmlCallCtrlNumPlanPrefixInfo->uInstanceNumber = index + 1;
                }
                pCallCtrlNumPlanPrefixInfoList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSet(PDML_CALLCONTROL_CALLINGFEATURES_SET_LIST_T pCallCtrlCFSetList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_CALLINGFEATURES_SET    pDmlCallCtrlCFSet    = NULL;

    if(pCallCtrlCFSetList != NULL)
    {
        if(index < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES)
        {
            //delete old
            if(pCallCtrlCFSetList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlCFSetList->pdata[index]);
                pCallCtrlCFSetList->pdata[index] = NULL;
                pCallCtrlCFSetList->ulQuantity--;
            }

            //create new
            pCallCtrlCFSetList->pdata[index] = (DML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_T));

            //set default
            if(pCallCtrlCFSetList->pdata[index] != NULL)
            {
                PDML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_T pCallCtrlCFSetData = pCallCtrlCFSetList->pdata[index];
                pCallCtrlCFSetData->updated = false;
                pDmlCallCtrlCFSet = &(pCallCtrlCFSetData->dml);
                if ( !pDmlCallCtrlCFSet )
                {
                    memset( pDmlCallCtrlCFSet, 0, sizeof( DML_CALLCONTROL_CALLINGFEATURES_SET ) );
                    pDmlCallCtrlCFSet->uInstanceNumber = index + 1;
                }
                pCallCtrlCFSetList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSetCFT(PDML_CALLCONTROL_CALLINGFEATURES_SET_CFT_LIST_T pCallCtrlCFSetCFTList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_CALLINGFEATURES_SET_CFT   pDmlCallCtrlCFSetCFT    = NULL;

    if(pCallCtrlCFSetCFTList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlCFSetCFTList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlCFSetCFTList->pdata[index]);
                pCallCtrlCFSetCFTList->pdata[index] = NULL;
                pCallCtrlCFSetCFTList->ulQuantity--;
            }

            //create new
            pCallCtrlCFSetCFTList->pdata[index] = (DML_CALLCONTROL_CALLINGFEATURES_SET_CFT_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_CALLINGFEATURES_SET_CFT_CTRL_T));

            //set default
            if(pCallCtrlCFSetCFTList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_CALLINGFEATURES_SET_CFT_CTRL_T* pDmlCallCtrlCFSetCFTData = pCallCtrlCFSetCFTList->pdata[index];
                pDmlCallCtrlCFSetCFTData->updated = false;
                pDmlCallCtrlCFSetCFT = &(pDmlCallCtrlCFSetCFTData->dml);
                if ( !pDmlCallCtrlCFSetCFT )
                {
                    memset( pDmlCallCtrlCFSetCFT, 0, sizeof( DML_CALLCONTROL_CALLINGFEATURES_SET_CFT ) );
                    pDmlCallCtrlCFSetCFT->uInstanceNumber = index + 1;
                }
                pCallCtrlCFSetCFTList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSetSCF(PDML_CALLCONTROL_CALLINGFEATURES_SET_SCF_LIST_T pCallCtrlCFSetSCFList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_CALLINGFEATURES_SET_SCF  pDmlCallCtrlCFSetSCF    = NULL;

    if(pCallCtrlCFSetSCFList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlCFSetSCFList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlCFSetSCFList->pdata[index]);
                pCallCtrlCFSetSCFList->pdata[index] = NULL;
                pCallCtrlCFSetSCFList->ulQuantity--;
            }

            //create new
            pCallCtrlCFSetSCFList->pdata[index] = (DML_CALLCONTROL_CALLINGFEATURES_SET_SCF_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_CALLINGFEATURES_SET_SCF_CTRL_T));

            //set default
            if(pCallCtrlCFSetSCFList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_CALLINGFEATURES_SET_SCF_CTRL_T* pDmlCallCtrlCFSetSCFData = pCallCtrlCFSetSCFList->pdata[index];
                pDmlCallCtrlCFSetSCFData->updated = false;
                pDmlCallCtrlCFSetSCF = &(pDmlCallCtrlCFSetSCFData->dml);
                if ( !pDmlCallCtrlCFSetSCF )
                {
                    memset( pDmlCallCtrlCFSetSCF, 0, sizeof( DML_CALLCONTROL_CALLINGFEATURES_SET_SCF ) );
                    pDmlCallCtrlCFSetSCF->uInstanceNumber = index + 1;
                }
                pCallCtrlCFSetSCFList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSetSCREJ(PDML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_LIST_T pCallCtrlCFSetSCREJList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ   pDmlCallCtrlCFSetSCREJ    = NULL;

    if(pCallCtrlCFSetSCREJList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlCFSetSCREJList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlCFSetSCREJList->pdata[index]);
                pCallCtrlCFSetSCREJList->pdata[index] = NULL;
                pCallCtrlCFSetSCREJList->ulQuantity--;
            }

            //create new
            pCallCtrlCFSetSCREJList->pdata[index] = (DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_CTRL_T));

            //set default
            if(pCallCtrlCFSetSCREJList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ_CTRL_T* pDmlCallCtrlCFSetSCREJData = pCallCtrlCFSetSCREJList->pdata[index];
                pDmlCallCtrlCFSetSCREJData->updated = false;
                pDmlCallCtrlCFSetSCREJ = &(pDmlCallCtrlCFSetSCREJData->dml);
                if ( !pDmlCallCtrlCFSetSCREJ )
                {
                    memset( pDmlCallCtrlCFSetSCREJ, 0, sizeof( DML_CALLCONTROL_CALLINGFEATURES_SET_SCREJ ) );
                    pDmlCallCtrlCFSetSCREJ->uInstanceNumber = index + 1;
                }
                pCallCtrlCFSetSCREJList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddCallCtrlCFSetFollowMe(PDML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_LIST_T pCallCtrlCFSetFollowMeList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME   pDmlCallCtrlCFSetFollowMe    = NULL;

    if(pCallCtrlCFSetFollowMeList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallCtrlCFSetFollowMeList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallCtrlCFSetFollowMeList->pdata[index]);
                pCallCtrlCFSetFollowMeList->pdata[index] = NULL;
                pCallCtrlCFSetFollowMeList->ulQuantity--;
            }

            //create new
            pCallCtrlCFSetFollowMeList->pdata[index] = (DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_CTRL_T));

            //set default
            if(pCallCtrlCFSetFollowMeList->pdata[index] != NULL)
            {
                DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME_CTRL_T* pDmlCallCtrlCFSetFollowMeData = pCallCtrlCFSetFollowMeList->pdata[index];
                pDmlCallCtrlCFSetFollowMeData->updated = false;
                pDmlCallCtrlCFSetFollowMe = &(pDmlCallCtrlCFSetFollowMeData->dml);
                if ( !pDmlCallCtrlCFSetFollowMe )
                {
                    memset( pDmlCallCtrlCFSetFollowMe, 0, sizeof( DML_CALLCONTROL_CALLINGFEATURES_SET_FOLLOWME ) );
                    pDmlCallCtrlCFSetFollowMe->uInstanceNumber = index + 1;
                }
                pCallCtrlCFSetFollowMeList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddInterwork(PDML_INTERWORK_LIST_T pInterworkList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_INTERWORK   pDmlInterwork    = NULL;

    if(pInterworkList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pInterworkList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pInterworkList->pdata[index]);
                pInterworkList->pdata[index] = NULL;
                pInterworkList->ulQuantity--;
            }

            //create new
            pInterworkList->pdata[index] = (DML_INTERWORK_CTRL_T*) AnscAllocateMemory(sizeof(DML_INTERWORK_CTRL_T));

            //set default
            if(pInterworkList->pdata[index] != NULL)
            {
                DML_INTERWORK_CTRL_T* pDmlInterworkData = pInterworkList->pdata[index];
                pDmlInterworkData->updated = false;
                pDmlInterwork = &(pDmlInterworkData->dml);
                if ( pDmlInterwork == NULL )
                {
                    memset( pDmlInterwork, 0, sizeof( DML_INTERWORK ) );
                    pDmlInterwork->uInstanceNumber = index + 1;
                }
                pInterworkList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddInterworkUI(PDML_INTERWORK_USERINTERFACE_LIST_T pInterworkUIList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_INTERWORK_USERINTERFACE   pDmlInterworkUI    = NULL;

    if(pInterworkUIList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pInterworkUIList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pInterworkUIList->pdata[index]);
                pInterworkUIList->pdata[index] = NULL;
                pInterworkUIList->ulQuantity--;
            }

            //create new
            pInterworkUIList->pdata[index] = (DML_INTERWORK_USERINTERFACE_CTRL_T*) AnscAllocateMemory(sizeof(DML_INTERWORK_USERINTERFACE_CTRL_T));

            //set default
            if(pInterworkUIList->pdata[index] != NULL)
            {
                DML_INTERWORK_USERINTERFACE_CTRL_T* pDmlInterworkUIData = pInterworkUIList->pdata[index];
                pDmlInterworkUIData->updated = false;
                pDmlInterworkUI = &(pDmlInterworkUIData->dml);
                if ( pDmlInterworkUI == NULL )
                {
                    memset( pDmlInterworkUI, 0, sizeof( DML_INTERWORK_USERINTERFACE ) );
                    pDmlInterworkUI->uInstanceNumber = index + 1;
                }
                pInterworkUIList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddInterworkMap(PDML_INTERWORK_MAP_LIST_T pInterworkMapList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_INTERWORK_MAP   pDmlInterworkMap    = NULL;

    if(pInterworkMapList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pInterworkMapList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pInterworkMapList->pdata[index]);
                pInterworkMapList->pdata[index] = NULL;
                pInterworkMapList->ulQuantity--;
            }

            //create new
            pInterworkMapList->pdata[index] = (DML_INTERWORK_MAP_CTRL_T*) AnscAllocateMemory(sizeof(DML_INTERWORK_MAP_CTRL_T));

            //set default
            if(pInterworkMapList->pdata[index] != NULL)
            {
                DML_INTERWORK_MAP_CTRL_T* pDmlInterworkMapData = pInterworkMapList->pdata[index];
                pDmlInterworkMapData->updated = false;
                pDmlInterworkMap = &(pDmlInterworkMapData->dml);
                if ( pDmlInterworkMap == NULL )
                {
                    memset( pDmlInterworkMap, 0, sizeof( DML_INTERWORK_MAP ) );
                    pDmlInterworkMap->uInstanceNumber = index + 1;
                }
                pInterworkMapList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddCallLog(PDML_CALLLOG_LIST_T pCallLogList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLLOG   pDmlCallLog    = NULL;

    if(pCallLogList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallLogList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallLogList->pdata[index]);
                pCallLogList->pdata[index] = NULL;
                pCallLogList->ulQuantity--;
            }

            //create new
            pCallLogList->pdata[index] = (DML_CALLLOG_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLLOG_CTRL_T));

            //set default
            if(pCallLogList->pdata[index] != NULL)
            {
                DML_CALLLOG_CTRL_T* pDmlCallLogData = pCallLogList->pdata[index];
                pDmlCallLogData->updated = false;
                pDmlCallLog = &(pDmlCallLogData->dml);
                if ( !pDmlCallLog )
                {
                    memset( pDmlCallLog, 0, sizeof( DML_CALLLOG ) );
                    pDmlCallLog->uInstanceNumber = index + 1;
                }
                pCallLogList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddCallLogSignalPerf(PDML_CALLLOG_SIGNALINGPERF_LIST_T pCallLogSignPerfList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLLOG_SIGNALINGPERF  pDmlCallLogSignPerf    = NULL;

    if(pCallLogSignPerfList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallLogSignPerfList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallLogSignPerfList->pdata[index]);
                pCallLogSignPerfList->pdata[index] = NULL;
                pCallLogSignPerfList->ulQuantity--;
            }

            //create new
            pCallLogSignPerfList->pdata[index] = (DML_CALLLOG_SIGNALINGPERF_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLLOG_SIGNALINGPERF_CTRL_T));

            //set default
            if(pCallLogSignPerfList->pdata[index] != NULL)
            {
                DML_CALLLOG_SIGNALINGPERF_CTRL_T* pDmlCallLogSignPerfData = pCallLogSignPerfList->pdata[index];
                pDmlCallLogSignPerfData->updated = false;
                pDmlCallLogSignPerf = &(pDmlCallLogSignPerfData->dml);
                if ( !pDmlCallLogSignPerf )
                {
                    memset( pDmlCallLogSignPerf, 0, sizeof( DML_CALLLOG_SIGNALINGPERF ) );
                    pDmlCallLogSignPerf->uInstanceNumber = index + 1;
                }
                pCallLogSignPerfList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddCallLogSession(PDML_CALLLOG_SESSION_LIST_T pCallLogSessionList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CALLLOG_SESSION   pDmlCallLogSession    = NULL;

    if(pCallLogSessionList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCallLogSessionList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCallLogSessionList->pdata[index]);
                pCallLogSessionList->pdata[index] = NULL;
                pCallLogSessionList->ulQuantity--;
            }

            //create new
            pCallLogSessionList->pdata[index] = (DML_CALLLOG_SESSION_CTRL_T*) AnscAllocateMemory(sizeof(DML_CALLLOG_SESSION_CTRL_T));

            //set default
            if(pCallLogSessionList->pdata[index] != NULL)
            {
                DML_CALLLOG_SESSION_CTRL_T* pDmlCallLogSessionData = pCallLogSessionList->pdata[index];
                pDmlCallLogSessionData->updated = false;
                pDmlCallLogSession = &(pDmlCallLogSessionData->dml);
                if ( !pDmlCallLogSession )
                {
                    memset( pDmlCallLogSession, 0, sizeof( DML_CALLLOG_SESSION ) );
                    pDmlCallLogSession->uInstanceNumber = index + 1;
                }
                pCallLogSessionList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddVoipProfile(PDML_VOIPPROFILE_LIST_T pVoipProfileList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_VOIPPROFILE    pDmlVoipProfile    = NULL;

    if(pVoipProfileList != NULL)
    {
        if(index < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES)
        {
            //delete old
            if(pVoipProfileList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pVoipProfileList->pdata[index]);
                pVoipProfileList->pdata[index] = NULL;
                pVoipProfileList->ulQuantity--;
            }

            //create new
            pVoipProfileList->pdata[index] = (DML_VOIPPROFILE_CTRL_T*) AnscAllocateMemory(sizeof(DML_VOIPPROFILE_CTRL_T));

            //set default
            if(pVoipProfileList->pdata[index] != NULL)
            {
                PDML_VOIPPROFILE_CTRL_T pVoipProfileData = pVoipProfileList->pdata[index];
                pVoipProfileData->updated = false;
                pDmlVoipProfile = &(pVoipProfileData->dml);
                if ( !pDmlVoipProfile )
                {
                    memset( pDmlVoipProfile, 0, sizeof( DML_VOIPPROFILE ) );
                    pDmlVoipProfile->uInstanceNumber = index + 1;
                }
                pVoipProfileList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddCodecProfile(PDML_CODECPROFILE_LIST_T pCodecProfileList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_CODECPROFILE   pDmlCodecProfile    = NULL;

    if(pCodecProfileList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pCodecProfileList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pCodecProfileList->pdata[index]);
                pCodecProfileList->pdata[index] = NULL;
                pCodecProfileList->ulQuantity--;
            }

            //create new
            pCodecProfileList->pdata[index] = (DML_CODECPROFILE_CTRL_T*) AnscAllocateMemory(sizeof(DML_CODECPROFILE_CTRL_T));

            //set default
            if(pCodecProfileList->pdata[index] != NULL)
            {
                DML_CODECPROFILE_CTRL_T* pDmlCodecProfileData = pCodecProfileList->pdata[index];
                pDmlCodecProfileData->updated = false;
                pDmlCodecProfile = &(pDmlCodecProfileData->dml);
                if ( !pDmlCodecProfile )
                {
                    memset( pDmlCodecProfile, 0, sizeof( DML_CODECPROFILE ) );
                    pDmlCodecProfile->uInstanceNumber = index + 1;
                }
                pCodecProfileList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddToneDesc(PDML_TONE_DESCRIPTION_LIST_T pToneDescList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_TONE_DESCRIPTION   pDmlToneDesc    = NULL;

    if(pToneDescList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pToneDescList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pToneDescList->pdata[index]);
                pToneDescList->pdata[index] = NULL;
                pToneDescList->ulQuantity--;
            }

            //create new
            pToneDescList->pdata[index] = (DML_TONE_DESCRIPTION_CTRL_T*) AnscAllocateMemory(sizeof(DML_TONE_DESCRIPTION_CTRL_T));

            //set default
            if(pToneDescList->pdata[index] != NULL)
            {
                DML_TONE_DESCRIPTION_CTRL_T* pDmlToneDescData = pToneDescList->pdata[index];
                pDmlToneDescData->updated = false;
                pDmlToneDesc = &(pDmlToneDescData->dml);
                if ( !pDmlToneDesc )
                {
                    memset( pDmlToneDesc, 0, sizeof( DML_TONE_DESCRIPTION ) );
                    pDmlToneDesc->uInstanceNumber = index + 1;
                }
                pToneDescList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddTonePattern(PDML_TONE_PATTERN_LIST_T pTonePatternList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_TONE_PATTERN   pDmlTonePattern    = NULL;

    if(pTonePatternList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pTonePatternList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pTonePatternList->pdata[index]);
                pTonePatternList->pdata[index] = NULL;
                pTonePatternList->ulQuantity--;
            }

            //create new
            pTonePatternList->pdata[index] = (DML_TONE_PATTERN_CTRL_T*) AnscAllocateMemory(sizeof(DML_TONE_PATTERN_CTRL_T));

            //set default
            if(pTonePatternList->pdata[index] != NULL)
            {
                DML_TONE_PATTERN_CTRL_T* pDmlTonePatternData = pTonePatternList->pdata[index];
                pDmlTonePatternData->updated = false;
                pDmlTonePattern = &(pDmlTonePatternData->dml);
                if ( !pDmlTonePattern )
                {
                    memset( pDmlTonePattern, 0, sizeof( DML_TONE_PATTERN ) );
                    pDmlTonePattern->uInstanceNumber = index + 1;
                }
                pTonePatternList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddToneEvtProf(PDML_TONE_EVENTPROFILE_LIST_T pToneEvtProfList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_TONE_EVENTPROFILE   pDmlToneEvtProf    = NULL;

    if(pToneEvtProfList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pToneEvtProfList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pToneEvtProfList->pdata[index]);
                pToneEvtProfList->pdata[index] = NULL;
                pToneEvtProfList->ulQuantity--;
            }

            //create new
            pToneEvtProfList->pdata[index] = (DML_TONE_EVENTPROFILE_CTRL_T*) AnscAllocateMemory(sizeof(DML_TONE_EVENTPROFILE_CTRL_T));

            //set default
            if(pToneEvtProfList->pdata[index] != NULL)
            {
                DML_TONE_EVENTPROFILE_CTRL_T* pDmlToneEvtProfData = pToneEvtProfList->pdata[index];
                pDmlToneEvtProfData->updated = false;
                pDmlToneEvtProf = &(pDmlToneEvtProfData->dml);
                if ( !pDmlToneEvtProf )
                {
                    memset( pDmlToneEvtProf, 0, sizeof( DML_TONE_EVENTPROFILE ) );
                    pDmlToneEvtProf->uInstanceNumber = index + 1;
                }
                pToneEvtProfList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddToneEvtProfEvt(PDML_TONE_EVENTPROFILE_EVENT_LIST_T pToneEvtProfEvtList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_TONE_EVENTPROFILE_EVENT   pDmlToneEvtProfEvt    = NULL;

    if(pToneEvtProfEvtList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pToneEvtProfEvtList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pToneEvtProfEvtList->pdata[index]);
                pToneEvtProfEvtList->pdata[index] = NULL;
                pToneEvtProfEvtList->ulQuantity--;
            }

            //create new
            pToneEvtProfEvtList->pdata[index] = (DML_TONE_EVENTPROFILE_EVENT_CTRL_T*) AnscAllocateMemory(sizeof(DML_TONE_EVENTPROFILE_EVENT_CTRL_T));

            //set default
            if(pToneEvtProfEvtList->pdata[index] != NULL)
            {
                DML_TONE_EVENTPROFILE_EVENT_CTRL_T* pDmlToneEvtProfEvtData = pToneEvtProfEvtList->pdata[index];
                pDmlToneEvtProfEvtData->updated = false;
                pDmlToneEvtProfEvt = &(pDmlToneEvtProfEvtData->dml);
                if ( !pDmlToneEvtProfEvt )
                {
                    memset( pDmlToneEvtProfEvt, 0, sizeof( DML_TONE_EVENTPROFILE_EVENT ) );
                    pDmlToneEvtProfEvt->uInstanceNumber = index + 1;
                }
                pToneEvtProfEvtList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddTerminal(PDML_VOICESERVICE_TERMINAL_LIST_T pTerminalList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_VOICESERVICE_TERMINAL    pDmlTerminal    = NULL;

    if(pTerminalList != NULL)
    {
        if(index < TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES)
        {
            //delete old
            if(pTerminalList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pTerminalList->pdata[index]);
                pTerminalList->pdata[index] = NULL;
                pTerminalList->ulQuantity--;
            }

            //create new
            pTerminalList->pdata[index] = (DML_VOICESERVICE_TERMINAL_CTRL_T*) AnscAllocateMemory(sizeof(DML_VOICESERVICE_TERMINAL_CTRL_T));

            //set default
            if(pTerminalList->pdata[index] != NULL)
            {
                PDML_VOICESERVICE_TERMINAL_CTRL_T pTerminalData = pTerminalList->pdata[index];
                pTerminalData->updated = false;
                pDmlTerminal = &(pTerminalData->dml);
                if ( pDmlTerminal == NULL )
                {
                    memset( pDmlTerminal, 0, sizeof( DML_VOICESERVICE_TERMINAL ) );
                    pDmlTerminal->uInstanceNumber = index + 1;
                }
                pTerminalList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddTerminalAudio(PDML_TERMINAL_AUDIO_LIST_T pTerminalAudioList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_TERMINAL_AUDIO   pDmlTerminalAudio    = NULL;

    if(pTerminalAudioList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pTerminalAudioList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pTerminalAudioList->pdata[index]);
                pTerminalAudioList->pdata[index] = NULL;
                pTerminalAudioList->ulQuantity--;
            }

            //create new
            pTerminalAudioList->pdata[index] = (DML_TERMINAL_AUDIO_CTRL_T*) AnscAllocateMemory(sizeof(DML_TERMINAL_AUDIO_CTRL_T));

            //set default
            if(pTerminalAudioList->pdata[index] != NULL)
            {
                DML_TERMINAL_AUDIO_CTRL_T* pDmlTerminalAudioData = pTerminalAudioList->pdata[index];
                pDmlTerminalAudioData->updated = false;
                pDmlTerminalAudio = &(pDmlTerminalAudioData->dml);
                if ( pDmlTerminalAudio == NULL )
                {
                    memset( pDmlTerminalAudio, 0, sizeof( DML_TERMINAL_AUDIO ) );
                    pDmlTerminalAudio->uInstanceNumber = index + 1;
                }
                pTerminalAudioList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddTermButtonMapButton(PDML_TERMINAL_BUTTONMAP_BUTTON_LIST_T pTermButtonMapButtonList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_TERMINAL_BUTTONMAP_BUTTON   pDmlTermButtonMapButton    = NULL;

    if(pTermButtonMapButtonList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pTermButtonMapButtonList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pTermButtonMapButtonList->pdata[index]);
                pTermButtonMapButtonList->pdata[index] = NULL;
                pTermButtonMapButtonList->ulQuantity--;
            }

            //create new
            pTermButtonMapButtonList->pdata[index] = (DML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T*) AnscAllocateMemory(sizeof(DML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T));

            //set default
            if(pTermButtonMapButtonList->pdata[index] != NULL)
            {
                DML_TERMINAL_BUTTONMAP_BUTTON_CTRL_T* pDmlTermButtonMapButtonData = pTermButtonMapButtonList->pdata[index];
                pDmlTermButtonMapButtonData->updated = false;
                pDmlTermButtonMapButton = &(pDmlTermButtonMapButtonData->dml);
                if ( !pDmlTermButtonMapButton )
                {
                    memset( pDmlTermButtonMapButton, 0, sizeof( DML_TERMINAL_BUTTONMAP_BUTTON ) );
                    pDmlTermButtonMapButton->uInstanceNumber = index + 1;
                }
                pTermButtonMapButtonList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddTermRingerDesc(PDML_TERMINAL_RINGER_DESCRIPTION_LIST_T pTermRingerDescList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_TERMINAL_RINGER_DESCRIPTION  pDmlTermRingerDesc    = NULL;

    if(pTermRingerDescList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pTermRingerDescList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pTermRingerDescList->pdata[index]);
                pTermRingerDescList->pdata[index] = NULL;
                pTermRingerDescList->ulQuantity--;
            }

            //create new
            pTermRingerDescList->pdata[index] = (DML_TERMINAL_RINGER_DESCRIPTION_CTRL_T*) AnscAllocateMemory(sizeof(DML_TERMINAL_RINGER_DESCRIPTION_CTRL_T));

            //set default
            if(pTermRingerDescList->pdata[index] != NULL)
            {
                DML_TERMINAL_RINGER_DESCRIPTION_CTRL_T* pDmlTermRingerDescData = pTermRingerDescList->pdata[index];
                pDmlTermRingerDescData->updated = false;
                pDmlTermRingerDesc = &(pDmlTermRingerDescData->dml);
                if ( !pDmlTermRingerDesc )
                {
                    memset( pDmlTermRingerDesc, 0, sizeof( DML_TERMINAL_RINGER_DESCRIPTION ) );
                    pDmlTermRingerDesc->uInstanceNumber = index + 1;
                }
                pTermRingerDescList->ulQuantity++;
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
ANSC_STATUS TelcoVoiceMgrDmlAddTermRingerPattern(PDML_TERMINAL_RINGER_PATTERN_LIST_T pTermRingerPatternList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_TERMINAL_RINGER_PATTERN   pDmlTermRingerPattern    = NULL;

    if(pTermRingerPatternList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pTermRingerPatternList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pTermRingerPatternList->pdata[index]);
                pTermRingerPatternList->pdata[index] = NULL;
                pTermRingerPatternList->ulQuantity--;
            }

            //create new
            pTermRingerPatternList->pdata[index] = (DML_TERMINAL_RINGER_PATTERN_CTRL_T*) AnscAllocateMemory(sizeof(DML_TERMINAL_RINGER_PATTERN_CTRL_T));

            //set default
            if(pTermRingerPatternList->pdata[index] != NULL)
            {
                DML_TERMINAL_RINGER_PATTERN_CTRL_T* pDmlTermRingerPatternData = pTermRingerPatternList->pdata[index];
                pDmlTermRingerPatternData->updated = false;
                pDmlTermRingerPattern = &(pDmlTermRingerPatternData->dml);
                if ( !pDmlTermRingerPattern )
                {
                    memset( pDmlTermRingerPattern, 0, sizeof( DML_TERMINAL_RINGER_PATTERN ) );
                    pDmlTermRingerPattern->uInstanceNumber = index + 1;
                }
                pTermRingerPatternList->ulQuantity++;
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

ANSC_STATUS TelcoVoiceMgrDmlAddClockSource(PDML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_LIST_T pClkSyncSrcList, int index)
{
    ANSC_STATUS     result  = ANSC_STATUS_FAILURE;
    PDML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE   pDmlClkSyncSrc    = NULL;

    if(pClkSyncSrcList != NULL)
    {
        if(index < TELCOVOICE_DATA_MAX)
        {
            //delete old
            if(pClkSyncSrcList->pdata[index] != NULL)
            {
                AnscFreeMemory((ANSC_HANDLE) pClkSyncSrcList->pdata[index]);
                pClkSyncSrcList->pdata[index] = NULL;
                pClkSyncSrcList->ulQuantity--;
            }

            //create new
            pClkSyncSrcList->pdata[index] = (DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_CTRL_T*) AnscAllocateMemory(sizeof(DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_CTRL_T));

            //set default
            if(pClkSyncSrcList->pdata[index] != NULL)
            {
                DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE_CTRL_T* pDmlClkSyncSrcData = pClkSyncSrcList->pdata[index];
                pDmlClkSyncSrcData->updated = false;
                pDmlClkSyncSrc = &(pDmlClkSyncSrcData->dml);
                if ( !pDmlClkSyncSrc )
                {
                    memset( pDmlClkSyncSrc, 0, sizeof( DML_VOICESERVICE_CLOCKSYNC_CLOCKSOURCE ) );
                    pDmlClkSyncSrc->uInstanceNumber = index + 1;
                }
                pClkSyncSrcList->ulQuantity++;
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
