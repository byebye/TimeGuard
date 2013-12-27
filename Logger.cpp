#include "logger.h"
#include <QTime>
#include <QDate>
#include <QDebug>

Logger::Logger(QObject *parent, FileManager *fileManager) :
  QObject(parent),
  fileManager(fileManager),
  logFile("log.txt")
{
}

void Logger::log(QString filename, QString event)
{
  QString logMsg("[" + getCurrentDateAndTime() + "] " + event);
  qDebug() << logMsg << endl;
  fileManager->saveLog(filename, logMsg);
}

QString Logger::getCurrentDateAndTime()
{
  return QDate::currentDate().toString("yyyy.MM.dd") + " " +
         QTime::currentTime().toString("hh:mm:ss");
}

void Logger::logAdminLoggedIn()
{
  log(logFile, "Admin - log in");
}

void Logger::logAdminLoggedOff()
{
  log(logFile, "Admin - log off");
}

void Logger::logAdminPasswordChanged()
{
  log(logFile, "Admin - password changed");
}

void Logger::logUserLoggedIn(QString username)
{
  log(username, username + " - log in");
  log(logFile, username + " - log in");
}

void Logger::logUserLoggedOff(QString username)
{
  log(username, username + " - log off");
  log(logFile, username + " - log off");
}

void Logger::logUserLimitActivated(QString username)
{
  log(username, username + " - limit activated");
  log(logFile, username + " - limit activated");
}

void Logger::logUserLimitDeactivated(QString username)
{
  log(username, username + " - limit deactivated");
  log(logFile, username + " - limit deactivated");
}

void Logger::logUserLimitChanged(QString username, QString limit)
{
  log(username, username + " - limit changed to " + limit);
  log(logFile, username + " - limit changed to " + limit);
}

void Logger::logUserTimePaused(QString username, QString time)
{
  log(username, username + " - time paused " + time);
  log(logFile, username + " - time paused " + time);
}

void Logger::logUserTimeStarted(QString username, QString time)
{
  log(username, username + " - time started " + time);
  log(logFile, username + " - time started " + time);
}

void Logger::logUserTimeReset(QString username, QString time)
{
  log(username, username + " - time reset to " + time);
  log(logFile, username + " - time reset to " + time);
}
