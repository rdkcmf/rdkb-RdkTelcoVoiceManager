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


#ifndef  _TELCOVOICEMGR_CONTROLLER_H
#define  _TELCOVOICEMGR_CONTROLLER_H

#include "ansc_platform.h"
#include "ansc_string_util.h"
#include "telcovoicemgr_dml_json_cfg_init.h"
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_services_apis_v2.h"
#else
#include "telcovoicemgr_services_apis_v1.h"
#endif

ANSC_STATUS TelcoVoiceMgr_Controller_Init();


#endif  //_TELCOVOICEMGR_CONTROLLER_H
