#include "Admin.h"
#include <QTextStream>
#include <QDataStream>
#include <QDebug>

Admin::Admin(QObject *parent) :
  QObject(parent)
{
  hasher = new QCryptographicHash(QCryptographicHash::Sha1);
  passwordFile = new QFile("passwd.txt");
  readCurrentPassword();
}

Admin::~Admin()
{
  delete hasher;
  delete passwordFile;
}

bool Admin::isPasswordCorrect(QString password)
{
  qDebug() << passwordHash << endl << hashPassword(password);
  return hashPassword(password) == passwordHash;
}

void Admin::readCurrentPassword()
{
  if(passwordFile->open(QFile::ReadWrite | QFile::Text))
    passwordHash = QTextStream(passwordFile).readLine();
  else
  {
    passwordHash = "";
    qDebug() << "Błąd przy wczytywaniu hasła!";
  }
  if(passwordHash == "")
    changePassword("");
  qDebug() << "Password read |" << passwordHash << "|";
}

void Admin::changePassword(QString newPassword)
{
  passwordHash = hashPassword(newPassword);
  QTextStream fileStream(passwordFile);
  fileStream.seek(0);
  fileStream << passwordHash << '\n';
  passwordFile->flush();
  qDebug() << "Password changed to |" << passwordHash << "|";
}

QString Admin::hashPassword(QString password)
{
  hasher->reset();
  hasher->addData(password.toUtf8());
  return QString(hasher->result().toHex());
}


