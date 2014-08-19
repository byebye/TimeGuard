#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include "filemanager.h"

class Logger : public QObject
{
  Q_OBJECT
public:
  explicit Logger(QObject *parent = 0, FileManager *fileManager = 0);
  void log(QString filename, QString event);
  QString getCurrentDateAndTime();
signals:
  
public slots:
  void logAdminLoggedIn();
  void logAdminLoggedOff();
  void logAdminPasswordChanged();
  void logUserLoggedIn(QString username);
  void logUserLoggedOff(QString username);
  void logUserLimitChanged(QString username, QString limit, FileManager::SettingName whichLimit);
  void logUserLimitEnabled(QString username);
  void logUserLimitDisabled(QString username);
  void logUserTimePaused(QString username, QString time);
  void logUserTimeStarted(QString username, QString time);
  void logUserTimeReset(QString username, QString time);
private:
  FileManager *fileManager;
  QString const logFile;

  void logAdmin(QString info);
  void logUserAndGlobal(QString username, QString info);
};

#endif // LOGGER_H
