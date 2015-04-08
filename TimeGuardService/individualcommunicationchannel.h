#ifndef INDIVIDUALCOMMUNICATIONCHANNEL_H
#define INDIVIDUALCOMMUNICATIONCHANNEL_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>

class IndividualCommunicationChannel : public QObject
{
   Q_OBJECT
public:
   explicit IndividualCommunicationChannel(QLocalServer *channel, QObject *parent = 0);
   ~IndividualCommunicationChannel();

signals:
   void noActiveConnections(const QString &channelName);
public slots:
   void clientConnected();
   void clientDisconnected();
   void noActiveConnections();
private:
   QLocalServer *channel;
   QLocalSocket *clientConnection;
   QTimer *waitForConnectionTimer;
};

#endif // INDIVIDUALCOMMUNICATIONCHANNEL_H
