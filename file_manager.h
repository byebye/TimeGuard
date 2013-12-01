#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

class FileManager
{
public:
  FileManager();

  bool readFromFile(QString userName);
  bool saveToFile(QString userName);
};

#endif // FILE_MANAGER_H
