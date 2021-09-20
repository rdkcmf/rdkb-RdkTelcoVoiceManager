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

#ifndef TELCOVOICEMGR_DML_JSON_CFG_INIT_H
#define TELCOVOICEMGR_DML_JSON_CFG_INIT_H
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_services_apis_v2.h"
#endif
/* Some file+path defines */
#define VOICE_CONFIG_DEFAULT_PATH "/usr/rdk/voicemanager/"     // Read only
#define VOICE_CONFIG_CURRENT_PATH "/opt/secure/"
#define VOICE_CONFIG_CURRENT_NAME "telcovoice_config_current.json"
#define VOICE_CONFIG_TEMP_NAME "telcovoice_config_temp.json"
#define VOICE_CONFIG_DEFAULT_NAME "telcovoice_config_default.json"
#define VOICE_CONFIG_DEFAULT_ITA_NAME "telcovoice_config_default_ITA.json"
#define VOICE_CONFIG_DEFAULT_UK_NAME "telcovoice_config_default_UK.json"
#define VOICE_CONFIG_DEFAULT_ROI_NAME "telcovoice_config_default_ROI.json"
#define VOICE_CONFIG_CHECKSUM_NAME "telcovoice_config_checksum"

#define MAX_REGION_LENGTH   16
#define MAX_FILENAME_LENGTH 128

#define   VOICE_SERVICE_TABLE_NAME            "Device.Services.VoiceService.%d."
#define   VOICE_SERVICE_STATUS                "Device.Services.VoiceService.%d.X_RDK_Status"
#define   X_RDK_DEBUG_TABLE_NAME              "Device.Services.VoiceService.%d.X_RDK_Debug."

#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
/*  TR104 V2 DML Tables */
#define   PHYINTERFACE_TABLE_NAME             "Device.Services.VoiceService.%d.Terminal.%d.Diagtests."
#define   PROFILE_TABLE_NAME                  "Device.Services.VoiceService.%d.VoIPProfile.%d."
#define   SIP_TABLE_NAME                      "Device.Services.VoiceService.%d.SIP.Network.%d."
#define   RTP_TABLE_NAME                      "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP."
#define   LINE_TABLE_NAME                     "Device.Services.VoiceService.%d.CallControl.Line.%d."
#define   LINE_SIP_TABLE_NAME                 "Device.Services.VoiceService.%d.SIP.Client.%d."
#define   LINE_VOICE_PROCESSING_TABLE_NAME    "Device.Services.VoiceService.%d.Terminal.%d.Audio.%d.VoiceProcessing."
#define   LINE_CALING_FEATURE_TABLE_NAME      "Device.Services.VoiceService.%d.CallControl.CallingFeatures.Set.%d."
#define   LINE_STATUS                         "Device.Services.VoiceService.%d.CallControl.Line.%d.Status"
#define   CALL_STATE                          "Device.Services.VoiceService.%d.CallControl.Line.%d.CallStatus"
#define   LINE_STATS_TABLE_NAME               "Device.Services.VoiceService.%d.CallControl.Line.%d.Stats."
#else
/* TR104 V1 DML Tables*/
#define   PHYINTERFACE_TABLE_NAME             "Device.Services.VoiceService.%d.PhyInterface.%d.Tests."
#define   PROFILE_TABLE_NAME                  "Device.Services.VoiceService.%d.VoiceProfile.%d."
#define   SIP_TABLE_NAME                      "Device.Services.VoiceService.%d.VoiceProfile.%d.SIP."
#define   RTP_TABLE_NAME                      "Device.Services.VoiceService.%d.VoiceProfile.%d.RTP."
#define   LINE_TABLE_NAME                     "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d."
#define   LINE_SIP_TABLE_NAME                 "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.SIP."
#define   LINE_VOICE_PROCESSING_TABLE_NAME    "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.VoiceProcessing."
#define   LINE_STATS_TABLE_NAME               "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.Stats."
#define   LINE_CALING_FEATURE_TABLE_NAME      "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallingFeatures."
#define   LINE_STATUS                         "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.Status"
#define   CALL_STATE                          "Device.Services.VoiceService.%d.VoiceProfile.%d.Line.%d.CallState"
#endif /*FEATURE_RDKB_VOICE_DM_TR104_V2*/

#include <stdbool.h>
#include "cJSON.h"

char gVOICE_CONFIG_DEFAULT_NAME[MAX_FILENAME_LENGTH];

/* checksum:  */
/**
* @description caculate the CRC 32 of the given data.
* @Param   : pdata - array of data.
* @Param   : size - number of input data bytes.
*
* @return CRC value
*
* @execution Synchronous.
* @sideeffect None.
*/
uint32_t checksum(const uint8_t *pdata, uint32_t size);

/* verifyChecksumFile: */
/**
* @description  Called by the code that initializes datamodel from the JSON file,
*           this function calculates the checksum of the latest JSON file and compares it against the stored expected checksum value.
*           This function is not available from the CCSP layer.
* @param uint8_t *pcbuf - input the test result
* @param uint32_t confSize - input config buffer size
*
* @return 0 on succes, -1 on failure
*
* @execution Synchronous.
* @sideeffect None.
*
*/
int32_t verifyChecksumFile(const uint8_t *pcbuf, uint32_t confSize);

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
int32_t writeChecksumFile(uint32_t crc);

/* createChecksumFile: */
/**
* @description  Called by the code that restores factory defaul configuration, this function creates the crc value for 
*               the factory default config file.
*
* @return 0 on succes, -1 on failure
*
* @execution Synchronous.
* @sideeffect None.
*
*/
int32_t createChecksumFile();

/* startJsonRead(): */
/* stopJsonRead(): */
/**
* @description  Called by the code that reads the current JSON values, to prevent 
*               the voice_hal_setXxxx() code from trying to write the new value
*               back to the same file!
*            This function is not available from the CCSP layer.
* @param none
*
* @return Nothing.
*
* @execution Synchronous.
* @sideeffect Stops JSON updates.
*
*/
void startJsonRead(void);
void stopJsonRead(void);

/* set_dm_defaults: */
/**
* @description Read a JSON configuration file & set the values accordingly, using either the
*              public 'TelcoVoiceMgrDmlXxxx()' functions or the private '_setXxxx()' functions.
*            This function is not available from the CCSP layer.
* @param none - the path to the JSON file is hardcoded
*
* @return 0 on succes, -1 on failure
*
* @execution Synchronous.
* @sideeffect None.
*
*/
int32_t TelcoVoiceJsonCfgSetDmDefaults(void);

/***********************************
 * These functions are called by TelcoVoiceMgrDmlSetXxxx()
 * to update the value in current.json
 *
 * These functions assume only 1 profile/line etc.
 * They will need updating for multiple services etc.
 ************************************/
int storeObjectString(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine,uint32_t phyIndex, char *nickName, const char *value);
int storeObjectInteger(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine,uint32_t phyIndex, char *nickName, int32_t value);
int storeObjectBool(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine,uint32_t phyIndex, char *nickName, bool value);

/*********************************************
 * the following are for the JSON file save routines
 */
/* A little union to carry the assortment of different parameters of the DM objects */
typedef union {
    const char *pValString;
    uint32_t unsignedInt;
    bool boolean;
    int integer;
    /* enums etc to be added */   
} VOICE_HAL_SAVE_PARAMS, *PVOICE_HAL_SAVE_PARAMS;

/* What type of thing are we looking for in the JSON settings file? */
enum OBJ_TYPE
{
    TEXT_OBJ,           /* VoiceService/Line/PhyInterface/Profile */
    LEAF_OBJ,           /* Last object in the hierarchy */
    VOICE_SERVICE_OBJ,  /* [vs] */
    VOICE_PROFILE_OBJ,  /* [vp] */
    LINE_INDEX_OBJ,     /* [li] */
    PHY_INDEX_OBJ,      /* [pi] */
    UNKNOWN_OBJ
};
/* The types of parameter supplied by the TelcoVoiceMgrDmlSetxx() functions */
enum PARAM_TYPE
{
    PARAM_TYPE_UNKNOWN,
    PARAM_TYPE_STRING,
    PARAM_TYPE_UNSIGNEDINT,
    PARAM_TYPE_INT,
    PARAM_TYPE_BOOLEAN
};

/* storeObjects: */
/**
 * @description Stores the updated value of the config item in the cuurnet JSON file
 *              (current.json), then writes it back to NVRAM
 * @param char *nName: the short name of the item to be stored 
 * @param uint32_t vp: voice profile index - always 1
 * @param uint32_t vs: voice servcie index - always 1
 * @param uint32_t li: line index - always 1
 * @param uint32_t pi; PHY interface index - always 1
 * @param PVOICE_HAL_SAVE_PARAMS pParam: one of these is the value to be stored
 * @param char *jsonFile: the name of the current configuration file (including path)
 *
 * @return 0 on succes, -1 on failure
 *
 * @execution Synchronous.
 * @sideeffect Updates NVRAM.
 *
 */
uint32_t storeObject(char *nName,  uint32_t vs,  uint32_t vp,  uint32_t li,  uint32_t pi, PVOICE_HAL_SAVE_PARAMS pParam);

enum OBJ_TYPE getNextObj(char *pObj, uint32_t *pLen);
enum PARAM_TYPE getParamType(char *param);
enum _TT {
    TT_Services, 
    TT_VoiceServices, 
    TT_PhyInterface, 
    TT_VoiceProfile, 
    TT_DiagTests,
    TT_SIP, 
    TT_RTP,
    TT_Line, 
    TT_List,
    TT_CallingFeatures,
    TT_VoiceProcessing,
    TT_UNKNOWN
} TEXT_TYPES;
enum _TT getTextType(char *text);
int storeServices(char *pParse, cJSON *jsonObj);
int storeVoiceProfile();
int saveCurrentFile(cJSON *obj);
int jsonPwdDecode(const char *pInBuf, uint32_t inLen, char *pOutBuf, uint32_t outLen);
int jsonPwdEncode(const char *pInBuf, uint32_t inLen, char *pOutBuf, uint32_t outLen);

/* Log levels used by voice process (X_BROADCOM_COM_LoggingLevel) */
enum VOICE_PROCESS_LOG_LEVEL
{
    VOICE_PROCESS_LOG_LEVEL_ERROR = 0,
    VOICE_PROCESS_LOG_LEVEL_NOTICE,
    VOICE_PROCESS_LOG_LEVEL_DEBUG
};

#endif
