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
//  char buffer[MAX_PATH];
//  GetCurrentDirectory(MAX_PATH, (LPWSTR) buffer);

  logEvent(L"TimeGuardServie is working", EVENTLOG_INFORMATION_TYPE);
//  logEvent((PWSTR)buffer, EVENTLOG_INFORMATION_TYPE);

  std::ofstream fileStream("D:\\timeguardservice.log");
//  if(fileStream.fail())
//  {
//    logEvent((PWSTR) strerror(errno), EVENTLOG_INFORMATION_TYPE);
//    stream << strerror(errno) << std::endl;
//    stream.close();
//  }

  fileStream << "LOG" << std::endl;
  int counter = 0;
  while(isRunning)
  {
    fileStream << counter << std::endl;
    ++counter;
    ::Sleep(1000);
  }
  fileStream.close();
  // else error opening file
}
