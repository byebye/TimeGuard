#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QString>
#include <QFile>

class FileManager
{
public:
  FileManager();
  ~FileManager();

  enum SettingName { TimeRemaining, WeeklyTimeRemaining, MonthlyTimeRemaining, DailyLimit, WeeklyLimit, MonthlyLimit, LastLogin, LimitEnabled };

  QString readFromFile(QString filename);
  bool saveToFile(QString filename, QString data,
                  QIODevice::OpenMode appendFlag = QIODevice::Append);
  QString readLog(QString filename);
  bool saveLog(QString filename, QString data);
  QString readSettings(QString filename, SettingName setting);
  bool saveSettings(QString filename, QString data, SettingName setting);
  bool fileExists(QString filename);
  bool settingsFileExists(QString filename);
  bool logFileExists(QString filename);
  bool deleteFile(QString filename);
  bool deleteSettingsFile(QString filename);
  bool deleteLogFile(QString filename);
private:
  QString const settingsDir;
  QString const logDir;
  QString const settingsExt;
  QString const logExt;
  QString getStringTag(SettingName setting);
  QString getDefaultContent(SettingName setting);
  QString generateDefaultSettingsXML();

  QString readSettingsFromXML(QString filename, SettingName setting);
  bool saveSettingsToXML(QString filename, QString data, SettingName setting);
};

#endif // FILE_MANAGER_H
