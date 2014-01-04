#include "servicebase.h"
#include <assert.h>
#include <strsafe.h>
#include <cstdio>

ServiceBase *ServiceBase::service = NULL;

//   PURPOSE: Register the executable for a service with the Service Control
//   Manager (SCM). After you call Run(ServiceBase), the SCM issues a Start
//   command, which results in a call to the onStart method in the service.
//   This method blocks until the service has stopped.
//
//   PARAMETERS:
//   * service - the reference to a ServiceBase object. It will become the
//     singleton service instance of this service application.
//
BOOL ServiceBase::run(ServiceBase &newService)
{
    service = &newService;

    SERVICE_TABLE_ENTRY serviceTable[] =
    {
        { service->name, serviceMain },
        { NULL, NULL }
    };

    // Connects the main thread of a service process to the service control
    // manager, which causes the thread to be the service control dispatcher
    // thread for the calling process. This call returns when the service has
    // stopped. The process should simply terminate when the call returns.
    return StartServiceCtrlDispatcher(serviceTable);
}

//   PURPOSE: Entry point for the service. It registers the handler function
//   for the service and starts the service.
//
//   PARAMETERS:
//   * dwArgc   - number of command line arguments
//   * lpszArgv - array of command line arguments
//
void WINAPI ServiceBase::serviceMain(DWORD dwArgc, PWSTR *pszArgv)
{
    assert(service != NULL);

    // Register the handler function for the service
    service->statusHandle = RegisterServiceCtrlHandler(
        service->name, serviceCtrlHandler);
    if (service->statusHandle == NULL)
    {
        throw GetLastError();
    }

    service->start(dwArgc, pszArgv);
}

//   PURPOSE: The function is called by the SCM whenever a control code is
//   sent to the service.
//
//   PARAMETERS:
//   * dwCtrlCode - the control code. This parameter can be one of the
//   following values:
//
//     SERVICE_CONTROL_CONTINUE
//     SERVICE_CONTROL_INTERROGATE
//     SERVICE_CONTROL_NETBINDADD
//     SERVICE_CONTROL_NETBINDDISABLE
//     SERVICE_CONTROL_NETBINDREMOVE
//     SERVICE_CONTROL_PARAMCHANGE
//     SERVICE_CONTROL_PAUSE
//     SERVICE_CONTROL_SHUTDOWN
//     SERVICE_CONTROL_STOP
//
//   This parameter can also be a user-defined control code ranges from 128
//   to 255.
//
void WINAPI ServiceBase::serviceCtrlHandler(DWORD dwCtrl)
{
    switch (dwCtrl)
    {
      case SERVICE_CONTROL_STOP: service->stop(); break;
      case SERVICE_CONTROL_PAUSE: service->pause(); break;
      case SERVICE_CONTROL_CONTINUE: service->resume(); break;
      case SERVICE_CONTROL_SHUTDOWN: service->shutdown(); break;
      case SERVICE_CONTROL_INTERROGATE: break;
      default: break;
    }
}

//   PURPOSE: The constructor of ServiceBase. It initializes a new instance
//   of the ServiceBase class.
//
//   PARAMETERS:
//   * pszServiceName - the name of the service
//   * fCanStop - the service can be stopped
//   * fCanShutdown - the service is notified when system shutdown occurs
//   * fCanPauseContinue - the service can be paused and continued
//
ServiceBase::ServiceBase(PWSTR pszServiceName,
                         BOOL fCanStop,
                         BOOL fCanShutdown,
                         BOOL fCanPauseContinue)
{
    // Service name must be a valid string and cannot be NULL.
    name = (pszServiceName == NULL) ? L"" : pszServiceName;

    statusHandle = NULL;

    // The service runs in its own process.
    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;

    // The service is starting.
    status.dwCurrentState = SERVICE_START_PENDING;

    // The accepted commands of the service.
    DWORD dwControlsAccepted = 0;
    if(fCanStop)
        dwControlsAccepted |= SERVICE_ACCEPT_STOP;
    if(fCanShutdown)
        dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
    if(fCanPauseContinue)
        dwControlsAccepted |= SERVICE_ACCEPT_PAUSE_CONTINUE;
    status.dwControlsAccepted = dwControlsAccepted;

    status.dwWin32ExitCode = NO_ERROR;
    status.dwServiceSpecificExitCode = 0;
    status.dwCheckPoint = 0;
    status.dwWaitHint = 0;
}

ServiceBase::~ServiceBase()
{
}

//   PURPOSE: Install the current application as a service to the local
//   service control manager database.
//
//   PARAMETERS:
//   * pszServiceName - the name of the service to be installed
//   * pszDisplayName - the display name of the service
//   * dwStartType - the service start option. This parameter can be one of
//     the following values: SERVICE_AUTO_START, SERVICE_BOOT_START,
//     SERVICE_DEMAND_START, SERVICE_DISABLED, SERVICE_SYSTEM_START.
//   * pszDependencies - a pointer to a double null-terminated array of null-
//     separated names of services or load ordering groups that the system
//     must start before this service.
//   * pszAccount - the name of the account under which the service runs.
//   * pszPassword - the password to the account name.
//
//   NOTE: If the function fails to install the service, it prints the error
//   in the standard output stream for users to diagnose the problem.
//
void ServiceBase::install(PWSTR pszServiceName,
                          PWSTR pszDisplayName,
                          DWORD dwStartType,
                          PWSTR pszDependencies,
                          PWSTR pszAccount,
                          PWSTR pszPassword)
{
  wchar_t szPath[MAX_PATH];
  SC_HANDLE schSCManager = NULL;
  SC_HANDLE schService = NULL;

  if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)) == 0)
  {
    wprintf(L"GetModuleFileName failed w/err 0x%08lx\n", GetLastError());
    goto Cleanup;
  }

  // Open the local default service control manager database
  schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT |
                               SC_MANAGER_CREATE_SERVICE);
  if (schSCManager == NULL)
  {
    wprintf(L"OpenSCManager failed w/err 0x%08lx\n", GetLastError());
    goto Cleanup;
  }

  // Install the service into SCM by calling CreateService
  schService = CreateService(
                 schSCManager,                   // SCManager database
                 pszServiceName,                 // Name of service
                 pszDisplayName,                 // Name to display
                 SERVICE_QUERY_STATUS,           // Desired access
                 SERVICE_WIN32_OWN_PROCESS,      // Service type
                 dwStartType,                    // Service start type
                 SERVICE_ERROR_NORMAL,           // Error control type
                 szPath,                         // Service's binary
                 NULL,                           // No load ordering group
                 NULL,                           // No tag identifier
                 pszDependencies,                // Dependencies
                 pszAccount,                     // Service running account
                 pszPassword                     // Password of the account
                 );
  if (schService == NULL)
  {
    wprintf(L"CreateService failed w/err 0x%08lx\n", GetLastError());
    goto Cleanup;
  }

  wprintf(L"%s is installed.\n", pszServiceName);

Cleanup:
  // Centralized cleanup for all allocated resources.
  if (schSCManager)
  {
    CloseServiceHandle(schSCManager);
    schSCManager = NULL;
  }
  if (schService)
  {
    CloseServiceHandle(schService);
    schService = NULL;
  }

}

//   PURPOSE: Stop and remove the service from the local service control
//   manager database.
//
//   PARAMETERS:
//   * pszServiceName - the name of the service to be removed.
//
//   NOTE: If the function fails to uninstall the service, it prints the
//   error in the standard output stream for users to diagnose the problem.
//
void ServiceBase::uninstall(PWSTR pszServiceName)
{
  SC_HANDLE schSCManager = NULL;
  SC_HANDLE schService = NULL;
  SERVICE_STATUS ssSvcStatus = {};

  // Open the local default service control manager database
  schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
  if (schSCManager == NULL)
  {
    wprintf(L"OpenSCManager failed w/err 0x%08lx\n", GetLastError());
    goto Cleanup;
  }

  // Open the service with delete, stop, and query status permissions
  schService = OpenService(schSCManager, pszServiceName, SERVICE_STOP |
                           SERVICE_QUERY_STATUS | DELETE);
  if (schService == NULL)
  {
    wprintf(L"OpenService failed w/err 0x%08lx\n", GetLastError());
    goto Cleanup;
  }

  // Try to stop the service
  if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus))
  {
    wprintf(L"Stopping %s.", pszServiceName);
    Sleep(1000);

    while (QueryServiceStatus(schService, &ssSvcStatus))
    {
      if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING)
      {
        wprintf(L".");
        Sleep(1000);
      }
      else break;
    }

    if (ssSvcStatus.dwCurrentState == SERVICE_STOPPED)
    {
      wprintf(L"\n%s is stopped.\n", pszServiceName);
    }
    else
    {
      wprintf(L"\n%s failed to stop.\n", pszServiceName);
    }
  }

  // Now remove the service by calling DeleteService.
  if (!DeleteService(schService))
  {
    wprintf(L"DeleteService failed w/err 0x%08lx\n", GetLastError());
    goto Cleanup;
  }

  wprintf(L"%s is removed.\n", pszServiceName);

Cleanup:
  // Centralized cleanup for all allocated resources.
  if (schSCManager)
  {
    CloseServiceHandle(schSCManager);
    schSCManager = NULL;
  }
  if (schService)
  {
    CloseServiceHandle(schService);
    schService = NULL;
  }
}

//
//   PURPOSE: The function starts the service. It calls the onStart virtual
//   function in which you can specify the actions to take when the service
//   starts. If an error occurs during the startup, the error will be logged
//   in the Application event log, and the service will be stopped.
//
//   PARAMETERS:
//   * dwArgc   - number of command line arguments
//   * lpszArgv - array of command line arguments
//
void ServiceBase::start(DWORD dwArgc, PWSTR *pszArgv)
{
    try
    {
        setServiceStatus(SERVICE_START_PENDING);
        // Perform service-specific initialization.
        onStart(dwArgc, pszArgv);
        setServiceStatus(SERVICE_RUNNING);
    }
    catch (DWORD dwError)
    {
        logError(L"Service Start", dwError);
        setServiceStatus(SERVICE_STOPPED, dwError);
    }
    catch (...)
    {
        logError(L"Service failed to start.", EVENTLOG_ERROR_TYPE);
        setServiceStatus(SERVICE_STOPPED);
    }
}


//   PURPOSE: When implemented in a derived class, executes when a start
//   command is sent to the service by the SCM or when the operating system
//   starts (for a service that starts automatically). Specifies actions to
//   take when the service starts. Be sure to periodically call
//   ServiceBase::setServiceStatus() with SERVICE_START_PENDING if the
//   procedure is going to take long time. You may also consider spawning a
//   new thread in OnStart to perform time-consuming initialization tasks.
//
//   PARAMETERS:
//   * dwArgc   - number of command line arguments
//   * lpszArgv - array of command line arguments
//
void ServiceBase::onStart(DWORD dwArgc, PWSTR *pszArgv)
{
}

//   PURPOSE: The function stops the service. It calls the onStop virtual
//   function in which you can specify the actions to take when the service
//   stops. If an error occurs, the error will be logged in the Application
//   event log, and the service will be restored to the original state.
//
void ServiceBase::stop()
{
    DWORD dwOriginalState = status.dwCurrentState;
    try
    {
        setServiceStatus(SERVICE_STOP_PENDING);
        // Perform service-specific stop operations.
        onStop();
        setServiceStatus(SERVICE_STOPPED);
    }
    catch (DWORD dwError)
    {
        logError(L"Service Stop", dwError);
        setServiceStatus(dwOriginalState);
    }
    catch (...)
    {
        logError(L"Service failed to stop.", EVENTLOG_ERROR_TYPE);
        setServiceStatus(dwOriginalState);
    }
}


//   PURPOSE: When implemented in a derived class, executes when a stop
//   command is sent to the service by the SCM. Specifies actions to take
//   when a service stops running. Be sure to periodically call
//   CerviceBase::SetServiceStatus() with SERVICE_STOP_PENDING if the
//   procedure is going to take long time.
//
void ServiceBase::onStop()
{
}

//   PURPOSE: The function pauses the service if the service supports pause
//   and continue. It calls the OnPause virtual function in which you can
//   specify the actions to take when the service pauses. If an error occurs,
//   the error will be logged in the Application event log, and the service
//   will become running.
//
void ServiceBase::pause()
{
    try
    {
        setServiceStatus(SERVICE_PAUSE_PENDING);
        // Perform service-specific pause operations.
        onPause();
        setServiceStatus(SERVICE_PAUSED);
    }
    catch (DWORD dwError)
    {
        logError(L"Service Pause", dwError);
        setServiceStatus(SERVICE_RUNNING);
    }
    catch (...)
    {
        logError(L"Service failed to pause.", EVENTLOG_ERROR_TYPE);
        setServiceStatus(SERVICE_RUNNING);
    }
}

//   PURPOSE: When implemented in a derived class, executes when a Pause
//   command is sent to the service by the SCM. Specifies actions to take
//   when a service pauses.
//
void ServiceBase::onPause()
{
}

//   PURPOSE: The function resumes normal functioning after being paused if
//   the service supports pause and continue. It calls the OnContinue virtual
//   function in which you can specify the actions to take when the service
//   continues. If an error occurs, the error will be logged in the
//   Application event log, and the service will still be paused.
//
void ServiceBase::resume()
{
    try
    {
        setServiceStatus(SERVICE_CONTINUE_PENDING);
        // Perform service-specific continue operations.
        onResume();
        setServiceStatus(SERVICE_RUNNING);
    }
    catch (DWORD dwError)
    {
        logError(L"Service Continue", dwError);
        setServiceStatus(SERVICE_PAUSED);
    }
    catch (...)
    {
        logError(L"Service failed to resume.", EVENTLOG_ERROR_TYPE);
        setServiceStatus(SERVICE_PAUSED);
    }
}

//   PURPOSE: When implemented in a derived class, OnContinue runs when a
//   Continue command is sent to the service by the SCM. Specifies actions to
//   take when a service resumes normal functioning after being paused.
//
void ServiceBase::onResume()
{
}

//   PURPOSE: The function executes when the system is shutting down. It
//   calls the OnShutdown virtual function in which you can specify what
//   should occur immediately prior to the system shutting down. If an error
//   occurs, the error will be logged in the Application event log.
//
void ServiceBase::shutdown()
{
    try
    {
        onShutdown();
        setServiceStatus(SERVICE_STOPPED);
    }
    catch (DWORD dwError)
    {
        logError(L"Service Shutdown", dwError);
    }
    catch (...)
    {
        logError(L"Service failed to shut down.", EVENTLOG_ERROR_TYPE);
    }
}

//   PURPOSE: When implemented in a derived class, executes when the system
//   is shutting down. Specifies what should occur immediately prior to the
//   system shutting down.
//
void ServiceBase::onShutdown()
{
}

//   PURPOSE: The function sets the service status and reports the status to
//   the SCM.
//
//   PARAMETERS:
//   * dwCurrentState - the state of the service
//   * dwWin32ExitCode - error code to report
//   * dwWaitHint - estimated time for pending operation, in milliseconds
//
void ServiceBase::setServiceStatus(DWORD dwCurrentState,
                                   DWORD dwWin32ExitCode,
                                   DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    status.dwCurrentState = dwCurrentState;
    status.dwWin32ExitCode = dwWin32ExitCode;
    status.dwWaitHint = dwWaitHint;

    status.dwCheckPoint =
        ((dwCurrentState == SERVICE_RUNNING) ||
        (dwCurrentState == SERVICE_STOPPED)) ?
        0 : dwCheckPoint++;

    // Report the status of the service to the SCM.
    ::SetServiceStatus(statusHandle, &status);
}


//   PURPOSE: Log a message to the Application event log.
//
//   PARAMETERS:
//   * pszMessage - string message to be logged.
//   * wType - the type of event to be logged. The parameter can be one of
//     the following values.
//
//     EVENTLOG_SUCCESS
//     EVENTLOG_AUDIT_FAILURE
//     EVENTLOG_AUDIT_SUCCESS
//     EVENTLOG_ERROR_TYPE
//     EVENTLOG_INFORMATION_TYPE
//     EVENTLOG_WARNING_TYPE
//
void ServiceBase::logEvent(PWSTR pszMessage, WORD wType)
{
    HANDLE hEventSource = NULL;
    LPCWSTR lpszStrings[2] = { NULL, NULL };

    hEventSource = RegisterEventSource(NULL, name);
    if (hEventSource)
    {
        lpszStrings[0] = name;
        lpszStrings[1] = pszMessage;

        ReportEvent(hEventSource,  // Event log handle
            wType,                 // Event type
            0,                     // Event category
            0,                     // Event identifier
            NULL,                  // No security identifier
            2,                     // Size of lpszStrings array
            0,                     // No binary data
            lpszStrings,           // Array of strings
            NULL                   // No binary data
            );

        DeregisterEventSource(hEventSource);
    }
}

//   PURPOSE: Log an error message to the Application event log.
//
//   PARAMETERS:
//   * pszFunction - the function that gives the error
//   * dwError - the error code
//
void ServiceBase::logError(PWSTR pszFunction, DWORD dwError)
{
    wchar_t szMessage[260];
    StringCchPrintf(szMessage, ARRAYSIZE(szMessage),
        L"%s failed w/err 0x%08lx", pszFunction, dwError);
    logEvent(szMessage, EVENTLOG_ERROR_TYPE);
}
