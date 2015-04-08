#include "userslimitcontroller.h"

UsersLimitController::UsersLimitController(QObject *parent) : QObject(parent)
{
   usersLimitTimer = new QHash<QString, QPointer<UserLimitTimer>>();
   limitSettingsManager = new LimitSettingsManager();
   communicationSocket = new GUICommunicationSocket();
}

UsersLimitController::~UsersLimitController()
{
   delete usersLimitTimer;
   delete limitSettingsManager;
   delete communicationSocket;
}

