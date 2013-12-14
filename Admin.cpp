#include "Admin.h"
#include <QTextStream>
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
  return hashPassword(password) == passwordHash;
}

void Admin::readCurrentPassword()
{
  if(passwordFile->open(QFile::ReadOnly))
    passwordHash = passwordFile->readLine();
  else
  {
    passwordHash = hashPassword("");
    qDebug() << "Błąd przy wczytywaniu hasła!" << endl;
  }
}

void Admin::changePassword(QString newPassword)
{
  QTextStream fileStream(passwordFile);
  fileStream << hashPassword(newPassword);
}

QString Admin::hashPassword(QString password)
{
  hasher->reset();
  hasher->addData(password.toUtf8());
  return QString(hasher->result().toHex());
}


