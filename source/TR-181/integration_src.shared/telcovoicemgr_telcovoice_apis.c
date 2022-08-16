
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


#define GET_LOCK_AND_SET_DML_STRING(lock, dmlVar, value) \
     lock = TelcoVoiceMgrDmlGetDataLocked(); \
     if(lock)\
     {\
        snprintf(dmlVar, sizeof(dmlVar), "%s", value);\
        TelcoVoiceMgrDmlGetDataRelease(lock);\
     }\
     else\
     {\
        CcspTraceError(("%s: Could not get lock for setting parameter %s, value: %s\n", __FUNCTION__, dmlVar, value));\
        execRetVal->ErrorCode = BLOB_EXEC_FAILURE;\
        return execRetVal;\
     }\

#define GET_LOCK_AND_SET_DML_NUMBER(lock, dmlVar, value) \
     lock = TelcoVoiceMgrDmlGetDataLocked(); \
     if(lock)\
     {\
        dmlVar = value;\
        TelcoVoiceMgrDmlGetDataRelease(lock);\
     }\
     else\
     {\
        CcspTraceError(("%s: Could not get lock for setting parameter %s, value: %d\n", __FUNCTION__, dmlVar, value));\
        execRetVal->ErrorCode = BLOB_EXEC_FAILURE;\
        return execRetVal;\
     }\

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
    int i, j;
    unsigned int uVsIndex = 0, uVpIndex = 0, uVlIndex = 0;
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    unsigned int uSipClientIndex = 0, uSipNetworkIndex = 0, uFXSIndex = 0, uCallControlSetIndex = 0;
#endif
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
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
        DML_PROFILE_CTRL_T             *pTelcoVoiceProfCtrl  = NULL;
        TELCOVOICEMGR_DML_VOICEPROFILE *pVoiceProfile        = NULL;
#else
        PDML_VOIPPROFILE                  pDmlVoiceProfile   = NULL;
        PDML_SIP                          pDmlSipObj         = NULL;
        PDML_SIP_NETWORK                  pDmlSipNetwork     = NULL;
#endif

        //Index Assignment
        uVsIndex = pServiceCfg->uiServiceInstanceNumber;
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
        uVpIndex = pServiceCfg->uiProfileInstanceNumber;
#else
        uVpIndex = 1;
#endif

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
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
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

        if( ( TRUE == pServiceCfg->IsOutboundProxyPresent ) && ( TelcoVoiceMgrDmlSetOutboundProxy(uVsIndex, uVpIndex, pServiceCfg->OutboundProxy) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.OutboundProxy, pServiceCfg->OutboundProxy)
        }

        if( ( TRUE == pServiceCfg->IsProxyServerPresent ) &&  ( TelcoVoiceMgrDmlSetProxyServer(uVsIndex, uVpIndex, pServiceCfg->ProxyServer) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.ProxyServer, pServiceCfg->ProxyServer)
        }

        if( ( TRUE == pServiceCfg->IsRegistrarServerPresent ) &&  ( TelcoVoiceMgrDmlSetRegistrarServer(uVsIndex, uVpIndex, pServiceCfg->RegistrarServer) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.RegistrarServer, pServiceCfg->RegistrarServer)
        }

        if( ( TRUE == pServiceCfg->IsUserAgentDomainPresent ) &&  ( TelcoVoiceMgrDmlSetUserAgentDomain(uVsIndex, uVpIndex, pServiceCfg->UserAgentDomain) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.UserAgentDomain, pServiceCfg->UserAgentDomain)
        }

        if( ( TRUE == pServiceCfg->IsConferencingURIPresent ) &&  ( TelcoVoiceMgrDmlSetConferencingURI(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_ConferencingURI) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.ConferencingURI, pServiceCfg->X_RDK_Central_COM_ConferencingURI)
        }

        if( ( TRUE == pServiceCfg->IsDigitMapPresent ) &&  ( TelcoVoiceMgrDmlSetDigitMap(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_DigitMap, "X_RDK-Central_COM_DigitMap") == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pVoiceProfile->DigitMap, pServiceCfg->X_RDK_Central_COM_DigitMap)
        }

        if( ( TRUE == pServiceCfg->IsEmergencyDigitMapPresent ) &&  ( TelcoVoiceMgrDmlSetDigitMap(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_EmergencyDigitMap, "X_RDK-Central_COM_EmergencyDigitMap") == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pVoiceProfile->EmergencyDigitMap, pServiceCfg->X_RDK_Central_COM_EmergencyDigitMap)
        }

        if( ( TRUE == pServiceCfg->IsOutboundProxyPortPresent ) &&  ( TelcoVoiceMgrDmlSetOutboundProxyPort(uVsIndex, uVpIndex, pServiceCfg->OutboundProxyPort) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.OutboundProxyPort, pServiceCfg->OutboundProxyPort)
        }

        if( ( TRUE == pServiceCfg->IsProxyServerPortPresent ) &&  ( TelcoVoiceMgrDmlSetProxyServerPort(uVsIndex, uVpIndex, pServiceCfg->ProxyServerPort) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.ProxyServerPort, pServiceCfg->ProxyServerPort)
        }

        if( ( TRUE == pServiceCfg->IsRegistrarServerPortPresent ) &&  ( TelcoVoiceMgrDmlSetRegistrarServerPort(uVsIndex, uVpIndex, pServiceCfg->RegistrarServerPort) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.RegistrarServerPort, pServiceCfg->RegistrarServerPort)
        }

        if( ( TRUE == pServiceCfg->IsSIPEthernetPriorityMarkPresent ) &&  ( TelcoVoiceMgrDmlSetSipEthernetPriorityMark(uVsIndex, uVpIndex, pServiceCfg->SIPEthernetPriorityMark) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.EthernetPriorityMark,pServiceCfg->SIPEthernetPriorityMark)
        }

        if( TRUE == pServiceCfg->IsSIPDSCPMarkPresent )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.DSCPMark, pServiceCfg->SIPDSCPMark)
            (void)storeObjectInteger(uVsIndex, uVpIndex, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "DSCPMark", pVoiceProfile->SIPObj.DSCPMark);
        }

        if( TRUE == pServiceCfg->IsRTPDSCPMarkPresent )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->RTPObj.DSCPMark, pServiceCfg->RTPDSCPMark)
            (void)storeObjectInteger(uVsIndex, uVpIndex, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "RtpDSCPMark", pVoiceProfile->RTPObj.DSCPMark);
        }

        if( ( TRUE == pServiceCfg->IsRTPEthernetPriorityMarkPresent ) &&  ( TelcoVoiceMgrDmlSetRtpEthernetPriorityMark(uVsIndex, uVpIndex, pServiceCfg->RTPEthernetPriorityMark) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->RTPObj.EthernetPriorityMark, pServiceCfg->RTPEthernetPriorityMark)
        }

        if( ( TRUE == pServiceCfg->IsSDigitTimerPresent ) &&  ( TelcoVoiceMgrDmlSetSDigitTimer(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_SDigitTimer) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->SDigitTimer, pServiceCfg->X_RDK_Central_COM_SDigitTimer)
        }

        if( ( TRUE == pServiceCfg->IsZDigitTimerPresent ) &&  ( TelcoVoiceMgrDmlSetZDigitTimer(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_ZDigitTimer) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->ZDigitTimer, pServiceCfg->X_RDK_Central_COM_ZDigitTimer)
        }

        if( ( TRUE == pServiceCfg->IsNetworkDisconnectPresent ) &&  ( TelcoVoiceMgrDmlSetNetworkDisconnect(uVsIndex, uVpIndex, pServiceCfg->X_RDK_Central_COM_NetworkDisconnect) == ANSC_STATUS_SUCCESS) )
        {
            GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pVoiceProfile->SIPObj.NetworkDisconnect, pServiceCfg->X_RDK_Central_COM_NetworkDisconnect)
        }

        for( j = 0; j < pServiceCfg->LineCount; j++ )
        {
            WebConfig_LineTable_t *pstLineCfg = &(pServiceCfg->pstLineInfo[j]);
            DML_LINE_CTRL_T       *pTelcoVoiceLineCtrl  = NULL;
            TELCOVOICEMGR_DML_LINE *pDMLLine            = NULL;

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


            if( ( TRUE == pstLineCfg->IsCallWaitingEnablePresent ) &&  ( TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_CALL_WAITING, pstLineCfg->CallWaitingEnable) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pDMLLine->LineCallingFeaturesObj.CallWaitingEnable, pstLineCfg->CallWaitingEnable)
            }

            if( ( TRUE == pstLineCfg->IsMWIEnablePresent ) &&  ( TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_MSG_WAIT_INDICATOR, pstLineCfg->MWIEnable) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pDMLLine->LineCallingFeaturesObj.MessageWaiting, pstLineCfg->MWIEnable)
            }

            if( ( TRUE == pstLineCfg->IsConferenceCallingEnablePresent ) &&  ( TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_CONF_CALL, pstLineCfg->X_RDK_Central_COM_ConferenceCallingEnable) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pDMLLine->LineCallingFeaturesObj.ConferenceCallingEnable, pstLineCfg->X_RDK_Central_COM_ConferenceCallingEnable)
            }

            if( ( TRUE == pstLineCfg->IsHoldEnablePresent ) &&  ( TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_HOLD, pstLineCfg->X_RDK_Central_COM_HoldEnable) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pDMLLine->LineCallingFeaturesObj.HoldEnable, pstLineCfg->X_RDK_Central_COM_HoldEnable)
            }

            if( ( TRUE == pstLineCfg->IsPhoneCallerIDEnablePresent ) &&  ( TelcoVoiceMgrDmlSetLineCallingFeatures(uVsIndex, uVpIndex, uVlIndex, VOICE_CALLING_FEATURE_CALLER_ID, pstLineCfg->X_RDK_Central_COM_PhoneCallerIDEnable) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pDMLLine->LineCallingFeaturesObj.CallerIDEnable, pstLineCfg->X_RDK_Central_COM_PhoneCallerIDEnable)
            }

            if( ( TRUE == pstLineCfg->IsDirectoryNumberPresent ) &&  ( TelcoVoiceMgrDmlSetDirectoryNumber(uVsIndex, uVpIndex, uVlIndex, pstLineCfg->DirectoryNumber) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pDMLLine->DirectoryNumber, pstLineCfg->DirectoryNumber)
            }
            if( TRUE == pstLineCfg->IsLineEnablePresent )
            {
                TELCOVOICEMGR_ENABLE_ENUM enVoiceEnable;
                unsigned char        bValidVoiceEnableParam = TRUE;

                if ( 0 == strcmp(pstLineCfg->LineEnable, "Enabled" ) )
                {
                    enVoiceEnable = ENABLED;
                }
                else if( 0 == strcmp(pstLineCfg->LineEnable, "Disabled" ) )
                {
                    enVoiceEnable = DISABLED;
                }
                else if( 0 == strcmp(pstLineCfg->LineEnable, "Quiescent" ) )
                {
                    enVoiceEnable = QUIESCENT;
                }
                else
                {
                    //No need to configure Line Enable
                    bValidVoiceEnableParam = FALSE;
                }

                if( ( TRUE == bValidVoiceEnableParam ) && (TelcoVoiceMgrDmlSetLineEnable(uVsIndex, uVpIndex, uVlIndex, enVoiceEnable) == ANSC_STATUS_SUCCESS) )
                {
                    GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pDMLLine->Enable, enVoiceEnable)
                }
            }

            if( ( TRUE == pstLineCfg->IsSIPAuthUserNamePresent ) &&  ( TelcoVoiceMgrDmlSetLineSIPAuthCredentials(uVsIndex, uVpIndex, uVlIndex, VOICE_HAL_AUTH_UNAME, pstLineCfg->SIPAuthUserName) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pDMLLine->LineSipObj.AuthUserName, pstLineCfg->SIPAuthUserName)
            }

            if( ( TRUE == pstLineCfg->IsSIPAuthPasswordPresent ) &&  ( TelcoVoiceMgrDmlSetLineSIPAuthCredentials(uVsIndex, uVpIndex, uVlIndex, VOICE_HAL_AUTH_PWD, pstLineCfg->SIPAuthPassword) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData, pDMLLine->LineSipObj.AuthPassword, pstLineCfg->SIPAuthPassword)
            }

            if( ( TRUE == pstLineCfg->IsSIPURIPresent ) &&  ( TelcoVoiceMgrDmlSetLineSipURI(uVsIndex, uVpIndex, uVlIndex, pstLineCfg->SIPURI) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_STRING(pTelcoVoiceMgrDmlData,pDMLLine->LineSipObj.URI, pstLineCfg->SIPURI)
            }

            if( ( TRUE == pstLineCfg->IsReceiveGainPresent ) &&  ( TelcoVoiceMgrDmlSetReceiveGain(uVsIndex, uVpIndex, uVlIndex, pstLineCfg->ReceiveGain) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pDMLLine->LineVoiceProcessingObj.ReceiveGain, pstLineCfg->ReceiveGain)
            }

            if( ( TRUE == pstLineCfg->IsTransmitGainPresent ) &&  ( TelcoVoiceMgrDmlSetTransmitGain(uVsIndex, uVpIndex, uVlIndex, pstLineCfg->TransmitGain) == ANSC_STATUS_SUCCESS) )
            {
                GET_LOCK_AND_SET_DML_NUMBER(pTelcoVoiceMgrDmlData, pDMLLine->LineVoiceProcessingObj.TransmitGain, pstLineCfg->TransmitGain)
            }


        }
#else
        for( j = 0; j < pServiceCfg->LineCount; j++ )
        {
            WebConfig_LineTable_t *pstLineCfg = &(pServiceCfg->pstLineInfo[j]);
            PDML_CALLCONTROL_LINE_CTRL_T pCallCtrlLineCtrl = NULL;
            PDML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_T pCallFeaturesSetCtrl = NULL;

            uVlIndex = pstLineCfg->uiLineInstanceNumber;

            //Get Voice CallControl Line DML structure
            pCallCtrlLineCtrl = (DML_CALLCONTROL_LINE_CTRL_T*) TelcoVoiceMgrDml_CallControl_LineList_GetEntry(pTelcoVoiceSrvCtrl, (uVlIndex - 1), &ulInstance);

            if( NULL == pCallCtrlLineCtrl )
            {
                CcspTraceError(("%s: GetEntry of Line failed Index:%d\n",__FUNCTION__,(uVlIndex - 1)));
                execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return execRetVal;
            }

            if( TRUE == pstLineCfg->IsLineEnablePresent )
            {
                TelcoVoiceMgrDml_CallControl_LineList_SetParamBoolValue(pCallCtrlLineCtrl, "Enable", pstLineCfg->LineEnable);
            }

            if( TRUE == pstLineCfg->IsDirectoryNumberPresent )
            {
                TelcoVoiceMgrDml_CallControl_LineList_SetParamStringValue(pCallCtrlLineCtrl, "DirectoryNumber", pstLineCfg->DirectoryNumber);
            }

            if( TRUE == pstLineCfg->IsLineProviderPresent )
            {
                TelcoVoiceMgrDml_CallControl_LineList_SetParamStringValue(pCallCtrlLineCtrl, "Provider", pstLineCfg->LineProvider);
            }

            if( TRUE == pstLineCfg->IsLineCallingFeaturesPresent )
            {
                TelcoVoiceMgrDml_CallControl_LineList_SetParamStringValue(pCallCtrlLineCtrl, "CallingFeatures", pstLineCfg->LineCallingFeatures);
            }

            uCallControlSetIndex = pstLineCfg->uiLineInstanceNumber;
            pCallFeaturesSetCtrl =  (DML_CALLCONTROL_CALLINGFEATURES_SET_CTRL_T*) TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_GetEntry(pTelcoVoiceSrvCtrl, (uCallControlSetIndex - 1), &ulInstance);

            if( NULL == pCallFeaturesSetCtrl )
            {
                CcspTraceError(("%s: GetEntry of Calling Features failed Index:%d\n",__FUNCTION__,(uCallControlSetIndex - 1)));
                execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return execRetVal;
            }

            if( TRUE == pstLineCfg->IsCallWaitingEnablePresent )
            {
                TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamBoolValue(pCallFeaturesSetCtrl, "CallWaitingEnable", pstLineCfg->CallWaitingEnable);
            }
            if( TRUE == pstLineCfg->IsMWIEnablePresent )
            {
                TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamBoolValue(pCallFeaturesSetCtrl, "MWIEnable", pstLineCfg->MWIEnable);
            }
            if( TRUE == pstLineCfg->IsConferenceCallingEnablePresent )
            {
                TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamBoolValue(pCallFeaturesSetCtrl, "X_RDK-Central_COM_ConferenceCallingEnable", pstLineCfg->ConferenceCallingEnable);
            }
            if( TRUE == pstLineCfg->IsHoldEnablePresent )
            {
                TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamBoolValue(pCallFeaturesSetCtrl, "X_RDK-Central_COM_HoldEnable", pstLineCfg->HoldEnable);
            }
            if( TRUE == pstLineCfg->IsPhoneCallerIDEnablePresent )
            {
                TelcoVoiceMgrDml_CallControl_CallingFeatures_SetList_SetParamBoolValue(pCallFeaturesSetCtrl, "X_RDK-Central_COM_PhoneCallerIDEnable", pstLineCfg->PhoneCallerIDEnable);
            }
        }

        for( j = 0; j < pServiceCfg->SipNwCount; j++ )
        {
            WebConfig_SipNwTable_t *pstSipNwCfg = &(pServiceCfg->pstSipNwInfo[j]);
            PDML_SIP_NETWORK_CTRL_T pSipNetworkCtrl = NULL;
            unsigned int transportProtocol = 0;

            uSipNetworkIndex = pstSipNwCfg->uiSipNwInstanceNumber;

            //Get Voice Sip Network DML structure
            pSipNetworkCtrl = (DML_SIP_NETWORK_CTRL_T*) TelcoVoiceMgrDml_SIP_NetworkList_GetEntry(pTelcoVoiceSrvCtrl, (uSipNetworkIndex - 1), &ulInstance);

            if( NULL == pSipNetworkCtrl )
            {
                CcspTraceError(("%s: GetEntry of Network failed Index:%d\n",__FUNCTION__,(uSipNetworkIndex - 1)));
                execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return execRetVal;
            }

            if( TRUE == pstSipNwCfg->IsSIPNetworkEnablePresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamBoolValue(pSipNetworkCtrl, "Enable", pstSipNwCfg->SIPNetworkEnable);
            }

            if( TRUE == pstSipNwCfg->IsOutboundProxyPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamStringValue(pSipNetworkCtrl, "OutboundProxy", pstSipNwCfg->OutboundProxy);
            }

            if( TRUE == pstSipNwCfg->IsProxyServerPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamStringValue(pSipNetworkCtrl, "ProxyServer", pstSipNwCfg->ProxyServer);
            }

            if( TRUE == pstSipNwCfg->IsProxyServerTransportPresent )
            {
                if(TelcoVoiceMgrConvertProtocolToEnum(pstSipNwCfg->ProxyServerTransport, &transportProtocol) == ANSC_STATUS_SUCCESS)
                {
                    TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue(pSipNetworkCtrl, "ProxyServerTransport", transportProtocol);
                }
            }

            if( TRUE == pstSipNwCfg->IsRegistrarServerPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamStringValue(pSipNetworkCtrl, "RegistrarServer", pstSipNwCfg->RegistrarServer);
            }

            if( TRUE == pstSipNwCfg->IsRegistrarServerTransportPresent )
            {
                if(TelcoVoiceMgrConvertProtocolToEnum(pstSipNwCfg->RegistrarServerTransport, &transportProtocol) == ANSC_STATUS_SUCCESS)
                {
                   TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue(pSipNetworkCtrl, "RegistrarServerTransport", transportProtocol);
                }
            }

            if( TRUE == pstSipNwCfg->IsUserAgentDomainPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamStringValue(pSipNetworkCtrl, "UserAgentDomain", pstSipNwCfg->UserAgentDomain);
            }

            if( TRUE == pstSipNwCfg->IsUserAgentTransportPresent )
            {
                if(TelcoVoiceMgrConvertProtocolToEnum(pstSipNwCfg->UserAgentTransport, &transportProtocol) == ANSC_STATUS_SUCCESS)
                {
                   TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue(pSipNetworkCtrl, "UserAgentTransport", transportProtocol);
                }
            }

            if( TRUE == pstSipNwCfg->IsConferencingURIPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamStringValue(pSipNetworkCtrl, "X_RDK-Central_COM_ConferencingURI", pstSipNwCfg->ConferencingURI);
            }

            if( TRUE == pstSipNwCfg->IsSIPNetworkVoIPProfilePresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamStringValue(pSipNetworkCtrl, "VoIPProfile", pstSipNwCfg->SIPNetworkVoIPProfile);
            }

            if( TRUE == pstSipNwCfg->IsOutboundProxyPortPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue(pSipNetworkCtrl, "OutboundProxyPort", pstSipNwCfg->OutboundProxyPort);
            }

            if( TRUE == pstSipNwCfg->IsProxyServerPortPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue(pSipNetworkCtrl, "ProxyServerPort", pstSipNwCfg->ProxyServerPort);
            }

            if( TRUE == pstSipNwCfg->IsRegistrarServerPortPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue(pSipNetworkCtrl, "RegistrarServerPort", pstSipNwCfg->RegistrarServerPort);
            }

            if( TRUE == pstSipNwCfg->IsUserAgentPortPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue(pSipNetworkCtrl, "UserAgentPort", pstSipNwCfg->UserAgentPort);
            }

            if( TRUE == pstSipNwCfg->IsSIPEthernetPriorityMarkPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamIntValue(pSipNetworkCtrl, "EthernetPriorityMark", pstSipNwCfg->SIPEthernetPriorityMark);
            }

            if( TRUE == pstSipNwCfg->IsSIPDSCPMarkPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamUlongValue(pSipNetworkCtrl, "DSCPMark", pstSipNwCfg->SIPDSCPMark);
            }

            if( TRUE == pstSipNwCfg->IsNetworkDisconnectPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamBoolValue(pSipNetworkCtrl, "X_RDK-Central_COM_NetworkDisconnect", pstSipNwCfg->NetworkDisconnect);
            }

            if( TRUE == pstSipNwCfg->IsPRACKRequiredPresent )
            {
                TelcoVoiceMgrDml_SIP_NetworkList_SetParamBoolValue(pSipNetworkCtrl, "X_RDK_PRACKRequired", pstSipNwCfg->PRACKRequired);
            }
        }

        for( j = 0; j < pServiceCfg->SipClientCount; j++ )
        {
            WebConfig_SipClientTable_t *pstSipClientCfg = &(pServiceCfg->pstSipClientInfo[j]);
            PDML_SIP_CLIENT_CTRL_T pSipClientCtrl = NULL;

            uSipClientIndex = pstSipClientCfg->uiSipClientInstanceNumber;

            //Get Voice Sip Network DML structure
            pSipClientCtrl = (DML_SIP_CLIENT_CTRL_T*) TelcoVoiceMgrDml_SIP_ClientList_GetEntry(pTelcoVoiceSrvCtrl, (uSipClientIndex - 1), &ulInstance);

            if( NULL == pSipClientCtrl )
            {
                CcspTraceError(("%s: GetEntry of Network failed Index:%d\n",__FUNCTION__,(uSipClientIndex - 1)));
                execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return execRetVal;
            }

            if( TRUE == pstSipClientCfg->IsSIPClientEnablePresent )
            {
                TelcoVoiceMgrDml_SIP_ClientList_SetParamBoolValue(pSipClientCtrl, "Enable", pstSipClientCfg->SIPClientEnable);
            }

            if( TRUE == pstSipClientCfg->IsSIPClientNetworkPresent )
            {
                TelcoVoiceMgrDml_SIP_ClientList_SetParamStringValue(pSipClientCtrl, "Network", pstSipClientCfg->SIPClientNetwork);
            }

            if( TRUE == pstSipClientCfg->IsSIPAuthUserNamePresent )
            {
                TelcoVoiceMgrDml_SIP_ClientList_SetParamStringValue(pSipClientCtrl, "AuthUserName", pstSipClientCfg->SIPAuthUserName);
            }

            if( TRUE == pstSipClientCfg->IsSIPAuthPasswordPresent )
            {
                TelcoVoiceMgrDml_SIP_ClientList_SetParamStringValue(pSipClientCtrl, "AuthPassword", pstSipClientCfg->SIPAuthPassword);
            }

            if( TRUE == pstSipClientCfg->IsSIPURIPresent )
            {
                TelcoVoiceMgrDml_SIP_ClientList_SetParamStringValue(pSipClientCtrl, "RegisterURI", pstSipClientCfg->SIPURI);
            }
        }

        for( j = 0; j < pServiceCfg->VoIPCount; j++ )
        {
            WebConfig_VoIPTable_t *pstVoIPCfg = &(pServiceCfg->pstVoIPInfo[j]);
            PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = NULL;

            uVpIndex = pstVoIPCfg->uiProfileInstanceNumber;

            //Get Voip DML structure
            pVoipProfileCtrl = (DML_VOIPPROFILE_CTRL_T*) TelcoVoiceMgrDml_VoipProfileList_GetEntry(pTelcoVoiceSrvCtrl, (uVpIndex - 1), &ulInstance);

            if( NULL == pVoipProfileCtrl )
            {
                CcspTraceError(("%s: GetEntry of Voice Profile failed Index:%d\n",__FUNCTION__,(uVpIndex - 1)));
                execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return execRetVal;
            }

            if( TRUE == pstVoIPCfg->IsProfileEnablePresent )
            {
                TelcoVoiceMgrDml_VoipProfileList_SetParamBoolValue(pVoipProfileCtrl, "Enable", pstVoIPCfg->ProfileEnable);
            }

            if( TRUE == pstVoIPCfg->IsRTPDSCPMarkPresent )
            {
                TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamUlongValue(pVoipProfileCtrl, "DSCPMark", pstVoIPCfg->RTPDSCPMark);
            }

            if( TRUE == pstVoIPCfg->IsRTPEthernetPriorityMarkPresent )
            {
                TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamIntValue(pVoipProfileCtrl, "EthernetPriorityMark", pstVoIPCfg->RTPEthernetPriorityMark);
            }

            if( TRUE == pstVoIPCfg->IsSDigitTimerPresent )
            {
                TelcoVoiceMgrDml_VoipProfileList_SetParamUlongValue(pVoipProfileCtrl, "X_RDK-Central_COM_SDigitTimer", pstVoIPCfg->SDigitTimer);
            }

            if( TRUE == pstVoIPCfg->IsZDigitTimerPresent )
            {
                TelcoVoiceMgrDml_VoipProfileList_SetParamUlongValue(pVoipProfileCtrl, "X_RDK-Central_COM_ZDigitTimer", pstVoIPCfg->ZDigitTimer);
            }

            if( TRUE == pstVoIPCfg->IsDigitMapPresent )
            {
                TelcoVoiceMgrDml_VoipProfileList_SetParamStringValue(pVoipProfileCtrl, "X_RDK-Central_COM_DigitMap", pstVoIPCfg->DigitMap);
            }

            if( TRUE == pstVoIPCfg->IsEmergencyDigitMapPresent )
            {
               TelcoVoiceMgrDml_VoipProfileList_SetParamStringValue(pVoipProfileCtrl, "X_RDK-Central_COM_EmergencyDigitMap", pstVoIPCfg->EmergencyDigitMap);
            }
        }

        for( j = 0; j < pServiceCfg->FXSCount; j++ )
        {
            WebConfig_POTSFxsTable_t *pstPOTSFxsCfg = &(pServiceCfg->pstPOTSFxsInfo[j]);
            PDML_POTS_FXS_CTRL_T pPOTSFxsCtrl = NULL;

            uFXSIndex = pstPOTSFxsCfg->uiFXSInstanceNumber;

            //Get Voice Sip Network DML structure
            pPOTSFxsCtrl = (DML_VOICESERVICE_TERMINAL_CTRL_T*) TelcoVoiceMgrDml_POTS_FXSList_GetEntry(pTelcoVoiceSrvCtrl, (uFXSIndex - 1), &ulInstance);

            if( NULL == pPOTSFxsCtrl )
            {
                CcspTraceError(("%s: GetEntry of FXS failed Index:%d\n",__FUNCTION__,(uFXSIndex - 1)));
                execRetVal->ErrorCode = BLOB_EXEC_FAILURE;
                return execRetVal;
            }

            if( TRUE == pstPOTSFxsCfg->IsFXSEnablePresent )
            {
                CcspTraceInfo(("%s : FXS Enable received Successfully\n",__FUNCTION__));
                TelcoVoiceMgrDml_POTS_FXSList_SetParamBoolValue(pPOTSFxsCtrl, "Enable", pstPOTSFxsCfg->FXSEnable);
            }
            if( TRUE == pstPOTSFxsCfg->IsReceiveGainPresent )
            {
                CcspTraceInfo(("%s : FXS ReceiveGain received Successfully\n",__FUNCTION__));
                TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamIntValue(pPOTSFxsCtrl, "ReceiveGain", pstPOTSFxsCfg->ReceiveGain);
            }
            if( TRUE == pstPOTSFxsCfg->IsTransmitGainPresent )
            {
                CcspTraceInfo(("%s : FXS TransmitGain received Successfully\n",__FUNCTION__));
                TelcoVoiceMgrDml_POTS_FXSList_VoiceProcessing_SetParamIntValue(pPOTSFxsCtrl, "TransmitGain", pstPOTSFxsCfg->TransmitGain);
            }
        }

#endif
    }
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
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            if( NULL != pWebConfig->pstServiceInfo->pstSipNwInfo )
            {
                free(pWebConfig->pstServiceInfo->pstSipNwInfo);
                pWebConfig->pstServiceInfo->pstSipNwInfo = NULL;
            }
            if( NULL != pWebConfig->pstServiceInfo->pstSipClientInfo )
            {
                free(pWebConfig->pstServiceInfo->pstSipClientInfo);
                pWebConfig->pstServiceInfo->pstSipClientInfo = NULL;
            }
            if( NULL != pWebConfig->pstServiceInfo->pstVoIPInfo )
            {
                free(pWebConfig->pstServiceInfo->pstVoIPInfo);
                pWebConfig->pstServiceInfo->pstVoIPInfo = NULL;
            }
            if( NULL != pWebConfig->pstServiceInfo->pstPOTSFxsInfo )
            {
                free(pWebConfig->pstServiceInfo->pstPOTSFxsInfo);
                pWebConfig->pstServiceInfo->pstPOTSFxsInfo = NULL;
            }
#endif
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
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        if (strncmp(map_ptr->key.via.str.ptr, "telcovoice",map_ptr->key.via.str.size) == 0) {
#else
        if (strncmp(map_ptr->key.via.str.ptr, "telcovoip",map_ptr->key.via.str.size) == 0) {
#endif
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
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
                        if( NULL != pWebConfig->pstServiceInfo->pstSipNwInfo )
                        {
                            free(pWebConfig->pstServiceInfo->pstSipNwInfo);
                            pWebConfig->pstServiceInfo->pstSipNwInfo = NULL;
                        }
                        if( NULL != pWebConfig->pstServiceInfo->pstSipClientInfo )
                        {
                            free(pWebConfig->pstServiceInfo->pstSipClientInfo);
                            pWebConfig->pstServiceInfo->pstSipClientInfo = NULL;
                        }
                        if( NULL != pWebConfig->pstServiceInfo->pstVoIPInfo )
                        {
                            free(pWebConfig->pstServiceInfo->pstVoIPInfo);
                            pWebConfig->pstServiceInfo->pstVoIPInfo = NULL;
                        }
                        if( NULL != pWebConfig->pstServiceInfo->pstPOTSFxsInfo )
                        {
                            free(pWebConfig->pstServiceInfo->pstPOTSFxsInfo);
                            pWebConfig->pstServiceInfo->pstPOTSFxsInfo = NULL;
                        }
#endif
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
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    char *sub_docs[TELCOVOICEDATA_SUBDOC_COUNT+1]= {"telcovoice",(char *) 0 };
#else
    char *sub_docs[TELCOVOICEDATA_SUBDOC_COUNT+1]= {"telcovoip",(char *) 0 };
#endif
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
