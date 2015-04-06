#ifndef USERSLIMITCONTROLLER_H
#define USERSLIMITCONTROLLER_H

#include <QObject>
#include <QHash>
#include <QtNetwork>
#include "userlimittimer.h"
#include "limitsettingsmanager.h"

class UsersLimitController : public QObject
{
   Q_OBJECT
public:
   explicit UsersLimitController(QObject *parent = 0);
   ~UsersLimitController();

signals:

public slots:
   void collectData();
private:
  QHash<QString, UserLimitTimer> *usersLimitTimer;
  LimitSettingsManager *limitSettingsManager;
  QLocalServer *server;
};

#endif // USERSLIMITCONTROLLER_H
