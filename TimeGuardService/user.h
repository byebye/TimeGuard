#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
   Q_OBJECT
public:
   explicit User(const QString &name, const QString &GUID, int sessionId, QObject *parent = 0);
   ~User();

signals:

public slots:
private:
   QString name;
   QString GUID;
   int sessionId;
};

#endif // USER_H
