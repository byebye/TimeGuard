#ifndef TIMEGUARD_H
#define TIMEGUARD_H

#include <QMainWindow>
#include <QIcon>
#include <QAction>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include "FileManager.h"
#include "Logger.h"
#include "User.h"
#include "AdminLoginDialog.h"

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
  void quit();
  void showLengthenTimeWindow();
  void adminSuccesfullyLogged();
private:
  Ui::TimeGuard *ui;
  FileManager *fileManager;
  Logger *logger;
  User *user;
  QIcon programIcon;
  QSystemTrayIcon *trayIcon;
  QMenu *trayContextMenu;
  QAction *quitAct;
  QAction *lengthenAct;
  bool closeFromTrayMenu;
  bool loggedAsAdmin;
  AdminLoginDialog *adminLoginDialog;

  void startTimer();
  void setTrayIcon();
  void closeEvent(QCloseEvent *event);
  void createActions();
  void addActions();
};

#endif // TIMEGUARD_H
