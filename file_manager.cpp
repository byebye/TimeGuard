#include "file_manager.h"
#include <QFile>
#include <QDir>

FileManager::FileManager() : settingsDir("settings/")
{
  if(!QDir(settingsDir).exists())
    QDir().mkdir(settingsDir);
}

bool FileManager::readFromFile(QString userName)
{
  QFile settingsFile(settingsDir + userName + ".set");
  if(!settingsFile.open(QIODevice::ReadWrite))
  {
    return false;
  }
  return true;
}

bool FileManager::saveToFile(QString userName)
{

  QFile settingsFile(settingsDir + userName + ".set");
  if(!settingsFile.open(QIODevice::ReadWrite))
    return false;
  return true;
}
