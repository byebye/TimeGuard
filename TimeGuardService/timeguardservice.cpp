#include "timeguardservice.h"
#include <QObject>
#include "QsLog.h"

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
   QLOG_INFO() << "Service started";
   QCoreApplication *app = application();
   usersLimitController = new UsersLimitController(app);
}

void TimeGuardService::pause()
{
   QLOG_INFO() << "Service paused";
}

void TimeGuardService::resume()
{
   QLOG_INFO() << "Service resumed";
}

void TimeGuardService::stop()
{
   QLOG_INFO() << "Service stopped";
   delete usersLimitController;
   usersLimitController = nullptr;
}

