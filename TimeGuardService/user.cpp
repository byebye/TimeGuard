#include "user.h"

User::User(const QString &name, const QString &GUID, int sessionId, QObject *parent) : QObject(parent),
   name(name), GUID(GUID), sessionId(sessionId)
{

}

User::~User()
{

}

