#include "guicommunicationsocket.h"
#include <QTime>

QString GUICommunicationSocket::globalSocketName = "TimeGuardGlobalSocket";

GUICommunicationSocket::GUICommunicationSocket(QObject *parent) : QObject(parent)
{
   logFile = new QFile("D:/timeguard.log");
   logFile->open(QFile::ReadWrite | QFile::Text);
   logFileStream = new QTextStream(logFile);
   (*logFileStream) << "Creating server: " << QTime::currentTime().toString("hh:mm") << "\n";
   logFileStream->flush();
   createGlobalServer();
}

GUICommunicationSocket::~GUICommunicationSocket()
{
   logFile->close();
   delete logFile;
}

void GUICommunicationSocket::createGlobalServer()
{
   globalServer = new QLocalServer(this);
   globalServer->setSocketOptions(QLocalServer::WorldAccessOption);
   if(!globalServer->listen(globalSocketName)) {
      (*logFileStream) << "Unable to start server\n";
      logFileStream->flush();
      return;
   }
   (*logFileStream) << "Server started\n";
   logFileStream->flush();
   connect(globalServer, SIGNAL(newConnection()),
           this, SLOT(collectDataFromGlobalConnection()));
}

void GUICommunicationSocket::collectDataFromGlobalConnection()
{
   QLocalSocket *clientConnection = globalServer->nextPendingConnection();
   connect(clientConnection, SIGNAL(disconnected()),
           clientConnection, SLOT(deleteLater()));
   if (clientConnection->waitForReadyRead(30000)) {
      QDataStream in(clientConnection);
      in.setVersion(QDataStream::Qt_5_4);
      QString individualChannelName;
      in >> individualChannelName;
      (*logFileStream) << "Channel name to create: " << individualChannelName << "\n";
      logFileStream->flush();
   }
   else {
      (*logFileStream) << "No data received\n";
      logFileStream->flush();
   }
}


