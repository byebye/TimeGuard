#ifndef USERLIMITTIMER_H
#define USERLIMITTIMER_H

#include <QObject>
#include <QTimer>
#include "user.h"

class UserLimitTimer : public QObject
{
   Q_OBJECT
public:
   explicit UserLimitTimer(const User &user, int seconds = -1, QObject *parent = 0);
   ~UserLimitTimer();

   void startTimer(int seconds);
   void stopTimer();
   void pauseTimer();
   void resumeTimer();
   int timeRemaining();
signals:
   void timeout(const User &user);
public slots:
   void timeoutReceived();
private:
   const User controlledUser;
   QTimer *timer;
};

#endif // USERLIMITTIMER_H
