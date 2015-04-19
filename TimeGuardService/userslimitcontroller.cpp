#include "userslimitcontroller.h"
#include "QsLog.h"

UsersLimitController::UsersLimitController(QObject *parent) : QObject(parent)
{
   limitSettingsManager = new LimitSettingsManager();
   usersSessionManager = new UsersSessionManager();
   communicationSocket = new GUICommunicationSocket();
   connect(communicationSocket, SIGNAL(newUserSessionStarted(User)),
           this, SLOT(newUserSession()));
}

UsersLimitController::~UsersLimitController()
{
   delete limitSettingsManager;
   delete communicationSocket;
   delete usersSessionManager;
}

void UsersLimitController::newUserSession(const User &user)
{
   const int limitMinutes = limitSettingsManager->readLimit(user, LimitSettingsManager::DailyLimit);
   usersSessionManager->monitorUserSession(user, limitMinutes);
}

