#include "messages.h"
#include <QMessageBox>

Messages::Messages(QObject *parent) :
  QObject(parent)
{
}

void Messages::critical(QString title, QString text, QWidget *parent)
{
  QMessageBox::critical(parent, title, text, QMessageBox::Ok);
}

void Messages::information(QString title, QString text, QWidget *parent)
{
  QMessageBox::information(parent, title, text, QMessageBox::Ok);
}

void Messages::critical(Message message, QWidget *parent)
{
  QString title, text;
  getTitleAndText(title, text, message);
  critical(title, text, parent);
}

void Messages::information(Message message, QWidget *parent)
{
  QString title, text;
  getTitleAndText(title, text, message);
  information(title, text, parent);
}

void Messages::getTitleAndText(QString &title, QString &text, Message message)
{
  switch(message)
  {
    case PasswordIncorrect:
      title = "";
      text = tr("Password incorrect!");
      break;
    case PasswordEmpty:
      title = "";
      text = tr("Password can not be empty!");
      break;
    case PasswordIdentical:
      title = "";
      text = tr("New password identical as the current! Use another one.");
      break;
    case PasswordNotMatch:
      title = "";
      text = tr("Passwords don't match!");
      break;
    case PasswordCorrect:
      title = "";
      text = tr("Password correct - welcome Admin!");
      break;
    case PasswordChanged:
      title = "";
      text = tr("Password has been changed!");
      break;
    case ErrorReadingUsers:
      title = "";
      text = tr("Error reading users list!");
      break;
    case LimitNotSet:
      title = "";
      text = tr("Limit is not set!");
      break;
    case UserTimeout:
      title = tr("Timeout");
      text = tr("Timeout - you will be logged out!");
      break;
  }
}
