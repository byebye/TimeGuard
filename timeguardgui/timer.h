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
  ~Timer();

  void setTime(QTime timeLimit);
  void startTime();
  void pauseTime();
  void resumeTime();
  void resetTime(QTime timeLimit);
  void resetTime();
  QString getTimeRemaining();
  bool isTimeSet();
  bool isTimeActive();
  void displayDefaultTime();
public slots:
  void startTimer();
  void setDisplay();
signals:
  void timeout();
  void timeToSaveTimeRemaining(QTime time);
private:
  QTimer *timer;
  QTime *timeRemaining;
  int saveTimePeriod;
  int secondsElapsedCounter;
  bool timeSet;

  void setTimer();
};

#endif // COUNTDOWN_H
