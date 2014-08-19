#ifndef USER_H
#define USER_H

#include <QObject>
#include <QTime>
#include "filemanager.h"
#include "systemquery.h"
#include "logger.h"
#include "timer.h"

class User : public QObject
{
  Q_OBJECT
public:
  explicit User(QObject *parent = 0, FileManager *fileManager = 0,
                 SystemQuery *systemQuery = 0, Logger *logger = 0);
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
  void saveTimeRemaining(QTime time);
private:
  FileManager *fileManager;
  SystemQuery *systemQuery;
  Logger *logger;
  QString const name;
  QTime *timeRemaining;
  bool limitActive;

  QString getSystemUsername();
  QTime readTimeRemaining();
  bool readLimitActive();
};

#endif // USER_H
