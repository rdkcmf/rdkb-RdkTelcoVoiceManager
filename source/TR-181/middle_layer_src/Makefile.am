##########################################################################
 # If not stated otherwise in this file or this component's LICENSE file the
 # following copyright and licenses apply:
 #
 # Copyright 2020 Sky
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
###########################################################################

AM_CFLAGS = -D_ANSC_LINUX
AM_CFLAGS += -D_ANSC_USER -DFEATURE_SUPPORT_RDKLOG $(DBUS_CFLAGS) $(SYSTEMD_CFLAGS)
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
AM_LDFLAGS = -lccsp_common lrdkloggers $(DBUS_LIBS) $(SYSTEMD_LDFLAGS)


AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4
hardware_platform = i686-linux-gnu

noinst_LTLIBRARIES = libCcsptelcovoice_manager_middle_layer_src.la
libCcsptelcovoice_manager_middle_layer_src_la_CPPFLAGS = -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/custom -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/include -I$(top_srcdir)/../CcspCommonLibrary/source/debug_api/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include/linux -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/package/slap/include -I$(top_srcdir)/../hal/include -I$(top_srcdir)/source/TR-181/board_sbapi -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/http/include -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/ansc/include -I$(top_srcdir)/source/TR-181/middle_layer_src -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/common/MessageBusHelper/include -I$(top_srcdir)/source/TR-181/include -I$(top_srcdir)/source/TelcoVOICEManager $(CPPFLAGS)

libCcsptelcovoice_manager_middle_layer_src_la_SOURCES = telcovoicemgr_dml_plugin_main.c telcovoicemgr_dml_backendmgr.c telcovoicemgr_dml_apis.c telcovoicemgr_dml_hal.c telcovoicemgr_dml_data.c
if VOICE_V2
libCcsptelcovoice_manager_middle_layer_src_la_SOURCES += telcovoicemgr_dml_data_v2.c telcovoicemgr_dml_v2.c telcovoicemgr_dml_hal_param_v2.c
libCcsptelcovoice_manager_middle_layer_src_la_SOURCES += telcovoicemgr_dml_v2_ClockSynchronization.c telcovoicemgr_dml_v2_Terminal.c telcovoicemgr_dml_v2_Tone.c telcovoicemgr_dml_v2_CodecProfile.c
libCcsptelcovoice_manager_middle_layer_src_la_SOURCES += telcovoicemgr_dml_v2_VoIPProfile.c telcovoicemgr_dml_v2_CallLog.c telcovoicemgr_dml_v2_Interwork.c telcovoicemgr_dml_v2_CallControl.c
libCcsptelcovoice_manager_middle_layer_src_la_SOURCES += telcovoicemgr_dml_v2_Trunk.c telcovoicemgr_dml_v2_H323.c telcovoicemgr_dml_v2_MGCP.c telcovoicemgr_dml_v2_SIP.c
libCcsptelcovoice_manager_middle_layer_src_la_SOURCES += telcovoicemgr_dml_v2_DECT.c  telcovoicemgr_dml_v2_POTS.c telcovoicemgr_dml_v2_ISDN.c telcovoicemgr_dml_v2_ReservedPorts.c
libCcsptelcovoice_manager_middle_layer_src_la_SOURCES += telcovoicemgr_dml_v2_Capabilities.c telcovoicemgr_dml_v2_Debug.c
else
libCcsptelcovoice_manager_middle_layer_src_la_SOURCES += telcovoicemgr_dml_data_v1.c telcovoicemgr_dml_services_v1.c telcovoicemgr_dml_profile_v1.c telcovoicemgr_dml_line_v1.c telcovoicemgr_dml_tone_v1.c telcovoicemgr_dml_numberingplan_v1.c telcovoicemgr_dml_hal_param_v1.c telcovoicemgr_dml_capabilities_v1.c telcovoicemgr_dml_physical_interface_v1.c 
endif
libCcsptelcovoice_manager_middle_layer_src_la_LDFLAGS =  -lccsp_common -ljson_hal_client -ljson-c -lcjson


