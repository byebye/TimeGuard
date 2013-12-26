#include "filemanager.h"
#include <QDir>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QDate>
#include <QDebug>

FileManager::FileManager() :
  settingsDir("settings/"), logDir("log/"),
  ext(".xml")
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
  return readFromFile(logDir + filename + ext);
}

QString FileManager::readSettings(QString filename, SettingName setting)
{
  return readSettingsFromXML(settingsDir + filename + ext, setting);
}

QString FileManager::readFromFile(QString filename)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Unable to open" << file.fileName() << endl;
    return QString();
  }

  QTextStream fileStream(&file);
  return fileStream.readAll();
}

QString FileManager::readSettingsFromXML(QString filename, SettingName setting)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Unable to open" << file.fileName() << endl;
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
//      qDebug() << xmlReader.text();
      return xmlReader.text().toString();
    }
  }
  return QString();
}

bool FileManager::saveLog(QString filename, QString data)
{
  return saveToFile(logDir + filename + ext, data);
}

bool FileManager::saveSettings(QString filename, QString data, SettingName setting)
{
  return saveSettingsToXML(settingsDir + filename + ext, data, setting);
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

bool FileManager::saveSettingsToXML(QString filename, QString data, SettingName setting)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
  {
    qDebug() << "Unable to open" << file.fileName();
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
    case TimeLimit:
      return "time-limit";
    case LastLogin:
      return "last-login";
    case LimitActive:
      return "limit-active";
    default:
      return "";
  }
}

QString FileManager::getDefaultContent(SettingName setting)
{
  switch(setting)
  {
    case TimeRemaining:
      return "00:00:00";
    case TimeLimit:
      return "00:00:00";
    case LastLogin:
      return "0000.00.00";
    case LimitActive:
      return "false";
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
  for(int setting = TimeRemaining; setting <= LimitActive; ++setting)
  {
    tag = getStringTag(static_cast<SettingName>(setting));
    content = getDefaultContent(static_cast<SettingName>(setting));
    xml.writeTextElement(tag, content);
  }
  xml.writeEndElement();
//  qDebug() << "Generated settings file:" << endl << settings;
  return settings;
}

bool FileManager::fileExists(QString filename)
{
  return QFile(filename).exists();
}

bool FileManager::settingsFileExists(QString filename)
{
  return fileExists(settingsDir + filename + ext);
}

bool FileManager::historyFileExists(QString filename)
{
  return fileExists(logDir + filename + ext);
}
