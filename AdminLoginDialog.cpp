#include "AdminLoginDialog.h"
#include "ui_AdminLoginDialog.h"
#include <QMessageBox>

AdminLoginDialog::AdminLoginDialog(QWidget *parent, Admin *admin) :
  QDialog(parent),
  ui(new Ui::AdminLoginDialog),
  admin(admin)
{
  ui->setupUi(this);
}

AdminLoginDialog::~AdminLoginDialog()
{
  delete ui;
  admin = NULL;
}

void AdminLoginDialog::accept()
{
  if(admin->isPasswordCorrect(ui->adminPasswordEdit->text()))
  {
    emit passwordAccepted();
    showPasswordAcceptedDialog();
    close();
  }
  else
    showPasswordRejectedDialog();
}

void AdminLoginDialog::on_buttonBox_clicked(QAbstractButton *button)
{
  if(ui->buttonBox->standardButton(button) == QDialogButtonBox::Cancel)
    close();
}

void AdminLoginDialog::close()
{
  ui->adminPasswordEdit->clear();
  ui->adminPasswordEdit->setEchoMode(QLineEdit::Password);
  QDialog::close();
}

void AdminLoginDialog::showPasswordAcceptedDialog()
{
  QMessageBox::information(this,
                           "",
                           tr("Password correct - welcome Admin!"),
                           QMessageBox::Ok);
}

void AdminLoginDialog::showPasswordRejectedDialog()
{
  QMessageBox::critical(this,
                        "",
                        tr("Password incorrect!"),
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
