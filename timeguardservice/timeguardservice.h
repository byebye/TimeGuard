#ifndef TIMEGUARDSERVICE_H
#define TIMEGUARDSERVICE_H

#include "servicebase.h"
#include <thread>

class TimeGuardService : public ServiceBase
{
public:
  TimeGuardService(PWSTR pszServiceName,
                   BOOL fCanStop = TRUE,
                   BOOL fCanShutdown = TRUE,
                   BOOL fCanPauseContinue = FALSE);
  virtual ~TimeGuardService();

protected:
  virtual void onStart(DWORD dwArgc, PWSTR *pszArgv);
  virtual void onStop();
  virtual void onPause();
  virtual void onResume();

  void serviceThreadWorker();

  std::thread serviceThread;
  bool isRunning;
};

#endif // TIMEGUARDSERVICE_H
