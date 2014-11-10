#include "timer.h"
#include "QMessageBox"

const QString Timer::ZERO_TIME = "00:00:00";

Timer::Timer(QWidget *parentWidget) : saveTimePeriod(10)
{
  setParent(parentWidget);
  setDigitCount(8);
  timeSet = false;
  timeRemaining = new TimeLimit();
  timer = new QTimer();
  timer->setInterval(1000);
  connect(timer, SIGNAL(timeout()), this, SLOT(setDisplay()));
}

Timer::~Timer()
{
  delete timer;
  delete timeRemaining;
}

void Timer::displayDefaultTime()
{
  display(ZERO_TIME);
}

void Timer::setTime(TimeLimit timeLimit)
{
  timeSet = true;
  resetTime(timeLimit);
}

void Timer::startTime()
{
  startTimer();
}

void Timer::pauseTime()
{
  timer->stop();
}

void Timer::resumeTime()
{
  startTimer();
}

void Timer::resetTime()
{
  timeSet = false;
  timer->stop();
  display(ZERO_TIME);
}

void Timer::resetTime(TimeLimit timeLimit)
{
  bool timerActive = timer->isActive();
  timer->stop();
  *timeRemaining = timeLimit;
  secondsElapsedCounter = 0;
  setDisplay();
  if(timerActive)
    startTimer();
}

TimeLimit Timer::getTimeRemaining()
{
  return *timeRemaining;
}

void Timer::startTimer()
{
  timer->start(1000);
}

void Timer::setDisplay()
{
  if(++secondsElapsedCounter % saveTimePeriod == 0)
    emit timeToSaveTimeRemaining(*timeRemaining);

  display(timeRemaining->toString());
  if(timeRemaining->isTimeOut())
  {
    timer->stop();
    emit timeout();
  }
  else
    timeRemaining->secondsElapsed(1);
}

bool Timer::isTimeSet()
{
  return timeSet;
}

bool Timer::isTimeActive()
{
  return timer->isActive();
}

QString Timer::timeToString(QTime time)
{
  return time.toString("hh:mm:ss");
}
