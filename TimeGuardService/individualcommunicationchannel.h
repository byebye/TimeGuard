#ifndef INDIVIDUALCOMMUNICATIONCHANNEL_H
#define INDIVIDUALCOMMUNICATIONCHANNEL_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QTimer>
#include <QStringList>
#include <QMutex>
#include <QWaitCondition>
#include "limitsettingsmanager.h"

namespace Communication {
   enum FeedbackStatus { Unknown, Fail, Success };
}

class IndividualCommunicationChannel : public QObject
{
   Q_OBJECT
public:
   explicit IndividualCommunicationChannel(QLocalServer *channel, QObject *parent = 0);
   ~IndividualCommunicationChannel();

   bool sendPackage(const QVariantMap &package);
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
   QMutex mutex;
   QWaitCondition feedbackReceived;
   Communication::FeedbackStatus feedbackStatus;

   bool processDataPackage(const QVariantMap &package);
   bool waitForFeedback(ulong timeout);
   bool processFeedback(bool status);
};

#endif // INDIVIDUALCOMMUNICATIONCHANNEL_H
