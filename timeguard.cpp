#include <QMessageBox>
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
  admin = new Admin(this);
  user = new User(this, fileManager, logger);

  ui->setupUi(this);
  logoffAdmin();
  ui->userNameLabel->setText(user->getName());
  ui->logBrowser->setPlainText(fileManager->readStats(user->getName()));
  ui->tabWidget->setCurrentIndex(0);

  ui->timerLCD->setTime(user->getTimeRemaining(), user->getSaveTimePeriod());
  connect(ui->timerLCD, SIGNAL(timeout()), this, SLOT(userTimeout()));
  connect(ui->timerLCD, SIGNAL(saveTimeMoment()), user, SLOT(saveTimeRemaining()));

  programIcon = QIcon("images/timeguard.png");
  this->setWindowIcon(programIcon);
  setTrayIcon();

  adminLoginDialog = new AdminLoginDialog(this, admin);
  connect(adminLoginDialog, SIGNAL(passwordAccepted()),
          this, SLOT(adminSuccesfullyLogged()));
}

TimeGuard::~TimeGuard()
{
  delete ui;
  delete fileManager;
  delete logger;
  delete admin;
  delete user;
  delete trayIcon;
  delete trayContextMenu;
  delete quitAct;
  delete lengthenAct;
  delete adminLoginDialog;
}

void TimeGuard::userTimeout()
{
  QMessageBox::information(NULL,
                           "Koniec czasu!",
                           "Czas się skończył!",
                           QMessageBox::Ok);
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
  QMessageBox::information(this,
                           tr("Lengthening time"),
                           tr("Time to lengthen the limit: "),
                           QMessageBox::Ok, QMessageBox::Cancel);
}

void TimeGuard::on_tabWidget_currentChanged(int tabIndex)
{
  if(tabIndex == 1 && !loggedAsAdmin)
  {
    adminLoginDialog->exec();
    if(!loggedAsAdmin)
      ui->tabWidget->setCurrentIndex(0);
    else
      addUsersToChooseUserBox();
  }
}

void TimeGuard::adminSuccesfullyLogged()
{
  loggedAsAdmin = true;
  ui->adminLoggedNotification->show();
  ui->adminLogoffButton->show();
}

void TimeGuard::logoffAdmin()
{
  loggedAsAdmin = false;
  ui->adminLoggedNotification->hide();
  ui->adminLogoffButton->hide();
  ui->tabWidget->setCurrentIndex(0);
}

void TimeGuard::changeAdminPassword()
{
  if(admin->isPasswordCorrect(ui->currentPasswordField->text()))
  {
    QString newPasword = ui->newPasswordField->text();
    if(newPasword == ui->newPaswordRepeatField->text())
    {
      if(admin->isPasswordCorrect(newPasword))
        QMessageBox::critical(this,
                              "",
                              tr("New password identical as the current! Use another one."),
                              QMessageBox::Ok);
      else
      {
        admin->changePassword(newPasword);
        QMessageBox::information(this,
                            "",
                            tr("Password has been changed!"),
                            QMessageBox::Ok);
        ui->currentPasswordField->clear();
        ui->newPasswordField->clear();
        ui->newPaswordRepeatField->clear();
      }
    }
    else
      QMessageBox::critical(this,
                            "",
                            tr("Passwords don't match!"),
                            QMessageBox::Ok);
  }
  else
  {
    QMessageBox::critical(this,
                          "",
                          tr("Password incorrect!"),
                          QMessageBox::Ok);
  }
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
    QMessageBox::critical(this,
                          "",
                          tr("Error reading users list!"),
                          QMessageBox::Ok);
  }
  return usersList;
}

void TimeGuard::on_adminLogoffButton_clicked()
{
  logoffAdmin();
}

void TimeGuard::on_changePasswordButton_clicked()
{
  changeAdminPassword();
}

void TimeGuard::on_timeEdit_timeChanged(const QTime &time)
{
  saveTimeLimit(time);
}

void TimeGuard::saveTimeLimit(const QTime &time)
{
  QString username = ui->chooseUserBox->currentText();
//  fileManager->saveSettings()
}

void TimeGuard::addUsersToChooseUserBox()
{
  QStringList usersList = getUsersList();
  for(QString username : usersList)
    ui->chooseUserBox->addItem(username);
}

void TimeGuard::on_saveTimeLimitButton_clicked()
{

}
