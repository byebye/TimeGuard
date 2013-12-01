#include "FileManager.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

FileManager::FileManager() : settingsDir("settings/"), statsDir("stats/")
{
  if(!QDir(settingsDir).exists())
    QDir().mkdir(settingsDir);
  if(!QDir(statsDir).exists())
    QDir().mkdir(statsDir);
}

QString FileManager::readFromFile(QString filename)
{
  QFile file(settingsDir + filename + ".set");
  if(!file.open(QIODevice::ReadOnly))
    return NULL;
  QTextStream fileStream(&file);
  if(!fileStream.atEnd())
    return fileStream.readLine();
  else
    return NULL;
}

bool FileManager::saveToFile(QString filename, QString data)
{
  QFile file(statsDir + filename + ".sts");
  if(!file.open(QIODevice::ReadWrite | QIODevice::Append))
    return false;
  QTextStream fileStream(&file);
  fileStream << data << endl;
  return true;
}
