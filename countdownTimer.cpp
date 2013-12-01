#include "countdownTimer.h"

countdownTimer::countdownTimer(QWidget *parentWidget)
{
  setParent(parentWidget);
}

void countdownTimer::setTime(int hours, int minutes)
{
  timer = new QTimer();
  timeRemaining = new QTime(hours, minutes, 0);
  connect(timer, SIGNAL(timeout()), this, SLOT(setDisplay()));
  setDigitCount(8);
}

void countdownTimer::startCounter()
{
  timer->start(1000);
}

void countdownTimer::setDisplay()
{
  *timeRemaining = timeRemaining->addSecs(-1);
  display(timeRemaining->toString("hh:mm:ss"));
  startCounter();
}
