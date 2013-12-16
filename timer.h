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

  void setTime(QTime timeLimit, int saveTimePeriod);
  void startTimer();
  void pauseTime();
  void resumeTime();
  void resetTime(QTime timeLimit);
  QString getTimeRemaining();
public slots:
  void setDisplay(bool timerActive = true);
signals:
  void timeout();
  void saveTimeMoment();
private:
  QTimer *timer;
  QTime *timeRemaining;
  int saveTimePeriod;
  int secondsElapsedCounter;

  void setTimer();
};

#endif // COUNTDOWN_H
