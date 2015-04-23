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

int LimitSettingsManager::readLimit(const User &user, LimitType limitType)
{
   QJsonObject settings = readSettings(user);
   return settings[optionToStringName(limitType)].toInt();
}

bool LimitSettingsManager::save(const User &user, const QString &option, const QVariant &value)
{
   QJsonObject settings = readSettings(user);
   if (option == "daily_limit")
      settings[option] = value.toInt();
   return writeSettings(user, settings);
}

bool LimitSettingsManager::saveLimit(const User &user, LimitType limitType, int limitSeconds)
{
   QJsonObject settings = readSettings(user);
   settings[optionToStringName(limitType)] = limitSeconds;
   return writeSettings(user, settings);
}

bool LimitSettingsManager::enableLimit(const User &user)
{
   QJsonObject settings = readSettings(user);
   settings["limit_enabled"] = true;
   return writeSettings(user, settings);
}

bool LimitSettingsManager::disableLimit(const User &user)
{
   QJsonObject settings = readSettings(user);
   settings["limit_enabled"] = false;
   return writeSettings(user, settings);
}

bool LimitSettingsManager::isLimitEnabled(const User &user)
{
   QJsonObject settings = readSettings(user);
   return settings["limit_enabled"].toBool();
}

QJsonObject LimitSettingsManager::readSettings(const User &user)
{
   QFile settingsFile(generateSettingsFileName(user.getName()));
   if(!settingsFile.open(QFile::ReadOnly | QFile::Text)) {
      QLOG_ERROR() << "Couldn't open settings file" << settingsFile.fileName() << "for reading";
      return QJsonObject();
   }
   QByteArray settingsData = settingsFile.readAll();
   return QJsonDocument::fromJson(settingsData).object();
}

bool LimitSettingsManager::writeSettings(const User &user, const QJsonObject &settings)
{
   QFile settingsFile(generateSettingsFileName(user.getName()));
   if(!settingsFile.open(QFile::WriteOnly | QFile::Text)) {
      QLOG_ERROR() << "Couldn't open settings file" << settingsFile.fileName() << "for writing";
      return false;
   }
   settingsFile.write(QJsonDocument(settings).toJson());
   return true;
}

QString LimitSettingsManager::optionToStringName(LimitType limitType)
{
   switch(limitType)
   {
      case LimitType::DailyLimit: return "daily_limit";
      case LimitType::WeekDayLimit: return "weekday_limit";
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
      {optionToStringName(LimitType::DailyLimit), 0},
      {optionToStringName(LimitType::WeekDayLimit), weekDays}
   };
   writeSettings(user, settings);
}

