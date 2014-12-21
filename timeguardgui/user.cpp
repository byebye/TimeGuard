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
  initialTimeRemaining = new TimeLimit();

  if(limitEnabled)
    *initialTimeRemaining = readTimeRemaining();
}

User::~User()
{
  delete timeRemaining;
  delete initialTimeRemaining;
  fileManager = NULL;
  systemQuery = NULL;
  logger = NULL;
}

TimeLimit User::readTimeRemaining()
{
  QDate currentDate = QDate::currentDate();
  QDate lastLoginDate = QDate::fromString(fileManager->readSettings(name, FileManager::LastLoginDate), "yyyy.MM.dd");
  fileManager->saveSettings(name, currentDate.toString("yyyy.MM.dd"), FileManager::LastLoginDate);
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
  const int secondsSinceLogIn = initialTimeRemaining->getTimeRemaining() - timeRemaining->getTimeRemaining();
  TimeLimit weeklyTimeRemaining = readLimit(FileManager::WeeklyTimeRemaining);
  TimeLimit monthlyTimeRemaining = readLimit(FileManager::MonthlyTimeRemaining);
  saveLimit(*initialTimeRemaining, FileManager::TimeRemaining);
  saveLimit(weeklyTimeRemaining.secondsElapsed(-secondsSinceLogIn), FileManager::WeeklyTimeRemaining);
  saveLimit(monthlyTimeRemaining.secondsElapsed(-secondsSinceLogIn), FileManager::MonthlyTimeRemaining);
  *timeRemaining = *initialTimeRemaining;
}

void User::saveTimeRemaining(int secondsElapsed)
{
  TimeLimit weeklyTimeRemaining = readLimit(FileManager::WeeklyTimeRemaining);
  TimeLimit monthlyTimeRemaining = readLimit(FileManager::MonthlyTimeRemaining);
  saveLimit(timeRemaining->secondsElapsed(secondsElapsed), FileManager::TimeRemaining);
  saveLimit(weeklyTimeRemaining.secondsElapsed(secondsElapsed), FileManager::WeeklyTimeRemaining);
  saveLimit(monthlyTimeRemaining.secondsElapsed(secondsElapsed), FileManager::MonthlyTimeRemaining);
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
