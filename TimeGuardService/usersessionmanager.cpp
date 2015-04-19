#include "usersessionmanager.h"
#include "QsLog.h"

UsersSessionManager::UsersSessionManager(QObject *parent) : QObject(parent)
{
   monitorSessionTimer = new QTimer();
   activeUsers = new QHash<User, QPointer<UserLimitTimer>>();
   connect(monitorSessionTimer, SIGNAL(timeout()),
           this, SLOT(monitorSessions()));
   monitorSessionTimer->start(30000);
}

UsersSessionManager::~UsersSessionManager()
{
   delete activeUsers;
   delete monitorSessionTimer;
}

void UsersSessionManager::monitorUserSession(const User &user, int limitSeconds)
{
   if(!activeUsers->contains(user)) {
      QLOG_DEBUG() << "Monitor user session:" << user;
      QPointer<UserLimitTimer> timer = new UserLimitTimer(user, limitSeconds);
      connect(timer, SIGNAL(timeout(User)),
              this, SLOT(stopUserSession(User)));
      activeUsers->insert(user, timer);
   }
}

void UsersSessionManager::stopUserSession(const User &user)
{
   QLOG_DEBUG() << "Stop monitoring user session:" << user;
   delete activeUsers->take(user);
}

void UsersSessionManager::monitorSessions()
{
   auto it = activeUsers->begin();
   while(it != activeUsers->end()) {
     const User &user = it.key();
     ++it;
     if(!user.isActive())
        stopUserSession(user);
   }
}

