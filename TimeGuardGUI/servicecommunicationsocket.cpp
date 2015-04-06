#include "servicecommunicationsocket.h"
#include "windows.h"
#include "wtsapi32.h"

QString ServiceCommunicationSocket::globalSocketName = "TimeGuardGlobalSocket";

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
   individualChannelName = generateIndividualChannelName();
   sendIndividualChannelName();
   socket->disconnectFromServer();
   socket->connectToServer(individualChannelName);
   return socket->waitForConnected(20000);
}

bool ServiceCommunicationSocket::sendIndividualChannelName()
{
   QLocalSocket *globalSocket = new QLocalSocket(this);
   globalSocket->connectToServer(globalSocketName);
   if (globalSocket->waitForConnected(20000)) {
      QDataStream globalSocketStream(globalSocket);
      globalSocketStream.setVersion(QDataStream::Qt_5_4);
      // TODO - protocol to distinguish consistent data chunks
      globalSocketStream << individualChannelName;
      return true;
   }
   return false;
}

QString ServiceCommunicationSocket::generateIndividualChannelName()
{
   QUuid uuid = QUuid::createUuid();
   return "s" + QString::number(getSessionId()) + uuid.toString();
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
   }
   return sessionId;
}

QString ServiceCommunicationSocket::getChannelName() const
{
   return individualChannelName;
}
