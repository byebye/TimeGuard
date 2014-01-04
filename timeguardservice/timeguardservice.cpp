#include "timeguardservice.h"

TimeGuardService::TimeGuardService(PWSTR pszServiceName,
                                   BOOL fCanStop,
                                   BOOL fCanShutdown,
                                   BOOL fCanPauseContinue)
  : ServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
}

TimeGuardService::~TimeGuardService()
{
  if(serviceThread.joinable())
    serviceThread.detach();
}

void TimeGuardService::onStart(DWORD dwArgc, PWSTR *pszArgv)
{
  logEvent(L"TimeGuardServie is being started", EVENTLOG_INFORMATION_TYPE);

  serviceThread = std::thread(&TimeGuardService::serviceThreadWorker, this);
}

void TimeGuardService::onStop()
{
  logEvent(L"TimeGuardServie is being stopped", EVENTLOG_INFORMATION_TYPE);
}

void TimeGuardService::onPause()
{
  logEvent(L"TimeGuardServie is being paused", EVENTLOG_INFORMATION_TYPE);
}

void TimeGuardService::onResume()
{
  logEvent(L"TimeGuardServie is being resumed", EVENTLOG_INFORMATION_TYPE);
}

void TimeGuardService::serviceThreadWorker()
{

}
