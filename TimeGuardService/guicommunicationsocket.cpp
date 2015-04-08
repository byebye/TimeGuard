#include "guicommunicationsocket.h"
#include <QTime>
#include "QsLog.h"

QString GUICommunicationSocket::globalChannelName = "\\\\.\\pipe\\TimeGuardGlobalSocket";

GUICommunicationSocket::GUICommunicationSocket(QObject *parent) : QObject(parent)
{
   individualChannels = new QHash<QString, QPointer<IndividualCommunicationChannel>>();
   createGlobalChannel();
}

GUICommunicationSocket::~GUICommunicationSocket()
{
   delete individualChannels;
   delete globalChannel;
}

void GUICommunicationSocket::createGlobalChannel()
{
   globalChannel = new QLocalServer(this);
   globalChannel->setSocketOptions(QLocalServer::WorldAccessOption);
   if(!globalChannel->listen(globalChannelName)) {
      QLOG_FATAL() << "Unable to create global communication channel";
      return;
   }
   connect(globalChannel, SIGNAL(newConnection()), this, SLOT(collectDataFromGlobalConnection()));
   QLOG_INFO() << "Global communication channel created";
}

void GUICommunicationSocket::collectDataFromGlobalConnection()
{
   QLOG_INFO() << "New global channel connection detected";
   QLocalSocket *clientConnection = globalChannel->nextPendingConnection();
   connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
   if (clientConnection->waitForReadyRead(30000)) {
      QDataStream in(clientConnection);
      in.setVersion(QDataStream::Qt_5_4);
      QString individualChannelName;
      in >> individualChannelName;
      in << createIndividualChannel(individualChannelName);
   }
   else {
      QLOG_WARN() << "New connection detected, but no data received - client will be disconnected";
      clientConnection->disconnectFromServer();
   }
}

bool GUICommunicationSocket::createIndividualChannel(const QString &individualChannelName)
{
   QLocalServer *individualChannel = new QLocalServer(this);
   individualChannel->setSocketOptions(QLocalServer::WorldAccessOption);
   if (!individualChannel->listen(individualChannelName)) {
      QLOG_ERROR() << "Unable to create individual communication channel: " << individualChannel->errorString();
      return false;
   }
   QPointer<IndividualCommunicationChannel> channel(new IndividualCommunicationChannel(individualChannel, this));
   connect(channel, SIGNAL(noActiveConnections(QString)), this, SLOT(removeIndividualChannel(QString)));
   individualChannels->insert(individualChannelName, channel);
   QLOG_INFO() << "Individual communication channel created: " << individualChannelName;
   return true;
}

void GUICommunicationSocket::removeIndividualChannel(const QString &individualChannelName)
{
   individualChannels->take(individualChannelName)->deleteLater();
   QLOG_INFO() << "Individual communication channel removed: " << individualChannelName;
}


