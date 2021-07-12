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
AM_CFLAGS += -D_ANSC_USER
AM_CFLAGS += -D_ANSC_LITTLE_ENDIAN_
AM_LDFLAGS = -lccsp_common
AM_LDFLAGS += -lwebconfig_framework

AM_CPPFLAGS = -Wall -Werror
ACLOCAL_AMFLAGS = -I m4


noinst_LTLIBRARIES = libCcsptelcovoice_manager_integration_src_shared.la

libCcsptelcovoice_manager_integration_src_shared_la_CPPFLAGS  = -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/custom -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/include -I$(top_srcdir)/../CcspCommonLibrary/source/debug_api/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/include/linux -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/include -I$(top_srcdir)/../CcspCommonLibrary/source/cosa/package/slap/include -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/http/include -I$(top_srcdir)/../CcspCommonLibrary/source/util_api/ansc/include -I$(top_srcdir)/source/TR-181/middle_layer_src -I$(top_srcdir)/../CcspCommonLibrary/source/ccsp/components/common/MessageBusHelper/include -I$(top_srcdir)/source/TR-181/include -I$(top_srcdir)/source/Custom -I$(top_srcdir)/source/TelcoVoiceManager $(CPPFLAGS)

libCcsptelcovoice_manager_integration_src_shared_la_SOURCES =  telcovoicemgr_webconfig.c telcovoicemgr_telcovoice_apis.c telcovoicemgr_services_apis.c telcovoicemgr_dml_json_cfg_init.c telcovoicemgr_dml_json_save.c voice_report.c voice_report_webpa_intf.c
libCcsptelcovoice_manager_integration_src_shared_la_LDFLAGS = -lccsp_common -lsysevent -lutapi -lutctx -ljson-c -lcjson -lhal_platform -lwebconfig_framework -lmsgpackc -lavro -luuid -ltrower-base64 -llibparodus
