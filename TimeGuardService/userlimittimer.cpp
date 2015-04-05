#include "userlimittimer.h"

UserLimitTimer::UserLimitTimer(User *user, QObject *parent) : QObject(parent),
   controlledUser(user)
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


