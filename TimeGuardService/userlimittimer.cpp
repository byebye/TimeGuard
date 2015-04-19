#include "userlimittimer.h"

UserLimitTimer::UserLimitTimer(int minutes, QObject *parent) : QObject(parent)
{
   timer = new QTimer();
   connect(timer, SIGNAL(timeout()),
           this, SLOT(timeoutReceived()));
   startTimer(minutes);
}

UserLimitTimer::~UserLimitTimer()
{
   delete timer;
}

void UserLimitTimer::startTimer(int minutes)
{
   if(minutes > 0)
      timer->start(minutes * 60 * 1000);
}

void UserLimitTimer::stopTimer()
{
   timer->stop();
}

void UserLimitTimer::pauseTimer()
{
   timer->stop();
}

void UserLimitTimer::resumeTimer()
{
   timer->start();
}

void UserLimitTimer::timeoutReceived()
{
   stopTimer();
   emit timeout();
}


