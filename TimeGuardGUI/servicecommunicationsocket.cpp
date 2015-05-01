#include "servicecommunicationsocket.h"
#include <QVariantMap>
#include "systeminfo.h"
#include "QsLog.h"

ServiceCommunicationSocket::ServiceCommunicationSocket(QObject *parent) : QObject(parent),
   userName{SystemInfo::getUserName()}, sessionId{SystemInfo::getSessionId()},
   socket{new QLocalSocket{this}}
{

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
      individualChannelName = CommunicationSocket::pipeNamePrefix
                              + generateIndividualChannelName(sessionId);
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
   if (!socket->isOpen())
      return false;
   QDataStream dataStream(socket);
   dataStream.setVersion(QDataStream::Qt_5_4);
   dataStream << package;
   if (socket->waitForReadyRead(500)) {
      QVariantMap feedback;
      dataStream >> feedback;
      if (dataStream.status() != QDataStream::Ok) {
         QLOG_ERROR() << "Received data package corrupted";
         return false;
      }
      if (feedback["command"] != "feedback") {
         QLOG_ERROR() << "Unexpected command" << feedback["command"] << " - should be 'feedback'";
         return false;
      }
      return feedback["success"].toBool();
   }
   QLOG_ERROR() << "No feedback received";
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
         {"username", userName},
         {"session_id", static_cast<qint32>(sessionId)}
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

QString ServiceCommunicationSocket::getChannelName() const
{
   return individualChannelName;
}
