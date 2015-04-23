#include "servicecommunicationsocket.h"
#include <QVariantMap>
#include "windows.h"
#include "wtsapi32.h"
#include "QsLog.h"

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
      individualChannelName = CommunicationSocket::pipeNamePrefix + generateIndividualChannelName(getSessionId());
      if (sendIndividualChannelName()) {
         socket->disconnectFromServer();
         socket->connectToServer(individualChannelName);
         connected = socket->waitForConnected(500);
      }
      else
         QLOG_ERROR() << "Individual channel:" << individualChannelName << "could not be created";
   }
   if(connected)
      QLOG_INFO() << "Connection through individual communication channel established";
   else
      QLOG_FATAL() << "Unable to connect with service through individual communication channel:"
                   << socket->errorString();
   return connected;
}

bool ServiceCommunicationSocket::sendPackage(const QVariantMap &package)
{
   return sendPackage(socket, package);
}

bool ServiceCommunicationSocket::sendPackage(QLocalSocket *socket, const QVariantMap &package)
{
   QDataStream socketStream(socket);
   socketStream.setVersion(QDataStream::Qt_5_4);
   socketStream << package;
   if (socket->waitForReadyRead(500)) {
      QVariantMap feedback;
      socketStream >> feedback;
      if (socketStream.status() != QDataStream::Ok)
         return false;
      // TODO - action when package is not adressed to me
      if (feedback["command"] == "feedback" && feedback["username"] == getUserName())
         return feedback["success"].toBool();
   }
   return false;
}

bool ServiceCommunicationSocket::sendIndividualChannelName()
{
   QLocalSocket *globalSocket = new QLocalSocket(this);
   globalSocket->connectToServer(CommunicationSocket::globalChannelName);
   if (globalSocket->waitForConnected(1000)) {
      QVariantMap createChannelPackage{
         {"command", "create_channel"},
         {"channel_name", individualChannelName},
         {"username", getUserName()},
         {"session_id", static_cast<qint32>(getSessionId())}
      };
      return sendPackage(globalSocket, createChannelPackage);
   }
   QLOG_FATAL() << "Unable to connect with service through global communication channel";
   return false;
}

QString ServiceCommunicationSocket::generateIndividualChannelName(ulong sessionId)
{
   QUuid uuid = QUuid::createUuid();
   return "TimeGuard_s" + QString::number(sessionId) + uuid.toString();
}

ulong ServiceCommunicationSocket::getSessionId()
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
