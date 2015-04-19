#ifndef USERSESSIONMANAGER_H
#define USERSESSIONMANAGER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QHash>
#include <QPointer>
#include "user.h"
#include "userlimittimer.h"

class UsersSessionManager : public QObject
{
   Q_OBJECT
public:
   explicit UsersSessionManager(QObject *parent = 0);
   ~UsersSessionManager();

   void stopUserSession(const User &user);
signals:
   void userLoggedOut(User user);
public slots:
   void monitorUserSession(const User &user, int limitMinutes);
private:
   QTimer *monitorSessionTimer;
   QHash<User, QPointer<UserLimitTimer>> *activeUsers;

   void monitorSessions();
};

#endif // USERSESSIONMANAGER_H
