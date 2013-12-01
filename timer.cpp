#include "timer.h"
#include "QMessageBox"

Timer::Timer(QWidget *parentWidget)
{
  setParent(parentWidget);
}

void Timer::setTime(int hours, int minutes, int seconds)
{
  timer = new QTimer();
  timer->setInterval(1000);
  timeRemaining = new QTime(hours, minutes, seconds);
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
  QString timeString = timeRemaining->toString("hh:mm:ss");
  display(timeString);
  if(timeString == "00:00:00")
  {
    timer->stop();
    QMessageBox::information(NULL, "Koniec czasu!", "Czas się skończył!", QMessageBox::Ok);
  }
  else
    startCounter();
}
