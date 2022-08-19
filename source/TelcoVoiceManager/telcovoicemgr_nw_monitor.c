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


/*!********************************************************************
 *
 * FILE: telcovoicemgr_nw_monitor.c
 *
 *
 * MODULE: Voice Manager
 *
 * Date Created: 21/05/2019
 *
 * Description: Voice Manager public interface
 *
 * Notes:
 *
 *********************************************************************/

/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/un.h>
#include <pthread.h>
#include <sys/stat.h>
#include <net/if.h>
#include <sysevent.h>
#include <syscfg.h>
#include "telcovoicemgr_nw_monitor.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"

/* ---- Global Constants -------------------------- */
#define LAN_IF         "LAN"
#define WAN_IF         "Any_WAN"
#define TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX   1
#define TELCOVOICEMGR_DML_VOICE_PROFILE_INDEX   1
#define SYSEVENT_IPV4_CONNECTION_STATE "ipv4_connection_state"
#define SYSEVENT_IPV6_CONNECTION_STATE "ipv6_connection_state"
#define SYSEVENT_IPV4_IP_ADDRESS "ipv4_%s_ipaddr"
#define SYSEVENT_IPV6_PREFIX "ipv6_prefix"
/*WAN/LAN specific sysevent fieldnames*/
#define SYSEVENT_CURRENT_WAN_IFNAME "current_wan_ifname"
#define SYSEVENT_LAN_STATUS "lan-status"
#define SYSEVENT_LAN_ADDRESS "current_lan_ipaddr"
#define SYSEVENT_FIREWALL_STATUS "firewall-status"
#define SYSEVENT_FIREWALL_RESTART "firewall-restart"
#define IP_ADDR_FAMILY_LENGTH 32
#define BOUND_IF_NAME_LENGTH 32

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
// V2 Functionality to update SKB Mark
#define SIP_INDEX TELCOVOICEMGR_DML_NUMBER_OF_SIP_NETWORK
#define RTP_INDEX TELCOVOICEMGR_DML_NUMBER_OF_VOIP_PROFILE
#else
// V1 Functionality to update SKB Mark
#define SIP_INDEX TELCOVOICEMGR_DML_VOICE_PROFILE_INDEX
#define RTP_INDEX TELCOVOICEMGR_DML_VOICE_PROFILE_INDEX
#endif
/* ---- Global Variables ------------------------------------ */
typedef enum {
    WAN_ERROR,
    WAN_UP,
    WAN_DOWN
} WanState_e;

typedef enum {
    FIREWALLSTATUS_STARTING,
    FIREWALLSTATUS_STARTED,
    FIREWALLSTATUS_STOPPING,
    FIREWALLSTATUS_STOPPED
} FirewallStatus_e;

extern bool bTelcoVoiceManagerRunning;
extern int sysevent_voice_fd;
extern token_t sysevent_voice_token;
static pthread_t sysevent_tid;
static int sysevent_rw_fd = -1;
static token_t sysevent_token = -1;
static int sysevent_listen_notfiy_fd = -1;
static token_t sysevent_notify_token = -1;
static char ipAddrFamily[IP_ADDR_FAMILY_LENGTH] = {0};
static char lanIpAddr[IP_ADDR_FAMILY_LENGTH] = {0};
static char boundIfName[BOUND_IF_NAME_LENGTH] = {0};
static int firewall_status = FIREWALLSTATUS_STARTING;

/* ---- Private Function Prototypes -------------------------- */
static void voice_event_handler(char *pEvtName, char *pEvtValue);
static void event_set_lan_status (void);
static void event_set_wan_status (void);
static void *voice_manager_nw_monitor(void *data);


void voicemgr_create_nw_monitor()
{
    int length = BOUND_IF_NAME_LENGTH;

    PTELCOVOICEMGR_DML_VOICESERVICE       pDmlVoiceService    = NULL;

    TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = TelcoVoiceMgrDmlGetDataLocked();
    if(pTelcoVoiceMgrDmlData == NULL)
    {
        CcspTraceError(("%s:%d:: TelcoVoiceMgrDmlGetDataLocked: Failed\n", __FUNCTION__, __LINE__));
        return ANSC_STATUS_RESOURCES;
    }
    DML_VOICE_SERVICE_CTRL_T* pVoiceService = pTelcoVoiceMgrDmlData->Service.VoiceService.pdata[TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX - 1];
    if(pVoiceService == NULL)
    {
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        return ANSC_STATUS_RESOURCES;
    }
    pDmlVoiceService = &(pVoiceService->dml);

    if ( !pDmlVoiceService )
    {
        CcspTraceError(("%s:%d:: pDmlVoiceService: NULL\n", __FUNCTION__, __LINE__));
        TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
        return ANSC_STATUS_RESOURCES;
    }
    if(pDmlVoiceService->X_RDK_BoundIfName != NULL)
    {
        CcspTraceWarning(("%s:%d:: boundIfName: %s\n", __FUNCTION__, __LINE__, pDmlVoiceService->X_RDK_BoundIfName));
        AnscCopyString(boundIfName, pDmlVoiceService->X_RDK_BoundIfName);
    }
    else
    {
        AnscCopyString(boundIfName, LAN_IF);
        CcspTraceWarning(("BoundIfName Null, proceed with LAN interface\n"));
    }

    if(pDmlVoiceService->X_RDK_IpAddressFamily != NULL)
    {
        CcspTraceWarning(("%s:%d:: ipAddrFamily: %s\n", __FUNCTION__, __LINE__, pDmlVoiceService->X_RDK_IpAddressFamily));
        AnscCopyString(ipAddrFamily, pDmlVoiceService->X_RDK_IpAddressFamily);
    }
    else
    {
        CcspTraceWarning(("Get IpAddressFamily Failed, Proceed with IPv4 !\n"));
        AnscCopyString(ipAddrFamily, STR_IPV4);
    }
    TelcoVoiceMgrDmlGetDataRelease(pTelcoVoiceMgrDmlData);
    /*LAN IPv6 address is not ready until WAN IPv6 address is ready.
        In both LAN and WAN modes, the voice process bounds to the same interface and IP.*/
    if (!strcmp(boundIfName, LAN_IF) && !strcmp(ipAddrFamily, STR_IPV4) )
    {
        event_set_lan_status();
    }
    else /* Any_WAN or LAN IPv6 */
    {
        event_set_wan_status();
    }
    pthread_create(&sysevent_tid, NULL, voice_manager_nw_monitor, NULL);
}

int voicemgr_sysevent_init()
{
    sysevent_rw_fd =  sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "voip", &sysevent_token);
    if (sysevent_rw_fd < 0)
        return -1;
    // Using a separate fd for listening to sysevent notifications to avoid sysevent read issues that may occur
    // while using the same fd for notification as well as sysevent get/set.
    sysevent_listen_notfiy_fd =  sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "voip_notify", &sysevent_notify_token);
    if (sysevent_listen_notfiy_fd < 0)
        return -1;
    return 0;
}

void voicemgr_sysevent_close()
{
    if (0 <= sysevent_rw_fd)
    {
        sysevent_close(sysevent_rw_fd, sysevent_token);
    }
}

static void *voice_manager_nw_monitor(void *data)
{
    async_id_t ifname_asyncid;
    async_id_t ipfamily_asyncid;
    async_id_t ipv4_connection_asyncid;
    async_id_t ipv6_connection_asyncid;
    async_id_t lan_connection_asyncid;
    async_id_t firewallstatus_asyncid;

    /*LAN Events*/
    sysevent_set_options(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_LAN_STATUS, TUPLE_FLAG_EVENT);
    sysevent_setnotification(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_LAN_STATUS,  &lan_connection_asyncid);

    /*WAN Events*/
    sysevent_set_options(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_IPV4_CONNECTION_STATE, TUPLE_FLAG_EVENT);
    sysevent_setnotification(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_IPV4_CONNECTION_STATE,  &ipv4_connection_asyncid);
    sysevent_set_options(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_IPV6_CONNECTION_STATE, TUPLE_FLAG_EVENT);
    sysevent_setnotification(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_IPV6_CONNECTION_STATE,  &ipv6_connection_asyncid);

    /*TELCO VOICE MANAGER Events*/
    sysevent_set_options(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_UPDATE_IFNAME, TUPLE_FLAG_EVENT);
    sysevent_setnotification(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_UPDATE_IFNAME,  &ifname_asyncid);
    sysevent_set_options(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_UPDATE_IPFAMILY, TUPLE_FLAG_EVENT);
    sysevent_setnotification(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_UPDATE_IPFAMILY,  &ipfamily_asyncid);

    /*Firewall Status*/
    sysevent_set_options(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_FIREWALL_STATUS, TUPLE_FLAG_EVENT);
    sysevent_setnotification(sysevent_listen_notfiy_fd, sysevent_notify_token, SYSEVENT_FIREWALL_STATUS,  &firewallstatus_asyncid);

    pthread_detach(pthread_self());

    while(1)
    {
        async_id_t getnotification_asyncid;
        unsigned char name[32] = {0}, val[40] = {0};
        int namelen = sizeof(name);
        int vallen  = sizeof(val);

        CcspTraceWarning(("Waiting for new wan event . . . !\n"));
        int err = sysevent_getnotification(sysevent_listen_notfiy_fd, sysevent_notify_token, name, &namelen, val, &vallen, &getnotification_asyncid); //blocking api
        if (0 != err)
        {
            if (!bTelcoVoiceManagerRunning)
            {
                break;
            }
            CcspTraceWarning(("internal communication error! : %d\n", err));
            sleep(2);
            continue;
        }
        CcspTraceWarning(("Notified Event : %s, Value : %s\n", name, val));
        voice_event_handler(name, val);
    }
    return NULL;
}

static void voice_event_handler(char *pEvtName, char *pEvtValue)
{
    CcspTraceWarning(("voice_event_handler => Evt Name : %s , Value : %s , IfName : %s, Family : %s\n", pEvtName, pEvtValue, boundIfName, ipAddrFamily));
    if (!strcmp(pEvtName, SYSEVENT_UPDATE_IFNAME))
    {
        strncpy(boundIfName, pEvtValue,(sizeof(boundIfName)-1));
        if (!strcmp(boundIfName, LAN_IF))
        {
            if(!strcmp(ipAddrFamily, STR_IPV4))
            {
                event_set_lan_status();
            }
            else if (!strcmp(ipAddrFamily, STR_IPV6))
            {
               /*LAN IPv6 address is not ready until WAN IPv6 address is ready.
                In both LAN and WAN modes, the voice process bounds to the same interface and IP.*/
                event_set_wan_status();
            }
        }
        else if (!strcmp(boundIfName, WAN_IF))
        {
            event_set_wan_status();
        }
    }
    else if (!strcmp(pEvtName, SYSEVENT_UPDATE_IPFAMILY))
    {
        strncpy(ipAddrFamily, pEvtValue,(sizeof(ipAddrFamily)-1));
        if (!strcmp(boundIfName, WAN_IF))
        {
            event_set_wan_status();
        }
        else if (!strcmp(boundIfName, LAN_IF))
        {
            if(!strcmp(ipAddrFamily, STR_IPV4))
            {
                event_set_lan_status();
            }
            else if (!strcmp(ipAddrFamily, STR_IPV6))
            {
              /*LAN IPv6 address is not ready until WAN IPv6 address is ready.
                In both LAN and WAN modes, the voice process bounds to the same interface and IP.*/
                event_set_wan_status();
            }
        }
    }
    else if (!strcmp(pEvtName, SYSEVENT_LAN_STATUS) && !strcmp(boundIfName, LAN_IF))
    {
        event_set_lan_status();
    }
    else if (!strcmp(pEvtName, SYSEVENT_IPV4_CONNECTION_STATE) && !strcmp(ipAddrFamily, STR_IPV4))
    {
        /* Check the current configured Interface name before setting the
           voice source IPv4 address to avoid overwriting LAN IPv4 address
           with WAN IPv4 address */
        if (!strcmp(boundIfName, WAN_IF))
        {
            event_set_wan_status();
        }
    }
    else if (!strcmp(pEvtName, SYSEVENT_IPV6_CONNECTION_STATE) && !strcmp(ipAddrFamily, STR_IPV6))
    {
        event_set_wan_status();
    }
    else if ( !strcmp(pEvtName, SYSEVENT_FIREWALL_STATUS) )
    {
        if ( !strcmp(pEvtValue, "started") )
        {
            firewall_status = FIREWALLSTATUS_STARTED;
        }
    }

}

static void event_set_lan_status (void)
{
    char lanStatus[BOUND_IF_NAME_LENGTH] = {0};
    char ipAddr[48] = {0};

    if (sysevent_get(sysevent_rw_fd, sysevent_token, SYSEVENT_LAN_STATUS, lanStatus, sizeof(lanStatus)) != 0)
    {
        CcspTraceError(("Failed to get lan status from sysevent \n"));
    }
    else
    {
        if (!strcmp(lanStatus, "started"))
        {
            if (!strcmp(ipAddrFamily, STR_IPV4))
            {
                if (sysevent_get(sysevent_rw_fd, sysevent_token, SYSEVENT_LAN_ADDRESS, ipAddr, sizeof(ipAddr)) == 0)
                {
                    CcspTraceInfo(("LAN - ipv4 address from sysevent : %s \n", ipAddr));
                    CcspTraceWarning(("Voice Manager: LAN up !\n"));
                    printf("Voice Manager: LAN up !\n");
                    CcspTraceNotice(("TELCOVOICEMANAGER_LANUP :: Voice Manager: LAN up\n"));
                    fflush(stdout);
                    TelcoVoiceMgrDmlSetBoundIpAddress(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, ipAddr);
                    TelcoVoiceMgrDmlSetLinkState(VOICE_HAL_IP_LINK_STATE_UP, ipAddrFamily, ipAddr);
                }
                else
                {
                    CcspTraceError(("Failed to get lan ipv4 address from sysevent \n"));
                }
            }
        }
        else
        {
            CcspTraceWarning(("Voice Manager: LAN down !\n"));
            CcspTraceNotice(("TELCOVOICEMANAGER_LANDOWN :: Voice Manager: LAN down\n"));
            printf("Voice Manager: LAN down !\n");
            fflush(stdout);
            TelcoVoiceMgrDmlSetLinkState(VOICE_HAL_IP_LINK_STATE_DOWN, ipAddrFamily, NULL);
        }
    }
}

static void event_set_wan_status (void)
{
    char ifName[BOUND_IF_NAME_LENGTH] = {0};
    char conState[12] = {0};
    char ipAddr[48] = {0};
    uint32_t uSipSkbMark = 0;
    uint32_t uRtpSkbMark = 0;
    int32_t iEthPriority;

    if (!strcmp(ipAddrFamily, STR_IPV4))
    {
        if (sysevent_get(sysevent_rw_fd, sysevent_token, SYSEVENT_IPV4_CONNECTION_STATE, conState, sizeof(conState)) != 0 )
        {
            CcspTraceError(("Failed to get wan ipv6 connection state sysevent \n"));
        }
        else
        {
            if (!strcmp(conState, "up"))
            {
                if (sysevent_get(sysevent_rw_fd, sysevent_token, SYSEVENT_CURRENT_WAN_IFNAME, ifName, sizeof(ifName)) == 0)
                {
                    char sysevent_param_name[32] = {0};
                    snprintf(sysevent_param_name, sizeof(sysevent_param_name), SYSEVENT_IPV4_IP_ADDRESS, ifName);
                    if (sysevent_get(sysevent_rw_fd, sysevent_token, sysevent_param_name, ipAddr, sizeof(ipAddr)) == 0)
                    {
                        CcspTraceInfo(("%s:: WAN IPv4 Address updated! { %s }\n", __FUNCTION__, ipAddr));
                        CcspTraceNotice(("TELCOVOICEMANAGER_IPV4_WANUP :: Voice Manager: IPV4 WAN up\n"));

                        //update the SKBmark reading from Wan Manager
                        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetEthernetPriorityMark(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, SIP_INDEX, SIP, &iEthPriority))
                        {
                            TelcoVoiceMgrDmlSetWanEthernetPriorityMark(SIP, iEthPriority);
                        }
                        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetEthernetPriorityMark(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, RTP_INDEX, RTP, &iEthPriority))
                        {
                            TelcoVoiceMgrDmlSetWanEthernetPriorityMark(RTP, iEthPriority);
                        }

                        if (TelcoVoiceMgrDmlGetWanSKBMarks(PARAM_NAME_IPV4_STATE, &uSipSkbMark,  &uRtpSkbMark)
                                                                            == ANSC_STATUS_SUCCESS)
                        {
                            TelcoVoiceMgrInitMark(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, SIP_INDEX, uSipSkbMark, SIP, PARAM_NAME_SKB_MARK);
                            TelcoVoiceMgrInitMark(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, RTP_INDEX, uRtpSkbMark, RTP, PARAM_NAME_SKB_MARK);
                        }

                        TelcoVoiceMgrDmlSetBoundIpAddress(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, ipAddr);
                        TelcoVoiceMgrDmlSetLinkState(VOICE_HAL_IP_LINK_STATE_UP, ipAddrFamily, ipAddr);
                    }
                    else
                    {
                        CcspTraceError(("Failed to get wan ipv4 address from sysevent \n"));
                    }
                }
            }
            else
            {
                CcspTraceWarning(("Voice Manager: WAN down !\n"));
                CcspTraceNotice(("TELCOVOICEMANAGER_IPV4_WANDOWN :: Voice Manager: IPV4 WAN down\n"));
                printf("Voice Manager: WAN down !\n");
                fflush(stdout);
                TelcoVoiceMgrDmlSetLinkState(VOICE_HAL_IP_LINK_STATE_DOWN, ipAddrFamily, NULL);
            }
        }
    }
    else if (!strcmp(ipAddrFamily, STR_IPV6))
    {
        if (sysevent_get(sysevent_rw_fd, sysevent_token, SYSEVENT_IPV6_CONNECTION_STATE, conState, sizeof(conState)) != 0 )
        {
            CcspTraceError(("Failed to get wan ipv6 connection state sysevent \n"));
        }
        else
        {
            if (!strcmp(conState, "up"))
            {
                if (sysevent_get(sysevent_rw_fd, sysevent_token, SYSEVENT_IPV6_PREFIX, ipAddr, sizeof(ipAddr)) == 0)
                {
                    char *ipv6_addr;
                    CcspTraceInfo(("%s:: IPv6 Prefix Address ----> { %s}\n", __FUNCTION__, ipAddr));
                    ipv6_addr = strchr(ipAddr, '/');
                    if(ipv6_addr != NULL)
                    {
                        strncpy(ipv6_addr, "1",2);
                        CcspTraceInfo(("%s:: WAN IPv6 Address updated! { %s }\n", __FUNCTION__, ipAddr));
                        CcspTraceNotice(("TELCOVOICEMANAGER_IPV6_WANUP :: Voice Manager: IPV6 WAN up\n"));
                        //update the SKBmark reading from Wan Manager

                        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetEthernetPriorityMark(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, SIP_INDEX, SIP, &iEthPriority))
                        {
                            TelcoVoiceMgrDmlSetWanEthernetPriorityMark(SIP, iEthPriority);
                        }
                        if(ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlGetEthernetPriorityMark(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, RTP_INDEX, RTP, &iEthPriority))
                        {
                            TelcoVoiceMgrDmlSetWanEthernetPriorityMark(RTP, iEthPriority);
                        }

                        if (TelcoVoiceMgrDmlGetWanSKBMarks(PARAM_NAME_IPV6_STATE, &uSipSkbMark,  &uRtpSkbMark)
                                                                                 == ANSC_STATUS_SUCCESS){
                            TelcoVoiceMgrInitMark(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, SIP_INDEX, uSipSkbMark, SIP, PARAM_NAME_SKB_MARK);
                            TelcoVoiceMgrInitMark(TELCOVOICEMGR_DML_VOICE_SERVICE_INDEX, RTP_INDEX, uRtpSkbMark, RTP, PARAM_NAME_SKB_MARK);
                        }

                        TelcoVoiceMgrDmlSetBoundIpAddress(1, ipAddr);
                        TelcoVoiceMgrDmlSetLinkState(VOICE_HAL_IP_LINK_STATE_UP, ipAddrFamily, ipAddr);
                    }
                }
                else
                {
                    CcspTraceError(("Failed to get wan ipv6 address from sysevent \n"));
                }
            }
            else
            {
                CcspTraceWarning(("Voice Manager: WAN down !\n"));
                CcspTraceNotice(("TELCOVOICEMANAGER_IPV6_WANDOWN :: Voice Manager: IPV6 WAN down\n"));
                printf("Voice Manager: WAN down !\n");
                fflush(stdout);
                TelcoVoiceMgrDmlSetLinkState(VOICE_HAL_IP_LINK_STATE_DOWN, ipAddrFamily, NULL);
            }
        }
    }
    else
    {
        CcspTraceError(("Invalid IP Address Family name Passed!\n"));
    }
}

/*firewall_restart_for_voice */
/*
* @description : Restart firewall and wait until firewall restart is completed
*
* @return The status of the operation.
* @retval ANSC_STATUS_SUCCESS if successful.
* @retval ANSC_STATUS_FAILURE if any error is detected
*/

int firewall_restart_for_voice(unsigned long timeout_ms)
{
    struct timeval tstart;
    struct timeval ttimeout;
    struct timeval tend;
    struct timeval tnow;

    gettimeofday(&tstart, NULL);
    ttimeout.tv_sec  = (timeout_ms/1000);
    ttimeout.tv_usec = (timeout_ms % 1000) * 1000;
    timeradd (&tstart, &ttimeout, &tend);

    firewall_status = FIREWALLSTATUS_STARTING;
    if (sysevent_set(sysevent_voice_fd, sysevent_voice_token, SYSEVENT_FIREWALL_RESTART, NULL, 0))
    {
        CcspTraceWarning(("%s :: SYSEVENT_FIREWALL_RESTART failed \n", __FUNCTION__));
        return ANSC_STATUS_FAILURE;
    }
    gettimeofday(&tnow, NULL);
    while (timercmp (&tnow, &tend, <))
    {
        if ( firewall_status == FIREWALLSTATUS_STARTED )
        {
            CcspTraceInfo (( "%s %d - firewall restart process finished\n", __FUNCTION__, __LINE__ ));
            return ANSC_STATUS_SUCCESS;
        }
        usleep(10000);
        gettimeofday(&tnow, NULL);
    }
    CcspTraceError (( "%s %d - firewall restart timeout\n", __FUNCTION__, __LINE__ ));
    return ANSC_STATUS_FAILURE;
}
