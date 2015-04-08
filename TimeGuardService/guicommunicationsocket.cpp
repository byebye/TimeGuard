#include "guicommunicationsocket.h"
#include <QTime>
#include "QsLog.h"

QString GUICommunicationSocket::globalSocketName = "TimeGuardGlobalSocket";

GUICommunicationSocket::GUICommunicationSocket(QObject *parent) : QObject(parent)
{
   individualSockets = new QHash<QString, QLocalServer*>();
   createGlobalServer();

}

GUICommunicationSocket::~GUICommunicationSocket()
{
   delete individualSockets;
   delete globalServer;
}

void GUICommunicationSocket::createGlobalServer()
{
   globalServer = new QLocalServer(this);
   globalServer->setSocketOptions(QLocalServer::WorldAccessOption);
   if(!globalServer->listen(globalSocketName)) {
      QLOG_FATAL() << "Unable to create global communication channel";
      return;
   }
   connect(globalServer, SIGNAL(newConnection()),
           this, SLOT(collectDataFromGlobalConnection()));
   QLOG_INFO() << "Global communication channel created";
}

void GUICommunicationSocket::collectDataFromGlobalConnection()
{
   QLOG_INFO() << "New connection detected";
   QLocalSocket *clientConnection = globalServer->nextPendingConnection();
   connect(clientConnection, SIGNAL(disconnected()),
           clientConnection, SLOT(deleteLater()));
   if (clientConnection->waitForReadyRead(30000)) {
      QDataStream in(clientConnection);
      in.setVersion(QDataStream::Qt_5_4);
      QString individualChannelName;
      in >> individualChannelName;
      in << createIndividualServer(individualChannelName);
   }
   else {
      QLOG_WARN() << "New connection detected, but no data received - client will be disconnected";
      clientConnection->disconnectFromServer();
   }
}

bool GUICommunicationSocket::createIndividualServer(const QString &individualChannelName)
{
   QLocalServer *individualServer = new QLocalServer(this);
   individualServer->setSocketOptions(QLocalServer::WorldAccessOption);
   if(!individualServer->listen(individualChannelName)) {
      QLOG_FATAL() << "Unable to create individual communication channel";
      return false;
   }
   individualSockets->insert(individualChannelName, individualServer);
   QLOG_INFO() << "Individual communication channel created: " << individualChannelName;
   return true;
}


