#include "admin.h"
#include <QTextStream>
#include <QDataStream>
#include <QDebug>

Admin::Admin(QObject *parent) :
  QObject(parent)
{
  hasher = new QCryptographicHash(QCryptographicHash::Sha3_512);
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
  if(passwordFile->open(QFile::ReadWrite | QFile::Text))
    passwordHash = QTextStream(passwordFile).readLine();
  else
  {
    qDebug() << "Error opening the password file!";
    passwordHash = "";
  }
  if(passwordHash.isEmpty())
    changePassword("");
}

void Admin::changePassword(QString newPassword)
{
  passwordHash = hashPassword(newPassword);
  QTextStream fileStream(passwordFile);
  fileStream.seek(0);
  fileStream << passwordHash << '\n';
  passwordFile->flush();
}

QString Admin::hashPassword(QString password)
{
  hasher->reset();
  hasher->addData(password.toUtf8());
  return QString(hasher->result().toHex());
}


