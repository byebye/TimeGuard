#include "userslimitcontroller.h"

UsersLimitController::UsersLimitController(QObject *parent) : QObject(parent)
{
   usersLimitTimer = new QHash<QString, UserLimitTimer>();
   limitSettingsManager = new LimitSettingsManager();
}

UsersLimitController::~UsersLimitController()
{

}

