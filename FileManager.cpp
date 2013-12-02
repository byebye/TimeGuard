#include "FileManager.h"
#include <QDir>
#include <QTextStream>
#include <QDebug>

FileManager::FileManager() :
  settingsDir("settings/"), statsDir("stats/"),
  settingsExt(".set"), statsExt(".sts")
{
  if(!QDir(settingsDir).exists())
    QDir().mkdir(settingsDir);
  if(!QDir(statsDir).exists())
    QDir().mkdir(statsDir);
}

QString FileManager::readStats(QString filename)
{
  return readFromFile(filename, statsDir, statsExt);
}

QString FileManager::readSettings(QString filename)
{
  return readFromFile(filename, settingsDir, settingsExt);
}

QString FileManager::readFromFile(QString filename, QString dir, QString ext)
{
  QFile file(dir + filename + ext);
  if(!file.open(QIODevice::ReadOnly))
  {
    qDebug() << "Unable to open" << file.fileName() << endl;
    return NULL;
  }

  QTextStream fileStream(&file);
  QString fileString;
  while(!fileStream.atEnd())
    fileString += fileStream.readLine();
  return fileString;
}

bool FileManager::saveStats(QString filename, QString data)
{
  return saveToFile(filename, data, statsDir, statsExt);
}

bool FileManager::saveSettings(QString filename, QString data)
{
  return saveToFile(filename, data, settingsDir, settingsExt, 0);
}

bool FileManager::saveToFile(QString filename, QString data,
                             QString dir, QString ext,
                             QIODevice::OpenMode appendFlag)
{
  QFile file(dir + filename + ext);
  if(!file.open(QIODevice::ReadWrite | appendFlag))
  {
    qDebug() << "Unable to open" << file.fileName() << endl;
    return false;
  }
  QTextStream fileStream(&file);
  fileStream << data << endl;
  return true;
}
