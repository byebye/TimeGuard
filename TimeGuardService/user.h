#ifndef USER_H
#define USER_H

#include <QString>
#include <QHash>
#include <QDebug>

class User
{
public:
   explicit User(const QString &name, ulong sessionId);
   ~User();

   QString getName() const;
   ulong getSessionId() const;
   bool isActive() const;

   bool operator==(const User &user) const;
   friend QDebug operator <<(QDebug stream, const User &user);
private:
   QString name;
   ulong sessionId;
};

inline uint qHash(const User &user, uint seed)
{
    return qHash(user.getName(), seed) ^ user.getSessionId();
}

#endif // USER_H
