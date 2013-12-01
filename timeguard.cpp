#include "timeguard.h"
#include "ui_timeguard.h"
#include "windows.h"
#include "lmcons.h"
#include "countdownTimer.h"

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard)
{
  ui->setupUi(this);
  ui->loggedUsername->setPlainText(getUserName());
  ui->timeRemainingLCD->setTime(1, 1);
  ui->timeRemainingLCD->startCounter();
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

