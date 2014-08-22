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
  QTime readLimit(FileManager::SettingName limitName);
  void saveLimit(QTime limit, FileManager::SettingName limitName);
  void saveLimit(QString limit, FileManager::SettingName limitName);
  void resetTimeRemaining();
  int getSaveTimePeriod();
  bool isLimitEnabled();
  void setLimitEnabled(bool active);
signals:
  
public slots:
  void saveTimeRemaining(QTime time);
private:
  FileManager *fileManager;
  SystemQuery *systemQuery;
  Logger *logger;
  QString const name;
  QTime *timeRemaining;
  bool limitEnabled;

  QString getSystemUsername();
  QTime readTimeRemaining();
  bool readLimitEnabled();
};

#endif // USER_H
