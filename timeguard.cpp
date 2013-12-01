#include "timeguard.h"
#include "ui_timeguard.h"
#include "windows.h"
#include "lmcons.h"

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard)
{
  ui->setupUi(this);
  ui->userNameLabel->setText(getUserName());
  ui->timerLCD->setTime(0, 0, 5);
  ui->timerLCD->startCounter();
  fileManager = new FileManager();
  fileManager->saveToFile(getUserName(), QTime::currentTime().toString("hh:mm:ss"));
}

TimeGuard::~TimeGuard()
{
  delete ui;
}

QString TimeGuard::getUserName()
{
  DWORD ULEN = UNLEN+1;
  TCHAR username[UNLEN+1];
  GetUserName(username, &ULEN);
  return QString(QString::fromWCharArray(username));
}

void TimeGuard::on_logOffButton_clicked()
{
  ExitWindowsEx(EWX_FORCE, 0);
}

