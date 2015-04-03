#include "timeguardservice.h"

TimeGuardService::TimeGuardService(int arg, char **argv)
   : QtService<QCoreApplication>(argc, argv, "TimeGuard Service")
{
   setServiceDescription(tr("TimeGuard service controlling time spent on the computer"));
   setServiceFlags(QtServiceBase::CanBeSuspended);
}

TimeGuardService::~TimeGuardService()
{

}

void TimeGuardService::start()
{

}

void TimeGuardService::pause()
{

}

void TimeGuardService::resume()
{

}

void TimeGuardService::stop()
{

}

