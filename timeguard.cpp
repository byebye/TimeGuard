#include <QMessageBox>
#include <QMenu>
#include <QDebug>
#include "TimeGuard.h"
#include "ui_timeguard.h"

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

  ui->timerLCD->setTime(user->getAvaiableTime(), user->getSaveTimePeriod());
  connect(ui->timerLCD, SIGNAL(timeout()), this, SLOT(userTimeout()));
  connect(ui->timerLCD, SIGNAL(saveTimeMoment()), user, SLOT(saveAvaiableTime()));

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
                           "Przedłużanie czasu",
                           "Podaj czas o jaki przedłużyć limit",
                           QMessageBox::Ok, QMessageBox::Cancel);
}

void TimeGuard::on_tabWidget_currentChanged(int tabIndex)
{
  if(tabIndex == 1 && !loggedAsAdmin)
  {
    adminLoginDialog->exec();
    if(!loggedAsAdmin)
      ui->tabWidget->setCurrentIndex(0);
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
    if(ui->newPasswordField->text() == ui->newPaswordRepeatField->text())
    {
      admin->changePassword(ui->newPasswordField->text());
      QMessageBox::information(this,
                            "",
                            "Hasło zmienione!",
                            QMessageBox::Ok);
      ui->currentPasswordField->clear();
      ui->newPasswordField->clear();
      ui->newPaswordRepeatField->clear();
    }
    else
      QMessageBox::critical(this,
                            "",
                            "Hasła się nie zgadzają!",
                            QMessageBox::Ok);
  }
  else
  {
    QMessageBox::critical(this,
                          "",
                          "Hasło niepoprawne!",
                          QMessageBox::Ok);
  }
}

void TimeGuard::on_adminLogoffButton_clicked()
{
  logoffAdmin();
}

void TimeGuard::on_changePasswordButton_clicked()
{
  changeAdminPassword();
}
