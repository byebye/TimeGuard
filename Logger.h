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
  void logUserLimitChanged(QString username, QString limit);
  void logUserLimitActivated(QString username);
  void logUserLimitDeactivated(QString username);
  void logUserTimePaused(QString username, QString time);
  void logUserTimeStarted(QString username, QString time);
  void logUserTimeReset(QString username, QString time);
private:
  FileManager *fileManager;
  QString const logFile;
};

#endif // LOGGER_H
