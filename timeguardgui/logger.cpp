#include "logger.h"
#include <QTime>
#include <QDate>

Logger::Logger(QObject *parent, FileManager *fileManager) :
  QObject(parent),
  fileManager(fileManager),
  logFile("TimeGuard")
{
}

void Logger::log(QString filename, QString event)
{
  QString logMsg("[" + getCurrentDateAndTime() + "] " + event);
  fileManager->saveLog(filename, logMsg);
}

QString Logger::getCurrentDateAndTime()
{
  return QDate::currentDate().toString("yyyy.MM.dd") + " " +
         QTime::currentTime().toString("hh:mm:ss");
}

void Logger::logAdmin(QString info)
{
  log(logFile, "Admin - " + info);
}

void Logger::logUserAndGlobal(QString username, QString info)
{
  log(username, username + " - " + info);
  log(logFile, username + " - " + info);
}

void Logger::logAdminLoggedIn()
{
  logAdmin("log in");
}

void Logger::logAdminLoggedOff()
{
  logAdmin("log off");
}

void Logger::logAdminPasswordChanged()
{
  logAdmin("password changed");
}

void Logger::logUserLoggedIn(QString username)
{
  logUserAndGlobal(username, "log in");
}

void Logger::logUserLoggedOff(QString username)
{
  logUserAndGlobal(username, "log off");
}

void Logger::logUserLimitEnabled(QString username)
{
  logUserAndGlobal(username, "limit enabled");
}

void Logger::logUserLimitDisabled(QString username)
{
  logUserAndGlobal(username, "limit disabled");
}

void Logger::logUserLimitChanged(QString username, QString limit)
{
  logUserAndGlobal(username, "limit changed to " + limit);
}

void Logger::logUserTimePaused(QString username, QString time)
{
  logUserAndGlobal(username, "time paused " + time);
}

void Logger::logUserTimeStarted(QString username, QString time)
{
  logUserAndGlobal(username, "time started " + time);
}

void Logger::logUserTimeReset(QString username, QString time)
{
  logUserAndGlobal(username, "time reset to " + time);
}
