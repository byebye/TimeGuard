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
   generateIndividualChannelName();
   sendIndividualChannelName();
   socket->disconnectFromServer();
   socket->connectToServer(channelName);
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
      globalSocketStream << channelName;
      return true;
   }
   return false;
}

QString ServiceCommunicationSocket::generateIndividualChannelName()
{
   LPTSTR data = nullptr;
   DWORD bytesReturned = 0;
   unsigned long sessionId = 0;
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
   QUuid uuid = QUuid::createUuid();
   channelName = "s" + QString::number(sessionId) + uuid.toString();
   return channelName;
}

QString ServiceCommunicationSocket::getChannelName() const
{
   return channelName;
}
