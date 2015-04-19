#ifndef USERSESSIONMANAGER_H
#define USERSESSIONMANAGER_H

#include <QObject>
#include <QString>
#include <QTimer>
#include <QList>
#include "user.h"

class UsersSessionManager : public QObject
{
   Q_OBJECT
public:
   explicit UsersSessionManager(QObject *parent = 0);
   ~UsersSessionManager();

   void monitorUserSession(const User &user);
   void stopUserSession(const User &user);
signals:
   void userLoggedOut(User user);
public slots:
   void monitorSessions();
private:
   QTimer *monitorSessionTimer;
   QList<User> *activeUsers;
};

#endif // USERSESSIONMANAGER_H
