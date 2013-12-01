#ifndef USER_H
#define USER_H

#include <QObject>
#include <QTime>
#include "FileManager.h"

class User : public QObject
{
  Q_OBJECT
public:
  explicit User(QObject *parent = 0, FileManager *fileManager = 0);
  ~User();
  void logOff();
  bool lengthenTime();

  QString getName();
  QTime getAvaiableTime();
  int getSaveTimePeriod();
signals:
  
public slots:
  void saveAvaiableTime();
  
private:
  FileManager *fileManager;
  QString const name;
  QTime *avaiableTime;
  int const saveTimePeriod;

  QString getSystemUsername();
  void readAvaiableTime();
  void saveLogInTime();
  void saveLogOffTime();
};

#endif // USER_H
