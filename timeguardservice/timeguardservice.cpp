#include "timeguardservice.h"
#include <fstream>

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
  logEvent(L"TimeGuardServie is working", EVENTLOG_INFORMATION_TYPE);


  std::ofstream fileStream("D:\\Programy\\TimeGuard\\timeguardservice.log");

  fileStream << "Service started working\n";

  int counter = 0;
  while(isRunning)
  {
    fileStream << counter << std::endl;
    ++counter;
    if(counter >= 100)
      break;
    ::Sleep(1000);
  }
  fileStream << counter << std::endl;
  fileStream << "Service ended working\n";
  // else error opening file

  fileStream.close();
}
