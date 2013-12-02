#ifndef TIMEGUARD_H
#define TIMEGUARD_H

#include <QMainWindow>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include "FileManager.h"
#include "Logger.h"
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
  void trayActivated(QSystemTrayIcon::ActivationReason reason);
private:
  Ui::TimeGuard *ui;
  FileManager *fileManager;
  Logger *logger;
  User *user;
  QIcon programIcon;
  QSystemTrayIcon *trayIcon;
  QMenu *trayContextMenu;

  void startTimer();
  void setTrayIcon();
  void closeEvent(QCloseEvent *event);
};

#endif // TIMEGUARD_H
