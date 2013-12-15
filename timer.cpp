#include "timer.h"
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

void Timer::setTime(QTime timeLimit, int saveTimePeriod)
{
  // add 1 second to start displaying and checking time at that given
  timeRemaining = new QTime(timeLimit.addSecs(1));
  this->saveTimePeriod = saveTimePeriod;
  secondsElapsedCounter = 0;
  timer = new QTimer();
  timer->setInterval(1000);
  connect(timer, SIGNAL(timeout()), this, SLOT(setDisplay()));
  setDigitCount(8);
  startCounter();
}

QString Timer::getTimeRemaining()
{
  return timeRemaining->toString("hh:mm:ss");
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
