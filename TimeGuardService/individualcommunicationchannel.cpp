#include "individualcommunicationchannel.h"
#include "QsLog.h"
#include <QMutexLocker>

IndividualCommunicationChannel::IndividualCommunicationChannel(QLocalServer *channel, QObject *parent)
   : QObject{parent}, channel{channel}, clientConnection{nullptr}, waitForConnectionTimer{new QTimer{this}}
{
   connect(this->channel, SIGNAL(newConnection()), this, SLOT(clientConnected()));
   connect(waitForConnectionTimer, SIGNAL(timeout()), this, SLOT(noActiveConnections()));
   waitForConnectionTimer->start(5000);
}

IndividualCommunicationChannel::~IndividualCommunicationChannel()
{
   channel->close();
   delete channel;
}

void IndividualCommunicationChannel::clientConnected()
{
   waitForConnectionTimer->stop();
   clientConnection = channel->nextPendingConnection();
   connect(clientConnection, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
   connect(clientConnection, SIGNAL(readyRead()), this, SLOT(collectData()));
}

void IndividualCommunicationChannel::clientDisconnected()
{
   clientConnection->deleteLater();
   noActiveConnections();
}

void IndividualCommunicationChannel::noActiveConnections()
{
   waitForConnectionTimer->stop();
   emit noActiveConnections(channel->serverName());
}

bool IndividualCommunicationChannel::collectData()
{
   QDataStream dataStream(clientConnection);
   dataStream.setVersion(QDataStream::Qt_5_4);
   QVariantMap package;
   dataStream >> package;
   bool success = false;
   if (dataStream.status() != QDataStream::Ok) {
      QLOG_ERROR() << "Received data package corrupted";
      success = false;
   }
   else
      success = processDataPackage(package);

   QVariantMap feedback{
      {"command", "feedback"},
      {"username", package["username"]},
      {"success", success}
   };
   dataStream << feedback;
   return success;
}

bool IndividualCommunicationChannel::processDataPackage(const QVariantMap &package)
{
   QLOG_DEBUG() << "Process received data package";
   emit packageReceived(package["users"].toStringList(), package["values"]);
   return true;
}


