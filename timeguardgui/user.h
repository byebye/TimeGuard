#ifndef USER_H
#define USER_H

#include <QObject>
#include <QTime>
#include "filemanager.h"
#include "systemquery.h"
#include "logger.h"
#include "timer.h"
#include "timelimit.h"

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
  TimeLimit getTimeRemaining();
  TimeLimit readLimit(FileManager::SettingName limitName);
  void saveLimit(TimeLimit limit, FileManager::SettingName limitName);
  void saveLimit(QString limit, FileManager::SettingName limitName);
  void resetTimeRemaining();
  int getSaveTimePeriod();
  bool isLimitEnabled();
  void setLimitEnabled(bool active);
signals:
  
public slots:
  void saveTimeRemaining(TimeLimit time);
private:
  FileManager *fileManager;
  SystemQuery *systemQuery;
  Logger *logger;
  QString const name;
  TimeLimit *timeRemaining;
  bool limitEnabled;

  QString getSystemUsername();
  TimeLimit readTimeRemaining();
  bool readLimitEnabled();
  void refreshDailyTimeRemaining();
  void refreshWeeklyTimeRemaining();
  void refreshMonthlyTimeRemaining();
};

#endif // USER_H
