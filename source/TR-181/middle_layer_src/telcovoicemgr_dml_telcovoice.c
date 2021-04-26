/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
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

#include "ansc_platform.h"
#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_plugin_main.h"
#include "telcovoicemgr_dml_telcovoice.h"
#include "telcovoicemgr_dml_apis.h"
#include "ccsp_trace.h"

/***********************************************************************/
/***********************************************************************

 APIs for Object:

    X_RDK_TelcoVoice.

    *  X_RDK_TelcoVoice_GetParamStringValue
    *  X_RDK_TelcoVoice_SetParamStringValue
    *  X_RDK_TelcoVoice_Validate
    *  X_RDK_TelcoVoice_Commit
    *  X_RDK_TelcoVoice_Rollback

***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_RDK_TelcoVoice_GetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pValue,
                ULONG*                      pUlSize
            );

    description:

        This function is called to retrieve String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer of returned ULONG value;

                ULONG*                      pUlSize
                The length of returned buffer value;

    return:     0 if succeeded.
                1 if not enough buffer size
                -1 if not supported

**********************************************************************/
LONG
X_RDK_TelcoVoice_GetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pValue,
        ULONG*                      pUlSize
    )
{
    PTELCOVOICEMGR_DATAMODEL_TELCOVOICE       pMyObject     = (PTELCOVOICEMGR_DATAMODEL_TELCOVOICE)g_pTelcoVoiceBEManager->hTelcoVoice;

    if( AnscEqualString(ParamName, "Data", TRUE) )
    {
        /* Data value should be empty for all get */
        snprintf(pValue, pUlSize, "%s", "");
        return 0;
    }
 
    return -1;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        X_RDK_TelcoVoice_SetParamStringValue
            (
                ANSC_HANDLE                 hInsContext,
                char*                       ParamName,
                char*                       pString
            );

    description:

        This function is called to set String parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The buffer to set value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
X_RDK_TelcoVoice_SetParamStringValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        char*                       pString
    )
{
    PTELCOVOICEMGR_DATAMODEL_TELCOVOICE       pMyObject     = (PTELCOVOICEMGR_DATAMODEL_TELCOVOICE)g_pTelcoVoiceBEManager->hTelcoVoice;

    if( AnscEqualString(ParamName, "Data", TRUE) )
    {
        char *webConf = NULL;
        int webSize = 0;

        webConf = AnscBase64Decode(pString, &webSize);

        //Validate pointer
        if( NULL == webConf )
        {
            CcspTraceInfo(("%s Failed to decode telcovoice webconfig blob\n",__FUNCTION__));
            return FALSE;
        }

        CcspTraceWarning(("%s Decoded file %s of size %d\n",__FUNCTION__,webConf,webSize));

        if ( ANSC_STATUS_SUCCESS == TelcoVoiceMgrDmlTelcoVoiceDataSet(webConf,webSize) )
        {
            CcspTraceInfo(("%s Success in parsing telcovoice web config blob\n",__FUNCTION__));
            AnscFreeMemory(webConf);
            return TRUE;
        }
        else 
        {
            CcspTraceWarning(("%s Failed to parse telcovoice webconfig blob\n",__FUNCTION__));
            AnscFreeMemory(webConf);
            return FALSE;
        }
    }

    return FALSE;
}

BOOL 
X_RDK_TelcoVoice_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    /* Stub */
    return TRUE;
}

ULONG
X_RDK_TelcoVoice_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    /* Stub */
    return 0;
}

ULONG
X_RDK_TelcoVoice_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    /* Stub */
    return 0;
}
