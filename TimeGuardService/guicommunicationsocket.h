#ifndef GUICOMMUNICATIONSOCKET_H
#define GUICOMMUNICATIONSOCKET_H

#include <QObject>

class GUICommunicationSocket : public QObject
{
   Q_OBJECT
public:
   explicit GUICommunicationSocket(QObject *parent = 0);
   ~GUICommunicationSocket();

signals:

public slots:
};

#endif // GUICOMMUNICATIONSOCKET_H
