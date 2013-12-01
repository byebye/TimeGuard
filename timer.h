#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QLCDNumber>
#include <QTimer>
#include <QTime>
#include <QString>

class Timer : public QLCDNumber
{
Q_OBJECT
public:
  Timer(QWidget *parentWidget);
  QTimer *timer;
  QTime *timeRemaining;

  void setTime(int hours, int minutes, int seconds = 0);
  void setTime(QTime avaiableTime);
  void startCounter();
public slots:
  void setDisplay();
signals:
  void timeout();
private:
  void setTimer();
};

#endif // COUNTDOWN_H
