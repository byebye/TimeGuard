#ifndef LIMITTIMER_H
#define LIMITTIMER_H

#include <QLCDNumber>
#include <QTimer>
#include <QTime>

class LimitTimer : public QLCDNumber
{
   Q_OBJECT
public:
   explicit LimitTimer(QWidget *parentWidget = 0);
   ~LimitTimer();

   void setTime(int limitSeconds);
   int getTimeRemaining() const;
   void startTime();
   void pauseTime();
   void resumeTime();
   bool isActive() const;
signals:

public slots:
   void updateDisplay();
   void updateTimeRemaining();
private:
   QTimer *timer;
   int timeRemaining;

   QTime fromSecondsToTime(int seconds);
};

#endif // LIMITTIMER_H
