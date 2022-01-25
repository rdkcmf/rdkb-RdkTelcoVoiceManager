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

/*********************************************************************************

    description:

        This is the template file of ssp_main.c for XxxxSsp.
        Please replace "XXXX" with your own ssp name with the same up/lower cases.

  ------------------------------------------------------------------------------

    revision:

        09/08/2011    initial revision.

**********************************************************************************/


#ifdef __GNUC__
#ifndef _BUILD_ANDROID
#include <execinfo.h>
#endif
#endif

#include "telcovoicemgr_ssp_global.h"
#include "stdlib.h"
#include "ccsp_dm_api.h"
#define DEBUG_INI_NAME "/etc/debug.ini"
#define _DEBUG 1
#ifdef ENABLE_SD_NOTIFY
#include <systemd/sd-daemon.h>
#endif


#include "webconfig_framework.h"
#ifdef INCLUDE_BREAKPAD
const int kExceptSig[] = {
  SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS, SIGTRAP
};
const int kNumHandSig =
    sizeof(kExceptSig) / sizeof(kExceptSig[0]);
struct sigaction old_handlers[(sizeof(kExceptSig) / sizeof(kExceptSig[0]))];
struct sigaction act;
#endif

extern char* pComponentName;
char g_Subsystem[32]         = {0};
extern ANSC_HANDLE bus_handle;
bool     bTelcoVoiceManagerRunning = TRUE;

int  cmd_dispatch(int  command)
{
    switch ( command )
    {
        case    'e' :

#ifdef _ANSC_LINUX
            CcspTraceInfo(("Connect to bus daemon...\n"));

            {
                char                            CName[256];

                if ( g_Subsystem[0] != 0 )
                {
                    _ansc_sprintf(CName, "%s%s", g_Subsystem, COMPONENT_ID_TELCOVOICEMANAGER);
                }
                else
                {
                    _ansc_sprintf(CName, "%s", COMPONENT_ID_TELCOVOICEMANAGER);
                }

                ssp_Mbi_MessageBusEngage
                    (
                        CName,
                        CCSP_MSG_BUS_CFG,
                        COMPONENT_PATH_VOICEMANAGER
                    );
            }
#endif

            ssp_create();
            ssp_engage();

            break;

        case    'm':

                AnscPrintComponentMemoryTable(pComponentName);

                break;

        case    't':

                AnscTraceMemoryTable();

                break;

        case    'c':

                ssp_cancel();

                break;

        default:
            break;
    }

    return 0;
}

static void _print_stack_backtrace(void)
{
#ifdef __GNUC__
#ifndef _BUILD_ANDROID
    void* tracePtrs[100];
    char** funcNames = NULL;
    int i, count = 0;

    count = backtrace( tracePtrs, 100 );
    backtrace_symbols_fd( tracePtrs, count, 2 );

    funcNames = backtrace_symbols( tracePtrs, count );

    if ( funcNames )
    {
            // Print the stack trace
        for( i = 0; i < count; i++ )
        printf("%s\n", funcNames[i] );

            // Free the string pointers
            free( funcNames );
    }
#endif
#endif
}

#if defined(_ANSC_LINUX)
static void daemonize(void) 
{
    int fd;
    switch (fork())
    {
        case 0:
            break;
        case -1:
            // Error
            CcspTraceInfo(("Error daemonizing (fork)! %d - %s\n", errno, strerror(
                errno)));
            exit(0);
        break;
        default:
            _exit(0);
    }

    if (setsid() < 0)
    {
        CcspTraceInfo(("Error demonizing (setsid)! %d - %s\n", errno, strerror(errno)));
        exit(0);
    }


#ifndef  _DEBUG
    //redirect fd's 0,1,2 to /dev/null
    fd = open("/dev/null", O_RDONLY);
    if (fd != 0) 
    {
        dup2(fd, 0);
        close(fd);
    }
    fd = open("/dev/null", O_WRONLY);
    if (fd != 1) 
    {
        dup2(fd, 1);
        close(fd);
    }
    fd = open("/dev/null", O_WRONLY);
    if (fd != 2) 
    {
        dup2(fd, 2);
        close(fd);
    }
#endif
}

#ifdef INCLUDE_BREAKPAD
void sig_handler(int sig, siginfo_t* info, void* uc)
#else
void sig_handler(int sig)
#endif
{
    if ( sig == SIGUSR1 ) 
    {
      #ifndef INCLUDE_BREAKPAD
        signal(SIGUSR1, sig_handler); /* reset it to this function */
      #endif
        CcspTraceInfo(("SIGUSR1 received!\n"));
    }
    else if ( sig == SIGUSR2 ) 
    {
      #ifndef INCLUDE_BREAKPAD
        signal(SIGUSR2, sig_handler); /* reset it to this function */
      #endif
        CcspTraceInfo(("SIGUSR2 received!\n"));
    }
    else if ( sig == SIGCHLD ) 
    {    
      #ifndef INCLUDE_BREAKPAD
        signal(SIGCHLD, sig_handler); /* reset it to this function */
      #endif
        CcspTraceInfo(("SIGCHLD received!\n"));
    }
    else if ( sig == SIGPIPE ) 
    {
      #ifndef INCLUDE_BREAKPAD
        signal(SIGPIPE, sig_handler); /* reset it to this function */
      #endif
        CcspTraceInfo(("SIGPIPE received!\n"));
    }
    else if ( sig == SIGALRM )
    {
      #ifndef INCLUDE_BREAKPAD
        signal(SIGALRM, sig_handler); /* reset it to this function */
      #endif
        CcspTraceWarning(("SIGALRM received!\n"));
        RDKLogEnable = GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_LoggerEnable");
        RDKLogLevel = (char)GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_LogLevel");
        TELCOVOICEMANAGER_RDKLogLevel = GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_TelcoVOICEManager_LogLevel");
        TELCOVOICEMANAGER_RDKLogEnable = (char)GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_TelcoVOICEManager_LoggerEnable");
        CcspTraceWarning(("RDKLogEnable %d, RDKLogLevel %d, TELCOVOICEMANAGER_RDKLogLevel %d, TELCOVOICEMANAGER_RDKLogEnable %d\n",
                          RDKLogEnable, RDKLogLevel, TELCOVOICEMANAGER_RDKLogLevel, TELCOVOICEMANAGER_RDKLogEnable));
    }
    else 
    {
        /* get stack trace first */
        _print_stack_backtrace();
        CcspTraceWarning(("Signal %d received, exiting!\n", sig));
        bTelcoVoiceManagerRunning = FALSE;
        voicemgr_sysevent_close();
        #ifndef INCLUDE_BREAKPAD
         exit(0);
        #endif
    }
    #ifdef INCLUDE_BREAKPAD
    //allow signal to be processed normally for correct core dump
         CcspTraceInfo(("Restore breakpad signal handlers\n"));
         for (int i = 0; i < kNumHandSig; ++i) {
             if(sig==kExceptSig[i])
             {
             if (sigaction(kExceptSig[i], &old_handlers[i], NULL) == -1) {
                 CcspTraceWarning(("sigaction could not restore the signal\n"));
                 signal(kExceptSig[i], SIG_DFL);
             }
             raise(sig);// (*old_handlers[i].sa_sigaction)(sig,info,uc);
             break;
             }

         }
    #endif
}
#endif



int main(int argc, char* argv[])
{
    ANSC_STATUS                     returnStatus       = ANSC_STATUS_SUCCESS;
    BOOL                            bRunAsDaemon       = TRUE;
    int                             cmdChar            = 0;
    int                             idx = 0;
    char                            cmd[1024]          = {0};
    FILE                           *fd                 = NULL;

    extern ANSC_HANDLE bus_handle;
    char *subSys            = NULL;
    DmErr_t    err;

    for (idx = 1; idx < argc; idx++)
    {
        if ( (strcmp(argv[idx], "-subsys") == 0) )
        {
            AnscCopyString(g_Subsystem, argv[idx+1]);
        }
        else if ( strcmp(argv[idx], "-c") == 0 )
        {
            bRunAsDaemon = FALSE;
        }
    }

    pComponentName          = COMPONENT_NAME_VOICEMANAGER;

#if  defined(_ANSC_WINDOWSNT)

    AnscStartupSocketWrapper(NULL);

    cmd_dispatch('e');

    while ( cmdChar != 'q' )
    {
        cmdChar = getchar();

        cmd_dispatch(cmdChar);
    }
#elif defined(_ANSC_LINUX)
    if ( bRunAsDaemon )
        daemonize();

    fd = fopen("/var/tmp/telcovoice_manager.pid", "w+");
    if ( !fd )
    {
        CcspTraceWarning(("Create /var/tmp/telcovoice_manager.pid error. \n"));
        return 1;
    }
    else
    {
        sprintf(cmd, "%d", getpid());
        fputs(cmd, fd);
        fclose(fd);
    }
#ifdef INCLUDE_BREAKPAD
  //auto constructor will call breakpad_ExceptionHandler 
  //backing up the old signal handlers
  for (int i = 0; i < kNumHandSig; ++i) {
       if (sigaction(kExceptSig[i], NULL, &old_handlers[i]) == -1) {
          CcspTraceWarning(("sigaction failed while trying to fetch existing handlers : %d",kExceptSig[i]));
         }
     }
    memset (&act, 0, sizeof(act));
    act.sa_sigaction = sig_handler;
    act.sa_flags = SA_ONSTACK | SA_SIGINFO | SA_NODEFER;

    sigaction(SIGSEGV, &act, NULL);

    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    sigaction(SIGPIPE, &act, NULL);
    sigaction(SIGBUS, &act, NULL);
    sigaction(SIGKILL, &act, NULL);
    sigaction(SIGFPE, &act, NULL);
    sigaction(SIGILL, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGHUP, &act, NULL);
    sigaction(SIGALRM, &act, NULL);
#else
    signal(SIGSEGV, sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGINT, sig_handler);
    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
    signal(SIGPIPE, sig_handler);
    signal(SIGBUS, sig_handler);
    signal(SIGKILL, sig_handler);
    signal(SIGFPE, sig_handler);
    signal(SIGILL, sig_handler);
    signal(SIGQUIT, sig_handler);
    signal(SIGHUP, sig_handler);
    signal(SIGALRM, sig_handler);

#endif

    cmd_dispatch('e');
#ifdef _COSA_SIM_
    subSys = "";        /* PC simu use empty string as subsystem */
#else
    subSys = NULL;      /* use default sub-system */
#endif
    err = Cdm_Init(bus_handle, subSys, NULL, NULL, pComponentName);
    if (err != CCSP_SUCCESS)
    {
        fprintf(stderr, "Cdm_Init: %s\n", Cdm_StrError(err));
        exit(1);
    }

    //Needs to configure initial logger setup
    RDKLogEnable = GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_LoggerEnable");
    RDKLogLevel = (char)GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_LogLevel");
    TELCOVOICEMANAGER_RDKLogLevel = GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_TelcoVOICEManager_LogLevel");
    TELCOVOICEMANAGER_RDKLogEnable = (char)GetLogInfo(bus_handle,"eRT.","Device.LogAgent.X_RDKCENTRAL-COM_TelcoVOICEManager_LoggerEnable");
    CcspTraceWarning(("RDKLogEnable %d, RDKLogLevel %d, TELCOVOICEMANAGER_RDKLogLevel %d, TELCOVOICEMANAGER_RDKLogEnable %d\n",
                          RDKLogEnable, RDKLogLevel, TELCOVOICEMANAGER_RDKLogLevel, TELCOVOICEMANAGER_RDKLogEnable));

    //rdk_logger_init("/fss/gw/lib/debug.ini");
    rdk_logger_init(DEBUG_INI_NAME);
    if (syscfg_init() < 0)
    {
        return 1;
    }
    voicemgr_sysevent_init();
    voicemgr_create_nw_monitor();

#ifdef ENABLE_SD_NOTIFY
    sd_notifyf(0, "READY=1\n"
              "STATUS=telcovoice_manager is Successfully Initialized\n"
              "MAINPID=%lu", (unsigned long) getpid());

    CcspTraceInfo(("RDKB_SYSTEM_BOOT_UP_LOG : telcovoice_manager sd_notify Called\n"));
#endif

    /* Inform Webconfig framework if component is coming after crash */
    check_component_crash("/tmp/telcovoicemgr_initialized");

    /* Creating file after initialization to indicate manager is up and running */
    if ((fd = fopen ("/tmp/telcovoicemgr_initialized", "w+")) != NULL) {
        fprintf(fd,"1");
        fclose(fd);
    }

    CcspTraceInfo(("RDKB_SYSTEM_BOOT_UP_LOG : TelcoVoiceMgr Initialized Successfully\n"));

    if ( bRunAsDaemon )
    {
        while(1)
        {
            if (!bTelcoVoiceManagerRunning)
            {
                break;
            }
            sleep(30);
        }
    }
    else
    {
        while ( cmdChar != 'q' )
        {
            cmdChar = getchar();

            cmd_dispatch(cmdChar);
        }
    }

#endif
    err = Cdm_Term();
    if (err != CCSP_SUCCESS)
    {
        fprintf(stderr, "Cdm_Term: %s\n", Cdm_StrError(err));
        exit(1);
    }

    ssp_cancel();
    voicemgr_sysevent_close();
    /*Wait 10 seconds to close voice process completely*/
    sleep(10);
    return 0;
}

