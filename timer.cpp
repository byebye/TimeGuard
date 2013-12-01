#include "Timer.h"
#include "QMessageBox"
#include <iostream>

Timer::Timer(QWidget *parentWidget)
{
  setParent(parentWidget);
}

Timer::~Timer()
{
  delete timer;
  delete timeRemaining;
}

void Timer::setTimer()
{
  saveTimePeriod = 5;
  secondsElapsedCounter = 0;
  timer = new QTimer();
  timer->setInterval(1000);
  connect(timer, SIGNAL(timeout()), this, SLOT(setDisplay()));
  setDigitCount(8);
  startCounter();
}

void Timer::setTime(int hours, int minutes, int seconds)
{
  timeRemaining = new QTime(hours, minutes, seconds);
  setTimer();
}

void Timer::setTime(QTime avaiableTime)
{
  timeRemaining = new QTime(avaiableTime);
  setTimer();
}

void Timer::startCounter()
{
  timer->start(1000);
}

void Timer::setDisplay()
{
  if(++secondsElapsedCounter % saveTimePeriod == 0)
  {
    std::cout << "SAVE!" << std::endl;
    emit saveTimeMoment();
  }

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
