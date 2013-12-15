#ifndef TIMEGUARD_H
#define TIMEGUARD_H

#include <QMainWindow>
#include <QIcon>
#include <QAction>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include "filemanager.h"
#include "logger.h"
#include "user.h"
#include "adminlogindialog.h"

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
  void userTimeout();
  void trayActivated(QSystemTrayIcon::ActivationReason reason);
  void quit();
  void showLengthenTimeWindow();
  void adminSuccesfullyLogged();
  void on_logOffButton_clicked();
  void on_tabWidget_currentChanged(int index);
  void on_adminLogoffButton_clicked();
  void on_changePasswordButton_clicked();
  void on_timeEdit_timeChanged(const QTime &time);
  void on_saveTimeLimitButton_clicked();
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
  void saveTimeLimit(const QTime &time);
};

#endif // TIMEGUARD_H
