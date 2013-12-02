#ifndef TIMEGUARD_H
#define TIMEGUARD_H

#include <QMainWindow>
#include <QIcon>
#include <QSystemTrayIcon>
#include "FileManager.h"
#include "User.h"

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
  void userTimeout();
private:
  Ui::TimeGuard *ui;
  FileManager *fileManager;
  User *user;
  QIcon programIcon;
  QSystemTrayIcon *trayIcon;

  void startTimer();
};

#endif // TIMEGUARD_H
