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
  void setTimer();
  void setTime(int hours, int minutes, int seconds = 0);
  void setTime(QString stringTime);
  void startCounter();
public slots:
  void setDisplay();
};

#endif // COUNTDOWN_H
