#include "systeminfo.h"
#include "windows.h"
#include "wtsapi32.h"
#include "QsLog.h"

ulong SystemInfo::getSessionId()
{
   ulong sessionId = 0;
   LPTSTR data = nullptr;
   DWORD bytesReturned = 0;
   if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
                              WTS_CURRENT_SESSION,
                              WTSSessionId,
                              &data,
                              &bytesReturned)) {
      sessionId = *data;
      WTSFreeMemory(data);
   }
   else
      QLOG_FATAL() << "Unable to retrieve current session id";
   return sessionId;
}

QString SystemInfo::getUserName()
{
   QString userName;
   LPTSTR data = nullptr;
   DWORD bytesReturned = 0;
   if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
                              WTS_CURRENT_SESSION,
                              WTSUserName,
                              &data,
                              &bytesReturned)) {
      userName = QString::fromWCharArray(data);
      WTSFreeMemory(data);
   }
   else
      QLOG_ERROR() << "Unable to retrieve current user name";
   return userName;
}

