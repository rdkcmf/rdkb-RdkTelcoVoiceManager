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
COPYRIGHT (C) 1986 Gary S. Brown.  You may use this program, or
code or tables extracted from it, as desired without restriction.
*/

/*  To store data in the JSON file.
 */
#include "cJSON.h"
#include <stdint.h>
#include <string.h>
#include <rdk_error.h>
#include <rdk_debug.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include "ccsp_trace.h"
#include "telcovoicemgr_dml_json_cfg_init.h"

#include <sys/stat.h>
#include <fcntl.h>


#define CRC32_INIT_VALUE 0xffffffffu /* Initial CRC32 checksum value */

static unsigned int Crc32_table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

// Enable this line in order to get JSON debug output on console
// Useful when debugging using dmcli commands.
// #define VOICE_HAL_JSON_DEBUG

/* This table is used to save current values in the JSON config file.
 * It makes a connection between the following:
 * nickName: the short name used by Set_Xxxx() to identify the item to be saved
 * type: the type of the item in the *JSON file*
 * fullName: the TR104-style name of the data model object to be stored
 *           [vs] [pi] [li] [vp] identify indexing of arrays (always 1 for now) 
 */
int saveCurrentFile(cJSON *obj);

struct jsonTable
{
    char *nickName;
    char *type;
    char *fullName;
} objs[] =
{
    { "Set-BoundIfName",         "string",     "Services.VoiceService.[vs].X_RDK_BoundIfName" }, 
    { "Set-IpAddressFamily",     "string",     "Services.VoiceService.[vs].X_RDK_IpAddressFamily" }, 
    { "DiagnosticsState",       "string",     "Services.VoiceService.[vs].PhyInterface.[pi].Tests.TestState" }, 
    { "TestResult",             "string",     "Services.VoiceService.[vs].PhyInterface.[pi].Tests.X_RDK_TestResult" }, 
    { "TestSelector",             "string",     "Services.VoiceService.[vs].PhyInterface.[pi].Tests.TestSelector" }, 
    { "CallingFeaturesCWI",        "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].CallingFeatures.CallWaitingEnable" }, 
    { "CallingFeaturesMWI",        "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].CallingFeatures.MWIEnable" }, 
    { "CallingFeaturesCCE",        "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].CallingFeatures.X_RDK-Central_COM_ConferenceCallingEnable" }, 
    { "CallingFeaturesHE",        "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].CallingFeatures.X_RDK-Central_COM_HoldEnable" }, 
    { "CallingFeaturesCID",        "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].CallingFeatures.X_RDK-Central_COM_PhoneCallerIDEnable" }, 
    { "DirectoryNumber",        "string­",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].DirectoryNumber" }, 
    { "Enable",                 "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].Enable" }, 
    { "AuthPassword",           "string­",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].SIP.AuthPassword" }, 
    { "AuthUserName",             "string­",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].SIP.AuthUserName" }, 
    { "URI",                     "string­",     "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].SIP.URI" },
    { "RxGain",                 "int",         "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].VoiceProcessing.ReceiveGain" }, 
    { "TxGain",                 "int",         "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].VoiceProcessing.TransmitGain" },  
    { "OutboundProxy",             "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.OutboundProxy" }, 
    { "OutboundProxyPort",         "int",         "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.OutboundProxyPort" }, 
    { "ProxyServer",             "string­",     "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.ProxyServer" }, 
    { "ProxyServerPort",         "int",         "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.ProxyServerPort" }, 
    { "RegistrarServer",         "string­",     "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.RegistrarServer" }, 
    { "RegistrarServerPort",     "int",         "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.RegistrarServerPort" }, 
    { "UserAgentDomain",        "string­",     "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.UserAgentDomain" }, 
    { "ConferencingURI",        "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.X_RDK-Central_COM_ConferencingURI" }, 
    { "NetworkDisconnect",      "string",    "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.X_RDK-Central_COM_NetworkDisconnect" },
    { "PRACKRequired",          "string",    "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.X_RDK_PRACKRequired" },  
    { "DigitMap",                 "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].DigitMap" }, 
    { "EmDigitMap",             "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].X_RDK-Central_COM_EmergencyDigitMap" },
    { "CustomDigitMap",                 "string",     "Services.VoiceService.[vs].VoiceProfile.[vp].X_RDK-Central_COM_DigitMap" },
    { "SDigitTimer",            "unsignedInt",     "Services.VoiceService.[vs].VoiceProfile.[vp].X_RDK-Central_COM_SDigitTimer" }, 
    { "ZDigitTimer",             "unsignedInt",     "Services.VoiceService.[vs].VoiceProfile.[vp].X_RDK-Central_COM_ZDigitTimer" }, 
    { "DisLoopCurrentReg",      "string",     "Services.VoiceService.[vs].X_RDK_DisableLoopCurrentUntilRegistered" },
    { "DSCPMark",                 "unsignedInt",     "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.DSCPMark" }, 
    { "EthernetPriorityMark",    "int",     "Services.VoiceService.[vs].VoiceProfile.[vp].SIP.EthernetPriorityMark" },
    { "RtpDSCPMark",             "unsignedInt",     "Services.VoiceService.[vs].VoiceProfile.[vp].RTP.DSCPMark" },
    { "RtpEthernetPriorityMark", "int",     "Services.VoiceService.[vs].VoiceProfile.[vp].RTP.EthernetPriorityMark" },
    { "LogServer",               "string",     "Services.VoiceService.[vs].X_RDK_Debug.LogServer" }, 
    { "LogServerPort",          "unsignedInt",     "Services.VoiceService.[vs].X_RDK_Debug.LogServerPort" }, 
    { "TraceGroup",             "string",     "Services.VoiceService.[vs].X_RDK_Debug.CCTKTraceGroup" }, 
    { "TraceLevel",             "string",     "Services.VoiceService.[vs].X_RDK_Debug.CCTKTraceLevel" }, 
    { "ModuleLogLevels",         "string",     "Services.VoiceService.[vs].X_RDK_Debug.ModuleLogLevels" }
};
/*  State machine

S_START ["Services"]   -> S_SERVICES
S_START ["DeviceInfo"] -> S_DEVINFO

S_SERVICES ["VoiceService"] -> S_VOICESERV
S_DEVINFO [any] -> S_LEAF

S_VOICESERV ["PhyInterface"] -> S_PHY
S_VOICESERV ["VoiceProfile"] -> S_PROFILE 
S_VOICESERV [ ?? ] -> LEAF

S_PHY [any] -> LEAF

S_PROFILE ["Line"] -> LEAF
S_PROFILE ["SIP"] -> LEAF
S_PROFILE [ any ] -> LEAF
*/
#define VOICE_HAL_NUM_ELEMS(x) (sizeof(x)/sizeof(x[0]))

int saveCurrentFile(cJSON *obj);
int storeOneLine(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam);
int storeAllLines(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam);
int storeOneVoiceProfile(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam);
int storeAllVoiceProfiles(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam);
int storeOneVoiceService(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam);
int storeAllVoiceServices(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam);

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
/* TR104V2 functionalities */
OBJECT_TYPE getNextObjType(char *fullName);
cJSON* get_array_item_by_uid(const cJSON *array, uint32_t uid);
int updateVoiceServicesCfg(char *fullName,cJSON **config, PVOICE_HAL_SAVE_TYPE_AND_PARAMS pParam);
int deleteVoiceServicesCfg(char *fullName,cJSON **config);
#endif

#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
uint32_t replaceLeafValue(cJSON *pNode, char *pLeafName, PVOICE_HAL_SAVE_PARAMS pParam);
#else
uint32_t replaceLeafValue(cJSON *pNode, char *pLeafName, PVOICE_HAL_SAVE_TYPE_AND_PARAMS pParam);
#endif
/***********************************
 * These functions are called by voice_hal_setXxxx()
 * to update the value in current.json
 *
 * These functions assume only 1 profile/line etc.
 * They will need updating for multiple services etc.
 ************************************/
static bool inInitFunc = false;


#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
int storeObjectString(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine,uint32_t phyIndex, char *nickName, const char *value)
{
    VOICE_HAL_SAVE_PARAMS pParam;
    if (inInitFunc) return 0;    // A little white lie.
    pParam.pValString = value;
    return storeObject(nickName, uiService,uiProfile,uiLine,1, &pParam);
}

int storeObjectInteger(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine,uint32_t phyIndex, char *nickName, int32_t value)
{
    VOICE_HAL_SAVE_PARAMS pParam;
    if (inInitFunc) return 0;    // A little white lie.
    pParam.integer = value;
    return storeObject(nickName, 1,1,1,1, &pParam);
}

int storeObjectBool(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine,uint32_t phyIndex, char *nickName, bool value)
{
    VOICE_HAL_SAVE_PARAMS pParam;
    if (inInitFunc) return 0;    // A little white lie.
    pParam.boolean = value;
    return storeObject(nickName, 1,1,1,1, &pParam);
}
#else
int storeObjectString(char *fullName, const char *value)
{
	VOICE_HAL_SAVE_TYPE_AND_PARAMS pParam;
	if (inInitFunc) return 0;    // A little white lie.
        if(value == NULL|| fullName == NULL)
        {
          return 0;
        }
	pParam.param.pValString = value;
	pParam.type = PARAM_TYPE_STRING;
	return storeObject(fullName,&pParam);
}

int storeObjectInteger(char *fullName,int32_t value)
{
	VOICE_HAL_SAVE_TYPE_AND_PARAMS pParam;
	if (inInitFunc) return 0;    // A little white lie.
        if(fullName == NULL)
        {
          return 0;
        }
	pParam.param.integer = value;
	pParam.type = PARAM_TYPE_INT;
	return storeObject(fullName,&pParam);
}

int storeObjectBool(char *fullName, bool value)
{
	VOICE_HAL_SAVE_TYPE_AND_PARAMS pParam;
	if (inInitFunc) return 0;    // A little white lie.
        if(fullName == NULL)
        {
          return 0;
        }
	pParam.param.boolean = value;
	pParam.type = PARAM_TYPE_BOOLEAN;
	return storeObject(fullName,&pParam);
}
#endif
/*
 * Called by the code that reads the current JSON values, to prevent 
 * the voice_hal_setXxxx() code from trying to write the new value
 * back to the same file!
 */
void startJsonRead(void)
{
    inInitFunc = true;
}
void stopJsonRead(void)
{
    inInitFunc = false;
}

/*******      DEBUG  ONLY    ******/
#ifdef VOICE_HAL_JSON_DEBUG
void dumpJSON_QQQCMS(cJSON *debugConfig)
{
    char *pJSON;
    pJSON = cJSON_Print(debugConfig);
    CcspTraceInfo(("JSON is %s\n", pJSON));
    free(pJSON);
}
#endif
/*******  END OF DEBUG ONLY  ********/
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
/*
 * cJSON *pNode                     the JSON node containing the leaf value to change
 * char *pLeafName                  the name of the leaf
 * PVOICE_HAL_SAVE_PARAMS pParam    the new value of the leaf
 */

uint32_t replaceLeafValue(cJSON *pNode, char *pLeafName, PVOICE_HAL_SAVE_PARAMS pParam)
{
    cJSON *pLeaf = NULL, *pNewValue = NULL;
    if ( NULL != (pLeaf = cJSON_GetObjectItem(pNode, pLeafName) ))
    {
        // Need pLeaf to get the JSON type that we are about to replace
        if (cJSON_IsBool(pLeaf))
        {
            pNewValue = cJSON_CreateBool(pParam->boolean);
        }
        if (cJSON_IsString(pLeaf))
        {
            pNewValue = cJSON_CreateString(pParam->pValString);
        }
        if (cJSON_IsNumber(pLeaf))
        {
            pNewValue = cJSON_CreateNumber(pParam->integer);
        }
        cJSON_ReplaceItemInObject(pNode, pLeafName, pNewValue);
        return 0;
    }
    else
    {
        CcspTraceInfo(("Unknown parameter type for %s\n", pLeafName));
        return (-1);  // No leaf in strucucture??
    }
}
#else
/*
 * cJSON *pNode                     :the JSON node containing the leaf value to change
 * char *pLeafName                  :the name of the leaf
 * PVOICE_HAL_SAVE_TYPE_AND_PARAMS  pParam   :the new value of the leaf and value type
 */
uint32_t replaceLeafValue(cJSON *pNode, char *pLeafName, PVOICE_HAL_SAVE_TYPE_AND_PARAMS pParam)
{
    cJSON *pLeaf = NULL, *pNewValue = NULL;
    if ( NULL != (pLeaf = cJSON_GetObjectItem(pNode, pLeafName) ))
    {
        // Need pLeaf to get the JSON type that we are about to replace
        if (cJSON_IsBool(pLeaf))
        {
            pNewValue = cJSON_CreateBool(pParam->param.boolean);
        }
        if (cJSON_IsString(pLeaf))
        {
            pNewValue = cJSON_CreateString(pParam->param.pValString);
        }
        if (cJSON_IsNumber(pLeaf))
        {
            pNewValue = cJSON_CreateNumber(pParam->param.integer);
        }
        cJSON_ReplaceItemInObject(pNode, pLeafName, pNewValue);
        return 0;
    }
    else
    {
        switch(pParam->type)
        {
            case PARAM_TYPE_STRING:
            {
                pNewValue = cJSON_CreateString(pParam->param.pValString);
                break;
            }
            case PARAM_TYPE_UNSIGNEDINT:
            {
                pNewValue = cJSON_CreateNumber(pParam->param.unsignedInt);
                break;
            }
            case PARAM_TYPE_INT:
            {
                pNewValue = cJSON_CreateNumber(pParam->param.integer);
                break;
            }
            case PARAM_TYPE_BOOLEAN:
            {
                pNewValue = cJSON_CreateBool(pParam->param.boolean);
                break;
            }
            default:return (-1);
        }
        cJSON_AddItemToObject(pNode, pLeafName, pNewValue);
        return (0);
    }
}
#endif

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
/* TR104V2 functionalities */

OBJECT_TYPE getNextObjType(char *fullName)
{
	char str[JSON_MAX_STR_ARR_SIZE] =  {0};
	strncpy(str, fullName, (sizeof(str)-1));
	char delim[] = ".";
	char *ptr = strtok(str, delim);
	char *tmpptr;
	OBJECT_TYPE ret =_UNKNOWN_;

	if (ptr == NULL)
    {
		 CcspTraceInfo(("\n fullName is empty \n"));
		 return ret;
    }

	(void)strtol(ptr, &tmpptr, 10);
	if (*tmpptr == '\0')
	{
		ptr = strtok(NULL, delim);
		if(ptr == NULL)
		{
			ret =_LEAF_UID_;
		}
		else
		{
			ret = _OBJ_UID_;
		}
	}
	else
	{
		ptr = strtok(NULL, delim);
		if(ptr != NULL)
		{
			(void)strtol(ptr, &tmpptr, 10);
			if (*tmpptr == '\0')
			{
				ret = _ARRAY_;
			}
			else
			{
				ret = _OBJECT_;
			}
		}
		else
		{
			ret = _LEAF_ ;
		}
	}
    return ret;
}

cJSON* get_array_item_by_uid(const cJSON *array, uint32_t uid)
{
    cJSON *current_child = NULL;
    cJSON *tmpItem=NULL;
    if (!cJSON_IsArray(array))
    {
        return NULL;
    }

    current_child = array->child;
    while ((current_child != NULL))
    {
        if (NULL != (tmpItem = cJSON_GetObjectItemCaseSensitive(current_child, "@uid")))
		{
			if(cJSON_IsNumber(tmpItem) && tmpItem->valueint==uid)
			{
				break;
			}
		}
        current_child = current_child->next;
    }
    return current_child;
}



/* updateVoiceServicesCfg: */
/**
 * @description Update The value of the config item in the cJSON list
 *
 * @param char *fullName: the full name of the item to be updated
 * @param cJSON **config: full config item of current JSON file
 * @param PVOICE_HAL_SAVE_TYPE_AND_PARAMS pParam: one of these is the value to be stored (string, number, bool)
 *
 * @return 0 on succes, -1 on failure
 *
 * @execution Synchronous.
 * @sideeffect Updates NVRAM.
 *
 */

int updateVoiceServicesCfg (char *fullName,cJSON **config, PVOICE_HAL_SAVE_TYPE_AND_PARAMS pParam)
{
	CcspTraceInfo(("Save '%s' in JSON file??? \n",fullName));
	char str[JSON_MAX_STR_ARR_SIZE] =  {0};
	strncpy(str, fullName, (sizeof(str)-1));
	cJSON *tmpItem=NULL;
	char delim[] = ".";
	char *rest;
	char *ptr = strtok_r(str, delim,&rest);/*The first string of fullName is Device*/
	cJSON *currObj=*config;

	while(ptr != NULL)
	{
		switch(getNextObjType(rest))
		{
		case _ARRAY_:
			/* get next object name*/
			ptr = strtok_r(NULL, delim,&rest);
			if(NULL == (tmpItem = cJSON_GetObjectItemCaseSensitive(currObj, ptr)))
			{
				/*create array if does not exist */
				if (NULL != (tmpItem=cJSON_CreateArray()))
					cJSON_AddItemToObject(currObj, ptr, tmpItem);
				currObj = cJSON_GetObjectItemCaseSensitive(currObj, ptr);
			}
			else
			{
				currObj=tmpItem;
			}
			if(NULL == currObj)
			{
				CcspTraceInfo(("No '%s' array in JSON file???\n",ptr));
				return (-1);
			}
			break;

		case _OBJECT_:
			/* get next object name*/
			ptr = strtok_r(NULL, delim,&rest);
			if(NULL == (tmpItem = cJSON_GetObjectItemCaseSensitive(currObj, ptr)))
			{
				/*create object if does not exist */
				if(NULL!=(tmpItem = cJSON_CreateObject()))
				{
					cJSON_AddItemToObject(currObj,  ptr, tmpItem);
				}
				currObj = cJSON_GetObjectItemCaseSensitive(currObj, ptr);
			}
			else
			{
				currObj=tmpItem;
			}
			if(NULL == currObj)
			{
				CcspTraceInfo(("No '%s' object in JSON file???\n",ptr));
				return (-1);
			}
			break;

		case _OBJ_UID_:
			/* get next object uid*/
			ptr = strtok_r(NULL, delim,&rest);
			if(NULL == (currObj=get_array_item_by_uid(currObj, (uint32_t)atoi(ptr))))
			{
				CcspTraceInfo(("No '%s' instance in JSON file???\n",ptr));
				return (-1);
			}
			break;

		case _LEAF_: //repalce or add leaf item value
			/* get leaf object name*/
			ptr = strtok_r(NULL, delim,&rest);
			(void)replaceLeafValue(currObj, ptr, pParam);
			return 0;

		case _LEAF_UID_:/* used to add object entry in json file*/
			/* get leaf uid*/
			ptr = strtok_r(NULL, delim,&rest);
			if(cJSON_IsArray(currObj))
			{
				tmpItem=get_array_item_by_uid(currObj, (uint32_t)atoi(ptr));
				if(tmpItem==NULL)
				{
					if(NULL!=(tmpItem = cJSON_CreateObject()))
					{
						cJSON_AddItemToArray(currObj, tmpItem);
						cJSON_AddItemToObject(tmpItem, "@uid", cJSON_CreateNumber(atoi(ptr)));
					}
				}
			}
			return 0;

		default:
			return -1;
		}
	}
	return 0;
}


/* deleteVoiceServicesCfg: */
/**
 * @description delete object entries from cJSON list
 * @param char *fullName: the full name of the item to be stored
 * @param cJSON **config: full config item of current JSON file
 *
 * @return 0 on succes, -1 on failure
 *
 * @execution Synchronous.
 * @sideeffect Updates NVRAM.
 *
 */

int deleteVoiceServicesCfg(char *fullName,cJSON **config)
{
	CcspTraceInfo(("Delete '%s'in JSON file???\n",fullName));
	char str[JSON_MAX_STR_ARR_SIZE] =  {0};
	strncpy(str, fullName, (sizeof(str)-1));
	char delim[] = ".";
	char *rest;
	char *ptr = strtok_r(str, delim,&rest);
	cJSON *tmpItem=NULL;
	cJSON *currObj=*config;

	while(ptr != NULL)
	{

		switch(getNextObjType(rest))
		{
		case _ARRAY_:
			/* get next obj name*/
			ptr = strtok_r(NULL, delim,&rest);
			/*get next obj by name from json config*/
			if(NULL == (currObj = cJSON_GetObjectItemCaseSensitive(currObj, ptr)))
			{
				CcspTraceInfo(("No '%s' array in JSON file???\n",ptr));
				return 0;
			}
			break;

		case _OBJECT_:
			/* get next obj name*/
			ptr = strtok_r(NULL, delim,&rest);
			if(NULL == (currObj = cJSON_GetObjectItemCaseSensitive(currObj, ptr)))
			{
				CcspTraceInfo(("No '%s' object in JSON file???\n",ptr));
				return 0;
			}
			break;

		case _OBJ_UID_:
			/* get next uid*/
			ptr = strtok_r(NULL, delim,&rest);

			if(NULL == (currObj=get_array_item_by_uid(currObj, (uint32_t)atoi(ptr))))
			{
				CcspTraceInfo(("No '%s' instance in JSON file???\n",ptr));
				return 0;
			}
			break;

		case _LEAF_UID_:
			/* get next uid*/
			ptr = strtok_r(NULL, delim,&rest);
			tmpItem=cJSON_DetachItemViaPointer(currObj,get_array_item_by_uid(currObj, (uint32_t)atoi(ptr)));
			cJSON_Delete(tmpItem);
			return 0;
		default:
			return -1;
		}
	}
	return 0;
}


/* deleteObject: */
/**
 * @description delete the array item in the current JSON file
 *              (current.json), then writes it back to NVRAM
 * @param char *fullName: full path of the item to be deleted
 *
 * @return 0 on succes, -1 on failure
 *
 * @execution Synchronous.
 * @sideeffect Updates NVRAM.
 *
 */
uint32_t deleteObject(char *fullName)
{
    cJSON *config = NULL, *currObj = NULL;
    uint32_t currentDMSize, i, entry, readBytes, objLen; void* pJsonConfig = NULL; FILE* fp;

    //   cmsLog_debug("%s:%s \n", __FILE__, __FUNCTION__);
    /* Initialise the cJSON parser with the 'current' set of voice data
     * Find the size of file, allocate memory, read it, parse it.
     * Then the original memory can be deleted
     */
    /* First, get the size of the file for JSON current values */
    /* Open the file in read-only mode */
    if (NULL == (fp = fopen(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME, "r")))
    {
        /* RDK log error and give up */
        CcspTraceError(("Failed to open JSON current file %s\n",
            VOICE_CONFIG_DEFAULT_PATH VOICE_CONFIG_CURRENT_NAME));
        return (-1);
    }

    fseek(fp, 0, SEEK_END); /*Move file pointer to the end of file.*/
    currentDMSize = ftell(fp);
    pJsonConfig = malloc(currentDMSize+10); /*Get the current position of the file pointer and get some memory.*/
    if (NULL == pJsonConfig)
    {
        CcspTraceError(("Failed to get memory for JSON file\n"));
        fclose(fp);  // close the file
        return (-1);  // No memory, so give up
    }
    memset(pJsonConfig, (int)0, currentDMSize+3);
    fseek(fp, 0, SEEK_SET);
    if (0 == (readBytes = fread(pJsonConfig, 1, currentDMSize, fp)))  // Any error here will be picked up later
    {
        free(pJsonConfig);
        fclose(fp);
        return (-1);
    }
    fclose(fp);
    if (NULL == (config = cJSON_Parse(pJsonConfig)))
    {
        /* No current nvram file,or it's corrupt - give up */
        cJSON_Delete(config);
        free(pJsonConfig);
        return (-1);
    }
    /* Don't need the in-memory copy of the fle - it's all in the JSON structure 'config' */
    free(pJsonConfig);

    if(deleteVoiceServicesCfg(fullName,&config)==0)
    {
        saveCurrentFile(config);
        cJSON_Delete(config);
        return 0;
    }

    cJSON_Delete(config);
    return (-1);
}
#endif //FEATURE_RDKB_VOICE_DM_TR104_V2

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
/* storeObjects: */
/**
 * @description Stores the updated value of the config item in the current JSON file
 *              (current.json), then writes it back to NVRAM
 * @param char *fullName: the full name of the item to be stored 
 * @param PVOICE_HAL_SAVE_TYPE_AND_PARAMS pParam: one of these is the value to be stored (string, number, bool)
 *
 * @return 0 on succes, -1 on failure
 *
 * @execution Synchronous.
 * @sideeffect Updates NVRAM.
 *
 */
uint32_t storeObject(char *fullName,PVOICE_HAL_SAVE_TYPE_AND_PARAMS pParam)
#else
/* storeObjects: */
/**
 * @description Stores the updated value of the config item in the current JSON file
 *              (current.json), then writes it back to NVRAM
 * @param char *nName: the short name of the item to be stored 
 * @param uint32_t vp: voice profile index - always 1
 * @param uint32_t vs: voice servcie index - always 1
 * @param uint32_t li: line index - always 1
 * @param uint32_t pi; PHY interface index - always 1
 * @param PVOICE_HAL_SAVE_PARAMS pParam: one of these is the value to be stored (string, number, bool)
 * @param char *jsonFile: the name of the current configuration file (including path)
 *
 * @return 0 on succes, -1 on failure
 *
 * @execution Synchronous.
 * @sideeffect Updates NVRAM.
 *
 */
uint32_t storeObject(char *nName, uint32_t vs,  uint32_t vp, uint32_t li,  uint32_t pi, PVOICE_HAL_SAVE_PARAMS pParam)
#endif
{
    cJSON *config = NULL, *currObj = NULL;
    uint32_t currentDMSize, i, entry, readBytes, objLen; void* pJsonConfig = NULL; FILE* fp;
    char *pParse = NULL, *pNickName = NULL;
    enum OBJ_TYPE objType;
//   cmsLog_debug("%s:%s \n", __FILE__, __FUNCTION__);
    /* Initialise the cJSON parser with the 'current' set of voice data 
     * Find the size of file, allocate memory, read it, parse it.
     * Then the original memory can be deleted
     */
    /* First, get the size of the file for JSON current values */
    /* Open the file in read-only mode */
    if (NULL == (fp = fopen(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME, "r")))
    {
        /* RDK log error and give up */
        CcspTraceError(("Failed to open JSON current file %s\n",
            VOICE_CONFIG_DEFAULT_PATH VOICE_CONFIG_CURRENT_NAME));
        return (-1);
    }

    fseek(fp, 0, SEEK_END); /*Move file pointer to the end of file.*/
    currentDMSize = ftell(fp);
    pJsonConfig = malloc(currentDMSize+10); /*Get the current position of the file pointer and get some memory.*/
    if (NULL == pJsonConfig)
    {
        CcspTraceError(("Failed to get memory for JSON file\n"));
        fclose(fp);  // close the file
        return (-1);  // No memory, so give up
    }
    memset(pJsonConfig, (int)0, currentDMSize+3);
    fseek(fp, 0, SEEK_SET);
    if (0 == (readBytes = fread(pJsonConfig, 1, currentDMSize, fp)))  // Any error here will be picked up later
    {
        free(pJsonConfig);
        fclose(fp);
        return (-1);
    }
    fclose(fp);
    if (NULL == (config = cJSON_Parse(pJsonConfig)))
    {
        /* No current nvram file,or it's corrupt - give up */
        cJSON_Delete(config);
        free(pJsonConfig);
        return (-1);
    }
    /* Don't need the in-memory copy of the fle - it's all in the JSON structure 'config' */
    free(pJsonConfig);


#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
/* TR104V2 functionalities */

    if(updateVoiceServicesCfg(fullName,&config,pParam)==0)
    {
        saveCurrentFile(config);
        cJSON_Delete(config);
        return 0;
    }

#else //FEATURE_RDKB_VOICE_DM_TR104_V2

    /*
     * Start looking for the value to replace. First check that we have a valid nick name stirng
     */
    for (i = 0; i<VOICE_HAL_NUM_ELEMS(objs) ; i++)
    {
        if (!strncmp(objs[i].nickName, nName, strlen(objs[i].nickName)))
        {
            pParse = objs[i].fullName;
            pNickName = objs[i].nickName;
            entry = i;   
        }
    }
    if (NULL == pParse)
    {
        CcspTraceInfo(("nickname %s NOT found\n", nName));
        cJSON_Delete(config);
        return (-1);     // Wrong nickname ??
    }

    /* Ready to go.
           pParse points to long object model name
           pNickName points to the short name used to call this function 
           entry is the index into the objs table for future reference
    */
    /* NOTE: we don't create new items. If they aren't in current.json, then it's an error */
    objType = getNextObj(pParse, &objLen);
    /* Must be  'Services' or 'DeviceInfo' */
    if (objType != TEXT_OBJ)
    {
        cJSON_Delete(config);
        return -1;  /* Badly formed DM name ??? */
    }
    switch (getTextType(pParse))
    {
        case TT_Services:
        pParse += 7;  // Service
        pParse++;    // '.'
        if (NULL != (currObj = cJSON_GetObjectItemCaseSensitive(config, "Services")))
        {
            /* 'Service' is alsways followed by 'VoiceService' */
            if (NULL != (currObj = cJSON_GetObjectItemCaseSensitive(currObj, "VoiceService")))
            {
                if (!storeAllVoiceServices(pParse, pNickName, entry, currObj, pParam) )
                {
                    /* Update the string value with the bool param */;
                    saveCurrentFile(config);
                    cJSON_Delete(config);
                    return 0;
                }
                else
                {
                    cJSON_Delete(config);
                    return (-1);
                }
            }
            else
            {
                CcspTraceInfo(("No 'VoiceService' section in JSON file???\n"));
            } 
        }
        else
        {
            CcspTraceInfo(("No 'Services' section in JSON file???\n"));
        }
        break;

        default:
            CcspTraceInfo(("Unreachable code reached !!\n"));
            cJSON_Delete(config);
            return (-1);    // Badly formed DM name ?? 
        break;
    }
#endif
    CcspTraceInfo(("Unrecognised type: %s\n", pParse));
    cJSON_Delete(config);
    return (-1);
}
struct {
    char *paramString;
    enum PARAM_TYPE type;
    uint32_t paramLen;
} csvTypes[] =
{
    { "string", PARAM_TYPE_STRING, 6 },
    { "boolean", PARAM_TYPE_BOOLEAN, 7 },
    { "unsignedInt", PARAM_TYPE_UNSIGNEDINT, 11 },  // Note capitalisation
    { "int", PARAM_TYPE_INT, 3 }
};

enum OBJ_TYPE getNextObj(char *pObj, uint32_t *pLen)
{
    if (*pObj == '[')
    {
        /* This is one of the index markers */
        *pLen = 4;
        if (!strncmp(pObj, "[vs]", 4))
        {
            return VOICE_SERVICE_OBJ;
        }
        if (!strncmp(pObj, "[vp]", 4))
        {
            return VOICE_PROFILE_OBJ;
        }
        if (!strncmp(pObj, "[li]", 4))
        {
            return LINE_INDEX_OBJ;
        }
        if (!strncmp(pObj, "[pi]", 4))
        {
            return PHY_INDEX_OBJ;
        }
    }
    else
    {
        char *pDot;
        /* set the object length of the leaf or next level */
        *pLen = (NULL == (pDot = strchr( pObj, (int)'.') ) ) ? (uint32_t)strlen(pObj) : (uint32_t)( pDot - pObj );
        return (pDot == NULL) ? LEAF_OBJ : TEXT_OBJ;
    } 
    return UNKNOWN_OBJ;   
}
enum PARAM_TYPE getParamType(char *param)
{
    int i;
    for ( i=0 ; i<VOICE_HAL_NUM_ELEMS(csvTypes) ; i++)
    {
        if (!strncmp(csvTypes[i].paramString, param, csvTypes[i].paramLen))
            return csvTypes[i].type;
    }
    return PARAM_TYPE_UNKNOWN;
}
struct {
enum _TT tt;
char *text;
int len;
} ttLookup[] =
{
  { TT_Services, "Services", 8 },
  { TT_VoiceServices, "VoiceServices", 13 },
  { TT_PhyInterface, "PhyInterface", 12 },
  { TT_DiagTests, "Tests", 5 },
  { TT_XRdkDebug, "X_RDK_Debug", sizeof("X_RDK_Debug") - 1},
  { TT_VoiceProfile, "VoiceProfile", 12 },
  { TT_SIP, "SIP", 3 },
  { TT_RTP, "RTP", 3 },
  { TT_Line, "Line", 4 },
  { TT_List, "List", 4 },
  { TT_VoiceProcessing, "VoiceProcessing", 15 },
  { TT_CallingFeatures, "CallingFeatures" }
}; 
enum _TT getTextType(char *text)
{
    int i;
    for ( i=0 ; i<VOICE_HAL_NUM_ELEMS(ttLookup) ; i++)
    {
        if (!strncmp(ttLookup[i].text, text, ttLookup[i].len))
            return ttLookup[i].tt;
    }
    return TT_UNKNOWN;
}
int storeAllVoiceServices(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam)
{
    int numVoiceServices;

    numVoiceServices = cJSON_GetArraySize(jsonObj);
    /* This is where the code needs extending to support multiple services */
    if (numVoiceServices)
    {
        cJSON *voiceObj = NULL;
        pParse += 13; // 'VoiceServices'
        pParse += 6;  // '[si].'
        voiceObj = cJSON_GetArrayItem(jsonObj, 0);
        if (NULL != voiceObj)
        {
            storeOneVoiceService(pParse, pNickName, entry, voiceObj, pParam);
        }
        else
        {
            CcspTraceInfo(("Not one line object?\n"));
        }
        
    }
    else
    {
        CcspTraceInfo(("Zero voice service array\n"));
        return (-1);
    }
    return 0;
}
int storeOneVoiceService(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam)
{
    enum OBJ_TYPE objType; uint32_t objLen; cJSON *leafObj = NULL;

    objType = getNextObj(pParse, &objLen);

    /* Could be LEAF or TEXT */
    if (LEAF_OBJ == objType)
    {
        (void)replaceLeafValue(jsonObj, pParse, pParam);
        return 0;
    }

    if (TEXT_OBJ == objType)
    {
        /* 'PhyInterface' or 'VoiceProfile'  */
        if (TT_VoiceProfile == getTextType(pParse))
        {
            pParse += 12;   // VoiceProfile
            pParse++;       // '.'
            pParse += 5;    // '[vp].'
            jsonObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "VoiceProfile");
            if (NULL != jsonObj)
            {
                CcspTraceInfo(("Found VoiceProfile\n"));
            }
            else
            {
                CcspTraceInfo(("Missing VoiceProfile\n"));
            }
            // jsonObj points to VoiceProfile array
            if (0 == storeAllVoiceProfiles(pParse, pNickName, entry, jsonObj, pParam))
                return 0;
            else
            {
                CcspTraceInfo(("storeAllVoiceProfiles failed !!\n"));
                return (-1);
            }
        }
        if (TT_PhyInterface == getTextType(pParse))
        {
            int numPhyInterfaces;
            pParse += 12;   // PhyInterface
            pParse++;       // '.'
            /* Only 3 items, do them in line */
            pParse += 5;    // '[pi].'
            jsonObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "PhyInterface");
            numPhyInterfaces = cJSON_GetArraySize(jsonObj);

            if (numPhyInterfaces == 1)
            {
                /* PhyInterface has only LEAF objects, all strings. Do them here */
                jsonObj = cJSON_GetArrayItem(jsonObj, 0);
                jsonObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "Tests");
                pParse += 27;   //Tests
                pParse++;       // '.'

                if ( 0 == replaceLeafValue(jsonObj, pParse, pParam) )
                {
                    return 0;
                }

                else
                {
                    CcspTraceInfo(("replaceLeafValue failed with parse point %s\n", pParse));
                    return (-1);
                }
            }
            else
            {
                CcspTraceInfo(("Wrong number of PhyInterface[] with parse point %s\n", pParse));
                return (-1);
            }
            CcspTraceInfo(("Unreachable code reached !!!\n"));
            return (-1);
        }
        if(TT_XRdkDebug ==  getTextType(pParse))
        {
            pParse += (sizeof("X_RDK_Debug") - 1);   // X_RDK_Debug
            pParse ++;  // pParse must now point to the leaf
            leafObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "X_RDK_Debug");
            if (leafObj)
            {
                objType = getNextObj(pParse, &objLen);

                /* Only LEAF is expected*/
                if (LEAF_OBJ == objType)
                {
                    if ( 0 == replaceLeafValue(leafObj, pParse, pParam) )
                    {
                        CcspTraceInfo(("Save success for %s...\n", pParse));
                        return 0;
                    }
                    else
                    {
                        CcspTraceInfo(("Save failed at parse point %s\n", pParse));
                        return (-1);
                    }
                }
                else
                {
                    CcspTraceInfo(("Invalid object type for %s\n", pParse));
                    return (-1);
                }
            }
            else
            {
                CcspTraceInfo(("Could not find json object: X_RDK_Debug\n"));
                return (-1);
            }
        }
/* TODO wrong place  */
        if (TT_DiagTests == getTextType(pParse))
        {
            pParse += 27;   // X_RDK-Central_COM_DiagTests
            pParse ++;  // pParse must now point to the leaf
            leafObj = cJSON_GetObjectItemCaseSensitive(jsonObj, pParse);
            /* 3 diagnostics. Do them in line */
            if (cJSON_IsString(leafObj) && (leafObj->valuestring != NULL))
            {
                /* Update the string value with the string param */;
                return 0;
            }
        }
/* TODO end */
        CcspTraceInfo(("Unreachable code reached !!!\n"));
        return (-1);   // Unknown item ??
    }
    CcspTraceInfo(("Unreachable code reached !!!\n"));
    return (-1); 
}
int storeAllVoiceProfiles(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam)
{
    int numVoiceProfiles;

    numVoiceProfiles = cJSON_GetArraySize(jsonObj);
    if (numVoiceProfiles == 1)
    {
        CcspTraceInfo(("Found one VoiceProfile\n"));
        jsonObj = cJSON_GetArrayItem(jsonObj, 0);
        if (NULL == jsonObj)
        {
            CcspTraceInfo(("No VoiceProfile array item\n"));
            return (-1);
        }
    }    
    else
    {
        CcspTraceInfo(("Wrong number of VoiceProfile[]\n"));
        return (-1);
    }
    // jsonObj points to VoiceProfile[1].Xxxx
    if (0 == storeOneVoiceProfile(pParse, pNickName, entry, jsonObj, pParam))
    {
        return 0;
    }
    else
    {
        CcspTraceInfo(("storeOneVoiceProfile failed\n"));
        return (-1);
    }
    CcspTraceInfo(("Unreachable code reached !!\n"));
    return (-1);
}
int storeOneVoiceProfile(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam)
{
    enum OBJ_TYPE objType; uint32_t objLen;
    
    /* Next item could be 'Line[]', 'SIP', 'RTP' or a LEAF */

    objType = getNextObj(pParse, &objLen);

    /* Could be LEAF or TEXT */
    if (LEAF_OBJ == objType)
    {
        if ( 0 == replaceLeafValue(jsonObj, pParse, pParam) )
        {
            return 0;
        }
        else
        {
            CcspTraceInfo(("replaceLeafValue failed at parse point %s\n", pParse));
            return (-1);
        }
    }

    if (TEXT_OBJ == objType)
    {
        /* Only 'Line[]', 'SIP' and 'RTP' permitted here */
        if (TT_Line == getTextType(pParse))
        {
            pParse += 4;   // 'Line'
            pParse += 6;    // '.[li].'
            if (0 == storeAllLines(pParse, pNickName, entry, jsonObj, pParam))
            {
                return 0;
            }
            else
            {
                CcspTraceInfo(("storeAllLines failed at parse point %s\n", pParse));
                return (-1);
            }
        }

        if (TT_SIP == getTextType(pParse))
        {
            pParse += 4;   // 'SIP.'
            /* Is all LEAF, so do in line */ 
            if (NULL == (jsonObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "SIP")))
            {
                CcspTraceInfo(("No VoiceProfile/SIP, parse point is %s\n", pParse));
                return (-1);
            }
            if ( 0 == replaceLeafValue(jsonObj, pParse, pParam) )
            {
                return 0;
            }
            else
            {
                CcspTraceInfo(("replaceLeafValue failed at parse point %s\n", pParse));
                return (-1);
            }
        }

        if (TT_RTP == getTextType(pParse))
        {
            pParse += 4;   // 'RTP.'
            /* Is all LEAF, so do in line */
            if (NULL == (jsonObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "RTP")))
            {
                CcspTraceInfo(("No VoiceProfile/RTP, parse point is %s\n", pParse));
                return (-1);
            }
            if ( 0 == replaceLeafValue(jsonObj, pParse, pParam) )
            {
                return 0;
            }
            else
            {
                CcspTraceInfo(("replaceLeafValue failed at parse point %s\n", pParse));
                return (-1);
            }
        }
    }
    CcspTraceInfo(("Unreachable code reached !!\n"));
    return (-1);
}
int storeAllLines(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam)
{
    int numLines; cJSON *lineObj = NULL;

    lineObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "Line");
    if (NULL == lineObj)
    {
        CcspTraceInfo(("No lineObj !!\n"));
        return 0;  // Might not be an error
    }
    numLines = cJSON_GetArraySize(lineObj);
    if (numLines == 1)
    {
        lineObj = cJSON_GetArrayItem(lineObj, 0);
        if (NULL != lineObj) 
        {
            CcspTraceInfo(("Got one lineObj\n"));
        }
        else
        {
            CcspTraceInfo(("No single lineObj\n"));
            return 0;   // Might not be an error
        }
    }
    else
    {
        CcspTraceInfo(("numLines is %d, pParse %s\n", numLines, pParse));
        return (-1);
    }
    // lineObj points to a single instance of the 'Line' array
    if (0 == storeOneLine(pParse, pNickName, entry, lineObj, pParam))
    {
        return 0;
    }
    else
    {
        CcspTraceInfo(("storeOneLine failed, parse point is %s\n", pParse));
        return (-1);
    }
    CcspTraceInfo(("Unreachable code reached !!\n"));
    return (-1);
}
int storeOneLine(char *pParse, char *pNickName, int entry, cJSON *jsonObj, PVOICE_HAL_SAVE_PARAMS pParam)
{
    enum OBJ_TYPE objType; uint32_t objLen; cJSON *leafObj = NULL;
    
    /* Next item could be 'Line[]', 'SIP' or a LEAF */

    objType = getNextObj(pParse, &objLen);

    CcspTraceInfo(("pParse is %s\n", pParse));
    /* Could be LEAF or TEXT */
    if (LEAF_OBJ == objType)
    {
        CcspTraceInfo(("Line object LEAF\n"));
        if ( 0 == replaceLeafValue(jsonObj, pParse, pParam) )
        {
            return 0;
        }
        else
        {
            CcspTraceInfo(("replaceLeafValue failed, parse point is %s\n", pParse));
            return (-1);
        }
    }

    if (TEXT_OBJ == objType)
    {
        /* 'CallingFeatures', 'SIP', 'VoiceProcessing' permitted here */
        CcspTraceInfo(("Line object TEXT_OBJ, pParse %s\n", pParse));
        if (TT_CallingFeatures == getTextType(pParse))
        {
            pParse += 16;   // 'CallingFeatures.'
            /* All CallingFeatures are LEAF objects, do them in line */
            if ( NULL == (jsonObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "CallingFeatures")))
            {
                CcspTraceInfo(("Could not find calling features\n"));
                return (-1);
            }
            if ( 0 == replaceLeafValue(jsonObj, pParse, pParam) )
            {
                return 0;
            }
            else
            {
                CcspTraceInfo(("replaceLeafValue failed, parse point %s\n", pParse));
                return (-1);
            }
        }
        if (TT_SIP == getTextType(pParse))
        {
            pParse += 4;   // 'SIP.'
            /* All LineSIP objects are LEAF, do them in line */
            CcspTraceInfo(("pParse is %s\n", pParse));
            if (NULL == (jsonObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "SIP")))
            {
                CcspTraceInfo(("(LINE) SIP not found\n"));
            }
            else
            {
                CcspTraceInfo(("Found (LINE) SIP\n"));
            }    
            if ( 0 == replaceLeafValue(jsonObj, pParse, pParam) )
            {
                return 0;
            }
            else
            {
                CcspTraceInfo(("Line/SIP replaceLeafValue failed, parse point %s\n", pParse));
                return (-1);
            }
        }

        if (TT_VoiceProcessing == getTextType(pParse))
        {
            pParse += 16;   // 'VoiceProcessing.'
            /* TxGain, RxGain are numeric and LEAF */
            CcspTraceInfo(("VoiceProcessing pParse is %s\n", pParse));
            jsonObj = cJSON_GetObjectItemCaseSensitive(jsonObj, "VoiceProcessing");
            if (NULL == jsonObj)
            {
                CcspTraceInfo(("No VoiceProcessing object\n"));
                return 0;   // Maybe not an error
            }
            leafObj = cJSON_GetObjectItemCaseSensitive(jsonObj, pParse);
            if (NULL == leafObj)
            {
               CcspTraceInfo(("VoiceProcessing leafObj %s is null\n", pParse));
               return 0;
            }
            if (cJSON_IsNumber(leafObj))
            {
                CcspTraceInfo(("SetNumberValue %d\n", pParam->integer));
                cJSON_SetNumberValue(leafObj, pParam->integer);
                /* Update the number value with the string param */;
                return 0;
            }
        }
    }
    CcspTraceInfo(("Unreachable code reached !!\n"));
    return (-1);
}
/* saveCurrentFile :  */
/**
* @description Save the updated JSON configuration in obj as a JSON text file in the 'current.json' file.
* @param cJSON *obj: A pointer to the in-,memory representation of the JSON current config values.
*              To minimize the risk of corruption due to an untimely power cycle, we create a new 
*              temp file first, then delete current.json and rename temp->current.json.
* Given the small size of the files (~5K bytes), this is unlikely to increase the wear rate of the NVRAM.
*
* @return The status of the operation.
* @retval 0 if successful.
* @retval -1 if any error is detected
*
* @execution Synchronous.
* @sideeffect NVRAM update.

*
* @note This function must not suspend and must not invoke any blocking system
* calls.
*
*/
#define VOICE_HAL_SLOW_FLASH        // To cure test script problems

int saveCurrentFile(cJSON *obj)
{
    char *pJSON;
    int ret = 0;
    unsigned int confSize = 0;
#ifdef VOICE_HAL_SLOW_FLASH
    int fdNew;
#else
    FILE *fpNew;
#endif
    /* 'Print' the file to memory */
    if (NULL == (pJSON = cJSON_Print(obj) ) )
    {
        CcspTraceInfo(("\n\nsaveCurrentFile error, JSON is <null>\n\n"));
        return (-1);
    }
    confSize = strlen(pJSON);
#ifdef VOICE_HAL_SLOW_FLASH
    /* The new method, hopefully causimng the JFFS2 flash to be updated reliably */
    /* Since fsync() uses fd's not fp's, the code is a bit different */
    if ( -1 == (fdNew = open(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME, 
        O_WRONLY | O_SYNC | O_TRUNC)))
    {
        CcspTraceInfo(("Could not open current file %s for writing!\n", 
            VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME));
        free(pJSON);
        return (-1);
    }   
    /* Write out the memory image to current */
    ret = write(fdNew, pJSON, confSize);
    if (ret == confSize)
    {
        uint32_t crc = checksum((const uint8_t *)pJSON, confSize);
        if (0 != (ret = writeChecksumFile(crc)))
        {
            CcspTraceInfo(("Error writing checksum file %s !\n", 
                          VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CHECKSUM_NAME));
            //next time we save try to recreate these files however if this api isnt called we accept losing the running config
            //as a consequence of a call to open with O_TRUNC the file was lost anyway
            close(fdNew);
            free(pJSON);      //Finished with this
            unlink(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME);
            unlink(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CHECKSUM_NAME);
            return -1;
        }
        free(pJSON);        //Finished with this
        fsync(fdNew);       //Should not return until data close to flash
        close(fdNew);
        sync();
        return 0;
        
    }
    else
    {
        CcspTraceInfo(("Could not update current file %s!\n", 
                      VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME));
        free(pJSON);        //Finished with this
        close(fdNew);
        //next time we save try to recreate these files however if this api isnt called we accept losing the running config
        //as a consequence of a call to open with O_TRUNC the file was lost anyway
        unlink(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME);
        unlink(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CHECKSUM_NAME);
        return -1;
    }

#else
    /* The old method. Updates are persist across reboot if 'reboot' from command line */
    /* But sometimes fail if power cycled within 2-3 seconds. Especially obvious in */
    /* test scenarios where multiple changes are performed in fast sequence by script */
    /* Create a temp file for the new config. If the file already exists (power cycle corruption??) then it's deleted */
    if ( NULL == (fpNew = fopen(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_TEMP_NAME, 
        "w")))
    {
        CcspTraceInfo(("Could not open temp file %s\n", VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_TEMP_NAME));
        free(pJSON);
        return (-1);
    }   
    /* Write out the memory image to temp */
    fwrite(pJSON, 1, strlen(pJSON), fpNew);
    free(pJSON);        //   Finished with this
    fclose(fpNew);      //   Posix remove()/rename() don't use fp's
    /* Delete the original file. Note: this is 'unlink', so the file will still exist if any other process has opened it */
    remove(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME);
    /* Rename the temp file to original. If it fails, not much I can do... */
    if ( 0!= rename(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_TEMP_NAME, VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME))
    {
        CcspTraceInfo(("Failed to rename temp JSON file! errno is %d\n", errno));
        return (-1);
    }
    sync();     // *Schedules* writing to file system
                // fdatasync(fp) might be required
      return 0;
#endif

}

/* checksum:  */
/**
* @description caculate the CRC 32 of the given data.
* @Param   : pdata - array of data.
* @Param   : size - number of input data bytes.
*
* @return CRC value
*/
uint32_t checksum(const uint8_t *pdata, uint32_t size) 
{
    uint32_t crc = CRC32_INIT_VALUE;
    while (size-- > 0)
    {
        crc = (crc >> 8) ^ Crc32_table[(crc ^ *pdata++) & 0xff];
    }
#ifndef HUB4_SDK_L07
    return crc ^ CRC32_INIT_VALUE;
#else
    return crc;
#endif
}

/* writeChecksumFile: */
/**
* @description  Called by the code that saves the current JSON values to file,
*           this function calculates the checksum for the latest JSON file and stores it in to a file.
*            This function is not available from the CCSP layer.
* @param uint32_t crc - input crc value
*
* @return 0 on succes, -1 on failure
*
* @execution Synchronous.
* @sideeffect None.
*
*/
int32_t writeChecksumFile(uint32_t crc)
{
    FILE *fpChksum = fopen(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CHECKSUM_NAME, "w");
    if ( NULL == fpChksum)
    {
        CcspTraceInfo(("Could not open checksum file %s\n", VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CHECKSUM_NAME));
        return -1;
    }
    if (0 > fprintf(fpChksum, "%u", crc)) 
    {
        CcspTraceInfo(("Could not write checksum file %s\n", VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CHECKSUM_NAME));
        fclose(fpChksum);
        return -1;
    }
    fsync(fileno(fpChksum));
    fclose(fpChksum);
    sync();
    return 0;
}

int32_t createChecksumFile()
{
    void *pJsonConfig = NULL;
    uint32_t confSize, readBytes;
    char filename[MAX_FILENAME_LENGTH] = {0};
    snprintf(filename, MAX_FILENAME_LENGTH, "%s%s", VOICE_CONFIG_DEFAULT_PATH,gVOICE_CONFIG_DEFAULT_NAME);
    FILE *fpChksum = fopen(filename, "r");
    if (NULL == fpChksum)
    {
        /* RDK log error and give up */
        CcspTraceError(("Failed to open JSON defaults file %s\n",filename));
        return -1;
    }

    fseek(fpChksum, 0, SEEK_END); /*Move file pointer to the end of file.*/
    pJsonConfig = malloc(confSize = ftell(fpChksum)); /*Get the current position of the file pointer and get some memory.*/
    if (NULL == pJsonConfig)
    {
        CcspTraceError(("Failed to get memory for JSON file\n"));
        fclose(fpChksum);  // close the file
        return -2;  // No memory, so give up, special case where we wont revert to factory defaults
    }
    fseek(fpChksum, 0, SEEK_SET);  
    if (confSize != (readBytes = fread(pJsonConfig, 1, confSize, fpChksum)))  // Any error here will be picked up later
    {
        CcspTraceInfo(("Failed to read JSON file %s of size %d. Read %d. errno %d \n",
                      filename , confSize, readBytes, errno));
        free(pJsonConfig);
        fclose(fpChksum);
        return -1;
    }
    uint32_t newCrc = checksum((const uint8_t *)pJsonConfig, confSize);
    free(pJsonConfig);
    fclose(fpChksum);
    return writeChecksumFile(newCrc);
}
