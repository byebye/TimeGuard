#include "user.h"
#include <windows.h>
#include <lmcons.h>

User::User(QObject *parent, FileManager *fileManager, Logger *logger) :
  QObject(parent),
  fileManager(fileManager),
  logger(logger),
  name(getSystemUsername()),
  saveTimePeriod(10)
{
  saveLogInTime();
  limitActive = readLimitActive();
  timeLimit = new QTime();
  *timeLimit = readTimeLimit();
  timeRemaining = new QTime();
  *timeRemaining = readTimeRemaining();
}

User::~User()
{
  delete timeRemaining;
  delete timeLimit;
  fileManager = NULL;
  logger = NULL;
}

int User::getSaveTimePeriod()
{
  return saveTimePeriod;
}

void User::saveLogInTime()
{
  logger->logIn(name);
}

void User::saveLogOffTime()
{
  logger->logOff(name);
}

QTime User::readTimeRemaining()
{
  QString currentDate = QDate::currentDate().toString("yyyy.MM.dd");
  if(fileManager->readSettings(name, FileManager::LastLogin) != currentDate)
  {
    fileManager->saveSettings(name, currentDate, FileManager::LastLogin);
    fileManager->saveSettings(name, timeLimit->toString("hh:mm:ss"),
                              FileManager::TimeRemaining);
    return *timeLimit;
  }
  return QTime::fromString(fileManager->readSettings(
                                            name, FileManager::TimeRemaining));
}

QTime User::readTimeLimit()
{
  QString timeString = fileManager->readSettings(name, FileManager::TimeLimit);
  return QTime::fromString(timeString);
}

void User::resetTimeRemaining()
{
  *timeRemaining = *timeLimit;
}

void User::saveTimeRemaining()
{
  *timeRemaining = timeRemaining->addSecs(-saveTimePeriod);
  fileManager->saveSettings(name, timeRemaining->toString(),
                            FileManager::TimeRemaining);
}

bool User::readLimitActive()
{
  return fileManager->readSettings(name, FileManager::LimitActive) == "true";
}

bool User::isLimitActive()
{
  return limitActive;
}

bool User::setLimitActive(bool active)
{
  limitActive = active;
  if(limitActive)
    fileManager->saveSettings(name, "true", FileManager::LimitActive);
  else
    fileManager->saveSettings(name, "false", FileManager::LimitActive);
}

void User::logOff()
{
  saveLogOffTime();
//  ExitWindowsEx(EWX_FORCE, 0);
}

QString User::getSystemUsername()
{
  DWORD ULEN = UNLEN+1;
  TCHAR username[UNLEN+1];
  GetUserName(username, &ULEN);
  return QString(QString::fromWCharArray(username));
}

QString User::getName()
{
  return name;
}

QTime User::getTimeRemaining()
{
  return *timeRemaining;
}
