#ifndef SERVICEBASE_H
#define SERVICEBASE_H

#include <windows.h>

class ServiceBase
{
public:
    static BOOL run(ServiceBase &newService);

    ServiceBase(PWSTR pszServiceName,
                BOOL fCanStop = TRUE,
                BOOL fCanShutdown = TRUE,
                BOOL fCanPauseContinue = FALSE);

    virtual ~ServiceBase();

    void stop();

    static void install(PWSTR pszServiceName,
                        PWSTR pszDisplayName,
                        DWORD dwStartType,
                        PWSTR pszDependencies,
                        PWSTR pszAccount,
                        PWSTR pszPassword);
    static void uninistall(PWSTR pszServiceName);

protected:
    virtual void onStart(DWORD dwArgc, PWSTR *pszArgv);
    virtual void onStop();
    virtual void onPause();
    virtual void onResume();
    virtual void onShutdown();

    void setServiceStatus(DWORD dwCurrentState,
        DWORD dwWin32ExitCode = NO_ERROR,
        DWORD dwWaitHint = 0);

    void logEvent(PWSTR pszMessage, WORD wType);
    void logError(PWSTR pszFunction, DWORD dwError = GetLastError());

private:

    // Entry point for the service. It registers the handler function for the
    // service and starts the service.
    static void WINAPI serviceMain(DWORD dwArgc, LPWSTR *lpszArgv);

    // The function is called by the SCM whenever a control code is sent to
    // the service.
    static void WINAPI serviceCtrlHandler(DWORD dwCtrl);

    void start(DWORD dwArgc, PWSTR *pszArgv);
    void pause();
    void resume();
    void shutdown();

    static ServiceBase *service;
    PWSTR name;
    SERVICE_STATUS status;
    SERVICE_STATUS_HANDLE statusHandle;
};

#endif // SERVICEBASE_H
