#include "User.h"

User::User(QObject *parent, QString username) :
  QObject(parent),
  name(username)
{
}
