#ifndef GUICOMMUNICATIONSOCKET_H
#define GUICOMMUNICATIONSOCKET_H

#include <QObject>
#include <QtNetwork>
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
   QFile *logFile;
   QTextStream *logFileStream;

   void createGlobalServer();
};

#endif // GUICOMMUNICATIONSOCKET_H
