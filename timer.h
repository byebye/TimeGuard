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

  void setTime(int hours, int minutes, int seconds = 0);
  void setTime(QTime avaiableTime);
  void startCounter();
public slots:
  void setDisplay();
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
