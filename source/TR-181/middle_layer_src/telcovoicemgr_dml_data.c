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

#include <pthread.h>
#include <sysevent.h>
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_services_apis_v2.h"
#else
#include "telcovoicemgr_services_apis_v1.h"
#endif

int sysevent_voice_fd = -1;
token_t sysevent_voice_token = -1;

/******** TELCOVOICEMGR DML DATABASE ********/
TELCOVOICEMGR_DML_DATA* pTelcoVoiceMgrDmlData = NULL;


/******** FUNCTIONS ********/
TELCOVOICEMGR_DML_DATA* TelcoVoiceMgrDmlGetDataLocked(void)
{
    TELCOVOICEMGR_DML_DATA* pDmlData = TelcoVoiceMgrDmlGetData();

    if(pDmlData != NULL)
    {
        if(pthread_mutex_lock(&(pDmlData->mDataMutex)) == 0)
        {
            return pDmlData;
        }
    }

    return NULL;
}

void TelcoVoiceMgrDmlGetDataRelease(TELCOVOICEMGR_DML_DATA* pDmlData)
{
    if(pDmlData != NULL)
    {
        pthread_mutex_unlock (&(pDmlData->mDataMutex));
    }
}


TELCOVOICEMGR_DML_DATA* TelcoVoiceMgrDmlGetData(void)
{
    if(pTelcoVoiceMgrDmlData == NULL)
    {
        //create instance
        pTelcoVoiceMgrDmlData = (TELCOVOICEMGR_DML_DATA*) AnscAllocateMemory(sizeof(TELCOVOICEMGR_DML_DATA));
    }
    return pTelcoVoiceMgrDmlData;
}

int TelcoVoiceMgrSyseventInit()
{
    sysevent_voice_fd =  sysevent_open("127.0.0.1", SE_SERVER_WELL_KNOWN_PORT, SE_VERSION, "voip_if", &sysevent_voice_token);
    if (sysevent_voice_fd < 0)
        return -1;
    return 0;
}

void TelcoVoiceMgrSyseventDeInit()
{
    if (0 <= sysevent_voice_fd)
    {
        sysevent_close(sysevent_voice_fd, sysevent_voice_token);
    }
}

ANSC_STATUS TelcoVoiceMgrDmlDataInit(void)
{
    ANSC_STATUS              result  = ANSC_STATUS_FAILURE;
    TELCOVOICEMGR_DML_DATA*  pTelcoVoiceMgrData = NULL;
    pthread_mutexattr_t      muttex_attr;

    pTelcoVoiceMgrData = TelcoVoiceMgrDmlGetData();
    if(pTelcoVoiceMgrData != NULL)
    {
        //default
        TelcoVoiceMgrDmlSetDefaultData(pTelcoVoiceMgrData);

        //Initialise mutex
        pthread_mutexattr_init(&muttex_attr);
        pthread_mutexattr_settype(&muttex_attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&(pTelcoVoiceMgrData->mDataMutex), &(muttex_attr));

        result = ANSC_STATUS_SUCCESS;
    }

    return result;
}