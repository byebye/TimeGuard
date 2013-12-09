#include "AdminLoginDialog.h"
#include "ui_AdminLoginDialog.h"
#include <QMessageBox>

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

void AdminLoginDialog::on_buttonBox_clicked(QAbstractButton *button)
{
  if(isPasswordCorrect(ui->adminPasswordEdit->text()))
    showPasswordAcceptedDialog();
  else
    showPasswordRejectedDialog();
}

bool AdminLoginDialog::isPasswordCorrect(QString password)
{
  return password == "admin1";
}

void AdminLoginDialog::showPasswordAcceptedDialog()
{
  QMessageBox::information(this,
                           "",
                           "Hasło poprawne - witaj Adminie!",
                           QMessageBox::Ok);
}

void AdminLoginDialog::showPasswordRejectedDialog()
{
  QMessageBox::critical(this,
                        "",
                        "Hasło niepoprawne!",
                        QMessageBox::Ok);
}

void AdminLoginDialog::on_showCharsCheckBox_stateChanged(int state)
{
  switch(state)
  {
    case Qt::Unchecked:
      ui->adminPasswordEdit->setEchoMode(QLineEdit::Password);
      break;
    case Qt::Checked:
      ui->adminPasswordEdit->setEchoMode(QLineEdit::Normal);
      break;
  }
}
