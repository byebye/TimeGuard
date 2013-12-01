#include "TimeGuard.h"
#include "ui_timeguard.h"

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard)
{
  fileManager = new FileManager();
  user = new User(this, fileManager);
  ui->setupUi(this);
  ui->userNameLabel->setText(user->getName());


  ui->timerLCD->setTime(user->getAvaiableTime());
  connect(ui->timerLCD, SIGNAL(timeout()), this, SLOT(saveLogOffTime()));
}

TimeGuard::~TimeGuard()
{
  delete ui;
}

void TimeGuard::saveLogOffTime()
{

}

void TimeGuard::on_logOffButton_clicked()
{
  user->logOff();
}

