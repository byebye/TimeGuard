#include "AdminLoginDialog.h"
#include "ui_AdminLoginDialog.h"

AdminLoginDialog::AdminLoginDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AdminLoginDialog)
{
  ui->setupUi(this);
}

AdminLoginDialog::~AdminLoginDialog()
{
  delete ui;
}
