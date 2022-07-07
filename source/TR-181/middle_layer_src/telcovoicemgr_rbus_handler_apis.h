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

/*
 * Copyright [2014] [Cisco Systems, Inc.]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     LICENSE-2.0" target="_blank" rel="nofollow">http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _TELCOVOICEMGR_RBUS_H_
#define _TELCOVOICEMGR_RBUS_H_
#if defined(RBUS_BUILD_FLAG_ENABLE) || defined(_HUB4_PRODUCT_REQ_)
#include "ansc_platform.h"

#define WANMGR_CONFIG_WAN_INTERFACEAVAILABLESTATUS   "Device.X_RDK_WanManager.InterfaceAvailableStatus"
#define WANMGR_CONFIG_WAN_INTERFACEACTIVESTATUS      "Device.X_RDK_WanManager.InterfaceActiveStatus"
#define REMOTE_LTE_ENABLED                           "REMOTE_LTE,1"
#define  ARRAY_SZ(x) (sizeof(x) / sizeof((x)[0]))

ANSC_STATUS TelcoVoiceMgr_Rbus_Init();
ANSC_STATUS TelcoVoiceMgr_Rbus_Exit();
void TelcoVoiceMgr_Rbus_SubscribeDML(void);
void TelcoVoiceMgr_Rbus_UnSubscribeDML(void);
#endif // RBUS_BUILD_FLAG_ENABLE _HUB4_PRODUCT_REQ_

#ifdef _HUB4_PRODUCT_REQ_
BOOL TelcoVoiceMgr_Rbus_discover_components(char const *pModuleList);
#endif //_HUB4_PRODUCT_REQ_
#endif
