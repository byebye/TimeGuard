#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QString>
#include <QFile>

class FileManager
{
public:
  FileManager();

  QString readFromFile(QString filename, QString dir, QString ext);
  bool saveToFile(QString filename, QString data, QString dir, QString ext,
                  QIODevice::OpenMode appendFlag = QIODevice::Append);
  QString readStats(QString filename);
  bool saveStats(QString filename, QString data);
  QString readSettings(QString filename);
  bool saveSettings(QString filename, QString data);
private:
  QString const settingsDir;
  QString const statsDir;
  QString const settingsExt;
  QString const statsExt;
};

#endif // FILE_MANAGER_H
