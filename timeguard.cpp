#include "TimeGuard.h"
#include "ui_timeguard.h"

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard)
{
  user = new User(this);

  ui->setupUi(this);
  ui->userNameLabel->setText(user->getName());

  fileManager = new FileManager();
  fileManager->saveToFile(user->getName(),
                         "login: " + QTime::currentTime().toString("hh:mm:ss"));

  QString avaiableTime = fileManager->readFromFile(user->getName());
  ui->timerLCD->setTime(avaiableTime);
  connect(ui->timerLCD, SIGNAL(timeout()), this, SLOT(saveLogOffTime()));
}

TimeGuard::~TimeGuard()
{
  delete ui;
}

void TimeGuard::saveLogOffTime()
{
  fileManager->saveToFile(user->getName(),
                        "logoff: " + QTime::currentTime().toString("hh:mm:ss"));
}

void TimeGuard::on_logOffButton_clicked()
{
  user->logOff();
}

