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

#ifndef  _TELCOVOICEMGR_DML_APIS_H
#define  _TELCOVOICEMGR_DML_APIS_H


#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_services_apis_v2.h"
#else
#include "telcovoicemgr_services_apis_v1.h"
#endif

#include "telcovoicemgr_telcovoice_apis.h"

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
ANSC_HANDLE TelcoVoiceMgr_setCallControlLineEnable(BOOL bEnable);
#endif

typedef ANSC_HANDLE(*PFN_COSADM_CREATE)(VOID);

typedef ANSC_STATUS(*PFN_COSADM_REMOVE)(ANSC_HANDLE hThisObject);

typedef ANSC_STATUS(*PFN_COSADM_INITIALIZE)(ANSC_HANDLE hThisObject);

typedef  struct _X_RDK_REPORT_VOICE_SERVICE_DEFAULT
{
    UINT                        ReportingPeriod;
    UINT                        OverrideTTL;
}
DML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT, *PDML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT;

typedef  struct _DML_X_RDK_REPORT_VOICE_SERVICE
{
    BOOL                                      Enabled;
    UINT                                      ReportingPeriod;
    BOOL                                      bEnableChanged;
    BOOL                                      bReportingPeriodChanged;
    PDML_X_RDK_REPORT_VOICE_SERVICE_DEFAULT   pVoiceServiceDefaultReport;
}
DML_X_RDK_REPORT_VOICE_SERVICE, *PDML_X_RDK_REPORT_VOICE_SERVICE;

/*
 * the main struct in cosa_xxx_apis.h need includes this struct and realize all functions. 
 */
#define  TELCOVOICEMGR_BASE_CONTENT                                                         \
    /* start of object class content */                                                     \
    ULONG                           Oid;                                                    \
    ANSC_HANDLE                     hSbContext;                                             \
                                                                                            \
    PFN_COSADM_CREATE               Create;                                                 \
    PFN_COSADM_REMOVE               Remove;                                                 \
    PFN_COSADM_INITIALIZE           Initialize;                                             \

typedef  struct
_TELCOVOICEMGR_BASE_OBJECT
{
    TELCOVOICEMGR_BASE_CONTENT
}
TELCOVOICEMGR_BASE_OBJECT,  *PTELCOVOICEMGR_BASE_OBJECT;

/*
*  This struct is for creating entry context link in writable table when call GetEntry()
*/
#define  TELCOVOICEMGR_CONTEXT_LINK_CLASS_CONTENT                                           \
         SINGLE_LINK_ENTRY                Linkage;                                          \
         ANSC_HANDLE                      hContext;                                         \
         ANSC_HANDLE                      hParentTable;  /* Back pointer */                 \
         ULONG                            InstanceNumber;                                   \
         BOOL                             bNew;                                             \
         ANSC_HANDLE                      hPoamIrepUpperFo;                                 \
         ANSC_HANDLE                      hPoamIrepFo;                                      \
         PDML_X_RDK_REPORT_VOICE_SERVICE  pVoiceServiceReport;                              \

typedef  struct
_TELCOVOICE_CONTEXT_LINK_OBJECT
{
    TELCOVOICEMGR_CONTEXT_LINK_CLASS_CONTENT
}
TELCOVOICE_CONTEXT_LINK_OBJECT,  *PTELCOVOICE_CONTEXT_LINK_OBJECT;

#define  ACCESS_TELCOVOICE_CONTEXT_LINK_OBJECT(p)              \
         ACCESS_CONTAINER(p, TELCOVOICE_CONTEXT_LINK_OBJECT, Linkage)

#define COSA_CONTEXT_LINK_INITIATION_CONTENT(cxt)                                      \
    (cxt)->hContext            = (ANSC_HANDLE)NULL;                                    \
    (cxt)->hParentTable        = (ANSC_HANDLE)NULL;                                    \
    (cxt)->InstanceNumber      = 0;                                                    \
    (cxt)->bNew                = FALSE;                                                \
    (cxt)->hPoamIrepUpperFo    = (ANSC_HANDLE)NULL;                                    \
    (cxt)->hPoamIrepFo         = (ANSC_HANDLE)NULL;                                    \


#define  COSA_DATAMODEL_SERVICES_CLASS_CONTENT                                         \
    /* duplication of the base object class content */                                 \
    TELCOVOICEMGR_BASE_CONTENT                                                         \
    TELCOVOICEMGR_DML_DATA*   data;                                                    \


typedef  struct
_TELCOVOICEMGR_DATAMODEL_SERVICES
{
    COSA_DATAMODEL_SERVICES_CLASS_CONTENT    
}
TELCOVOICEMGR_DATAMODEL_SERVICES,  *PTELCOVOICEMGR_DATAMODEL_SERVICES;

#define  COSA_DATAMODEL_TELCOVOICE_CLASS_CONTENT                                              \
    /* duplication of the base object class content */                                      \
    TELCOVOICEMGR_BASE_CONTENT                                                              \

typedef  struct
_COSA_DATAMODEL_TELCOVOICE
{
	COSA_DATAMODEL_TELCOVOICE_CLASS_CONTENT
}
TELCOVOICEMGR_DATAMODEL_TELCOVOICE,  *PTELCOVOICEMGR_DATAMODEL_TELCOVOICE;

/**********************************
  Standard function declaration
 ***********************************/
ANSC_HANDLE TelcoVoiceMgrServicesCreate(VOID);
ANSC_STATUS TelcoVoiceMgrServicesInitialize(ANSC_HANDLE hThisObject);
ANSC_STATUS TelcoVoiceMgrServicesRemove(ANSC_HANDLE hThisObject);

ANSC_HANDLE TelcoVoiceMgrTelcoVoiceCreate(VOID);
ANSC_STATUS TelcoVoiceMgrTelcoVoiceInitialize(ANSC_HANDLE hThisObject);
ANSC_STATUS TelcoVoiceMgrTelcoVoiceRemove(ANSC_HANDLE hThisObject);

ANSC_STATUS DmlVoiceServiceReportInit(PANSC_HANDLE phContext);

ANSC_STATUS CosaSListPushEntryByInsNum(PSLIST_HEADER pListHead, PTELCOVOICE_CONTEXT_LINK_OBJECT pCosaContext);
PTELCOVOICE_CONTEXT_LINK_OBJECT CosaSListGetEntryByInsNum(PSLIST_HEADER pListHead, ULONG InstanceNumber);

BOOL TelcoVoiceMgrAnscValidateInputString(char *pString);

#endif
