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

#include "telcovoicemgr_dml_apis.h"
#include "ccsp_dm_api.h"
#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_hal.h"
#include "voice_report.h"
#ifdef RBUS_BUILD_FLAG_ENABLE
#include "telcovoicemgr_rbus_handler_apis.h"
#include "telcovoicemgr_dml_json_cfg_init.h"
#endif //RBUS_BUILD_FLAG_ENABLE
#ifdef _ANSC_LINUX
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/types.h>
#endif
#include "ansc_platform.h"

extern char * VoiceServiceReportStatusEnable;
extern char * VoiceServiceReportStatusDfltReportingPeriod;
extern char * VoiceServiceReportStatusReportingPeriod;
/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrServicesCreate(VOID);

    description:

        This function constructs cosa Services object and return handle.

    argument:

    return:     newly created Services object.

**********************************************************************/

ANSC_HANDLE TelcoVoiceMgrServicesCreate(VOID)
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTELCOVOICEMGR_DATAMODEL_SERVICES        pMyObject    = (PTELCOVOICEMGR_DATAMODEL_SERVICES)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */

    pMyObject = (PTELCOVOICEMGR_DATAMODEL_SERVICES)AnscAllocateMemory(sizeof(TELCOVOICEMGR_DATAMODEL_SERVICES));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }


    pMyObject->data = TelcoVoiceMgrDmlGetData();
    if ( !pMyObject->data )
    {
        AnscFreeMemory((ANSC_HANDLE)pMyObject);
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Create            = TelcoVoiceMgrServicesCreate;
    pMyObject->Remove            = TelcoVoiceMgrServicesRemove;
    pMyObject->Initialize        = TelcoVoiceMgrServicesInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS TelcoVoiceMgrServicesInitialize(ANSC_HANDLE hThisObject);

    description:

        This function initiate  cosa device info object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS TelcoVoiceMgrServicesInitialize(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                  returnStatus        = ANSC_STATUS_SUCCESS;
    PTELCOVOICEMGR_DATAMODEL_SERVICES     pMyObject           = (PTELCOVOICEMGR_DATAMODEL_SERVICES)hThisObject;

    //Initialise dml data handler
    returnStatus = TelcoVoiceMgrDmlDataInit();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    //Wait HAL initilisation
    returnStatus = TelcoVoiceMgrHal_Init();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    // Initialize sysevent daemon
    if (TelcoVoiceMgrSyseventInit() < 0)
    {
        CcspTraceError(("Sysevent Init failed !!!!\n"));
        return ANSC_STATUS_FAILURE;
    }

    //Send default configuration to Hal
    CcspTraceInfo(("%s %d - sending default configuration to Hal. \n", __FUNCTION__, __LINE__ ));
    returnStatus = TelcoVoiceJsonCfgSetDmDefaults();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        CcspTraceInfo(("%s %d - Error in sending default configuration to Hal. \n", __FUNCTION__, __LINE__ ));
        return returnStatus;
    }

    //Get HAL data
    returnStatus = TelcoVoiceMgrHal_GetInitData();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("TelcoVoiceMgrHal_GetInitData failed !!!!\n"));
        return returnStatus;
    }

    //Register subscribe event callback and send data to hal
    returnStatus = TelcoVoiceMgr_Controller_Init();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("TelcoVoiceMgr_Controller_Init failed !!!!\n"));
        return returnStatus;
    }

    /*
     * Voice Diagnostics init
     * Services.X_RDK_Report.Voice.
     */
    returnStatus = DmlVoiceServiceReportInit( (PANSC_HANDLE)pMyObject );
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("DmlVoiceServiceReportInit failed !!!!\n"));
        return returnStatus;
    }
#ifdef RBUS_BUILD_FLAG_ENABLE
    //Starts the Rbus Initialize
    returnStatus = TelcoVoiceMgr_Rbus_Init();
    if(returnStatus == ANSC_STATUS_SUCCESS)
    {
        TelcoVoiceMgr_Rbus_SubscribeDML();
    }
    else
    {
        CcspTraceError(("%s %d - Rbus Init failed !\n", __FUNCTION__, __LINE__ ));
    }
#endif //RBUS_BUILD_FLAG_ENABLE

    return returnStatus;
}

/* DmlVoiceServiceReportInit */
ANSC_STATUS DmlVoiceServiceReportInit(PANSC_HANDLE phContext)
{
    int                               retPsmGet                        = 0;
    ULONG                             psmValue                         = 0;
    PTELCOVOICE_CONTEXT_LINK_OBJECT   pMyObject                        = (PTELCOVOICE_CONTEXT_LINK_OBJECT)phContext;
    PDML_X_RDK_REPORT_VOICE_SERVICE   pVoiceServiceReportTmp           = NULL;
    PDML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT pVoiceServiceReportDfltTmp = NULL;

    //Parodus init
    initparodusTask();

    pVoiceServiceReportTmp = (PDML_X_RDK_REPORT_VOICE_SERVICE) AnscAllocateMemory( sizeof(DML_X_RDK_REPORT_VOICE_SERVICE) );
    //Return failure if allocation failiure
    if( NULL == pVoiceServiceReportTmp )
    {
        return ANSC_STATUS_FAILURE;
    }

    pVoiceServiceReportDfltTmp = (PDML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT) AnscAllocateMemory (sizeof (DML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT));
    //Return failure if allocation failiure
    if( NULL == pVoiceServiceReportDfltTmp )
    {
        AnscFreeMemory(pVoiceServiceReportTmp);
        return ANSC_STATUS_FAILURE;
    }

    //Memset all memory
    memset( pVoiceServiceReportDfltTmp, 0, sizeof(DML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT) );
    GetNVRamULONGConfiguration(VoiceServiceReportStatusDfltReportingPeriod, &psmValue);
    VoiceServiceReportSetDefaultReportingPeriod(psmValue);
    pVoiceServiceReportDfltTmp->ReportingPeriod = psmValue;

    //Memset all memory
    memset( pVoiceServiceReportTmp, 0, sizeof(DML_X_RDK_REPORT_VOICE_SERVICE) );
    GetNVRamULONGConfiguration(VoiceServiceReportStatusReportingPeriod, &psmValue);
    VoiceServiceReportSetReportingPeriod(psmValue);
    pVoiceServiceReportTmp->ReportingPeriod = psmValue;

    GetNVRamULONGConfiguration(VoiceServiceReportStatusEnable, &psmValue);
    VoiceServiceReportSetStatus(psmValue);
    pVoiceServiceReportTmp->Enabled = psmValue;

    //Assign the memory address to oringinal structure
    pVoiceServiceReportTmp->pVoiceServiceDefaultReport = pVoiceServiceReportDfltTmp;
    pMyObject->pVoiceServiceReport = pVoiceServiceReportTmp;

    return ANSC_STATUS_SUCCESS;
}
/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS TelcoVoiceMgrServicesRemove(ANSC_HANDLE hThisObject);

    description:

        This function initiate  cosa device info object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS TelcoVoiceMgrServicesRemove(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                  returnStatus        = ANSC_STATUS_SUCCESS;
    PTELCOVOICEMGR_DATAMODEL_SERVICES     pMyObject           = (PTELCOVOICEMGR_DATAMODEL_SERVICES)hThisObject;
    bool bStatus = 0;

    TelcoVoiceMgrHal_InitData(bStatus);

    //delete DML data
    TelcoVoiceMgrDmlDataDelete();


    //close sysevent
    TelcoVoiceMgrSyseventDeInit();

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        TelcoVoiceMgrTelcoVoiceCreate
            (
                VOID
            );

    description:

        This function constructs cosa Services object and return handle.

    argument:

    return:     newly created Services object.

**********************************************************************/

ANSC_HANDLE
TelcoVoiceMgrTelcoVoiceCreate
    (
        VOID
    )
{
    ANSC_STATUS                              returnStatus = ANSC_STATUS_SUCCESS;
    PTELCOVOICEMGR_DATAMODEL_TELCOVOICE      pMyObject    = (PTELCOVOICEMGR_DATAMODEL_TELCOVOICE)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */

    pMyObject = (PTELCOVOICEMGR_DATAMODEL_TELCOVOICE)AnscAllocateMemory(sizeof(TELCOVOICEMGR_DATAMODEL_TELCOVOICE));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Create            = TelcoVoiceMgrTelcoVoiceCreate;
    pMyObject->Remove            = TelcoVoiceMgrTelcoVoiceRemove;
    pMyObject->Initialize        = TelcoVoiceMgrTelcoVoiceInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        TelcoVoiceMgrTelcoVoiceInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa device info object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
TelcoVoiceMgrTelcoVoiceInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                           returnStatus        = ANSC_STATUS_SUCCESS;
    PTELCOVOICEMGR_DATAMODEL_TELCOVOICE   pMyObject           = (PTELCOVOICEMGR_DATAMODEL_TELCOVOICE)hThisObject;

    /* Initialize WebConfig */
    TelcoVoiceMgrDmlTelcoVoiceWebConfigInit( );

    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS
        TelcoVoiceMgrTelcoVoiceRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function initiate  cosa device info object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS
TelcoVoiceMgrTelcoVoiceRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                           returnStatus        = ANSC_STATUS_SUCCESS;
    PTELCOVOICEMGR_DATAMODEL_TELCOVOICE   pMyObject           = (PTELCOVOICEMGR_DATAMODEL_TELCOVOICE)hThisObject;
#ifdef RBUS_BUILD_FLAG_ENABLE
    //Starts the Rbus Initialize
    returnStatus = TelcoVoiceMgr_RbusExit();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("%s %d - Rbus Init failed !\n", __FUNCTION__, __LINE__ ));
    }
#endif //RBUS_BUILD_FLAG_ENABLE

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}

BOOL TelcoVoiceMgrAnscValidateInputString(char *pString)
{

    char disallowed_chars[] = "<>%`|'";       /*To avoid command injection */
    int i = 0;

    /* check if pstring doesn't hold NULL or whitespaces */
    if((pString == NULL) || (*pString =='\0')) {
        return FALSE;
    }

    while(pString[i] != '\0')
    {
        if (!strchr(disallowed_chars,pString[i]))
            i++;
        else
            return FALSE;
    }

    return TRUE;
}
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2

ANSC_HANDLE TelcoVoiceMgr_getCallControlLineEnable(BOOL *bEnable)
{
    ANSC_HANDLE ret = ANSC_STATUS_FAILURE;
    hal_param_t req_param;
    ULONG uVsIndex = 1;
    ULONG uLineIndex = 1;
    memset(&req_param, 0, sizeof(req_param));
    snprintf(req_param.name, sizeof(req_param.name), "Device.Services.VoiceService.%d.CallControl.Line.%d.Enable", uVsIndex, uLineIndex);
    if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
    {
       if(strcmp(req_param.value, "true") == 0 || strcmp(req_param.value, "1") == 0)
       {
           *bEnable = true;
       }
       else
       {
           *bEnable = false;
       }
        ret = ANSC_STATUS_SUCCESS;
    }
    return ret;
}

ANSC_HANDLE TelcoVoiceMgr_setCallControlLineEnable(BOOL bEnable)
{
    ANSC_HANDLE ret = ANSC_STATUS_FAILURE;
    char HalName[MAX_STR_LEN] = {0};
    ULONG uVsIndex = 1;
    ULONG uLineIndex = 1;

    snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.CallControl.Line.%d.Enable",uVsIndex,uLineIndex);

    if (TelcoVoiceMgrHal_SetParamBool(HalName,bEnable) == ANSC_STATUS_SUCCESS)
    {
        TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlDataLock = TelcoVoiceMgrDmlGetDataLocked();
        if(pTelcoVoiceMgrDmlDataLock == NULL)
        {
            CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
            goto exit;
        }

        PTELCOVOICEMGR_DML_SERVICE pTVMDmlService = &(pTelcoVoiceMgrDmlDataLock->Service);
        if(pTVMDmlService == NULL)
        {
            CcspTraceError(("%s:%d:: pTVMDmlService: Failed\n", __FUNCTION__, __LINE__));
            goto exit;
        }

        PDML_VOICE_SERVICE_LIST_T pTVMVoiceServiceList = &(pTVMDmlService->VoiceService);
        if(pTVMVoiceServiceList == NULL)
        {
            CcspTraceError(("%s:%d:: pTVMVoiceServiceList: Failed\n", __FUNCTION__, __LINE__));
            goto exit;
        }

        PDML_VOICE_SERVICE_CTRL_T pTVMVoiceServiceCtrl = &(pTVMVoiceServiceList->pdata[uVsIndex - 1]);
        if(pTVMVoiceServiceCtrl == NULL)
        {
            CcspTraceError(("%s:%d:: pTVMVoiceServiceCtrl: Failed\n", __FUNCTION__, __LINE__));
            goto exit;
        }

        PTELCOVOICEMGR_DML_VOICESERVICE pTVMVoiceServiceDml = &(pTVMVoiceServiceCtrl->dml);
        if(pTVMVoiceServiceDml == NULL)
        {
            CcspTraceError(("%s:%d:: pTVMVoiceServiceDml: Failed\n", __FUNCTION__, __LINE__));
            goto exit;
        }

        PDML_CALLCONTROL_LINE_LIST_T pCallCtrlLineList = &(pTVMVoiceServiceDml->CallControl_obj.Line);
        if(pCallCtrlLineList == NULL)
        {
            CcspTraceError(("%s:%d:: pCallCtrlLineList: Failed\n", __FUNCTION__, __LINE__));
            goto exit;
        }

        PDML_CALLCONTROL_LINE_CTRL_T pLineCtrl = &(pCallCtrlLineList->pdata[uLineIndex - 1]);
        if(pLineCtrl == NULL)
        {
            CcspTraceError(("%s:%d:: pLineCtrl: Failed\n", __FUNCTION__, __LINE__));
            goto exit;
        }

        PDML_CALLCONTROL_LINE pHEAD = &(pLineCtrl->dml);
        if(pHEAD == NULL)
        {
            CcspTraceError(("%s:%d:: pHEAD: Failed\n", __FUNCTION__, __LINE__));
            goto exit;
        }

        pHEAD->Enable = bEnable;

        ret = ANSC_STATUS_SUCCESS;
exit:
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlDataLock);

    }
        return ret;
}
#endif //FEATURE_RDKB_VOICE_DM_TR104_V2
