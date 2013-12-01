#include "file_manager.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

FileManager::FileManager() : settingsDir("settings/")
{
  if(!QDir(settingsDir).exists())
    QDir().mkdir(settingsDir);
}

QString FileManager::readFromFile(QString userName)
{
  QFile settingsFile(settingsDir + userName + ".set");
  if(!settingsFile.open(QIODevice::ReadOnly))
    return NULL;
  QTextStream textStream(&settingsFile);
  if(!textStream.atEnd())
    return textStream.readLine();
  else
    return NULL;
}

bool FileManager::saveToFile(QString userName, QString data)
{
  QFile settingsFile(settingsDir + userName + ".set");
  if(!settingsFile.open(QIODevice::ReadWrite | QIODevice::Append))
    return false;
  QTextStream textStream(&settingsFile);
  textStream << data << endl;

  settingsFile.close();
  return true;
}
