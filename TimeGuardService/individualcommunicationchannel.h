#ifndef INDIVIDUALCOMMUNICATIONCHANNEL_H
#define INDIVIDUALCOMMUNICATIONCHANNEL_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>

class IndividualCommunicationChannel : public QObject
{
   Q_OBJECT
public:
   explicit IndividualCommunicationChannel(QLocalServer *channel, QObject *parent = 0);
   ~IndividualCommunicationChannel();

signals:
   void disconnected(const QString &channelName);
public slots:
   void clientConnected();
   void clientDisconnected();
private:
   QLocalServer *channel;
   QLocalSocket *clientConnection;
};

#endif // INDIVIDUALCOMMUNICATIONCHANNEL_H
