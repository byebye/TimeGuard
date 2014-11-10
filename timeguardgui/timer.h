#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QLCDNumber>
#include <QTimer>
#include <QTime>
#include <QString>
#include "timelimit.h"

class Timer : public QLCDNumber
{
  Q_OBJECT
public:
  Timer(QWidget *parentWidget);
  ~Timer();

  void setTime(TimeLimit timeLimit);
  void startTime();
  void pauseTime();
  void resumeTime();
  void resetTime(TimeLimit timeLimit);
  void resetTime();
  TimeLimit getTimeRemaining();
  bool isTimeSet();
  bool isTimeActive();
  void displayDefaultTime();
  const static QString ZERO_TIME;
  static QString timeToString(QTime qtime);
public slots:
  void startTimer();
  void setDisplay();
signals:
  void timeout();
  void timeToSaveTimeRemaining(TimeLimit time);
private:
  QTimer *timer;
  TimeLimit *timeRemaining;
  const int saveTimePeriod;
  int secondsElapsedCounter;
  bool timeSet;

  void setTimer();
};

#endif // COUNTDOWN_H
