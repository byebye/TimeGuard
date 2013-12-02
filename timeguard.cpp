#include <QMessageBox>
#include "TimeGuard.h"
#include "ui_timeguard.h"

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard)
{
  programIcon = QIcon("images/timeguard.png");
  this->setWindowIcon(programIcon);

  trayIcon = new QSystemTrayIcon(programIcon, this);
  trayIcon->setVisible(true);

  fileManager = new FileManager();
  user = new User(this, fileManager);

  ui->setupUi(this);
  ui->userNameLabel->setText(user->getName());

  ui->timerLCD->setTime(user->getAvaiableTime(), user->getSaveTimePeriod());
  connect(ui->timerLCD, SIGNAL(timeout()), this, SLOT(userTimeout()));
  connect(ui->timerLCD, SIGNAL(saveTimeMoment()), user, SLOT(saveAvaiableTime()));
}

TimeGuard::~TimeGuard()
{
  delete ui;
  delete fileManager;
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

void TimeGuard::on_logOffButton_clicked()
{
  user->logOff();
}

