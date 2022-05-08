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

/**
* @file voice_hal_init.c
* @author sky
* @brief Initialisation file for RdkTelcoVoiceManager
*
* @description This file reads a JSON file holding default configuration values, and sets
*              the corresponding DM item via the voice_hal functions.
*              To read current values (rather than default values), call with telcovoice_config_current.json
*              rather than telcovoice_config_default.json.
*              To change region, copy telcovoice_config_default_UK | telcovoice_config_default_ITA | telcovoice_config_default_ROI -> telcovoice_config_default.json and call.
*/
#include "cJSON.h"              // Used by voice_hal and voice_hal_priv
#include <rdk_error.h>
#include <rdk_debug.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "ccsp_trace.h"

#include "telcovoicemgr_dml_json_cfg_init.h"
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#include "telcovoicemgr_services_apis_v2.h"
#else
#include "telcovoicemgr_services_apis_v1.h"
#endif
#include "telcovoicemgr_dml_hal.h"

#define VOICE_HAL_NORMAL_DIGIT_MAP  "DigitMap"
#define VOICE_HAL_EMERGENCY_DIGIT_MAP  "X_RDK-Central_COM_EmergencyDigitMap"
#define VOICE_HAL_CUSTOM_DIGIT_MAP  "X_RDK-Central_COM_DigitMap"

/* Forward declarations */
static void jsonParseVoiceService(uint32_t index, cJSON *voiceService);
static void jsonParseAllPhyInterfaces(uint32_t voiceService, cJSON *phyInterfaces);
static void jsonParseAllVoiceProfiles(uint32_t voiceService, cJSON *voiceProfiles);
static void jsonParseX_RDK_Debug(uint32_t service, cJSON *X_RDK_DebugObj);
static void jsonParseOnePhyInterface(uint32_t service, uint32_t index, cJSON *phyInterface);
static void jsonParseOneVoiceProfile(uint32_t service, uint32_t index, cJSON *voiceProfile);
static int32_t jsonParseAllLines(uint32_t service, uint32_t profile, cJSON *lines);
static void jsonParseOneLine(uint32_t service, uint32_t profile, uint32_t line, cJSON *vLine);
static void jsonParseLineSIP(uint32_t service, uint32_t profile, uint32_t line, cJSON *vp);
static int32_t jsonParseProfileSIP(uint32_t service, uint32_t profile, cJSON *ps);
static int32_t jsonParseProfileRTP(uint32_t service, uint32_t profile, cJSON *ps);

static void jsonCfgDoCallingFeature(uint32_t service, uint32_t profile, uint32_t line, cJSON *lineObj);
static void jsonCfgDooVoiceProcessing(uint32_t service, uint32_t profile, uint32_t line, cJSON *lineObj);
static int32_t jsonCfgSetLineEnable(uint32_t service, uint32_t profile, uint32_t line, char* buffer);
static void jsonCfgDoVoiceProcessing(uint32_t service, uint32_t profile, uint32_t line, cJSON *lineObj);

static void fcopy(char *src, char *dst);

static int32_t voiceHalInitDmDefaults(void);
static ANSC_STATUS initialise_line_calling_features(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM eFeature, BOOL bStatus);

static json_object *jInitMsg = NULL;
static hal_param_t initParam;

/* TelcoVoiceJsonCfgSetDmDefaults: */
/**
* @description Checks for the presence of a 'current.json' file.
*            If not present, it copies it from the default version.
* @param none
*
* @return 0 on succes, -1 on failure
*
* @execution Synchronous.
* @sideeffect Updates data model.
*
*/
int32_t TelcoVoiceJsonCfgSetDmDefaults(void)
{
    struct stat buffer;
    int32_t ret = -1;
    if (0 == stat( VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME, &buffer ))
    {
        /* Now read the current config file to set DM values */
        ret = voiceHalInitDmDefaults();
    }
    // json file parsing failed for some reason revert to factory defaults state and retry
    if (-1 == ret)
    {
        /* Copy telcovoip_config_default.json to telcovoice_config_current.json */

        char filename[MAX_FILENAME_LENGTH] = {0};
#ifdef _SKY_HUB_COMMON_PRODUCT_REQ_
        char region[MAX_REGION_LENGTH] = {0};
        if(platform_hal_GetRouterRegion(region) == RETURN_OK)
        {
            if(0 == strncmp(region,"IT",2))
            {
                snprintf(gVOICE_CONFIG_DEFAULT_NAME, MAX_FILENAME_LENGTH, VOICE_CONFIG_DEFAULT_ITA_NAME);
            }
            else if(0 == strncmp(region,"GB",2))
            {
                snprintf(gVOICE_CONFIG_DEFAULT_NAME, MAX_FILENAME_LENGTH, VOICE_CONFIG_DEFAULT_UK_NAME);
            }
            else if(0 == strncmp(region,"ROI",3))
            {
                snprintf(gVOICE_CONFIG_DEFAULT_NAME, MAX_FILENAME_LENGTH, VOICE_CONFIG_DEFAULT_ROI_NAME);
            }
            else
            {
                CcspTraceError(("Unknown Region %s\n",region));
                snprintf(gVOICE_CONFIG_DEFAULT_NAME, MAX_FILENAME_LENGTH, VOICE_CONFIG_DEFAULT_NAME);
            }
        }
        else
        {
            CcspTraceError(("platform_hal_GetRouterRegion - Failed\n"));
            return ANSC_STATUS_FAILURE;
        }
#else
        snprintf(gVOICE_CONFIG_DEFAULT_NAME, MAX_FILENAME_LENGTH, VOICE_CONFIG_DEFAULT_NAME);
#endif
        snprintf(filename, MAX_FILENAME_LENGTH, "%s%s", VOICE_CONFIG_DEFAULT_PATH,gVOICE_CONFIG_DEFAULT_NAME);

        CcspTraceInfo(("copying %s to %s\n",
            filename, VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME));

        fcopy(filename, VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME);
        if (0 != createChecksumFile())
            return ANSC_STATUS_FAILURE;
        else
            return voiceHalInitDmDefaults();
    }

    return ret;
}

/* fcopy: */
/**
* @description Copies the source file to the destination.
*              Source is unchaged, destination overwritten if it already exists.
*              Default permissions for destination file.
*      No Posix function for this.
* @param char *src - input the file to be copied
* @param char *dst - input the file to be created/overwritten
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect None
*
*/
static void fcopy(char *src, char *dst)
{
    char            buffer[512];
    size_t          n;
    FILE *fpSrc, *fpDst;

    if (NULL == (fpSrc = fopen(src, "r")))
    {
        CcspTraceInfo(("Failed to open source %s for copy\n", src));
        return;
    }
    if (NULL == (fpDst = fopen(dst, "w")))
    {
        CcspTraceInfo(("Failed to open destination %s for copy, errno is %d\n", dst, errno));
        fclose(fpSrc);
        return;
    }
    while ((n = fread(buffer, sizeof(char), sizeof(buffer), fpSrc)) > 0)
    {
        if (fwrite(buffer, sizeof(char), n, fpDst) != n)
            CcspTraceInfo(("Copy failed\n"));
    }
    fclose(fpSrc);
    fsync(fileno(fpDst));
    fclose(fpDst);
    sync();
    CcspTraceInfo(("Copy %s->%s succeded\n", src, dst));
}

/* verifyChecksumFile: */
/**
* @description  Called by the code that initializes datamodel form the JSON file,
*           this function calculates the checksum for the latest JSON file and compares it against the stored checksum value from file.
*            This function is not available from the CCSP layer.
* @param uint8_t *pcbuf - input the test result
* @param uint32_t confSize - input config buffer size
*
* @return Nothing.
*
* @execution Synchronous.
* @sideeffect None.
*
*/
int32_t verifyChecksumFile(const uint8_t *pcbuf, uint32_t confSize)
{
    uint32_t newCrc = 0;
    uint32_t prevCrc = -1;
    FILE *fpChksum = fopen(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CHECKSUM_NAME, "r");
    if ( NULL == fpChksum)
    {
        CcspTraceInfo(("Could not open checksum file %s\n", VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CHECKSUM_NAME));
        return -1;
    }
    (void)fscanf(fpChksum, "%u", &prevCrc);
    newCrc = checksum(pcbuf, confSize);
    fclose(fpChksum);
    if (newCrc == prevCrc)
    {
        return 0;
    }
    else
    {
        CcspTraceInfo(("Voice config file checksum failed prev %u new %u!\n", prevCrc, newCrc));
        return -1;
    }
}

/* voiceHalInitDmDefaults: */
/**
* @description Sets the initial value of DM configuration items in the voiceApp.
*            This version reads the default JSON file.
*            No point in a return code - if it fails, it fails.
* @param none
*
* @return 0 for success negative value for error
*
* @execution Synchronous.
* @sideeffect Updates data model.
*
*/
static int32_t voiceHalInitDmDefaults()
{
    cJSON *config = NULL, *services = NULL, *voiceService = NULL;
    uint32_t numVoiceServices, defaultDMSize, i, readBytes; void* pJsonConfig = NULL; FILE* fp;
    int ret = 0;

    CcspTraceInfo(("%s:%s \n", __FILE__, __FUNCTION__));
    /* Initialise the cJSON parser with the 'default' set of voice data
     * Find the size of file, allocate memory, read it, parse it.
     * Then the original memory can be deleted
     */
    /* First, get the size of the file for JSON default values */
    /* Open the file in read-only mode */
    if (NULL == (fp = fopen(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME, "r")))
    {
        /* RDK log error and give up */
        CcspTraceInfo(("Failed to open JSON defaults file %s\n",
            VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME));
        return -1;
    }

    fseek(fp, 0, SEEK_END); /*Move file pointer to the end of file.*/

    pJsonConfig = malloc(defaultDMSize = ftell(fp)); /*Get the current position of the file pointer and get some memory.*/

    if (NULL == pJsonConfig)
    {
        CcspTraceInfo(("Failed to get memory for JSON file\n"));
        fclose(fp);  // close the file
        return -2;  // No memory, so give up, special case where we wont revert to factory defaults
    }
    fseek(fp, 0, SEEK_SET);
    if (0 == (readBytes = fread(pJsonConfig, 1, defaultDMSize, fp)))
    {
        CcspTraceInfo(("Failed to read JSON file %s of size %d. Read %d. errno %d \n",
            VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME, defaultDMSize, readBytes, errno));
    }
    fclose(fp);
    ret = verifyChecksumFile(pJsonConfig, defaultDMSize); //calculate checksum value and compare it with a known value
    if (ret)
    {
        free(pJsonConfig);
        return ret;
    }
    if (NULL == (config = cJSON_Parse(pJsonConfig)))
    {
        /*
         * No JSON file, so tidy up and return
         */
        CcspTraceInfo(("Failed to parse JSON file\n"));
        CcspTraceInfo(("Failed to parse JSON file, error is at %p, start is at %p\n", cJSON_GetErrorPtr(), pJsonConfig));
        stopJsonRead();
        free(pJsonConfig);
        return -1;
    }

    free(pJsonConfig);  // Config parsed, so delete the in-memory copy of the file

    /*
     * Inhibit writes to the JSON file while we are reading it.
     */
    startJsonRead();
    jInitMsg = json_hal_client_get_request_header(SET_PARAMETER_METHOD);
    /*
     * Parse the JSON file for the current configuration
     * If an item can't be found, just carry on regardless. It isn't necessarily an error.
     */

    /* device.services.voiceService */
    if (NULL == (services = cJSON_GetObjectItemCaseSensitive(config, "Services")))
    {
        /* Current configuration does not include device.services ?? Give up */
        cJSON_Delete(config);
        stopJsonRead();
        return -1;
    }

    if (NULL != (voiceService = cJSON_GetObjectItemCaseSensitive(services, "VoiceService")))
    {
        numVoiceServices = (uint32_t)cJSON_GetArraySize(voiceService);

        /* device.services.voiceService[] */
        if (numVoiceServices)
        {
            for ( i=0 ; i<numVoiceServices ; i++ )
            {
                jsonParseVoiceService(i, cJSON_GetArrayItem(voiceService, (uint32_t)i));
            }
        }
        else
        {
            CcspTraceInfo(("Device.Services.VoiceServices zero loop!\n"));
            stopJsonRead();
            cJSON_Delete(config);
            return -1;
        }
    }
    else
    {
        /* No device.services.voiceService[] */
        CcspTraceInfo(("No device.Services.VoiceServices found\n"));
        stopJsonRead();
        cJSON_Delete(config);
        return -1;
    }

    CcspTraceInfo(("Parse done successfully\n"));
    /* All done, tidy up */
    stopJsonRead();
    cJSON_Delete(config);
    CcspTraceInfo(("%s: JSON parsed successfully!\n", __FUNCTION__));
    if(TelcoVoiceMgrHal_SendJsonRequest(jInitMsg) == ANSC_STATUS_SUCCESS)
    {
       CcspTraceInfo(("%s: VOICE HAL Initialised successfully with Json defaults !!\n", __FUNCTION__));
    }
    else
    {
       CcspTraceInfo(("%s: VOICE HAL Initialisation failed !!\n", __FUNCTION__));
    }

    return 0;
}

/* This structure makes the connection between VoiceService
 * JSON objects and the voice_hal functions that set them */

static int32_t jsonCfgSetDigitMap(uint32_t service, uint32_t profile, char * map_type, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], map_type [%s], value[%s]", __func__, __LINE__,service,profile,map_type,value);
    memset(&initParam, 0, sizeof(initParam));
    if(!strcmp(map_type, "X_RDK-Central_COM_DigitMap"))
    {
       snprintf(initParam.name, sizeof(initParam), PROFILE_TABLE_NAME"%s", service, profile, "X_RDK-Central_COM_DigitMap");
    }
    else if(!strcmp(map_type, "X_RDK-Central_COM_EmergencyDigitMap"))
    {
       snprintf(initParam.name, sizeof(initParam), PROFILE_TABLE_NAME"%s", service, profile, "X_RDK-Central_COM_EmergencyDigitMap");  
    }
    else if(!strcmp(map_type, "DigitMap"))
    {
       snprintf(initParam.name, sizeof(initParam), PROFILE_TABLE_NAME"%s", service, profile, "DigitMap");
    }
    else
    {
        return -1;
    }

    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetSDigitTimer(uint32_t service, uint32_t profile, uint32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d],value[%d]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), PROFILE_TABLE_NAME"%s", service, profile, "X_RDK-Central_COM_SDigitTimer");
    snprintf(initParam.value, sizeof(initParam.value), "%lu", value);
    initParam.type = PARAM_UNSIGNED_INTEGER;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetZDigitTimer(uint32_t service, uint32_t profile,  uint32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), PROFILE_TABLE_NAME"%s", service, profile, "X_RDK-Central_COM_ZDigitTimer");
    snprintf(initParam.value, sizeof(initParam.value), "%lu", value);
    initParam.type = PARAM_UNSIGNED_INTEGER;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetTestState(uint32_t service, uint32_t phy_interface, char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], phy_interface[%d], value[%s]", __func__, __LINE__,service,phy_interface,value);
    memset(&initParam, 0, sizeof(initParam));
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
#define FIELD_NAME "DiagnosticsState"
#else
#define FIELD_NAME "TestState"
#endif
    snprintf(initParam.name, sizeof(initParam), PHYINTERFACE_TABLE_NAME"%s", service, phy_interface, FIELD_NAME);
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetTestSelector(uint32_t service, uint32_t phy_interface, char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], phy_interface[%d], value[%s]", __func__, __LINE__,service,phy_interface,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), PHYINTERFACE_TABLE_NAME"%s", service, phy_interface, "TestSelector");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetBoundIfName(uint32_t service, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], value[%s]", __func__, __LINE__,service,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), VOICE_SERVICE_TABLE_NAME"%s", service, "X_RDK_BoundIfName");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    TelcoVoiceMgrSetSyseventData(SYSEVENT_UPDATE_IFNAME, value);
    return 0;
}
static int32_t jsonCfgSetIpAddressFamily(uint32_t service, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], value[%s]", __func__, __LINE__,service,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), VOICE_SERVICE_TABLE_NAME"%s", service, "X_RDK_IpAddressFamily");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    TelcoVoiceMgrSetSyseventData(SYSEVENT_UPDATE_IPFAMILY, value);
    return 0;
}

static int32_t jsonCfgDoDisableLoopCurrent(uint32_t service, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d],  value[%s]", __func__, __LINE__,service,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), VOICE_SERVICE_TABLE_NAME"%s", service, "X_RDK_DisableLoopCurrentUntilRegistered");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_BOOLEAN;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}

static struct
{
    char *obj;
    int32_t (*func) (uint32_t, const char *);
    void (*handler) (uint32_t index, cJSON *obj);
} serviceFuncs[] =
{
/* When using multiple VoiceServices, uncomment this line, write handler func that
 * sets the current VoiceService index. For efficiency, keep this as first in the list.
 * Replace references to 'index & index+1 with references to value of VoiceServiceIndex */
/*  { "VoiceServiceIndex", NULL, jsonCheckVoiceServiceIndex } */
    { "X_RDK_BoundIfName", jsonCfgSetBoundIfName, NULL },
    { "X_RDK_IpAddressFamily", jsonCfgSetIpAddressFamily, NULL },
    { "X_RDK_Debug", NULL, jsonParseX_RDK_Debug },
    { "PhyInterface", NULL, jsonParseAllPhyInterfaces },
    { "VoiceProfile", NULL, jsonParseAllVoiceProfiles },
    { "X_RDK_DisableLoopCurrentUntilRegistered", jsonCfgDoDisableLoopCurrent, NULL }
};
#define VOICE_HAL_NUM_ELEMS(x) (sizeof(x)/sizeof(x[0]))

/* jsonParseVoiceService: */
/**
* @description Given a voiceService JSON object, parse the array of voice services.
* @param cJSON *voiceService - pointer to the JSON nth array structure holding values
* @param uint32_t index - array index, voice service[i]
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*
*/
static void jsonParseVoiceService(uint32_t index, cJSON *voiceService)
{
    uint32_t j;
    cJSON *vsItem = NULL;

    /* In theory, we might get a null pointer from the array call. Best check */
    if (NULL == voiceService)
    {
        CcspTraceError(("%s: NULL JSON object found!\n", __FUNCTION__));
        return;
    }

    /* Use the function pointer table to parse each of the elements in the voiceService section */
    for ( j=0 ; j< VOICE_HAL_NUM_ELEMS(serviceFuncs) ; j++)
    {
        if (NULL != (vsItem = cJSON_GetObjectItemCaseSensitive(voiceService, serviceFuncs[j].obj)))
        {
            CcspTraceInfo(("%s: Handling json config item:%s , type: %s\n", __FUNCTION__, serviceFuncs[j].obj,
                                                                      cJSON_IsString(vsItem)?"String":
                                                                       cJSON_IsArray(vsItem)?"Array":
                                                                       cJSON_IsObject(vsItem)?"Object":"Unknown" ));
            if (cJSON_IsString(vsItem))
            {
                if (NULL != vsItem->valuestring)      // Item has a string value, not another object
                    if (NULL != serviceFuncs[j].func)
                        (void)(*serviceFuncs[j].func)(index+1, vsItem->valuestring);
                if (NULL != serviceFuncs[j].handler)
                    /* This item is handled by a function, usually the next level parser */
                    (*serviceFuncs[j].handler)(index, vsItem);
            }
            else if (cJSON_IsArray(vsItem) || cJSON_IsObject(vsItem))
            {
                if (NULL != serviceFuncs[j].handler)
                    /* This item is handled by a function, usually the next level parser */
                    (*serviceFuncs[j].handler)(index, vsItem);
                /* Not a valid item - log error */
            }
            else
            {
                CcspTraceInfo(("Unhandled type tag %s in VoiceService: %s\n",
                    cJSON_IsBool(vsItem) ? "boolean" : cJSON_IsNumber(vsItem) ? "number" :
                    cJSON_IsArray(vsItem) ? "array" : "null/object/raw", serviceFuncs[j].obj));
            }
        }
        else
        {
            /* missing config item? */
        }
    }
}
static int32_t jsonCfgSetCCTKTraceGroup(uint32_t service, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], value[%s]", __func__, __LINE__,service,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), X_RDK_DEBUG_TABLE_NAME"%s", service, "CCTKTraceGroup");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetCCTKTraceLevel(uint32_t service, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d],  value[%s]", __func__, __LINE__,service,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), X_RDK_DEBUG_TABLE_NAME"%s", service, "CCTKTraceLevel");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetModuleLogLevels(uint32_t service, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d],  value[%s]", __func__, __LINE__,service,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), X_RDK_DEBUG_TABLE_NAME"%s", service, "ModuleLogLevels");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetLogServer(uint32_t service, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d],value[%s]", __func__, __LINE__,service, value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), X_RDK_DEBUG_TABLE_NAME"%s", service, "LogServer");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetLogServerPort(uint32_t service, uint32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d],value[%lu]", __func__, __LINE__,service,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), X_RDK_DEBUG_TABLE_NAME"%s", service, "LogServerPort");
    snprintf(initParam.value, sizeof(initParam.value), "%lu", value);
    initParam.type = PARAM_UNSIGNED_INTEGER;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}

/* This structure makes the connection between X_RDK_Debug
 * JSON objects and the voice hal functions that set them */
static struct
{
    char *obj;
    int32_t (*func) ();
    void (*handler) (void);        // For consistency - not used here
} X_RDK_DebugFuncs[] =
{
    { "LogServer", jsonCfgSetLogServer, NULL },
    { "LogServerPort", jsonCfgSetLogServerPort, NULL },
    { "CCTKTraceGroup", jsonCfgSetCCTKTraceGroup, NULL },
    { "CCTKTraceLevel", jsonCfgSetCCTKTraceLevel, NULL },
    { "ModuleLogLevels", jsonCfgSetModuleLogLevels, NULL }
};
/* jsonParseX_RDK_Debug: */
/**
* @description Given a X_RDK_Debug JSON object, parse it
* @param uint32_t service - input the (zero-based) voiceService loop number being parsed
* @param cJSON *X_RDK_Debug - input pointer to the JSON structure holding values
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*/
static void jsonParseX_RDK_Debug(uint32_t service, cJSON *X_RDK_DebugObj)
{
    int i;
    cJSON *vsItem = NULL;

    /* Check for invalid object */
    if (NULL == X_RDK_DebugObj)
    {
        CcspTraceError(("%s: NULL JSON object found!\n", __FUNCTION__));
        return;
    }

    for (i=0 ; i<VOICE_HAL_NUM_ELEMS(X_RDK_DebugFuncs) ; i++)
    {
        if (NULL != (vsItem = cJSON_GetObjectItemCaseSensitive(X_RDK_DebugObj, X_RDK_DebugFuncs[i].obj)))
        {
            if (cJSON_IsString(vsItem))
            {
                if (vsItem->valuestring != NULL)
                {
                    (void)(*X_RDK_DebugFuncs[i].func)(service+1, vsItem->valuestring);
                }
            }
            else if (cJSON_IsNumber(vsItem))
            {
                (void)(*X_RDK_DebugFuncs[i].func)(service+1, vsItem->valueint);
            }
        }
    }
}

/* jsonParseAllPhyInterfaces: */
/**
* @description Given a voiceService JSON object, parse the array of PHY interfaces.
*           Find the number of entries in the array (usually 1) and parse each Phy in turn.
* @param cJSON *phyInterfaces - pointer to the JSON object holding the PHY array
* @param uint32_t voiceService - the index of the voice service loop
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*
*/
static void jsonParseAllPhyInterfaces(uint32_t voiceService, cJSON *phyInterfaces)
{
    int numPhys, k;

    numPhys = cJSON_GetArraySize(phyInterfaces);
    if (numPhys)
    {
        for (k=0 ; k<numPhys ; k++)
        {
            jsonParseOnePhyInterface(voiceService, k, cJSON_GetArrayItem(phyInterfaces, k));
        }
    }
}
/* jsonParseAllVoiceProfiles: */
/**
* @description Given a voiceProfile JSON object, parse the array of voice profiles.
*           Find the number of entries in the array (usually 1) and parse each VoiceProfile in turn.
* @param cJSON *voiceProfiles - pointer to the JSON object holding the VoiceProfile array
* @param uint32_t voiceService - the index of the voice service loop
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*
*/
static void jsonParseAllVoiceProfiles(uint32_t voiceService, cJSON *voiceProfiles)
{
    int numProfiles, k;

    numProfiles = cJSON_GetArraySize(voiceProfiles);
    if (numProfiles)
    {
        for (k=0 ; k<numProfiles ; k++)
        {
            jsonParseOneVoiceProfile(voiceService, k, cJSON_GetArrayItem(voiceProfiles, k));
        }
    }
}
/* This structure makes the connection between PhyInterface
 * JSON objects and the voice_hal functions that set them */
static struct
{
    char *obj;
    int32_t (*func) (uint32_t, uint32_t, char *);
    void (*handler) (void);        // For consistency - not used here
} phyDiagFuncs[] =
{
/* When using multiple PhyInterfaces, uncomment this line, write handler func which
 * updates the current PhyInterface index. For efficiency, keep this as first in the list.
 * Replace references to 'index & index+1 with references to value of PhyInterfaceIndex */
/*  { "PhyInterfaceIndex", NULL, jsonCheckPhyInterfaceIndex } */
    { "TestState", jsonCfgSetTestState, NULL },
    { "TestSelector", jsonCfgSetTestSelector, NULL }
};
/* jsonParseOnePhyInterface: */
/**
* @description Given a phyInterface JSON object, parse it
* @param uint32_t service - input the (zero-based) voiceService loop number being parsed
* @param uint32_t index - input the (zero-based) index of the PhyInterface being parsed
* @param cJSON *phyInterface - input pointer to the JSON structure holding values
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*/
static void jsonParseOnePhyInterface(uint32_t service, uint32_t index, cJSON *phyInterface)
{
    cJSON *vsItem = NULL, *diagTest = NULL;
    int i;

    /* It's possible that we might have a NULL cJSON object */
    if (NULL == phyInterface)
    {
        CcspTraceError(("%s: NULL JSON object found!\n", __FUNCTION__));
        return;
    }

    if (NULL != (diagTest = cJSON_GetObjectItemCaseSensitive(phyInterface, "Tests")))
    {
        for ( i=0 ; i<VOICE_HAL_NUM_ELEMS(phyDiagFuncs) ; i++ )
        {
            if (NULL != (vsItem = cJSON_GetObjectItemCaseSensitive(diagTest, phyDiagFuncs[i].obj)))
            {
                if (cJSON_IsString(vsItem) && (vsItem->valuestring != NULL))
                {
                    (void)(*phyDiagFuncs[i].func)(service+1, index+1, vsItem->valuestring);
                }
            }
        }
    }
}

/* This structure makes the connection between VoiceProfile
 * JSON objects and the voice_hal functions that set them */
enum vpTag {
    VP_TAG_Line,
    VP_TAG_SIP,
    VP_TAG_RTP,
    VP_TAG_DigitMap,
    VP_TAG_EmergencyDigitMap,
    VP_TAG_CustomDigitMap,
    VP_TAG_SDigitTimer,
    VP_TAG_ZDigitTimer
};

static struct
{
    char *obj;
    enum vpTag tag;
    int32_t (*func) ();
} voiceProfileFuncs[] =
{

/* When using multiple VoiceProfiles, uncomment this line, write handler func which aborts if
 * this is not the Voice profile index for which we are looking.
 * For efficiency, keep this as first in the list. Use setjmp()/longjmp() to exit.
 * Replace references to 'index & index+1 with references to value of VoiceProfileIndex */
/*  { "VoiceProfileIndex", NULL, jsonCheckVoiceProfileIndex } */
    { "Line", VP_TAG_Line, jsonParseAllLines },
    { "SIP", VP_TAG_SIP, jsonParseProfileSIP },
    { "RTP", VP_TAG_RTP, jsonParseProfileRTP },
    { "X_RDK-Central_COM_DigitMap", VP_TAG_CustomDigitMap, jsonCfgSetDigitMap },
    { "X_RDK-Central_COM_EmergencyDigitMap", VP_TAG_EmergencyDigitMap, jsonCfgSetDigitMap },
    { "DigitMap", VP_TAG_DigitMap, jsonCfgSetDigitMap },
    { "X_RDK-Central_COM_SDigitTimer", VP_TAG_SDigitTimer, jsonCfgSetSDigitTimer },
    { "X_RDK-Central_COM_ZDigitTimer", VP_TAG_ZDigitTimer, jsonCfgSetZDigitTimer }
};
/* jsonParseOneVoiceProfile: */
/**
* @description Given a voiceProfile JSON object, parse it.
* @param uint32_t service - input the voiceService loop being parsed
* @param uint32_t index - input the number of the voiceProfile loop being processed
* @param cJSON *voiceProfile - pointer to the JSON structure holding values
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*/
static void jsonParseOneVoiceProfile(uint32_t service, uint32_t index, cJSON *voiceProfile)
{
    cJSON *vpItem = NULL;
    uint32_t j;

    /* It's possible that this is called with an invalid cJSON object */
    if (NULL == voiceProfile)
    {
        CcspTraceError(("%s: NULL JSON object found!\n", __FUNCTION__));
        return;
    }

    for ( j=0 ; j<VOICE_HAL_NUM_ELEMS(voiceProfileFuncs) ; j++ )
    {
        if (NULL != (vpItem = cJSON_GetObjectItemCaseSensitive(voiceProfile, voiceProfileFuncs[j].obj)))
        {
            if ((cJSON_IsString(vpItem)) || (cJSON_IsArray(vpItem)))
            {
                int i;
                /* Keep this code in step with the voiceProfileFuncs array !!! */
                /* Note that 'service' and 'index' are zero based in the json parser, 1 based in voice_hal */
                switch (i = voiceProfileFuncs[j].tag)
                {
                case VP_TAG_Line:
                case VP_TAG_SIP:
                case VP_TAG_RTP:
                    /* Parse the next level */
                    (*voiceProfileFuncs[j].func)(service, index, vpItem);
                    break;
                case VP_TAG_DigitMap:
                case VP_TAG_EmergencyDigitMap:
                case VP_TAG_CustomDigitMap:
                    /* Param is an enum and a string ... */
                    (void)(*voiceProfileFuncs[j].func)(service + 1, index + 1,
                        (i == VP_TAG_DigitMap) ? VOICE_HAL_NORMAL_DIGIT_MAP : (i == VP_TAG_EmergencyDigitMap) ? VOICE_HAL_EMERGENCY_DIGIT_MAP :  VOICE_HAL_CUSTOM_DIGIT_MAP,
                         vpItem->valuestring);
                    break;
                default:
                    /* What went wrong?? */
                    CcspTraceInfo(("Unhandled type tag (%d) in voiceProfile string/array object %s\n", i,
                        voiceProfileFuncs[j].obj));
                    break;
                }
            }
            if (cJSON_IsNumber(vpItem))
            {
                int i;
                /* Keep this code in step with the voiceProfileFuncs array !!! */
                /* Note that 'service' and 'index' are zero based in the json parser, 1 based in voice_hal */
                switch (i = voiceProfileFuncs[j].tag)
                {
                case VP_TAG_SDigitTimer:
                    /* Param is an enum and a uint ... */
                    (void)(*voiceProfileFuncs[j].func)(service + 1, index + 1,vpItem->valueint);
                    break;
                case VP_TAG_ZDigitTimer:
                    /* Param is an enum and a uint ... */
                    (void)(*voiceProfileFuncs[j].func)(service + 1, index + 1,vpItem->valueint);
                    break;
                default:
                    /* What went wrong?? */
                    CcspTraceInfo(("Unhandled type tag (%d) in voiceProfile number object %s\n", i,
                        voiceProfileFuncs[j].obj));
                    break;
                }
            }
            if (cJSON_IsObject(vpItem))
            {
                if (VP_TAG_SIP == voiceProfileFuncs[j].tag || VP_TAG_RTP == voiceProfileFuncs[j].tag)
                {
                    (*voiceProfileFuncs[j].func)(service, index, vpItem);
                }
                else
                {
                    CcspTraceInfo(("Unhandled type tag (%d) in voiceProfile next object\n", voiceProfileFuncs[j].tag));
                }
            }
        }
    }
}
/* jsonParseAllLines: */
/**
* @description Given a 'line' JSON object array, prepare to parse it one line at a time
* @param uint32_t service - input the voiceService loop being parsed
* @param uint32_t index - input the number of the voiceProfile loop being processed
* @param uint32_t profile - input the voiceProfile loop being parsed
* @param cJSON *voiceProfile - pointer to the JSON structure holding line array
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return 0 for compatibility with other functions
*
* @execution Synchronous.
* @sideeffect Updates data model.
*/
static int32_t jsonParseAllLines(uint32_t service, uint32_t profile, cJSON *lines)
{
    int numLines, k;

    numLines = cJSON_GetArraySize(lines);
    if (numLines)
    {
        for (k=0 ; k<numLines ; k++)
        {
            jsonParseOneLine(service, profile, k, cJSON_GetArrayItem(lines, k));
        }
    }
    return 0;
}
/* This structure makes the connection between Line
 * JSON objects and the voice_hal functions that set them */

static int32_t jsonCfgSetDirectoryNumber(uint32_t service, uint32_t profile, uint32_t line, char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%s]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(initParam.name, sizeof(initParam), LINE_TABLE_NAME"%s", service, line, "DirectoryNumber");
#else
    snprintf(initParam.name, sizeof(initParam), LINE_TABLE_NAME"%s", service, profile, line, "DirectoryNumber");
#endif
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}

static struct
{
    char *obj;
    int32_t (*func) (uint32_t/* service */, uint32_t /* profile */, uint32_t /* line */, char * /* value */);
    void (*handler) (uint32_t/* service */, uint32_t /* profile */, uint32_t /* line */, cJSON *lineObj);
} lineFuncs[] =
{

/* When using multiple Lines, uncomment this line, write handler func which updates the current Line index.
 * For efficiency, keep this as first in the list.
 * Replace references to 'index & index+1 with references to value of LineIndex */
/*  { "LineIndex", NULL, jsonCheckLineIndex } */
    { "CallingFeatures", NULL, jsonCfgDoCallingFeature },
    { "DirectoryNumber", jsonCfgSetDirectoryNumber, NULL },
    { "Enable", jsonCfgSetLineEnable, NULL },
    { "SIP", NULL, jsonParseLineSIP },
    { "VoiceProcessing", NULL, jsonCfgDoVoiceProcessing }
};
/* jsonParseOneLine: */
/**
* @description Given a single line JSON object, parse it
* @param uint32_t service - the service loop index
* @param uint32_t profile - the profile loop index
* @param uint32_t line - the line loop index
* @param cJSON *voiceProfile - pointer to the JSON line structure holding values
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*/
static void jsonParseOneLine(uint32_t service, uint32_t profile, uint32_t line, cJSON *vLine)
{
    uint32_t i; cJSON *vlItem = NULL;

    for (i=0 ; i<VOICE_HAL_NUM_ELEMS(lineFuncs) ; i++)
    {
        if (NULL != (vlItem = cJSON_GetObjectItemCaseSensitive(vLine, lineFuncs[i].obj)))
        {
            if (cJSON_IsString(vlItem))
            {
                if (vlItem->valuestring != NULL)
                {
                    /* These are voice_hal_XXXX() functions, so correct for zero-based */
                    (void)(*lineFuncs[i].func)(service + 1, profile + 1, line + 1, vlItem->valuestring);
                }
                else
                {
                    if (NULL != lineFuncs[i].handler)
                    {
                        CcspTraceInfo(("jsonParseOneLine: %s: serv = %d, prof = %d, line = %d \n",
                        lineFuncs[i].obj, service, profile, line));
                        /* These are internal handler functions, so DO NOT correct for zero-based */
                        (*lineFuncs[i].handler)(service, profile, line, vlItem);
                    }
                    else
                    {
                        CcspTraceInfo(("jsonParseOneLine string name = %s, about to call NULL func !!!!\n", lineFuncs[i].obj));
                        /* code */
                    }
                }
            }
            if (cJSON_IsObject(vlItem))
            {
                if (NULL != lineFuncs[i].handler)
                {
                    /* These are internal handler functions, so DO NOT correct for zero-based */
                    (*lineFuncs[i].handler)(service, profile, line, vlItem);
                }
                else
                {
                    CcspTraceInfo(("jsonParseOneLine obj name = %s, about to call NULL func !!!!\n", lineFuncs[i].obj));
                    /* code */
                }
            }
        }
    }
}

static int32_t jsonCfgSetAuthCredentials
    (
        uint32_t                service,
        uint32_t                profile,
        uint32_t                line,
        TELCOVOICEMGR_VOICE_CREDENTIAL_TYPE_ENUM   eAuthCredential,
        char*                   value
    )
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], Line[%d] value[%s]", __func__, __LINE__,service,profile,line,value);
    if(eAuthCredential == VOICE_HAL_AUTH_UNAME)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(initParam.name, JSON_MAX_STR_ARR_SIZE, LINE_SIP_TABLE_NAME"%s", service, line, "AuthUserName");
#else
    snprintf(initParam.name, JSON_MAX_STR_ARR_SIZE, LINE_SIP_TABLE_NAME"%s", service, profile, line, "AuthUserName");
#endif
    }
    else if(eAuthCredential == VOICE_HAL_AUTH_PWD)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(initParam.name, JSON_MAX_STR_ARR_SIZE, LINE_SIP_TABLE_NAME"%s", service, line, "AuthPassword");
#else
    snprintf(initParam.name, JSON_MAX_STR_ARR_SIZE, LINE_SIP_TABLE_NAME"%s", service, profile, line, "AuthPassword");
#endif
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}

int32_t jsonDecryptAndSavePassword(uint32_t service, uint32_t profile, uint32_t line, char *valuestring)
{
    char *pOutBuffer = NULL; uint32_t inLen, outLen; int32_t res;
    inLen = strlen(valuestring);
    outLen = (1 + inLen/2);
    /* The decrypted pwd is half the length of the encrypted one plus \0 */
    pOutBuffer = malloc(outLen);

    jsonPwdDecode(valuestring, inLen+1, pOutBuffer, outLen);
    res = jsonCfgSetAuthCredentials(service, profile, line, VOICE_HAL_AUTH_PWD, pOutBuffer);
    free(pOutBuffer);
    return res;
}
/* jsonParseUserName */
/**
* @description A simple utility function that converts from the 4 parameter format used by other SIP
*               profile functions, to the 5 parameters used by voice_hal_setAuthCredentials()
* @param uint32_t service - input the index of the service loop being parsed, usually 1
* @param uint32_t profile - input the index of the profile loop being parsed, usually 1
* @param uint32_t line - input the index of the line loop being parsed, usually 1
* @param char * userName - the user name as read form the JSON file
*
* @return the status of the voice_hal_setXxx() function.
*
* @execution Synchronous.
* @sideeffect Updates data model.
*/
int32_t jsonParseUserName(uint32_t service, uint32_t profile, uint32_t line, char *userName)
{
    return jsonCfgSetAuthCredentials(service, profile, line,
        VOICE_HAL_AUTH_UNAME, userName );
}
/* This structure makes the connection between Line/SIP JSON settings
 * (user/pass/SIPuri) and the voice_hal functions that handle them */

 static int32_t jsonCfgSetSipUri(uint32_t service, uint32_t profile, uint32_t line, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%s]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(initParam.name, sizeof(initParam), LINE_SIP_TABLE_NAME"%s", service, line, "RegisterURI");
#else
    snprintf(initParam.name, sizeof(initParam), LINE_SIP_TABLE_NAME"%s", service, profile, line, "URI");
#endif
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}

static struct
{
    char *obj;
    int32_t (*func) ();
    void (*handler)(void);   // Not used - for consistency
} lineSipFuncs[] =
{
    { "AuthPassword", jsonDecryptAndSavePassword, NULL },
    { "AuthUserName", jsonParseUserName, NULL },
    { "URI", jsonCfgSetSipUri, NULL }
};
/* jsonParseLineSIP: */
/**
* @description Given a Line/SIP JSON object, parse the SIP object and call the functions to set the value
*              Note: Profile/SIP also exists in the data model, and is parsed by jsonParseProfileSIP
* @param uint32_t service - input the index of the service loop being parsed, usually 1
* @param uint32_t profile - input the index of the profile loop being parsed, usually 1
* @param uint32_t line - input the index of the line loop being parsed, usually 1
* @param cJSON *SIP - pointer to the JSON structure holding values
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*/
static void jsonParseLineSIP(uint32_t service, uint32_t profile, uint32_t line, cJSON *vp)
{
    int i;
    cJSON *vsItem = NULL;

    /* Check for invalid object */
    if (NULL == vp)
    {
        CcspTraceError(("%s: NULL JSON object found!\n", __FUNCTION__));
        return;
    }

    for (i=0 ; i<VOICE_HAL_NUM_ELEMS(lineSipFuncs) ; i++)
    {
        if (NULL != (vsItem = cJSON_GetObjectItemCaseSensitive(vp, lineSipFuncs[i].obj)))
        {
            if (cJSON_IsString(vsItem))
            {
                if (vsItem->valuestring != NULL)
                {
                    CcspTraceInfo(("SIP URI/user/pass: type is %d, param value is %s\n", i, vsItem->valuestring));
                    (void)(*lineSipFuncs[i].func)(service+1, profile+1, line+1, vsItem->valuestring);
                }
            }
        }
    }
}

static int32_t jsonCfgSetSipDscpMark(uint32_t service, uint32_t profile, uint32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    TelcoVoiceMgrInitMark(service, profile, value, SIP, PARAM_NAME_DSCP_MARK);
    return 0;
}
static int32_t jsonCfgSetSipEthernetPriorityMark(uint32_t service, uint32_t profile, int32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    TelcoVoiceMgrInitMark(service, profile, value, SIP, PARAM_NAME_ETHERNET_PRIORITY_MARK);

    return 0;
}
static int32_t jsonCfgSetOutboundProxy(uint32_t service, uint32_t profile, const char * value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%s]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "OutboundProxy");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);

    return 0;
}
static int32_t jsonCfgSetOutboundProxyPort(uint32_t service, uint32_t profile, const uint32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "OutboundProxyPort");
    snprintf(initParam.value, sizeof(initParam.value), "%lu", value);
    initParam.type = PARAM_UNSIGNED_INTEGER;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetProxyServer(uint32_t service, uint32_t profile, const char * value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%s]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "ProxyServer");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetProxyServerPort(uint32_t service, uint32_t profile, const uint32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "ProxyServerPort");
    snprintf(initParam.value, sizeof(initParam.value), "%lu", value);
    initParam.type = PARAM_UNSIGNED_INTEGER;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetRegistrarServer(uint32_t service, uint32_t profile, const char * value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%s]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "RegistrarServer");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetRegistrarServerPort(uint32_t service, uint32_t profile, const uint32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "RegistrarServerPort");
    snprintf(initParam.value, sizeof(initParam.value), "%lu", value);
    initParam.type = PARAM_UNSIGNED_INTEGER;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetUserAgentDomain(uint32_t service, uint32_t profile, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%s]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "UserAgentDomain");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetConferencingUri(uint32_t service, uint32_t profile, const char *value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%s]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "X_RDK-Central_COM_ConferencingURI");
    snprintf(initParam.value, sizeof(initParam.value), "%s", value);
    initParam.type = PARAM_STRING;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetEnablePrackRequired(uint32_t service, uint32_t profile, bool value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "X_RDK_PRACKRequired");
    if(value)
    {
       snprintf(initParam.value, sizeof(initParam.value), "%s", "true");
    }
    else
    {
       snprintf(initParam.value, sizeof(initParam.value), "%s", "false");
    }
    initParam.type = PARAM_BOOLEAN;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}
static int32_t jsonCfgSetEnableNetworkDisconnect(uint32_t service, uint32_t profile, bool value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    memset(&initParam, 0, sizeof(initParam));
    snprintf(initParam.name, sizeof(initParam), SIP_TABLE_NAME"%s", service, profile, "X_RDK-Central_COM_NetworkDisconnect");
    if(value)
    {
       snprintf(initParam.value, sizeof(initParam.value), "%s", "true");
    }
    else
    {
       snprintf(initParam.value, sizeof(initParam.value), "%s", "false");
    }
    initParam.type = PARAM_BOOLEAN;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}

/* This structure makes the connection between Profile/SIP JSON
 * settings and the voice_hal functions that handle them */
static struct
{
    char *obj;
    int32_t (*strHandler) (uint32_t/* service */, uint32_t /* profile */, const char * /* value */);
    int32_t (*boolHandler) (uint32_t/* service */, uint32_t /* profile */, bool enable);
    int32_t (*uintHandler) (uint32_t/* service */, uint32_t /* profile */, uint32_t value);
    int32_t (*intHandler) (uint32_t/* service */, uint32_t /* profile */, int32_t value);
} profSipFuncs[] =
{
  { "DSCPMark", NULL, NULL, jsonCfgSetSipDscpMark, NULL },
  { "EthernetPriorityMark", NULL, NULL, NULL, jsonCfgSetSipEthernetPriorityMark },
  { "OutboundProxy", jsonCfgSetOutboundProxy, NULL, NULL },
  { "OutboundProxyPort", NULL, NULL, jsonCfgSetOutboundProxyPort },
  { "ProxyServer", jsonCfgSetProxyServer, NULL, NULL },
  { "ProxyServerPort", NULL, NULL, jsonCfgSetProxyServerPort},
  { "RegistrarServer", jsonCfgSetRegistrarServer, NULL, NULL },
  { "RegistrarServerPort", NULL, NULL, jsonCfgSetRegistrarServerPort },
  { "UserAgentDomain", jsonCfgSetUserAgentDomain, NULL, NULL },
  { "X_RDK-Central_COM_ConferencingURI", jsonCfgSetConferencingUri, NULL, NULL },
  { "X_RDK_PRACKRequired", NULL, jsonCfgSetEnablePrackRequired, NULL },
  { "X_RDK-Central_COM_NetworkDisconnect", NULL, jsonCfgSetEnableNetworkDisconnect, NULL }
};
/* jsonParseProfileSIP: */
/**
* @description Given a Profile/SIP JSON object, parse the SIP object and call the functions to set the value
*              Note: Line/SIP also exists in the data model, and is parsed by jsonParseLineSIP
* @param uint32_t service - input the index of the service loop being parsed, usually 1
* @param uint32_t profile - input the index of the profile loop being parsed, usually 1
* @param cJSON *SIP - pointer to the JSON structure holding values
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return 0 - for compatibility with other functions
*
* @execution Synchronous.
* @sideeffect Updates data model.
*/
static int32_t jsonParseProfileSIP(uint32_t service, uint32_t profile, cJSON *ps)
{
    int i;
    cJSON *psItem = NULL;

    CcspTraceInfo(("%s\n", __FUNCTION__));
    /* Check for invalid object */
    if (NULL == ps)
    {
        CcspTraceError(("%s: NULL JSON object found!\n", __FUNCTION__));
        return 0;
    }

    for (i=0 ; i<VOICE_HAL_NUM_ELEMS(profSipFuncs) ; i++)
    {
        if (NULL != (psItem = cJSON_GetObjectItemCaseSensitive(ps, profSipFuncs[i].obj)))
        {
            if (cJSON_IsString(psItem))
            {
                if (NULL != profSipFuncs[i].boolHandler)
                {
                    /* This object requires special treatment */
                    /* A bool rather than string */
                    (*profSipFuncs[i].boolHandler)(service+1, profile+1, (0 == strcmp("true", psItem->valuestring)) ? true : false);
                }
                else
                {
                    if (psItem->valuestring != NULL)
                    {
                        /* Just a common or garden string */
                        (void)(*profSipFuncs[i].strHandler)(service + 1, profile + 1, psItem->valuestring);
                    }
                    else
                    {
                        /* Something went wrong */
                        CcspTraceInfo(("jsonParseProfileSIP unknown type %s !!!!\n",
                            profSipFuncs[i].obj));
                    }
                }
            }
            if (cJSON_IsNumber(psItem))
            {
                if (NULL != profSipFuncs[i].uintHandler)
                {
                    /* Set a numeric value rather than string */
                    (*profSipFuncs[i].uintHandler)(service+1, profile+1, (uint32_t)psItem->valueint);
                }
                else
                {
                    if (NULL != profSipFuncs[i].intHandler)
                    {
                        (*profSipFuncs[i].intHandler)(service+1, profile+1, (int32_t)psItem->valueint);
                    }
                }
            }
        }
    }
    return 0;
}

static int32_t jsonCfgSetRtpDscpMark(uint32_t service,uint32_t profile,uint32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    TelcoVoiceMgrInitMark(service, profile, value, RTP, PARAM_NAME_DSCP_MARK);
    return 0;
}
static int32_t jsonCfgSetRtpEthernetPriorityMark(uint32_t service,uint32_t profile,int32_t value)
{
    fprintf(stderr,"\n%s(%d) - service[%d], profile[%d], value[%d]", __func__, __LINE__,service,profile,value);
    TelcoVoiceMgrInitMark(service, profile, value, RTP, PARAM_NAME_ETHERNET_PRIORITY_MARK);
    return 0;
}

/* This structure makes the connection between Profile/RTP JSON
 * settings and the voice_hal functions that handle them */
static struct
{
    char *obj;
    int32_t (*uintHandler) (uint32_t/* service */, uint32_t /* profile */, uint32_t value);
    int32_t (*intHandler) (uint32_t/* service */, uint32_t /* profile */, int32_t value);
} profRtpFuncs[] =
{
  { "DSCPMark", jsonCfgSetRtpDscpMark, NULL },
  { "EthernetPriorityMark", NULL, jsonCfgSetRtpEthernetPriorityMark }
};
/* jsonParseProfileRTP: */
/**
* @description Given a Profile/RTP JSON object, parse the RTP object and call the functions to set the value
* @param uint32_t service - input the index of the service loop being parsed, usually 1
* @param uint32_t profile - input the index of the profile loop being parsed, usually 1
* @param cJSON *ps - pointer to the JSON structure holding values
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return 0 - for compatibility with other functions
*
* @execution Synchronous.
* @sideeffect Updates data model.
*/
static int32_t jsonParseProfileRTP(uint32_t service, uint32_t profile, cJSON *ps)
{
    int i;
    cJSON *psItem = NULL;

    CcspTraceInfo(("%s\n", __FUNCTION__));
    /* Check for invalid object */
    if (NULL == ps)
    {
        CcspTraceError(("%s: NULL JSON object found!\n", __FUNCTION__));
        return 0;
    }

    for (i=0 ; i<VOICE_HAL_NUM_ELEMS(profRtpFuncs) ; i++)
    {
        if (NULL != (psItem = cJSON_GetObjectItemCaseSensitive(ps, profRtpFuncs[i].obj)))
        {
            if (cJSON_IsNumber(psItem))
            {
                if (NULL != profRtpFuncs[i].uintHandler)
                {
                    /* Set a numeric value rather than string */
                    (*profRtpFuncs[i].uintHandler)(service+1, profile+1, (uint32_t)psItem->valueint);
                }
                else
                {
                    if (NULL != profRtpFuncs[i].intHandler)
                    {
                        (*profRtpFuncs[i].intHandler)(service+1, profile+1, (int32_t)psItem->valueint);
                    }
                }
            }
        }
    }
    return 0;
}
/***********   Password utility functions  ****************/

/* jsonPwdEncode: */
/**
* @description Encrypt the new password before it is stored in the easily accessible current.json file
*          Note that XOR on a string may produce an early null, leading to truncation, so store as hex
* @param const char *pInBuf input the text string to be encoded
* @param uint32_t inLen input it's length
* @param char *pOutBuf output the encrypted string
* @param uint32_t outLen input the length of this buffer must be at least twice the length of input
*
* @return 0 if OK, -1 if outbuffer too small or bad parameters
*
* @execution Synchronous.
* @sideeffect None.
*/
static char *xorString = "Services.VoiceService.[vs].VoiceProfile.[vp].Line.[li].CallingFeatures.X_RDK-Central_COM_ConferenceCallingEnable";
int jsonPwdEncode(const char *pInBuf, uint32_t inLen, char *pOutBuf, uint32_t outLen)
{
    int i; unsigned char tmp;
    char hex[] = "0123456789ABCDEF";

    if (outLen < (2*inLen+1))
    {
        CcspTraceError(("%s: Output buffer is too small!\n", __FUNCTION__));
        return (-1);   // Output needs to be at least 2 * input
    }
    if ( (NULL == pInBuf) || (NULL == pOutBuf) )
    {
        CcspTraceError(("%s: Null buffers!\n", __FUNCTION__));
        return (-1);
    }
    for (i = 0; i < inLen; i++)
    {
        if ( 0 == (tmp = pInBuf[i]))
            break;      // Stop on null or end of buffer
        tmp ^= xorString[i];
        pOutBuf[(i*2)+0] = hex[(int)(tmp >> 4)];
        pOutBuf[(i*2)+1] = hex[(int)(tmp & 0x0f)];
    }
    pOutBuf[(i*2)] = '\0';
    return 0;
}

/* jsonPwdDecode: */
/**
* @description Decrypt the password before it is passed down to the voice process
*
* @param const char *pInBuf input the text string to be decoded
* @param uint32_t inLen input it's length
* @param char *pOutBuf output the decrypted string
* @param uint32_t outLen input the length of this buffer
*
* @return 0 if OK, -1 if buffers null
*
* @execution Synchronous.
* @sideeffect None.
*/
int jsonPwdDecode(const char *pInBuf, uint32_t inLen, char *pOutBuf, uint32_t outLen)
{
    char tmp, *pOut = NULL, *pKey = xorString;
    const char *pIn = NULL;
    unsigned char result;

    pIn = pInBuf; pOut = pOutBuf;
    if ((NULL == pIn) || (NULL == pOut))
    {
        CcspTraceError(("%s: Null buffers!\n", __FUNCTION__));
        return -1;
    }

    while ((*pIn) && (pOut < (pOutBuf+outLen)))
    {
        tmp = *pIn;
        result = (tmp > '@') ? (tmp - 'A' + 10) : (tmp - '0');
        pIn++;
        tmp = *pIn;
        if (0 == tmp)
        {
            CcspTraceError(("%s: input requires even number of chars!\n", __FUNCTION__));
            return (-1);    // An odd number of hex chars??
        }
        result = (result << 4) | ((tmp > '@') ? (tmp - 'A' + 10) : (tmp - '0'));
        pIn++;
        *pOut = result ^ (*pKey); pOut++; pKey++;
    }
    *pOut = '\0';
    return 0;
}

/* jsonCfgDoVoiceProcessing: */
/**
* @description Given a Line JSON object, parse the Tx, Rx gains
* @param cJSON *lineObj - pointer to the JSON object holding the callingFeatures
* @param uint32_t voiceService - the index of the voice service loop
* @param uint32_t profile - the index of the profile loop
* @param uint32_t line - the index of the line loop
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*
*/

static void jsonCfgDoVoiceProcessing(uint32_t service, uint32_t profile, uint32_t line, cJSON *lineObj)
{
    cJSON *procItem = NULL;
    /* Only 2 objects, so do them in-line */
    if (NULL != (procItem = cJSON_GetObjectItemCaseSensitive(lineObj, "ReceiveGain")))
    {
        if (cJSON_IsNumber(procItem))
        {
            memset(&initParam, 0, sizeof(initParam));
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
            snprintf(initParam.name, sizeof(initParam), LINE_VOICE_PROCESSING_TABLE_NAME"%s", service+1, profile+1, line+1, "ReceiveGain");
#else
            snprintf(initParam.name, sizeof(initParam), LINE_VOICE_PROCESSING_TABLE_NAME"%s", service+1, line+1, "ReceiveGain");
#endif
            snprintf(initParam.value, sizeof(initParam.value), "%d", procItem->valueint);
            initParam.type = PARAM_INTEGER;
            json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
        }
        else
        {
            CcspTraceError((" jsonCfgDoVoiceProcessing invalid Rx format!\n"));
        }

    }
    if (NULL != (procItem = cJSON_GetObjectItemCaseSensitive(lineObj, "TransmitGain")))
    {
        if (cJSON_IsNumber(procItem))
        {
            memset(&initParam, 0, sizeof(initParam));
#ifndef FEATURE_RDKB_VOICE_DM_TR104_V2
            snprintf(initParam.name, sizeof(initParam), LINE_VOICE_PROCESSING_TABLE_NAME"%s", service+1, profile+1, line+1, "TransmitGain");
#else
            snprintf(initParam.name, sizeof(initParam), LINE_VOICE_PROCESSING_TABLE_NAME"%s", service+1, line+1, "TransmitGain");
#endif
            snprintf(initParam.value, sizeof(initParam.value), "%d", procItem->valueint);
            initParam.type = PARAM_INTEGER;
            json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
        }
        else
        {
             CcspTraceError((" jsonCfgDoVoiceProcessing invalid Tx format!\n"));
        }
    }

}

static bool checkBool(bool *out, char *inString)
{
    if ( !strncmp(inString, "true", 4) || !strncmp(inString, "TRUE", 4) )
    {
        *out = true;
        return true;
    }
    if ( !strncmp(inString, "false", 5) || !strncmp(inString, "FALSE", 5) )
    {
        *out = false;
        return true;
    }
    return false;
}

/* jsonCfgDoCallingFeatures: */
/**
* @description Given a Line JSON object, parse the 5 calling feature flags
* @param cJSON *lineObj - pointer to the JSON object holding the callingFeatures
* @param uint32_t voiceService - the index of the voice service loop
* @param uint32_t profile - the index of the profile loop
* @param uint32_t line - the index of the line loop
*              Note that missing or invalid items do not cause a failure -just skip them
*
* @return nothing
*
* @execution Synchronous.
* @sideeffect Updates data model.
*
*/
/*
 * the cfFuncs array makes the connection between DM object
 * strings and the typedefs used in the get/set functions */

static struct
{
    char *obj;
    TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM e;
} cfFuncs[] =
{
    { "CallWaitingEnable",  VOICE_CALLING_FEATURE_CALL_WAITING },
    { "MWIEnable", VOICE_CALLING_FEATURE_MSG_WAIT_INDICATOR },
    { "X_RDK-Central_COM_ConferenceCallingEnable", VOICE_CALLING_FEATURE_CONF_CALL },
    { "X_RDK-Central_COM_HoldEnable", VOICE_CALLING_FEATURE_HOLD },
    { "X_RDK-Central_COM_PhoneCallerIDEnable", VOICE_CALLING_FEATURE_CALLER_ID }
};
void jsonCfgDoCallingFeature(uint32_t service, uint32_t profile, uint32_t line, cJSON *lineObj)
{
    cJSON *cfItem = NULL;
    uint32_t i;
    bool cfValue;

    for (i=0 ; i< VOICE_HAL_NUM_ELEMS(cfFuncs) ; i++)
    {
        if (NULL != (cfItem = cJSON_GetObjectItemCaseSensitive(lineObj, cfFuncs[i].obj)))
        {
            CcspTraceInfo(("jsonCfgDoCallingFeature match \n"));
            if (cJSON_IsString(cfItem))
                if (cfItem->valuestring != NULL)
                {
                    if (checkBool(&cfValue, cfItem->valuestring))
                    {
                        initialise_line_calling_features(service + 1, profile + 1, line + 1, cfFuncs[i].e, cfValue);
                        CcspTraceInfo(("jsonCfgDoCallingFeature found a bool value %s \n", cfItem->valuestring));
                    }
                            else
                    {
                        CcspTraceInfo(("jsonCfgDoCallingFeature not a bool value %s \n", cfItem->valuestring));
                    }

                }
        }
        else
        {
            CcspTraceInfo(("jsonCfgDoCallingFeature no match \n"));
        }

    }
}

static int32_t jsonCfgSetLineEnable(uint32_t service, uint32_t profile, uint32_t line, char* buffer)
{
    if(!buffer)
    {
        CcspTraceInfo(("Invalid buffer \n"));
        return -1;
    }
    memset(&initParam, 0, sizeof(initParam));
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
    snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_TABLE_NAME"%s", service,line,"Enable");
    initParam.type = PARAM_BOOLEAN;
#else
    snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_TABLE_NAME"%s", service,profile,line,"Enable");
    initParam.type = PARAM_STRING;
#endif

    if (!strcmp(buffer, "Enabled"))
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(initParam.value, sizeof(initParam.value), "%s", "true");
#else
        snprintf(initParam.value, sizeof(initParam.value), "%s", "Enabled");
#endif
    }
    else if (!strcmp(buffer, "Disabled"))
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(initParam.value, sizeof(initParam.value), "%s", "false");
#else
        snprintf(initParam.value, sizeof(initParam.value), "%s", "Disabled");
#endif
    }
    else if (!strcmp(buffer, "Quiescent"))
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
        snprintf(initParam.value, sizeof(initParam.value), "%s", "false");
#else
        snprintf(initParam.value, sizeof(initParam.value), "%s", "Quiescent");
#endif
    }
    else
    {
        return -1;
    }

    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return 0;
}

ANSC_STATUS voice_process_factory_default()
{
    ANSC_STATUS returnStatus = ANSC_STATUS_SUCCESS;
    (void)remove(VOICE_CONFIG_CURRENT_PATH VOICE_CONFIG_CURRENT_NAME);
    CcspTraceInfo(("%s %d - sending default configuration to Hal. \n", __FUNCTION__, __LINE__ ));
    returnStatus = TelcoVoiceJsonCfgSetDmDefaults();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        CcspTraceInfo(("%s %d - Error in sending default configuration to Hal. \n", __FUNCTION__, __LINE__ ));
        return returnStatus;
    }
    returnStatus = TelcoVoiceMgrHal_GetInitData();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }
    return returnStatus;
}

static ANSC_STATUS initialise_line_calling_features(uint32_t uiService, uint32_t uiProfile, uint32_t uiLine, TELCOVOICEMGR_VOICE_CALL_FEATURE_TYPE_ENUM eFeature, BOOL bStatus)
{
    memset(&initParam, 0, sizeof(initParam));

    if(eFeature == VOICE_CALLING_FEATURE_CALL_WAITING)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,"CallWaitingEnable");
#else
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,uiLine,"CallWaitingEnable");
#endif
    }
    else if(eFeature == VOICE_CALLING_FEATURE_MSG_WAIT_INDICATOR)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,"MWIEnable");
#else
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,uiLine,"MWIEnable");
#endif
    }
    else if(eFeature == VOICE_CALLING_FEATURE_CONF_CALL)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,"X_RDK-Central_COM_ConferenceCallingEnable");
#else
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,uiLine,"X_RDK-Central_COM_ConferenceCallingEnable");
#endif
    }
    else if(eFeature == VOICE_CALLING_FEATURE_HOLD)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,"X_RDK-Central_COM_HoldEnable");
#else
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,uiLine,"X_RDK-Central_COM_HoldEnable");
#endif
    }
    else if(eFeature == VOICE_CALLING_FEATURE_CALLER_ID)
    {
#ifdef FEATURE_RDKB_VOICE_DM_TR104_V2
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,"X_RDK-Central_COM_PhoneCallerIDEnable");
#else
       snprintf(initParam.name,JSON_MAX_STR_ARR_SIZE, LINE_CALING_FEATURE_TABLE_NAME"%s", uiService,uiProfile,uiLine,"X_RDK-Central_COM_PhoneCallerIDEnable");
#endif
    }
    else
    {
        return ANSC_STATUS_FAILURE;
    }

    if(bStatus)
    {
       snprintf(initParam.value, sizeof(initParam.value),"%s","true");
    }
    else
    {
       snprintf(initParam.value, sizeof(initParam.value),"%s","false");
    }
    
    initParam.type = PARAM_BOOLEAN;
    json_hal_add_param(jInitMsg, SET_REQUEST_MESSAGE, &initParam);
    return ANSC_STATUS_SUCCESS;
}
