#include "ansc_platform.h"
#include "telcovoicemgr_ssp_global.h"
#include "stdlib.h"
#include "ccsp_dm_api.h"
#include <uuid/uuid.h>
#include "sysevent/sysevent.h"
#include "telcovoicemgr_dml_apis.h"
#include "telcovoicemgr_dml_hal.h"
#include "voice_report.h"
#include "ccsp_trace.h"

char *rt_schema_buffer = NULL;

static BOOL VoiceServiceReportStatus = FALSE;
static ULONG VoiceServiceReportReportingPeriod = DEFAULT_REPORTING_INTERVAL;
static ULONG VoiceServiceReportDfltReportingPeriod = DEFAULT_REPORTING_INTERVAL;
static ULONG VoiceServiceReportOverrideTTL = DEFAULT_OVERRIDE_TTL;
static ULONG CurrentOverrideReportingPeriod = 0;
extern ANSC_HANDLE bus_handle;
char deviceMAC[32] = {'\0'};

static pthread_mutex_t VoiceReportMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t VoiceReportCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t device_mac_mutex = PTHREAD_MUTEX_INITIALIZER;
static BOOL rt_schema_file_parsed = FALSE;
static avro_value_iface_t *iface = NULL;
static char ReportSource[] = "VOICE";
static char CPE_TYPE_STRING[] = "Gateway";
static char *macStr = NULL;
static char CpemacStr[32];
static int consoleDebugEnable = 0;
static int cpe_parent_exists = FALSE;
static size_t AvroRTSerializedSize;
static size_t OneAvroRTSerializedSize;
static char AvroRTSerializedBuf[WRITER_BUF_SIZE];

// MD5SUM VoiceReport.avsc = b3b6d1668867a02f070437e425cf7348 version 2

static uint8_t RT_HASH[16] = {0xbd,0x95,0xe5,0xe3,0x37,0x2a,0x4e,0x03,
                              0xbc,0xf7,0x9a,0x3f,0x31,0x20,0x85,0xa6};

// TODO: Need to check and include correct UUID below
static uint8_t RT_UUID[16] = {0xb3,0xb6,0xd1,0x66,0x88,0x67,0xa0,0x2f,
                              0x07,0x04,0x37,0xe4,0x25,0xcf,0x73,0x48};

char * VoiceServiceReportSchemaID = "bd95e5e3-372a-4e03-bcf7-9a3f312085a6/b3b6d1668867a02f070437e425cf7348";

#define WAIT_TIME_100_MS_IN_NSEC (100 * 1000000)
#define TIME_1_SEC_IN_NSEC 1000000000

/*
 * Returns the size of Schema contents
 */
int VoiceServiceReportGetSchemaBufferSize()
{
    if (rt_schema_buffer)
    {
        return strlen(rt_schema_buffer);
    }
    return 0;
}

/*
 * Returns the schema contents
 */
char *VoiceServiceReportGetSchemaBuffer()
{
    CcspTraceInfo(("FUNC %s LINE %d\n", __FUNCTION__, __LINE__));
    if (rt_schema_buffer)
        return rt_schema_buffer;

    CcspTraceInfo(("FUNC %s LINE %d\n", __FUNCTION__, __LINE__));
    return NULL;
}

/*
 * Returns the schemaID size
 */
int VoiceServiceReportGetSchemaIDBufferSize()
{
    if (VoiceServiceReportGetSchemaIDBuffer())
        return strlen(VoiceServiceReportSchemaID);
    return 0;
}

/*
 * Returns the schemaID
 */
char *VoiceServiceReportGetSchemaIDBuffer()
{
    return VoiceServiceReportSchemaID;
}

/*
 * Checks value of Reporting Period.
 * Returns:     TRUE if ReportingPeriod is valid
 *              FALSE if ReportingPeriod is not valid
 */
BOOL VoiceServiceReportValidateReportingPeriod(UINT value)
{
    int arr[] = {1, 5, 15, 30, 60, 300, 900, 1800, 3600, 10800, 21600, 43200, 86400};
    int i = 0;
    for (i = 0; i < (sizeof(arr) / sizeof(int)); i++)
    {
        if (arr[i] == value)
        {
            return true;
        }
    }
    return false;
}

/*
 * Retuns the VOICE Report Status
 * Returns:     TRUE if VOICE Reporting is Enabled
 *              FALSE if VOICE Reporting is Disabled
 */
BOOL VoiceServiceReportGetStatus()
{
    return VoiceServiceReportStatus;
}

/**
 * @brief Retrieve VOICE data and pack the report structure.
 * This API communicates with HAL layer to retrieve different statistics information
 * and filled with incoming structure object.
 */
static ANSC_STATUS VoiceServicePrepareReportData(int iVoiceServiceID, VoiceService *pstReportData)
{
    TELCOVOICEMGR_DML_VOICESERVICE_STATS  stStats = { 0 };

    VoiceProfile *pstVoiceProfile = NULL;
    int rc = ANSC_STATUS_SUCCESS, iCommonEnumRet = 0;
    unsigned int i, j, voice_profile_id = 0, line_id = 0, phy_interface_id = 0;
    ULONG ulCommonRet = 0;
    char *err;

    hal_param_t req_param;
    memset(&req_param, 0, sizeof(req_param));

    if (NULL == pstReportData)
    {
        CcspTraceError(("%s Invalid Memory\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    //Assign respected structure address
    pstVoiceProfile = pstReportData->astVoiceProfile;

    /*
     * Prepare Voice Service Report Data
     * - Voice Service Object
     */
     memset(pstReportData->VoiceServiceVersion, 0, sizeof(pstReportData->VoiceServiceVersion));

     snprintf(req_param.name, sizeof(req_param.name), HAL_VS_BOUNDIFNAME, iVoiceServiceID);
     if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
     {
         //Fallback case
         memset(pstReportData->BoundIfName, 0, sizeof(pstReportData->BoundIfName));
     }
     else
     {
        snprintf(pstReportData->BoundIfName, sizeof(pstReportData->BoundIfName),"%s",req_param.value);
     }

     snprintf(req_param.name, sizeof(req_param.name), HAL_VS_IPADDRESSFAMILY, iVoiceServiceID);
     if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
     {
         //Fallback case
         memset(pstReportData->IpAddressFamily, 0, sizeof(pstReportData->IpAddressFamily));
     }
     else
     {
        snprintf(pstReportData->IpAddressFamily, sizeof(pstReportData->IpAddressFamily),"%s",req_param.value);
     }

     snprintf(req_param.name, sizeof(req_param.name), HAL_VS_BOUNDIPADDR, iVoiceServiceID);
     if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
     {
         //Fallback case
         memset(pstReportData->BoundIpAddr, 0, sizeof(pstReportData->BoundIpAddr));
     }
     else
     {
        snprintf(pstReportData->BoundIpAddr, sizeof(pstReportData->BoundIpAddr),"%s",req_param.value);
     }

     snprintf(req_param.name, sizeof(req_param.name), HAL_VS_CCTKTRACEGROUP, iVoiceServiceID);
     if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
     {
         //Fallback case
         memset(pstReportData->CCTKTraceGroup, 0, sizeof(pstReportData->CCTKTraceGroup));
     }
     else
     {
        snprintf(pstReportData->CCTKTraceGroup, sizeof(pstReportData->CCTKTraceGroup),"%s",req_param.value);
     }

     snprintf(req_param.name, sizeof(req_param.name), HAL_VS_CCTKTRACELEVEL, iVoiceServiceID);
     if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
     {
         //Fallback case
         memset(pstReportData->CCTKTraceLevel, 0, sizeof(pstReportData->CCTKTraceLevel));
     }
     else
     {
         snprintf(pstReportData->CCTKTraceLevel, sizeof(pstReportData->CCTKTraceLevel),"%s",req_param.value);
     }

     snprintf(req_param.name, sizeof(req_param.name), HAL_VS_ENABLE, iVoiceServiceID);
     if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
     {
         //Fallback case
         memset(pstReportData->VoiceProcessState, 0, sizeof(pstReportData->VoiceProcessState));
     }
     else
     {
         snprintf(pstReportData->VoiceProcessState, sizeof(pstReportData->VoiceProcessState),"%s",req_param.value);
     }

     snprintf(req_param.name, sizeof(req_param.name), HAL_VS_STATUS, iVoiceServiceID);
     if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
     {
         //Fallback case
         memset(pstReportData->VoiceProcessStatus, 0, sizeof(pstReportData->VoiceProcessStatus));
     }
     else
     {
         snprintf(pstReportData->VoiceProcessStatus, sizeof(pstReportData->VoiceProcessStatus),"%s",req_param.value);
     }

     pstReportData->VoiceProfileNumberOfEntries = 1;
     /* Fill Voice Profile Object */
     for( i = 0; i < pstReportData->VoiceProfileNumberOfEntries;  i++ )
     {
        voice_profile_id = i + 1;

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        unsigned int sip_network_id = i + 1;

        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_OBPROXY, iVoiceServiceID, sip_network_id);
#else
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_OBPROXY, iVoiceServiceID, voice_profile_id);
#endif
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
             //Fallback case
             memset(pstVoiceProfile[i].SIPOutboundProxy, 0, sizeof(pstVoiceProfile[i].SIPOutboundProxy));
        }
        else
        {
             snprintf(pstVoiceProfile[i].SIPOutboundProxy, sizeof(pstVoiceProfile[i].SIPOutboundProxy),"%s",req_param.value);
        }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_OBPROXYPORT, iVoiceServiceID, sip_network_id);
#else
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_OBPROXYPORT, iVoiceServiceID, voice_profile_id);
#endif
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            pstVoiceProfile[i].SIPOutboundProxyPort = 0;
        }
        else
        {
             pstVoiceProfile[i].SIPOutboundProxyPort = strtoul(req_param.value,&err,10);
        }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_PROXYSERV, iVoiceServiceID, sip_network_id);
#else
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_PROXYSERV, iVoiceServiceID, voice_profile_id);
#endif
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
             //Fallback case
             memset(pstVoiceProfile[i].SIPProxyServer, 0, sizeof(pstVoiceProfile[i].SIPProxyServer));
        }
        else
        {
             snprintf(pstVoiceProfile[i].SIPProxyServer, sizeof(pstVoiceProfile[i].SIPProxyServer),"%s",req_param.value);
        }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_PROXYSERVPORT, iVoiceServiceID, sip_network_id);
#else
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_PROXYSERVPORT, iVoiceServiceID, voice_profile_id);
#endif
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            pstVoiceProfile[i].SIPProxyServerPort = 0;
        }
        else
        {
             pstVoiceProfile[i].SIPProxyServerPort = strtoul(req_param.value,&err,10);
        }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_REGSERV, iVoiceServiceID, sip_network_id);
#else
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_REGSERV, iVoiceServiceID, voice_profile_id);
#endif
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
             //Fallback case
             memset(pstVoiceProfile[i].SIPRegistrarServer, 0, sizeof(pstVoiceProfile[i].SIPRegistrarServer));
        }
        else
        {
             snprintf(pstVoiceProfile[i].SIPRegistrarServer, sizeof(pstVoiceProfile[i].SIPRegistrarServer),"%s",req_param.value);
        }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_REGSERVPORT, iVoiceServiceID, sip_network_id);
#else
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_REGSERVPORT, iVoiceServiceID, voice_profile_id);
#endif
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            pstVoiceProfile[i].SIPRegistrarServerPort = 0;
        }
        else
        {
            pstVoiceProfile[i].SIPRegistrarServerPort = strtoul(req_param.value,&err,10);
        }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_USERAGENTDOMAIN, iVoiceServiceID, sip_network_id);
#else
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_USERAGENTDOMAIN, iVoiceServiceID, voice_profile_id);
#endif
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
             //Fallback case
            memset(pstVoiceProfile[i].SIPUserAgentDomain, 0, sizeof(pstVoiceProfile[i].SIPUserAgentDomain));
        }
        else
        {
            snprintf(pstVoiceProfile[i].SIPUserAgentDomain, sizeof(pstVoiceProfile[i].SIPUserAgentDomain),"%s",req_param.value);
        }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_CONFURI, iVoiceServiceID, sip_network_id);
#else
        snprintf(req_param.name, sizeof(req_param.name), HAL_SIP_CONFURI, iVoiceServiceID, voice_profile_id);
#endif
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            //Fallback case
            memset(pstVoiceProfile[i].SIPConferencingURI, 0, sizeof(pstVoiceProfile[i].SIPConferencingURI));
        }
        else
        {
            snprintf(pstVoiceProfile[i].SIPConferencingURI, sizeof(pstVoiceProfile[i].SIPConferencingURI),"%s",req_param.value);
        }

        snprintf(req_param.name, sizeof(req_param.name), HAL_VP_DIGITMAP, iVoiceServiceID, voice_profile_id);
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            //Fallback case
            memset(pstVoiceProfile[i].DigitMap, 0, sizeof(pstVoiceProfile[i].DigitMap));
        }
        else
        {
            snprintf(pstVoiceProfile[i].DigitMap, sizeof(pstVoiceProfile[i].DigitMap),"%s",req_param.value);
        }

        snprintf(req_param.name, sizeof(req_param.name), HAL_VP_EDIGITMAP, iVoiceServiceID, voice_profile_id);
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            //Fallback case
            memset(pstVoiceProfile[i].EmergencyDigitMap, 0, sizeof(pstVoiceProfile[i].EmergencyDigitMap));
        }
        else
        {
            snprintf(pstVoiceProfile[i].EmergencyDigitMap, sizeof(pstVoiceProfile[i].EmergencyDigitMap),"%s",req_param.value);
        }

        snprintf(req_param.name, sizeof(req_param.name), HAL_VP_STIMER, iVoiceServiceID, voice_profile_id);
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            pstVoiceProfile[i].SDigitTimer = 0;
        }
        else
        {
             pstVoiceProfile[i].SDigitTimer = strtoul(req_param.value,&err,10);
        }

        snprintf(req_param.name, sizeof(req_param.name), HAL_VP_ZTIMER, iVoiceServiceID, voice_profile_id);
        if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            pstVoiceProfile[i].ZDigitTimer = 0;
        }
        else
        {
             pstVoiceProfile[i].ZDigitTimer = strtoul(req_param.value,&err,10);
        }

        //Fill line object with respect to voice profile
        for( j = 0; j < pstVoiceProfile[i].VoiceProfileNumberOfLines; j++ )
        {
            VoiceLine   *pstLine = pstVoiceProfile[i].astLine;

            line_id = j + 1;

            pstLine->CallWaitingEnable = FALSE;
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            unsigned int callfeature_set_id = j + 1;
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_CW, iVoiceServiceID, callfeature_set_id);
#else
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_CW, iVoiceServiceID, voice_profile_id, line_id);
#endif
            if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
            {
                pstLine->CallWaitingEnable = strstr(req_param.value,"1")?TRUE:FALSE;
            }

            pstLine->MWIEnable = FALSE;
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_MWI, iVoiceServiceID, callfeature_set_id);
#else
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_MWI, iVoiceServiceID, voice_profile_id, line_id);
#endif
            if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
            {
                pstLine->MWIEnable = strstr(req_param.value,"1")?TRUE:FALSE;
            }

            pstLine->ConferenceCallingEnable = FALSE;

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_CONF, iVoiceServiceID, callfeature_set_id);
#else
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_CONF, iVoiceServiceID, voice_profile_id, line_id);
#endif
            if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
            {
                pstLine->ConferenceCallingEnable = strstr(req_param.value,"1")?TRUE:FALSE;
            }

            pstLine->HoldEnable = FALSE;
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_HOLD, iVoiceServiceID, callfeature_set_id);
#else
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_HOLD, iVoiceServiceID, voice_profile_id, line_id);
#endif
            if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
            {
                pstLine->HoldEnable = strstr(req_param.value,"1")?TRUE:FALSE;
            }

            pstLine->PhoneCallerIDEnable = FALSE;
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_CID, iVoiceServiceID, callfeature_set_id);
#else
            snprintf(req_param.name, sizeof(req_param.name), HAL_CF_CID, iVoiceServiceID, voice_profile_id, line_id);
#endif
            if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
            {
                pstLine->PhoneCallerIDEnable = strstr(req_param.value,"1")?TRUE:FALSE;
            }

            snprintf(pstLine->CallerIDName, sizeof(pstLine->CallerIDName),"%s","");

            if( ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetLineCallState( iVoiceServiceID,
                                                                voice_profile_id,
                                                                line_id,
                                                                &ulCommonRet ) )
            {
                CHAR array_list[][16] = { "Idle", "Calling", "Ringing", "Connecting", "InCall", "Hold", "Disconnecting" };

                snprintf(pstLine->CallState, sizeof(pstLine->CallState),"%s",array_list[ulCommonRet]);
            }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            snprintf(req_param.name, sizeof(req_param.name),HAL_LINE_DIRECTORYNUM, iVoiceServiceID, line_id);
#else
            snprintf(req_param.name, sizeof(req_param.name),HAL_LINE_DIRECTORYNUM, iVoiceServiceID, voice_profile_id, line_id);
#endif
            if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
            {
                 //Fallback case
                 memset(pstLine->DirectoryNumber, 0, sizeof(pstLine->DirectoryNumber));
            }
            else
            {
                 snprintf(pstLine->DirectoryNumber, sizeof(pstLine->DirectoryNumber),"%s",req_param.value);
            }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            snprintf(req_param.name, sizeof(req_param.name),HAL_LINE_ENABLE, iVoiceServiceID, line_id);
#else
            snprintf(req_param.name, sizeof(req_param.name),HAL_LINE_ENABLE, iVoiceServiceID, voice_profile_id, line_id);
#endif
            if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
            {
                 memset(pstLine->VoiceServiceEnable, 0, sizeof(pstLine->VoiceServiceEnable));
            }
            else
            {
                snprintf(pstLine->VoiceServiceEnable, sizeof(pstLine->VoiceServiceEnable),"%s",req_param.value);
            }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
             unsigned int sip_client_id = j + 1;
             snprintf(req_param.name, sizeof(req_param.name),HAL_LINE_SIP_AUTHUSER, iVoiceServiceID, sip_client_id );
#else
             snprintf(req_param.name, sizeof(req_param.name),HAL_LINE_SIP_AUTHUSER, iVoiceServiceID, voice_profile_id, line_id);
#endif
             if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
             {
                 //Fallback case
                 memset(pstLine->SIPAuthUserName, 0, sizeof(pstLine->SIPAuthUserName));
             }
             else
             {
                 snprintf(pstLine->SIPAuthUserName, sizeof(pstLine->SIPAuthUserName),"%s",req_param.value);
             }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
             snprintf(req_param.name, sizeof(req_param.name),HAL_LINE_SIP_URI, iVoiceServiceID, sip_client_id);
#else
             snprintf(req_param.name, sizeof(req_param.name),HAL_LINE_SIP_URI, iVoiceServiceID, voice_profile_id, line_id);
#endif
             if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
             {
                 //Fallback case
                 memset(pstLine->SIPURI, 0, sizeof(pstLine->SIPURI));
             }
             else
             {
                 snprintf(pstLine->SIPURI, sizeof(pstLine->SIPURI),"%s",req_param.value);
             }

             if ( ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetLineStats( iVoiceServiceID,
                                                              voice_profile_id,
                                                              line_id,
                                                              &stStats ) )
             {
                 //Assign Stats Values
                 pstLine->AverageFarEndInterarrivalJitter = stStats.FarEndInterarrivalJitter;
                 pstLine->AverageReceiveInterarrivalJitter = stStats.AverageReceiveInterarrivalJitter;
                 pstLine->AverageRoundTripDelay = stStats.AverageRoundTripDelay;
                 pstLine->BytesReceived = stStats.BytesReceived;
                 pstLine->BytesSent = stStats.BytesSent;
                 pstLine->FarEndInterarrivalJitter = stStats.FarEndInterarrivalJitter;
                 pstLine->FarEndPacketLossRate = stStats.FarEndPacketLossRate;
                 pstLine->IncomingCallsAnswered = stStats.IncomingCallsAnswered;
                 pstLine->IncomingCallsConnected = stStats.IncomingCallsConnected;
                 pstLine->IncomingCallsFailed = stStats.IncomingCallsFailed;
                 pstLine->IncomingCallsReceived = stStats.IncomingCallsReceived;
                 pstLine->OutgoingCallsAnswered = stStats.OutgoingCallsAnswered;
                 pstLine->OutgoingCallsAttempted = stStats.OutgoingCallsAttempted;
                 pstLine->OutgoingCallsConnected = stStats.OutgoingCallsConnected;
                 pstLine->OutgoingCallsFailed = stStats.OutgoingCallsFailed;
                 pstLine->PacketsLost = stStats.PacketsLost;
                 pstLine->PacketsReceived = stStats.PacketsReceived;
                 pstLine->PacketsSent = stStats.PacketsSent;
                 pstLine->ReceiveInterarrivalJitter = stStats.ReceiveInterarrivalJitter;
                 pstLine->ReceivePacketLossRate = stStats.ReceivePacketLossRate;
                 pstLine->ResetStatistics = stStats.ResetStatistics;
                 pstLine->RoundTripDelay = stStats.RoundTripDelay;
#ifdef HUB4_SDK_L07
                 pstLine->OverRuns = stStats.Overruns;
                 pstLine->UnderRuns = stStats.Underruns;
                 pstLine->CallsDropped = stStats.CallsDropped;
                 pstLine->TotalCallTime = stStats.TotalCallTime;
                 pstLine->ServerDownTime = stStats.ServerDownTime;
#endif /* HUB4_SDK_L07 */
             }

             if( ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetLineStatus( iVoiceServiceID,
                                                              voice_profile_id,
                                                              line_id,
                                                              &iCommonEnumRet ) )
             {
                 CHAR array_list[][32] = { "Up", "Initializing", "Registering", "Unregistering", "Error", "Testing", "Quiescent", "Disabled" };

                 snprintf(pstLine->Status, sizeof(pstLine->Status),"%s",array_list[iCommonEnumRet]);
             }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
             unsigned int terminal_id = i + 1;
             unsigned int audio_id    = j + 1;
             snprintf(req_param.name, sizeof(req_param.name), HAL_VPROCESS_RXGAIN, iVoiceServiceID, terminal_id, audio_id);
#else
             snprintf(req_param.name, sizeof(req_param.name), HAL_VPROCESS_RXGAIN, iVoiceServiceID, voice_profile_id, line_id);
#endif
             if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
             {
                pstLine->ReceiveGain = strtoul(req_param.value,&err,10);
             }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
             snprintf(req_param.name, sizeof(req_param.name), HAL_VPROCESS_TXGAIN, iVoiceServiceID, terminal_id, audio_id);
#else
             snprintf(req_param.name, sizeof(req_param.name), HAL_VPROCESS_TXGAIN, iVoiceServiceID, voice_profile_id, line_id);
#endif
             if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
             {
                pstLine->TransmitGain = strtoul(req_param.value,&err,10);
             }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
             snprintf(req_param.name, sizeof(req_param.name), HAL_LINE_OBPROXYADDR, iVoiceServiceID, sip_network_id);
#else
             snprintf(req_param.name, sizeof(req_param.name), HAL_LINE_OBPROXYADDR, iVoiceServiceID, voice_profile_id, line_id);
#endif
             if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
             {
                 //Fallback case
                 memset(pstLine->OutboundProxyAddresses, 0, sizeof(pstLine->OutboundProxyAddresses));
             }
             else
             {
                 snprintf(pstLine->OutboundProxyAddresses, sizeof(pstLine->OutboundProxyAddresses),"%s",req_param.value);
             }
        }

     }

     return ANSC_STATUS_SUCCESS;
}

/*
 * Function to prepare Avro Writer
 */
avro_writer_t prepare_rt_writer(void)
{
    avro_writer_t writer = NULL;
    long lsSize = 0;

    CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));

    CcspTraceInfo(("Avro prepares to serialize data\n"));

    if (rt_schema_file_parsed == FALSE)
    {
        CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));
        FILE *fp;

        /* open schema file */
        fp = fopen(VOICE_AVRO_SCHEMA_FILE, "rb");
        if (!fp)
        {
            CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));
            perror(VOICE_AVRO_SCHEMA_FILE " doesn't exist.");
            CcspTraceInfo(("%s file does'nt exist\n", VOICE_AVRO_SCHEMA_FILE));
            return NULL;
        }
        CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));

        /* seek through file and get file size*/
        fseek(fp, 0L, SEEK_END);
        lsSize = ftell(fp);
        CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));

        /*back to the start of the file*/
        rewind(fp);

        /* allocate memory for entire content */
        rt_schema_buffer = calloc(1, lsSize + 1);
        memset(rt_schema_buffer, 0, lsSize + 1);
        CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));

        if (!rt_schema_buffer)
        {
            fclose(fp);
            fputs("memory alloc fails", stderr);
            CcspTraceInfo(("Unable to allocate memory\n"));
            return NULL;
        }
        CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));

        /* copy the file into the RT_schema_buffer */
        if (1 != fread(rt_schema_buffer, lsSize, 1, fp))
        {
            fclose(fp);
            CcspTraceInfo(("VOICE REPORT %s : LINE %d rt_schema_buffer = %p\n", __FUNCTION__, __LINE__, rt_schema_buffer));
            free(rt_schema_buffer);
            fputs("entire read fails", stderr);
            CcspTraceInfo(("fread() failed\n"));
            return NULL;
        }

        CcspTraceInfo(("VOICE REPORT %s : LINE %d rt_schema_buffer = %p\n", __FUNCTION__, __LINE__, rt_schema_buffer));
        fclose(fp);

        //schemas
        avro_schema_error_t error = NULL;

        CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));
        //Master report/datum
        avro_schema_t voice_report_schema = NULL;
        CcspTraceInfo(("VOICE REPORT %s : LINE %d rt_schema_buffer - %s\n", __FUNCTION__, __LINE__, rt_schema_buffer));
        avro_schema_from_json(rt_schema_buffer, strlen(rt_schema_buffer),
                              &voice_report_schema, &error);

        if (CHK_AVRO_ERR)
        {
            CcspTraceInfo(("%s %s : %d avro_schema_from_json fail:\n", avro_strerror(), __func__, __LINE__));
            return NULL;
        }
        CcspTraceInfo(("VOICE REPORT %s : LINE %d voice_report_schema = %p\n", __FUNCTION__, __LINE__, voice_report_schema));

        //generate an avro class from our schema and get a pointer to the value interface
        iface = avro_generic_class_from_schema(voice_report_schema);
        if (CHK_AVRO_ERR)
        {
            CcspTraceInfo(("%s iface = %p\n", avro_strerror(), iface));
            return NULL;
        }
        CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));
        avro_schema_decref(voice_report_schema);
        CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));
        rt_schema_file_parsed = TRUE; // parse schema file once only
        CcspTraceInfo(("Read Avro schema file ONCE, lsSize = %ld, RT_schema_buffer = 0x%lx.\n", lsSize + 1, (ulong)rt_schema_buffer));
    }
    else
        CcspTraceInfo(("Stored lsSize = %ld, pRT_schema_buffer = 0x%lx.\n", lsSize + 1, (ulong)rt_schema_buffer));

    CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));
    memset(&AvroRTSerializedBuf[0], 0, sizeof(AvroRTSerializedBuf));

    AvroRTSerializedBuf[0] = MAGIC_NUMBER; /* fill MAGIC number */
    memcpy(&AvroRTSerializedBuf[MAGIC_NUMBER_SIZE], RT_UUID, sizeof(RT_UUID));
    memcpy(&AvroRTSerializedBuf[MAGIC_NUMBER_SIZE + sizeof(RT_UUID)], RT_HASH, sizeof(RT_HASH));
    CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));
    writer = avro_writer_memory((char *)&AvroRTSerializedBuf[MAGIC_NUMBER_SIZE + SCHEMA_ID_LENGTH],
                                sizeof(AvroRTSerializedBuf) - MAGIC_NUMBER_SIZE - SCHEMA_ID_LENGTH);

    CcspTraceInfo(("VOICE REPORT %s : LINE %d \n", __FUNCTION__, __LINE__));
    CcspTraceInfo(("VOICE REPORT %s : EXIT \n", __FUNCTION__));

    return writer;
}

/*
 * VOICE Data Packing Function
 */
static int harvester_report_voice_service(VoiceServiceReportData *head)
{
    int k = 0, i, j;
    avro_writer_t writer;
    uint8_t *b64buffer = NULL;
    char *serviceName = "VOICE";
    char *dest = "event:raw.kestrel.reports.VoiceDiagnostics";
    char *contentType = "avro/binary"; // contentType "application/json", "avro/binary"
    size_t decodesize = 0;
    char trans_id[37];
    uuid_t transaction_id;
    VoiceServiceReportData *ptr = head;

    CcspTraceInfo(("VOICE REPORT %s : ENTER \n", __FUNCTION__));

    // goes thru total number of elements in link list
    writer = prepare_rt_writer();

    if (writer == NULL)
        return 1;

    //Reset out writer
    avro_writer_reset(writer);

    avro_value_t adr = {0};
    avro_value_t adrField = {0};
    avro_value_t optional = {0};

    avro_generic_value_new(iface, &adr);
    if (CHK_AVRO_ERR)
    {
        CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
        return 1;
    }
    CcspTraceInfo(("VOICE Report\tType: %d\n", avro_value_get_type(&adr)));

    // header block
    // timestamp
    avro_value_get_by_name(&adr, "header", &adrField, NULL);
    if (CHK_AVRO_ERR)
        CcspTraceInfo(("%s\n", avro_strerror()));
    avro_value_get_by_name(&adrField, "timestamp", &adrField, NULL);
    avro_value_set_branch(&adrField, 1, &optional);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d \n", avro_strerror(), __LINE__));

    struct timeval ts;
    gettimeofday(&ts, NULL);

    uint64_t tstamp_av_main = ((uint64_t)(ts.tv_sec) * 1000000) + (uint64_t)ts.tv_usec;
    tstamp_av_main = tstamp_av_main / 1000; //timestamp in ms

    avro_value_set_long(&optional, tstamp_av_main);
    CcspTraceInfo(("timestamp = %llu\n", tstamp_av_main));
    CcspTraceInfo(("timestamp\tType: %d\n", avro_value_get_type(&optional)));
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));

    // uuid
    uuid_generate_random(transaction_id);
    uuid_unparse(transaction_id, trans_id);

    avro_value_get_by_name(&adr, "header", &adrField, NULL);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s __LINE__\n", avro_strerror(), __LINE__));
    avro_value_get_by_name(&adrField, "uuid", &adrField, NULL);
    avro_value_set_branch(&adrField, 1, &optional);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
    avro_value_set_fixed(&optional, transaction_id, 16);
    unsigned char *ptxn = (unsigned char *)transaction_id;
    CcspTraceInfo(("uuid = 0x%02X, 0x%02X ... 0x%02X, 0x%02X\n", ptxn[0], ptxn[1], ptxn[14], ptxn[15]));
    CcspTraceInfo(("uuid\tType: %d\n", avro_value_get_type(&optional)));
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d \n", avro_strerror(), __LINE__));

    //source
    avro_value_get_by_name(&adr, "header", &adrField, NULL);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d \n", avro_strerror(), __LINE__));
    avro_value_get_by_name(&adrField, "source", &adrField, NULL);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
    avro_value_set_branch(&adrField, 0, &optional);
    avro_value_set_string(&optional, ReportSource);
    CcspTraceInfo(("source = \"%s\"\n", ReportSource));
    CcspTraceInfo(("source\tType: %d\n", avro_value_get_type(&optional)));
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));

    //cpe_id block
    // MacAddress
    if (macStr == NULL)
    {
        macStr = getDeviceMac();

        strncpy(CpemacStr, macStr, sizeof(CpemacStr));
        CcspTraceInfo(("Received DeviceMac is: %s\n", macStr));
    }

    char CpeMacHoldingBuf[20] = {0};
    unsigned char CpeMacid[7] = {0};
    for (k = 0; k < 6; k++)
    {
        /* copy 2 bytes */
        CpeMacHoldingBuf[k * 2] = CpemacStr[k * 2];
        CpeMacHoldingBuf[k * 2 + 1] = CpemacStr[k * 2 + 1];
        CpeMacid[k] = (unsigned char)strtol(&CpeMacHoldingBuf[k * 2], NULL, 16);
    }
    avro_value_get_by_name(&adr, "cpe_id", &adrField, NULL);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
    avro_value_get_by_name(&adrField, "mac_address", &adrField, NULL);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
    avro_value_set_branch(&adrField, 1, &optional);
    avro_value_set_fixed(&optional, CpeMacid, 6);
    unsigned char *pMac = (unsigned char *)CpeMacid;
    CcspTraceInfo(("mac_address = 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", pMac[0], pMac[1], pMac[2], pMac[3], pMac[4], pMac[5]));
    CcspTraceInfo(("mac_address\tType: %d\n", avro_value_get_type(&optional)));
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s\n", avro_strerror()));

    // cpe_type
    avro_value_get_by_name(&adr, "cpe_id", &adrField, NULL);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
    avro_value_get_by_name(&adrField, "cpe_type", &adrField, NULL);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d \n", avro_strerror(), __LINE__));
    avro_value_set_branch(&adrField, 0, &optional);
    avro_value_set_string(&optional, CPE_TYPE_STRING);
    CcspTraceInfo(("cpe_type = \"%s\"\n", CPE_TYPE_STRING));
    CcspTraceInfo(("cpe_type\tType: %d\n", avro_value_get_type(&optional)));
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));

    // cpe_parent - Recurrsive CPEIdentifier block
    avro_value_get_by_name(&adr, "cpe_id", &adrField, NULL);
    if ( CHK_AVRO_ERR ) CcspTraceInfo(("%d LINE %s\n", __LINE__,avro_strerror()));
    avro_value_get_by_name(&adrField, "cpe_parent", &adrField, NULL);
    if ( CHK_AVRO_ERR ) CcspTraceInfo(("%d LINE %s\n", __LINE__,avro_strerror()));

    if ( cpe_parent_exists == FALSE )
    {
       avro_value_set_branch(&adrField, 0, &optional);
       avro_value_set_null(&optional);
       CcspTraceInfo(("cpe_parent = %s\n", "NULL" ));
       CcspTraceInfo(("cpe_parent\tType: %d\n", avro_value_get_type(&optional)));
       if ( CHK_AVRO_ERR ) CcspTraceInfo(("%d LINE %s\n", __LINE__,avro_strerror()));
    }

    //Data Field block
    avro_value_get_by_name(&adr, "data", &adrField, NULL);
    if (CHK_AVRO_ERR)
        CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));

    //drfield now contains a reference to the Voice Service Reports Array
    //Voice Service Report
    avro_value_t dr = {0};

    //Current Voice Service Report Field
    avro_value_t drField = {0};

    for( i = 0; i < ptr->VoiceServiceNumberOfEntries; i++ )
    {
        VoiceService   *pstVoiceService = &(ptr->pstVoiceService[i]);

        CcspTraceInfo(("LINE %d Current Link List Ptr = [0x%lx], numServices = %d\n", __LINE__,(ulong)pstVoiceService, ptr->VoiceServiceNumberOfEntries ));
        CcspTraceInfo(("LINE %d Voice Service entry #: %d\n",__LINE__,i + 1));

        //Append a Voice Service Report item to array
        avro_value_append(&adrField, &dr, NULL);
        CcspTraceInfo(("RDK_LOG_DEBUG, \tInterface Report\tType: %d\n", avro_value_get_type(&dr)));

        // VoiceServiceVersion
        avro_value_get_by_name(&dr, "VoiceServiceVersion", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_string(&optional, pstVoiceService->VoiceServiceVersion);
        CcspTraceInfo(("VoiceServiceVersion\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // BoundIfName
        avro_value_get_by_name(&dr, "BoundIfName", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_string(&optional, pstVoiceService->BoundIfName);
        CcspTraceInfo(("BoundIfName\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // IpAddressFamily
        avro_value_get_by_name(&dr, "IpAddressFamily", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_string(&optional, pstVoiceService->IpAddressFamily);
        CcspTraceInfo(("IpAddressFamily\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // BoundIpAddr
        avro_value_get_by_name(&dr, "BoundIpAddr", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_string(&optional, pstVoiceService->BoundIpAddr);
        CcspTraceInfo(("BoundIpAddr\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // CCTKTraceGroup
        avro_value_get_by_name(&dr, "CCTKTraceGroup", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_string(&optional, pstVoiceService->CCTKTraceGroup);
        CcspTraceInfo(("CCTKTraceGroup\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // CCTKTraceLevel
        avro_value_get_by_name(&dr, "CCTKTraceLevel", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_string(&optional, pstVoiceService->CCTKTraceLevel);
        CcspTraceInfo(("CCTKTraceLevel\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // VoiceProcessState
        avro_value_get_by_name(&dr, "VoiceProcessState", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_string(&optional, pstVoiceService->VoiceProcessState);
        CcspTraceInfo(("VoiceProcessState\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // VoiceProcessStatus
        avro_value_get_by_name(&dr, "VoiceProcessStatus", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_string(&optional, pstVoiceService->VoiceProcessStatus);
        CcspTraceInfo(("VoiceProcessStatus\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // MaxServiceCount
        avro_value_get_by_name(&dr, "MaxServiceCount", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_double(&optional, (double)ptr->MaxServiceCount);
        CcspTraceInfo(("MaxServiceCount\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // MaxPhyInterfaceCount
        avro_value_get_by_name(&dr, "MaxPhyInterfaceCount", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_double(&optional, (double)pstVoiceService->MaxPhyInterfaceCount);
        CcspTraceInfo(("MaxPhyInterfaceCount\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // MaxProfileCount
        avro_value_get_by_name(&dr, "MaxProfileCount", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_double(&optional, (double)pstVoiceService->MaxProfileCount);
        CcspTraceInfo(("MaxProfileCount\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // VoiceServiceNumberOfEntries
        avro_value_get_by_name(&dr, "VoiceServiceNumberOfEntries", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_double(&optional, (double)ptr->VoiceServiceNumberOfEntries);
        CcspTraceInfo(("VoiceServiceNumberOfEntries\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // PhyInterfaceNumberOfEntries
        avro_value_get_by_name(&dr, "PhyInterfaceNumberOfEntries", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_double(&optional, (double)pstVoiceService->PhyInterfaceNumberOfEntries);
        CcspTraceInfo(("PhyInterfaceNumberOfEntries\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        // VoiceProfileNumberOfEntries
        avro_value_get_by_name(&dr, "VoiceProfileNumberOfEntries", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
        avro_value_set_branch(&drField, 1, &optional);
        avro_value_set_double(&optional, (double)pstVoiceService->VoiceProfileNumberOfEntries);
        CcspTraceInfo(("VoiceProfileNumberOfEntries\tType: %d\n", avro_value_get_type(&optional)));
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

        //Voice Profiles Fields
        avro_value_get_by_name(&dr, "VoiceProfiles", &drField, NULL);
        if (CHK_AVRO_ERR)
            CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));

        //Voice Profiles Data
        for( j = 0; j < pstVoiceService->VoiceProfileNumberOfEntries; j++ )
        {
            //Current Voice Service Profiles Report Field
            avro_value_t drVoiceProf = {0};
            avro_value_t drVoiceProfField = {0};
            VoiceProfile *pstVoiceProfile = &(pstVoiceService->astVoiceProfile[j]);

            CcspTraceInfo(("LINE %d Voice Service Profile entry #: %d\n",__LINE__,j + 1));

            //Append a Voice Service Profiles Report item to array
            avro_value_append(&drField, &drVoiceProf, NULL);
            CcspTraceInfo(("RDK_LOG_DEBUG, \tVoice Profile Report\tType: %d\n", avro_value_get_type(&drVoiceProf)));

            // SIPOutboundProxy
            avro_value_get_by_name(&drVoiceProf, "SIPOutboundProxy", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_string(&optional, pstVoiceProfile->SIPOutboundProxy);
            CcspTraceInfo(("SIPOutboundProxy\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // SIPOutboundProxyPort
            avro_value_get_by_name(&drVoiceProf, "SIPOutboundProxyPort", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_double(&optional, (double)pstVoiceProfile->SIPOutboundProxyPort);
            CcspTraceInfo(("SIPOutboundProxyPort\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // SIPProxyServer
            avro_value_get_by_name(&drVoiceProf, "SIPProxyServer", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_string(&optional, pstVoiceProfile->SIPProxyServer);
            CcspTraceInfo(("SIPProxyServer\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // SIPProxyServerPort
            avro_value_get_by_name(&drVoiceProf, "SIPProxyServerPort", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_double(&optional, (double)pstVoiceProfile->SIPProxyServerPort);
            CcspTraceInfo(("SIPProxyServerPort\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // SIPRegistrarServer
            avro_value_get_by_name(&drVoiceProf, "SIPRegistrarServer", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_string(&optional, pstVoiceProfile->SIPRegistrarServer);
            CcspTraceInfo(("SIPRegistrarServer\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // SIPRegistrarServerPort
            avro_value_get_by_name(&drVoiceProf, "SIPRegistrarServerPort", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_double(&optional, (double)pstVoiceProfile->SIPRegistrarServerPort);
            CcspTraceInfo(("SIPRegistrarServerPort\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // SIPUserAgentDomain
            avro_value_get_by_name(&drVoiceProf, "SIPUserAgentDomain", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_string(&optional, pstVoiceProfile->SIPUserAgentDomain);
            CcspTraceInfo(("SIPUserAgentDomain\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // SIPConferencingURI
            avro_value_get_by_name(&drVoiceProf, "SIPConferencingURI", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_string(&optional, pstVoiceProfile->SIPConferencingURI);
            CcspTraceInfo(("SIPConferencingURI\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // DigitMap
            avro_value_get_by_name(&drVoiceProf, "DigitMap", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_string(&optional, pstVoiceProfile->DigitMap);
            CcspTraceInfo(("DigitMap\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // DigitMap
            avro_value_get_by_name(&drVoiceProf, "EmergencyDigitMap", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_string(&optional, pstVoiceProfile->EmergencyDigitMap);
            CcspTraceInfo(("EmergencyDigitMap\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // SDigitTimer
            avro_value_get_by_name(&drVoiceProf, "SDigitTimer", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_double(&optional, (double)pstVoiceProfile->SDigitTimer);
            CcspTraceInfo(("SDigitTimer\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // ZDigitTimer
            avro_value_get_by_name(&drVoiceProf, "ZDigitTimer", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_double(&optional, (double)pstVoiceProfile->ZDigitTimer);
            CcspTraceInfo(("ZDigitTimer\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s\n", avro_strerror()));

            // MaxLineCount
            avro_value_get_by_name(&drVoiceProf, "MaxLineCount", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_double(&optional, (double)pstVoiceProfile->MaxLineCount);
            CcspTraceInfo(("MaxLineCount\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

            // VoiceProfileNumberOfLines
            avro_value_get_by_name(&drVoiceProf, "VoiceProfileNumberOfLines", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
            avro_value_set_branch(&drVoiceProfField, 1, &optional);
            avro_value_set_double(&optional, (double)pstVoiceProfile->VoiceProfileNumberOfLines);
            CcspTraceInfo(("VoiceProfileNumberOfLines\tType: %d\n", avro_value_get_type(&optional)));
            if (CHK_AVRO_ERR)
            CcspTraceError(("%s\n", avro_strerror()));

            //Voice Lines Fields
            avro_value_get_by_name(&drVoiceProf, "Lines", &drVoiceProfField, NULL);
            if (CHK_AVRO_ERR)
                CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));

            //Voice Lines Data
            for( k = 0; k < pstVoiceProfile->VoiceProfileNumberOfLines; k++ )
            {
                //Current Voice Lines Report Field
                avro_value_t drLine = {0};
                avro_value_t drLineField = {0};
                VoiceLine *pstLine = &(pstVoiceProfile->astLine[k]);

                CcspTraceInfo(("LINE %d Voice Line entry #: %d\n",__LINE__,k + 1));

                //Append a Voice Service Profiles Report item to array
                avro_value_append(&drVoiceProfField, &drLine, NULL);
                CcspTraceInfo(("RDK_LOG_DEBUG, \tVoice Line Report\tType: %d\n", avro_value_get_type(&drLine)));

                // CallWaitingEnable
                avro_value_get_by_name(&drLine, "CallWaitingEnable", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_boolean(&optional, pstLine->CallWaitingEnable);
                CcspTraceInfo(("CallWaitingEnable\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s\n", avro_strerror()));

                // MWIEnable
                avro_value_get_by_name(&drLine, "MWIEnable", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_boolean(&optional, pstLine->MWIEnable);
                CcspTraceInfo(("MWIEnable\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s\n", avro_strerror()));

                // ConferenceCallingEnable
                avro_value_get_by_name(&drLine, "ConferenceCallingEnable", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_boolean(&optional, pstLine->ConferenceCallingEnable);
                CcspTraceInfo(("ConferenceCallingEnable\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s\n", avro_strerror()));

                // HoldEnable
                avro_value_get_by_name(&drLine, "HoldEnable", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_boolean(&optional, pstLine->HoldEnable);
                CcspTraceInfo(("HoldEnable\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s\n", avro_strerror()));

                // PhoneCallerIDEnable
                avro_value_get_by_name(&drLine, "PhoneCallerIDEnable", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_boolean(&optional, pstLine->PhoneCallerIDEnable);
                CcspTraceInfo(("PhoneCallerIDEnable\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s\n", avro_strerror()));

                // CallerIDName
                avro_value_get_by_name(&drLine, "CallerIDName", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_string(&optional, pstLine->CallerIDName);
                CcspTraceInfo(("CallerIDName\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s\n", avro_strerror()));

                // CallState
                avro_value_get_by_name(&drLine, "CallState", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_string(&optional, pstLine->CallState);
                CcspTraceInfo(("CallState\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s\n", avro_strerror()));

                // DirectoryNumber
                avro_value_get_by_name(&drLine, "DirectoryNumber", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_string(&optional, pstLine->DirectoryNumber);
                CcspTraceInfo(("DirectoryNumber\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s\n", avro_strerror()));

                // VoiceServiceEnable
                avro_value_get_by_name(&drLine, "VoiceServiceEnable", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_string(&optional, pstLine->VoiceServiceEnable);
                CcspTraceInfo(("VoiceServiceEnable\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceError(("%s\n", avro_strerror()));

                // SIPAuthUserName
                avro_value_get_by_name(&drLine, "SIPAuthUserName", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_string(&optional, pstLine->SIPAuthUserName);
                CcspTraceInfo(("SIPAuthUserName\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // SIPURI
                avro_value_get_by_name(&drLine, "SIPURI", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_string(&optional, pstLine->SIPURI);
                CcspTraceInfo(("SIPURI\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // AverageFarEndInterarrivalJitter
                avro_value_get_by_name(&drLine, "AverageFarEndInterarrivalJitter", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->AverageFarEndInterarrivalJitter);
                CcspTraceInfo(("AverageFarEndInterarrivalJitter\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // AverageReceiveInterarrivalJitter
                avro_value_get_by_name(&drLine, "AverageReceiveInterarrivalJitter", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->AverageReceiveInterarrivalJitter);
                CcspTraceInfo(("AverageReceiveInterarrivalJitter\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // AverageRoundTripDelay
                avro_value_get_by_name(&drLine, "AverageRoundTripDelay", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->AverageRoundTripDelay);
                CcspTraceInfo(("AverageRoundTripDelay\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // BytesReceived
                avro_value_get_by_name(&drLine, "BytesReceived", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->BytesReceived);
                CcspTraceInfo(("BytesReceived\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // BytesSent
                avro_value_get_by_name(&drLine, "BytesSent", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->BytesSent);
                CcspTraceInfo(("BytesSent\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // FarEndInterarrivalJitter
                avro_value_get_by_name(&drLine, "FarEndInterarrivalJitter", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->FarEndInterarrivalJitter);
                CcspTraceInfo(("FarEndInterarrivalJitter\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // FarEndPacketLossRate
                avro_value_get_by_name(&drLine, "FarEndPacketLossRate", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->FarEndPacketLossRate);
                CcspTraceInfo(("FarEndPacketLossRate\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // IncomingCallsAnswered
                avro_value_get_by_name(&drLine, "IncomingCallsAnswered", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->IncomingCallsAnswered);
                CcspTraceInfo(("IncomingCallsAnswered\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // IncomingCallsConnected
                avro_value_get_by_name(&drLine, "IncomingCallsConnected", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->IncomingCallsConnected);
                CcspTraceInfo(("IncomingCallsConnected\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // IncomingCallsFailed
                avro_value_get_by_name(&drLine, "IncomingCallsFailed", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->IncomingCallsFailed);
                CcspTraceInfo(("IncomingCallsFailed\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // IncomingCallsReceived
                avro_value_get_by_name(&drLine, "IncomingCallsReceived", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->IncomingCallsReceived);
                CcspTraceInfo(("IncomingCallsReceived\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // OutgoingCallsAnswered
                avro_value_get_by_name(&drLine, "OutgoingCallsAnswered", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->OutgoingCallsAnswered);
                CcspTraceInfo(("OutgoingCallsAnswered\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // OutgoingCallsAttempted
                avro_value_get_by_name(&drLine, "OutgoingCallsAttempted", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->OutgoingCallsAttempted);
                CcspTraceInfo(("OutgoingCallsAttempted\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // OutgoingCallsConnected
                avro_value_get_by_name(&drLine, "OutgoingCallsConnected", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->OutgoingCallsConnected);
                CcspTraceInfo(("OutgoingCallsConnected\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // OutgoingCallsFailed
                avro_value_get_by_name(&drLine, "OutgoingCallsFailed", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->OutgoingCallsFailed);
                CcspTraceInfo(("OutgoingCallsFailed\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // PacketsLost
                avro_value_get_by_name(&drLine, "PacketsLost", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->PacketsLost);
                CcspTraceInfo(("PacketsLost\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // PacketsReceived
                avro_value_get_by_name(&drLine, "PacketsReceived", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->PacketsReceived);
                CcspTraceInfo(("PacketsReceived\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // PacketsSent
                avro_value_get_by_name(&drLine, "PacketsSent", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->PacketsSent);
                CcspTraceInfo(("PacketsSent\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // ReceiveInterarrivalJitter
                avro_value_get_by_name(&drLine, "ReceiveInterarrivalJitter", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->ReceiveInterarrivalJitter);
                CcspTraceInfo(("ReceiveInterarrivalJitter\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // ReceivePacketLossRate
                avro_value_get_by_name(&drLine, "ReceivePacketLossRate", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->ReceivePacketLossRate);
                CcspTraceInfo(("ReceivePacketLossRate\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // ResetStatistics
                avro_value_get_by_name(&drLine, "ResetStatistics", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_boolean(&optional, pstLine->ResetStatistics);
                CcspTraceInfo(("ResetStatistics\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // RoundTripDelay
                avro_value_get_by_name(&drLine, "RoundTripDelay", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->RoundTripDelay);
                CcspTraceInfo(("RoundTripDelay\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // Status
                avro_value_get_by_name(&drLine, "Status", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_string(&optional, pstLine->Status);
                CcspTraceInfo(("Status\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // ReceiveGain
                avro_value_get_by_name(&drLine, "ReceiveGain", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->ReceiveGain);
                CcspTraceInfo(("ReceiveGain\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // TransmitGain
                avro_value_get_by_name(&drLine, "TransmitGain", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_double(&optional, (double)pstLine->TransmitGain);
                CcspTraceInfo(("TransmitGain\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // OutboundProxyAddresses
                avro_value_get_by_name(&drLine, "OutboundProxyAddresses", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_string(&optional, pstLine->OutboundProxyAddresses);
                CcspTraceInfo(("OutboundProxyAddresses\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // OverRuns
                avro_value_get_by_name(&drLine, "OverRuns", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->OverRuns);
                CcspTraceInfo(("OverRuns\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // UnderRuns
                avro_value_get_by_name(&drLine, "UnderRuns", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->UnderRuns);
                CcspTraceInfo(("UnderRuns\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // CallsDropped
                avro_value_get_by_name(&drLine, "CallsDropped", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->CallsDropped);
                CcspTraceInfo(("CallsDropped\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // TotalCallTime
                avro_value_get_by_name(&drLine, "TotalCallTime", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->TotalCallTime);
                CcspTraceInfo(("TotalCallTime\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));

                // ServerDownTime
                avro_value_get_by_name(&drLine, "ServerDownTime", &drLineField, NULL);
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s LINE %d\n", avro_strerror(), __LINE__));
                avro_value_set_branch(&drLineField, 1, &optional);
                avro_value_set_long(&optional, (long)pstLine->ServerDownTime);
                CcspTraceInfo(("ServerDownTime\tType: %d\n", avro_value_get_type(&optional)));
                if (CHK_AVRO_ERR)
                    CcspTraceInfo(("%s\n", avro_strerror()));
            }
        }

        /* check for writer size, if buffer is almost full, skip trailing linklist */
        avro_value_sizeof(&adr, &AvroRTSerializedSize);
        OneAvroRTSerializedSize = (OneAvroRTSerializedSize == 0) ? AvroRTSerializedSize : OneAvroRTSerializedSize;

        if ((WRITER_BUF_SIZE - AvroRTSerializedSize) < OneAvroRTSerializedSize)
        {
            CcspTraceWarning(("AVRO write buffer is almost full, size = %d func %s, exit!\n", (int)AvroRTSerializedSize, __FUNCTION__));
            break;
        }
    }

    //Thats the end of that
    avro_value_write(writer, &adr);

    avro_value_sizeof(&adr, &AvroRTSerializedSize);
    AvroRTSerializedSize += MAGIC_NUMBER_SIZE + SCHEMA_ID_LENGTH;
    CcspTraceInfo(("Serialized writer size %d\n", (int)AvroRTSerializedSize));

    //Free up memory
    avro_value_decref(&adr);

    CcspTraceInfo(("VOICE REPORT %s : LINE %d free()\n", __FUNCTION__, __LINE__));
    avro_writer_free(writer);

    // b64 encoding
    decodesize = b64_get_encoded_buffer_size(AvroRTSerializedSize);
    b64buffer = malloc(decodesize * sizeof(uint8_t));
    if (b64buffer != NULL)
    {
        b64_encode((uint8_t *)AvroRTSerializedBuf, AvroRTSerializedSize, b64buffer);

        if (consoleDebugEnable)
        {
            fprintf(stderr, "\nAVro serialized data\n");
            for (k = 0; k < (int)AvroRTSerializedSize; k++)
            {
                char buf[30];
                if ((k % 32) == 0)
                    fprintf(stderr, "\n");
                sprintf(buf, "%02X", (unsigned char)AvroRTSerializedBuf[k]);
                fprintf(stderr, "%c%c", buf[0], buf[1]);
            }

            fprintf(stderr, "\n\nB64 data\n");
            for (k = 0; k < (int)decodesize; k++)
            {
                if ((k % 32) == 0)
                    fprintf(stderr, "\n");
                fprintf(stderr, "%c", b64buffer[k]);
            }
            fprintf(stderr, "\n\n");
        }
        free(b64buffer);
    }

    CcspTraceInfo(("Before RT WebPA SEND message call\n"));
    CcspTraceInfo(("serviceName: %s\n", serviceName));
    CcspTraceInfo(("dest: %s\n", dest));
    CcspTraceInfo(("trans_id: %s\n", trans_id));
    CcspTraceInfo(("contentType: %s\n", contentType));
    CcspTraceInfo(("AvroRTSerializedBuf: %s\n", AvroRTSerializedBuf));
    CcspTraceInfo(("AvroRTSerializedSize: %d\n", (int)AvroRTSerializedSize));

    // Send data from VOICE REPORT to webpa using CCSP bus interface
    sendWebpaMsg(serviceName, dest, trans_id, contentType, AvroRTSerializedBuf, AvroRTSerializedSize);
    CcspTraceInfo(("%s report sent to Webpa, Destination=%s, Transaction-Id=%s  \n", VOICE_REPORT_NAME, dest, trans_id));

    CcspTraceInfo(("After RT WebPA SEND message call\n"));

    CcspTraceInfo(("VOICE REPORT %s : LINE %d free()\n", __FUNCTION__, __LINE__));

    CcspTraceInfo(("VOICE REPORT %s : EXIT \n", __FUNCTION__));

    return 0;
}

/*
 * Avroe cleanup function
 */
static void rt_avro_cleanup()
{
    if (rt_schema_buffer != NULL)
    {
        CcspTraceInfo(("VOICE REPORT %s : LINE %d free()\n", __FUNCTION__, __LINE__));
        free(rt_schema_buffer);
        rt_schema_buffer = NULL;
    }
    if (iface != NULL)
    {
        avro_value_iface_decref(iface);
        iface = NULL;
    }
    rt_schema_file_parsed = FALSE;
}

/*
 * Function to wait for ReportingPeriod
 */
static void WaitForPthreadConditionTimeout(ULONG waitingPeriod)
{
    struct timespec _ts;
    struct timespec _now;
    int n;

    memset(&_ts, 0, sizeof(struct timespec));
    memset(&_now, 0, sizeof(struct timespec));

    pthread_mutex_lock(&VoiceReportMutex);

    clock_gettime(CLOCK_REALTIME, &_now);
    if (waitingPeriod)
    {
        _ts.tv_sec = _now.tv_sec + waitingPeriod;
        CcspTraceInfo(("VOICE REPORT %s : Waiting for %d sec\n", __FUNCTION__, waitingPeriod));
    }
    else
    {
        if (_now.tv_nsec + WAIT_TIME_100_MS_IN_NSEC >= TIME_1_SEC_IN_NSEC)
        {
            _ts.tv_sec = _now.tv_sec + 1;
            _ts.tv_nsec = _now.tv_nsec + WAIT_TIME_100_MS_IN_NSEC - TIME_1_SEC_IN_NSEC;
        }
        else
        {
            _ts.tv_nsec = _now.tv_nsec + WAIT_TIME_100_MS_IN_NSEC;
        }
    }

    n = pthread_cond_timedwait(&VoiceReportCond, &VoiceReportMutex, &_ts);
    if (n == 0)
    {
        CcspTraceInfo(("VOICE REPORT %s : pthread_cond_timedwait SIGNALLED OK!!!\n", __FUNCTION__));
    }
    else if (n != ETIMEDOUT)
    {
        CcspTraceInfo(("VOICE REPORT %s : pthread_cond_timedwait ERROR(%d)!!!. %s \n", __FUNCTION__, n, strerror(n)));
    }

    pthread_mutex_unlock(&VoiceReportMutex);
}

int TelcoVoiceMgr_getCountVoiceServices()
{
    return TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES;
}

int TelcoVoiceMgr_getCountPhyInterfaces()
{
    return TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE;
}

int TelcoVoiceMgr_getCountProfiles()
{
    return TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE;
}

int TelcoVoiceMgr_getCountLines()
{
    return TELCOVOICEMGR_DML_NUMBER_OF_LINE;
}

static int PrepareAndSendVoiceServiceReport(void)
{
    int ret = 0, tmpActiveServicesList[TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES] = { 0 }, iCommonEnumRet;
    unsigned int uiTotalVoiceServices = 0, uiTotalActiveVoiceServices = 0;

    /* Note: voice HAL instance startes from 1 */

    //Count total services supported
    uiTotalVoiceServices = TelcoVoiceMgr_getCountVoiceServices();

    if( uiTotalVoiceServices > 0 )
    {
        int iServiceID;

        for( iServiceID = 0; iServiceID < uiTotalVoiceServices; iServiceID++ )
        {
                hal_param_t req_param;
                memset(&req_param, 0, sizeof(req_param));
                snprintf(req_param.name, sizeof(req_param.name), HAL_VS_STATUS, iServiceID + 1);
                if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
                {
                    if (strstr(req_param.value, "Started"))
                    {
                        tmpActiveServicesList[uiTotalActiveVoiceServices] = iServiceID + 1;
                        uiTotalActiveVoiceServices++;
                    }
                }
        }

        //Collect active voice services
        if( uiTotalActiveVoiceServices > 0 )
        {
           VoiceServiceReportData stFullReportData = { 0 };
           VoiceService           *pstVoiceService = NULL;

           stFullReportData.VoiceServiceNumberOfEntries = uiTotalActiveVoiceServices;
           stFullReportData.MaxServiceCount = uiTotalVoiceServices;

           pstVoiceService = ( VoiceService* ) malloc( uiTotalActiveVoiceServices * sizeof(VoiceService) );

           if( NULL != pstVoiceService )
           {
              int iServiceID, iProfileID, i;

              memset( pstVoiceService, 0, uiTotalActiveVoiceServices * sizeof(VoiceService) );

              for( i = 0; i < uiTotalActiveVoiceServices; i++ )
              {
                  iServiceID = tmpActiveServicesList[i];

                  //Count total phy interfaces supported
                  pstVoiceService[i].MaxPhyInterfaceCount = TelcoVoiceMgr_getCountPhyInterfaces();
                  pstVoiceService[i].PhyInterfaceNumberOfEntries = TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE;

                  //Count total voice profiles
                  pstVoiceService[i].MaxProfileCount = TelcoVoiceMgr_getCountProfiles();
                  pstVoiceService[i].VoiceProfileNumberOfEntries = TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE;

                  for( iProfileID = 0; iProfileID < pstVoiceService[i].VoiceProfileNumberOfEntries; iProfileID++ )
                  {
                      //Count total lines per profiles
                      pstVoiceService[i].astVoiceProfile[iProfileID].MaxLineCount = TelcoVoiceMgr_getCountLines();
                      pstVoiceService[i].astVoiceProfile[iProfileID].VoiceProfileNumberOfLines = TELCOVOICEMGR_DML_NUMBER_OF_LINE;
                  }

                  //Prepare Report Data
                  ret = VoiceServicePrepareReportData( iServiceID, &(pstVoiceService[i]) );
                  if (ret)
                  {
                      CcspTraceWarning(("VoiceServicePrepareReportData returned error [%d] \n", ret));

                      //Free Resource
                      if( NULL != pstVoiceService )
                      {
                        free(pstVoiceService);
                        pstVoiceService = NULL;
                      }

                      return ret;
                  }
              }

              //Send report via webpa
              stFullReportData.pstVoiceService = pstVoiceService;
              ret = harvester_report_voice_service(&stFullReportData);
              if (ret)
              {
                 CcspTraceWarning(("harvester_report_voice_service returned error [%d] \n", ret));
              }

              //Free Resource
              if( NULL != pstVoiceService )
              {
                free(pstVoiceService);
                pstVoiceService = NULL;
              }
            }
        }
    }

    return ret;
}

/*
 *  VOICE Report Thread Function
 */
void *StartVoiceReporting(void *args)
{
    CcspTraceInfo(("VOICE REPORT %s ENTER \n", __FUNCTION__));
    CcspTraceWarning(("VOICE REPORT %s Starting Thread to start VOICE data Harvesting  \n", __FUNCTION__));
    int ret = 0;
    ULONG uDftOverrideTTL = 0;
    ULONG uDftReportingPeriod = 0;
    ULONG uOverrideReportingPeriod = 0;
    ULONG waitingTimePeriod = 0;

    while (!ret && VoiceServiceReportGetStatus())
    {
        uDftOverrideTTL = VoiceServiceReportGetDefaultOverrideTTL();
        uDftReportingPeriod = VoiceServiceReportGetDefaultReportingPeriod();
        uOverrideReportingPeriod = VoiceServiceReportGetReportingPeriod();
        if (uDftOverrideTTL != 0)
        {
            if (uOverrideReportingPeriod == 0)
            {
                waitingTimePeriod = uDftOverrideTTL;
                VoiceServiceReportSetDefaultOverrideTTL(0);
            }
            else
            {
                if (CurrentOverrideReportingPeriod >= uDftOverrideTTL)
                {
                    CurrentOverrideReportingPeriod = 0;
                    VoiceServiceReportSetDefaultOverrideTTL(0);
                    // we don't send the report once TTL is completed.
                    // Do not wait here to send the report in Default ReportingPeriod.
                    waitingTimePeriod = 0;
                }
                else
                {
                    CurrentOverrideReportingPeriod = CurrentOverrideReportingPeriod + uOverrideReportingPeriod;
                    CcspTraceInfo(("CurrentOverrideReportingPeriod[%ld]\n", CurrentOverrideReportingPeriod));
                    ret = PrepareAndSendVoiceServiceReport();
                    waitingTimePeriod = uOverrideReportingPeriod;
                }
            }
        }
        else
        {
            if (uDftReportingPeriod != 0)
            {
                ret = PrepareAndSendVoiceServiceReport();
                waitingTimePeriod = uDftReportingPeriod;
            }
            else
            {
                //disabled
                waitingTimePeriod = 0;
            }
        }
        WaitForPthreadConditionTimeout(waitingTimePeriod);
    }

    VoiceServiceReportSetStatus(FALSE);
    rt_avro_cleanup();
    CcspTraceInfo(("VOICE REPORT %s EXIT \n", __FUNCTION__));
    CcspTraceInfo(("VOICE REPORT %s Stopped Thread for VOICE Data Harvesting  \n", __FUNCTION__));

    return NULL; // shouldn't return;
}

/*
 * Sets the VOICE Reporting Status
 */
int VoiceServiceReportSetStatus(BOOL status)
{
    CcspTraceInfo(("VOICE REPORT %s ENTER\n", __FUNCTION__));
    CcspTraceInfo(("VOICE REPORT %s  Old[%d] New[%d] \n", __FUNCTION__, VoiceServiceReportStatus, status));
    if (VoiceServiceReportStatus != status)
        VoiceServiceReportStatus = status;
    else
        return 0;

    if (VoiceServiceReportStatus)
    {
        pthread_t tid;
        CcspTraceInfo(("VOICE REPORT %s Starting Thread to start VOICE Data Harvesting  \n", __FUNCTION__));
        if (pthread_create(&tid, NULL, StartVoiceReporting, NULL))
        {
            CcspTraceWarning(("VOICE REPORT %s : Failed to Start Thread to start VOICE Harvesting  \n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        int ret;
        pthread_mutex_lock(&VoiceReportMutex);
        // Send signal to unblock thread that uses conditional variable VoiceReportCond
        ret = pthread_cond_signal(&VoiceReportCond);
        pthread_mutex_unlock(&VoiceReportMutex);
        if (ret == 0)
        {
            CcspTraceInfo(("VOICE REPORT %s : pthread_cond_signal success\n", __FUNCTION__));
        }
        else
        {
            CcspTraceInfo(("VOICE REPORT %s : pthread_cond_signal fail\n", __FUNCTION__));
        }
    }
    CcspTraceInfo(("VOICE REPORT %s : EXIT \n", __FUNCTION__));

    return 0;
}

/*
 * Returns the VOICE Temporary Report Reporting Period - used till TTL is expired
 */
ULONG VoiceServiceReportGetReportingPeriod()
{
    return VoiceServiceReportReportingPeriod;
}

/*
 * Sets the VOICE Temporary Report Reporting Period - used till TTL is expired
 */
int VoiceServiceReportSetReportingPeriod(ULONG interval)
{
    int ret;

    VoiceServiceReportReportingPeriod = interval;

    pthread_mutex_lock(&VoiceReportMutex);
    // Send signal to unblock thread that uses conditional variable VoiceReportCond
    ret = pthread_cond_signal(&VoiceReportCond);
    pthread_mutex_unlock(&VoiceReportMutex);
    if (ret == 0)
    {
        CcspTraceInfo(("VOICE REPORT %s : pthread_cond_signal success\n", __FUNCTION__));
    }
    else
    {
        CcspTraceInfo(("VOICE REPORT %s : pthread_cond_signal fail\n", __FUNCTION__));
    }

    return 0;
}

/*
 * Returns the VOICE Default Report Reporting Period - Used after TTL is expired
 */
ULONG VoiceServiceReportGetDefaultReportingPeriod()
{
    return VoiceServiceReportDfltReportingPeriod;
}

/*
 * Sets the VOICE Default Report Reporting Period - Used after TTL is expired
 */
int VoiceServiceReportSetDefaultReportingPeriod(ULONG interval)
{
    int ret;

    VoiceServiceReportDfltReportingPeriod = interval;

    pthread_mutex_lock(&VoiceReportMutex);
    // Send signal to unblock thread that uses conditional variable VoiceReportCond
    ret = pthread_cond_signal(&VoiceReportCond);
    pthread_mutex_unlock(&VoiceReportMutex);
    if (ret == 0)
    {
        CcspTraceInfo(("VOICE REPORT %s : pthread_cond_signal success\n", __FUNCTION__));
    }
    else
    {
        CcspTraceInfo(("VOICE REPORT %s : pthread_cond_signal fail\n", __FUNCTION__));
    }

    return 0;
}

/*
 * Returns the VOICE Report Overrride TTL
 */
ULONG VoiceServiceReportGetDefaultOverrideTTL()
{
    return VoiceServiceReportOverrideTTL;
}

/*
 * Sets the VOICE Report Overrride TTL
 */
int VoiceServiceReportSetDefaultOverrideTTL(ULONG interval)
{
    int ret;

    VoiceServiceReportOverrideTTL = interval;

    pthread_mutex_lock(&VoiceReportMutex);
    // Send signal to unblock thread that uses conditional variable VoiceReportCond
    ret = pthread_cond_signal(&VoiceReportCond);
    pthread_mutex_unlock(&VoiceReportMutex);
    if (ret == 0)
    {
        CcspTraceInfo(("VOICE REPORT %s : pthread_cond_signal success\n", __FUNCTION__));
    }
    else
    {
        CcspTraceInfo(("VOICE REPORT %s : pthread_cond_signal fail\n", __FUNCTION__));
    }

    return 0;
}

int _syscmd(char *cmd, char *retBuf, int retBufSize)
{
    CcspTraceInfo((" VOICE REPORT %s ENTER\n", __FUNCTION__));

    FILE *f;
    char *ptr = retBuf;
    int bufSize = retBufSize, bufbytes = 0, readbytes = 0;

    if ((f = popen(cmd, "r")) == NULL)
    {
        CcspTraceInfo((" VOICE REPORT %s : popen %s error\n", __FUNCTION__, cmd));
        return -1;
    }

    while (!feof(f))
    {
        *ptr = 0;
        if (bufSize >= 128)
        {
            bufbytes = 128;
        }
        else
        {
            bufbytes = bufSize - 1;
        }

        fgets(ptr, bufbytes, f);
        readbytes = strlen(ptr);
        if (readbytes == 0)
            break;
        bufSize -= readbytes;
        ptr += readbytes;
    }
    pclose(f);
    retBuf[retBufSize - 1] = 0;

    CcspTraceInfo((" VOICE REPORT %s EXIT\n", __FUNCTION__));

    return 0;
}

void macToLower(char macValue[])
{
    CcspTraceInfo((" VOICE REPORT %s ENTER\n", __FUNCTION__));

    int i = 0;
    int j;
    char *token[32];
    char tmp[32];
    strncpy(tmp, macValue, sizeof(tmp));
    token[i] = strtok(tmp, ":");
    if (token[i] != NULL)
    {
        strncpy(deviceMAC, token[i], sizeof(deviceMAC) - 1);
        deviceMAC[31] = '\0';
        i++;
    }
    while ((token[i] = strtok(NULL, ":")) != NULL)
    {
        strncat(deviceMAC, token[i], sizeof(deviceMAC) - 1);
        deviceMAC[31] = '\0';
        i++;
    }
    deviceMAC[31] = '\0';
    for (j = 0; deviceMAC[j]; j++)
    {
        deviceMAC[j] = tolower(deviceMAC[j]);
    }

    CcspTraceInfo((" Inside macToLower:: Device MAC: %s check\n", deviceMAC));
    CcspTraceInfo((" VOICE REPORT %s EXIT\n", __FUNCTION__));
}

char *getDeviceMac()
{
    char server_ip[16] = {0};
    int rc,fd = 0,server_port;
    unsigned int token;

    CcspTraceInfo((" VOICE REPORT %s ENTER\n", __FUNCTION__));

    //Open sysevent
    snprintf(server_ip, sizeof(server_ip), "127.0.0.1");
    server_port = SE_SERVER_WELL_KNOWN_PORT;

    fd = sysevent_open(server_ip, server_port, SE_VERSION, "voice_report", &token);
    if (!fd) {
        return NULL;
    }

    while (!strlen(deviceMAC))
    {
        pthread_mutex_lock(&device_mac_mutex);

        char deviceMACValue[32] = {'\0'};

        if (strlen(deviceMAC))
        {
            pthread_mutex_unlock(&device_mac_mutex);
            break;
        }

        if (CCSP_SUCCESS == sysevent_get(fd, token, "eth_wan_mac", deviceMACValue, sizeof(deviceMACValue)) == 0 && deviceMACValue[0] != '\0')
        {
            macToLower(deviceMACValue);
            CcspTraceInfo(("deviceMAC is %s\n", deviceMAC));
        }

        pthread_mutex_unlock(&device_mac_mutex);
    }

    //Close sysevent
    sysevent_close(fd, token);

    CcspTraceInfo((" VOICE REPORT %s EXIT\n", __FUNCTION__));

    return deviceMAC;
}
