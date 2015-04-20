#ifndef SERVICECOMMUNICATIONSOCKET_H
#define SERVICECOMMUNICATIONSOCKET_H

#include <QObject>
#include <QtNetwork>

class ServiceCommunicationSocket : public QObject
{
   Q_OBJECT
public:
   explicit ServiceCommunicationSocket(QObject *parent = 0);
   ~ServiceCommunicationSocket();

   bool createIndividualCommunicationChannel();
   QString getChannelName() const;

signals:

public slots:
private:
   QLocalSocket *socket;
   QString individualChannelName;

   QString generateIndividualChannelName(ulong sessionId);
   ulong getSessionId();
   QString getUserName();
   bool sendIndividualChannelName();
};

namespace CommunicationSocket {
   const QString pipeNamePrefix = "\\\\.\\pipe\\";
   const QString globalChannelName = pipeNamePrefix + "TimeGuardGlobalSocket";
}

#endif // SERVICECOMMUNICATIONSOCKET_H
