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

void Timer::pauseTime()
{
  timer->stop();
}

void Timer::resumeTime()
{
  startTimer();
}

void Timer::resetTime(QTime timeLimit)
{
  bool timerActive = timer->isActive();
  timer->stop();
  *timeRemaining = timeLimit;
  secondsElapsedCounter = 0;
  setDisplay(timerActive);
}

QString Timer::getTimeRemaining()
{
  return timeRemaining->toString("hh:mm:ss");
}

void Timer::startTimer()
{
  timer->start(1000);
}

void Timer::setDisplay(bool timerActive)
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
  else if(timerActive)
    startTimer();
  *timeRemaining = timeRemaining->addSecs(-1);
}
