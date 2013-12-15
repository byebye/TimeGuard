#include "logger.h"
#include <QTime>
#include <QDate>
#include <QDebug>

Logger::Logger(QObject *parent, FileManager *fileManager) :
  QObject(parent),
  fileManager(fileManager)
{
}

void Logger::log(QString filename, QString event)
{
  QString logMsg("[" + getCurrentDateAndTime() + "] " + event);
  qDebug() << logMsg << endl;
  fileManager->saveStats(filename, logMsg );
}

void Logger::logIn(QString username)
{
  log(username, username + " - log in");
}

void Logger::logOff(QString username)
{
  log(username, username + " - log off");
}

QString Logger::getCurrentDateAndTime()
{
  return QDate::currentDate().toString("yyyy.MM.dd") + " " +
         QTime::currentTime().toString("hh:mm:ss");
}
