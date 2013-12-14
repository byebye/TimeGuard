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
  void on_tabWidget_currentChanged(int index);
  void on_adminLogoffButton_clicked();
  void on_changePasswordButton_clicked();

private:
  Ui::TimeGuard *ui;
  FileManager *fileManager;
  Logger *logger;
  Admin *admin;
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
  void logoffAdmin();
  void changeAdminPassword();
};

#endif // TIMEGUARD_H
