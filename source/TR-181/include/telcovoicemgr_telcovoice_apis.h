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


#ifndef  _TELCOVOICEMGR_TELCOVOICE_APIS_H
#define  _TELCOVOICEMGR_TELCOVOICE_APIS_H

/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>
#include "ansc_platform.h"
#include "ansc_string_util.h"

 /***********************************
     Actual definition declaration
 ************************************/
 /*
     TelcoVoice Part
 */

ANSC_STATUS TelcoVoiceMgrDmlTelcoVoiceDataSet(const void *pData, size_t len);
ANSC_STATUS TelcoVoiceMgrDmlTelcoVoiceWebConfigInit( void );
unsigned int getTelcoVoiceDataBlobVersion(char* pSubDoc);
int setTelcoVoiceDataBlobVersion(char *pSubDoc, unsigned int version);
int TelcoVoiceMgr_TelcoVoiceData_Rollback_Handler(void);
size_t TelcoVoiceMgr_TelcoVoiceData_Timeout_Handler(size_t numOfEntries);
void TelcoVoiceMgr_TelcoVoiceData_Free_Resources(void *arg);

#endif /* _TELCOVOICEMGR_TELCOVOICE_APIS_H */
