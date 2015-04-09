#include "userslimitcontroller.h"
#include "QsLog.h"

UsersLimitController::UsersLimitController(QObject *parent) : QObject(parent)
{
   usersLimitTimer = new QHash<QString, QPointer<UserLimitTimer>>();
   limitSettingsManager = new LimitSettingsManager();
   communicationSocket = new GUICommunicationSocket();
   connect(communicationSocket, SIGNAL(newUserSessionStarted(ulong,QString)),
           this, SLOT(newUserSessionStarted(ulong,QString)));
}

UsersLimitController::~UsersLimitController()
{
   delete usersLimitTimer;
   delete limitSettingsManager;
   delete communicationSocket;
}

void UsersLimitController::newUserSessionStarted(ulong sessionId, QString userName)
{
   QLOG_DEBUG() << "New user session" << sessionId << "-" << userName;
}

