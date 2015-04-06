#ifndef GUICOMMUNICATIONSOCKET_H
#define GUICOMMUNICATIONSOCKET_H

#include <QObject>
#include <QtNetwork>
#include <QHash>
#include <QFile>
#include <QTextStream>

class GUICommunicationSocket : public QObject
{
   Q_OBJECT
public:
   explicit GUICommunicationSocket(QObject *parent = 0);
   ~GUICommunicationSocket();

signals:

public slots:
   void collectDataFromGlobalConnection();
private:
   static QString globalSocketName;
   QLocalServer *globalServer;
   QHash<QString, QLocalServer*> *individualSockets;
   QFile *logFile;
   QTextStream *logFileStream;


   void createGlobalServer();
   void createIndividualServer(const QString &individualChannelName);
};

#endif // GUICOMMUNICATIONSOCKET_H
