#ifndef USERSLIMITCONTROLLER_H
#define USERSLIMITCONTROLLER_H

#include <QObject>
#include <QHash>
#include "userlimittimer.h"
#include "limitsettingsmanager.h"
#include "guicommunicationsocket.h"

class UsersLimitController : public QObject
{
   Q_OBJECT
public:
   explicit UsersLimitController(QObject *parent = 0);
   ~UsersLimitController();

signals:

public slots:

private:
   GUICommunicationSocket *communicationSocket;
   QHash<QString, UserLimitTimer> *usersLimitTimer;
   LimitSettingsManager *limitSettingsManager;
};

#endif // USERSLIMITCONTROLLER_H
