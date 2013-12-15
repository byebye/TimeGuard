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

  enum SettingName { TimeRemaining, TimeLimit };

  QString readFromFile(QString filename);
  bool saveToFile(QString filename, QString data,
                  QIODevice::OpenMode appendFlag = QIODevice::Append);
  bool saveToFileXML(QString filename, QString data, SettingName setting);
  QString readStats(QString filename);
  bool saveStats(QString filename, QString data);
  QString readSettings(QString filename);
  bool saveSettings(QString filename, QString data);
private:
  QString const settingsDir;
  QString const statsDir;
  QString const settingsExt;
  QString const statsExt;
  QXmlStreamReader *xmlReader;
  QXmlStreamWriter *xmlWriter;
};

#endif // FILE_MANAGER_H
