#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
   Q_OBJECT
public:
   explicit User(const QString &name, int sessionId, QObject *parent = 0);
   ~User();

   int getSessionId() const;
   void setSessionId(int value);

signals:

public slots:
private:
   QString name;
   int sessionId;

   bool isActive() const;
};

#endif // USER_H
