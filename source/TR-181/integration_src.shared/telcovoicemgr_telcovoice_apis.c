
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

#include <assert.h>
#include <stdlib.h>
#include "ansc_status.h"
#include "telcovoicemgr_dml_hal.h"
#include <sysevent.h>
#include <syscfg.h>
#include "ccsp_trace.h"
#include "ccsp_syslog.h"
#include "ccsp_message_bus.h"
#include "ccsp_base_api.h"
#include "telcovoicemgr_dml_json_cfg_init.h"
#include "telcovoicemgr_telcovoice_apis.h"
#include "telcovoicemgr_webconfig.h"
#include "telcovoicemgr_dml_apis.h"

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_services_apis_v2.h"
#else
#include "telcovoicemgr_services_apis_v1.h"
#endif //FEATURE_RDKB_VOICE_DM_TR104_V2

/**
 * Execute blob request callback handler
 *
 * @param  Data  Pointer to structure holding telcovoice parameters
 *
 * returns pErr structure populated with return code and error string incase of failure
 *
 */
pErr TelcoVoiceMgr_Process_Webconfig_Request(void *Data)
{
    pErr execRetVal = NULL;
/* TO DO: Implement Webconfig for TR104V2  (RDKB-36794) */
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
    int i, j;
    unsigned int uVsIndex = 0, uVpIndex = 0, uVlIndex = 0;
    unsigned long ulInstance;

    if (Data == NULL) {
        CcspTraceError(("%s: Input Data is NULL\n",__FUNCTION__));
        return execRetVal;
    }

    TelcoVoice_WebConfig_t *pWebConfig = (TelcoVoice_WebConfig_t *) Data;

    execRetVal = (pErr ) malloc (sizeof(Err));
    if (execRetVal == NULL )
    {
        CcspTraceError(("%s : Malloc failed for pErr\n",__FUNCTION__));
        return execRetVal;
    }

    memset(execRetVal,0,(sizeof(Err)));
    execRetVal->ErrorCode = BLOB_EXEC_SUCCESS;

    /* Apply all parsed values into HAL */
    for( i = 0; i < pWebConfig->ServiceCount; i++ )
    {
        WebConfig_VoiceServiceTable_t *pServiceCfg = &(pWebConfig->pstServiceInfo[i]);
        TELCOVOICEMGR_DML_DATA        *pTelcoVoiceMgrDmlData = NULL;

        DML_VOICE_SERVICE_CTRL_T      *pTelcoVoiceSrvCtrl    = NULL;
        TELCOVOICEMGR_DML_VOICESERVICE *pVoiceService        = NULL;

        DML_PROFILE_CTRL_T             *pTelcoVoiceProfCtrl  = NULL;
        TELCOVOICEMGR_DML_VOICEPROFILE *pVoiceProfile        = NULL;

        //Index Assignment
        uVsIndex = pServiceCfg->uiServiceInstanceNumber;
        uVpIndex = pServiceCfg->uiProfileInstanceNumber;

        //Get Voice Service DML structure
        pTelcoVoiceSrvCtrl = (DML_VOICE_SERVICE_CTRL_T*) VoiceService_GetEntry(NULL, (uVsIndex - 1), &ulInstance);

        if( NULL == pTelcoVoiceSrvCtrl )
        {
            CcspTraceError(("%s: GetEntry of VoiceService failed Index:%d\n",__FUNCTION__,(uVsIndex - 1)));
            execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return execRetVal;
        }

        pVoiceService = &(pTelcoVoiceSrvCtrl->dml);

        if( NULL == pVoiceService )
        {
            CcspTraceError(("%s: Invalid VoiceService structure pointer. Index:%d\n",__FUNCTION__,(uVsIndex - 1)));
            execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return execRetVal;
        }

        //Get Voice Profile DML structure
        pTelcoVoiceProfCtrl = (DML_PROFILE_CTRL_T*) VoiceProfile_GetEntry(pTelcoVoiceSrvCtrl, (uVpIndex - 1), &ulInstance);

        if( NULL == pTelcoVoiceProfCtrl )
        {
            CcspTraceError(("%s: GetEntry of VoiceProfile failed Index:%d\n",__FUNCTION__,(uVpIndex - 1)));
            execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return execRetVal;
        }

        pVoiceProfile = &(pTelcoVoiceProfCtrl->dml);

        if( NULL == pVoiceProfile )
        {
            CcspTraceError(("%s: Invalid VoiceProfile structure pointer. Index:%d\n",__FUNCTION__,(uVpIndex - 1)));
            execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
            return execRetVal;
        }

        //DML Resource Locked
        pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();

        if(TelcoVoiceMgrDmlSetOutboundProxy(uVsIndex, uVpIndex, pServiceCfg->OutboundProxy) == ANSC_STATUS_SUCCESS)
        {
            snprintf(pVoiceProfile->SIPObj.OutboundProxy, sizeof(pVoiceProfile->SIPObj.OutboundProxy), "%s", pServiceCfg->OutboundProxy);
        }

        if(TelcoVoiceMgrDmlSetProxyServer(uVsIndex, uVpIndex, pServiceCfg->ProxyServer) == ANSC_STATUS_SUCCESS)
        {
            snprintf(pVoiceProfile->SIPObj.ProxyServer, sizeof(pVoiceProfile->SIPObj.ProxyServer), "%s", pServiceCfg->ProxyServer);
        }

        if(TelcoVoiceMgrDmlSetRegistrarServer(uVsIndex, uVpIndex, pServiceCfg->RegistrarServer) == ANSC_STATUS_SUCCESS)
        {
            snprintf(pVoiceProfile->SIPObj.RegistrarServer, sizeof(pVoiceProfile->SIPObj.RegistrarServer), "%s", pServiceCfg->RegistrarServer);
        }

        if(TelcoVoiceMgrDmlSetUserAgentDomain(uVsIndex, uVpIndex, pServiceCfg->UserAgentDomain) == ANSC_STATUS_SUCCESS)
        {
            snprintf(pVoiceProfile->SIPObj.UserAgentDomain, sizeof(pVoiceProfile->SIPObj.UserAgentDomain), "%s", pServiceCfg->UserAgentDomain);
        }

        if(TelcoVoiceMgrDmlSetConferencingURI(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_ConferencingURI) == ANSC_STATUS_SUCCESS)
        {
            snprintf(pVoiceProfile->SIPObj.ConferencingURI, sizeof(pVoiceProfile->SIPObj.ConferencingURI), "%s", pServiceCfg->X_RDK_Central_COM_ConferencingURI);
        }

        if(TelcoVoiceMgrDmlSetDigitMap(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_DigitMap, "X_RDK-Central_COM_DigitMap") == ANSC_STATUS_SUCCESS)
        {
            snprintf(pVoiceProfile->DigitMap, sizeof(pVoiceProfile->DigitMap), "%s", pServiceCfg->X_RDK_Central_COM_DigitMap);
        }

        if(TelcoVoiceMgrDmlSetDigitMap(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_EmergencyDigitMap, "X_RDK-Central_COM_EmergencyDigitMap") == ANSC_STATUS_SUCCESS)
        {
            snprintf(pVoiceProfile->EmergencyDigitMap, sizeof(pVoiceProfile->EmergencyDigitMap), "%s", pServiceCfg->X_RDK_Central_COM_EmergencyDigitMap);
        }

        if(TelcoVoiceMgrDmlSetOutboundProxyPort(uVsIndex, uVpIndex, pServiceCfg->OutboundProxyPort) == ANSC_STATUS_SUCCESS)
        {
            pVoiceProfile->SIPObj.OutboundProxyPort = pServiceCfg->OutboundProxyPort;
        }

        if(TelcoVoiceMgrDmlSetProxyServerPort(uVsIndex, uVpIndex, pServiceCfg->ProxyServerPort) == ANSC_STATUS_SUCCESS)
        {
            pVoiceProfile->SIPObj.ProxyServerPort = pServiceCfg->ProxyServerPort;
        }

        if(TelcoVoiceMgrDmlSetRegistrarServerPort(uVsIndex, uVpIndex, pServiceCfg->RegistrarServerPort) == ANSC_STATUS_SUCCESS)
        {
            pVoiceProfile->SIPObj.RegistrarServerPort = pServiceCfg->RegistrarServerPort;
        }

        if(TelcoVoiceMgrDmlSetSipEthernetPriorityMark(uVsIndex, uVpIndex, pServiceCfg->SIPEthernetPriorityMark) == ANSC_STATUS_SUCCESS)
        {
            pVoiceProfile->SIPObj.EthernetPriorityMark = pServiceCfg->SIPEthernetPriorityMark;
        }

        pVoiceProfile->SIPObj.DSCPMark  =  pServiceCfg->SIPDSCPMark;
        (void)storeObjectInteger(uVsIndex, uVpIndex, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "DSCPMark", pVoiceProfile->SIPObj.DSCPMark);
        
        pVoiceProfile->RTPObj.DSCPMark  =  pServiceCfg->RTPDSCPMark;
        (void)storeObjectInteger(uVsIndex, uVpIndex, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "DSCPMark", pVoiceProfile->RTPObj.DSCPMark);

        if(TelcoVoiceMgrDmlSetRtpEthernetPriorityMark(uVsIndex, uVpIndex, pServiceCfg->RTPEthernetPriorityMark) == ANSC_STATUS_SUCCESS)
        {
            pVoiceProfile->RTPObj.EthernetPriorityMark = pServiceCfg->RTPEthernetPriorityMark;
        }

        if(TelcoVoiceMgrDmlSetSDigitTimer(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_SDigitTimer) == ANSC_STATUS_SUCCESS)
        {
            pVoiceProfile->SDigitTimer = pServiceCfg->X_RDK_Central_COM_SDigitTimer;
        }

        if(TelcoVoiceMgrDmlSetZDigitTimer(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_ZDigitTimer) == ANSC_STATUS_SUCCESS)
        {
            pVoiceProfile->ZDigitTimer = pServiceCfg->X_RDK_Central_COM_ZDigitTimer;
        }

        if(TelcoVoiceMgrDmlSetNetworkDisconnect(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_NetworkDisconnect) == ANSC_STATUS_SUCCESS)
        {
            pVoiceProfile->SIPObj.NetworkDisconnect = pServiceCfg->X_RDK_Central_COM_NetworkDisconnect;
        }

        //DML Resource Unlocked
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    
        for( j = 0; j < pServiceCfg->LineCount; j++ )
        {
            WebConfig_LineTable_t *pstLineCfg = &(pServiceCfg->pstLineInfo[j]);
            DML_LINE_CTRL_T       *pTelcoVoiceLineCtrl  = NULL;
            TELCOVOICEMGR_DML_LINE *pDMLLine            = NULL;
            unsigned char           bLineEnable         = FALSE;

            uVlIndex = pstLineCfg->uiLineInstanceNumber;

            //Get Voice Profile Line DML structure
            pTelcoVoiceLineCtrl = (DML_LINE_CTRL_T*) Line_GetEntry(pTelcoVoiceProfCtrl, (uVlIndex - 1), &ulInstance);

            if( NULL == pTelcoVoiceLineCtrl )
            {
                CcspTraceError(("%s: GetEntry of VoiceProfile Line failed Index:%d\n",__FUNCTION__,(uVlIndex - 1)));
                execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return execRetVal;
            }

            pDMLLine = &(pTelcoVoiceLineCtrl->dml);

            if( NULL == pDMLLine )
            {
                CcspTraceError(("%s: Invalid VoiceProfile Line structure pointer. Index:%d\n",__FUNCTION__,(uVlIndex - 1)));
                execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return execRetVal;
            }

            //DML Resource Locked
            pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();

            if(TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_CALL_WAITING, pstLineCfg->CallWaitingEnable) == ANSC_STATUS_SUCCESS)
            {
                pDMLLine->LineCallingFeaturesObj.CallWaitingEnable = pstLineCfg->CallWaitingEnable;
            }

            if(TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_MSG_WAIT_INDICATOR, pstLineCfg->MWIEnable) == ANSC_STATUS_SUCCESS)
            {
                pDMLLine->LineCallingFeaturesObj.MessageWaiting = pstLineCfg->MWIEnable;
            }

            if(TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_CONF_CALL, pstLineCfg->X_RDK_Central_COM_ConferenceCallingEnable) == ANSC_STATUS_SUCCESS)
            {
                pDMLLine->LineCallingFeaturesObj.ConferenceCallingEnable = pstLineCfg->X_RDK_Central_COM_ConferenceCallingEnable;
            }

            if(TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_HOLD, pstLineCfg->X_RDK_Central_COM_HoldEnable) == ANSC_STATUS_SUCCESS)
            {
                pDMLLine->LineCallingFeaturesObj.HoldEnable = pstLineCfg->X_RDK_Central_COM_HoldEnable;
            }

            if(TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_CALLER_ID, pstLineCfg->X_RDK_Central_COM_PhoneCallerIDEnable) == ANSC_STATUS_SUCCESS)
            {
                pDMLLine->LineCallingFeaturesObj.CallerIDEnable = pstLineCfg->X_RDK_Central_COM_PhoneCallerIDEnable;
            }

            if(TelcoVoiceMgrDmlSetDirectoryNumber(uVsIndex, uVpIndex, uVlIndex, pstLineCfg->DirectoryNumber) == ANSC_STATUS_SUCCESS)
            {
                snprintf(pDMLLine->DirectoryNumber, sizeof(pDMLLine->DirectoryNumber), "%s", pstLineCfg->DirectoryNumber);
            }

            bLineEnable = (0 == strcmp(pstLineCfg->LineEnable, "Enabled")) ? TRUE : FALSE;
            if(TelcoVoiceMgrDmlSetLineEnable(uVsIndex, uVpIndex, uVlIndex, bLineEnable) == ANSC_STATUS_SUCCESS)
            {
                pDMLLine->Enable = bLineEnable;
            }

            if(TelcoVoiceMgrDmlSetLineSIPAuthCredentials(uVsIndex, uVpIndex, uVlIndex, VOICE_HAL_AUTH_UNAME, pstLineCfg->SIPAuthUserName) == ANSC_STATUS_SUCCESS)
            {
                snprintf(pDMLLine->LineSipObj.AuthUserName, sizeof(pDMLLine->LineSipObj.AuthUserName), "%s", pstLineCfg->SIPAuthUserName);
            }

            if(TelcoVoiceMgrDmlSetLineSIPAuthCredentials(uVsIndex, uVpIndex, uVlIndex, VOICE_HAL_AUTH_PWD, pstLineCfg->SIPAuthPassword) == ANSC_STATUS_SUCCESS)
            {
                snprintf(pDMLLine->LineSipObj.AuthPassword, sizeof(pDMLLine->LineSipObj.AuthPassword), "%s", pstLineCfg->SIPAuthPassword);
            }

            if(TelcoVoiceMgrDmlSetLineSipURI(uVsIndex, uVpIndex, uVlIndex, pstLineCfg->SIPURI) == ANSC_STATUS_SUCCESS)
            {
                snprintf(pDMLLine->LineSipObj.URI, sizeof(pDMLLine->LineSipObj.URI), "%s", pstLineCfg->SIPURI);
            }

            if(TelcoVoiceMgrDmlSetReceiveGain(uVsIndex, uVpIndex, uVlIndex, pstLineCfg->ReceiveGain) == ANSC_STATUS_SUCCESS)
            {
                pDMLLine->LineVoiceProcessingObj.ReceiveGain = pstLineCfg->ReceiveGain;
            }

            if(TelcoVoiceMgrDmlSetTransmitGain(uVsIndex, uVpIndex, uVlIndex, pstLineCfg->TransmitGain) == ANSC_STATUS_SUCCESS)
            {
                pDMLLine->LineVoiceProcessingObj.TransmitGain = pstLineCfg->TransmitGain;
            }

            //DML Resource Unlocked
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        }
    }
#endif
    CcspTraceInfo(("%s : Telcovoice Blob Data Applied Successfully\n",__FUNCTION__));

    return execRetVal;
}

/**
 *  Function to calculate timeout value for executing the blob
 *
 *  @param numOfEntries Number of Entries of blob
 *
 * returns timeout value
 */
size_t TelcoVoiceMgr_TelcoVoiceData_Timeout_Handler(size_t numOfEntries)
{
    return (numOfEntries * TELCOVOICEDATA_DEFAULT_TIMEOUT);
}

/*
 * Function to rollback to previous configs if apply failed
 *
 * @returns 0 on success, failure otherwise
 */
int TelcoVoiceMgr_TelcoVoiceData_Rollback_Handler(void)
{
    //TODO
    return 0;
}

/* API to free the resources after blob apply */
void TelcoVoiceMgr_TelcoVoiceData_Free_Resources(void *arg)
{
    CcspTraceInfo(("Entering: %s\n",__FUNCTION__));

    if ( arg == NULL ) 
    {
        CcspTraceError(("%s: Input Data is NULL\n",__FUNCTION__));
        return;
    }

    execData *blob_exec_data  = (execData*) arg;
    TelcoVoice_WebConfig_t *pWebConfig   = (TelcoVoice_WebConfig_t *) blob_exec_data->user_data;
    free(blob_exec_data);

    if ( NULL != pWebConfig ) 
    {
        if( NULL != pWebConfig->pstServiceInfo )
        {
            if( NULL != pWebConfig->pstServiceInfo->pstLineInfo )
            {
                free(pWebConfig->pstServiceInfo->pstLineInfo);
                pWebConfig->pstServiceInfo->pstLineInfo = NULL;
            }
            
            free(pWebConfig->pstServiceInfo);
            pWebConfig->pstServiceInfo = NULL;
        }

        free(pWebConfig);
        pWebConfig = NULL;

        CcspTraceInfo(("%s:Success in clearing TelcoVoice webconfig resources\n",__FUNCTION__));
    } 
}

/* TelcoVoiceMgrDmlTelcoVoiceDataSet: */
/**
* @description set private data blob for telcovoice params
*
* @param char *pData - CJSON data
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlTelcoVoiceDataSet(const void *pData, size_t len)
{
    size_t offset = 0;
    msgpack_unpacked msg;
    msgpack_unpack_return mp_rv;
    msgpack_object_map *map = NULL;
    msgpack_object_kv* map_ptr  = NULL;
    execData *execDataPf = NULL;
    TelcoVoice_WebConfig_t *pWebConfig = NULL;  
    int i = 0;
 
    msgpack_unpacked_init( &msg );
    len +=  1;

    /* The outermost wrapper MUST be a map. */
    mp_rv = msgpack_unpack_next( &msg, (const char*) pData, len, &offset );
    if (mp_rv != MSGPACK_UNPACK_SUCCESS) {
        CcspTraceError(("%s: Failed to unpack telcovoice msg blob. Error %d",__FUNCTION__,mp_rv));
        msgpack_unpacked_destroy( &msg );
        return ANSC_STATUS_FAILURE;
    }

    CcspTraceInfo(("%s:Msg unpack success. Offset is %lu\n", __FUNCTION__,offset));
    msgpack_object obj = msg.data;
    
    map = &msg.data.via.map;
    
    map_ptr = obj.via.map.ptr;
    if ((!map) || (!map_ptr)) {
        CcspTraceError(("Failed to get object map\n"));
        msgpack_unpacked_destroy( &msg );
        return ANSC_STATUS_FAILURE;
    }

    if (msg.data.type != MSGPACK_OBJECT_MAP) {
        CcspTraceError(("%s: Invalid msgpack type",__FUNCTION__));
        msgpack_unpacked_destroy( &msg );
        return ANSC_STATUS_FAILURE;
    }

    /* Allocate memory for telcovoice webconfig structure */
    pWebConfig = (TelcoVoice_WebConfig_t *) malloc(sizeof(TelcoVoice_WebConfig_t));
    if ( pWebConfig == NULL ) 
    {
        CcspTraceError(("%s: Telcovoice Struct malloc error\n",__FUNCTION__));
        msgpack_unpacked_destroy( &msg );
        return ANSC_STATUS_FAILURE;
    }

    memset( pWebConfig, 0, sizeof(TelcoVoice_WebConfig_t) );

    /* Parsing Config Msg String to TelcoVoice Structure */
    for (i = 0;i < map->size;i++) 
    {
        if (strncmp(map_ptr->key.via.str.ptr, "telcovoip",map_ptr->key.via.str.size) == 0) {
            if (TelcoVoiceMgr_WebConfig_Process_TelcoVoipParams(map_ptr->val, pWebConfig) != ANSC_STATUS_SUCCESS) {
                CcspTraceError(("%s:Failed to copy telcovoip params",__FUNCTION__));
                msgpack_unpacked_destroy( &msg );
                if ( NULL != pWebConfig ) {
                    if( NULL != pWebConfig->pstServiceInfo )
                    {
                        if( NULL != pWebConfig->pstServiceInfo->pstLineInfo )
                        {
                            free(pWebConfig->pstServiceInfo->pstLineInfo);
                            pWebConfig->pstServiceInfo->pstLineInfo = NULL;
                        }
                        free(pWebConfig->pstServiceInfo);
                        pWebConfig->pstServiceInfo = NULL;
                    }
                    free(pWebConfig);
                    pWebConfig = NULL;
                } 

                return ANSC_STATUS_FAILURE;
            }
        }
        else if (strncmp(map_ptr->key.via.str.ptr, "subdoc_name",map_ptr->key.via.str.size) == 0)
        {
            if (map_ptr->val.type == MSGPACK_OBJECT_STR) {
                strncpy(pWebConfig->subdoc_name, map_ptr->val.via.str.ptr, map_ptr->val.via.str.size);
                pWebConfig->subdoc_name[map_ptr->val.via.str.size] = '\0';
                CcspTraceInfo(("subdoc name :%s\n", pWebConfig->subdoc_name));
            }
        }
        else if (strncmp(map_ptr->key.via.str.ptr, "version",map_ptr->key.via.str.size) == 0)
        {
            if (map_ptr->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                pWebConfig->version = (unsigned long) map_ptr->val.via.u64;
                CcspTraceInfo(("Version :%lu\n",pWebConfig->version));
            }
        }
        else if (strncmp(map_ptr->key.via.str.ptr, "transaction_id",map_ptr->key.via.str.size) == 0)
        {
            if (map_ptr->val.type == MSGPACK_OBJECT_POSITIVE_INTEGER) {
                pWebConfig->transaction_id = (unsigned short) map_ptr->val.via.u64;
                CcspTraceInfo(("Tx id :%d\n",pWebConfig->transaction_id));
            }
        }

        ++map_ptr;
    }

    //Free allocated resource
    msgpack_unpacked_destroy( &msg );

    //Push blob request after collection
    execDataPf = (execData*) malloc (sizeof(execData));
    if ( execDataPf != NULL ) 
    {
        memset(execDataPf, 0, sizeof(execData));
        execDataPf->txid = pWebConfig->transaction_id;
        execDataPf->version = pWebConfig->version;
        execDataPf->numOfEntries = 1;
        strncpy(execDataPf->subdoc_name,pWebConfig->subdoc_name, sizeof(execDataPf->subdoc_name)-1);
        execDataPf->user_data = (void*) pWebConfig;
        execDataPf->calcTimeout = TelcoVoiceMgr_TelcoVoiceData_Timeout_Handler;
        execDataPf->executeBlobRequest = TelcoVoiceMgr_Process_Webconfig_Request;
        execDataPf->rollbackFunc = TelcoVoiceMgr_TelcoVoiceData_Rollback_Handler;
        execDataPf->freeResources = TelcoVoiceMgr_TelcoVoiceData_Free_Resources;
        PushBlobRequest(execDataPf);
        CcspTraceInfo(("%s PushBlobRequest Complete\n",__FUNCTION__));
    }

    return ANSC_STATUS_SUCCESS;  
}

/* TelcoVoiceMgrDmlTelcoVoiceWebConfigInit: */
/**
* @description WebConfig Blob Initialization
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlTelcoVoiceWebConfigInit( void )
{
    char *sub_docs[TELCOVOICEDATA_SUBDOC_COUNT+1]= {"telcovoip",(char *) 0 };
    blobRegInfo *blobData        = NULL,
                *blobDataPointer = NULL;
    int i;
    
    //Allocate memory for blob registration structure
    blobData = (blobRegInfo*) malloc(TELCOVOICEDATA_SUBDOC_COUNT * sizeof(blobRegInfo));

    //Validate Memory
    if (blobData == NULL) 
    {
        CcspTraceError(("%s: Failed to allocate memory\n",__FUNCTION__)); 
        return ANSC_STATUS_FAILURE;
    }

    memset(blobData, 0, TELCOVOICEDATA_SUBDOC_COUNT * sizeof(blobRegInfo));
    
    //Separate subdocs information
    blobDataPointer = blobData;
    for ( i = 0; i < TELCOVOICEDATA_SUBDOC_COUNT; i++ )
    {
        strncpy(blobDataPointer->subdoc_name, sub_docs[i], sizeof(blobDataPointer->subdoc_name) - 1);
        blobDataPointer++;
    }
    blobDataPointer = blobData;
 
    //Register subdocs
    getVersion versionGet = getTelcoVoiceDataBlobVersion;
    setVersion versionSet = setTelcoVoiceDataBlobVersion;
    register_sub_docs(blobData, TELCOVOICEDATA_SUBDOC_COUNT, versionGet, versionSet);

    CcspTraceInfo(("%s Telcovoice Webconfig Subdoc Registration Complete\n",__FUNCTION__));    

    return ANSC_STATUS_SUCCESS;  
}

/**
 * API to get Blob version from Utopia db
 *
 * @param pSubDoc Pointer to name of the subdoc
 *
 * returns version number if present, 0 otherwise
 */
unsigned int getTelcoVoiceDataBlobVersion(char *pSubDoc)
{
    char  subdoc_ver[BUFFER_LENGTH_64] = {0},
          buf[BUFFER_LENGTH_64]        = {0};
    int retval;
    unsigned int version = 0;

    //Get blob version from DB
    snprintf(buf,sizeof(buf), "%s_version", pSubDoc);
    if ( syscfg_get( NULL, buf, subdoc_ver, sizeof(subdoc_ver)) == 0 )
    {
        version = strtoul(subdoc_ver, NULL, 10);
        CcspTraceInfo(("%s TelcoVoice data %s blob version %s\n",__FUNCTION__, pSubDoc, subdoc_ver));
        return version;
    }

    CcspTraceInfo(("%s Failed to get TelcoVoice Data %s blob version\n",__FUNCTION__, pSubDoc));

    return 0;
}

/**
 * API to set Blob version in Utopia db
 *
 * @param subdoc  Pointer to name of the subdoc
 * @param version Version of the blob
 *
 * returns 0 on success, error otherwise
 */
int setTelcoVoiceDataBlobVersion(char *pSubDoc, unsigned int version)
{
    char subdoc_ver[BUFFER_LENGTH_64] = {0}, 
         buf[BUFFER_LENGTH_64]        = {0};

    snprintf(subdoc_ver, sizeof(subdoc_ver), "%u", version);
    snprintf(buf, sizeof(buf), "%s_version", pSubDoc);

    //Set blob version to DB
    if( syscfg_set( NULL, buf, subdoc_ver ) != 0 )
    {
        return -1;
    }
    else
    {
        if ( 0 != syscfg_commit( ) ) 
        {
            return -1;
        }
    }

    CcspTraceInfo(("%s TelcoVoice data %s blob version %s set successfully\n",__FUNCTION__, pSubDoc, subdoc_ver));

    return 0;
}
