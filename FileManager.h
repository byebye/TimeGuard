#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QString>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class FileManager
{
public:
  FileManager();
  ~FileManager();

  enum SettingName { TimeRemaining, TimeLimit, LastLogin, LimitActive };

  QString readFromFile(QString filename);
  QString readFromFileXML(QString filename, SettingName setting);
  bool saveToFile(QString filename, QString data,
                  QIODevice::OpenMode appendFlag = QIODevice::Append);
  bool saveToFileXML(QString filename, QString data, SettingName setting);
  QString readStats(QString filename);
  bool saveStats(QString filename, QString data);
  QString readSettings(QString filename, SettingName setting);
  bool saveSettings(QString filename, QString data, SettingName setting);
  bool fileExists(QString filename) const;
  bool settingsFileExists(QString filename) const;
  bool statsFileExists(QString filename) const;
private:
  QString const settingsDir;
  QString const statsDir;
  QString const settingsExt;
  QString const statsExt;
  QString getStringTag(SettingName setting);
  QString getDefaultContent(SettingName setting);
  QString generateDefaultSettingsXML();
};

#endif // FILE_MANAGER_H
