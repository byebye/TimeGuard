#include "userslimitcontroller.h"
#include "QsLog.h"

UsersLimitController::UsersLimitController(QObject *parent) : QObject(parent)
{
   usersLimitTimer = new QHash<QString, QPointer<UserLimitTimer>>();
   limitSettingsManager = new LimitSettingsManager();
   usersSessionManager = new UsersSessionManager();
   communicationSocket = new GUICommunicationSocket();
   connect(communicationSocket, SIGNAL(newUserSessionStarted(User)),
           this, SLOT(newUserSessionStarted(User)));
}

UsersLimitController::~UsersLimitController()
{
   delete usersLimitTimer;
   delete limitSettingsManager;
   delete communicationSocket;
   delete usersSessionManager;
}

void UsersLimitController::newUserSessionStarted(const User &user)
{
   QLOG_DEBUG() << "New user session" << user.getSessionId() << "-" << user.getName();
}

