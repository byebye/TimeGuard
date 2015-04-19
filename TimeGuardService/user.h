#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
   explicit User(const QString &name, ulong sessionId);
   User(const User &user);
   ~User();

   QString getName() const;
   ulong getSessionId() const;
   bool isActive() const;

   bool operator==(const User &user) const;
private:
   QString name;
   ulong sessionId;
};

#endif // USER_H
