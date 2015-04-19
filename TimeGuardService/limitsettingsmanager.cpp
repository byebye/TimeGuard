#include "limitsettingsmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "QsLog.h"

LimitSettingsManager::LimitSettingsManager(QObject *parent) : QObject(parent)
{

}

LimitSettingsManager::~LimitSettingsManager()
{

}

int LimitSettingsManager::readLimit(const User &user, LimitSettingsManager::LimitType limitType)
{
   QJsonObject settings = readSettings(user).object();
   return settings[optionToStringName(limitType)].toInt();
}

bool LimitSettingsManager::saveLimit(const User &user, LimitSettingsManager::LimitType limitType, int limitSeconds)
{
   QJsonDocument settings = readSettings(user);
   settings.object()[optionToStringName(limitType)] = limitSeconds;
   return writeSettings(user, settings);
}

bool LimitSettingsManager::enableLimit(const User &user)
{
   QJsonDocument settings = readSettings(user);
   settings.object()["limit_enabled"] = true;
   return writeSettings(user, settings);
}

bool LimitSettingsManager::disableLimit(const User &user)
{
   QJsonDocument settings = readSettings(user);
   settings.object()["limit_enabled"] = false;
   return writeSettings(user, settings);
}

bool LimitSettingsManager::isLimitEnabled(const User &user)
{
   return false;
}

QJsonDocument LimitSettingsManager::readSettings(const User &user)
{
   QFile settingsFile(generateSettingsFileName(user.getName()));
   if(!settingsFile.open(QFile::ReadOnly | QFile::Text)) {
      QLOG_ERROR() << "Couldn't open settings file" << settingsFile.fileName() << "for reading";
      return QJsonDocument();
   }
   QByteArray settingsData = settingsFile.readAll();
   return QJsonDocument(QJsonDocument::fromJson(settingsData));
}

bool LimitSettingsManager::writeSettings(const User &user, const QJsonDocument &settings)
{
   QFile settingsFile(generateSettingsFileName(user.getName()));
   if(!settingsFile.open(QFile::WriteOnly | QFile::Text)) {
      QLOG_ERROR() << "Couldn't open settings file" << settingsFile.fileName() << "for writing";
      return false;
   }
   settingsFile.write(settings.toJson());
   return true;
}

QString LimitSettingsManager::optionToStringName(LimitSettingsManager::LimitType limitType)
{
   switch(limitType)
   {
      case DailyLimit: return "daily_limit";
      case WeekDayLimit: return "weekday_limit";
   }
   return "";
}

QString LimitSettingsManager::generateSettingsFileName(const QString &userName)
{
   return QString("D:/" + userName + ".json");
}

void LimitSettingsManager::generateDefaultSettingsFile(const User &user)
{
   QJsonObject weekDays{
      {"Monday", 0},
      {"Tuesday", 0},
      {"Wednesday", 0},
      {"Thursday", 0},
      {"Friday", 0},
      {"Saturday", 0},
      {"Sunday", 0}
   };
   QJsonObject settings
   {
      {"limit_enabled", false},
      {optionToStringName(DailyLimit), 0},
      {optionToStringName(WeekDayLimit), weekDays}
   };
   writeSettings(user, QJsonDocument(settings));
}

