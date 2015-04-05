#ifndef USERLIMITTIMER_H
#define USERLIMITTIMER_H

#include <QObject>
#include <QTimer>
#include "user.h"

class UserLimitTimer : public QObject
{
   Q_OBJECT
public:
   explicit UserLimitTimer(User *user, QObject *parent = 0);
   ~UserLimitTimer();

   void startTimer(int minutes);
   void stopTimer();
   void pauseTimer();
   void resumeTimer();
signals:

public slots:

private:
   User *controlledUser;
   QTimer *timer;
};

#endif // USERLIMITTIMER_H
