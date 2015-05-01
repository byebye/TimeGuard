#include "userslimitcontroller.h"
#include "QsLog.h"
#include <QVariant>
#include <QList>

UsersLimitController::UsersLimitController(QObject *parent) : QObject(parent)
{
   limitSettingsManager = new LimitSettingsManager();
   usersSessionManager = new UsersSessionManager();
   communicationSocket = new GUICommunicationSocket();
   connect(communicationSocket, SIGNAL(newUserSessionStarted(User)),
           this, SLOT(newUserSession(User)));
   connect(communicationSocket, SIGNAL(settingsPackageReceived(QStringList,QVariant)),
           this, SLOT(processSettingsPackage(QStringList,QVariant)));
}

UsersLimitController::~UsersLimitController()
{
   delete limitSettingsManager;
   delete communicationSocket;
   delete usersSessionManager;
}

void UsersLimitController::newUserSession(const User &user)
{
   if (!limitSettingsManager->settingsFileExist(user))
      limitSettingsManager->generateDefaultSettingsFile(user);
   limitSettingsManager->generateDefaultSettingsFile(user);
   if (limitSettingsManager->isLimitEnabled(user)) {
      const int limitSeconds = limitSettingsManager->readLimit(user, LimitType::DailyLimit);
      usersSessionManager->monitorUserSession(user, limitSeconds);
   }
}

void UsersLimitController::processSettingsPackage(const QStringList &users, const QVariant &settings)
{
   QLOG_DEBUG() << "Process settings package";
   for (const QString &userName : users) {
      for (const QVariant &setting : settings.toList()) {
         QString option = setting.toList().at(0).toString();
         QVariant value = setting.toList().at(1);
         limitSettingsManager->save(User(userName, 0L), option, value);
      }
   }
}

