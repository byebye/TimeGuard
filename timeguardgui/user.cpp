#include "user.h"
#include <algorithm>
#include <QDebug>

User::User(QObject *parent, FileManager *fileManager,
           SystemQuery *systemQuery, Logger *logger) :
  QObject(parent),
  fileManager(fileManager),
  systemQuery(systemQuery),
  logger(logger),
  name(systemQuery->getCurrentUserName())
{
  limitEnabled = readLimitEnabled();
  timeRemaining = new TimeLimit();

  if(limitEnabled)
    readTimeRemaining();
}

User::~User()
{
  delete timeRemaining;
  fileManager = NULL;
  systemQuery = NULL;
  logger = NULL;
}

TimeLimit User::readTimeRemaining()
{
  QDate currentDate = QDate::currentDate();
  QDate lastLoginDate = QDate::fromString(fileManager->readSettings(name, FileManager::LastLogin));
  fileManager->saveSettings(name, currentDate.toString("yyyy.MM.dd"), FileManager::LastLogin);
  if(lastLoginDate != currentDate)
    refreshDailyTimeRemaining();
  if(lastLoginDate.weekNumber() != currentDate.weekNumber())
    refreshWeeklyTimeRemaining();
  if(lastLoginDate.month() != currentDate.month())
    refreshMonthlyTimeRemaining();
  TimeLimit dailyTimeRemaining = readLimit(FileManager::TimeRemaining);
  TimeLimit weeklyTimeRemaining = readLimit(FileManager::WeeklyTimeRemaining);
  TimeLimit monthlyTimeRemaining = readLimit(FileManager::MonthlyTimeRemaining);
  TimeLimit timeRemaining = std::min({dailyTimeRemaining, weeklyTimeRemaining, monthlyTimeRemaining});
  return timeRemaining;
}

void User::refreshDailyTimeRemaining() {
  TimeLimit dailyTimeRemaining = readLimit(FileManager::DailyLimit);
  saveLimit(dailyTimeRemaining, FileManager::TimeRemaining);
}

void User::refreshWeeklyTimeRemaining() {
  TimeLimit weeklyTimeRemaining = readLimit(FileManager::WeeklyLimit).convertToWeeklyTimeRemaining();
  saveLimit(weeklyTimeRemaining, FileManager::WeeklyTimeRemaining);
}

void User::refreshMonthlyTimeRemaining() {
  TimeLimit monthlyTimeRemaining = readLimit(FileManager::MonthlyLimit).convertToMonthlyTimeRemaining(QDate::currentDate().daysInMonth());
  saveLimit(monthlyTimeRemaining, FileManager::MonthlyTimeRemaining);
}

TimeLimit User::readLimit(FileManager::SettingName limitName)
{
  return TimeLimit(fileManager->readSettings(name, limitName));
}

void User::saveLimit(TimeLimit limit, FileManager::SettingName limitName)
{
  fileManager->saveSettings(name, limit.toString(), limitName);
}

void User::saveLimit(QString limit, FileManager::SettingName limitName)
{
  fileManager->saveSettings(name, limit, limitName);
}

void User::resetTimeRemaining()
{
  *timeRemaining = readLimit(FileManager::DailyLimit); // TODO consider all limit types
}

void User::saveTimeRemaining(TimeLimit time)
{
  *timeRemaining = time;
  fileManager->saveSettings(name, timeRemaining->toString(), FileManager::TimeRemaining);
  //fileManager->saveSettings(name, Timer::timeToString(), FileManager::WeeklyTimeRemaining);
  //fileManager->saveSettings(name, Timer::timeToString(), FileManager::MonthlyTimeRemaining);
}

bool User::readLimitEnabled()
{
  return fileManager->readSettings(name, FileManager::LimitEnabled) == "1";
}

bool User::isLimitEnabled()
{
  return limitEnabled;
}

void User::setLimitEnabled(bool enabled)
{
  limitEnabled = enabled;
  if(limitEnabled)
    fileManager->saveSettings(name, "1", FileManager::LimitEnabled);
  else
    fileManager->saveSettings(name, "0", FileManager::LimitEnabled);
}

void User::logOff()
{
//  ExitWindowsEx(EWX_FORCE, 0);
}

QString User::getName()
{
  return name;
}

TimeLimit User::getTimeRemaining()
{
  *timeRemaining = readTimeRemaining();
  return *timeRemaining;
}
