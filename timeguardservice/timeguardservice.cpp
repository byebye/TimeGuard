#include "timeguardservice.h"

TimeGuardService::TimeGuardService(PWSTR pszServiceName,
                                   BOOL fCanStop,
                                   BOOL fCanShutdown,
                                   BOOL fCanPauseContinue)
  : ServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
  isRunning = false;
}

TimeGuardService::~TimeGuardService()
{
  if(serviceThread.joinable())
    serviceThread.detach();
}

void TimeGuardService::onStart(DWORD dwArgc, PWSTR *pszArgv)
{
  logEvent(L"TimeGuardServie is being started", EVENTLOG_INFORMATION_TYPE);

  isRunning = true;
  serviceThread = std::thread(&TimeGuardService::serviceThreadWorker, this);
}

void TimeGuardService::onStop()
{
  logEvent(L"TimeGuardServie is being stopped", EVENTLOG_INFORMATION_TYPE);

  isRunning = false;
}

void TimeGuardService::onPause()
{
  logEvent(L"TimeGuardServie is being paused", EVENTLOG_INFORMATION_TYPE);

  isRunning = false;
}

void TimeGuardService::onResume()
{
  logEvent(L"TimeGuardServie is being resumed", EVENTLOG_INFORMATION_TYPE);

  isRunning = true;
}

void TimeGuardService::serviceThreadWorker()
{
  while(isRunning)
  {
    // do some work
  }
}
