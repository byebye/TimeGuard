#include "user.h"
#include "windows.h"
#include "wtsapi32.h"
#include "QsLog.h"

User::User(const QString &name, int sessionId, QObject *parent) : QObject(parent),
   name(name), sessionId(sessionId)
{

}

User::~User()
{

}


int User::getSessionId() const
{
   return sessionId;
}

void User::setSessionId(int sid)
{
   sessionId = sid;
}

bool User::isActive() const
{
   bool active = false;
   LPTSTR data = nullptr;
   DWORD bytesReturned = 0;
   if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
                              sessionId,
                              WTSConnectState,
                              &data,
                              &bytesReturned)) {
      active = (*data == WTSActive);
      WTSFreeMemory(data);
   }
   else
      QLOG_WARN() << "Unable to check if user " << name << " is still active - session is about to close";

   return active;
}
