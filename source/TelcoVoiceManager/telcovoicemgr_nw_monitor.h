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


/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

/*!********************************************************************
 *
 * FILE: telcovoicemgr_nw_monitor.h
 *
 *
 * MODULE: Telco Voice Manager
 *
 * Date Created: 21/05/2019
 *
 * Description: Voice Manager public interface
 *
 * Notes:
 *
 *********************************************************************/
#ifndef _TELCOVOICEMGR_NW_MONITOR_H
#define _TELCOVOICEMGR_NW_MONITOR_H

/* ---- Include Files ---------------------------------------- */
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_services_apis_v2.h"
#else
#include "telcovoicemgr_services_apis_v1.h"
#endif

/* ---- Global Constants -------------------------- */

/* ---- Global Types -------------------------- */

/* ---- Global Variables -------------------------- */

/* ---- Global Prototypes -------------------------- */
int  voicemgr_sysevent_init(void);
void voicemgr_sysevent_close(void);
void voicemgr_create_nw_monitor(void);
#endif // _TELCOVOICEMGR_NW_MONITOR_H
