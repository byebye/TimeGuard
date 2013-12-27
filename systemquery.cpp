#include "systemquery.h"
#include <QDebug>
#include <windows.h>
#include <lmcons.h>
#include <lm.h>

SystemQuery::SystemQuery(QObject *parent, Messages *messages) :
  QObject(parent), messages(messages)
{
}

SystemQuery::~SystemQuery()
{
  messages = NULL;
}

QString SystemQuery::getCurrentUserName()
{
  DWORD ULEN = UNLEN+1;
  TCHAR username[UNLEN+1];
  GetUserName(username, &ULEN);
  return QString(QString::fromWCharArray(username));
}

QStringList SystemQuery::getUsersList()
{
  // -------------- VARIABLES -------------------
  LPTSTR serverName = NULL; // NULL -> local computer
  DWORD dwLevel = 0; // information level: 0 -> only names
  LPUSER_INFO_0 usersBuf = NULL;
  DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
  DWORD dwUserEntries = 0;
  DWORD dwTotalUserEntries = 0;
  DWORD dwResumeHandle = 0;
  NET_API_STATUS nStatus;

  LPLOCALGROUP_USERS_INFO_0 groupsBuf = NULL;
  DWORD dwGroupEntries = 0;
  DWORD dwTotalGroupEntries = 0;
  // ---------------------------------------------
  nStatus = NetUserEnum(serverName,
              dwLevel,
              FILTER_NORMAL_ACCOUNT,
              (LPBYTE*)&usersBuf,
              dwPrefMaxLen,
              &dwUserEntries,
              &dwTotalUserEntries,
              &dwResumeHandle
              );
  QStringList usersList;
  if(nStatus == NERR_Success)
  {
    for(DWORD i = 0; i < dwUserEntries; ++i)
    {
      bool normalUser = false;
      nStatus = NetUserGetLocalGroups(serverName,
                                      usersBuf->usri0_name,
                                      dwLevel,
                                      LG_INCLUDE_INDIRECT,
                                      (LPBYTE*)&groupsBuf,
                                      dwPrefMaxLen,
                                      &dwGroupEntries,
                                      &dwTotalGroupEntries
                                      );
      if(nStatus != NERR_Success)
      {
        qDebug() << "Error reading groups list for "
                 << QString::fromWCharArray(usersBuf->usri0_name);
        normalUser = true;
      }
      else
      {
        for(DWORD j = 0; j < dwGroupEntries; ++j)
        {
          QString group = QString::fromWCharArray(groupsBuf->lgrui0_name);
          if(group == tr("Użytkownicy") || group == tr("Administratorzy"))
          {
            normalUser = true;
            break;
          }
          ++groupsBuf;
        }
      }
      if(normalUser)
      {
        qDebug() << "User added to list: "
                 << QString::fromWCharArray(usersBuf->usri0_name);
        usersList.push_back(QString::fromWCharArray(usersBuf->usri0_name));
      }
      ++usersBuf;
    }
  }
  else
  {
    qDebug() << "Error reading users list!";
    messages->critical(Messages::ErrorReadingUsers);
  }
  return usersList;
}
