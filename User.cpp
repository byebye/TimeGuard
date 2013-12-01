#include "User.h"
#include "windows.h"
#include "lmcons.h"

User::User(QObject *parent) :
  QObject(parent),
  name(getSystemUsername())
{
}

void User::logOff()
{
  ExitWindowsEx(EWX_FORCE, 0);
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
