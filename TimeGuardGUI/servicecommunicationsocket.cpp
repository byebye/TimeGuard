#include "servicecommunicationsocket.h"
#include "windows.h"
#include "wtsapi32.h"
#include "QsLog.h"

QString ServiceCommunicationSocket::globalChannelName = "\\\\.\\pipe\\TimeGuardGlobalSocket";

ServiceCommunicationSocket::ServiceCommunicationSocket(QObject *parent) : QObject(parent)
{
   socket = new QLocalSocket(this);
}

ServiceCommunicationSocket::~ServiceCommunicationSocket()
{
   delete socket;
}

bool ServiceCommunicationSocket::createIndividualCommunicationChannel()
{
   int connectionAttempts = 3;
   bool connected = false;
   while(!connected && --connectionAttempts >= 0) {
      individualChannelName = QString("\\\\.\\pipe\\") + generateIndividualChannelName(getSessionId());
      sendIndividualChannelName();
      socket->disconnectFromServer();
      socket->connectToServer(individualChannelName);
      connected = socket->waitForConnected(30000);
   }
   if(connected)
      QLOG_INFO() << "Connection through individual communication channel established";
   else
      QLOG_FATAL() << "Unable to connect with service through individual communication channel: "
                   << socket->errorString();
   return connected;
}

bool ServiceCommunicationSocket::sendIndividualChannelName()
{
   QLocalSocket *globalSocket = new QLocalSocket(this);
   globalSocket->connectToServer(globalChannelName);
   if (globalSocket->waitForConnected(30000)) {
      QDataStream globalSocketStream(globalSocket);
      globalSocketStream.setVersion(QDataStream::Qt_5_4);
      // TODO - protocol to distinguish consistent data chunks
      globalSocketStream << individualChannelName << (qint32) getSessionId() << getUserName();
      if (globalSocket->waitForReadyRead(30000)) {
         QDataStream globalSocketStream(globalSocket);
         globalSocketStream.setVersion(QDataStream::Qt_5_4);
         bool individualChannelCreated = false;
         globalSocketStream >> individualChannelCreated;
         return individualChannelCreated;
      }
      QLOG_WARN() << "No information received about individual communication channel being created";
      return false;
   }
   QLOG_FATAL() << "Unable to connect with service through global communication channel";
   return false;
}

QString ServiceCommunicationSocket::generateIndividualChannelName(unsigned long sessionId)
{
   QUuid uuid = QUuid::createUuid();
   return "TimeGuard_s" + QString::number(sessionId) + uuid.toString();
}

unsigned long ServiceCommunicationSocket::getSessionId()
{
   unsigned long sessionId = 0;
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
   else {
      // TODO - action when unable to retrieve session id
      QLOG_FATAL() << "Unable to retrieve current session id";
   }
   return sessionId;
}

QString ServiceCommunicationSocket::getUserName()
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

QString ServiceCommunicationSocket::getChannelName() const
{
   return individualChannelName;
}
