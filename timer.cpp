#include "timer.h"
#include "QMessageBox"

Timer::Timer(QWidget *parentWidget)
{
  setParent(parentWidget);
}

void Timer::setTimer()
{
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

void Timer::setTime(QString stringTime)
{
  timeRemaining = new QTime(QTime::fromString(stringTime, "hh:mm:ss"));
  setTimer();
}

void Timer::startCounter()
{
  timer->start(1000);
}

void Timer::setDisplay()
{
  *timeRemaining = timeRemaining->addSecs(-1);
  QString timeString = timeRemaining->toString("hh:mm:ss");
  display(timeString);
  if(timeString == "00:00:00")
  {
    timer->stop();
    emit timeout();
    QMessageBox::information(NULL, "Koniec czasu!", "Czas się skończył!", QMessageBox::Ok);
  }
  else
    startCounter();
}
