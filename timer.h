#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QLCDNumber>
#include <QTimer>
#include <QTime>

class Timer : public QLCDNumber
{
Q_OBJECT
public:
  Timer(QWidget *parentWidget);
  QTimer *timer;
  QTime *timeRemaining;
  void setTime(int hours, int minutes);
  void startCounter();
public slots:
  void setDisplay();
};

#endif // COUNTDOWN_H
