#include "individualcommunicationchannel.h"
#include "QsLog.h"

IndividualCommunicationChannel::IndividualCommunicationChannel(QLocalServer *channel, QObject *parent)
   : QObject(parent), channel(channel), clientConnection(nullptr)
{
   connect(channel, SIGNAL(newConnection()), this, SLOT(clientConnected()));
   waitForConnectionTimer = new QTimer(this);
   connect(waitForConnectionTimer, SIGNAL(timeout()), this, SLOT(noActiveConnections()));
   waitForConnectionTimer->start(50000);
}

IndividualCommunicationChannel::~IndividualCommunicationChannel()
{
   delete channel;
}

void IndividualCommunicationChannel::clientConnected()
{
   waitForConnectionTimer->stop();
   clientConnection = channel->nextPendingConnection();
   connect(clientConnection, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
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

