#ifndef SYSTEMQUERY_H
#define SYSTEMQUERY_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "messages.h"

class SystemQuery : public QObject
{
  Q_OBJECT
public:
  explicit SystemQuery(QObject *parent = 0, Messages *messages = 0);
  ~SystemQuery();

  QString getCurrentUserName();
  QStringList getUsersList();
private:
  Messages *messages;
};

#endif // SYSTEMQUERY_H
