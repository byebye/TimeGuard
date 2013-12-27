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
#include "messages.h"
#include "adminlogindialog.h"
#include "systemquery.h"

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
  void showExtendLimitWindow();
  void adminSuccesfullyLogged();
  void userChosenToSet();
  void on_logOffButton_clicked();
  void on_adminLoggingButton_clicked();
  void on_changePasswordButton_clicked();
  void on_saveTimeLimitButton_clicked();
  void on_resetTimeButton_clicked();
  void on_resumePauseTimeButton_clicked();
  void on_changeLimitActivityButton_clicked();
  void on_deleteUserFilesButton_clicked();

signals:
  void programClosed();
  void adminLoggedIn();
  void adminLoggedOff();
  void adminPasswordChanged();
  void userLoggedIn(QString username);
  void userLoggedOff(QString username);
  void userLimitChanged(QString username, QString limit);
  void userLimitActivated(QString username);
  void userLimitDeactivated(QString username);
  void userTimePaused(QString username, QString time);
  void userTimeStarted(QString username, QString time);
  void userTimeReset(QString username, QString time);
private:
  Ui::TimeGuard *ui;
  FileManager *fileManager;
  Logger *logger;
  SystemQuery *systemQuery;
  Messages *messages;
  Admin *admin;
  User *user;
  QIcon programIcon;
  QIcon pauseIcon;
  QIcon resumeIcon;
  QSystemTrayIcon *trayIcon;
  QMenu *trayContextMenu;
  QAction *quitAct;
  QAction *extendLimitAct;
  bool closeFromTrayMenu;
  bool loggedAsAdmin;
  AdminLoginDialog *adminLoginDialog;

  bool setTime();
  void setupLogger();
  void setupUi();
  void setupIcons();
  void setTrayIcon();
  void closeEvent(QCloseEvent *event);
  void createActions();
  void addActions();
  void logOffAdmin();
  void changeAdminPassword();
  void setResumePauseButtonIcon();
  QStringList getUsersList();
  void addUsersToChooseUserBox();
  void setUiLimitActive(bool active);
};

#endif // TIMEGUARD_H
