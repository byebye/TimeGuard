#include "timer.h"
#include "QMessageBox"

Timer::Timer(QWidget *parentWidget)
{
  setParent(parentWidget);
  setDigitCount(8);
  timeSet = false;
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
  timeSet = true;
  this->saveTimePeriod = saveTimePeriod;
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

void Timer::resetTime(QTime timeLimit)
{
  bool timerActive = timer->isActive();
  timer->stop();
  *timeRemaining = timeLimit;
  secondsElapsedCounter = 0;
  setDisplay();
  if(timerActive)
    startTimer();
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
    *timeRemaining = timeRemaining->addSecs(-1);
}

bool Timer::isTimeSet()
{
  return timeSet;
}

bool Timer::isTimeActive()
{
  return timer->isActive();
}
