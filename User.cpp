#include "User.h"
#include "windows.h"
#include "lmcons.h"

User::User(QObject *parent, FileManager *fileManager) :
  QObject(parent),
  fileManager(fileManager),
  name(getSystemUsername())
{
  saveLogInTime();
  readAvaiableTime();
}

User::~User()
{
  delete avaiableTime;
}

void User::saveLogInTime()
{
  fileManager->saveToFile(name,
                         "login: " + QTime::currentTime().toString("hh:mm:ss"));
}

void User::saveLogOffTime()
{
  fileManager->saveToFile(name,
                        "logoff: " + QTime::currentTime().toString("hh:mm:ss"));
}

void User::readAvaiableTime()
{
  QString timeString = fileManager->readFromFile(name);
  avaiableTime = new QTime(QTime::fromString(timeString));
}

void User::saveAvaiableTime()
{

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
