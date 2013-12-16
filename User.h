#ifndef USER_H
#define USER_H

#include <QObject>
#include <QTime>
#include "filemanager.h"
#include "logger.h"

class User : public QObject
{
  Q_OBJECT
public:
  explicit User(QObject *parent = 0, FileManager *fileManager = 0,
                Logger *logger = 0);
  ~User();
  void logOff();
  bool lengthenTime();

  QString getName();
  QTime getTimeRemaining();
  void resetTimeRemaining();
  int getSaveTimePeriod();
signals:
  
public slots:
  void saveTimeRemaining();
private:
  FileManager *fileManager;
  Logger *logger;
  QString const name;
  QTime *timeRemaining;
  QTime *timeLimit;
  int const saveTimePeriod;

  QString getSystemUsername();
  QTime readTimeRemaining();
  QTime readTimeLimit();
  void saveLogInTime();
  void saveLogOffTime();
};

#endif // USER_H
