/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 Sky
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "telcovoicemgr_webconfig.h"

/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/
/* none */
/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
/* none */
/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/
/**
 *  Convert the msgpack map into the WebConfig_LineTable_t structure.
 *
 *  @param e    the entry pointer for line table
 *  @param map  the msgpack map pointer
 *
 *  @return 0 on success, error otherwise
 */
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_LineTable( WebConfig_LineTable_t *e, msgpack_object_map *map )
{
    int  i;
    msgpack_object_kv *p = NULL;

    //Validate param
    if( ( NULL == e ) || ( NULL == map ) )
    {
        CcspTraceError(("%s Invalid Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    p = map->ptr;

    //Validate param
    if( NULL == p )
    {
        CcspTraceError(("%s Invalid msg pack Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0;i < map->size; i++)
    {
        if( MSGPACK_OBJECT_BOOLEAN == p->val.type )
        {
            if( 0 == match(p, "CallWaitingEnable") )
            {
                e->CallWaitingEnable = p->val.via.boolean;
                e->IsCallWaitingEnablePresent = TRUE;
                CcspTraceInfo(("CallWaitingEnable :%d\n", e->CallWaitingEnable));
            }
            else if( 0 == match(p, "MWIEnable") )
            {
                e->MWIEnable = p->val.via.boolean;
                e->IsMWIEnablePresent = TRUE;
                CcspTraceInfo(("MWIEnable :%d\n", e->MWIEnable));
            }
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
            else if( 0 == match(p, "X_RDK-Central_COM_ConferenceCallingEnable") )
            {
                e->X_RDK_Central_COM_ConferenceCallingEnable = p->val.via.boolean;
                e->IsConferenceCallingEnablePresent = TRUE;
                CcspTraceInfo(("X_RDK_Central_COM_ConferenceCallingEnable :%d\n", e->X_RDK_Central_COM_ConferenceCallingEnable));
            }
            else if( 0 == match(p, "X_RDK-Central_COM_HoldEnable") )
            {
                e->X_RDK_Central_COM_HoldEnable = p->val.via.boolean;
                e->IsHoldEnablePresent = TRUE;
                CcspTraceInfo(("X_RDK_Central_COM_HoldEnable :%d\n", e->X_RDK_Central_COM_HoldEnable));
            }
            else if( 0 == match(p, "X_RDK-Central_COM_PhoneCallerIDEnable") )
            {
                e->X_RDK_Central_COM_PhoneCallerIDEnable = p->val.via.boolean;
                e->IsPhoneCallerIDEnablePresent = TRUE;
                CcspTraceInfo(("X_RDK_Central_COM_PhoneCallerIDEnable :%d\n", e->X_RDK_Central_COM_PhoneCallerIDEnable));
            }
#else
            else if( 0 == match(p, "ConferenceCallingEnable") )
            {
                e->ConferenceCallingEnable = p->val.via.boolean;
                e->IsConferenceCallingEnablePresent = TRUE;
                CcspTraceInfo(("ConferenceCallingEnable :%d\n", e->ConferenceCallingEnable));
            }
            else if( 0 == match(p, "HoldEnable") )
            {
                e->HoldEnable = p->val.via.boolean;
                e->IsHoldEnablePresent = TRUE;
                CcspTraceInfo(("HoldEnable :%d\n", e->HoldEnable));
            }
            else if( 0 == match(p, "PhoneCallerIDEnable") )
            {
                e->PhoneCallerIDEnable = p->val.via.boolean;
                e->IsPhoneCallerIDEnablePresent = TRUE;
                CcspTraceInfo(("PhoneCallerIDEnable :%d\n", e->PhoneCallerIDEnable));
            }
            else if( 0 == match(p, "LineEnable") )
            {
                e->LineEnable = p->val.via.boolean;
                e->IsLineEnablePresent = TRUE;
                CcspTraceInfo(("LineEnable :%d\n", e->LineEnable));
            }
#endif
        }
        else if(MSGPACK_OBJECT_STR == p->val.type)
        {
            if( 0 == match(p, "DirectoryNumber") )
            {
                strncpy(e->DirectoryNumber,p->val.via.str.ptr, p->val.via.str.size);
                e->DirectoryNumber[p->val.via.str.size] = '\0';
                e->IsDirectoryNumberPresent = TRUE;
                CcspTraceInfo(("DirectoryNumber :%s\n", e->DirectoryNumber));
            }
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
            else if( 0 == match(p, "LineEnable") )
            {
                strncpy(e->LineEnable,p->val.via.str.ptr, p->val.via.str.size);
                e->LineEnable[p->val.via.str.size] = '\0';
                e->IsLineEnablePresent = TRUE;
                CcspTraceInfo(("LineEnable :%s\n", e->LineEnable));
            }
            else if( 0 == match(p, "SIPAuthUserName") )
            {
                strncpy(e->SIPAuthUserName,p->val.via.str.ptr, p->val.via.str.size);
                e->SIPAuthUserName[p->val.via.str.size] = '\0';
                e->IsSIPAuthUserNamePresent = TRUE;
                CcspTraceInfo(("SIPAuthUserName :%s\n", e->SIPAuthUserName));
            }
            else if( 0 == match(p, "SIPAuthPassword") )
            {
                strncpy(e->SIPAuthPassword,p->val.via.str.ptr, p->val.via.str.size);
                e->SIPAuthPassword[p->val.via.str.size] = '\0';
                e->IsSIPAuthPasswordPresent = TRUE;
                CcspTraceInfo(("SIPAuthPassword :%s\n", e->SIPAuthPassword));
            }
            else if( 0 == match(p, "SIPURI") )
            {
                strncpy(e->SIPURI,p->val.via.str.ptr, p->val.via.str.size);
                e->SIPURI[p->val.via.str.size] = '\0';
                e->IsSIPURIPresent = TRUE;
                CcspTraceInfo(("SIPURI :%s\n", e->SIPURI));
            }
#else
            else if( 0 == match(p, "LineProvider") )
            {
                strncpy(e->LineProvider,p->val.via.str.ptr, p->val.via.str.size);
                e->LineProvider[p->val.via.str.size] = '\0';
                e->IsLineProviderPresent = TRUE;
                CcspTraceInfo(("LineProvider :%s\n", e->LineProvider));
            }
            else if( 0 == match(p, "LineCallingFeatures") )
            {
                strncpy(e->LineCallingFeatures,p->val.via.str.ptr, p->val.via.str.size);
                e->LineCallingFeatures[p->val.via.str.size] = '\0';
                e->IsLineCallingFeaturesPresent = TRUE;
                CcspTraceInfo(("LineCallingFeatures :%s\n", e->LineCallingFeatures));
            }
#endif
        }
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
        else if(MSGPACK_OBJECT_POSITIVE_INTEGER == p->val.type)
        {
            if( 0 == match(p, "ReceiveGain") )
            {
                e->ReceiveGain = (unsigned int)p->val.via.u64;
                e->IsReceiveGainPresent = TRUE;
                CcspTraceInfo(("ReceiveGain :%d\n", e->ReceiveGain));
            }
            else if( 0 == match(p, "TransmitGain") )
            {
                e->TransmitGain = (unsigned int)p->val.via.u64;
                e->IsTransmitGainPresent = TRUE;
                CcspTraceInfo(("TransmitGain :%d\n", e->TransmitGain));
            }
        }
#endif
        p++;
    }
        
    return ANSC_STATUS_SUCCESS; 
}

/**
 *  Convert the msgpack map into the WebConfig_VoiceServiceTable_t structure.
 *
 *  @param e    the entry pointer
 *  @param map  the msgpack map pointer
 *
 *  @return 0 on success, error otherwise
 */
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_VoiceServiceParams( WebConfig_VoiceServiceTable_t *e, msgpack_object_map *map )
{
    int i = 0, j = 0;
    msgpack_object_kv *p = NULL;

    //Validate param
    if( ( NULL == e ) || ( NULL == map ))
    {
        CcspTraceError(("%s Invalid Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    p = map->ptr;

    //Validate param
    if( NULL == p )
    {
        CcspTraceError(("%s Invalid msg pack Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0;i < map->size; i++)
    {
        if( MSGPACK_OBJECT_STR == p->val.type )
        {
            if( 0 == match(p, "OutboundProxy") )
            {
                strncpy(e->OutboundProxy,p->val.via.str.ptr, p->val.via.str.size);
                e->OutboundProxy[p->val.via.str.size] = '\0';
                e->IsOutboundProxyPresent = TRUE;
                CcspTraceInfo(("OutboundProxy :%s\n", e->OutboundProxy));
            }
            else if( 0 == match(p, "ProxyServer") )
            {
                strncpy(e->ProxyServer,p->val.via.str.ptr, p->val.via.str.size);
                e->ProxyServer[p->val.via.str.size] = '\0';
                e->IsProxyServerPresent = TRUE;
                CcspTraceInfo(("ProxyServer :%s\n", e->ProxyServer));
            }
            else if( 0 == match(p, "RegistrarServer") )
            {
                strncpy(e->RegistrarServer,p->val.via.str.ptr, p->val.via.str.size);
                e->RegistrarServer[p->val.via.str.size] = '\0';
                e->IsRegistrarServerPresent = TRUE;
                CcspTraceInfo(("RegistrarServer :%s\n", e->RegistrarServer));
            }
            else if( 0 == match(p, "UserAgentDomain") )
            {
                strncpy(e->UserAgentDomain,p->val.via.str.ptr, p->val.via.str.size);
                e->UserAgentDomain[p->val.via.str.size] = '\0';
                e->IsUserAgentDomainPresent = TRUE;
                CcspTraceInfo(("UserAgentDomain :%s\n", e->UserAgentDomain));
            }
            else if( 0 == match(p, "X_RDK-Central_COM_ConferencingURI") )
            {
                strncpy(e->X_RDK_Central_COM_ConferencingURI,p->val.via.str.ptr, p->val.via.str.size);
                e->X_RDK_Central_COM_ConferencingURI[p->val.via.str.size] = '\0';
                e->IsConferencingURIPresent = TRUE;
                CcspTraceInfo(("X_RDK_Central_COM_ConferencingURI :%s\n", e->X_RDK_Central_COM_ConferencingURI));
            }
            else if( 0 == match(p, "X_RDK-Central_COM_DigitMap") )
            {
                strncpy(e->X_RDK_Central_COM_DigitMap,p->val.via.str.ptr, p->val.via.str.size);
                e->X_RDK_Central_COM_DigitMap[p->val.via.str.size] = '\0';
                e->IsDigitMapPresent = TRUE;
                CcspTraceInfo(("X_RDK_Central_COM_DigitMap :%s\n", e->X_RDK_Central_COM_DigitMap));
            }
            else if( 0 == match(p, "X_RDK-Central_COM_EmergencyDigitMap") )
            {
                strncpy(e->X_RDK_Central_COM_EmergencyDigitMap,p->val.via.str.ptr, p->val.via.str.size);
                e->X_RDK_Central_COM_EmergencyDigitMap[p->val.via.str.size] = '\0';
                e->IsEmergencyDigitMapPresent = TRUE;
                CcspTraceInfo(("X_RDK_Central_COM_EmergencyDigitMap :%s\n", e->X_RDK_Central_COM_EmergencyDigitMap));
            }
        }
        else if( MSGPACK_OBJECT_POSITIVE_INTEGER == p->val.type )
        {
            if( 0 == match(p, "OutboundProxyPort") )
            {
                e->OutboundProxyPort = (unsigned int)p->val.via.u64;
                e->IsOutboundProxyPortPresent = TRUE;
                CcspTraceInfo(("OutboundProxyPort :%d\n", e->OutboundProxyPort));
            }
            else if( 0 == match(p, "ProxyServerPort") )
            {
                e->ProxyServerPort = (unsigned int)p->val.via.u64;
                e->IsProxyServerPortPresent = TRUE;
                CcspTraceInfo(("ProxyServerPort :%d\n", e->ProxyServerPort));
            }
            else if( 0 == match(p, "RegistrarServerPort") )
            {
                e->RegistrarServerPort = (unsigned int)p->val.via.u64;
                e->IsRegistrarServerPortPresent = TRUE;
                CcspTraceInfo(("RegistrarServerPort :%d\n", e->RegistrarServerPort));
            }
            else if( 0 == match(p, "SIPDSCPMark") )
            {
                e->SIPDSCPMark = (unsigned int)p->val.via.u64;
                e->IsSIPDSCPMarkPresent = TRUE;
                CcspTraceInfo(("SIPDSCPMark :%d\n", e->SIPDSCPMark));
            }
            else if( 0 == match(p, "SIPEthernetPriorityMark") )
            {
                e->SIPEthernetPriorityMark = (unsigned int)p->val.via.u64;
                e->IsSIPEthernetPriorityMarkPresent = TRUE;
                CcspTraceInfo(("SIPEthernetPriorityMark :%d\n", e->SIPEthernetPriorityMark));
            }
            else if( 0 == match(p, "RTPDSCPMark") )
            {
                e->RTPDSCPMark = (unsigned int)p->val.via.u64;
                e->IsRTPDSCPMarkPresent = TRUE;
                CcspTraceInfo(("RTPDSCPMark :%d\n", e->RTPDSCPMark));
            }
            else if( 0 == match(p, "RTPEthernetPriorityMark") )
            {
                e->RTPEthernetPriorityMark = (unsigned int)p->val.via.u64;
                e->IsRTPEthernetPriorityMarkPresent = TRUE;
                CcspTraceInfo(("RTPEthernetPriorityMark :%d\n", e->RTPEthernetPriorityMark));
            }
            else if( 0 == match(p, "X_RDK-Central_COM_SDigitTimer") )
            {
                e->X_RDK_Central_COM_SDigitTimer = (unsigned int)p->val.via.u64;
                e->IsSDigitTimerPresent = TRUE;
                CcspTraceInfo(("X_RDK_Central_COM_SDigitTimer :%d\n", e->X_RDK_Central_COM_SDigitTimer));
            }
            else if( 0 == match(p, "X_RDK-Central_COM_ZDigitTimer") )
            {
                e->X_RDK_Central_COM_ZDigitTimer = (unsigned int)p->val.via.u64;
                e->IsZDigitTimerPresent = TRUE;
                CcspTraceInfo(("X_RDK_Central_COM_ZDigitTimer :%d\n", e->X_RDK_Central_COM_ZDigitTimer));
            }
        }
        else if( MSGPACK_OBJECT_BOOLEAN == p->val.type )
        {
            if( 0 == match(p, "X_RDK-Central_COM_NetworkDisconnect") )
            {
                e->X_RDK_Central_COM_NetworkDisconnect = p->val.via.boolean;
                e->IsNetworkDisconnectPresent = TRUE;
                CcspTraceInfo(("X_RDK_Central_COM_NetworkDisconnect :%d\n", e->X_RDK_Central_COM_NetworkDisconnect));
            }
        }
        else if( MSGPACK_OBJECT_ARRAY == p->val.type )
        {
            if( 0 == match(p, "LineTable") )
            {
                e->LineCount = p->val.via.array.size;
                e->pstLineInfo = (WebConfig_LineTable_t *) malloc( sizeof(WebConfig_LineTable_t) * e->LineCount);
                if( NULL == e->pstLineInfo )
                {
                    e->LineCount = 0;
                    return ANSC_STATUS_FAILURE;
                }

                CcspTraceInfo(("Voice Line Count %d\n",e->LineCount));

                memset( e->pstLineInfo, 0, sizeof(WebConfig_LineTable_t) * e->LineCount );
                for( j = 0; j < e->LineCount; j++ )
                {
                    if( MSGPACK_OBJECT_MAP != p->val.via.array.ptr[j].type )
                    {
                        CcspTraceError(("%s %d - Invalid OBJECT \n",__FUNCTION__,__LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    e->pstLineInfo[j].uiLineInstanceNumber = j + 1;
                    if( ANSC_STATUS_SUCCESS != TelcoVoiceMgr_WebConfig_Process_LineTable(&e->pstLineInfo[j], &p->val.via.array.ptr[j].via.map) )
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }
            }
        }

        p++;
    }
        
    return ANSC_STATUS_SUCCESS;
}
#else
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_VoiceServiceParams( WebConfig_VoiceServiceTable_t *e, msgpack_object_map *map )
{
    int i = 0, j = 0;
    msgpack_object_kv *p = NULL;

    //Validate param
    if( ( NULL == e ) || ( NULL == map ))
    {
        CcspTraceError(("%s Invalid Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    p = map->ptr;

    //Validate param
    if( NULL == p )
    {
        CcspTraceError(("%s Invalid msg pack Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0;i < map->size; i++)
    {
        if( MSGPACK_OBJECT_ARRAY == p->val.type )
        {
            if( 0 == match(p, "SipNwTable") )
            {
                e->SipNwCount = p->val.via.array.size;
                e->pstSipNwInfo = (WebConfig_SipNwTable_t *) malloc( sizeof(WebConfig_SipNwTable_t) * e->SipNwCount);
                if( NULL == e->pstSipNwInfo )
                {
                    e->SipNwCount = 0;
                    return ANSC_STATUS_FAILURE;
                }

                CcspTraceInfo(("Voice Sip Network Count %d\n",e->SipNwCount));

                memset( e->pstSipNwInfo, 0, sizeof(WebConfig_SipNwTable_t) * e->SipNwCount );
                for( j = 0; j < e->SipNwCount; j++ )
                {
                    if( MSGPACK_OBJECT_MAP != p->val.via.array.ptr[j].type )
                    {
                        CcspTraceError(("%s %d - Invalid OBJECT \n",__FUNCTION__,__LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    e->pstSipNwInfo[j].uiSipNwInstanceNumber = j + 1;
                    if( ANSC_STATUS_SUCCESS != TelcoVoiceMgr_WebConfig_Process_SipNetworkTable(&e->pstSipNwInfo[j], &p->val.via.array.ptr[j].via.map) )
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }
            }
            else if( 0 == match(p, "SipClientTable") )
            {
                e->SipClientCount = p->val.via.array.size;
                e->pstSipClientInfo = (WebConfig_SipClientTable_t *) malloc( sizeof(WebConfig_SipClientTable_t) * e->SipClientCount);
                if( NULL == e->pstSipClientInfo )
                {
                    e->SipClientCount = 0;
                    return ANSC_STATUS_FAILURE;
                }

                CcspTraceInfo(("Voice Sip Client Count %d\n",e->SipClientCount));

                memset( e->pstSipClientInfo, 0, sizeof(WebConfig_SipClientTable_t) * e->SipClientCount );
                for( j = 0; j < e->SipClientCount; j++ )
                {
                    if( MSGPACK_OBJECT_MAP != p->val.via.array.ptr[j].type )
                    {
                        CcspTraceError(("%s %d - Invalid OBJECT \n",__FUNCTION__,__LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    e->pstSipClientInfo[j].uiSipClientInstanceNumber = j + 1;
                    if( ANSC_STATUS_SUCCESS != TelcoVoiceMgr_WebConfig_Process_SipClientTable(&e->pstSipClientInfo[j], &p->val.via.array.ptr[j].via.map) )
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }
            }
            else if( 0 == match(p, "VoIPTable") )
            {
                e->VoIPCount = p->val.via.array.size;
                e->pstVoIPInfo = (WebConfig_VoIPTable_t *) malloc( sizeof(WebConfig_VoIPTable_t) * e->VoIPCount);
                if( NULL == e->pstVoIPInfo )
                {
                    e->VoIPCount = 0;
                    return ANSC_STATUS_FAILURE;
                }

                CcspTraceInfo(("VoIP Profile Count %d\n",e->VoIPCount));

                memset( e->pstVoIPInfo, 0, sizeof(WebConfig_VoIPTable_t) * e->VoIPCount );
                for( j = 0; j < e->VoIPCount; j++ )
                {
                    if( MSGPACK_OBJECT_MAP != p->val.via.array.ptr[j].type )
                    {
                        CcspTraceError(("%s %d - Invalid OBJECT \n",__FUNCTION__,__LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    e->pstVoIPInfo[j].uiProfileInstanceNumber = j + 1;
                    if( ANSC_STATUS_SUCCESS != TelcoVoiceMgr_WebConfig_Process_ProfileTable(&e->pstVoIPInfo[j], &p->val.via.array.ptr[j].via.map) )
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }
            }
            else if( 0 == match(p, "LineTable") )
            {
                e->LineCount = p->val.via.array.size;
                e->pstLineInfo = (WebConfig_LineTable_t *) malloc( sizeof(WebConfig_LineTable_t) * e->LineCount);
                if( NULL == e->pstLineInfo )
                {
                    e->LineCount = 0;
                    return ANSC_STATUS_FAILURE;
                }

                CcspTraceInfo(("Voice Line Count %d\n",e->LineCount));

                memset( e->pstLineInfo, 0, sizeof(WebConfig_LineTable_t) * e->LineCount );
                for( j = 0; j < e->LineCount; j++ )
                {
                    if( MSGPACK_OBJECT_MAP != p->val.via.array.ptr[j].type )
                    {
                        CcspTraceError(("%s %d - Invalid OBJECT \n",__FUNCTION__,__LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    e->pstLineInfo[j].uiLineInstanceNumber = j + 1;
                    if( ANSC_STATUS_SUCCESS != TelcoVoiceMgr_WebConfig_Process_LineTable(&e->pstLineInfo[j], &p->val.via.array.ptr[j].via.map) )
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }
            }
            else if( 0 == match(p, "POTSFxsTable") )
            {
                e->FXSCount = p->val.via.array.size;
                e->pstPOTSFxsInfo = (WebConfig_POTSFxsTable_t *) malloc( sizeof(WebConfig_POTSFxsTable_t) * e->FXSCount);
                if( NULL == e->pstPOTSFxsInfo )
                {
                    e->FXSCount = 0;
                    return ANSC_STATUS_FAILURE;
                }

                CcspTraceInfo(("Voice FXS Count %d\n",e->FXSCount));

                memset( e->pstPOTSFxsInfo, 0, sizeof(WebConfig_POTSFxsTable_t) * e->FXSCount );
                for( j = 0; j < e->FXSCount; j++ )
                {
                    if( MSGPACK_OBJECT_MAP != p->val.via.array.ptr[j].type )
                    {
                        CcspTraceError(("%s %d - Invalid OBJECT \n",__FUNCTION__,__LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    e->pstPOTSFxsInfo[j].uiFXSInstanceNumber = j + 1;
                    if( ANSC_STATUS_SUCCESS != TelcoVoiceMgr_WebConfig_Process_POTSFxsTable(&e->pstPOTSFxsInfo[j], &p->val.via.array.ptr[j].via.map) )
                    {
                        return ANSC_STATUS_FAILURE;
                    }
                }
            }
        }

        p++;
    }
    return ANSC_STATUS_SUCCESS;
}

/**
 *  Convert the msgpack map into the WebConfig_SipNwTable_t structure.
 *
 *  @param e    the entry pointer for line table
 *  @param map  the msgpack map pointer
 *
 *  @return 0 on success, error otherwise
 */
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_SipNetworkTable( WebConfig_SipNwTable_t *e, msgpack_object_map *map )
{
    int  i;
    msgpack_object_kv *p = NULL;

    //Validate param
    if( ( NULL == e ) || ( NULL == map ) )
    {
        CcspTraceError(("%s Invalid Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    p = map->ptr;

    //Validate param
    if( NULL == p )
    {
        CcspTraceError(("%s Invalid msg pack Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0;i < map->size; i++)
    {
        if( MSGPACK_OBJECT_STR == p->val.type )
        {
            if( 0 == match(p, "OutboundProxy") )
            {
                strncpy(e->OutboundProxy,p->val.via.str.ptr, p->val.via.str.size);
                e->OutboundProxy[p->val.via.str.size] = '\0';
                e->IsOutboundProxyPresent = TRUE;
                CcspTraceInfo(("OutboundProxy :%s\n", e->OutboundProxy));
            }
            else if( 0 == match(p, "ProxyServer") )
            {
                strncpy(e->ProxyServer,p->val.via.str.ptr, p->val.via.str.size);
                e->ProxyServer[p->val.via.str.size] = '\0';
                e->IsProxyServerPresent = TRUE;
                CcspTraceInfo(("ProxyServer :%s\n", e->ProxyServer));
            }
            else if( 0 == match(p, "ProxyServerTransport") )
            {
                strncpy(e->ProxyServerTransport,p->val.via.str.ptr, p->val.via.str.size);
                e->ProxyServerTransport[p->val.via.str.size] = '\0';
                e->IsProxyServerTransportPresent = TRUE;
                CcspTraceInfo(("ProxyServerTransport :%s\n", e->ProxyServerTransport));
            }
            else if( 0 == match(p, "RegistrarServer") )
            {
                strncpy(e->RegistrarServer,p->val.via.str.ptr, p->val.via.str.size);
                e->RegistrarServer[p->val.via.str.size] = '\0';
                e->IsRegistrarServerPresent = TRUE;
                CcspTraceInfo(("RegistrarServer :%s\n", e->RegistrarServer));
            }
            else if( 0 == match(p, "RegistrarServerTransport") )
            {
                strncpy(e->RegistrarServerTransport,p->val.via.str.ptr, p->val.via.str.size);
                e->RegistrarServerTransport[p->val.via.str.size] = '\0';
                e->IsRegistrarServerTransportPresent = TRUE;
                CcspTraceInfo(("RegistrarServerTransport :%s\n", e->RegistrarServerTransport));
            }
            else if( 0 == match(p, "UserAgentDomain") )
            {
                strncpy(e->UserAgentDomain,p->val.via.str.ptr, p->val.via.str.size);
                e->UserAgentDomain[p->val.via.str.size] = '\0';
                e->IsUserAgentDomainPresent = TRUE;
                CcspTraceInfo(("UserAgentDomain :%s\n", e->UserAgentDomain));
            }
            else if( 0 == match(p, "UserAgentTransport") )
            {
                strncpy(e->UserAgentTransport,p->val.via.str.ptr, p->val.via.str.size);
                e->UserAgentTransport[p->val.via.str.size] = '\0';
                e->IsUserAgentTransportPresent = TRUE;
                CcspTraceInfo(("UserAgentTransport :%s\n", e->UserAgentTransport));
            }
            else if( 0 == match(p, "SIPNetworkVoIPProfile") )
            {
                strncpy(e->SIPNetworkVoIPProfile,p->val.via.str.ptr, p->val.via.str.size);
                e->SIPNetworkVoIPProfile[p->val.via.str.size] = '\0';
                e->IsSIPNetworkVoIPProfilePresent = TRUE;
                CcspTraceInfo(("SIPNetworkVoIPProfile :%s\n", e->SIPNetworkVoIPProfile));
            }
            else if( 0 == match(p, "ConferencingURI") )
            {
                strncpy(e->ConferencingURI,p->val.via.str.ptr, p->val.via.str.size);
                e->ConferencingURI[p->val.via.str.size] = '\0';
                e->IsConferencingURIPresent = TRUE;
                CcspTraceInfo(("ConferencingURI :%s\n", e->ConferencingURI));
            }
        }
        else if( MSGPACK_OBJECT_POSITIVE_INTEGER == p->val.type )
        {
            if( 0 == match(p, "OutboundProxyPort") )
            {
                e->OutboundProxyPort = (unsigned int)p->val.via.u64;
                e->IsOutboundProxyPortPresent = TRUE;
                CcspTraceInfo(("OutboundProxyPort :%d\n", e->OutboundProxyPort));
            }
            else if( 0 == match(p, "ProxyServerPort") )
            {
                e->ProxyServerPort = (unsigned int)p->val.via.u64;
                e->IsProxyServerPortPresent = TRUE;
                CcspTraceInfo(("ProxyServerPort :%d\n", e->ProxyServerPort));
            }
            else if( 0 == match(p, "UserAgentPort") )
            {
                e->UserAgentPort = (unsigned int)p->val.via.u64;
                e->IsUserAgentPortPresent = TRUE;
                CcspTraceInfo(("UserAgentPort :%d\n", e->UserAgentPort));
            }
            else if( 0 == match(p, "RegistrarServerPort") )
            {
                e->RegistrarServerPort = (unsigned int)p->val.via.u64;
                e->IsRegistrarServerPortPresent = TRUE;
                CcspTraceInfo(("RegistrarServerPort :%d\n", e->RegistrarServerPort));
            }
            else if( 0 == match(p, "SIPDSCPMark") )
            {
                e->SIPDSCPMark = (unsigned int)p->val.via.u64;
                e->IsSIPDSCPMarkPresent = TRUE;
                CcspTraceInfo(("SIPDSCPMark :%d\n", e->SIPDSCPMark));
            }
            else if( 0 == match(p, "SIPEthernetPriorityMark") )
            {
                e->SIPEthernetPriorityMark = (unsigned int)p->val.via.u64;
                e->IsSIPEthernetPriorityMarkPresent = TRUE;
                CcspTraceInfo(("SIPEthernetPriorityMark :%d\n", e->SIPEthernetPriorityMark));
            }
        }
        else if( MSGPACK_OBJECT_BOOLEAN == p->val.type )
        {
            if( 0 == match(p, "SIPNetworkEnable") )
            {
                e->SIPNetworkEnable = p->val.via.boolean;
                e->IsSIPNetworkEnablePresent = TRUE;
                CcspTraceInfo(("SIPNetworkEnable :%d\n", e->SIPNetworkEnable));
            }
            if( 0 == match(p, "NetworkDisconnect") )
            {
                e->NetworkDisconnect = p->val.via.boolean;
                e->IsNetworkDisconnectPresent = TRUE;
                CcspTraceInfo(("NetworkDisconnect :%d\n", e->NetworkDisconnect));
            }
            if( 0 == match(p, "PRACKRequired") )
            {
                e->PRACKRequired = p->val.via.boolean;
                e->IsPRACKRequiredPresent = TRUE;
                CcspTraceInfo(("PRACKRequired :%d\n", e->PRACKRequired));
            }
        }
        p++;
    }

    return ANSC_STATUS_SUCCESS;
}

/**
 *  Convert the msgpack map into the WebConfig_SipClientTable_t structure.
 *
 *  @param e    the entry pointer for line table
 *  @param map  the msgpack map pointer
 *
 *  @return 0 on success, error otherwise
 */
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_SipClientTable( WebConfig_SipClientTable_t *e, msgpack_object_map *map )
{
    int  i;
    msgpack_object_kv *p = NULL;

    //Validate param
    if( ( NULL == e ) || ( NULL == map ) )
    {
        CcspTraceError(("%s Invalid Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    p = map->ptr;

    //Validate param
    if( NULL == p )
    {
        CcspTraceError(("%s Invalid msg pack Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0;i < map->size; i++)
    {
        if(MSGPACK_OBJECT_STR == p->val.type)
        {
            if( 0 == match(p, "SIPAuthUserName") )
            {
                strncpy(e->SIPAuthUserName,p->val.via.str.ptr, p->val.via.str.size);
                e->SIPAuthUserName[p->val.via.str.size] = '\0';
                e->IsSIPAuthUserNamePresent = TRUE;
                CcspTraceInfo(("SIPAuthUserName :%s\n", e->SIPAuthUserName));
            }
            else if( 0 == match(p, "SIPAuthPassword") )
            {
                strncpy(e->SIPAuthPassword,p->val.via.str.ptr, p->val.via.str.size);
                e->SIPAuthPassword[p->val.via.str.size] = '\0';
                e->IsSIPAuthPasswordPresent = TRUE;
                CcspTraceInfo(("SIPAuthPassword :%s\n", e->SIPAuthPassword));
            }
            else if( 0 == match(p, "SIPURI") )
            {
                strncpy(e->SIPURI,p->val.via.str.ptr, p->val.via.str.size);
                e->SIPURI[p->val.via.str.size] = '\0';
                e->IsSIPURIPresent = TRUE;
                CcspTraceInfo(("SIPURI :%s\n", e->SIPURI));
            }
            else if( 0 == match(p, "SIPClientNetwork") )
            {
                strncpy(e->SIPClientNetwork,p->val.via.str.ptr, p->val.via.str.size);
                e->SIPClientNetwork[p->val.via.str.size] = '\0';
                e->IsSIPClientNetworkPresent = TRUE;
                CcspTraceInfo(("SIPClientNetwork :%s\n", e->SIPClientNetwork));
            }
        }
        else if( MSGPACK_OBJECT_BOOLEAN == p->val.type )
        {
            if( 0 == match(p, "SIPClientEnable") )
            {
                e->SIPClientEnable = p->val.via.boolean;
                e->IsSIPClientEnablePresent = TRUE;
                CcspTraceInfo(("SIPClientEnable :%d\n", e->SIPClientEnable));
            }
        }
        p++;
    }

    return ANSC_STATUS_SUCCESS;
}


/**
 *  Convert the msgpack map into the WebConfig_VoIPTable_t structure.
 *
 *  @param e    the entry pointer for line table
 *  @param map  the msgpack map pointer
 *
 *  @return 0 on success, error otherwise
 */
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_ProfileTable( WebConfig_VoIPTable_t *e, msgpack_object_map *map )
{
    int  i;
    msgpack_object_kv *p = NULL;

    //Validate param
    if( ( NULL == e ) || ( NULL == map ) )
    {
        CcspTraceError(("%s Invalid Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    p = map->ptr;

    //Validate param
    if( NULL == p )
    {
        CcspTraceError(("%s Invalid msg pack Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0;i < map->size; i++)
    {
        if( MSGPACK_OBJECT_POSITIVE_INTEGER == p->val.type )
        {
            if( 0 == match(p, "RTPDSCPMark") )
            {
                e->RTPDSCPMark = (unsigned int)p->val.via.u64;
                e->IsRTPDSCPMarkPresent = TRUE;
                CcspTraceInfo(("RTPDSCPMark :%d\n", e->RTPDSCPMark));
            }
            else if( 0 == match(p, "RTPEthernetPriorityMark") )
            {
                e->RTPEthernetPriorityMark = (unsigned int)p->val.via.u64;
                e->IsRTPEthernetPriorityMarkPresent = TRUE;
                CcspTraceInfo(("RTPEthernetPriorityMark :%d\n", e->RTPEthernetPriorityMark));
            }
            else if( 0 == match(p, "SDigitTimer") )
            {
                e->SDigitTimer = (unsigned int)p->val.via.u64;
                e->IsSDigitTimerPresent = TRUE;
                CcspTraceInfo(("SDigitTimer :%d\n", e->SDigitTimer));
            }
            else if( 0 == match(p, "ZDigitTimer") )
            {
                e->ZDigitTimer = (unsigned int)p->val.via.u64;
                e->IsZDigitTimerPresent = TRUE;
                CcspTraceInfo(("ZDigitTimer :%d\n", e->ZDigitTimer));
            }
        }
	else if( MSGPACK_OBJECT_STR == p->val.type )
        {
            if( 0 == match(p, "DigitMap") )
            {
                strncpy(e->DigitMap,p->val.via.str.ptr, p->val.via.str.size);
                e->DigitMap[p->val.via.str.size] = '\0';
                e->IsDigitMapPresent = TRUE;
                CcspTraceInfo(("DigitMap :%s\n", e->DigitMap));
            }
            else if( 0 == match(p, "EmergencyDigitMap") )
            {
                strncpy(e->EmergencyDigitMap,p->val.via.str.ptr, p->val.via.str.size);
                e->EmergencyDigitMap[p->val.via.str.size] = '\0';
                e->IsEmergencyDigitMapPresent = TRUE;
                CcspTraceInfo(("EmergencyDigitMap :%s\n", e->EmergencyDigitMap));
            }
        }
        else if( MSGPACK_OBJECT_BOOLEAN == p->val.type )
        {
            if( 0 == match(p, "ProfileEnable") )
            {
                e->ProfileEnable = p->val.via.boolean;
                e->IsProfileEnablePresent = TRUE;
                CcspTraceInfo(("ProfileEnable :%d\n", e->ProfileEnable));
            }
        }
        p++;
    }
    
    return ANSC_STATUS_SUCCESS;
}

/**
 *  Convert the msgpack map into the WebConfig_POTSFxsTable_t structure.
 *
 *  @param e    the entry pointer for line table
 *  @param map  the msgpack map pointer
 *
 *  @return 0 on success, error otherwise
 */
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_POTSFxsTable( WebConfig_POTSFxsTable_t *e, msgpack_object_map *map )
{
    int  i;
    msgpack_object_kv *p = NULL;

    //Validate param
    if( ( NULL == e ) || ( NULL == map ) )
    {
        CcspTraceError(("%s Invalid Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    p = map->ptr;

    //Validate param
    if( NULL == p )
    {
        CcspTraceError(("%s Invalid msg pack Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    for(i = 0;i < map->size; i++)
    {
        if(MSGPACK_OBJECT_POSITIVE_INTEGER == p->val.type)
        {
            if( 0 == match(p, "ReceiveGain") )
            {
                e->ReceiveGain = (unsigned int)p->val.via.u64;
                e->IsReceiveGainPresent = TRUE;
                CcspTraceInfo(("ReceiveGain :%d\n", e->ReceiveGain));
            }
            else if( 0 == match(p, "TransmitGain") )
            {
                e->TransmitGain = (unsigned int)p->val.via.u64;
                e->IsTransmitGainPresent = TRUE;
                CcspTraceInfo(("TransmitGain :%d\n", e->TransmitGain));
            }
        }
        else if( MSGPACK_OBJECT_BOOLEAN == p->val.type )
        {
            if( 0 == match(p, "FXSEnable") )
            {
                e->FXSEnable = p->val.via.boolean;
                e->IsFXSEnablePresent = TRUE;
                CcspTraceInfo(("FXSEnable :%d\n", e->FXSEnable));
            }
        }
        p++;
    }
  
    return ANSC_STATUS_SUCCESS;
}
#endif


/**
 *  Convert the msgpack obj into the WebConfig_VoiceServiceTable_t structure.
 *
 *  @param obj    the entry pointer
 *  @param pWebConfig  the telcovoice webconfig pointer
 *
 *  @return 0 on success, error otherwise
 */
ANSC_STATUS TelcoVoiceMgr_WebConfig_Process_TelcoVoipParams(msgpack_object obj, TelcoVoice_WebConfig_t *pWebConfig) 
{
    int i, j;
    msgpack_object_kv *p = NULL;

    //Validate param
    if( NULL == pWebConfig )
    {
        CcspTraceError(("%s Invalid Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    p = obj.via.map.ptr;

    //Validate param
    if( NULL == p )
    {
        CcspTraceError(("%s Invalid msg pack Pointer\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    CcspTraceInfo(("*************** TelcoVoice WebConfig Data *******************\n"));

    for(i = 0;i < obj.via.map.size;i++) {
        if ( 0 == match(p, "Spec") )
        {
            if (p->val.type == MSGPACK_OBJECT_STR) {
                strncpy(pWebConfig->spec,p->val.via.str.ptr, p->val.via.str.size);
                pWebConfig->spec[p->val.via.str.size] = '\0';
                CcspTraceInfo(("Spec :%s\n", pWebConfig->spec));
                if(strcmp(pWebConfig->spec, CURRENT_DML_VERSION))
                {
                    CcspTraceInfo(("Version mismatch Error :: Current DML Version : %s , Spec :%s\n", CURRENT_DML_VERSION, pWebConfig->spec));
                    return ANSC_STATUS_FAILURE;
                }
            }
        }
        else if ( 0 == match(p, "VoiceServiceTable") )
        {
            if( MSGPACK_OBJECT_ARRAY == p->val.type )
            {
                pWebConfig->ServiceCount = p->val.via.array.size;
                pWebConfig->pstServiceInfo = (WebConfig_VoiceServiceTable_t *) malloc( sizeof(WebConfig_VoiceServiceTable_t) * pWebConfig->ServiceCount);
                if( NULL == pWebConfig->pstServiceInfo )
                {
                    pWebConfig->ServiceCount = 0;
                    CcspTraceError(("%s %d - Failed to allocate memory \n",__FUNCTION__,__LINE__));
                    return ANSC_STATUS_FAILURE;
                }

                CcspTraceInfo(("Voice Service Count %d\n",pWebConfig->ServiceCount));

                memset( pWebConfig->pstServiceInfo, 0, sizeof(WebConfig_VoiceServiceTable_t) * pWebConfig->ServiceCount );
                for( j = 0; j < pWebConfig->ServiceCount; j++ )
                {
                    if( MSGPACK_OBJECT_MAP != p->val.via.array.ptr[j].type )
                    {
                        CcspTraceError(("%s %d - Invalid OBJECT \n",__FUNCTION__,__LINE__));
                        return ANSC_STATUS_FAILURE;
                    }

                    pWebConfig->pstServiceInfo[j].uiServiceInstanceNumber = j + 1;
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
                    pWebConfig->pstServiceInfo[j].uiProfileInstanceNumber = j + 1;
#endif
                    if( ANSC_STATUS_SUCCESS != TelcoVoiceMgr_WebConfig_Process_VoiceServiceParams(&pWebConfig->pstServiceInfo[j], &p->val.via.array.ptr[j].via.map) )
                    {
                        return ANSC_STATUS_FAILURE;
                    }

                }
            }
        }

        ++p;
    }

    CcspTraceInfo(("*************************************************************\n"));

    return ANSC_STATUS_SUCCESS;
}
