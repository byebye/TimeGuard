#ifndef USERLIMITTIMER_H
#define USERLIMITTIMER_H

#include <QObject>
#include <QTimer>

class UserLimitTimer : public QObject
{
   Q_OBJECT
public:
   explicit UserLimitTimer(int minutes = -1, QObject *parent = 0);
   ~UserLimitTimer();

   void startTimer(int minutes);
   void stopTimer();
   void pauseTimer();
   void resumeTimer();
signals:
   void timeout();
public slots:
   void timeoutReceived();
private:
   QTimer *timer;
};

#endif // USERLIMITTIMER_H
