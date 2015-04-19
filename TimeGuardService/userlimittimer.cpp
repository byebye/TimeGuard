#include "userlimittimer.h"
#include "QsLog.h"

UserLimitTimer::UserLimitTimer(const User &user, int seconds, QObject *parent) : QObject(parent),
   controlledUser(user)
{
   timer = new QTimer();
   connect(timer, SIGNAL(timeout()),
           this, SLOT(timeoutReceived()));
   startTimer(seconds);
}

UserLimitTimer::~UserLimitTimer()
{
   delete timer;
}

void UserLimitTimer::startTimer(int seconds)
{
   QLOG_DEBUG() << "Timer started:" << seconds << "s for user" << controlledUser;
   if(seconds >= 0)
      timer->start(seconds * 1000);
}

void UserLimitTimer::stopTimer()
{
   QLOG_DEBUG() << "Timer stopped for user" << controlledUser;
   timer->stop();
}

void UserLimitTimer::pauseTimer()
{
   QLOG_DEBUG() << "Timer paused:" << timeRemaining() << "s for user" << controlledUser;
   timer->stop();
}

void UserLimitTimer::resumeTimer()
{
   QLOG_DEBUG() << "Timer resumed:" << timeRemaining() << "s for user" << controlledUser;
   timer->start();
}

int UserLimitTimer::timeRemaining()
{
   int ms = timer->remainingTime();
   if(ms == -1)
      return -1;
   return ms / 1000;
}

void UserLimitTimer::timeoutReceived()
{
   QLOG_DEBUG() << "Timeout for user" << controlledUser;
   stopTimer();
   emit timeout(controlledUser);
}


