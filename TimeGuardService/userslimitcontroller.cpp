#include "userslimitcontroller.h"

UsersLimitController::UsersLimitController(QObject *parent) : QObject(parent)
{
   usersLimitTimer = new QHash<QString, UserLimitTimer>();
   limitSettingsManager = new LimitSettingsManager();
   QFile logFile("D:/timeguard.log");
   logFile.open(QFile::ReadWrite | QFile::Text);
   QTextStream logFileStream(&logFile);
   server = new QLocalServer();
   server->setSocketOptions(QLocalServer::WorldAccessOption);
   if(!server->listen("timeguardserver")) {
      logFileStream << "Unable to start server\n";
      logFile.close();
      return;
   }
   logFileStream << "Server started\n";
   connect(server, SIGNAL(newConnection()),
           this, SLOT(collectData()));
}

UsersLimitController::~UsersLimitController()
{
   QLocalSocket *clientConnection = server->nextPendingConnection();
   connect(clientConnection, SIGNAL(disconnected()),
           clientConnection, SLOT(deleteLater()));
   QDataStream in(clientConnection);
   in.setVersion(QDataStream::Qt_5_4);
   QString secretName;
   in >> secretName;
   QFile logFile("D:/timeguard.log");
   logFile.open(QFile::ReadWrite | QFile::Text);
   QTextStream logFileStream(&logFile);
   logFileStream << secretName;
   logFile.close();
}

void UsersLimitController::collectData()
{

}

