
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
#include "ansc_status.h"
#include "telcovoicemgr_dml_hal.h"
#include <sysevent.h>
#include "ccsp_trace.h"
#include "ccsp_syslog.h"
#include "ccsp_message_bus.h"
#include "ccsp_base_api.h"
#include "telcovoicemgr_dml_json_cfg_init.h"
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_services_apis_v2.h"
#else
#include "telcovoicemgr_services_apis_v1.h"
#endif //FEATURE_RDKB_VOICE_DM_TR104_V2

#define  VOICE_STATUS_STOPPED         "Stopped"
#define  VOICE_STATUS_STARTING        "Starting"
#define  VOICE_STATUS_STARTED         "Started"
#define  VOICE_STATUS_STOPPING        "Stopping"

#define  LINE_STATUS_UP              "Up"
#ifndef  FEATURE_RDKB_VOICE_DM_TR104_V2
#define  LINE_STATUS_INITIALIZING    "Initializing"
#define  LINE_STATUS_REGISTERING     "Registering"
#define  LINE_STATUS_UNREGISTERING   "Unregistering"
#endif   /*FEATURE_RDKB_VOICE_DM_TR104_V2*/
#define  LINE_STATUS_ERROR           "Error"
#define  LINE_STATUS_TESTING         "Testing"
#define  LINE_STATUS_QUIESCENT       "Quiescent"
#define  LINE_STATUS_DISABLED        "Disabled"

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#define  CALL_STATUS_IDLE            "Idle"
#define  CALL_STATUS_DIALING         "Dialing"
#define  CALL_STATUS_DELIVERED       "Delivered"
#define  CALL_STATUS_CONNECTED       "Connected"
#define  CALL_STATUS_ALERTING        "Alerting"
#define  CALL_STATUS_DISCONNECTED    "Disconnected"
#else
#define  CALL_STATUS_IDLE            "Idle"
#define  CALL_STATUS_CALLING         "Calling"
#define  CALL_STATUS_RINGING         "Ringing"
#define  CALL_STATUS_CONNECTING      "Connecting"
#define  CALL_STATUS_INCALL          "InCall"
#define  CALL_STATUS_HOLD            "Hold"
#define  CALL_STATUS_DISCONNECTING   "Disconnecting"
#endif /*FEATURE_RDKB_VOICE_DM_TR104_V2*/

#define SYSEVENT_VOICE_IPV4_PROXYLIST "voice_ipv4_outbound_proxy_addresses"
#define SYSEVENT_VOICE_IPV4_RTPLIST "voice_ipv4_rtp_pinholes"
#define SYSEVENT_VOICE_IPV4_ETHERNETPRIORITY "voice_ipv4_ethernetpriority"
#define SYSEVENT_VOICE_IPV4_DSCP "voice_ipv4_dscp"

#define SYSEVENT_VOICE_IPV6_PROXYLIST "voice_ipv6_outbound_proxy_addresses"
#define SYSEVENT_VOICE_IPV6_RTPLIST "voice_ipv6_rtp_pinholes"
#define SYSEVENT_VOICE_IPV6_ETHERNETPRIORITY "voice_ipv6_ethernetpriority"
#define SYSEVENT_VOICE_IPV6_DSCP "voice_ipv6_dscp"
#define SYSEVENT_FIELD_IPV6_DNS_PRIMARY   "ipv6_dns_0"
#define SYSEVENT_CURRENT_WAN_IFNAME "current_wan_ifname"
#define SYSEVENT_IPV4_DNS_PRIMARY "ipv4_%s_dns_0"
#define SYSEVENT_IPV4_DNS_SECONDARY "ipv4_%s_dns_1"
#define SYSEVENT_FIELD_IPV6_DNS_SECONDARY "ipv6_dns_1"
#define UTOPIA_FIREWALL_RESTART_TIMEOUT_MS  3000 /* ms */

#define ADD_RULE     "Add_Rule"
#define DELETE_RULE  "Delete_Rule"
#define PHY_WAN_IF_NAME  "erouter0"

extern int sysevent_voice_fd;
extern token_t sysevent_voice_token;
extern ANSC_HANDLE bus_handle;
static char *bTrueStr = "true", *bFalseStr = "false";
static TELCOVOICEMGR_VOICE_IP_LINK_STATE gLinkState = VOICE_HAL_IP_LINK_STATE_DOWN;

static ANSC_STATUS TelcoVoiceMgrDmlGetDnsServers(char *dns_server_address);

/* generate_voice_firewall_sysevent_string : */
/**
* @description : Generic Function to parse the Firewall Rule Data obtained from the subscribe event callback
*                and generate sysevent string .
* @param char* firewallData
* Format of X_RDK_FirewallRuleData - "enable(1 or 0), protocol1, destPort1, srcIpAddr1;enable(1 or 0),protocol2,destPort2,srcIpAddr2"
* Parse the data and convert it to sysevent value format
*
* @param ULONG sipSkbMark  //SIP SKB Mark
* @param ULONG rtpSkbMark //RTP SKB Mark
* @param ULONG sipDscpMark //SIP DSCP Mark
* @param ULONG rtpDscpMark //RTP DSCP Mark
* @param char* ethernetPriorityBuffer //Buffer to be filled with Ethernet priority sysevent value
* @param char* dscpBuffer //Buffer to be filled with DSCP sysevent value
* @param char* sipOutBoundProxyBuffer //Buffer to be filled with  sip outbound proxy sysevent value
* @param char* rtpPinholeBuffer //Buffer to be filled with  rtp pinhole sysevent value
* @param int   size //Size of Buffer
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
*/

static ANSC_STATUS generate_voice_firewall_sysevent_string(char* firewallData,
                   ULONG sipSkbMark,
                   ULONG rtpSkbMark,
                   ULONG sipDscpMark,
                   ULONG rtpDscpMark,
                   char* ethernetPriorityBuffer,
                   char* dscpBuffer,
                   char* sipOutBoundProxyBuffer,
                   char* rtpPinholeBuffer,                
                   uint16_t size)
{
    char buffer[BUF_LEN_1024] = {0};
    char *pToken = NULL;
    char *pTokenSub = NULL;
    char *endStr;
    bool enable;
    char ipAddr[IP_ADDR_LENGTH];
    char port[BUF_LEN_16];
    char protocol[BUF_LEN_16];
    char tmpSkbMarkBuffer[BUF_LEN_512] = {0};
    char tmpDscpMarkBuffer[BUF_LEN_512] = {0};
    char tmpOutBoundProxyBuffer[BUF_LEN_512] = {0};
    char tmpRtpPinholeBuffer[BUF_LEN_512] = {0};

    uint16_t skbLen = 0;
    uint16_t dscpLen = 0;
    uint16_t pinholeLen = 0;
    uint16_t outbtLen = 0;

    if(!firewallData || !ethernetPriorityBuffer || !dscpBuffer || !sipOutBoundProxyBuffer ||!rtpPinholeBuffer)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    snprintf(buffer, sizeof(buffer), "%s", firewallData);
    pToken = strtok_r(buffer, ";", &endStr);

    while (pToken != NULL)
    {
        char *end_token;
        pTokenSub = strtok_r(pToken, ",", &end_token);
        if(pTokenSub != NULL)
        {
            // Enable or Disable rule
            enable = atoi(pToken);
            pTokenSub = strtok_r(NULL, ",", &end_token);
            if(pTokenSub != NULL)
            {
                // Parse protocol from firewall rule data string(sip/rtp)
                memset(protocol, 0, sizeof(protocol));
                snprintf(protocol, sizeof(protocol), "%s", pTokenSub);
                pTokenSub = strtok_r(NULL, ",", &end_token);
                if(pTokenSub != NULL)
                {
                    // Parse port from firewall rule data string
                    memset(port, 0, sizeof(port));
                    snprintf(port, sizeof(port), "%s", pTokenSub);
                    pTokenSub = strtok_r(NULL, ",", &end_token);
                    if(pTokenSub != NULL)
                    {
                        // Parse IpAddress from firewall rule data string
                        memset(ipAddr, 0, sizeof(ipAddr));
                        snprintf(ipAddr, sizeof(ipAddr), "%s", pTokenSub);
                        if((!strcmp(protocol, "sip")) && (enable))
                        {
                            if(tmpSkbMarkBuffer[0] == '\0')
                            {
                                skbLen = snprintf((char*)&tmpSkbMarkBuffer[0], (sizeof(tmpSkbMarkBuffer)), "%s,%s,%lu;", ipAddr, port, sipSkbMark);
                                dscpLen = snprintf((char*)&tmpDscpMarkBuffer[0], (sizeof(tmpDscpMarkBuffer)), "%s,%s,%lu;", ipAddr, port, sipDscpMark);
                                outbtLen = snprintf((char*)&tmpOutBoundProxyBuffer[0], (sizeof(tmpOutBoundProxyBuffer)), "%s", ipAddr);
                            }
                            else
                            {
                                if(skbLen < sizeof(tmpSkbMarkBuffer))
                                {
                                   skbLen += snprintf((char*)&tmpSkbMarkBuffer[skbLen], (sizeof(tmpSkbMarkBuffer) - skbLen), "%s,%s,%lu;", ipAddr, port, sipSkbMark);
                                }
                                if(dscpLen < sizeof(tmpDscpMarkBuffer))
                                {
                                   dscpLen += snprintf((char*)&tmpDscpMarkBuffer[dscpLen], (sizeof(tmpDscpMarkBuffer) - dscpLen), "%s,%s,%lu;", ipAddr, port, sipDscpMark);
                                }
                                if(outbtLen < sizeof(tmpOutBoundProxyBuffer))
                                {
                                   outbtLen += snprintf((char*)&tmpOutBoundProxyBuffer[outbtLen], (sizeof(tmpOutBoundProxyBuffer) - outbtLen), ",%s", ipAddr);
                                }
                            }
                        }
                        else if((!strcmp(protocol, "rtp")) && (enable))
                        {
                            if(tmpSkbMarkBuffer[0] == '\0')
                            {
                                skbLen = snprintf((char*)&tmpSkbMarkBuffer[0], (sizeof(tmpSkbMarkBuffer)), "%s,%s,%lu;", ipAddr, port, rtpSkbMark);
                                dscpLen = snprintf((char*)&tmpDscpMarkBuffer[0], (sizeof(tmpDscpMarkBuffer)), "%s,%s,%lu;", ipAddr, port, rtpDscpMark);
                                pinholeLen = snprintf((char*)&tmpRtpPinholeBuffer[0], (sizeof(tmpRtpPinholeBuffer)), "%s,%s;", ipAddr, port);
                            }
                            else
                            {
                                if(skbLen < sizeof(tmpSkbMarkBuffer))
                                {
                                   skbLen += snprintf((char*)&tmpSkbMarkBuffer[skbLen], (sizeof(tmpSkbMarkBuffer) - skbLen), "%s,%s,%lu;", ipAddr, port, rtpSkbMark);
                                }
                                if(dscpLen < sizeof(tmpDscpMarkBuffer))
                                {
                                   dscpLen += snprintf((char*)&tmpDscpMarkBuffer[dscpLen], (sizeof(tmpDscpMarkBuffer) - dscpLen), "%s,%s,%lu;", ipAddr, port, rtpDscpMark);
                                }
                                if(pinholeLen < sizeof(tmpRtpPinholeBuffer))
                                {
                                   pinholeLen += snprintf((char*)&tmpRtpPinholeBuffer[pinholeLen], (sizeof(tmpRtpPinholeBuffer) - pinholeLen), "%s,%s;", ipAddr, port);
                                }
                            }
                        }
                    }               
                }
            }
        }
        pToken = strtok_r(NULL, ";", &endStr);
    }
    memcpy(ethernetPriorityBuffer, tmpSkbMarkBuffer, skbLen);
    memcpy(dscpBuffer, tmpDscpMarkBuffer, dscpLen);
    memcpy(sipOutBoundProxyBuffer, tmpOutBoundProxyBuffer, outbtLen);
    memcpy(rtpPinholeBuffer, tmpRtpPinholeBuffer, pinholeLen);
    return ANSC_STATUS_SUCCESS;
}

/* set_iptable_rules_for_rtp : */
/**
* @description : Generic Function to add/delete ip table rules for RTP Protocol.
* Parse the input buffer and add/delete ip table rules for each port and ip.
* RTP rules are needed to establish the voice call and it can't be delayed by the firewall restart on utopia.
* Therefore iptable rules are added here and Firewall is not restarted for RTP events.
* @param char* inputBuffer  -  input buffer to be parsed (SIP X_RDK_FirewallRuleData)
* @param UINT rtpDscpValue
* @param UINT rtpSkbMark
* @param TELCOVOICEMGR_VOICE_IP_ADD_FAMILY ipAddrFamily - IPv4/IPv6
* @param char *action - ADD/DELETE firewall rule
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
*/
static ANSC_STATUS set_iptable_rules_for_rtp(char *inputBuffer, UINT rtpDscpValue, UINT rtpSkbMark, TELCOVOICEMGR_VOICE_IP_ADD_FAMILY ipAddrFamily, char *action)
{
    char buffer[BUF_LEN_512];
    char ipTableRule[BUF_LEN_2048];
    char addr[IP_ADDR_LENGTH];
    char port[BUF_LEN_16];
    char ipTableVersion[BUF_LEN_16];
    char *pToken = NULL;

    if(!inputBuffer || !action)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    if(ipAddrFamily == VOICE_HAL_AF_INET_V4)
        strncpy(ipTableVersion, "iptables", sizeof(ipTableVersion));
    else if(ipAddrFamily == VOICE_HAL_AF_INET_V6)
        strncpy(ipTableVersion, "ip6tables", sizeof(ipTableVersion));
    else
    {
        CcspTraceWarning(("%s Invalid Iptable Version\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    strncpy(buffer, inputBuffer, sizeof(buffer));
    pToken = strtok(buffer, ",");
    while( pToken != NULL )
    {
        memset(addr, 0, sizeof(addr));
        strncpy(addr, pToken, sizeof(addr) -1);
        pToken = strtok(NULL, ";");
        if(pToken != NULL)
        {
            memset(port, 0, sizeof(port));
            strncpy(port, pToken, sizeof(port)-1);

            if(!strcmp(action, ADD_RULE))
            {
                snprintf((char*)&ipTableRule[0],sizeof(ipTableRule),
                                "%s -t mangle -A VOICE_MANGLE -d %s -p udp -m udp --sport %s -j MARK --set-xmark %lu/%lu;\
                                %s -t mangle -A VOICE_MANGLE -d %s -p udp -m udp --sport %s -j DSCP --set-dscp %lu\n",
                                ipTableVersion, addr, port, rtpSkbMark, rtpSkbMark,
                                ipTableVersion, addr, port, rtpDscpValue);
                system(ipTableRule);
                snprintf((char*)&ipTableRule[0],sizeof(ipTableRule),"%s -I VOICE 1 -s %s -i %s -p udp -m udp --dport %s -j ACCEPT\n", ipTableVersion, addr, PHY_WAN_IF_NAME, port);
                system(ipTableRule);
            }
            else if(!strcmp(action, DELETE_RULE))
            {
                snprintf((char*)&ipTableRule[0],sizeof(ipTableRule),"%s -D VOICE -s %s -i %s -p udp -m udp --dport %s -j ACCEPT\n", ipTableVersion, addr, PHY_WAN_IF_NAME, port);
                system(ipTableRule);

                snprintf((char*)&ipTableRule[0],sizeof(ipTableRule),"%s -t mangle -D VOICE_MANGLE -d %s -p udp -m udp --sport %s -j DSCP --set-dscp %lu\n", ipTableVersion, addr, port, rtpDscpValue);
                system(ipTableRule);

                snprintf((char*)&ipTableRule[0],sizeof(ipTableRule),"%s -t mangle -D VOICE_MANGLE -d %s -p udp -m udp --sport %s -j MARK --set-xmark %lu/%lu\n", ipTableVersion, addr, port, rtpSkbMark, rtpSkbMark);
                system(ipTableRule);
            }

        }
        pToken = strtok(NULL, ",");
    }

    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrSetSyseventData: */
/**
* @description Helper function to set sysevent with corresponding value for ipAddressFamily and BoundIfname
*
* @param char* eventName
* @param char* eventValue
* @param char* pTableName - Table name
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrSetSyseventData(char *eventName, char *eventValue)
{
    if (sysevent_set(sysevent_voice_fd, sysevent_voice_token, eventName, eventValue, 0))
    {
        CcspTraceWarning(("%s :: sysevent_set failed for %s\n", __FUNCTION__, eventName));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
/* TelcoVoiceMgrConvertProtocolToEnum */
/**
* @description helper function to convert Transport Protocol to corresponding enum.
*
* @param char* protocol 
* @param uint32_t *protocolEnum
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrConvertProtocolToEnum(char *protocol, uint32_t *protocolEnum)
{
    if(!protocol || !protocolEnum)
    {
      CcspTraceWarning(("%s :: NULL Param Passed\n", __FUNCTION__));
      return ANSC_STATUS_FAILURE;
    }
    if(!strcmp(protocol, "TCP"))
    {
      *protocolEnum = TRANSPORT_TCP;
    }
    else if(!strcmp(protocol, "UDP"))
    {
      *protocolEnum = TRANSPORT_UDP;
    }
    else if(!strcmp(protocol, "TLS"))
    {
      *protocolEnum = TRANSPORT_TLS;
    }
    else if(!strcmp(protocol, "SCTP"))
    {
      *protocolEnum = TRANSPORT_SCTP;
    }
    else
    {
      CcspTraceWarning(("%s :: Invalid Protocol Passed\n", __FUNCTION__));
      return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
#endif
/* TelcoVoiceMgrDmlGetTableEntryNames: */
/**
* @description Helper function to get table entry names from data model.Retrieves
* a list of names of existing table records.
*
* @param char* pComponent - Component name
* @param char* pBus - Data bus path
* @param char* pTableName - Table name
* @param char  aRecordPathList[][256] - Output, list of entry names
* @param int*  pNumRecords - Output, Number of records(table entries)
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
static ANSC_STATUS TelcoVoiceMgrDmlGetTableEntryNames(
                                 char *pComponent, char *pBus,
                                 char *pTableName, int maxRecords, char aRecordNameList[][256],
                                                              int *pNumRecords)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterInfoStruct_t **retInfo = NULL;
    int ret = 0, nval = 0;
    int iLoopCount;

    if(!(pTableName && aRecordNameList && pNumRecords))
    {
        CcspTraceError(("%s: Invalid parameters..\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    *pNumRecords = 0;
    ret = CcspBaseIf_getParameterNames(bus_handle, pComponent, pBus,
                                       pTableName, 1, &nval, &retInfo);
    if (CCSP_SUCCESS == ret)
    {
        for (iLoopCount = 0; iLoopCount < maxRecords && iLoopCount < nval; iLoopCount++)
        {
            if (NULL != retInfo[iLoopCount]->parameterName)
            {
                CcspTraceInfo(("%s parameterName[%d,%s]\n", __FUNCTION__, iLoopCount,
                                                             retInfo[iLoopCount]->parameterName));
                snprintf(aRecordNameList[iLoopCount], sizeof(aRecordNameList[0]),
                                                       "%s", retInfo[iLoopCount]->parameterName);
            }
        }
        if (retInfo)
        {
            free_parameterInfoStruct_t(bus_handle, nval, retInfo);
        }
        *pNumRecords = iLoopCount;
        return ANSC_STATUS_SUCCESS;
    }
    if (retInfo)
    {
        free_parameterInfoStruct_t(bus_handle, nval, retInfo);
    }
    return ANSC_STATUS_FAILURE;
}

/* TelcoVoiceMgrDmlGetWanInterfaceRecordNames: */
/**
* @description Get the WAN interface table entry names.
*
* @param char**  aRecordNameList - Output, list of interface table entries
* @param int*    pNumRecords - Output, number of interface table entries.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
static ANSC_STATUS TelcoVoiceMgrDmlGetWanInterfaceRecordNames(char aRecordNameList[][256],
                                                                     int *pNumRecords)
{
    if(!(aRecordNameList && pNumRecords))
    {
        return ANSC_STATUS_FAILURE;
    }
    return TelcoVoiceMgrDmlGetTableEntryNames(WAN_COMPONENT_NAME, WAN_DBUS_PATH,
                              TABLE_NAME_WAN_INTERFACE, MAX_INTERFACES, aRecordNameList, pNumRecords);
}

/* TelcoVoiceMgrDmlGetWanMarkingRecordNames: */
/**
* @description Get the marking table entry names from Interface.
*
* @param char*   pWanIfEntryName - Interface name
* @param char**  aRecordNameList - Output, list of Marking table entries
* @param int*    pNumRecords - Output, number of marking table entries.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
static ANSC_STATUS TelcoVoiceMgrDmlGetWanMarkingRecordNames(char *pWanIfEntryName,
                                      char aRecordNameList[][256], int *pNumRecords)
{
    char aTableName[512] = {0};
    if(!(pWanIfEntryName && aRecordNameList && pNumRecords))
    {
        return ANSC_STATUS_FAILURE;
    }
    snprintf(aTableName, sizeof(aTableName), "%s%s", pWanIfEntryName, TABLE_NAME_WAN_MARKING);
    return TelcoVoiceMgrDmlGetTableEntryNames(WAN_COMPONENT_NAME, WAN_DBUS_PATH,
                                    aTableName, MAX_MARK_ENTRY, aRecordNameList, pNumRecords);
}

/* TelcoVoiceMgrDmlGetParamValue: */
/**
* @description Get value of a parameter from datamodel.
*
* @param char*      pComponent -  Component name
* @param char*      pBus - Bus name
* @param char*      pParamName - Parameter to read.
* @param char*      pParamVal - Output, Parameter value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
static ANSC_STATUS TelcoVoiceMgrDmlGetParamValue(char *pComponent, char *pBus, char *pParamName, char *pReturnVal)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterValStruct_t **retVal = NULL;
    char *ParamName[1];
    int ret = 0, nval = 0;

    ParamName[0] = pParamName;

    ret = CcspBaseIf_getParameterValues(
        bus_handle,
        pComponent,
        pBus,
        ParamName,
        1,
        &nval,
        &retVal);

    if (CCSP_SUCCESS == ret)
    {
        CcspTraceInfo(("%s parameterValue[%s]\n", __FUNCTION__, retVal[0]->parameterValue));
        if (NULL != retVal[0]->parameterValue)
        {
            memcpy(pReturnVal, retVal[0]->parameterValue, strlen(retVal[0]->parameterValue) + 1);
        }
        if (retVal)
        {
            free_parameterValStruct_t(bus_handle, nval, retVal);
        }
        return ANSC_STATUS_SUCCESS;
    }

    if (retVal)
    {
        free_parameterValStruct_t(bus_handle, nval, retVal);
    }
    CcspTraceInfo(("%s ANSC_STATUS_FAILURE\n", __FUNCTION__));
    return ANSC_STATUS_FAILURE;
}

/* TelcoVoiceMgrDmlSetParamValue: */
/**
* @description Set value of a parameter in datamodel.
*
* @param char*      pComponent -  Component name
* @param char*      pBus - Bus name
* @param char*      pParamName - Parameter to set.
* @param char*      pParamVal - New parameter value.
* @param dataType_e type -  Parameter data type.
* @param uint       bCommitFlag -  Commit flag
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
static ANSC_STATUS TelcoVoiceMgrDmlSetParamValue(char *pComponent, char *pBus,
                      char *pParamName, char *pParamVal, enum dataType_e type, unsigned int bCommitFlag)
{
    CCSP_MESSAGE_BUS_INFO *bus_info = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterValStruct_t param_val[1] = {0};
    char *faultParam = NULL;
    char acParameterName[256] = {0},
         acParameterValue[128] = {0};
    int ret = 0;

    snprintf(acParameterName, sizeof(acParameterName),"%s", pParamName);
    param_val[0].parameterName = acParameterName;

    snprintf(acParameterValue, sizeof(acParameterValue),"%s", pParamVal);
    param_val[0].parameterValue = acParameterValue;

    param_val[0].type = type;

    ret = CcspBaseIf_setParameterValues(
                                        bus_handle,
                                        pComponent,
                                        pBus,
                                        0,
                                        0,
                                        &param_val[0],
                                        1,
                                        bCommitFlag,
                                        &faultParam);

    CcspTraceInfo(("Value being set [%d] \n", ret));

    if ((ret != CCSP_SUCCESS) && (faultParam != NULL))
    {
        CcspTraceError(("%s-%d Failed to set %s\n", __FUNCTION__, __LINE__, pParamName));
        bus_info->freefunc(faultParam);
        return ANSC_STATUS_FAILURE;
    }

    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlAddMarkingEntry: */
/**
* @description Adds a new table entry to the Marking table.
*
* @param char*      pAliasValue -  Value for Alias field.
* @param char*      pMark - Value for EthernetPriority field.
* @param char*      pIfRecordName - Interface instance name in table.
* @param int*       pNewTableIndex - Output, index of the new Marking entry.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
static ANSC_STATUS TelcoVoiceMgrDmlAddMarkingEntry(char *pAliasValue, char *pMark, char *pIfRecordName, int *pNewTableIndex)
{
    char aTableName[512] = {0};
    int iNewTableIndex = -1;
    char acSetParamName[256] = {0};

    if(!(pNewTableIndex && pIfRecordName && pAliasValue))
    {
        return ANSC_STATUS_FAILURE;
    }
    *pNewTableIndex = iNewTableIndex;
    snprintf(aTableName, sizeof(aTableName), "%s%s", pIfRecordName, TABLE_NAME_WAN_MARKING);

    if (CCSP_SUCCESS != CcspBaseIf_AddTblRow(
                            bus_handle,
                            WAN_COMPONENT_NAME,
                            WAN_DBUS_PATH,
                            0, /* session id */
                            aTableName,
                            &iNewTableIndex))
    {
        CcspTraceError(("%s Failed to add Ethernet Priority Mark entry to table %s\n", __FUNCTION__, aTableName));
        return ANSC_STATUS_FAILURE;
    }

    CcspTraceInfo(("%s %d Added marking table entry at index:%d\n", __FUNCTION__, __LINE__, iNewTableIndex));

    //Set Alias
    snprintf(acSetParamName, sizeof(acSetParamName), "%s%d.%s", aTableName, iNewTableIndex, PARAM_NAME_MARK_ALIAS);
    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetParamValue(WAN_COMPONENT_NAME,
                                           WAN_DBUS_PATH, acSetParamName, pAliasValue, ccsp_string,FALSE))
    {
        //Set EthernetPriorityMark
        memset(acSetParamName, 0, sizeof(acSetParamName));
        snprintf(acSetParamName, sizeof(acSetParamName), "%s%d.%s", aTableName, iNewTableIndex, PARAM_NAME_ETHERNET_PRIORITY_MARK);
        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlSetParamValue(WAN_COMPONENT_NAME,
                              WAN_DBUS_PATH, acSetParamName, pMark, ccsp_int, TRUE))
        {
            *pNewTableIndex = iNewTableIndex;
            return ANSC_STATUS_SUCCESS;
        }
    }
    return ANSC_STATUS_FAILURE;
}

/* TelcoVoiceMgrDmlSetMarkingEthPriorityMark: */
/**
* @description Set ethernet priority mark for an interface in WAN data model.
*
* @param char*      pIfRecordName -  (*pAction)(char *aIfRecordName, void *) - Action function pointer.
* @param void*      pArgs - Action specific parameters.
*                   PROTOCOL_TYPE protocol , Protocol specifier(SIP/RTP);
*                   int32_t       iEthPriorityMark, Ethernet priority mark to be set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
* @retval ANSC_STATUS_DISCARD if the new value is same as old value.
*
* @execution Synchronous.
* @sideeffect None.
*
*/
static ANSC_STATUS TelcoVoiceMgrDmlSetMarkingEthPriorityMark(char *pIfRecordName, void *pArgs)
{
    int iLoopCount = 0;
    char paramName[512] = {0};
    char aParamValue[256] = {0};
    char aAliasValue[16] = {0};
    char aMrkRecordList[MAX_MARK_ENTRY][256] = {0};
    int numMarkingRecords = 0;
    int iNewInstanceNumber = -1;

    char aEthPriority[4] = {0};

    if(!pArgs || !pIfRecordName)
    {
        return ANSC_STATUS_FAILURE;
    }
    ethPriorityValStruct_t *pValStruct = (ethPriorityValStruct_t *)pArgs;

    switch(pValStruct->protocol)
    {
        case SIP:
            strncpy(aAliasValue, PARAM_VALUE_SIP_ALIAS, sizeof(aAliasValue));
        break;
        case RTP:
            strncpy(aAliasValue, PARAM_VALUE_RTP_ALIAS, sizeof(aAliasValue));
        break;
        default:
            return ANSC_STATUS_FAILURE;
    }
    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetWanMarkingRecordNames(
                                 pIfRecordName,aMrkRecordList, &numMarkingRecords))
    {
        //Got WAN Marking table entries for interface, Now iterate through marking entries.
        for(iLoopCount = 0; iLoopCount < numMarkingRecords; iLoopCount ++)
        {
            memset(paramName, 0, sizeof(paramName));
            memset(aParamValue, 0, sizeof(aParamValue));
            snprintf(paramName, sizeof(paramName), "%s%s", aMrkRecordList[iLoopCount],
                                                                        PARAM_NAME_MARK_ALIAS);
            // Now check for alias passed as argument(voicesip1/voicertp1).
            if (ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetParamValue(WAN_COMPONENT_NAME,
                                                                WAN_DBUS_PATH, paramName, aParamValue))
            {
                if(!strncmp(aAliasValue, aParamValue, sizeof(aAliasValue)))
                {   // Found an entry with the alias voicesip1/voicertp1
                    memset(paramName, 0, sizeof(paramName));
                    snprintf(paramName, sizeof(paramName), "%s%s", aMrkRecordList[iLoopCount],
                                                                        PARAM_NAME_ETHERNET_PRIORITY_MARK);
                    //read and check if the new value is different from its old value.
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetParamValue(WAN_COMPONENT_NAME, WAN_DBUS_PATH,
                                                                                  paramName, aParamValue))
                    {
                        memset(aEthPriority, 0, sizeof(aEthPriority));
                        snprintf(aEthPriority,sizeof(aEthPriority), "%d", pValStruct->iEthPriorityMark);
                        if(!strncmp(aEthPriority, aParamValue, sizeof(aEthPriority)))
                        {
                            // New value is same to old value, returning without modifying it.
                            pValStruct->iUpdateStatus = TRUE;
                            return ANSC_STATUS_DISCARD;
                        }
                        if (pValStruct->iEthPriorityMark == -1) // Remove the entry if new value is -1.
                        {
                            if (CCSP_SUCCESS == CcspBaseIf_DeleteTblRow(bus_handle, WAN_COMPONENT_NAME,
                                                            WAN_DBUS_PATH, 0, aMrkRecordList[iLoopCount]))
                            {
                                CcspTraceInfo(("%s Deleted marking table entry %s\n",
                                                            __FUNCTION__, aMrkRecordList[iLoopCount]));
                                pValStruct->iUpdateStatus = TRUE;
                            }
                            else
                            {
                                CcspTraceError(("%s Failed to delete marking table entry %s\n",
                                                                __FUNCTION__, aMrkRecordList[iLoopCount]));
                                return ANSC_STATUS_FAILURE;
                            }
                        }
                        else   // Update the entry with new value.
                        {
                            if(ANSC_STATUS_SUCCESS ==  TelcoVoiceMgrDmlSetParamValue(WAN_COMPONENT_NAME, WAN_DBUS_PATH,
                                                                            paramName, aEthPriority, ccsp_int, TRUE))
                            {
                                CcspTraceInfo(("%s Updated marking table entry %s\n",
                                                               __FUNCTION__, aMrkRecordList[iLoopCount]));
                                pValStruct->iUpdateStatus = TRUE;
                            }
                            else
                            {
                                CcspTraceError(("%s Failed to update marking table entry %s\n",
                                                                __FUNCTION__, aMrkRecordList[iLoopCount]));
                                return ANSC_STATUS_FAILURE;
                            }
                        }
                        CcspTraceError(("%s ####WanMgr refresh####\n",__FUNCTION__));
                        // Successfully updated/deteled marking entry, now do wan refresh.
                        memset(paramName, 0, sizeof(paramName));
                        snprintf(paramName, sizeof(paramName), "%s%s", pIfRecordName,
                                                                                PARAM_NAME_WAN_REFRESH);
                        return TelcoVoiceMgrDmlSetParamValue(WAN_COMPONENT_NAME, WAN_DBUS_PATH,
                                                                paramName, STR_TRUE, ccsp_boolean, TRUE);
                    }
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                return ANSC_STATUS_FAILURE;
            }
        }
        // Reaching here means we could not find an entry with desired alias
        if (pValStruct->iEthPriorityMark == -1)
        {   // No need to add an entry , return with success.
            pValStruct->iUpdateStatus = TRUE;
            return ANSC_STATUS_SUCCESS;
        }
        // Now to add new entry with desired alias and ethernet priority value.
        memset(aParamValue, 0, sizeof(aParamValue));
        snprintf(aParamValue, sizeof(aParamValue), "%d", pValStruct->iEthPriorityMark);
        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlAddMarkingEntry(aAliasValue,
                                        aParamValue, pIfRecordName, &iNewInstanceNumber))
        {
            CcspTraceInfo(("Added EthernetPriorityMark entry to table %s%s%d.\n",
                                                            pIfRecordName, TABLE_NAME_WAN_MARKING, iNewInstanceNumber));
            pValStruct->iUpdateStatus = TRUE;
            memset(paramName, 0, sizeof(paramName));
            snprintf(paramName, sizeof(paramName), "%s%s", pIfRecordName,
                                                                    PARAM_NAME_WAN_REFRESH);
            return TelcoVoiceMgrDmlSetParamValue(WAN_COMPONENT_NAME, WAN_DBUS_PATH,
                                            paramName, STR_TRUE, ccsp_boolean, TRUE);
        }
        else
        {
            CcspTraceError(("Failed to add EthernetPriorityMark entry to table %s%s\n",
                                                pIfRecordName, TABLE_NAME_WAN_MARKING));
        }
    }
    return ANSC_STATUS_FAILURE;
}

/* TelcoVoiceMgrDmlIterateEnabledInterfaces: */
/**
* @description Iterate helper function for WAN interface table.
*              Calls action function for each interface entry.
*
* @param ANSC_STATUS (*pAction)(char *aIfRecordName, void *) - Action function pointer.
* @param void *      pActionArgs - Action specific parameters.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlIterateEnabledInterfaces(ANSC_STATUS (*pAction)(char *aIfRecordName, void *), void *pActionArgs)
{
    char aIfRecordNameList[MAX_INTERFACES][256] = {0};
    char aIfUpdateList[MAX_INTERFACES][256] = {0};
    char aUpdatedEntryName[256] = {0};

    int numIfRecords = 0;
    int iLoopCount = 0;
    char paramName[512] = {0};
    char aParamValue[256] = {0};
    uint uUpdateCount = 0;

    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetWanInterfaceRecordNames
                                                 (aIfRecordNameList, &numIfRecords))
    {
        for(iLoopCount = 0; iLoopCount < numIfRecords; iLoopCount ++)
        {
            memset(paramName, 0, sizeof(paramName));
            memset(aParamValue, 0, sizeof(aParamValue));
            snprintf(paramName, sizeof(paramName), "%s%s", aIfRecordNameList[iLoopCount],
                                                                      PARAM_NAME_WAN_ENABLE);
            if (ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetParamValue(WAN_COMPONENT_NAME,
                                                              WAN_DBUS_PATH, paramName, aParamValue))
            {
                if(!strncmp(STR_TRUE, aParamValue, sizeof(STR_TRUE)))
                {
                    pAction(aIfRecordNameList[iLoopCount], pActionArgs);
                }
            }
        }
        return ANSC_STATUS_SUCCESS;
    }

    return ANSC_STATUS_FAILURE;
}

/* TelcoVoiceMgrDmlGetMarkingEthPriorityMark: */
/**
* @description Get ethernet priority mark for an interface in WAN data model.
*
* @param char*      pIfRecordName -  (*pAction)(char *aIfRecordName, void *) - Action function pointer.
* @param void*      pArgs - Action specific parameters.
*                   PROTOCOL_TYPE protocol , Protocol specifier(SIP/RTP);
*                   int32_t       iEthPriorityMark, buffer for output value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
* 
* @execution Synchronous.
* @sideeffect None.
*
*/

static ANSC_STATUS TelcoVoiceMgrDmlGetMarkingEthPriorityMark(char *pIfRecordName, void *pArgs)
{
    int iLoopCount = 0;
    char paramName[512] = {0};
    char aParamValue[256] = {0};
    char aAliasValue[16] = {0};
    char aMrkRecordList[MAX_MARK_ENTRY][256] = {0};
    int numMarkingRecords = 0;
    int iNewInstanceNumber = -1;

    char aEthPriority[4] = {0};

    if(!pArgs || !pIfRecordName)
    {
        return ANSC_STATUS_FAILURE;
    }
    ethPriorityValStruct_t *pValStruct = (ethPriorityValStruct_t *)pArgs;

    switch(pValStruct->protocol)
    {
        case SIP:
            strncpy(aAliasValue, PARAM_VALUE_SIP_ALIAS, sizeof(aAliasValue));
        break;
        case RTP:
            strncpy(aAliasValue, PARAM_VALUE_RTP_ALIAS, sizeof(aAliasValue));
        break;
        default:
            return ANSC_STATUS_FAILURE;
    }
    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetWanMarkingRecordNames(
                                 pIfRecordName,aMrkRecordList, &numMarkingRecords))
    {
        //Got WAN Marking table entries for interface, Now iterate through marking entries.
        for(iLoopCount = 0; iLoopCount < numMarkingRecords; iLoopCount ++)
        {
            memset(paramName, 0, sizeof(paramName));
            memset(aParamValue, 0, sizeof(aParamValue));
            snprintf(paramName, sizeof(paramName), "%s%s", aMrkRecordList[iLoopCount],
                                                                        PARAM_NAME_MARK_ALIAS);
            // Now check for alias passed as argument(voicesip1/voicertp1).
            if (ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetParamValue(WAN_COMPONENT_NAME,
                                                                WAN_DBUS_PATH, paramName, aParamValue))
            {
                if(!strncmp(aAliasValue, aParamValue, sizeof(aAliasValue)))
                {   // Found an entry with the alias voicesip1/voicertp1
                    memset(paramName, 0, sizeof(paramName));
                    snprintf(paramName, sizeof(paramName), "%s%s", aMrkRecordList[iLoopCount],
                                                                        PARAM_NAME_ETHERNET_PRIORITY_MARK);
                    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetParamValue(WAN_COMPONENT_NAME, WAN_DBUS_PATH,
                                                                                  paramName, aParamValue))
                    {
                        pValStruct->iEthPriorityMark = atoi(aParamValue);
                        pValStruct->iUpdateStatus = TRUE;
                        return ANSC_STATUS_SUCCESS;
                    }
                    CcspTraceError(("%s:%d:: Could not read ethernet priority for %s....\n",
                                                                                __FUNCTION__, __LINE__, aAliasValue));
                    return ANSC_STATUS_FAILURE;
                }
            }
            else
            {
                CcspTraceError(("%s:%d:: Could not read alias for %s....\n",
                                                                                __FUNCTION__, __LINE__, aAliasValue));
                return ANSC_STATUS_FAILURE;
            }
        }
        CcspTraceInfo(("%s %d: No entry found\n", __func__, __LINE__));
        // Reaching here means we could not find the required alias entry for this interface
        pValStruct->iEthPriorityMark = -1;
        pValStruct->iUpdateStatus = TRUE;
        return ANSC_STATUS_SUCCESS;

    }
    CcspTraceError(("%s:%d:: Could not retrieve table entries for %s....\n",
                                                        __FUNCTION__, __LINE__, aAliasValue));
    return ANSC_STATUS_FAILURE;
}

/* TelcoVoiceMgrDmlGetWanEthernetPriorityMark: */
/**
* @description Set ethernet priority mark for interfaces in WAN data model
*              interface table.
*
* @param PROTOCOL_TYPE protocol - protocol specifier, SIP/RTP
* @param int32_t       *iValue  - pointer for output value.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlGetWanEthernetPriorityMark(PROTOCOL_TYPE protocol, int32_t *iValue)
{
    ethPriorityValStruct_t valStruct;
    if(!iValue)
    {
        CcspTraceError(("%s:%d:: invalid parameter....\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_FAILURE;
    }
    valStruct.protocol = protocol;
    valStruct.iUpdateStatus = FALSE;
    if( TelcoVoiceMgrDmlIterateEnabledInterfaces(
                TelcoVoiceMgrDmlGetMarkingEthPriorityMark, (void *)&valStruct) == ANSC_STATUS_SUCCESS)
    {
        if(TRUE == valStruct.iUpdateStatus)
        {
            *iValue = valStruct.iEthPriorityMark;
            CcspTraceInfo(("%s:%d:: DML Data from WanMgr, Protocol[%s],iValue[%d]\n", __FUNCTION__, __LINE__,(protocol==0)?"SIP":"RTP",*iValue));
            return ANSC_STATUS_SUCCESS;
        }
    }
    RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.TELCOVOICEMANAGER", "Could not get Wan Ethernet Priority Mark..\n");
    return ANSC_STATUS_FAILURE;
}

/* TelcoVoiceMgrDmlSetWanEthernetPriorityMark: */
/**
* @description Set ethernet priority mark for interfaces in WAN data model
*              interface table.
*
* @param PROTOCOL_TYPE protocol - protocol specifier, SIP/RTP
* @param int32_t       iValue   - priority value to set.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetWanEthernetPriorityMark(PROTOCOL_TYPE protocol, int32_t iValue)
{
    ethPriorityValStruct_t valStruct;
    valStruct.protocol = protocol;
    valStruct.iEthPriorityMark = iValue;
    valStruct.iUpdateStatus = FALSE;
    CcspTraceInfo(("%s:%d:: Protocol[%d],iValue[%d]\n", __FUNCTION__, __LINE__,protocol,iValue));
    if( TelcoVoiceMgrDmlIterateEnabledInterfaces(
                TelcoVoiceMgrDmlSetMarkingEthPriorityMark, (void *)&valStruct) == ANSC_STATUS_SUCCESS)
    {
        if(TRUE == valStruct.iUpdateStatus)
        {
            return ANSC_STATUS_SUCCESS;
        }
    }
    RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.TELCOVOICEMANAGER", "Could not set Wan Ethernet Priority Mark..\n");
    return ANSC_STATUS_FAILURE;
}

/* TelcoVoiceMgrDmlGetEnabledWanSKBMarks: */
/**
* @description get SIP and RTP SKB marks from WAN data model interface entry whos
*              IP state is up.
*
* @param char* pIfRecordName - Interface entry name
* @param void* pArgs - aIpStateParamName, input, secifies ipv6/ipv4 to be looked for.
*                      bMarksRead, output, SKB Mark read status.
*                      sipSkbMark, output, SIP SKB mark value.
*                      ripSkbMark, output, RTP SKB mark.
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
* @retval ANSC_STATUS_DISCARD if ip state is not up.
*
* @execution Synchronous.
* @sideeffect None.
*
*/
static ANSC_STATUS TelcoVoiceMgrDmlGetEnabledWanSKBMarks(char *pIfRecordName, void *pArgs)
{
    int iLoopCount = 0;
    char paramName[512] = {0};
    char aParamValue[256] = {0};
    char aMrkRecordList[MAX_MARK_ENTRY][256] = {0};
    int numMarkingRecords = 0;

    retSkbMarksStruct_t *pSkbMarks;

    if(!(pArgs && pIfRecordName))
    {
        return ANSC_STATUS_FAILURE;
    }
    pSkbMarks = (retSkbMarksStruct_t *)pArgs;

    snprintf(paramName, sizeof(paramName), "%s%s", pIfRecordName, pSkbMarks->aIpStateParamName);
    if (ANSC_STATUS_FAILURE == TelcoVoiceMgrDmlGetParamValue(WAN_COMPONENT_NAME,
                                                        WAN_DBUS_PATH, paramName, aParamValue))
    {
        return ANSC_STATUS_FAILURE;
    }
    if (strncmp(PARAM_VALUE_IP_STATE_UP, aParamValue, sizeof(PARAM_VALUE_IP_STATE_UP)))
    {
        return ANSC_STATUS_DISCARD;
    }

    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetWanMarkingRecordNames(
                                 pIfRecordName,aMrkRecordList, &numMarkingRecords))
    {
        for(iLoopCount = 0; iLoopCount < numMarkingRecords; iLoopCount ++)
        {
            memset(paramName, 0, sizeof(paramName));
            memset(aParamValue, 0, sizeof(aParamValue));
            snprintf(paramName, sizeof(paramName), "%s%s", aMrkRecordList[iLoopCount],
                                                                        PARAM_NAME_MARK_ALIAS);
            if (ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetParamValue(WAN_COMPONENT_NAME,
                                                                WAN_DBUS_PATH, paramName, aParamValue))
            {
                if(!strncmp(PARAM_VALUE_SIP_ALIAS, aParamValue, sizeof(PARAM_VALUE_SIP_ALIAS)))
                {
                    memset(paramName, 0, sizeof(paramName));
                    memset(aParamValue, 0, sizeof(aParamValue));
                    snprintf(paramName, sizeof(paramName), "%s%s", aMrkRecordList[iLoopCount],
                                                                              PARAM_NAME_SKB_MARK);
                    TelcoVoiceMgrDmlGetParamValue(WAN_COMPONENT_NAME,
                                                        WAN_DBUS_PATH, paramName, aParamValue);
                    pSkbMarks->sipSkbMark = atoi(aParamValue);
                    pSkbMarks->bSipMarkRead = TRUE;

                }
                else if (!strncmp(PARAM_VALUE_RTP_ALIAS, aParamValue, sizeof(PARAM_VALUE_RTP_ALIAS)))
                {
                    memset(paramName, 0, sizeof(paramName));
                    memset(aParamValue, 0, sizeof(aParamValue));
                    snprintf(paramName, sizeof(paramName), "%s%s", aMrkRecordList[iLoopCount],
                                                                              PARAM_NAME_SKB_MARK);
                    TelcoVoiceMgrDmlGetParamValue(WAN_COMPONENT_NAME,
                                                        WAN_DBUS_PATH, paramName, aParamValue);
                    pSkbMarks->rtpSkbMark = atoi(aParamValue);
                    pSkbMarks->bRtpMarkRead = TRUE;
                }
            }
            else
            {
                return ANSC_STATUS_FAILURE;
            }
        }
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlGetWanSKBMarks: */
/**
* @description get SIP and RTP SKB marks from WAN data model.
*
* @param char* pIpStateParamName - ipv6/ipv4 state option specifier
* @param int32_t* pSipSkbMark - output the SIP SKB mark
* @param int32_t* pRtpSkbMark - output the RTP SKB mark
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlGetWanSKBMarks(char *pIpStateParamName, uint32_t *pSipSkbMark,  uint32_t *pRtpSkbMark)
{
    retSkbMarksStruct_t uSkbMarks;
    if(!(pSipSkbMark && pRtpSkbMark && pIpStateParamName))
    {
        return ANSC_STATUS_FAILURE;
    }

    uSkbMarks.bSipMarkRead = FALSE;
    uSkbMarks.bRtpMarkRead = FALSE;
    memset(uSkbMarks.aIpStateParamName,0,sizeof(uSkbMarks.aIpStateParamName));
    strncpy(uSkbMarks.aIpStateParamName, pIpStateParamName, (sizeof(uSkbMarks.aIpStateParamName)-1));
    if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlIterateEnabledInterfaces(
                                               TelcoVoiceMgrDmlGetEnabledWanSKBMarks, (void *)&uSkbMarks))
    {
        if(uSkbMarks.bSipMarkRead == TRUE)
        {
            *pSipSkbMark = uSkbMarks.sipSkbMark;
        }
        else
        {
            *pSipSkbMark = 0;
        }
        if(uSkbMarks.bRtpMarkRead == TRUE)
        {
            *pRtpSkbMark = uSkbMarks.rtpSkbMark;
        }
        else
        {
            *pRtpSkbMark = 0;
        }
        return ANSC_STATUS_SUCCESS;
    }
    return ANSC_STATUS_FAILURE;
}

/* TelcoVoiceMgrDmlSetLoopCurrentDisabled: */
/**
* @description set the loop current diabled
*
* @param uint32_t uiService - input the voice service index
* @param BOOL bStatus - input the Disable Loop Current Until Registered
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetLoopCurrentDisabled(uint32_t uiService, BOOL bStatus)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,VOICE_SERVICE_TABLE_NAME"%s",uiService,"X_RDK_DisableLoopCurrentUntilRegistered");
    if(bStatus)
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","true");
    }
    else
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","false");
    }
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_BOOLEAN,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService,TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "DisLoopCurrentReg", strValue);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetBoundIfname: */
/**
* @description set the bound If name
*
* @param uint32_t uiService - input the voice service index
* @param char* BoundIfname
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetBoundIfname(uint32_t uiService, char *BoundIfname)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!BoundIfname)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,VOICE_SERVICE_TABLE_NAME"%s",uiService,"X_RDK_BoundIfName");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",BoundIfname);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService,TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "Set-BoundIfName", BoundIfname);
    if(TelcoVoiceMgrSetSyseventData(SYSEVENT_UPDATE_IFNAME, BoundIfname) != ANSC_STATUS_SUCCESS)
    {
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetIpAddressFamily: */
/**
* @description set the name/address and port of the voice logging server
*
* @param uint32_t uiService - input the voice service index
* @param char* IpAddressFamily - input the Disable Loop Current Until Registered
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetIpAddressFamily(uint32_t uiService, char *IpAddressFamily)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!IpAddressFamily)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,VOICE_SERVICE_TABLE_NAME"%s",uiService,"X_RDK_IpAddressFamily");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",IpAddressFamily);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService,TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "Set-IpAddressFamily", IpAddressFamily);
    if(TelcoVoiceMgrSetSyseventData(SYSEVENT_UPDATE_IPFAMILY, IpAddressFamily) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceWarning(("%s :: sysevent_set Failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetBoundIpAddress: */
/**
* @description set the name/address and port of the voice logging server
*
* @param uint32_t uiService - input the voice service index
* @param char* BoundIpAddress- input the Disable Loop Current Until Registered
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetBoundIpAddress(uint32_t uiService, char *BoundIpAddress)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    if(!BoundIpAddress)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,VOICE_SERVICE_TABLE_NAME"%s",uiService,"X_RDK_BoundIpAddr");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",BoundIpAddress);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }

    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;
    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrDmlData == NULL)
    {
        CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_RESOURCES;
    }

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrDmlData->Service.VoiceService.pdata[uiService - 1];
    pDmlVoiceService = &(pVoiceService->dml);
    if ( !pDmlVoiceService )
    {
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        return ANSC_STATUS_RESOURCES;
    }
    AnscCopyString(pDmlVoiceService->X_RDK_BoundIpAddr, BoundIpAddress);

    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);

    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetCCTKTraceGroup: */
/**
* @description set the CCTK Trace Group
*
* @param uint32_t uiService - input the voice service index
* @param char* CCTKTraceGroup - input the Disable Loop Current Until Registered
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetCCTKTraceGroup(uint32_t uiService, char *CCTKTraceGroup)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!CCTKTraceGroup)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,X_RDK_DEBUG_TABLE_NAME"%s",uiService,"CCTKTraceGroup");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",CCTKTraceGroup);

    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService,TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "TraceGroup", CCTKTraceGroup);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetCCTKTraceLevel: */
/**
* @description set the CCTK TraceLevel
*
* @param uint32_t uiService - input the voice service index
* @param char* CCTKTraceLevel - input the CCTKTraceLevel Value
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetCCTKTraceLevel(uint32_t uiService, char *CCTKTraceLevel)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!CCTKTraceLevel)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,X_RDK_DEBUG_TABLE_NAME"%s",uiService,"CCTKTraceLevel");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",CCTKTraceLevel);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService,TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "TraceLevel", CCTKTraceLevel);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetModuleLogLevel: */
/**
* @description set the CCTK Trace Level
*
* @param uint32_t uiService - input the voice service index
* @param char* logLevels - input the Disable Loop Current Until Registered
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetModuleLogLevel(uint32_t uiService, char *logLevels)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!logLevels)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,X_RDK_DEBUG_TABLE_NAME"%s",uiService,"ModuleLogLevels");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",logLevels);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService,TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "ModuleLogLevels", logLevels);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetLogServer: */
/**
* @description set the name/address of the voice logging server
*
* @param uint32_t uiService - input the voice service index
* @param char* pLogServer - input the log server name
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetLogServer(uint32_t uiService, char* pLogServer)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!pLogServer)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,X_RDK_DEBUG_TABLE_NAME"%s",uiService,"LogServer");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",pLogServer);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService,TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "LogServer", pLogServer);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetLogServerPort: */
/**
* @description set the name/address and port of the voice logging server
*
* @param uint32_t uiService - input the voice service index
* @param ULONG uLSPort - input the log server port value
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetLogServerPort(uint32_t uiService, ULONG uLSPort)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,X_RDK_DEBUG_TABLE_NAME"%s",uiService,"LogServerPort");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%lu",uLSPort);

    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_UNSIGNED_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectInteger(uiService,TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "LogServerPort", uLSPort);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetLinkState: */
/**
* @description set Wan Link State Up/Down
* Used only from telcovoicemgr_nw_monitor.c to notify wan up/down to voice
* @param VOICE_HAL_IP_LINK_STATE_UP/VOICE_HAL_IP_LINK_STATE_DOWN
* @param Ip Address Family (IPv4/IPv6)
* @param IpAddress
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetLinkState(TELCOVOICEMGR_VOICE_IP_LINK_STATE linkState, char *ipAddrFamily, char *wanIpAddress)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    char dns_server_address[BUF_LEN_256]={0};
    ULONG uVsIndex = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;
    gLinkState = linkState;

    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrDmlData == NULL)
    {
        CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_RESOURCES;
    }

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrDmlData->Service.VoiceService.pdata[TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES - 1];
    pDmlVoiceService = &(pVoiceService->dml);
    if ( !pDmlVoiceService )
    {
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        return ANSC_STATUS_RESOURCES;
    }
    pDmlVoiceService->X_RDK_Enable = linkState;
    uVsIndex = pDmlVoiceService->InstanceNumber;
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);

    if(linkState == VOICE_HAL_IP_LINK_STATE_UP)
    {
        //Send Dns Server Address to voice stack
        TelcoVoiceMgrDmlGetDnsServers(&dns_server_address);
        if (TelcoVoiceMgrHal_SetLinkUp(uVsIndex, dns_server_address, ipAddrFamily, wanIpAddress) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("%s:%d::  Failed\n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        if(TelcoVoiceMgrDmlSetVoiceProcessState(uVsIndex, linkState) != ANSC_STATUS_SUCCESS)
        {
           CcspTraceError(("%s:%d::  Failed\n", __FUNCTION__, __LINE__));
           return ANSC_STATUS_FAILURE;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlGetDnsServers: */
/**
* @description send DnsServer Address to voice stack
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

static ANSC_STATUS TelcoVoiceMgrDmlGetDnsServers(char *dns_server_address)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    char ipAddrFamily[IP_ADDR_FAMILY_LENGTH] = {0};
    char dns_server_prim[48] = {0};
    char dns_server_sec[48] = {0};
    char ifName[32] = {0};
    char sysevent_param_name[32] = {0};

    if(dns_server_address == NULL)
    {
        return ANSC_STATUS_FAILURE;
    }

    sysevent_get(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_CURRENT_WAN_IFNAME, ifName, sizeof(ifName));
    if ((sysevent_get(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_FIELD_IPV6_DNS_PRIMARY, dns_server_prim, sizeof(dns_server_prim)) != 0) || (strlen(dns_server_prim) == 0))
    {
        //Look for Ipv4 Dns Server
        snprintf(sysevent_param_name, sizeof(sysevent_param_name), SYSEVENT_IPV4_DNS_PRIMARY, ifName);
        if ((sysevent_get(sysevent_voice_fd, sysevent_voice_token, sysevent_param_name, dns_server_prim, sizeof(dns_server_prim)) != 0) || (strlen(dns_server_prim) == 0))
        {
            //Set default
            CcspTraceError(("[%s]::[%d] Set default Dns Server Address !!!! \n", __FUNCTION__,__LINE__));
            snprintf(dns_server_prim, sizeof(dns_server_prim), "%s", "0.0.0.0");
        }
    }

    if ((sysevent_get(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_FIELD_IPV6_DNS_SECONDARY, dns_server_sec, sizeof(dns_server_sec)) != 0) || (strlen(dns_server_sec) == 0))
    {
        //Look for Ipv4 Dns Server
        memset(sysevent_param_name, 0, sizeof(sysevent_param_name));
        snprintf(sysevent_param_name, sizeof(sysevent_param_name), SYSEVENT_IPV4_DNS_SECONDARY, ifName);
        if ((sysevent_get(sysevent_voice_fd, sysevent_voice_token, sysevent_param_name, dns_server_sec, sizeof(dns_server_sec)) != 0) || (strlen(dns_server_sec) == 0))
        {
            //Set default
            CcspTraceError(("[%s]::[%d] Set default Dns Server Address !!!! \n", __FUNCTION__,__LINE__));
            snprintf(dns_server_sec, sizeof(dns_server_sec), "%s", "0.0.0.0");
        }
    }

    snprintf(dns_server_address,JSON_MAX_VAL_ARR_SIZE,"%s,%s", dns_server_prim, dns_server_sec);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlFactoryReset: */
/**
* @description Factory Reset Voice Process (Datamodel reset only)
*
* @param uint32_t uiService - input the voice service index
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlFactoryReset(uint32_t uiService, TELCOVOICEMGR_VOICE_ENABLE_ENUM VoiceState)
{
    char strName[JSON_MAX_STR_ARR_SIZE]={0};


    if(TelcoVoiceMgrDmlSetVoiceProcessState(uiService, VOICE_SERVICE_DISABLE) != ANSC_STATUS_SUCCESS)
    {
        CcspTraceWarning(("[%s]::[%d] Voice process stop failed!!!! \n", __FUNCTION__,__LINE__));
        return ANSC_STATUS_FAILURE;
    }
    CcspTraceWarning(("TELCOVOICEMANAGER_FACTORY_DEF :: Voice factory defaults requested\n"));
    if(voice_process_factory_default() != ANSC_STATUS_SUCCESS)
    {
        CcspTraceInfo(("%s %d - Error in initialising datamodels on factory reset. \n", __FUNCTION__, __LINE__ ));
        return ANSC_STATUS_FAILURE;
    }

    /* Platform specific operations for factory reset */
    snprintf(strName,JSON_MAX_STR_ARR_SIZE, VOICE_SERVICE_TABLE_NAME"%s",uiService,"X_RDK_FactoryReset");
    TelcoVoiceMgrHal_SetParam(strName, PARAM_BOOLEAN, "true");

    if(VoiceState != VOICE_SERVICE_DISABLE)
    {
        if(TelcoVoiceMgrDmlSetVoiceProcessState(uiService, VOICE_SERVICE_ENABLE) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceWarning(("[%s]::[%d] Voice process start failed!!!! \n", __FUNCTION__,__LINE__));
            return ANSC_STATUS_FAILURE;
        }
    }

    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetVoiceProcessState: */
/**
* @description set Voice Process State
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uState    - input the process state
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetVoiceProcessState(uint32_t uiService, uint32_t uState)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,VOICE_SERVICE_TABLE_NAME"%s",uiService,"X_RDK_Enable");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%lu",uState);
    if(uState == VOICE_SERVICE_ENABLE)
    {
        if(gLinkState == VOICE_HAL_IP_LINK_STATE_DOWN)
        {
            CcspTraceError(("%s:%d:: LinkStatus down. Cannot start voice application \n", __FUNCTION__, __LINE__));
            return ANSC_STATUS_SUCCESS;
        }
    }
    else
    {
        //reset sysevents
        sysevent_set(sysevent_voice_fd, sysevent_voice_token, "voice_ipv6_outbound_proxy_addresses", "", 0);
        sysevent_set(sysevent_voice_fd, sysevent_voice_token, "voice_ipv6_rtp_pinholes", "", 0);
        sysevent_set(sysevent_voice_fd, sysevent_voice_token, "voice_ipv6_ethernetpriority", "", 0);
        sysevent_set(sysevent_voice_fd, sysevent_voice_token, "voice_ipv6_dscp", "", 0);
    }
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_UNSIGNED_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlGetVoiceProcessStatus : */
/**
* @description Get Current Voice Process State
*
* @param uint32_t uiService - input the voice service index
* @param TELCOVOICEMGR_VOICE_STATUS_ENUM  *pvoice_status - voice process status
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlGetVoiceProcessStatus(uint32_t uiService,TELCOVOICEMGR_VOICE_STATUS_ENUM *pvoice_status)
{
    ANSC_STATUS returnStatus = ANSC_STATUS_FAILURE;
    hal_param_t req_param;
    memset(&req_param, 0, sizeof(req_param));

    //Validate buffer
    if ( NULL == pvoice_status )
    {
        CcspTraceError(("%s %d - Invalid buffer\n",__FUNCTION__,__LINE__));
        return returnStatus;
    }
    /**
     * Construct Full DML path.
     * Device.Services.VoiceService.%d.X_RDK_Status
     */
    snprintf(req_param.name, sizeof(req_param.name), VOICE_SERVICE_STATUS, uiService);
    if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
    {
        *pvoice_status = VOICE_PROCESS_STATUS_ERROR;
        CcspTraceError(("%s:%d:: X_RDK_Status:get failed \n", __FUNCTION__, __LINE__));
        return  returnStatus;
    }
    else
    {
        /**
         * Convert status message and returned.
         */
        if (strcmp(req_param.value, VOICE_STATUS_STOPPED) == 0)
        {
            *pvoice_status = VOICE_PROCESS_STATUS_STOPPED;
        }
        else if (strcmp(req_param.value, VOICE_STATUS_STARTING) == 0)
        {
            *pvoice_status = VOICE_PROCESS_STATUS_STARTING;
        }
        else if (strcmp(req_param.value, VOICE_STATUS_STARTED) == 0)
        {
            *pvoice_status = VOICE_PROCESS_STATUS_STARTED;
        }
        else if (strcmp(req_param.value, VOICE_STATUS_STOPPING) == 0)
        {
            *pvoice_status = VOICE_PROCESS_STATUS_STOPPING;
        }
        else
        {
            *pvoice_status = VOICE_PROCESS_STATUS_ERROR;
        }

        CcspTraceInfo(("%s:%d:: X_RDK_Status: %d\n", __FUNCTION__, __LINE__, *pvoice_status));
        returnStatus = ANSC_STATUS_SUCCESS;
    }
    return  returnStatus;
}

/* TelcoVoiceMgrDmlSetDirectoryNumber : */
/**
* @description set Directory Number
*
* @param uint32_t uiService - input the Voice Service Index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param char *pDirName     - input the Voice Directory
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetDirectoryNumber(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, char* pDirName)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!pDirName)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_TABLE_NAME"%s",uiService,uiLine,"DirectoryNumber");
#else
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_TABLE_NAME"%s",uiService,uiProfile,uiLine,"DirectoryNumber");
#endif
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",pDirName);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "DirectoryNumber", pDirName);

    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetLineEnable : */
/**
* @description set the Voice Line Enable
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param ULONG uLineEnable  - input the Voice Line Enable
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
ANSC_STATUS TelcoVoiceMgrDmlSetLineEnable(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, BOOL uLineEnable)
#else
ANSC_STATUS TelcoVoiceMgrDmlSetLineEnable(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, ULONG uLineEnable)
#endif
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_TABLE_NAME"%s",uiService,uiLine,"Enable");

    if(uLineEnable == TRUE)
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","true");
    }
    else
    {
        snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","false");
    }
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_BOOLEAN,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "Enable", uLineEnable == TRUE ?
        "Enabled" : "Disabled");
#else
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_TABLE_NAME"%s",uiService,uiProfile,uiLine,"Enable");

    if(uLineEnable == QUIESCENT)
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","Quiescent");
    }
    else if(uLineEnable == ENABLED)
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","Enabled");
    }
    else
    {
        snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","Disabled");
    }
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "Enable", uLineEnable == QUIESCENT ?
        "Quiescent" : uLineEnable == DISABLED ?
        "Disabled" : "Enabled");
#endif
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlGetLineStatus : */
/**
* @description get Line Status
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param uint32_t *pLineStatus - output the voice Line Status
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlGetLineStatus(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, uint32_t *pLineStatus)
{
    //Fetch line status from voice stack
    ANSC_STATUS returnStatus = ANSC_STATUS_FAILURE;
    hal_param_t req_param;
    memset(&req_param, 0, sizeof(req_param));

    //Validate buffer
    if ( NULL == pLineStatus )
    {
        CcspTraceError(("%s %d - Invalid buffer\n",__FUNCTION__,__LINE__));
        return returnStatus;
    }
    /**
     * Construct Full DML path.
     * Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.Status
     */
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(req_param.name, sizeof(req_param.name), LINE_STATUS, uiService, uiLine);
#else
    snprintf(req_param.name, sizeof(req_param.name), LINE_STATUS, uiService, uiProfile, uiLine);
#endif
    if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
    {
        *pLineStatus = VOICE_LINE_STATE_ERROR;
        CcspTraceError(("%s:%d:: Line Status:get failed \n", __FUNCTION__, __LINE__));
        return  returnStatus;
    }
    else
    {
        /**
         * Convert status message and returned.
         */
        if (strcmp(req_param.value, LINE_STATUS_UP) == 0)
        {
            *pLineStatus = VOICE_LINE_STATE_UP;
        }
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
        else if (strcmp(req_param.value, LINE_STATUS_INITIALIZING) == 0)
        {
            *pLineStatus = VOICE_LINE_STATE_INITIALIZING;
        }
        else if (strcmp(req_param.value, LINE_STATUS_REGISTERING) == 0)
        {
            *pLineStatus = VOICE_LINE_STATE_REGISTERING;
        }
        else if (strcmp(req_param.value, LINE_STATUS_UNREGISTERING) == 0)
        {
            *pLineStatus = VOICE_LINE_STATE_UNREGISTERING;
        }
#endif /*FEATURE_RDKB_VOICE_DM_TR104_V2*/
        else if (strcmp(req_param.value, LINE_STATUS_TESTING) == 0)
        {
            *pLineStatus = VOICE_LINE_STATE_TESTING;
        }
        else if (strcmp(req_param.value, LINE_STATUS_QUIESCENT) == 0)
        {
            *pLineStatus = VOICE_LINE_STATE_QUIESCENT;
        }
        else if (strcmp(req_param.value, LINE_STATUS_DISABLED) == 0)
        {
            *pLineStatus = VOICE_LINE_STATE_DISABLED;
        }
        else
        {
            *pLineStatus = VOICE_LINE_STATE_ERROR;
        }

        CcspTraceInfo(("%s:%d:: Line Status: %d\n", __FUNCTION__, __LINE__, *pLineStatus));
        returnStatus = ANSC_STATUS_SUCCESS;
    }
    return  returnStatus;
}

/* TelcoVoiceMgrDmlGetLineCallState : */
/**
* @description get Line Call State
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param ULONG  *pCallState - output the voice Line Status
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlGetLineCallState(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, ULONG* pCallState)
{
   //Fetch line status from voice stack
    ANSC_STATUS returnStatus = ANSC_STATUS_FAILURE;
    hal_param_t req_param;
    memset(&req_param, 0, sizeof(req_param));

    //Validate buffer
    if ( NULL == pCallState )
    {
        CcspTraceError(("%s %d - Invalid buffer\n",__FUNCTION__,__LINE__));
        return returnStatus;
    }
    /**
     * Construct Full DML path.
     * Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallState
     */
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(req_param.name, sizeof(req_param.name), CALL_STATE, uiService, uiLine);
#else
    snprintf(req_param.name, sizeof(req_param.name), CALL_STATE, uiService, uiProfile, uiLine);
#endif
    if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetSingleParameter(&req_param))
    {
        *pCallState = VOICE_CALL_STATE_IDLE;
        CcspTraceError(("%s:%d:: Call State get failed \n", __FUNCTION__, __LINE__));
        return  returnStatus;
    }
    else
    {
        /**
         * Convert status message and returned.
         */
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2

        if (strcmp(req_param.value, CALL_STATUS_IDLE) == 0)
        {
            *pCallState = CALLCTRL_STATUS_IDLE;
        }
        else if (strcmp(req_param.value, CALL_STATUS_DIALING) == 0)
        {
            *pCallState = CALLCTRL_STATUS_DIALING;
        }
        else if (strcmp(req_param.value, CALL_STATUS_DELIVERED) == 0)
        {
            *pCallState = CALLCTRL_STATUS_DELIVERED;
        }
        else if (strcmp(req_param.value, CALL_STATUS_CONNECTED) == 0)
        {
            *pCallState = CALLCTRL_STATUS_CONNECTED;
        }
        else if (strcmp(req_param.value, CALL_STATUS_ALERTING) == 0)
        {
            *pCallState = CALLCTRL_STATUS_ALERTING;
        }
        else if (strcmp(req_param.value, CALL_STATUS_DISCONNECTED) == 0)
        {
            *pCallState = CALLCTRL_STATUS_DISCONNECTED;
        }
        else
        {
            *pCallState = CALLCTRL_STATUS_IDLE;
        }
#else
        if (strcmp(req_param.value, CALL_STATUS_IDLE) == 0)
        {
            *pCallState = VOICE_CALL_STATE_IDLE;
        }
        else if (strcmp(req_param.value, CALL_STATUS_CALLING) == 0)
        {
            *pCallState = VOICE_CALL_STATE_CALLING;
        }
        else if (strcmp(req_param.value, CALL_STATUS_RINGING) == 0)
        {
            *pCallState = VOICE_CALL_STATE_RINGING;
        }
        else if (strcmp(req_param.value, CALL_STATUS_CONNECTING) == 0)
        {
            *pCallState = VOICE_CALL_STATE_CONNECTING;
        }
        else if (strcmp(req_param.value, CALL_STATUS_INCALL) == 0)
        {
            *pCallState = VOICE_CALL_STATE_INCALL;
        }
        else if (strcmp(req_param.value, CALL_STATUS_HOLD) == 0)
        {
            *pCallState = VOICE_CALL_STATE_HOLD;
        }
        else if (strcmp(req_param.value, CALL_STATUS_DISCONNECTING) == 0)
        {
            *pCallState = VOICE_CALL_STATE_DISCONNECTING;
        }
        else
        {
            *pCallState = VOICE_CALL_STATE_IDLE;
        }
#endif
        CcspTraceInfo(("%s:%d:: Call State: %d\n", __FUNCTION__, __LINE__, *pCallState));
        returnStatus = ANSC_STATUS_SUCCESS;
    }
    return  returnStatus;
}

/* TelcoVoiceMgrDmlSetOutboundProxy : */
/**
* @description Set Outbound Proxy
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param char *pProxyName   - input Outbound proxy Name
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetOutboundProxy(uint32_t uiService, uint32_t uiProfile, char* pProxyName)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!pProxyName)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"OutboundProxy");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",pProxyName);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "OutboundProxy", pProxyName);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetOutboundProxyPort : */
/**
* @description set Outbound Proxy Port
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param ULONG uValue       - input the Voice Outbound Proxy Port
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetOutboundProxyPort( uint32_t uiService, uint32_t uiProfile, ULONG uValue)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"OutboundProxyPort");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%lu",uValue);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_UNSIGNED_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "OutboundProxyPort", uValue);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetSipDscpMark : */
/**
* @description Set Sip Ethernet Priority Mark
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param int uValue         - input the DSCP Mark
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetSipDscpMark(uint32_t uiService, uint32_t uiProfile, ULONG uValue)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"DSCPMark");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%lu",uValue);

    /* Validate and restart voice process */
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_UNSIGNED_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }

    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "DSCPMark", uValue);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetSipEthernetPriorityMark : */
/**
* @description Set Sip Ethernet Priority Mark
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param int iValue         - input the Sip Ethernet Priority Mark
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetSipEthernetPriorityMark(uint32_t uiService, uint32_t uiProfile, int iValue)
{
    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = NULL;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    int32_t iEthValue = 0;

    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"EthernetPriorityMark");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%d",iValue);

    if(ANSC_STATUS_SUCCESS != TelcoVoiceMgrDmlGetWanEthernetPriorityMark(SIP, &iEthValue))
    {
        CcspTraceError(("%s:%d:: Couldnot read wan ethernet priority mark[%d]..\n", __FUNCTION__, __LINE__, iEthValue));
        return ANSC_STATUS_FAILURE;
    }

    if(iEthValue == iValue)
    {
        CcspTraceInfo(("%s:%d:: No change in value(%d)..\n", __FUNCTION__, __LINE__, iEthValue));
        return ANSC_STATUS_SUCCESS;
    }
    /* Validate parameter value */
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    CcspTraceInfo(("%s:%d:: Changing value from %d -> %d..\n", __FUNCTION__, __LINE__, iEthValue, iValue));
    /* We are already holding the dml lock */
    pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetData();
    pVoiceService = pTelcoVoiceMgrDmlData->Service.VoiceService.pdata[TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES - 1];
    pDmlVoiceService = &(pVoiceService->dml);
    pDmlVoiceService->X_RDK_Enable = VOICE_HAL_IP_LINK_STATE_DOWN;

    /* Set the voice down to allow deregister before wan goes down */
    TelcoVoiceMgrDmlSetVoiceProcessState(uiService,VOICE_SERVICE_DISABLE);
    sleep(1);

    if (TelcoVoiceMgrDmlSetWanEthernetPriorityMark(SIP, iValue) != ANSC_STATUS_SUCCESS)
    {
        return ANSC_STATUS_DISCARD;
    }
    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "EthernetPriorityMark", iValue);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrInitMark : */
/**
* @description Common Api to initialise the Marking with values from default Json file.
* Also invoked during factory reset.
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param PROTOCOL_TYPE protocol - input Sip/Rtp
* @param paramName  - input which parameter to be set
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrInitMark(uint32_t uiService, uint32_t uiProfile, int iValue, PROTOCOL_TYPE protocol, char* paramName)
{
    if(!paramName)
    {
        CcspTraceWarning(("%s Invalid ParamName\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    ANSC_STATUS returnStatus = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    PDML_VOIPPROFILE                  pDmlVoiceProfile   = NULL;
    PDML_SIP                          pDmlSipObj         = NULL;
    PDML_SIP_NETWORK                  pDmlSipNetwork     = NULL;
#else
    PTELCOVOICEMGR_DML_PROFILE        pDmlVoiceProfile   = NULL;
#endif
    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrDmlData == NULL)
    {
        CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_RESOURCES;
    }

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrDmlData->Service.VoiceService.pdata[uiService - 1];
    pDmlVoiceService = &(pVoiceService->dml);
    if ( !pDmlVoiceService )
    {
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        returnStatus = ANSC_STATUS_RESOURCES;
        goto EXIT;
    }
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    DML_VOIPPROFILE_CTRL_T* pVoiceProfile = pDmlVoiceService->VoIPProfile->pdata[uiProfile - 1];
#else
    DML_PROFILE_CTRL_T* pVoiceProfile = pDmlVoiceService->VoiceProfileList.pdata[uiProfile - 1];
#endif
    pDmlVoiceProfile = &(pVoiceProfile->dml);
    if ( !pDmlVoiceProfile )
    {
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        returnStatus = ANSC_STATUS_RESOURCES;
        goto EXIT;
    }
    switch(protocol)
    {
        case SIP:
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            pDmlSipObj = &(pDmlVoiceService->SIP_obj);
            if( !pDmlSipObj )
            {
                CcspTraceError(("%s:%d:: pDmlSipObj: NULL\n", __FUNCTION__, __LINE__));
                returnStatus = ANSC_STATUS_RESOURCES;
                goto EXIT;
            }

            DML_SIP_NETWORK_CTRL_T* pSipNetwork = pDmlSipObj->Network.pdata[uiProfile - 1];
            pDmlSipNetwork = &(pSipNetwork->dml);

            if ( pDmlSipNetwork == NULL )
            {
                CcspTraceError(("%s:%d:: pDmlSipNetwork: NULL\n", __FUNCTION__, __LINE__));
                returnStatus = ANSC_STATUS_RESOURCES;
                goto EXIT;
            }
#endif
            if(!strcmp(paramName, PARAM_NAME_SKB_MARK))
            {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
               pDmlSipNetwork->X_RDK_SKBMark = iValue;
#else
               pDmlVoiceProfile->SIPObj.X_RDK_SKBMark = iValue;
#endif
               returnStatus = ANSC_STATUS_SUCCESS;
            }
            else if(!strcmp(paramName, PARAM_NAME_ETHERNET_PRIORITY_MARK))
            {
                int getValue = -1;
                if ((TelcoVoiceMgrDmlGetWanEthernetPriorityMark(SIP, &getValue) == ANSC_STATUS_SUCCESS) &&
                    (getValue != -1) && getValue != iValue)
                {
                    if (TelcoVoiceMgrDmlSetWanEthernetPriorityMark(SIP, iValue) != ANSC_STATUS_SUCCESS)
                    {
                        CcspTraceInfo(("%s %d: Update Wanmgr: iValue[%d] Failed\n", __func__, __LINE__, iValue));
                        returnStatus = ANSC_STATUS_DISCARD;
                        goto EXIT;
                    }
                }
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
               pDmlSipNetwork->EthernetPriorityMark = iValue;
#else
               pDmlVoiceProfile->SIPObj.EthernetPriorityMark = iValue;
#endif
               returnStatus = ANSC_STATUS_SUCCESS;
            }
            else if(!strcmp(paramName, PARAM_NAME_DSCP_MARK))
            {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
               pDmlSipNetwork->DSCPMark = iValue;
#else
               pDmlVoiceProfile->SIPObj.DSCPMark = iValue;
#endif
               returnStatus = ANSC_STATUS_SUCCESS;
            }
            break;
        case RTP:
            if(!strcmp(paramName, PARAM_NAME_SKB_MARK))
            {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
               pDmlVoiceProfile->RTP.X_RDK_SKBMark = iValue;
#else
               pDmlVoiceProfile->RTPObj.X_RDK_SKBMark = iValue;
#endif
               returnStatus = ANSC_STATUS_SUCCESS;
            }
            else if(!strcmp(paramName, PARAM_NAME_ETHERNET_PRIORITY_MARK))
            {
                int getValue = -1;
                if ((TelcoVoiceMgrDmlGetWanEthernetPriorityMark(RTP, &getValue) == ANSC_STATUS_SUCCESS) && (getValue != -1) && getValue != iValue)
                {
                    if (TelcoVoiceMgrDmlSetWanEthernetPriorityMark(RTP, iValue) != ANSC_STATUS_SUCCESS)
                    {
                        CcspTraceInfo(("%s %d: Update Wanmgr: iValue[%d] Failed\n", __func__, __LINE__, iValue));
                        returnStatus = ANSC_STATUS_DISCARD;
                        goto EXIT;
                    }
                }
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
               pDmlVoiceProfile->RTP.EthernetPriorityMark = iValue;
#else
               pDmlVoiceProfile->RTPObj.EthernetPriorityMark = iValue;
#endif
               returnStatus = ANSC_STATUS_SUCCESS;
            }
            else if(!strcmp(paramName, PARAM_NAME_DSCP_MARK))
            {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
               pDmlVoiceProfile->RTP.DSCPMark = iValue;
#else
               pDmlVoiceProfile->RTPObj.DSCPMark = iValue;
#endif
               returnStatus = ANSC_STATUS_SUCCESS;
            }
            break;
        default:
            returnStatus = ANSC_STATUS_FAILURE;
    }

EXIT:
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    return returnStatus;
}

static ANSC_STATUS TelcoVoiceMgrDmlGetMarking(uint32_t uiService,  uint32_t uiProfile, uint32_t *sipSkbMark, uint32_t *rtpSkbMark, uint32_t *sipDscpMark, uint32_t *rtpDscpMark)
{
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    PDML_VOIPPROFILE                  pDmlVoiceProfile   = NULL;
    PDML_SIP                          pDmlSipObj         = NULL;
    PDML_SIP_NETWORK                  pDmlSipNetwork     = NULL;
#else
    PTELCOVOICEMGR_DML_PROFILE        pDmlVoiceProfile   = NULL;
#endif

    TELCOVOICEMGR_DML_DATA*  pTelcoVoiceMgrData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrData != NULL)
    {
        DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrData->Service.VoiceService.pdata[uiService - 1];
        pDmlVoiceService = &(pVoiceService->dml);
        if(!pDmlVoiceService)
        {
            CcspTraceWarning(("%s pDmlVoiceService NULL\n", __FUNCTION__));
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
            return ANSC_STATUS_FAILURE;
        }

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        DML_VOIPPROFILE_CTRL_T* pVoiceProfile = pDmlVoiceService->VoIPProfile->pdata[uiProfile - 1];
#else
        DML_PROFILE_CTRL_T* pVoiceProfile = pDmlVoiceService->VoiceProfileList.pdata[uiProfile - 1];
#endif
        pDmlVoiceProfile = &(pVoiceProfile->dml);
        if ( (!pDmlVoiceProfile) || !(pDmlVoiceProfile->Enable))
        {
            CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL or Disabled\n", __FUNCTION__, __LINE__));
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
            return ANSC_STATUS_FAILURE;
        }
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        pDmlSipObj = &(pDmlVoiceService->SIP_obj);
        if( !pDmlSipObj )
        {
            CcspTraceError(("%s:%d:: pDmlSipObj: NULL\n", __FUNCTION__, __LINE__));
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
            return ANSC_STATUS_RESOURCES;
        }

        DML_SIP_NETWORK_CTRL_T* pSipNetwork = pDmlSipObj->Network.pdata[uiProfile - 1];
        pDmlSipNetwork = &(pSipNetwork->dml);

        if ( pDmlSipNetwork == NULL )
        {
            CcspTraceError(("%s:%d:: pDmlSipNetwork: NULL\n", __FUNCTION__, __LINE__));
            TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
            return ANSC_STATUS_RESOURCES;
        }
        *sipSkbMark = pDmlSipNetwork->X_RDK_SKBMark;
        *sipDscpMark = pDmlSipNetwork->DSCPMark;
        *rtpSkbMark = pDmlVoiceProfile->RTP.X_RDK_SKBMark;
        *rtpDscpMark = pDmlVoiceProfile->RTP.DSCPMark;

#else
        *sipSkbMark = pDmlVoiceProfile->SIPObj.X_RDK_SKBMark;
        *sipDscpMark = pDmlVoiceProfile->SIPObj.DSCPMark;
        *rtpSkbMark = pDmlVoiceProfile->RTPObj.X_RDK_SKBMark;
        *rtpDscpMark = pDmlVoiceProfile->RTPObj.DSCPMark;
#endif
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrData);
    }

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS TelcoVoiceMgrDmlGetSeparateRuleLists(char *prevRtpRuleData, char*currRtpRuleData,
                                                        char *deleteList, char *addList, char *presentList, int listSize)
{
    char *pToken = NULL;
    int len = 0;
    int pLen = 0;
    char tempBuff[BUF_LEN_512] = {0};
    if(!(prevRtpRuleData && currRtpRuleData && deleteList && addList))
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }

    // Prepare list of rules to be deleted
    strncpy(tempBuff, prevRtpRuleData, sizeof(tempBuff));
    pToken = strtok(tempBuff, ";");
    while( pToken != NULL )
    {
        if(!strstr(currRtpRuleData, pToken))
        {
            len += snprintf((char *)&deleteList[len], listSize, "%s;" , pToken);
        }
        else{
            pLen += snprintf((char *)&presentList[pLen], listSize, "%s;" , pToken);
        }
        pToken = strtok(NULL, ";");
    }

    // Prepare list of rules to be added newly
    strncpy(tempBuff, currRtpRuleData, sizeof(tempBuff));
    pToken = strtok(tempBuff, ";");
    len = 0;
    while( pToken != NULL )
    {
        if(!strstr(prevRtpRuleData, pToken))
        {
            len += snprintf((char *)&addList[len], listSize, "%s;" , pToken);
            pLen += snprintf((char *)&presentList[pLen], listSize, "%s;" , pToken);
        }
        pToken = strtok(NULL, ";");
    }
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetX_RDK_FirewallRuleData : */
/**
* @description Set Firewall Rule Data to sysevent
* @param PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetX_RDK_FirewallRuleData(char * FirewallRuleData, ULONG uVsIndex, ULONG uVpQuantity)
{
    char ethernetPriorityBuffer[BUF_LEN_512] = {0};
    char dscpBuffer[BUF_LEN_512] = {0};
    char ipAddrFamily[IP_ADDR_FAMILY_LENGTH] = {0};
    char sipOutBoundProxyBuffer[BUF_LEN_512] = {0};
    char rtpPinholeBuffer[BUF_LEN_512] = {0};
    static char prevRtpRuleData[BUF_LEN_512] = {0};
    char addList[BUF_LEN_512] = {0};
    char deleteList[BUF_LEN_512] = {0};
    char presentList[BUF_LEN_512] = {0};
    static uint prevRtpDscpMark = 0;
    static uint prevRtpSkbMark = 0;
    ULONG  uVpIndex = 0;
    static TELCOVOICEMGR_VOICE_IP_ADD_FAMILY previpAddressFamily = VOICE_HAL_AF_INET_V4;
    uint sipSkbMark, rtpSkbMark, sipDscpMark, rtpDscpMark;

    if(!FirewallRuleData)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    if (sysevent_get(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_UPDATE_IPFAMILY, ipAddrFamily, sizeof(ipAddrFamily)) != 0)
    {
        CcspTraceError(("Failed to get ipAddressFamily from sysevent \n"));
        return ANSC_STATUS_FAILURE;
    }

    for(uVpIndex = 1; uVpIndex <= uVpQuantity; uVpIndex++)
    {
        TelcoVoiceMgrDmlGetMarking(uVsIndex, uVpIndex, &sipSkbMark, &rtpSkbMark, &sipDscpMark, &rtpDscpMark);
        memset(ethernetPriorityBuffer, 0, sizeof(ethernetPriorityBuffer));
        memset(dscpBuffer, 0, sizeof(dscpBuffer));
        memset(sipOutBoundProxyBuffer, 0, sizeof(sipOutBoundProxyBuffer));
        memset(rtpPinholeBuffer, 0, sizeof(rtpPinholeBuffer));
        if(generate_voice_firewall_sysevent_string(FirewallRuleData,
                        sipSkbMark,
                        rtpSkbMark,
                        sipDscpMark,
                        rtpDscpMark,
                        ethernetPriorityBuffer,
                        dscpBuffer,
                        sipOutBoundProxyBuffer,
                        rtpPinholeBuffer,
                        BUF_LEN_512) != ANSC_STATUS_SUCCESS)
        {
            CcspTraceError(("%s Failed to generate sysevent\n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }  

        if( !strcmp(ipAddrFamily, STR_IPV4) )
        {
            /*
            * Prepare sysevent for SKBMark, firewall rules set from utopia based on this sysevent value
            * eg:Format: sipIp1,sipPort1,sipSKBMark;sipIp2,sipPort2,sipSKBMark;rtpIp1,rtpPort1,rtpSKBMark;rtpIp2,rtpPort2,rtpSKBMark;
            */
            sysevent_set(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_VOICE_IPV4_ETHERNETPRIORITY, ethernetPriorityBuffer, 0);
            CcspTraceInfo(("[%s:%d] SYSEVENT_VOICE_IPV4_ETHERNETPRIORITY %s\n", __FUNCTION__, __LINE__, ethernetPriorityBuffer));

            /*
            * Prepare sysevent for DSCPMark, firewall rules set from utopia based on this sysevent value
            * eg:Format: sipIp1,sipPort1,sipDSCPMark;sipIp2,sipPort2,sipDSCPMark;rtpIp1,rtpPort1,rtpDSCPMark;rtpIp2,rtpPort2,rtpDSCPMark;
            */
            sysevent_set(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_VOICE_IPV4_DSCP, dscpBuffer, 0);
            CcspTraceInfo(("[%s:%d] SYSEVENT_VOICE_IPV4_DSCP %s\n", __FUNCTION__, __LINE__, dscpBuffer));

            sysevent_set(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_VOICE_IPV4_PROXYLIST, sipOutBoundProxyBuffer, 0);
            CcspTraceInfo(("[%s:%d] SYSEVENT_VOICE_IPV4_PROXYLIST %s\n", __FUNCTION__, __LINE__, sipOutBoundProxyBuffer));


            /* Delete old rule and add new one for RTP*/
            if(prevRtpRuleData[0] != '\0')
            {
                set_iptable_rules_for_rtp(prevRtpRuleData, prevRtpDscpMark, prevRtpSkbMark, previpAddressFamily, DELETE_RULE);
            }

            //For RTP events, set sysevent and apply iptable rules from here, do not restart firewall.
            set_iptable_rules_for_rtp(rtpPinholeBuffer, rtpDscpMark, rtpSkbMark, VOICE_HAL_AF_INET_V4, ADD_RULE);

            sysevent_set(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_VOICE_IPV4_RTPLIST, rtpPinholeBuffer, 0);
            CcspTraceInfo(("[%s:%d] SYSEVENT_VOICE_IPV4_RTPLIST %s\n", __FUNCTION__, __LINE__, rtpPinholeBuffer));

            if((rtpPinholeBuffer[0] == '\0') && (prevRtpRuleData[0] == '\0'))
            {
                //Restart firewall for only SIP events.
                //Iptable rules for RTP are explicitly added without firewall restart.
                firewall_restart_for_voice(UTOPIA_FIREWALL_RESTART_TIMEOUT_MS);
            }
            /* Save previous data and delete old rules in next iteration.*/
            snprintf(prevRtpRuleData,sizeof(prevRtpRuleData), "%s", rtpPinholeBuffer);
            prevRtpDscpMark = rtpDscpMark;
            prevRtpSkbMark = rtpSkbMark;
            previpAddressFamily = VOICE_HAL_AF_INET_V4;
        }
        else if( !strcmp(ipAddrFamily, STR_IPV6) )
        {
            /*
            * Prepare sysevent for SKBMark, firewall rules set from utopia based on this sysevent value
            * eg:Format: sipIp1,sipPort1,sipSKBMark;sipIp2,sipPort2,sipSKBMark;rtpIp1,rtpPort1,rtpSKBMark;rtpIp2,rtpPort2,rtpSKBMark;
            */
            sysevent_set(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_VOICE_IPV6_ETHERNETPRIORITY, ethernetPriorityBuffer, 0);
            CcspTraceInfo(("[%s:%d] SYSEVENT_VOICE_IPV6_ETHERNETPRIORITY %s\n", __FUNCTION__, __LINE__, ethernetPriorityBuffer));

            /*
            * Prepare sysevent for DSCPMark, firewall rules set from utopia based on this sysevent value
            * eg:Format: sipIp1,sipPort1,sipDSCPMark;sipIp2,sipPort2,sipDSCPMark;rtpIp1,rtpPort1,rtpDSCPMark;rtpIp2,rtpPort2,rtpDSCPMark;
            */
            sysevent_set(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_VOICE_IPV6_DSCP, dscpBuffer, 0);
            CcspTraceInfo(("[%s:%d] SYSEVENT_VOICE_IPV6_DSCP %s\n", __FUNCTION__, __LINE__, dscpBuffer));

            sysevent_set(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_VOICE_IPV6_PROXYLIST, sipOutBoundProxyBuffer, 0);
            CcspTraceInfo(("[%s:%d] SYSEVENT_VOICE_IPV6_PROXYLIST %s\n", __FUNCTION__, __LINE__, sipOutBoundProxyBuffer));

            /* Delete old rule and add new one for RTP*/
            if(prevRtpRuleData[0] != '\0')
            {
                strncpy(deleteList, prevRtpRuleData, sizeof(deleteList));
                strncpy(addList, rtpPinholeBuffer, sizeof(addList));
                if (prevRtpDscpMark == rtpDscpMark && prevRtpSkbMark == rtpSkbMark)
                {
                    memset(addList, 0, sizeof(addList));
                    memset(deleteList, 0, sizeof(deleteList));
                    TelcoVoiceMgrDmlGetSeparateRuleLists(prevRtpRuleData, rtpPinholeBuffer, deleteList, addList, presentList, sizeof(addList));
                }
                if(deleteList[0] != '\0')
                {
                    set_iptable_rules_for_rtp(deleteList, prevRtpDscpMark, prevRtpSkbMark, previpAddressFamily, DELETE_RULE);
                }
                if(addList[0] != '\0')
                {
                    set_iptable_rules_for_rtp(addList, rtpDscpMark, rtpSkbMark, VOICE_HAL_AF_INET_V6, ADD_RULE);
                }
            }
            else if (rtpPinholeBuffer[0] != '\0')
            {
                //For RTP events, set sysevent and apply iptable rules from here, do not restart firewall.
                set_iptable_rules_for_rtp(rtpPinholeBuffer, rtpDscpMark, rtpSkbMark, VOICE_HAL_AF_INET_V6, ADD_RULE);
                strncpy(presentList, rtpPinholeBuffer, sizeof(presentList));
            }

            sysevent_set(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_VOICE_IPV6_RTPLIST, presentList, 0);
            CcspTraceInfo(("[%s:%d] SYSEVENT_VOICE_IPV6_RTPLIST %s\n", __FUNCTION__, __LINE__, presentList));

            if(FirewallRuleData[0] == 0)
            { // Need to restart the firewall to remove the SIP rules in case of clearing the data model.
                prevRtpRuleData[0] = 0;
            }

            if((rtpPinholeBuffer[0] == '\0') && (prevRtpRuleData[0] == '\0'))
            {
                //Restart firewall for only SIP events.
                //Iptable rules for RTP are explicitly added without firewall restart.
                firewall_restart_for_voice(UTOPIA_FIREWALL_RESTART_TIMEOUT_MS);
            }
            /* Save previous data and delete old rules in next iteration.*/
            snprintf(prevRtpRuleData,sizeof(prevRtpRuleData), "%s", presentList);
            prevRtpDscpMark = rtpDscpMark;
            prevRtpSkbMark = rtpSkbMark;
            previpAddressFamily = VOICE_HAL_AF_INET_V6;
        }
        else
        {
            CcspTraceError(("Invalid ipAddressFamily, Failed to set sysevent for Sip firewall rule \n"));
            return ANSC_STATUS_FAILURE;
        }
    }

    return ANSC_STATUS_SUCCESS;
}


/* TelcoVoiceMgrDmlSetProxyServer : */
/**
* @description Set Proxy Server
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param char *pProxyServer - input the Proxy Server
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetProxyServer(uint32_t uiService, uint32_t uiProfile, char* pProxyServer)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!pProxyServer)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"ProxyServer");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",pProxyServer);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "ProxyServer", pProxyServer);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetProxyServerPort : */
/**
* @description Set Proxy Server Port
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param ULONG uValue       - input the Voice Proxy Server Port
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetProxyServerPort(uint32_t uiService, uint32_t uiProfile, ULONG uValue)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"ProxyServerPort");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%lu",uValue);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_UNSIGNED_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "ProxyServerPort", uValue);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetRegistrarServer : */
/**
* @description Set Registrar Server.
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param char *pRegServer   - Input the Voice Registrar Server
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetRegistrarServer(uint32_t uiService, uint32_t uiProfile, char* pRegServer)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!pRegServer)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"RegistrarServer");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",pRegServer);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "RegistrarServer", pRegServer);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetRegistrarServerPort : */
/**
* @description Set Registrar Server Port
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param ULONG    uValue    - input the Voice Registrar Server Port
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetRegistrarServerPort(uint32_t uiService, uint32_t uiProfile, ULONG uValue)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"RegistrarServerPort");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%lu",uValue);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_UNSIGNED_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE,"RegistrarServerPort", uValue);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetUserAgentDomain : */
/**
* @description Set User Agent  Domain
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param char *pUsrAgent    - input the User Agent Domain
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetUserAgentDomain(uint32_t uiService, uint32_t uiProfile, char* pUsrAgent)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!pUsrAgent)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"UserAgentDomain");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",pUsrAgent);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "UserAgentDomain", pUsrAgent);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetConferencingURI : */
/**
* @description Set Conference URI
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param char*    pConfURI  - input the Conference Server URI
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetConferencingURI(uint32_t uiService, uint32_t uiProfile, char* pConfURI)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!pConfURI)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"X_RDK-Central_COM_ConferencingURI");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",pConfURI);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "ConferencingURI", pConfURI);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetNetworkDisconnect : */
/**
* @description Set Network Disconnect Enable
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param BOOL bvalue        - input Network Disconnect Enable
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetNetworkDisconnect(uint32_t uiService, uint32_t uiProfile, BOOL bValue)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"X_RDK-Central_COM_NetworkDisconnect");

    if(bValue)
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","true");
    }
    else
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","false");
    }
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_BOOLEAN,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "NetworkDisconnect",
        bValue ? bTrueStr : bFalseStr);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetPrackRequired : */
/**
* @description Set PrackRequired Enable flag
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param BOOL *pValue       - input PrackRequired Enable flag
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetPrackRequired(uint32_t uiService, uint32_t uiProfile, BOOL bValue)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,SIP_TABLE_NAME"%s",uiService,uiProfile,"X_RDK_PRACKRequired");
    if(bValue)
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","true");
    }
    else
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","false");
    }
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_BOOLEAN,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "PRACKRequired",
        bValue ? bTrueStr : bFalseStr);
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetRtpDscpMark : */
/**
* @description Set RTP DSCP Mark
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param int uValue         - input the DSCP Mark
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetRtpDscpMark(uint32_t uiService, uint32_t uiProfile, ULONG uValue)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,RTP_TABLE_NAME"%s",uiService,uiProfile,"DSCPMark");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%lu",uValue);

    /* Validate the Dscp value */
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_UNSIGNED_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }

    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "RtpDSCPMark", uValue);
    return ANSC_STATUS_SUCCESS;
}


/* TelcoVoiceMgrDmlSetRtpEthernetPriorityMark : */
/**
* @description Set RTP Ethernet Priority Mark
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param int iValue         - input the value of RTP Ethernet Priority Mark
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetRtpEthernetPriorityMark(uint32_t uiService, uint32_t uiProfile, int iValue)
{
    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = NULL;
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = NULL;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;
    int32_t iEthValue = 0;

    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,RTP_TABLE_NAME"%s",uiService,uiProfile,"EthernetPriorityMark");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%d",iValue);

    if(ANSC_STATUS_SUCCESS != TelcoVoiceMgrDmlGetWanEthernetPriorityMark(RTP, &iEthValue))
    {
        CcspTraceError(("%s:%d:: Couldnot read wan ethernet priority mark[%d]..\n", __FUNCTION__, __LINE__, iEthValue));
        return ANSC_STATUS_FAILURE;
    }

    if(iEthValue == iValue)
    {
        CcspTraceInfo(("%s:%d:: No change in value(%d)..\n", __FUNCTION__, __LINE__, iEthValue));
        return ANSC_STATUS_SUCCESS;
    }

    /* Validate parameter value */
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    CcspTraceInfo(("%s:%d:: Changing value from %d -> %d..\n", __FUNCTION__, __LINE__, iEthValue, iValue));
    /* We are already holding the dml lock */
    pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetData();
    pVoiceService = pTelcoVoiceMgrDmlData->Service.VoiceService.pdata[TELCOVOICEMGR_DML_NUMBER_OF_VOICE_SERVICES - 1];
    pDmlVoiceService = &(pVoiceService->dml);
    pDmlVoiceService->X_RDK_Enable = VOICE_HAL_IP_LINK_STATE_DOWN;

    /* Set the voice down to allow deregister before wan goes down */
    TelcoVoiceMgrDmlSetVoiceProcessState(uiService,VOICE_SERVICE_DISABLE);
    sleep(1);

    if (TelcoVoiceMgrDmlSetWanEthernetPriorityMark(RTP, iValue) != ANSC_STATUS_SUCCESS)
    {
        return ANSC_STATUS_DISCARD;
    }
    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "RtpEthernetPriorityMark", iValue);
    return ANSC_STATUS_SUCCESS;
}


/* TelcoVoiceMgrDmlGetEthernetPriorityMark : */
/**
* @description Get SIP Ethernet Priority Mark
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param int        *pValue - output the SIP Ethernet Priority Mark
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlGetEthernetPriorityMark(uint32_t uiService, uint32_t uiProfile, PROTOCOL_TYPE protocol, int* pValue)
{
    ANSC_STATUS                       returnStatus       = ANSC_STATUS_FAILURE;
    PTELCOVOICEMGR_DML_VOICESERVICE   pDmlVoiceService   = NULL;
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    PDML_VOIPPROFILE                  pDmlVoiceProfile   = NULL;
    PDML_SIP                          pDmlSipObj         = NULL;
    PDML_SIP_NETWORK                  pDmlSipNetwork     = NULL;
#else
    PTELCOVOICEMGR_DML_PROFILE        pDmlVoiceProfile   = NULL;
    PTELCOVOICEMGR_DML_SIP pDmlSipObj = NULL;
    PTELCOVOICEMGR_DML_RTP pDmlRtpObj = NULL;
#endif

    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrDmlData == NULL || pValue == NULL)
    {
        CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked Failed / pValue: [NULL]\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_RESOURCES;
    }

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrDmlData->Service.VoiceService.pdata[uiService - 1];
    pDmlVoiceService = &(pVoiceService->dml);
    if ( !pDmlVoiceService )
    {
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        returnStatus = ANSC_STATUS_RESOURCES;
        goto EXIT;
    }
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    PDML_VOIPPROFILE_LIST_T    pVoipProfileList = pDmlVoiceService->VoIPProfile;
    DML_VOIPPROFILE_CTRL_T* pVoiceProfile = pVoipProfileList->pdata[uiProfile - 1];
#else
    DML_PROFILE_CTRL_T* pVoiceProfile = pDmlVoiceService->VoiceProfileList.pdata[uiProfile - 1];
#endif
    pDmlVoiceProfile = &(pVoiceProfile->dml);
    if ( !pDmlVoiceProfile )
    {
        CcspTraceError(("%s:%d:: pDmlVoiceProfile: NULL\n", __FUNCTION__, __LINE__));
        returnStatus = ANSC_STATUS_RESOURCES;
        goto EXIT;
    }
    switch(protocol)
    {
        case SIP:
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            pDmlSipObj = &(pDmlVoiceService->SIP_obj);
            if( pDmlSipObj )
            {
                DML_SIP_NETWORK_CTRL_T* pSipNetwork = pDmlSipObj->Network.pdata[uiProfile - 1];
                pDmlSipNetwork = &(pSipNetwork->dml);

                if ( pDmlSipNetwork )
                {
                    CcspTraceWarning(("%s:%d:: Sip Ethernet Priority (DML): %d\n", __FUNCTION__, __LINE__, pDmlSipNetwork->EthernetPriorityMark));
                    *pValue = pDmlSipNetwork->EthernetPriorityMark;
                    returnStatus = ANSC_STATUS_SUCCESS;
                    goto EXIT;
                }
            }
#else
            pDmlSipObj = &(pDmlVoiceProfile->SIPObj);
            if(pDmlSipObj != NULL)
            {
                CcspTraceInfo(("%s:%d:: Sip Ethernet Priority: %d\n", __FUNCTION__, __LINE__, pDmlSipObj->EthernetPriorityMark));
                *pValue = pDmlSipObj->EthernetPriorityMark;
                returnStatus = ANSC_STATUS_SUCCESS;
                goto EXIT;
            }
#endif
            else
            {
                CcspTraceWarning(("%s:%d:: Sip Ethernet Priority: NULL\n", __FUNCTION__, __LINE__));
                returnStatus = ANSC_STATUS_FAILURE;
                goto EXIT;
            }
            break;
        case RTP:
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
            if(pDmlVoiceProfile->RTP.EthernetPriorityMark != NULL)
            {
                CcspTraceWarning(("%s:%d:: Rtp Ethernet Priority (DML): %d\n", __FUNCTION__, __LINE__, pDmlVoiceProfile->RTP.EthernetPriorityMark));
                *pValue = pDmlVoiceProfile->RTP.EthernetPriorityMark;
                returnStatus = ANSC_STATUS_SUCCESS;
                goto EXIT;
            }
#else
            pDmlRtpObj = &(pDmlVoiceProfile->RTPObj);
            if(pDmlRtpObj != NULL)
            {
                CcspTraceInfo(("%s:%d:: Rtp Ethernet Priority: %d\n", __FUNCTION__, __LINE__, pDmlRtpObj->EthernetPriorityMark));
                *pValue = pDmlRtpObj->EthernetPriorityMark;
                returnStatus = ANSC_STATUS_SUCCESS;
                goto EXIT;
            }
#endif
            else
            {
                CcspTraceWarning(("%s:%d:: Rtp Ethernet Priority: NULL\n", __FUNCTION__, __LINE__));
                returnStatus = ANSC_STATUS_FAILURE;
                goto EXIT;
            }
            break;
        default:
            CcspTraceWarning(("%s:%d:: Invalid protocol\n", __FUNCTION__, __LINE__));
            returnStatus = ANSC_STATUS_FAILURE;
            goto EXIT;
    }

EXIT:
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    CcspTraceWarning(("%s:%d:: returnStatus[%s]\n", __FUNCTION__, __LINE__,(returnStatus==ANSC_STATUS_SUCCESS)?"Success":"Failed"));
    return returnStatus;
}

/* TelcoVoiceMgrDmlSetDigitMap: */
/**
* @description set the Digit map
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param char* pDigitMap - input the digitmap value
* @param char* typeOfDigitMap - input the digitmap type
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetDigitMap(uint32_t uiService, uint32_t uiProfile, char* pDigitMap, char* typeOfDigitMap)
{
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    char strValue[MAXLENGTH_DIGITMAP]={0};

    if(!pDigitMap || !typeOfDigitMap)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    if(!strcmp(typeOfDigitMap, "X_RDK-Central_COM_DigitMap"))
    {
       snprintf(strName,JSON_MAX_STR_ARR_SIZE,PROFILE_TABLE_NAME"%s",uiService, uiProfile, "X_RDK-Central_COM_DigitMap");
       (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "CustomDigitMap", pDigitMap);
    }
    else if(!strcmp(typeOfDigitMap, "X_RDK-Central_COM_EmergencyDigitMap"))
    {
       snprintf(strName,JSON_MAX_STR_ARR_SIZE,PROFILE_TABLE_NAME"%s",uiService, uiProfile, "X_RDK-Central_COM_EmergencyDigitMap");
       (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "EmDigitMap", pDigitMap);
    }
    else if(!strcmp(typeOfDigitMap, "DigitMap"))
    {
       snprintf(strName,JSON_MAX_STR_ARR_SIZE,PROFILE_TABLE_NAME"%s",uiService, uiProfile, "DigitMap");
       (void)storeObjectString(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE,TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "DigitMap", pDigitMap);
    }
    else
    {
       CcspTraceWarning(("%s Invalid type passed, set failed\n", __FUNCTION__));
       return ANSC_STATUS_FAILURE;
    }
    snprintf(strValue,MAXLENGTH_DIGITMAP,"%s",pDigitMap);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetDigitTimer: */
/**
* @description set the Digit Timer
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param ULONG uDgtTimer - input the digital timer value
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetSDigitTimer(uint32_t uiService, uint32_t uiProfile, ULONG uDgtTimer)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE, PROFILE_TABLE_NAME"%s", uiService, uiProfile, "X_RDK-Central_COM_SDigitTimer");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%lu",uDgtTimer);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_UNSIGNED_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "SDigitTimer", uDgtTimer);
    return ANSC_STATUS_SUCCESS;
}


/* TelcoVoiceMgrDmlSetDigitTimer: */
/**
* @description set the Digit Timer
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param ULONG uDgtTimer - input the digital timer value
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/
ANSC_STATUS TelcoVoiceMgrDmlSetZDigitTimer(uint32_t uiService, uint32_t uiProfile, ULONG uDgtTimer)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE, PROFILE_TABLE_NAME"%s", uiService, uiProfile, "X_RDK-Central_COM_ZDigitTimer");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%lu",uDgtTimer);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_UNSIGNED_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "ZDigitTimer", uDgtTimer);

    return ANSC_STATUS_SUCCESS;
}

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
/* TelcoVoiceMgrDmlV2_MapLineStats : */
/**
* @description Map Line Stats for V2
*
* @param PDML_CALLCONTROL_LINE pDmlCallCtrlLine - Line Stats struct to be filled
* @param TELCOVOICEMGR_DML_VOICESERVICE_STATS *pStats - Line Stats temporary struct
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDml_MapLineStats(PDML_CALLCONTROL_LINE pDmlCallCtrlLine, TELCOVOICEMGR_DML_VOICESERVICE_STATS *pStats)
{
    if ( NULL == pDmlCallCtrlLine  || NULL == pStats )
    {
        CcspTraceError(("%s %d - Invalid Parameter\n",__FUNCTION__,__LINE__));
        return ANSC_STATUS_FAILURE;
    }
    pDmlCallCtrlLine->Stats.IncomingCalls.CallsReceived = pStats->IncomingCallsReceived;
    pDmlCallCtrlLine->Stats.IncomingCalls.CallsConnected = pStats->IncomingCallsConnected;
    pDmlCallCtrlLine->Stats.IncomingCalls.CallsDropped = pStats->IncomingCallsDropped;
    pDmlCallCtrlLine->Stats.IncomingCalls.CallsFailed = pStats->IncomingCallsFailed;
    pDmlCallCtrlLine->Stats.IncomingCalls.TotalCallTime = pStats->IncomingTotalCallTime;
    pDmlCallCtrlLine->Stats.OutgoingCalls.CallsAttempted = pStats->OutgoingCallsAttempted;
    pDmlCallCtrlLine->Stats.OutgoingCalls.CallsConnected = pStats->OutgoingCallsConnected;
    pDmlCallCtrlLine->Stats.OutgoingCalls.CallsFailed = pStats->OutgoingCallsFailed;
    pDmlCallCtrlLine->Stats.OutgoingCalls.CallsDropped = pStats->OutgoingCallsDropped;
    pDmlCallCtrlLine->Stats.OutgoingCalls.TotalCallTime = pStats->OutgoingTotalCallTime;
    pDmlCallCtrlLine->Stats.RTP.PacketsReceived = pStats->PacketsReceived;
    pDmlCallCtrlLine->Stats.RTP.PacketsSent = pStats->PacketsSent;
    pDmlCallCtrlLine->Stats.RTP.PacketsLost = pStats->PacketsLost;
    pDmlCallCtrlLine->Stats.RTP.BytesSent = pStats->BytesSent;
    pDmlCallCtrlLine->Stats.RTP.BytesReceived = pStats->BytesReceived;
    pDmlCallCtrlLine->Stats.DSP.Overruns = pStats->Overruns;
    pDmlCallCtrlLine->Stats.DSP.Underruns = pStats->Underruns;

    return ANSC_STATUS_SUCCESS;
}
#endif
/* TelcoVoiceMgrDmlGetLineStats : */
/**
* @description Get Line Stats
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param TELCOVOICEMGR_DML_VOICESERVICE_STATS *pStats - output the Line Stats
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/


ANSC_STATUS TelcoVoiceMgrDmlGetLineStats(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, TELCOVOICEMGR_DML_VOICESERVICE_STATS *pStats)
{
    //Fetch line stats from voice stack
    ANSC_STATUS returnStatus = ANSC_STATUS_FAILURE;
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    //Validate buffer
    if ( NULL == pStats )
    {
        CcspTraceError(("%s %d - Invalid buffer\n",__FUNCTION__,__LINE__));
        return returnStatus;
    }
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    // Device.Services.VoiceService.%d.CallControl.Line.%d.Stats.
    snprintf(strName, JSON_MAX_STR_ARR_SIZE, LINE_STATS_TABLE_NAME, uiService, uiLine);
#else
    /**
     * Construct Full DML path.
     * Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.Stats
     */
    snprintf(strName, JSON_MAX_STR_ARR_SIZE, LINE_STATS_TABLE_NAME, uiService, uiProfile, uiLine);
#endif
    if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetLineStats(strName, pStats))
    {
        CcspTraceError(("%s:%d:: Line Stats get failed \n", __FUNCTION__, __LINE__));
        return  returnStatus;
    }

    CcspTraceInfo(("%s:%d:: Line Stats fetched \n", __FUNCTION__, __LINE__));
    return  ANSC_STATUS_SUCCESS;
}

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
ANSC_STATUS TelcoVoiceMgrDmlGetCallLogStats(uint32_t uiService, uint32_t uiCallLog, uint32_t uiSession, TELCOVOICEMGR_DML_VOICESERVICE_CALLLOG_STATS *pStats)
{
    //Fetch line stats from voice stack
    ANSC_STATUS returnStatus = ANSC_STATUS_FAILURE;
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    //Validate buffer
    if ( NULL == pStats )
    {
        CcspTraceError(("%s %d - Invalid buffer\n",__FUNCTION__,__LINE__));
        return returnStatus;
    }

    // Device.Services.VoiceService.%d.CallLog.%d.Session.%d.Source.RTP
    snprintf(strName, JSON_MAX_STR_ARR_SIZE, CALLLOG_SRC_RTP_STATS, uiService, uiCallLog,uiSession);
    if (ANSC_STATUS_SUCCESS != TelcoVoiceHal_GetCallLogStats(strName, pStats))
    {
        CcspTraceError(("%s:%d:: call log Stats get failed \n", __FUNCTION__, __LINE__));
        return  returnStatus;
    }

    CcspTraceInfo(("%s:%d:: call log Stats fetched \n", __FUNCTION__, __LINE__));
    return  ANSC_STATUS_SUCCESS;
}
#endif

#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
/* TelcoVoiceMgrDmlResetLineStats : */
/**
* @description Reset Line Stats
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t    uiLine - input the Reset Line Stats
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlResetLineStats(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine)
{
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};


    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_STATS_TABLE_NAME"%s",uiService,uiProfile,uiLine,"ResetStatistics");
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","true");
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_BOOLEAN,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}
#endif

/* TelcoVoiceMgrDmlSetReceiveGain : */
/**
* @description Set Receive Gain
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param int32_t   iGain    - input the value of Receive Gain
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetReceiveGain(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, int32_t iGain)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_VOICE_PROCESSING_TABLE_NAME"%s",uiService,uiProfile,uiLine,"ReceiveGain");
#else
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_VOICE_PROCESSING_TABLE_NAME"%s",uiService,uiLine,"ReceiveGain");
#endif
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%d",iGain);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "RxGain", iGain);

    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetTransmitGain : */
/**
* @description Set Transmit Gain
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param int32_t   iGain    - input the value of Transmit Gain
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetTransmitGain(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, int32_t iGain)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_VOICE_PROCESSING_TABLE_NAME"%s",uiService,uiProfile,uiLine,"TransmitGain");
#else
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_VOICE_PROCESSING_TABLE_NAME"%s",uiService,uiLine,"TransmitGain");
#endif
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%d",iGain);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_INTEGER,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectInteger(uiService, uiProfile, TELCOVOICEMGR_DML_NUMBER_OF_LINE, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "TxGain", iGain);

    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetLineCallingFeatures : */
/**
* @description Set Line Calling Features
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM  eFeature  - Calling Feature Type
* @param BOOL *pStatus  - input the voice Line calling feature status
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetLineCallingFeatures(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM eFeature, BOOL bStatus)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(eFeature == VOICE_CALLING_FEATURE_CALL_WAITING)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
       snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME,uiService,uiProfile,"CallWaitingEnable");
#else
       snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME"%s",uiService,uiProfile,uiLine,"CallWaitingEnable");
#endif
       (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "CallingFeaturesCWI",
        bStatus ? bTrueStr : bFalseStr);
    }
    else if(eFeature == VOICE_CALLING_FEATURE_MSG_WAIT_INDICATOR)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME"%s",uiService,uiProfile,"MWIEnable");
#else
        snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME"%s",uiService,uiProfile,uiLine,"MWIEnable");
#endif
        (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "CallingFeaturesMWI",
         bStatus ? bTrueStr : bFalseStr);
    }
    else if(eFeature == VOICE_CALLING_FEATURE_CONF_CALL)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME"%s",uiService,uiProfile,"X_RDK-Central_COM_ConferenceCallingEnable");
#else
        snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME"%s",uiService,uiProfile,uiLine,"X_RDK-Central_COM_ConferenceCallingEnable");
#endif
        (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "CallingFeaturesCCE",
         bStatus ? bTrueStr : bFalseStr);
    }
    else if(eFeature == VOICE_CALLING_FEATURE_HOLD)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME"%s",uiService,uiProfile,"X_RDK-Central_COM_HoldEnable");
#else
        snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME"%s",uiService,uiProfile,uiLine,"X_RDK-Central_COM_HoldEnable");
#endif
        (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "CallingFeaturesHE",
         bStatus ? bTrueStr : bFalseStr);
    }
    else if(eFeature == VOICE_CALLING_FEATURE_CALLER_ID)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME"%s",uiService,uiProfile,"X_RDK-Central_COM_PhoneCallerIDEnable");
#else
        snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_CALING_FEATURE_TABLE_NAME"%s",uiService,uiProfile,uiLine,"X_RDK-Central_COM_PhoneCallerIDEnable");
#endif
        (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "CallingFeaturesCID",
         bStatus ? bTrueStr : bFalseStr);
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    if(bStatus)
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","true");
    }
    else
    {
       snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s","false");
    }
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_BOOLEAN,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetLineSIPAuthCredentials : */
/**
* @description Set Line SIP AuthCredentials
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param char*    pBuffer   - input the user name OR Password buffer
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/

ANSC_STATUS TelcoVoiceMgrDmlSetLineSIPAuthCredentials(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, TELCOVOICEMGR_VOICE_CREDENTIAL_TYPE_ENUM eAuthCredential, char* pBuffer)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    uint32_t inLen, outLen; char *pOutBuf = NULL;

    if(!pBuffer)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    if(eAuthCredential == VOICE_HAL_AUTH_UNAME)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
       snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_SIP_TABLE_NAME"%s",uiService,uiLine,"AuthUserName");
#else
       snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_SIP_TABLE_NAME"%s",uiService,uiProfile,uiLine,"AuthUserName");
#endif
       (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "AuthUserName",pBuffer);
    }
    else if(eAuthCredential == VOICE_HAL_AUTH_PWD)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
       snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_SIP_TABLE_NAME"%s",uiService,uiLine,"AuthPassword");
#else
       snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_SIP_TABLE_NAME"%s",uiService,uiProfile,uiLine,"AuthPassword");
#endif
        inLen = strlen(pBuffer);
        outLen = 1 + 2*inLen;
        /* Encrypt the password before storing in NVRAM */
        pOutBuf = malloc(outLen);
        if(pOutBuf)
        {
            jsonPwdEncode(pBuffer, inLen, pOutBuf, outLen);
            (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "AuthPassword",pOutBuf);
            free(pOutBuf);
        }
        else
        {
            CcspTraceError(("%s pOutBuf NULL, Set failed\n", __FUNCTION__));
            return ANSC_STATUS_FAILURE;
        }
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",pBuffer);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    return ANSC_STATUS_SUCCESS;
}

/* TelcoVoiceMgrDmlSetLineSipURI : */
/**
* @description Set Line Sip URI
*
* @param uint32_t uiService - input the voice service index
* @param uint32_t uiProfile - input the Voice Profile Index
* @param uint32_t uiLine    - input the Voice Line Index
* @param char     *pSipURI  - input pointer to a buffer holding the SIP URI
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*
* @execution Synchronous.
* @sideeffect None.
*
*/


ANSC_STATUS TelcoVoiceMgrDmlSetLineSipURI(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, char *pSipURI)
{
    char strValue[JSON_MAX_VAL_ARR_SIZE]={0};
    char strName[JSON_MAX_STR_ARR_SIZE]={0};

    if(!pSipURI)
    {
        CcspTraceWarning(("%s Null Value passed, set failed\n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_SIP_TABLE_NAME"%s",uiService,uiLine,"RegisterURI");
#else
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,LINE_SIP_TABLE_NAME"%s",uiService,uiProfile,uiLine,"URI");
#endif
    snprintf(strValue,JSON_MAX_VAL_ARR_SIZE,"%s",pSipURI);
    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,strValue) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }
    (void)storeObjectString(uiService, uiProfile, uiLine, TELCOVOICEMGR_DML_NUMBER_OF_PHY_INTERFACE, "URI",pSipURI);

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS TelcoVoiceMgrDmlSetTestState(uint32_t uiService, uint32_t uiPhyInterface, ULONG uState)
{
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    char testState[JSON_MAX_VAL_ARR_SIZE]={0};
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#define FIELD_NAME "DiagnosticsState"
#else
#define FIELD_NAME "TestState"
#endif //FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(strName,JSON_MAX_STR_ARR_SIZE,PHYINTERFACE_TABLE_NAME"%s",uiService,uiPhyInterface,FIELD_NAME);
    switch(uState)
    {
        case PHYINTERFACE_TESTSTATE_NONE:
            snprintf(testState, JSON_MAX_VAL_ARR_SIZE, "%s", "None");
            break;
        case PHYINTERFACE_TESTSTATE_REQUESTED:
            snprintf(testState, JSON_MAX_VAL_ARR_SIZE, "%s", "Requested");
            break;
        case PHYINTERFACE_TESTSTATE_COMPLETE:
            snprintf(testState, JSON_MAX_VAL_ARR_SIZE, "%s", "Complete");
            break;
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        case PHYINTERFACE_TESTSTATE_ERROR_INTERNAL:
            snprintf(testState, JSON_MAX_VAL_ARR_SIZE, "%s", "Error_Internal");
            break;
        case PHYINTERFACE_TESTSTATE_ERROR_OTHER:
            snprintf(testState, JSON_MAX_VAL_ARR_SIZE, "%s", "Error_Other");
            break;
#else
        case PHYINTERFACE_TESTSTATE_ERROR_TESTNOTSUPPORTED:
            snprintf(testState, JSON_MAX_VAL_ARR_SIZE, "%s", "Error_TestNotSupported");
            break;
#endif
        default:
            CcspTraceWarning(("[%s][%d] Invalid \n", __FUNCTION__,__LINE__));
            return ANSC_STATUS_FAILURE;
    }

    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,testState) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }

    (void)storeObjectString(uiService, TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE, uiPhyInterface, "DiagnosticsState", testState);
    return ANSC_STATUS_SUCCESS;
}


ANSC_STATUS TelcoVoiceMgrDmlSetTestSelector(uint32_t uiService, uint32_t uiPhyInterface, ULONG uState)
{
    char strName[JSON_MAX_STR_ARR_SIZE]={0};
    char testSelector[JSON_MAX_VAL_ARR_SIZE]={0};

    snprintf(strName,JSON_MAX_STR_ARR_SIZE,PHYINTERFACE_TABLE_NAME"%s",uiService,uiPhyInterface,"TestSelector");
    switch(uState)
    {
        case PHYINTERFACE_TESTSELECTOR_PHONE_CONNECTIVITY_TEST:
            snprintf(testSelector, JSON_MAX_VAL_ARR_SIZE, "%s", "PhoneConnectivityTest");
            break;
        case PHYINTERFACE_TESTSELECTOR_HAZARD_POTENTIAL:
            snprintf(testSelector, JSON_MAX_VAL_ARR_SIZE, "%s", "Hazard Potential");
            break;
        case PHYINTERFACE_TESTSELECTOR_FOREIGN_VOLTAGE:
            snprintf(testSelector, JSON_MAX_VAL_ARR_SIZE, "%s", "Foreign Voltage");
            break;
        case PHYINTERFACE_TESTSELECTOR_RESISTIVE_FAULTS:
            snprintf(testSelector, JSON_MAX_VAL_ARR_SIZE, "%s", "Resistive Faults");
            break;
        case PHYINTERFACE_TESTSELECTOR_OFF_HOOK:
            snprintf(testSelector, JSON_MAX_VAL_ARR_SIZE, "%s", "Off-hook");
            break;
        case PHYINTERFACE_TESTSELECTOR_REN:
            snprintf(testSelector, JSON_MAX_VAL_ARR_SIZE, "%s", "REN");
            break;
        default:
            CcspTraceWarning(("[%s][%d] Invalid \n", __FUNCTION__,__LINE__));
            return ANSC_STATUS_FAILURE;
    }

    if (TelcoVoiceMgrHal_SetParam(strName,PARAM_STRING,testSelector) != ANSC_STATUS_SUCCESS)
    {
       return ANSC_STATUS_FAILURE;
    }

    (void)storeObjectString(uiService, TELCOVOICEMGR_DML_NUMBER_OF_VOICE_PROFILE, TELCOVOICEMGR_DML_NUMBER_OF_LINE, uiPhyInterface, "TestSelector", testSelector);
    return ANSC_STATUS_SUCCESS;
}
