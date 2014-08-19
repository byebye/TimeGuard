#include <QMenu>
#include <QDebug>
#include <QCheckBox>
#include "timeguard.h"
#include "ui_timeguard.h"

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard),
  USER_TAB(0), SETTINGS_TAB(1), ADMIN_TAB(2)
{
  initObjects();
  setupUi();
  setupLogger();
  initLoggedUser();
  initUsersTableModel();

  connect(ui->timerLCD, SIGNAL(timeout()), this, SLOT(userTimeout()));
  connect(ui->timerLCD, SIGNAL(timeToSaveTimeRemaining(QTime)), user, SLOT(saveTimeRemaining(QTime)));
  connect(adminLoginDialog, SIGNAL(passwordAccepted()), this, SLOT(adminSuccesfullyLogged()));
}

void TimeGuard::initUsersTableModel()
{
  usersTableModel = new UsersTableModel();
  ui->tableView->setModel(usersTableModel);
  const int CHECKBOXES_COLUMN = 0;
  ui->tableView->setColumnWidth(CHECKBOXES_COLUMN, 20);
  ui->tableView->horizontalHeader()->setSectionResizeMode(CHECKBOXES_COLUMN, QHeaderView::Fixed);
  ui->tableView->show();
}

TimeGuard::~TimeGuard()
{
  delete ui;
  delete fileManager;
  delete logger;
  delete systemQuery;
  delete messages;
  delete admin;
  delete user;
  delete trayIcon;
  delete trayContextMenu;
  delete quitAct;
  delete extendLimitAct;
  delete adminLoginDialog;
  delete usersTableModel;
}

void TimeGuard::initObjects()
{
  closeFromTrayMenu = false;

  fileManager = new FileManager();
  logger = new Logger(this, fileManager);
  messages = new Messages(this);
  systemQuery = new SystemQuery(messages);
  admin = new Admin(this);
  user = new User(this, fileManager, systemQuery, logger);
}

void TimeGuard::initLoggedUser()
{
  emit userLoggedIn(user->getName());
  setTimeIfLimitIsSet();
  if(user->isLimitActive())
  {
    ui->timerLCD->startTime();
    emit userTimeStarted(user->getName(), Timer::timeToString(user->getTimeRemaining()));
  }
}

void TimeGuard::setupUi()
{
  setupIcons();
  ui->setupUi(this);
  logOffAdmin();
  ui->userNameLabel->setText(user->getName());
  ui->logBrowser->setPlainText(fileManager->readLog(user->getName()));
  ui->timerLCD->displayDefaultTime();

  this->setWindowIcon(programIcon);
  setupTray();
  adminLoginDialog = new AdminLoginDialog(this, messages, admin);
}

void TimeGuard::setResumePauseButtonIcon()
{
  if(ui->timerLCD->isTimeActive())
    ui->resumePauseTimeButton->setIcon(pauseIcon);
  else
    ui->resumePauseTimeButton->setIcon(resumeIcon);
}

void TimeGuard::setupLogger()
{
  connect(this, SIGNAL(adminLoggedIn()), logger, SLOT(logAdminLoggedIn()));
  connect(this, SIGNAL(adminLoggedOff()), logger, SLOT(logAdminLoggedOff()));
  connect(this, SIGNAL(adminPasswordChanged()), logger, SLOT(logAdminPasswordChanged()));
  connect(this, SIGNAL(userLoggedIn(QString)), logger, SLOT(logUserLoggedIn(QString)));
  connect(this, SIGNAL(userLoggedOff(QString)),logger, SLOT(logUserLoggedOff(QString)));
  connect(this, SIGNAL(userLimitEnabled(QString)), logger, SLOT(logUserLimitEnabled(QString)));
  connect(this, SIGNAL(userLimitDisabled(QString)), logger, SLOT(logUserLimitDisabled(QString)));
  connect(this, SIGNAL(userLimitChanged(QString,QString)), logger, SLOT(logUserLimitChanged(QString,QString)));
  connect(this, SIGNAL(userTimePaused(QString,QString)), logger, SLOT(logUserTimePaused(QString,QString)));
  connect(this, SIGNAL(userTimeStarted(QString,QString)), logger, SLOT(logUserTimeStarted(QString,QString)));
  connect(this, SIGNAL(userTimeReset(QString,QString)), logger, SLOT(logUserTimeReset(QString,QString)));
}

void TimeGuard::setupIcons()
{
  programIcon = QIcon(":/images/timeguard.png");
  pauseIcon = QIcon(":/images/pause.png");
  resumeIcon = QIcon(":/images/resume.png");
}

void TimeGuard::setupTray()
{
  trayIcon = new QSystemTrayIcon(programIcon, this);
  trayIcon->setVisible(true);

  trayContextMenu = new QMenu(this);
  trayContextMenu->setStyleSheet("width: 125px");
  createActions();
  addActions();
  trayIcon->setContextMenu(trayContextMenu);

  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));
}

void TimeGuard::createActions()
{
  quitAct = new QAction(tr("Exit"), this);
  connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));
  extendLimitAct = new QAction(tr("Extend limit"), this);
  connect(extendLimitAct, SIGNAL(triggered()), this, SLOT(showExtendLimitWindow()));
}

void TimeGuard::addActions()
{
  trayContextMenu->addAction(extendLimitAct);
  trayContextMenu->addAction(quitAct);
}

void TimeGuard::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch(reason)
  {
    case QSystemTrayIcon::Context:
      trayIcon->contextMenu()->exec();
      break;
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
      isHidden() ? show() : hide();
      break;
  }
}

void TimeGuard::quit()
{
  closeFromTrayMenu = true;
  close();
}

void TimeGuard::closeEvent(QCloseEvent *event)
{
  // TODO save that info to file
  static bool messageShown = false;
  if(closeFromTrayMenu)
  {
    closeFromTrayMenu = false;
    if(!loggedAsAdmin)
      adminLoginDialog->exec();
    if(loggedAsAdmin)
      emit programQuit();
    else
      event->ignore();
  }
  else if(!messageShown)
  {
    trayIcon->showMessage(tr("Program is still working"),
                          tr("Program has been minimized to tray. "
                             "Click icon to maximize it"));
    messageShown = true;
  }
}

void TimeGuard::showExtendLimitWindow()
{
//  QMessageBox::information(this,
//                           tr("Extend Limit"),
//                           tr("Extend limit for: "),
//                           QMessageBox::Ok, QMessageBox::Cancel);
}

void TimeGuard::adminSuccesfullyLogged()
{
  emit adminLoggedIn();
  loggedAsAdmin = true;
  enableAdminUiElements();
}

void TimeGuard::enableAdminUiElements()
{
  ui->adminLoggedNotification->setText("<html><head/><body><p><span style=\" font-size:11pt; font-weight:600; text-decoration: underline; color:#55aa00;\">"
                                       + tr("Logged as Admin")
                                       + "</span></p></body></html>");
  ui->adminLoggingButton->setText(tr("Log off"));
  readUsersSettings();
  ui->tabWidget->setTabEnabled(SETTINGS_TAB, true);
  ui->tabWidget->setTabEnabled(ADMIN_TAB, true);
  ui->tabWidget->setCurrentIndex(SETTINGS_TAB);
  ui->resumePauseTimeButton->setEnabled(true);
  ui->resetTimeButton->setEnabled(true);
  uncheckSettingsCheckBoxes();
  setResumePauseButtonIcon();
}

void TimeGuard::uncheckSettingsCheckBoxes()
{
  ui->undoSavedSettingsButton->setEnabled(false);
  ui->deleteFilesCheckBox->setChecked(false);
  ui->limitControlGroupBox->setChecked(false);
  ui->enableRadioButton->setChecked(true);
  ui->dailyLimitCheckBox->setChecked(false);
  ui->dailyTimeEdit->setEnabled(false);
  ui->weeklyLimitCheckBox->setChecked(false);
  ui->weeklyTimeEdit->setEnabled(false);
  ui->monthlyLimitCheckBox->setChecked(false);
  ui->monthlyTimeEdit->setEnabled(false);
}

void TimeGuard::disableAdminUiElements()
{
  ui->adminLoggedNotification->setText(tr("Log in as Admin"));
  ui->adminLoggingButton->setText(tr("Log in"));
  ui->tabWidget->setCurrentIndex(USER_TAB);
  ui->tabWidget->setTabEnabled(SETTINGS_TAB, false);
  ui->tabWidget->setTabEnabled(ADMIN_TAB, false);
  ui->resumePauseTimeButton->setDisabled(true);
  ui->resetTimeButton->setDisabled(true);
  setResumePauseButtonIcon();
}

void TimeGuard::logOffAdmin()
{
  emit adminLoggedOff();
  loggedAsAdmin = false;
  disableAdminUiElements();
}

void TimeGuard::changeAdminPassword()
{
  if(admin->isPasswordCorrect(ui->currentPasswordField->text()))
  {
    QString newPassword = ui->newPasswordField->text();
    QString newPasswordRepeated = ui->newPaswordRepeatField->text();
    if(newPassword == newPasswordRepeated)
    {
      if(admin->isPasswordCorrect(newPassword))
        messages->critical(Messages::PasswordIdentical);
      else if(newPassword.isEmpty())
        messages->critical(Messages::PasswordEmpty);
      else
      {
        admin->changePassword(newPassword);
        emit adminPasswordChanged();
        messages->information(Messages::PasswordChanged);
        ui->currentPasswordField->clear();
        ui->newPasswordField->clear();
        ui->newPaswordRepeatField->clear();
      }
    }
    else
      messages->critical(Messages::PasswordNotMatch);
  }
  else
    messages->critical(Messages::PasswordIncorrect);
}

bool TimeGuard::setTimeIfLimitIsSet()
{
  if(!ui->timerLCD->isTimeSet())
  {
    if(!fileManager->settingsFileExists(user->getName()))
      return false;
    ui->timerLCD->setTime(user->getTimeRemaining());
  }
  return true;
}

void TimeGuard::userTimeout()
{
  emit userLoggedOff(user->getName());
  messages->information(Messages::UserTimeout);
  user->logOff();
}

void TimeGuard::on_logOffButton_clicked()
{
  emit userLoggedOff(user->getName());
  user->logOff();
}

void TimeGuard::on_adminLoggingButton_clicked()
{
  loggedAsAdmin ? logOffAdmin() : adminLoginDialog->exec();
}

void TimeGuard::on_changePasswordButton_clicked()
{
  changeAdminPassword();
}

void TimeGuard::readUsersSettings()
{
  QStringList usersList = systemQuery->getUsersList();
  QVector<QVector<QVariant>> settings(usersList.size());
  for(int i = 0; i < usersList.size(); ++i)
  {
    settings[i].resize(usersTableModel->columnCount());
    QString user = usersList[i];
    settings[i][UsersTableModel::Username] = user;
    settings[i][UsersTableModel::LimitStatus] = fileManager->readSettings(user, FileManager::LimitActive) == "1" ? "enabled" : "disabled";
    QString timeLimit = fileManager->readSettings(user, FileManager::TimeLimit);
    settings[i][UsersTableModel::TodayLimit] = timeLimit.isEmpty() ? "Not set" : timeLimit;
    settings[i][UsersTableModel::TimeUsedToday] = "00:00:00";
  }
  usersTableModel->setUsersData(settings);
}

void TimeGuard::on_applyChangedSettingsButton_clicked()
{
  if(ui->deleteFilesCheckBox->isChecked() &&
     messages->information(Messages::QuestionDeleteUserFiles, QMessageBox::Ok | QMessageBox::Cancel)
     != QMessageBox::Ok)
    return;
  beforeSaveSettings = usersTableModel->getData();
  for(auto username : usersTableModel->getSelectedUsers())
  {
    if(ui->dailyLimitCheckBox->isChecked())
      setDailyLimit(username, Timer::timeToString(ui->dailyTimeEdit->time()));
    if(ui->weeklyLimitCheckBox->isChecked())
      setWeeklyLimit(username, Timer::timeToString(ui->weeklyTimeEdit->time()));
    if(ui->monthlyLimitCheckBox->isChecked())
      setMonthlyLimit(username, Timer::timeToString(ui->monthlyTimeEdit->time()));
    if(ui->limitControlGroupBox->isChecked())
      setLimitEnabled(username, ui->enableRadioButton->isChecked());
    if(ui->deleteFilesCheckBox->isChecked())
      deleteUserFiles(username);
  }
  readUsersSettings();
  ui->undoSavedSettingsButton->setEnabled(true);
}

void TimeGuard::setDailyLimit(QString username, QString limit)
{
  fileManager->saveSettings(username, limit, FileManager::TimeLimit);
  emit userLimitChanged(username, limit);
}

void TimeGuard::setWeeklyLimit(QString username, QString limit)
{

}

void TimeGuard::setMonthlyLimit(QString username, QString limit)
{

}

void TimeGuard::setLimitEnabled(QString username, bool enable)
{
  QString limitStatus;
  if(enable)
  {
    limitStatus = "1";
    emit userLimitEnabled(username);
  }
  else
  {
    limitStatus = "0";
    emit userLimitDisabled(username);
    if(username == user->getName())
    {
      ui->timerLCD->resetTime();
      setResumePauseButtonIcon();
    }
  }
  fileManager->saveSettings(username, limitStatus, FileManager::LimitActive);
}

void TimeGuard::deleteUserFiles(QString username)
{
  if(fileManager->deleteLogFile(username) && fileManager->deleteSettingsFile(username))
  {
//    messages->information(Messages::FilesDeleted);
    if(username == user->getName())
    {
      ui->timerLCD->resetTime();
      setResumePauseButtonIcon();
    }
  }
//  else
//    messages->critical(Messages::UnableToDeleteFiles);
}

void TimeGuard::on_undoSavedSettingsButton_clicked()
{
  ui->undoSavedSettingsButton->setEnabled(false);
  for(int i = 0; i < beforeSaveSettings.size(); ++i)
  {
    QString username = beforeSaveSettings[i][UsersTableModel::Username].toString();
    QString dailyLimit = beforeSaveSettings[i][UsersTableModel::TodayLimit].toString();
    setDailyLimit(username, dailyLimit);
    QString limitStatus = beforeSaveSettings[i][UsersTableModel::LimitStatus].toString();
    setLimitEnabled(username, limitStatus == "enabled");
  }
  readUsersSettings();
}

void TimeGuard::on_dailyLimitCheckBox_clicked()
{
  bool status = ui->dailyLimitCheckBox->isChecked();
  ui->dailyTimeEdit->setEnabled(status);
}

void TimeGuard::on_weeklyLimitCheckBox_clicked()
{
  bool status = ui->weeklyLimitCheckBox->isChecked();
  ui->weeklyTimeEdit->setEnabled(status);
}

void TimeGuard::on_monthlyLimitCheckBox_clicked()
{
  bool status = ui->monthlyLimitCheckBox->isChecked();
  ui->monthlyTimeEdit->setEnabled(status);
}

void TimeGuard::on_resetTimeButton_clicked()
{
  if(!fileManager->settingsFileExists(user->getName()))
  {
    messages->critical(Messages::LimitNotSet);
    return;
  }
  user->resetTimeRemaining();
  QTime timeRemaining = user->readTimeLimit();
  ui->timerLCD->resetTime(timeRemaining);
  emit userTimeReset(user->getName(), Timer::timeToString(timeRemaining));
}

void TimeGuard::on_resumePauseTimeButton_clicked()
{
  if(ui->timerLCD->isTimeActive())
  {
    emit userTimePaused(user->getName(), ui->timerLCD->getTimeRemaining());
    ui->timerLCD->pauseTime();
  }
  else
  {
    if(setTimeIfLimitIsSet())
    {
      emit userTimeStarted(user->getName(), ui->timerLCD->getTimeRemaining());
      ui->timerLCD->resumeTime();
    }
    else
      messages->critical(Messages::LimitNotSet);
  }
  setResumePauseButtonIcon();
}

