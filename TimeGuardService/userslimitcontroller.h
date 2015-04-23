#ifndef USERSLIMITCONTROLLER_H
#define USERSLIMITCONTROLLER_H

#include <QObject>
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
   void newUserSession(const User &user);
   void processSettingsPackage(const QStringList &users, const QVariant &settings);
private:
   GUICommunicationSocket *communicationSocket;
   LimitSettingsManager *limitSettingsManager;
   UsersSessionManager *usersSessionManager;
};

#endif // USERSLIMITCONTROLLER_H
