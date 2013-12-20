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
  void userToSetChosen();
  void on_logOffButton_clicked();
  void on_adminLoggingButton_clicked();
  void on_changePasswordButton_clicked();
  void on_saveTimeLimitButton_clicked();
  void on_resetTimeButton_clicked();
  void on_resumePauseTimeButton_clicked();
  void on_changeLimitActivityButton_clicked();
signals:
private:
  Ui::TimeGuard *ui;
  FileManager *fileManager;
  Logger *logger;
  Admin *admin;
  User *user;
  QIcon programIcon;
  QIcon pauseIcon;
  QIcon resumeIcon;
  QSystemTrayIcon *trayIcon;
  QMenu *trayContextMenu;
  QAction *quitAct;
  QAction *lengthenAct;
  bool closeFromTrayMenu;
  bool loggedAsAdmin;
  AdminLoginDialog *adminLoginDialog;

  bool setTime();
  void setupUi();
  void setupIcons();
  void setTrayIcon();
  void closeEvent(QCloseEvent *event);
  void createActions();
  void addActions();
  void logoffAdmin();
  void changeAdminPassword();
  void setResumePauseButtonIcon();
  QStringList getUsersList();
  void addUsersToChooseUserBox();
  void setUiLimitActive(bool active);
};

#endif // TIMEGUARD_H
