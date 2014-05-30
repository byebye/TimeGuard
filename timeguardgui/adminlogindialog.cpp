#include "adminlogindialog.h"
#include "ui_adminlogindialog.h"

AdminLoginDialog::AdminLoginDialog(QWidget *parent, Messages *messages, Admin *admin) :
  QDialog(parent),
  ui(new Ui::AdminLoginDialog),
  messages(messages),
  admin(admin)
{
  ui->setupUi(this);
}

AdminLoginDialog::~AdminLoginDialog()
{
  delete ui;
  messages = NULL;
  admin = NULL;
}

void AdminLoginDialog::accept()
{
  if(admin->isPasswordCorrect(ui->adminPasswordEdit->text()))
  {
    emit passwordAccepted();
    close();
  }
  else
    messages->critical(Messages::PasswordIncorrect);
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

