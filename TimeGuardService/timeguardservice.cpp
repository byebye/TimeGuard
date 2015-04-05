#include "timeguardservice.h"
#include <QObject>

TimeGuardService::TimeGuardService(int argc, char **argv)
   : QtService<QCoreApplication>(argc, argv, "TimeGuard Service")
{
   setServiceDescription(QObject::tr("TimeGuard service controlling time spent on the computer"));
   setServiceFlags(QtServiceBase::CanBeSuspended);
}

TimeGuardService::~TimeGuardService()
{
}

void TimeGuardService::start()
{
   QCoreApplication *app = application();
   usersLimitController = new UsersLimitController(app);
//   QObject::connect(userManager, SIGNAL(timeout()),
//           app, SLOT(quit()));
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

