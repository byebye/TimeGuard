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
  QTime readTimeLimit();
  void resetTimeRemaining();
  int getSaveTimePeriod();
  bool isLimitActive();
  void setLimitActive(bool active);
signals:
  
public slots:
  void saveTimeRemaining();
private:
  FileManager *fileManager;
  Logger *logger;
  QString const name;
  QTime *timeRemaining;
  int const saveTimePeriod;
  bool limitActive;

  QString getSystemUsername();
  QTime readTimeRemaining();
  bool readLimitActive();
  void saveLogInTime();
  void saveLogOffTime();
};

#endif // USER_H
