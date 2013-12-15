#include "filemanager.h"
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

FileManager::~FileManager()
{
  delete xmlReader;
  delete xmlWriter;
}

QString FileManager::readStats(QString filename)
{
  return readFromFile(statsDir + filename + statsExt);
}

QString FileManager::readSettings(QString filename)
{
  return readFromFile(settingsDir + filename + settingsExt);
}

QString FileManager::readFromFile(QString filename)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Unable to open" << file.fileName() << endl;
    return NULL;
  }

  QTextStream fileStream(&file);
  return fileStream.readAll();
}

bool FileManager::saveStats(QString filename, QString data)
{
  return saveToFile(statsDir + filename + statsExt, data);
}

bool FileManager::saveSettings(QString filename, QString data)
{
  return saveToFile(settingsDir + filename + settingsExt, data, 0);
}

bool FileManager::saveToFile(QString filename, QString data,
                             QIODevice::OpenMode appendFlag)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadWrite | QIODevice::Text | appendFlag))
  {
    qDebug() << "Unable to open" << file.fileName();
    return false;
  }
  QTextStream fileStream(&file);
  fileStream << data << endl;
  return true;
}

bool FileManager::saveToFileXML(QString filename, QString data, SettingName setting)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    qDebug() << "Unable to open" << file.fileName();
    return false;
  }
  QString settings = file.readAll();
  QString tag;
  switch(setting)
  {
    case TimeRemaining:
      tag = "time-remaining";
      break;
    case TimeLimit:
      tag = "time-limit";
      break;
  }
  settings.replace(QRegExp("(<"+tag+">)[^<]*(</"+tag+">)"),
                   "\\1" + data + "\\2");
  file.seek(0);
  QTextStream fileStream(&file);
  fileStream << settings;
  file.resize(settings.size());

  return true;
}
