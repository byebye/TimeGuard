#include <QMessageBox>
#include <QMenu>
#include "TimeGuard.h"
#include "ui_timeguard.h"

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard)
{
  fileManager = new FileManager();
  logger = new Logger(this, fileManager);
  user = new User(this, fileManager, logger);

  ui->setupUi(this);
  ui->userNameLabel->setText(user->getName());

  ui->timerLCD->setTime(user->getAvaiableTime(), user->getSaveTimePeriod());
  connect(ui->timerLCD, SIGNAL(timeout()), this, SLOT(userTimeout()));
  connect(ui->timerLCD, SIGNAL(saveTimeMoment()), user, SLOT(saveAvaiableTime()));

  programIcon = QIcon("images/timeguard.png");
  this->setWindowIcon(programIcon);
  setTrayIcon();
}

TimeGuard::~TimeGuard()
{
  delete ui;
  delete fileManager;
  delete logger;
  delete user;
  delete trayIcon;
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

  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this, SLOT(trayActivated(QSystemTrayIcon::ActivationReason)));

  QMenu contextMenu("Tray menu");
  contextMenu.addAction("Nic 1");
  contextMenu.addAction("Nic 2");
//  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
//          contextMenu, SLOT(show()));
  trayIcon->setContextMenu(&contextMenu);
  trayIcon->show();
}

void TimeGuard::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
  switch(reason)
  {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
      this->isHidden() ? this->show() : this->hide();
      break;
    case QSystemTrayIcon::Context:
      break;
  }
}

void TimeGuard::on_logOffButton_clicked()
{
  user->logOff();
}

