#include "filemanager.h"
#include <QDir>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QDate>
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

}

QString FileManager::readStats(QString filename)
{
  return readFromFile(statsDir + filename + statsExt);
}

QString FileManager::readSettings(QString filename, SettingName setting)
{
  return readFromFileXML(settingsDir + filename + settingsExt, setting);
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

QString FileManager::readFromFileXML(QString filename, SettingName setting)
{
  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    qDebug() << "Unable to open" << file.fileName() << endl;
    return NULL;
  }
  QString tag = getStringTag(setting);
  QXmlStreamReader xmlReader(&file);
  while(!xmlReader.atEnd())
  {
    xmlReader.readNextStartElement();
    if(xmlReader.qualifiedName() == tag)
    {
      xmlReader.readNext();
      qDebug() << xmlReader.text();
      return xmlReader.text().toString();
    }
  }
  return NULL;
}

bool FileManager::saveStats(QString filename, QString data)
{
  return saveToFile(statsDir + filename + statsExt, data);
}

bool FileManager::saveSettings(QString filename, QString data, SettingName setting)
{
  return saveToFileXML(settingsDir + filename + settingsExt, data, setting);
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
  if(settings.isEmpty())
    settings = generateSettingsXML();
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
      return QDate::currentDate().toString("yyyy.MM.dd");
    default:
      return "";
  }
}

QString FileManager::generateSettingsXML()
{
  QString settings;
  QXmlStreamWriter xml(&settings);
  xml.setAutoFormatting(true);
  xml.writeStartDocument();
  xml.writeStartElement("TimeGuard");
  QString tag, content;
  for(int setting = TimeRemaining; setting <= LastLogin; ++setting)
  {
    tag = getStringTag(static_cast<SettingName>(setting));
    content = getDefaultContent(static_cast<SettingName>(setting));
    xml.writeTextElement(tag, content);
  }
  xml.writeEndElement();
//  qDebug() << "Generated settings file:" << endl << settings;
  return settings;
}
