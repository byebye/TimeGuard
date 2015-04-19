#ifndef USERSLIMITCONTROLLER_H
#define USERSLIMITCONTROLLER_H

#include <QObject>
#include <QHash>
#include <QPointer>
#include "userlimittimer.h"
#include "limitsettingsmanager.h"
#include "guicommunicationsocket.h"
#include "usersessionmanager.h"

class UsersLimitController : public QObject
{
   Q_OBJECT
public:
   explicit UsersLimitController(QObject *parent = 0);
   ~UsersLimitController();

signals:

public slots:
   void newUserSessionStarted(const User &user);
private:
   GUICommunicationSocket *communicationSocket;
   QHash<QString, QPointer<UserLimitTimer>> *usersLimitTimer;
   LimitSettingsManager *limitSettingsManager;
   UsersSessionManager *usersSessionManager;
};

#endif // USERSLIMITCONTROLLER_H
