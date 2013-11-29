#include "timeguard.h"
#include "ui_timeguard.h"

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
