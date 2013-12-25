#include <QMenu>
#include <QDebug>
#include "timeguard.h"
#include "ui_timeguard.h"
#include <windows.h>
#include <lm.h>

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard)
{
  closeFromTrayMenu = false;

  fileManager = new FileManager();
  logger = new Logger(this, fileManager);
  messages = new Messages(this);
  admin = new Admin(this);
  user = new User(this, fileManager, logger);

  setupUi();

  if(user->isLimitActive())
  {
    setTime();
    ui->timerLCD->startTime();
  }

  connect(ui->timerLCD, SIGNAL(timeout()), this, SLOT(userTimeout()));
  connect(ui->timerLCD, SIGNAL(saveTimeMoment()), user, SLOT(saveTimeRemaining()));
  connect(adminLoginDialog, SIGNAL(passwordAccepted()),
          this, SLOT(adminSuccesfullyLogged()));
  connect(ui->chooseUserBox, SIGNAL(currentTextChanged(QString)),
          this, SLOT(userToSetChosen()));
}

TimeGuard::~TimeGuard()
{
  delete ui;
  delete fileManager;
  delete logger;
  delete messages;
  delete admin;
  delete user;
  delete trayIcon;
  delete trayContextMenu;
  delete quitAct;
  delete lengthenAct;
  delete adminLoginDialog;
}

void TimeGuard::setupUi()
{
  setupIcons();
  ui->setupUi(this);
  logoffAdmin();
  ui->userNameLabel->setText(user->getName());
  ui->logBrowser->setPlainText(fileManager->readStats(user->getName()));
  ui->tabWidget->setCurrentIndex(0);
  ui->tabWidget->setTabEnabled(1, false);
  ui->timerLCD->display("00:00:00");

  this->setWindowIcon(programIcon);
  setTrayIcon();
  adminLoginDialog = new AdminLoginDialog(this, messages, admin);
}

void TimeGuard::setupIcons()
{
  programIcon = QIcon(":/images/timeguard.png");
  pauseIcon = QIcon(":/images/pause.png");
  resumeIcon = QIcon(":/images/resume.png");
}

void TimeGuard::userTimeout()
{
  messages->information(Messages::UserTimeout);
  user->logOff();
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

void TimeGuard::closeEvent(QCloseEvent *event)
{
  static bool msgShown = false;
  if(closeFromTrayMenu)
  {
    if(!loggedAsAdmin) adminLoginDialog->exec();
    loggedAsAdmin ? event->accept() : event->ignore();
  }
  else
  {
    event->ignore();
    hide();
    if(!msgShown)
    {
      trayIcon->showMessage("Aplikacja wciąż działa",
                          QString("Program został zminimalizowany do traya. ") +
                          "Naciśnij na ikonkę, by przywrócić okno programu");
      msgShown = true;
    }
  }
}

void TimeGuard::on_logOffButton_clicked()
{
  user->logOff();
}

void TimeGuard::createActions()
{
  quitAct = new QAction(tr("&Exit"), this);
  connect(quitAct, SIGNAL(triggered()), this, SLOT(quit()));

  lengthenAct = new QAction(tr("&Lengthen time"), this);
  connect(lengthenAct, SIGNAL(triggered()), this, SLOT(showLengthenTimeWindow()));
}

void TimeGuard::addActions()
{
  trayContextMenu->addAction(lengthenAct);
  trayContextMenu->addAction(quitAct);
}

void TimeGuard::quit()
{
  closeFromTrayMenu = true;
  close();
}

void TimeGuard::showLengthenTimeWindow()
{
//  QMessageBox::information(this,
//                           tr("Lengthening time"),
//                           tr("Time to lengthen the limit: "),
//                           QMessageBox::Ok, QMessageBox::Cancel);
}

void TimeGuard::adminSuccesfullyLogged()
{
  loggedAsAdmin = true;
  ui->adminLoggedNotification->setText("<html><head/><body><p><span style=\" font-size:11pt; font-weight:600; text-decoration: underline; color:#55aa00;\">Logged as Admin</span></p></body></html>");
  ui->adminLoggingButton->setText(tr("Log off"));
  addUsersToChooseUserBox();
  ui->tabWidget->setTabEnabled(1, true);
  ui->tabWidget->setCurrentIndex(1);
  ui->resumePauseTimeButton->setEnabled(true);
  ui->resetTimeButton->setEnabled(true);
  setResumePauseButtonIcon();
}

void TimeGuard::logoffAdmin()
{
  loggedAsAdmin = false;
  ui->adminLoggedNotification->setText("Log in as Admin");
  ui->adminLoggingButton->setText(tr("Log in"));
  ui->tabWidget->setCurrentIndex(0);
  ui->tabWidget->setTabEnabled(1, false);
  ui->resumePauseTimeButton->setDisabled(true);
  ui->resetTimeButton->setDisabled(true);
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

void TimeGuard::userToSetChosen()
{
  QString userChosen = ui->chooseUserBox->currentText();
  QString timeLimit = fileManager->readSettings(userChosen, FileManager::TimeLimit);
  if(timeLimit.isEmpty())
    timeLimit = "00:00:00";
  ui->timeLimitEdit->setTime(QTime::fromString(timeLimit, "hh:mm:ss"));
  QString limitActive = fileManager->readSettings(userChosen, FileManager::LimitActive);
  setUiLimitActive(limitActive == "true");
}

QStringList TimeGuard::getUsersList()
{
  // -------------- VARIABLES -------------------
  LPTSTR serverName = NULL; // NULL -> local computer
  DWORD dwLevel = 0; // information level: 0 -> only names
  LPUSER_INFO_0 usersBuf = NULL;
  DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
  DWORD dwUserEntries = 0;
  DWORD dwTotalUserEntries = 0;
  DWORD dwResumeHandle = 0;
  NET_API_STATUS nStatus;

  LPLOCALGROUP_USERS_INFO_0 groupsBuf = NULL;
  DWORD dwGroupEntries = 0;
  DWORD dwTotalGroupEntries = 0;
  // ---------------------------------------------
  nStatus = NetUserEnum(serverName,
              dwLevel,
              FILTER_NORMAL_ACCOUNT,
              (LPBYTE*)&usersBuf,
              dwPrefMaxLen,
              &dwUserEntries,
              &dwTotalUserEntries,
              &dwResumeHandle
              );
  QStringList usersList;
  if(nStatus == NERR_Success)
  {
    for(DWORD i = 0; i < dwUserEntries; ++i)
    {
      bool normalUser = false;
      nStatus = NetUserGetLocalGroups(serverName,
                                      usersBuf->usri0_name,
                                      dwLevel,
                                      LG_INCLUDE_INDIRECT,
                                      (LPBYTE*)&groupsBuf,
                                      dwPrefMaxLen,
                                      &dwGroupEntries,
                                      &dwTotalGroupEntries
                                      );
      if(nStatus != NERR_Success)
      {
        qDebug() << "Error reading groups list for "
                 << QString::fromWCharArray(usersBuf->usri0_name);
        normalUser = true;
      }
      else
      {
        for(DWORD j = 0; j < dwGroupEntries; ++j)
        {
          QString group = QString::fromWCharArray(groupsBuf->lgrui0_name);
          if(group == tr("Użytkownicy") || group == tr("Administratorzy"))
          {
            normalUser = true;
            break;
          }
          ++groupsBuf;
        }
      }
      if(normalUser)
      {
        qDebug() << "User added to list: "
                 << QString::fromWCharArray(usersBuf->usri0_name);
        usersList.push_back(QString::fromWCharArray(usersBuf->usri0_name));
      }
      ++usersBuf;
    }
  }
  else
  {
    qDebug() << "Error reading users list!";
    messages->critical(Messages::ErrorReadingUsers);
  }
  return usersList;
}

void TimeGuard::on_adminLoggingButton_clicked()
{
  loggedAsAdmin ? logoffAdmin() : adminLoginDialog->exec();
}

void TimeGuard::on_changePasswordButton_clicked()
{
  changeAdminPassword();
}

void TimeGuard::addUsersToChooseUserBox()
{
  QStringList usersList = getUsersList();
  for(QString username : usersList)
    ui->chooseUserBox->addItem(username);
}

void TimeGuard::on_saveTimeLimitButton_clicked()
{
  QString username = ui->chooseUserBox->currentText();
  fileManager->saveSettings(username,
                            ui->timeLimitEdit->time().toString("hh:mm:ss"),
                            FileManager::TimeLimit);
}

void TimeGuard::on_resetTimeButton_clicked()
{
  user->resetTimeRemaining();
  ui->timerLCD->resetTime(user->readTimeLimit());
}

void TimeGuard::on_resumePauseTimeButton_clicked()
{
  if(ui->timerLCD->isTimeActive())
    ui->timerLCD->pauseTime();
  else
  {
    if(setTime())
      ui->timerLCD->resumeTime();
    else
      messages->critical(Messages::LimitNotSet);
  }
  setResumePauseButtonIcon();
}

void TimeGuard::setResumePauseButtonIcon()
{
  if(ui->timerLCD->isTimeActive())
    ui->resumePauseTimeButton->setIcon(pauseIcon);
  else
    ui->resumePauseTimeButton->setIcon(resumeIcon);
}

void TimeGuard::on_changeLimitActivityButton_clicked()
{
  QString username = ui->chooseUserBox->currentText();
  QString active = "false";
  if(ui->changeLimitActivityButton->text() == "Activate")
    active = "true";
  fileManager->saveSettings(username, active, FileManager::LimitActive);
  setUiLimitActive(active == "true");
}

void TimeGuard::setUiLimitActive(bool active)
{
  QString buttonText, labelText;
  if(active)
  {
    labelText = "<html><head/><body><p><span style=\"font-size:10pt;"
                "font-weight:600; text-decoration: underline; color:#65cb00;\">"
                "Active</span></p></body></html>";
    buttonText = "Deactivate";
  }
  else
  {
    labelText = "<html><head/><body><p><span style=\"font-size:10pt; color:#ee0000;\">"
                "Not active</span></p></body></html>";
    buttonText = "Activate";
  }
  ui->limitActivityLabel->setText(labelText);
  ui->changeLimitActivityButton->setText(buttonText);
}

bool TimeGuard::setTime()
{
  if(!ui->timerLCD->isTimeSet())
  {
    if(!fileManager->settingsFileExists(user->getName()))
      return false;
    ui->timerLCD->setTime(user->getTimeRemaining(), user->getSaveTimePeriod());
  }
  return true;
}
