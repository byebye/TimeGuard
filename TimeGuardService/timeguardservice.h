#ifndef TIMEGUARDSERVICE_H
#define TIMEGUARDSERVICE_H

#include "qtservice.h"
#include "userslimitcontroller.h"

class TimeGuardService : public QtService<QCoreApplication>
{
public:
   TimeGuardService(int argc, char **argv);
   ~TimeGuardService();

protected:
   void start();
   void pause();
   void resume();
   void stop();

private:
   UsersLimitController *usersLimitController;
};

#endif // TIMEGUARDSERVICE_H
