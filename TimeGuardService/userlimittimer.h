#ifndef USERLIMITTIMER_H
#define USERLIMITTIMER_H

#include <QObject>
#include <QTimer>

class UserLimitTimer : public QObject
{
   Q_OBJECT
public:
   explicit UserLimitTimer(QObject *parent = 0);
   ~UserLimitTimer();

   void startTimer(int minutes);
   void stopTimer();
   void pauseTimer();
   void resumeTimer();
signals:

public slots:

private:
   QTimer *timer;
};

#endif // USERLIMITTIMER_H
