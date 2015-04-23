#ifndef GUICOMMUNICATIONSOCKET_H
#define GUICOMMUNICATIONSOCKET_H

#include <QObject>
#include <QtNetwork>
#include <QHash>
#include <QPointer>
#include <QVariantMap>
#include "individualcommunicationchannel.h"
#include "user.h"

class GUICommunicationSocket : public QObject
{
   Q_OBJECT
public:
   explicit GUICommunicationSocket(QObject *parent = 0);
   ~GUICommunicationSocket();

signals:
   void newUserSessionStarted(const User &user);
   void settingsPackageReceived(const QStringList &users, const QVariant &settings);
public slots:
   void collectDataFromGlobalConnection();
   void removeIndividualChannel(const QString &individualChannelName);
   void passReceivedPackage(const QStringList &users, const QVariant &values);
private:
   QLocalServer *globalChannel;
   QHash<QString, QPointer<IndividualCommunicationChannel>> *individualChannels;

   bool processDataPackage(const QVariantMap &package);
   void createGlobalChannel();
   bool createIndividualChannel(const QString &individualChannelName);
};

namespace CommunicationSocket {
   const QString pipeNamePrefix = "\\\\.\\pipe\\";
   const QString globalChannelName = pipeNamePrefix + "TimeGuardGlobalSocket";
}

#endif // GUICOMMUNICATIONSOCKET_H
