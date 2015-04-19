#include "usersessionmanager.h"

UsersSessionManager::UsersSessionManager(QObject *parent) : QObject(parent)
{
   monitorSessionTimer = new QTimer();
   activeUsers = new QList<User>();
   connect(monitorSessionTimer, SIGNAL(timeout()),
           this, SLOT(monitorSessions()));
   monitorSessionTimer->start(30000);
}

UsersSessionManager::~UsersSessionManager()
{
   delete activeUsers;
   delete monitorSessionTimer;
}

void UsersSessionManager::monitorUserSession(const User &user)
{
   if(!activeUsers->contains(user))
      activeUsers->push_back(user);
}

void UsersSessionManager::stopUserSession(const User &user)
{
   QMutableListIterator<User> it(*activeUsers);
   if(it.findNext(user)) {
      emit userLoggedOut(user);
      it.remove();
   }
}

void UsersSessionManager::monitorSessions()
{
   QMutableListIterator<User> it(*activeUsers);
   while(it.hasNext()) {
      User &user = it.next();
      if(!user.isActive())
         stopUserSession(user);
   }
}

