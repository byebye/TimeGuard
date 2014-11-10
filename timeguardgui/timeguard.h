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
#include "userstablemodel.h"

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
  void on_logOffButton_clicked();
  void on_adminLoggingButton_clicked();
  void on_changePasswordButton_clicked();
  void on_resetTimeButton_clicked();
  void on_resumePauseTimeButton_clicked();
  void on_applyChangedSettingsButton_clicked();
  void on_undoSavedSettingsButton_clicked();
  void on_dailyLimitCheckBox_clicked();
  void on_weeklyLimitCheckBox_clicked();
  void on_monthlyLimitCheckBox_clicked();

signals:
  void programQuit();
  void adminLoggedIn();
  void adminLoggedOff();
  void adminPasswordChanged();
  void userLoggedIn(QString username);
  void userLoggedOff(QString username);
  void userLimitChanged(QString username, QString limit, FileManager::SettingName whichLimit);
  void userLimitEnabled(QString username);
  void userLimitDisabled(QString username);
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
  UsersTableModel *usersTableModel;
  bool closeFromTrayMenu;
  bool loggedAsAdmin;
  AdminLoginDialog *adminLoginDialog;
  const int USER_TAB;
  const int SETTINGS_TAB;
  const int ADMIN_TAB;
  QVector<QVector<QVariant>> beforeSaveSettings;

  bool setTimeIfLimitIsSet();
  void initObjects();
  void initLoggedUser();
  void initUsersTableModel();
  void setupLogger();
  void setupUi();
  void setupIcons();
  void setupTray();
  void closeEvent(QCloseEvent *event);
  void createActions();
  void addActions();
  void logOffAdmin();
  void changeAdminPassword();
  void setResumePauseButtonIcon();
  QStringList getUsersList();
  void readUsersSettings();
  void enableAdminUiElements();
  void disableAdminUiElements();
  void uncheckSettingsCheckBoxes();
  void setDailyLimit(QString username, TimeLimit limit);
  void setWeeklyLimit(QString username, TimeLimit limit);
  void setMonthlyLimit(QString username, TimeLimit limit);
  void setLimitEnabled(QString username, bool state);
  void deleteUserFiles(QString username);
};

#endif // TIMEGUARD_H
