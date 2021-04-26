
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
##########################################################################

bin_PROGRAMS = telcovoice_manager

AM_CPPFLAGS = -I$(top_srcdir)/hal/include \
                -I$(top_srcdir)/source/LogComponent \
                -I=${includedir} \
                -I=${includedir}/ccsp

AM_LDFLAGS = -lwebconfig_framework

AM_CXXFLAGS = -I$(top_srcdir)/breakpad_wrapper_telco.h

telcovoice_manager_DEPENDENCIES = \
        $(EXTRA_DEPENDENCIES) \
	${top_builddir}/source/TR-181/middle_layer_src/libCcsptelcovoice_manager_middle_layer_src.la \
	${top_builddir}/source/TR-181/integration_src.shared/libCcsptelcovoice_manager_integration_src_shared.la

telcovoice_manager_CFLAGS = -D_ANSC_LINUX -D_ANSC_USER -D_ANSC_LITTLE_ENDIAN_ -DFEATURE_SUPPORT_RDKLOG $(DBUS_CFLAGS) $(SYSTEMD_CFLAGS) -DVOICE_IPV6_SUPPORT
telcovoice_manager_CPPFLAGS = -I$(top_srcdir)/source/TR-181/middle_layer_src -I$(top_srcdir)/source/TR-181/include $(CPPFLAGS)
telcovoice_manager_LDFLAGS = -lccsp_common -lrdkloggers -lsyscfg -lsysevent $(DBUS_LIBS) $(SYSTEMD_LDFLAGS) -lwebconfig_framework

telcovoice_manager_SOURCES = telcovoicemgr_ssp_action.c telcovoicemgr_ssp_messagebus_interface.c telcovoicemgr_ssp_main.c telcovoicemgr_breakpad_wrapper.cpp telcovoicemgr_controller.c telcovoicemgr_nw_monitor.c

telcovoice_manager_LDADD =  $(telcovoice_manager_DEPENDENCIES)


