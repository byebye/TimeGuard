#ifndef INDIVIDUALCOMMUNICATIONCHANNEL_H
#define INDIVIDUALCOMMUNICATIONCHANNEL_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>
#include <QStringList>
#include "limitsettingsmanager.h"

class IndividualCommunicationChannel : public QObject
{
   Q_OBJECT
public:
   explicit IndividualCommunicationChannel(QLocalServer *channel, QObject *parent = 0);
   ~IndividualCommunicationChannel();

signals:
   void noActiveConnections(const QString &channelName);
   void packageReceived(const QStringList &users, const QVariant &values);
public slots:
   void clientConnected();
   void clientDisconnected();
   void noActiveConnections();
   bool collectData();
private:
   QLocalServer *channel;
   QLocalSocket *clientConnection;
   QTimer *waitForConnectionTimer;

   bool processDataPackage(const QVariantMap &package);
};

#endif // INDIVIDUALCOMMUNICATIONCHANNEL_H
