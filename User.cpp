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
  timeRemaining = new QTime();

  if(limitActive)
    readTimeRemaining();
}

User::~User()
{
  delete timeRemaining;
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
  return QTime::fromString(timeString);
}

void User::resetTimeRemaining()
{
  *timeRemaining = readTimeLimit();
}

void User::saveTimeRemaining()
{
  *timeRemaining = timeRemaining->addSecs(-saveTimePeriod);
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
  *timeRemaining = readTimeRemaining();
  return *timeRemaining;
}
