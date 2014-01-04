//#include <QCoreApplication>
#include "timeguardservice.h"

int main(int argc, char *argv[])
{
  if ((argc > 1) && ((*argv[1] == '-' || (*argv[1] == '/'))))
  {
    if (_stricmp("install", argv[1] + 1) == 0)
      ServiceBase::install(L"TimeGuardService",
                           L"TimeGuardService - early version",
                           SERVICE_DEMAND_START,          // Service start type
                           L"",                           // Dependencies
                           L"NT AUTHORITY\\LocalService", // Service running account
                           NULL                           // Password of the account
                           );
    else if (_stricmp("uninstall", argv[1] + 1) == 0)
      ServiceBase::uninstall(L"TimeGuardService");
    else
      printf("Parameters:\n"
             "\t-install          to install the service.\n"
             "\t-uninstall        to uninstall the service.\n"
             "\twithout arguments to run the service.\n");
  }
  else
  {
    TimeGuardService timeGuardService(L"TimeGuardService");
    if(!ServiceBase::run(timeGuardService))
      printf("Service failed to run w/err 0x%08lx\n", GetLastError());
    else
      printf("Service started successfully\n");
  }

  return 0;
}
