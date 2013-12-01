#ifndef TIMEGUARD_H
#define TIMEGUARD_H

#include <QMainWindow>
#include "file_manager.h"

namespace Ui {
  class TimeGuard;
}

class TimeGuard : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit TimeGuard(QWidget *parent = 0);
  ~TimeGuard();
  
private slots:
  void on_logOffButton_clicked();
  void saveLogOffTime();

private:
  Ui::TimeGuard *ui;
  FileManager *fileManager;
  QString const username;

  QString getUsername();
  void startTimer();
};

#endif // TIMEGUARD_H
