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

#ifndef _TELCOVOICEMGR_DML_HAL_PARAM_V2_H 
#define _TELCOVOICEMGR_DML_HAL_PARAM_V2_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_hal_common.h"
#include "json_hal_client.h"
#include "telcovoicemgr_services_apis_v2.h"
#define MAX_STR_LEN 256

#define DML_VOICESERVICE_PARAM_NAME                                   "Device.Services.VoiceService.%d."
#define DML_VOICESERVICE_DEBUG_PARAM_NAME                             DML_VOICESERVICE_PARAM_NAME"X_RDK_Debug."
#define DML_VOICESERVICE_CAPABILITIES_CODEC_PARAM_NAME                DML_VOICESERVICE_PARAM_NAME"Capabilities.Codec.%d."
#define DML_VOICESERVICE_ISDN_BRI_PARAM_NAME                          DML_VOICESERVICE_PARAM_NAME"ISDN.BRI.%d."
#define DML_VOICESERVICE_ISDN_PRI_PARAM_NAME                          DML_VOICESERVICE_PARAM_NAME"ISDN.PRI.%d."
#define DML_VOICESERVICE_POTS_FXO_PARAM_NAME                          DML_VOICESERVICE_PARAM_NAME"POTS.FXO.%d."
#define DML_VOICESERVICE_POTS_FXS_PARAM_NAME                          DML_VOICESERVICE_PARAM_NAME"POTS.FXS.%d."
#define DML_VOICESERVICE_POTS_RINGER_EVT_PARAM_NAME                   DML_VOICESERVICE_PARAM_NAME"POTS.Ringer.Event.%d."
#define DML_VOICESERVICE_DECT_BASE_PARAM_NAME                         DML_VOICESERVICE_PARAM_NAME"DECT.Base.%d."
#define DML_VOICESERVICE_DECT_PORTABLE_PARAM_NAME                     DML_VOICESERVICE_PARAM_NAME"DECT.Portable.%d."
#define DML_VOICESERVICE_SIP_CLIENT_PARAM_NAME                        DML_VOICESERVICE_PARAM_NAME"SIP.Client.%d."
#define DML_VOICESERVICE_SIP_CLIENT_CONTACT_PARAM_NAME                DML_VOICESERVICE_PARAM_NAME"SIP.Client.%d.Contact.%d."
#define DML_VOICESERVICE_SIP_CLIENT_EVTSUBSCRIBE_PARAM_NAME           DML_VOICESERVICE_PARAM_NAME"SIP.Client.%d.EventSubscribe.%d."
#define DML_VOICESERVICE_SIP_NETWORK_PARAM_NAME                       DML_VOICESERVICE_PARAM_NAME"SIP.Network.%d."
#define DML_VOICESERVICE_SIP_NETWORK_FQDNSERVER_PARAM_NAME            DML_VOICESERVICE_PARAM_NAME"SIP.Network.%d.FQDNServer.%d."
#define DML_VOICESERVICE_SIP_NETWORK_EVTSUBSCRIBE_PARAM_NAME          DML_VOICESERVICE_PARAM_NAME"SIP.Network.%d.EventSubscribe.%d."
#define DML_VOICESERVICE_SIP_NETWORK_RESPMAP_PARAM_NAME               DML_VOICESERVICE_PARAM_NAME"SIP.Network.%d.ResponseMap.%d."
#define DML_VOICESERVICE_SIP_PROXY_PARAM_NAME                         DML_VOICESERVICE_PARAM_NAME"SIP.Proxy.%d."
#define DML_VOICESERVICE_SIP_REGISTRAR_PARAM_NAME                     DML_VOICESERVICE_PARAM_NAME"SIP.Registrar.%d."
#define DML_VOICESERVICE_SIP_REGISTRAR_ACCOUNT_PARAM_NAME             DML_VOICESERVICE_PARAM_NAME"SIP.Registrar.%d.Account.%d."
#define DML_VOICESERVICE_SIP_REGISTRAR_ACCOUNT_CONTACT_PARAM_NAME     DML_VOICESERVICE_PARAM_NAME"SIP.Registrar.%d.Account.%d.Contact.%d."
#define DML_VOICESERVICE_MGCP_CLIENT_PARAM_NAME                       DML_VOICESERVICE_PARAM_NAME"MGCP.Client.%d."
#define DML_VOICESERVICE_MGCP_NETWORK_PARAM_NAME                      DML_VOICESERVICE_PARAM_NAME"MGCP.Network.%d."
#define DML_VOICESERVICE_H323_CLIENT_PARAM_NAME                       DML_VOICESERVICE_PARAM_NAME"H323.Client.%d."
#define DML_VOICESERVICE_H323_NETWORK_PARAM_NAME                      DML_VOICESERVICE_PARAM_NAME"H323.Network.%d."
#define DML_VOICESERVICE_TRUNK_PARAM_NAME                             DML_VOICESERVICE_PARAM_NAME"Trunk.%d."
#define DML_VOICESERVICE_CALLCONTROL_LINE_PARAM_NAME                  DML_VOICESERVICE_PARAM_NAME"CallControl.Line.%d."
#define DML_VOICESERVICE_CALLCONTROL_EXTENSION_PARAM_NAME             DML_VOICESERVICE_PARAM_NAME"CallControl.Extension.%d."
#define DML_VOICESERVICE_CALLCONTROL_GROUP_PARAM_NAME                 DML_VOICESERVICE_PARAM_NAME"CallControl.Group.%d."
#define DML_VOICESERVICE_CALLCONTROL_MAILBOX_PARAM_NAME               DML_VOICESERVICE_PARAM_NAME"CallControl.Mailbox.%d."
#define DML_VOICESERVICE_CALLCONTROL_INCOMMINGMAP_PARAM_NAME          DML_VOICESERVICE_PARAM_NAME"CallControl.IncomingMap.%d."
#define DML_VOICESERVICE_CALLCONTROL_OUTGOINGMAP_PARAM_NAME           DML_VOICESERVICE_PARAM_NAME"CallControl.OutgoingMap.%d."
#define DML_VOICESERVICE_CALLCONTROL_NUMPLAN_PARAM_NAME               DML_VOICESERVICE_PARAM_NAME"CallControl.NumberingPlan.%d."
#define DML_VOICESERVICE_CALLCONTROL_NUMPLAN_PREFIXINFO_PARAM_NAME    DML_VOICESERVICE_PARAM_NAME"CallControl.NumberingPlan.%d.PrefixInfo.%d."
#define DML_VOICESERVICE_CALLCONTROL_SET_PARAM_NAME                   DML_VOICESERVICE_PARAM_NAME"CallControl.CallingFeatures.Set.%d."
#define DML_VOICESERVICE_CALLCONTROL_SET_CFT_PARAM_NAME               DML_VOICESERVICE_PARAM_NAME"CallControl.CallingFeatures.Set.%d.CFT.%d."
#define DML_VOICESERVICE_CALLCONTROL_SET_SCF_PARAM_NAME               DML_VOICESERVICE_PARAM_NAME"CallControl.CallingFeatures.Set.%d.SCF.%d."
#define DML_VOICESERVICE_CALLCONTROL_SET_SCREJ_PARAM_NAME             DML_VOICESERVICE_PARAM_NAME"CallControl.CallingFeatures.Set.%d.SCREJ.%d."
#define DML_VOICESERVICE_CALLCONTROL_SET_FOLLOWME_PARAM_NAME          DML_VOICESERVICE_PARAM_NAME"CallControl.CallingFeatures.Set.%d.FollowMe.%d."
#define DML_VOICESERVICE_INTERWORK_PARAM_NAME                         DML_VOICESERVICE_PARAM_NAME"Interwork.%d."
#define DML_VOICESERVICE_INTERWORK_USERINTERFACE_PARAM_NAME           DML_VOICESERVICE_PARAM_NAME"Interwork.%d.UserInterface.%d."
#define DML_VOICESERVICE_INTERWORK_MAP_PARAM_NAME                     DML_VOICESERVICE_PARAM_NAME"Interwork.%d.Map.%d."
#define DML_VOICESERVICE_CALLLOG_PARAM_NAME                           DML_VOICESERVICE_PARAM_NAME"CallLog.%d."
#define DML_VOICESERVICE_CALLLOG_SIGNALINGPERFORMANCE_PARAM_NAME      DML_VOICESERVICE_PARAM_NAME"CallLog.%d.SignalingPerformance.%d."
#define DML_VOICESERVICE_CALLLOG_SESSION_PARAM_NAME                   DML_VOICESERVICE_PARAM_NAME"CallLog.%d.Session.%d."
#define DML_VOICESERVICE_CALLLOG_SESSION_SOURCE_RTP_PARAM_NAME        DML_VOICESERVICE_PARAM_NAME"CallLog.%d.Session.%d.Source.RTP."
#define DML_VOICESERVICE_VOIPPROF_PARAM_NAME                          DML_VOICESERVICE_PARAM_NAME"VoIPProfile.%d."
#define DML_VOICESERVICE_CODECPROF_PARAM_NAME                         DML_VOICESERVICE_PARAM_NAME"CodecProfile.%d."
#define DML_VOICESERVICE_TONE_DESC_PARAM_NAME                         DML_VOICESERVICE_PARAM_NAME"Tone.Description.%d."
#define DML_VOICESERVICE_TONE_PATTERN_PARAM_NAME                      DML_VOICESERVICE_PARAM_NAME"Tone.Pattern.%d."
#define DML_VOICESERVICE_TONE_EVTPROF_PARAM_NAME                      DML_VOICESERVICE_PARAM_NAME"Tone.EventProfile.%d."
#define DML_VOICESERVICE_TONE_EVTPROF_EVT_PARAM_NAME                  DML_VOICESERVICE_PARAM_NAME"Tone.EventProfile.%d.Event.%d."
#define DML_VOICESERVICE_TERMINAL_PARAM_NAME                          DML_VOICESERVICE_PARAM_NAME"Terminal.%d."
#define DML_VOICESERVICE_TERMINAL_AUDIO_PARAM_NAME                    DML_VOICESERVICE_PARAM_NAME"Terminal.%d.Audio.%d."
#define DML_VOICESERVICE_TERMINAL_BUTTONMAP_BUTTON_PARAM_NAME         DML_VOICESERVICE_PARAM_NAME"Terminal.%d.ButtonMap.Button.%d."
#define DML_VOICESERVICE_TERMINAL_RINGER_DESC_PARAM_NAME              DML_VOICESERVICE_PARAM_NAME"Terminal.%d.Ringer.Description.%d."
#define DML_VOICESERVICE_TERMINAL_RINGER_PATTERN_PARAM_NAME           DML_VOICESERVICE_PARAM_NAME"Terminal.%d.Ringer.Pattern.%d."
#define DML_VOICESERVICE_CLKSRC_PARAM_NAME                            DML_VOICESERVICE_PARAM_NAME"ClockSynchronization.ClockSource.%d."
#define DML_VOICESERVICE_VOICE_PROCESSING_PARAM_NAME                  DML_VOICESERVICE_TERMINAL_AUDIO_PARAM_NAME"VoiceProcessing."

#define telcovoicemgr_hal_get_VoiceService_index                                                     telcovoicemgr_hal_get_first_index
#define telcovoicemgr_hal_get_VoiceService_Capabilities_Codec_index                                  telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_ISDN_BRI_index                                            telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_ISDN_PRI_index                                            telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_POTS_FXO_index                                            telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_POTS_FXS_index                                            telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_POTS_Ringer_Event_index                                   telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_DECT_Base_index                                           telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_DECT_Portable_index                                       telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Client_index                                          telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Client_Contact_index                                  telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Client_EventSubscribe_index                           telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Network_index                                         telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Network_FQDNServer_index                              telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Network_EventSubscribe_index                          telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Network_ResponseMap_index                             telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Proxy_index                                           telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Registrar_index                                       telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Registrar_Account_index                               telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_SIP_Registrar_Account_Contact_index                       telcovoicemgr_hal_get_fourth_index
#define telcovoicemgr_hal_get_VoiceService_MGCP_Client_index                                         telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_MGCP_Network_index                                        telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_H323_Client_index                                         telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_H323_Network_index                                        telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_Trunk_index                                               telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_Line_index                                    telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_Extension_index                               telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_Group_index                                   telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_Mailbox_index                                 telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_IncomingMap_index                             telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_OutgoingMap_index                             telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_NumberingPlan_index                           telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_NumberingPlan_PrefixInfo_index                telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_index                     telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_CFT_index                 telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_SCF_index                 telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_SCREJ_index               telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_CallControl_CallingFeatures_Set_FollowMe_index            telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_Interwork_index                                           telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_Interwork_UserInterface_index                             telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_Interwork_Map_index                                       telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_CallLog_index                                             telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CallLog_SignalingPerformance_index                        telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_CallLog_Session_index                                     telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_VoIPProfile_index                                         telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_CodecProfile_index                                        telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_Tone_Description_index                                    telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_Tone_Pattern_index                                        telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_Tone_EventProfile_index                                   telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_Tone_EventProfile_Event_index                             telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_Terminal_index                                            telcovoicemgr_hal_get_second_index
#define telcovoicemgr_hal_get_VoiceService_Terminal_Audio_index                                      telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_Terminal_ButtonMap_Button_index                           telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_Terminal_Ringer_Description_index                         telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_Terminal_Ringer_Pattern_index                             telcovoicemgr_hal_get_third_index
#define telcovoicemgr_hal_get_VoiceService_ClockSynchronization_ClockSource_index                    telcovoicemgr_hal_get_second_index

ANSC_STATUS Map_hal_dml_capabilities(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_reservePorts(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_ISDN(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_POTS(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_DECT(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_SIP(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_MGCP(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_H323(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_Trunk(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_CallControl(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_Interwork(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_CallLog(PTELCOVOICEMGR_DML_VOICESERVICE pVoiceService, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_VoipProfile(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_codecProfile(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_Tone(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_Terminal(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
ANSC_STATUS Map_hal_dml_clockSync(PDML_VOICE_SERVICE_LIST_T pVoiceServiceList, char* ParamName, char* pValue);
#endif //_TELCOVOICEMGR_DML_HAL_PARAM_H
