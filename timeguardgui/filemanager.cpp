#include "filemanager.h"
#include <QDir>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QDate>
#include <QDebug>
#include "timer.h"
#include "timelimit.h"

FileManager::FileManager() :
  settingsDir("settings/"), logDir("log/"),
  settingsExt(".xml"), logExt(".log")
{
  if(!QDir(settingsDir).exists())
    QDir().mkdir(settingsDir);
  if(!QDir(logDir).exists())
    QDir().mkdir(logDir);
}

FileManager::~FileManager()
{

}

QString FileManager::readLog(QString filename)
{
  return readFromFile(logDir + filename + logExt);
}

QString FileManager::readSettings(QString filename, SettingName setting)
{
  return readSettingsFromXML(settingsDir + filename + settingsExt, setting);
}

QString FileManager::readFromFile(QString filename)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Unable to open" << file.fileName() << " for reading";
    return "";
  }
  QTextStream fileStream(&file);
  return fileStream.readAll();
}

QString FileManager::readSettingsFromXML(QString filename, SettingName setting)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Unable to open" << file.fileName() << " for reading settings";
    return QString();
  }
  QString tag = getStringTag(setting);
  QXmlStreamReader xmlReader(&file);
  while(!xmlReader.atEnd())
  {
    xmlReader.readNextStartElement();
    if(xmlReader.qualifiedName() == tag)
    {
      xmlReader.readNext();
      return xmlReader.text().toString();
    }
  }
  return QString();
}

bool FileManager::saveLog(QString filename, QString data)
{
  qDebug() << "Log:" << data << "--->" << filename + logExt;
  return saveToFile(logDir + filename + logExt, data);
}

bool FileManager::saveSettings(QString filename, QString data, SettingName setting)
{
  return saveSettingsToXML(settingsDir + filename + settingsExt, data, setting);
}

bool FileManager::saveToFile(QString filename, QString data,
                             QIODevice::OpenMode appendFlag)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadWrite | QIODevice::Text | appendFlag))
  {
    qDebug() << "Unable to open" << file.fileName() << " for writing";
    return false;
  }
  QTextStream fileStream(&file);
  fileStream << data << endl;
  return true;
}

bool FileManager::saveSettingsToXML(QString filename, QString data, SettingName setting)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    qDebug() << "Unable to open" << file.fileName() << " for writing settings";
    return false;
  }
  QString settings = file.readAll();
  if(settings.isEmpty())
    settings = generateDefaultSettingsXML();
  QString tag = getStringTag(setting);

  settings.replace(QRegExp("(<"+tag+">)[^<]*(</"+tag+">)"),
                   "\\1" + data + "\\2");
  file.seek(0);
  qDebug() << "Saving XML to " << filename;
  QTextStream fileStream(&file);
  fileStream << settings;
  file.resize(settings.size());

  return true;
}

QString FileManager::getStringTag(SettingName setting)
{
  switch(setting)
  {
    case TimeRemaining:
      return "time-remaining";
    case WeeklyTimeRemaining:
      return "weekly-time-remaining";
    case MonthlyTimeRemaining:
      return "monthly-time-remaining";
    case DailyLimit:
      return "daily-limit";
    case WeeklyLimit:
      return "weekly-limit";
    case MonthlyLimit:
      return "monthly-limit";
    case LastLoginDate:
      return "last-login-date";
    case LimitEnabled:
      return "limit-enabled";
    default:
      return "";
  }
}

QString FileManager::getDefaultContent(SettingName setting)
{
  switch(setting)
  {
    case TimeRemaining:
    case WeeklyTimeRemaining:
    case MonthlyTimeRemaining:
    case DailyLimit:
    case WeeklyLimit:
    case MonthlyLimit:
      return TimeLimit(0).toString();
    case LastLoginDate:
      return "0000.00.00";
    case LimitEnabled:
      return "0";
    default:
      return "";
  }
}

QString FileManager::generateDefaultSettingsXML()
{
  QString settings;
  QXmlStreamWriter xml(&settings);
  xml.setAutoFormatting(true);
  xml.writeStartDocument();
  xml.writeStartElement("TimeGuard");
  QString tag, content;
  for(int setting = TimeRemaining; setting <= LimitEnabled; ++setting)
  {
    tag = getStringTag(static_cast<SettingName>(setting));
    content = getDefaultContent(static_cast<SettingName>(setting));
    xml.writeTextElement(tag, content);
  }
  xml.writeEndElement();
  return settings;
}

bool FileManager::fileExists(QString filename)
{
  return QFile(filename).exists();
}

bool FileManager::settingsFileExists(QString filename)
{
  return fileExists(settingsDir + filename + settingsExt);
}

bool FileManager::logFileExists(QString filename)
{
  return fileExists(logDir + filename + logExt);
}

bool FileManager::deleteFile(QString filename)
{
  return fileExists(filename) ? QFile(filename).remove() : true;
}

bool FileManager::deleteSettingsFile(QString filename)
{
  return deleteFile(settingsDir + filename + settingsExt);
}

bool FileManager::deleteLogFile(QString filename)
{
  return deleteFile(logDir + filename + logExt);
}
