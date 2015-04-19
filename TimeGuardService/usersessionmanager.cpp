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

void UsersSessionManager::monitorUserSession(const User &user, int limitMinutes)
{
   if(!activeUsers->contains(user)) {
      QLOG_DEBUG() << "Monitor user session:" << user.getName() << " with session id =" << user.getSessionId();
      QPointer<UserLimitTimer> timer = new UserLimitTimer(limitMinutes);
      activeUsers->insert(user, timer);
   }
}

void UsersSessionManager::stopUserSession(const User &user)
{
   QLOG_DEBUG() << "Stop monitoring user session: " << user.getName() << " with session id =" << user.getSessionId();
   activeUsers->remove(user);
}

void UsersSessionManager::monitorSessions()
{
   auto it = activeUsers->begin();
   while(it != activeUsers->end()) {
     const User &user = it.key();
      if(!user.isActive()) {
         ++it;
         stopUserSession(user);
      }
   }
}

