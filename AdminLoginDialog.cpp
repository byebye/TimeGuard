#include "AdminLoginDialog.h"
#include "ui_AdminLoginDialog.h"
#include <QMessageBox>
#include <QDebug>

AdminLoginDialog::AdminLoginDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AdminLoginDialog)
{
  ui->setupUi(this);

  hasher = new QCryptographicHash(QCryptographicHash::Sha1);
  passwordFile = new QFile("passwd.txt");
  readCurrentPassword();
}

AdminLoginDialog::~AdminLoginDialog()
{
  delete ui;
  delete hasher;
  delete passwordFile;
}

void AdminLoginDialog::accept()
{
  if(isPasswordCorrect(ui->adminPasswordEdit->text()))
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

bool AdminLoginDialog::isPasswordCorrect(QString password)
{
  hasher->reset();
  hasher->addData(password.toUtf8());
  return QString(hasher->result().toHex()) == currentPassword;
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

void AdminLoginDialog::readCurrentPassword()
{
  if(passwordFile->open(QFile::ReadOnly))
    currentPassword = passwordFile->readLine();
  else
  {
    currentPassword = "";
    qDebug() << "Błąd przy wczytywaniu hasła!" << endl;
  }
}
