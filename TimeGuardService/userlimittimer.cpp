#include "userlimittimer.h"

UserLimitTimer::UserLimitTimer(QObject *parent) : QObject(parent)
{
   timer = new QTimer();
}

UserLimitTimer::~UserLimitTimer()
{
   delete timer;
}

void UserLimitTimer::startTimer(int minutes)
{

}

void UserLimitTimer::stopTimer()
{

}

void UserLimitTimer::pauseTimer()
{

}

void UserLimitTimer::resumeTimer()
{

}


