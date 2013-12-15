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

void User::readTimeRemaining()
{
  QString currentDate = QDate::currentDate().toString("yyyy.MM.dd");
  if(fileManager->readSettings(name, FileManager::LastLogin)
     != currentDate)
  {
    fileManager->saveSettings(name, currentDate, FileManager::LastLogin);
    QString timeLimit = fileManager->readSettings(name, FileManager::TimeLimit);
    fileManager->saveSettings(name, timeLimit, FileManager::TimeRemaining);
  }
  QString timeString = fileManager->readSettings(name, FileManager::TimeRemaining);
  timeRemaining = new QTime(QTime::fromString(timeString));
}

void User::saveTimeRemaining()
{
  *timeRemaining = timeRemaining->addSecs(-saveTimePeriod);
  fileManager->saveSettings(name, timeRemaining->toString(),
                            FileManager::TimeRemaining);
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
