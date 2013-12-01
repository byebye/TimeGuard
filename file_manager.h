#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QString>

class FileManager
{
public:
  FileManager();

  bool readFromFile(QString userName);
  bool saveToFile(QString userName);

private:
  QString const settingsDir;
};

#endif // FILE_MANAGER_H
