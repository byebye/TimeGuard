#ifndef GUICOMMUNICATIONSOCKET_H
#define GUICOMMUNICATIONSOCKET_H

#include <QObject>
#include <QtNetwork>
#include <QHash>
#include <QPointer>
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
public slots:
   void collectDataFromGlobalConnection();
   void removeIndividualChannel(const QString &individualChannelName);
private:
   static QString globalChannelName;
   QLocalServer *globalChannel;
   QHash<QString, QPointer<IndividualCommunicationChannel>> *individualChannels;

   void createGlobalChannel();
   bool createIndividualChannel(const QString &individualChannelName);
};

#endif // GUICOMMUNICATIONSOCKET_H
