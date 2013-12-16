#include "timer.h"
#include "QMessageBox"

Timer::Timer(QWidget *parentWidget)
{
  setParent(parentWidget);
  setDigitCount(8);
  timeRemaining = new QTime();
  timer = new QTimer();
  timer->setInterval(1000);
  connect(timer, SIGNAL(timeout()), this, SLOT(setDisplay()));
}

Timer::~Timer()
{
  delete timer;
  delete timeRemaining;
}

void Timer::setTime(QTime timeLimit, int saveTimePeriod)
{
  this->saveTimePeriod = saveTimePeriod;
  resetTime(timeLimit);
}

void Timer::resetTime(QTime timeLimit)
{
  timer->stop();
  *timeRemaining = timeLimit;
  secondsElapsedCounter = 0;
  setDisplay();
}

QString Timer::getTimeRemaining()
{
  return timeRemaining->toString("hh:mm:ss");
}

void Timer::startTimer()
{
  timer->start(1000);
}

void Timer::setDisplay()
{
  if(++secondsElapsedCounter % saveTimePeriod == 0)
    emit saveTimeMoment();

  QString timeString = timeRemaining->toString("hh:mm:ss");
  display(timeString);
  if(timeString == "00:00:00")
  {
    timer->stop();
    emit timeout();
  }
  else
    startTimer();
  *timeRemaining = timeRemaining->addSecs(-1);
}
