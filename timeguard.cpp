#include <QMenu>
#include <QDebug>
#include "timeguard.h"
#include "ui_timeguard.h"

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard)
{
  closeFromTrayMenu = false;

  fileManager = new FileManager();
  logger = new Logger(this, fileManager);
  messages = new Messages(this);
  systemQuery = new SystemQuery(messages);
  admin = new Admin(this);
  user = new User(this, fileManager, systemQuery, logger);

  setupUi();
  setupLogger();

  emit userLoggedIn(user->getName());
  setTime();
  if(user->isLimitActive())
  {
    ui->timerLCD->startTime();
    emit userTimeStarted(user->getName(),
                         user->getTimeRemaining().toString("hh:mm:ss"));
  }

  connect(ui->timerLCD, SIGNAL(timeout()), this, SLOT(userTimeout()));
  connect(ui->timerLCD, SIGNAL(timeToSaveTimeRemaining(QTime)),
          user, SLOT(saveTimeRemaining(QTime)));
  connect(adminLoginDialog, SIGNAL(passwordAccepted()),
          this, SLOT(adminSuccesfullyLogged()));
  connect(ui->chooseUserBox, SIGNAL(currentTextChanged(QString)),
          this, SLOT(userChosenToSet()));
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
}

void TimeGuard::setupUi()
{
  setupIcons();
  ui->setupUi(this);
  logOffAdmin();
  ui->userNameLabel->setText(user->getName());
  ui->logBrowser->setPlainText(fileManager->readLog(user->getName()));
  ui->tabWidget->setCurrentIndex(0);
  ui->tabWidget->setTabEnabled(1, false);
  ui->timerLCD->display("00:00:00");

  this->setWindowIcon(programIcon);
  setTrayIcon();
  adminLoginDialog = new AdminLoginDialog(this, messages, admin);
}

void TimeGuard::setResumePauseButtonIcon()
{
  if(ui->timerLCD->isTimeActive())
    ui->resumePauseTimeButton->setIcon(pauseIcon);
  else
    ui->resumePauseTimeButton->setIcon(resumeIcon);
}

void TimeGuard::setUiLimitActive(bool active)
{
  QString buttonText, labelText;
  if(active)
  {
    labelText = "<html><head/><body><p><span style=\"font-size:10pt;"
                "font-weight:600; text-decoration: underline; color:#65cb00;\">"
                "Active</span></p></body></html>";
    buttonText = tr("Deactivate");
  }
  else
  {
    labelText = "<html><head/><body><p><span style=\"font-size:10pt; color:#ee0000;\">"
                "Not active</span></p></body></html>";
    buttonText = tr("Activate");
  }
  ui->limitActivityLabel->setText(labelText);
  ui->changeLimitActivityButton->setText(buttonText);
}

void TimeGuard::setupLogger()
{
  connect(this, SIGNAL(adminLoggedIn()),
          logger, SLOT(logAdminLoggedIn()));
  connect(this, SIGNAL(adminLoggedOff()),
          logger, SLOT(logAdminLoggedOff()));
  connect(this, SIGNAL(adminPasswordChanged()),
          logger, SLOT(logAdminPasswordChanged()));
  connect(this, SIGNAL(userLoggedIn(QString)),
          logger, SLOT(logUserLoggedIn(QString)));
  connect(this, SIGNAL(userLoggedOff(QString)),
          logger, SLOT(logUserLoggedOff(QString)));
  connect(this, SIGNAL(userLimitActivated(QString)),
          logger, SLOT(logUserLimitActivated(QString)));
  connect(this, SIGNAL(userLimitDeactivated(QString)),
          logger, SLOT(logUserLimitDeactivated(QString)));
  connect(this, SIGNAL(userLimitChanged(QString,QString)),
          logger, SLOT(logUserLimitChanged(QString,QString)));
  connect(this, SIGNAL(userTimePaused(QString,QString)),
          logger, SLOT(logUserTimePaused(QString,QString)));
  connect(this, SIGNAL(userTimeStarted(QString,QString)),
          logger, SLOT(logUserTimeStarted(QString,QString)));
  connect(this, SIGNAL(userTimeReset(QString,QString)),
          logger, SLOT(logUserTimeReset(QString,QString)));
}

void TimeGuard::setupIcons()
{
  programIcon = QIcon(":/images/timeguard.png");
  pauseIcon = QIcon(":/images/pause.png");
  resumeIcon = QIcon(":/images/resume.png");
}

void TimeGuard::setTrayIcon()
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
  static bool msgShown = false;
  if(closeFromTrayMenu)
  {
    closeFromTrayMenu = false;
    if(!loggedAsAdmin) adminLoginDialog->exec();
    if(loggedAsAdmin)
    {
      emit programClosed();
      event->accept();
    }
    else
      event->ignore();
  }
  else
  {
    event->ignore();
    showMinimized();
    if(!msgShown)
    {
      trayIcon->showMessage(tr("Program is still working"),
                            tr("Program has been minimized to tray. "
                               "Click icon to maximize it"));
      msgShown = true;
    }
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
  ui->adminLoggedNotification->setText(QString("<html><head/><body><p><span style=\" font-size:11pt; font-weight:600; text-decoration: underline; color:#55aa00;\">")
                                       + tr("Logged as Admin")
                                       + QString("</span></p></body></html>"));
  ui->adminLoggingButton->setText(tr("Log off"));
  addUsersToChooseUserBox();
  ui->tabWidget->setTabEnabled(1, true);
  ui->tabWidget->setCurrentIndex(1);
  ui->resumePauseTimeButton->setEnabled(true);
  ui->resetTimeButton->setEnabled(true);
  setResumePauseButtonIcon();
}

void TimeGuard::logOffAdmin()
{
  emit adminLoggedOff();
  loggedAsAdmin = false;
  ui->adminLoggedNotification->setText(tr("Log in as Admin"));
  ui->adminLoggingButton->setText(tr("Log in"));
  ui->tabWidget->setCurrentIndex(0);
  ui->tabWidget->setTabEnabled(1, false);
  ui->resumePauseTimeButton->setDisabled(true);
  ui->resetTimeButton->setDisabled(true);
  ui->chooseUserBox->clear();
  setResumePauseButtonIcon();
}

void TimeGuard::changeAdminPassword()
{
  if(admin->isPasswordCorrect(ui->currentPasswordField->text()))
  {
    QString newPassword = ui->newPasswordField->text();
    if(newPassword == ui->newPaswordRepeatField->text())
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

bool TimeGuard::setTime()
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

void TimeGuard::userChosenToSet()
{
  QString userChosen = ui->chooseUserBox->currentText();
  QString timeLimit = fileManager->readSettings(userChosen, FileManager::TimeLimit);
  if(timeLimit.isEmpty())
    timeLimit = "00:00:00";
  ui->timeLimitEdit->setTime(QTime::fromString(timeLimit, "hh:mm:ss"));
  QString limitActive = fileManager->readSettings(userChosen, FileManager::LimitActive);
  setUiLimitActive(limitActive == "1");
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

void TimeGuard::addUsersToChooseUserBox()
{
  QStringList usersList = systemQuery->getUsersList();
  for(QString username : usersList)
    ui->chooseUserBox->addItem(username);
}

void TimeGuard::on_saveTimeLimitButton_clicked()
{
  QString username = ui->chooseUserBox->currentText();
  QString limit =  ui->timeLimitEdit->time().toString("hh:mm:ss");
  fileManager->saveSettings(username,
                            limit,
                            FileManager::TimeLimit);
  emit userLimitChanged(username, limit);
}

void TimeGuard::on_resetTimeButton_clicked()
{
  user->resetTimeRemaining();
  QTime timeRemaining = user->readTimeLimit();
  ui->timerLCD->resetTime(timeRemaining);
  emit userTimeReset(user->getName(), timeRemaining.toString("hh:mm:ss"));
}

void TimeGuard::on_resumePauseTimeButton_clicked()
{
  if(ui->timerLCD->isTimeActive())
  {
    emit userTimePaused(user->getName(),
                        ui->timerLCD->getTimeRemaining());
    ui->timerLCD->pauseTime();
  }
  else
  {
    if(setTime())
    {
      emit userTimeStarted(user->getName(),
                           ui->timerLCD->getTimeRemaining());
      ui->timerLCD->resumeTime();
    }
    else
      messages->critical(Messages::LimitNotSet);
  }
  setResumePauseButtonIcon();
}

void TimeGuard::on_changeLimitActivityButton_clicked()
{
  QString username = ui->chooseUserBox->currentText();
  QString active = "0"; // false
  if(ui->changeLimitActivityButton->text() == tr("Activate"))
    active = "1"; // true
  fileManager->saveSettings(username, active, FileManager::LimitActive);
  if(active == "1")
    emit userLimitActivated(username);
  else
    emit userLimitDeactivated(username);
  setUiLimitActive(active == "1");
}
