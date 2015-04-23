#ifndef SERVICECOMMUNICATIONSOCKET_H
#define SERVICECOMMUNICATIONSOCKET_H

#include <QObject>
#include <QtNetwork>
#include <QVariantMap>

class ServiceCommunicationSocket : public QObject
{
   Q_OBJECT
public:
   explicit ServiceCommunicationSocket(QObject *parent = 0);
   ~ServiceCommunicationSocket();

   bool createIndividualCommunicationChannel();
   QString getChannelName() const;
   bool sendPackage(const QVariantMap &package);
signals:

public slots:
private:
   QLocalSocket *socket;
   QString individualChannelName;
   const QString userName;
   const ulong sessionId;

   QString generateIndividualChannelName(ulong sessionId);
   bool sendIndividualChannelName();
   bool sendPackage(QLocalSocket *socket, const QVariantMap &package);
};

namespace CommunicationSocket {
   const QString pipeNamePrefix = "\\\\.\\pipe\\";
   const QString globalChannelName = pipeNamePrefix + "TimeGuardGlobalSocket";
}

#endif // SERVICECOMMUNICATIONSOCKET_H
