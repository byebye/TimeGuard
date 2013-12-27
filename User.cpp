#include "user.h"

User::User(QObject *parent, FileManager *fileManager,
           SystemQuery *systemQuery, Logger *logger) :
  QObject(parent),
  fileManager(fileManager),
  systemQuery(systemQuery),
  logger(logger),
  name(systemQuery->getCurrentUserName())
{
  limitActive = readLimitActive();
  timeRemaining = new QTime();

  if(limitActive)
    readTimeRemaining();
}

User::~User()
{
  delete timeRemaining;
  fileManager = NULL;
  systemQuery = NULL;
  logger = NULL;
}

QTime User::readTimeRemaining()
{
  QString currentDate = QDate::currentDate().toString("yyyy.MM.dd");
  if(fileManager->readSettings(name, FileManager::LastLogin) != currentDate)
  {
    QTime timeLimit = readTimeLimit();
    fileManager->saveSettings(name, currentDate, FileManager::LastLogin);
    fileManager->saveSettings(name, timeLimit.toString("hh:mm:ss"),
                              FileManager::TimeRemaining);
    return timeLimit;
  }
  return QTime::fromString(fileManager->readSettings(
                                            name, FileManager::TimeRemaining));
}

QTime User::readTimeLimit()
{
  QString timeString = fileManager->readSettings(name, FileManager::TimeLimit);
  if(timeString.isEmpty())
    timeString = "00:00:00";
  return QTime::fromString(timeString);
}

void User::resetTimeRemaining()
{
  *timeRemaining = readTimeLimit();
}

void User::saveTimeRemaining(QTime time)
{
  *timeRemaining = time;
  fileManager->saveSettings(name, timeRemaining->toString(),
                            FileManager::TimeRemaining);
}

bool User::readLimitActive()
{
  return fileManager->readSettings(name, FileManager::LimitActive) == "1";
}

bool User::isLimitActive()
{
  return limitActive;
}

void User::setLimitActive(bool active)
{
  limitActive = active;
  if(limitActive)
    fileManager->saveSettings(name, "1", FileManager::LimitActive);
  else
    fileManager->saveSettings(name, "0", FileManager::LimitActive);
}

void User::logOff()
{
//  ExitWindowsEx(EWX_FORCE, 0);
}

QString User::getName()
{
  return name;
}

QTime User::getTimeRemaining()
{
  *timeRemaining = readTimeRemaining();
  return *timeRemaining;
}
