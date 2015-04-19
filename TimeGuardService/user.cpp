#include "user.h"
#include "windows.h"
#include "wtsapi32.h"
#include "QsLog.h"

User::User(const QString &name, ulong sessionId) :
   name(name), sessionId(sessionId)
{

}

User::~User()
{

}

QString User::getName() const
{
   return name;
}


ulong User::getSessionId() const
{
   return sessionId;
}

bool User::operator==(const User &user) const
{
   return name == user.name && sessionId == user.sessionId;
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
      QLOG_WARN() << "Unable to check if user" << *this << "is still active - session is about to close";

   return active;
}

QDebug operator <<(QDebug stream, const User &user)
{
   stream << "name:" << user.name << ", session id:" << user.sessionId;
   return stream;
}
