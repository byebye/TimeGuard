#include "User.h"
#include "windows.h"
#include "lmcons.h"

User::User(QObject *parent, FileManager *fileManager, Logger *logger) :
  QObject(parent),
  fileManager(fileManager),
  logger(logger),
  name(getSystemUsername()),
  saveTimePeriod(10)
{
  saveLogInTime();
  readAvaiableTime();
}

User::~User()
{
  delete avaiableTime;
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

void User::readAvaiableTime()
{
  QString timeString = fileManager->readSettings(name);
  avaiableTime = new QTime(QTime::fromString(timeString));
}

void User::saveAvaiableTime()
{
  *avaiableTime = avaiableTime->addSecs(-saveTimePeriod);
  fileManager->saveSettings(name, avaiableTime->toString());
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

QTime User::getAvaiableTime()
{
  return *avaiableTime;
}
