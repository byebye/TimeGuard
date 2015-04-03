#ifndef TIMEGUARDSERVICE_H
#define TIMEGUARDSERVICE_H

#include "qtservice.h"


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
};

#endif // TIMEGUARDSERVICE_H
