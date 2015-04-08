#include "individualcommunicationchannel.h"

IndividualCommunicationChannel::IndividualCommunicationChannel(QLocalServer *channel, QObject *parent)
   : QObject(parent), channel(channel)
{
   connect(channel, SIGNAL(newConnection()), this, SLOT(clientConnected()));
}

IndividualCommunicationChannel::~IndividualCommunicationChannel()
{
   delete channel;
}

void IndividualCommunicationChannel::clientConnected()
{
   clientConnection = channel->nextPendingConnection();
   connect(clientConnection, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
}

void IndividualCommunicationChannel::clientDisconnected()
{
   emit disconnected(channel->serverName());
}

