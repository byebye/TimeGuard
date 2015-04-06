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
   static QString globalSocketName;
   QLocalSocket *socket;
   QString channelName;

   QString generateIndividualChannelName();
   bool sendIndividualChannelName();
};

#endif // SERVICECOMMUNICATIONSOCKET_H