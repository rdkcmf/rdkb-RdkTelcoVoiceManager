#ifndef _VOICE_REPORT_H
#define _VOICE_REPORT_H

#include <stdio.h>
#include <sys/time.h>
#include <libparodus.h>
#include <avro.h>
#include "telcovoicemgr_dml_apis.h"

#define VOICE_AVRO_SCHEMA_FILE "/usr/ccsp/harvester/VoiceDiagnostics.avsc"
#define DEFAULT_REPORTING_INTERVAL 0
#define DEFAULT_OVERRIDE_TTL 0
#define CHK_AVRO_ERR (strlen(avro_strerror()) > 0)
#define WRITER_BUF_SIZE (1024 * 30) // 30K
#define MAGIC_NUMBER_SIZE 1
#define MAGIC_NUMBER 0x8a
#define SCHEMA_ID_LENGTH 32
#define MAX_PARAMETERNAME_LEN 512
#define CCSP_AGENT_WEBPA_SUBSYSTEM "eRT."
#define DEVICE_PROPS_FILE "/etc/device.properties"
#define VOICE_REPORT_NAME "VoiceDiagnostics"

#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
#define HAL_VS_BOUNDIFNAME      "Device.Services.VoiceService.%d.X_RDK_BoundIfName"
#define HAL_VS_IPADDRESSFAMILY  "Device.Services.VoiceService.%d.X_RDK_IpAddressFamily"
#define HAL_VS_BOUNDIPADDR      "Device.Services.VoiceService.%d.X_RDK_BoundIpAddr"
#define HAL_VS_CCTKTRACEGROUP   "Device.Services.VoiceService.%d.X_RDK_Debug.CCTKTraceGroup"
#define HAL_VS_CCTKTRACELEVEL   "Device.Services.VoiceService.%d.X_RDK_Debug.CCTKTraceLevel"
#define HAL_VS_ENABLE           "Device.Services.VoiceService.%d.X_RDK_Enable"
#define HAL_VS_STATUS           "Device.Services.VoiceService.%d.X_RDK_Status"

#define HAL_SIP_OBPROXY         "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP.OutboundProxy"
#define HAL_SIP_OBPROXYPORT     "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP.OutboundProxyPort"
#define HAL_SIP_PROXYSERV       "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP.ProxyServer"
#define HAL_SIP_PROXYSERVPORT   "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP.ProxyServerPort"
#define HAL_SIP_REGSERV         "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP.RegistrarServer"
#define HAL_SIP_REGSERVPORT     "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP.RegistrarServerPort"
#define HAL_SIP_USERAGENTDOMAIN "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP.UserAgentDomain"
#define HAL_SIP_CONFURI         "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP.X_RDK-Central_COM_ConferencingURI"

#define HAL_VP_DIGITMAP         "Device.Services.VoiceService.%d.VoiceProfile.%d.X_RDK-Central_COM_DigitMap"
#define HAL_VP_EDIGITMAP        "Device.Services.VoiceService.%d.VoiceProfile.%d.X_RDK-Central_COM_EmergencyDigitMap"
#define HAL_VP_STIMER           "Device.Services.VoiceService.%d.VoiceProfile.%d.X_RDK-Central_COM_SDigitTimer"
#define HAL_VP_ZTIMER           "Device.Services.VoiceService.%d.VoiceProfile.%d.X_RDK-Central_COM_ZDigitTimer"

#define HAL_CF_CW               "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallingFeatures.CallWaitingEnable"
#define HAL_CF_MWI              "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallingFeatures.MWIEnable"
#define HAL_CF_CONF             "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallingFeatures.X_RDK-Central_COM_ConferenceCallingEnable"
#define HAL_CF_HOLD             "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallingFeatures.X_RDK-Central_COM_HoldEnable"
#define HAL_CF_CID              "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallingFeatures.X_RDK-Central_COM_PhoneCallerIDEnable"

#define HAL_LINE_DIRECTORYNUM   "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.DirectoryNumber"
#define HAL_LINE_ENABLE         "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.Enable"
#define HAL_LINE_OBPROXYADDR    "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.X_RDK_OutboundProxyAddresses"

#define HAL_LINE_SIP_AUTHUSER   "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.SIP.AuthUserName"
#define HAL_LINE_SIP_URI        "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.SIP.URI"

#define HAL_VPROCESS_RXGAIN     "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.VoiceProcessing.ReceiveGain"
#define HAL_VPROCESS_TXGAIN     "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.VoiceProcessing.TransmitGain"

#else

#define HAL_VS_BOUNDIFNAME      "Device.Services.VoiceService.%d.X_RDK_BoundIfName"
#define HAL_VS_IPADDRESSFAMILY  "Device.Services.VoiceService.%d.X_RDK_IpAddressFamily"
#define HAL_VS_BOUNDIPADDR      "Device.Services.VoiceService.%d.X_RDK_BoundIpAddr"
#define HAL_VS_CCTKTRACEGROUP   "Device.Services.VoiceService.%d.X_RDK_Debug.CCTKTraceGroup"
#define HAL_VS_CCTKTRACELEVEL   "Device.Services.VoiceService.%d.X_RDK_Debug.CCTKTraceLevel"
#define HAL_VS_ENABLE           "Device.Services.VoiceService.%d.X_RDK_Enable"
#define HAL_VS_STATUS           "Device.Services.VoiceService.%d.X_RDK_Status"

#define HAL_SIP_OBPROXY         "Device.Services.VoiceService.%d.SIP.Network.%d.OutboundProxy"
#define HAL_SIP_OBPROXYPORT     "Device.Services.VoiceService.%d.SIP.Network.%d.OutboundProxy"
#define HAL_SIP_PROXYSERV       "Device.Services.VoiceService.%d.SIP.Network.%d.ProxyServer"
#define HAL_SIP_PROXYSERVPORT   "Device.Services.VoiceService.%d.SIP.Network.%d.ProxyServerPort"
#define HAL_SIP_REGSERV         "Device.Services.VoiceService.%d.SIP.Network.%d.RegistrarServer"
#define HAL_SIP_REGSERVPORT     "Device.Services.VoiceService.%d.SIP.Network.%d.RegistrarServerPort"
#define HAL_SIP_USERAGENTDOMAIN "Device.Services.VoiceService.%d.SIP.Network.%d.UserAgentDomain"
#define HAL_SIP_CONFURI         "Device.Services.VoiceService.%d.SIP.Network.%d.X_RDK-Central_COM_ConferencingURI"

#define HAL_VP_DIGITMAP         "Device.Services.VoiceService.%d.VoIPProfile.%d.X_RDK-Central_COM_DigitMap"
#define HAL_VP_EDIGITMAP        "Device.Services.VoiceService.%d.VoIPProfile.%d.X_RDK-Central_COM_EmergencyDigitMap"
#define HAL_VP_STIMER           "Device.Services.VoiceService.%d.VoIPProfile.%d.X_RDK-Central_COM_SDigitTimer"
#define HAL_VP_ZTIMER           "Device.Services.VoiceService.%d.VoIPProfile.%d.X_RDK-Central_COM_ZDigitTimer"

#define HAL_CF_CW               "Device.Services.VoiceService.%d.CallControl.CallingFeatures.Set.%d.CallWaitingEnable"
#define HAL_CF_MWI              "Device.Services.VoiceService.%d.CallControl.CallingFeatures.Set.%d.MWIEnable"
#define HAL_CF_CONF             "Device.Services.VoiceService.%d.CallControl.CallingFeatures.Set.%d.X_RDK-Central_COM_ConferenceCallingEnable"
#define HAL_CF_HOLD             "Device.Services.VoiceService.%d.CallControl.CallingFeatures.Set.%d.X_RDK-Central_COM_HoldEnable"
#define HAL_CF_CID              "Device.Services.VoiceService.%d.CallControl.CallingFeatures.Set.%d.X_RDK-Central_COM_PhoneCallerIDEnable"

#define HAL_LINE_DIRECTORYNUM   "Device.Services.VoiceService.%d.CallControl.Line.%d.DirectoryNumber"
#define HAL_LINE_ENABLE         "Device.Services.VoiceService.%d.CallControl.Line.%d.Enable"
#define HAL_LINE_OBPROXYADDR    "Device.Services.VoiceService.%d.SIP.Network.%d.OutboundProxyResolvedAddress"

#define HAL_LINE_SIP_AUTHUSER   "Device.Services.VoiceService.%d.SIP.Client.%d.AuthUserName"
#define HAL_LINE_SIP_URI        "Device.Services.VoiceService.%d.SIP.Client.%d.RegisterURI"

#define HAL_VPROCESS_RXGAIN     "Device.Services.VoiceService.%d.Terminal.%d.Audio.%d.VoiceProcessing.ReceiveGain"
#define HAL_VPROCESS_TXGAIN     "Device.Services.VoiceService.%d.Terminal.%d.Audio.%d.VoiceProcessing.TransmitGain"
#endif /* FEATURE_RDKB_VOICE_DM_TR104_V2 */
/**
 * @struct used to strore the line data.
 */
typedef struct _VoiceLine
{
    CHAR   Status[16];
    CHAR   CallState[32];
    CHAR   DirectoryNumber[32];
    CHAR   VoiceServiceEnable[16];
    CHAR   SIPAuthUserName[64];
    CHAR   SIPURI[32];
    BOOL   CallWaitingEnable;
    BOOL   MWIEnable;
    BOOL   ConferenceCallingEnable;
    BOOL   HoldEnable;
    BOOL   PhoneCallerIDEnable;
    CHAR   CallerIDName[32];
    INT    AverageFarEndInterarrivalJitter;
    INT    AverageReceiveInterarrivalJitter;
    INT    AverageRoundTripDelay;
    UINT   BytesReceived;
    UINT   BytesSent;
    UINT   FarEndInterarrivalJitter;
    INT    FarEndPacketLossRate;
    INT    IncomingCallsAnswered;
    INT    IncomingCallsConnected;
    INT    IncomingCallsFailed;
    INT    IncomingCallsReceived;
    INT    OutgoingCallsAnswered;
    INT    OutgoingCallsAttempted;
    INT    OutgoingCallsConnected;
    INT    OutgoingCallsFailed;
    UINT   PacketsLost;
    UINT   PacketsReceived;
    UINT   PacketsSent;
    INT    ReceiveInterarrivalJitter;
    INT    ReceivePacketLossRate;
    BOOL   ResetStatistics;
    INT    RoundTripDelay;
    INT    ReceiveGain;
    INT    TransmitGain;
    CHAR   OutboundProxyAddresses[128];
    UINT   OverRuns;
    UINT   UnderRuns;
    UINT   CallsDropped;
    UINT   TotalCallTime;
    UINT   ServerDownTime;

}VoiceLine;

/**
 * @struct used to strore the Voice Profile data.
 */
typedef struct _VoiceProfile
{
    CHAR   SIPOutboundProxy[256];
    INT    SIPOutboundProxyPort;
    CHAR   SIPProxyServer[256];
    INT    SIPProxyServerPort;
    CHAR   SIPRegistrarServer[256];
    INT    SIPRegistrarServerPort;
    CHAR   SIPUserAgentDomain[256];
    CHAR   SIPConferencingURI[256];
    CHAR   DigitMap[2048];
    CHAR   EmergencyDigitMap[2048];
    INT    SDigitTimer;
    INT    ZDigitTimer;
    INT    VoiceProfileNumberOfLines;
    INT    MaxLineCount;
    VoiceLine   astLine[TELCOVOICEMGR_DML_NUMBER_OF_LINE];
}VoiceProfile;

/**
 * @struct used to strore the Voice Service data.
 */
typedef struct _VoiceService
{
    CHAR   VoiceServiceVersion[16];
    CHAR   BoundIfName[32];
    CHAR   IpAddressFamily[32];
    CHAR   BoundIpAddr[128];
    CHAR   CCTKTraceGroup[256];
    CHAR   CCTKTraceLevel[256];
    CHAR   VoiceProcessState[16];
    CHAR   VoiceProcessStatus[16];
    INT    MaxProfileCount;
    INT    MaxPhyInterfaceCount;
    INT    PhyInterfaceNumberOfEntries;
    INT    VoiceProfileNumberOfEntries;
    VoiceProfile astVoiceProfile[TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE];
}VoiceService;

/**
 * @struct used to strore the voice service report data.
 */
typedef struct _VoiceServiceReportData
{
    struct timeval timestamp;
    INT            VoiceServiceNumberOfEntries;
    INT            MaxServiceCount;
    VoiceService   *pstVoiceService;

} VoiceServiceReportData;

/**
 * @brief API to return size of the buffer contains schema contents
 * @return length of buffer
 */
int VoiceServiceReportGetSchemaBufferSize();

/**
 * @brief API to return string contains avro schema contents
 * @return string contains schema contents
 */
char *VoiceServiceReportGetSchemaBuffer();

/**
 * @brief API to return size of schemaID
 * @return length of schemaID buffer
 */
int VoiceServiceReportGetSchemaIDBufferSize();

/**
 * @brief API to return string contains schema id
 * @return string contains schema id
 */
char *VoiceServiceReportGetSchemaIDBuffer();

/**
 * @brief API to check value of Reporting Period.
 * @param value - indicates reporting period
 * @return: TRUE if ReportingPeriod is valid FALSE if ReportingPeriod is not valid
 */
BOOL VoiceServiceReportValidateReportingPeriod(UINT value);

/**
 * @brief API to return the VOICE Report Status
 * @return TRUE if VOICE Reporting is Enabled FALSE if XDSL Reporting is Disabled
 */
BOOL VoiceServiceReportGetStatus();

/**
 * @brief API to set the VOICE Reporting Status
 * @param status - Boolean flag indicate to set/reset
 * @return 0 if executed successfully
 */
int VoiceServiceReportSetStatus(BOOL status);

/**
 * @brief API to return the VOICE Report Reporting Period
 * @return Return reporting period
 */
ULONG VoiceServiceReportGetReportingPeriod();

/**
 * @brief API to set reporting time period
 * @param interval - reporting time peirod value
 * @return 0 if executed successfully
 */
int VoiceServiceReportSetReportingPeriod(ULONG interval);

/**
 * @brief Returns MacAdress of Device
 */
char *getDeviceMac();

/**
 * @brief API to get TTL value
 * @return TTL value
 */
ULONG VoiceServiceReportGetDefaultOverrideTTL();

/**
 * @brief API to get Default Reporting Period value
 * @return the default Reporting Period
 */
ULONG VoiceServiceReportGetDefaultReportingPeriod();

/**
 * @brief API to set Default Reporting Period value
 * @return 0 if executed successfully
 */
int VoiceServiceReportSetDefaultReportingPeriod(ULONG interval);

/**
 * @brief API to set Default Override TTL value
 * @return 0 if executed successfully
 */
int VoiceServiceReportSetDefaultOverrideTTL(ULONG interval);

/**
 * @brief API to send message via webpa
 * @return 0 if executed successfully
 */
void sendWebpaMsg(char *serviceName, char *dest, char *trans_id, char *contentType, char *payload, unsigned int payload_len);
#endif
