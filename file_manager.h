#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QString>

class FileManager
{
public:
  FileManager();

  QString readFromFile(QString userName);
  bool saveToFile(QString userName, QString data);

private:
  QString const settingsDir;
  QString const statsDir;
};

#endif // FILE_MANAGER_H
