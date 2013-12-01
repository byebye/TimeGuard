#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
  Q_OBJECT
public:
  explicit User(QObject *parent = 0);
  void logOff();
  bool lengthenTime();
  QString getName();
signals:
  
public slots:
  
private:
  QString const name;
  QString avaiableTime;

  QString getSystemUsername();
};

#endif // USER_H
