#include "timer.h"

Timer::Timer(QWidget *parentWidget)
{
  setParent(parentWidget);
}

void Timer::setTime(int hours, int minutes)
{
  timer = new QTimer();
  timeRemaining = new QTime(hours, minutes, 0);
  connect(timer, SIGNAL(timeout()), this, SLOT(setDisplay()));
  setDigitCount(8);
}

void Timer::startCounter()
{
  timer->start(1000);
}

void Timer::setDisplay()
{
  *timeRemaining = timeRemaining->addSecs(-1);
  display(timeRemaining->toString("hh:mm:ss"));
  startCounter();
}
