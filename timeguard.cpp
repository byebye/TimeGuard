#include "timeguard.h"
#include "ui_timeguard.h"
#include "windows.h"
#include "lmcons.h"

TimeGuard::TimeGuard(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::TimeGuard)
{
  ui->setupUi(this);
}

TimeGuard::~TimeGuard()
{
  delete ui;
}

QString TimeGuard::getUserName()
{
//  DWORD ULEN = UNLEN+1;
//  TCHAR username[UNLEN+1];
//  GetUserName(username, &ULEN);
//  QString name = QString::fromWCharArray(username);

  return QString(getenv("USERNAME"));
}

void TimeGuard::on_getUserNameButton_clicked()
{
  ui->loggedUsername->setPlainText(getUserName());
}
