#include "limittimer.h"
#include <QTime>

LimitTimer::LimitTimer(QWidget *parentWidget) :
   timeRemaining{0},
   timer{new QTimer()}
{
   setParent(parentWidget);
   setDigitCount(8);
   timer->setInterval(1000);
   connect(timer, SIGNAL(timeout()), this, SLOT(updateDisplay()));
   connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeRemaining()));
}

LimitTimer::~LimitTimer()
{
   delete timer;
}

void LimitTimer::updateDisplay()
{
   display(fromSecondsToTime(timeRemaining).toString("hh:mm:ss"));
}

void LimitTimer::updateTimeRemaining()
{
   --timeRemaining;
}

QTime LimitTimer::fromSecondsToTime(int seconds)
{
   int hours = seconds / 3600;
   int minutes = seconds % 3600 / 60;
   seconds %= 60;
   return QTime(hours, minutes, seconds);
}

void LimitTimer::setTime(int limitSeconds)
{
   timeRemaining = limitSeconds;
   updateDisplay();
}

int LimitTimer::getTimeRemaining() const
{
   return timeRemaining;
}

void LimitTimer::startTime()
{
   timer->start();
}

void LimitTimer::pauseTime()
{
   timer->stop();
}

void LimitTimer::resumeTime()
{
   timer->start();
}

bool LimitTimer::isActive() const
{
   return timer->isActive();
}


