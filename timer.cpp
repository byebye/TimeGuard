#include "Timer.h"
#include "QMessageBox"

Timer::Timer(QWidget *parentWidget)
{
  setParent(parentWidget);
}

Timer::~Timer()
{
  delete timer;
  delete timeRemaining;
}

void Timer::setTime(QTime avaiableTime, int saveTimePeriod)
{
  timeRemaining = new QTime(avaiableTime);
  this->saveTimePeriod = saveTimePeriod;
  secondsElapsedCounter = 0;
  timer = new QTimer();
  timer->setInterval(1000);
  connect(timer, SIGNAL(timeout()), this, SLOT(setDisplay()));
  setDigitCount(8);
  startCounter();
}

void Timer::startCounter()
{
  timer->start(1000);
}

void Timer::setDisplay()
{
  if(++secondsElapsedCounter % saveTimePeriod == 0)
    emit saveTimeMoment();

  *timeRemaining = timeRemaining->addSecs(-1);
  QString timeString = timeRemaining->toString("hh:mm:ss");
  display(timeString);
  if(timeString == "00:00:00")
  {
    timer->stop();
    emit timeout();
  }
  else
    startCounter();
}
