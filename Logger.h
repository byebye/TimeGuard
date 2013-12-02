#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>
#include "FileManager.h"

class Logger : public QObject
{
  Q_OBJECT
public:
  explicit Logger(QObject *parent = 0, FileManager *fileManager = 0);
  void log(QString filename, QString event);
  void logIn(QString username);
  void logOff(QString username);
  QString getCurrentDateAndTime();
signals:
  
public slots:

private:
  FileManager *fileManager;
  
};

#endif // LOGGER_H
