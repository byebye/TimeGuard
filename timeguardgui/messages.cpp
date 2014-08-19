#include "messages.h"

Messages::Messages(QObject *parent) :
  QObject(parent)
{
}

QMessageBox::StandardButton Messages::critical(QString title, QString text,
                        QMessageBox::StandardButtons buttons,
                        QWidget *parent)
{
  return QMessageBox::critical(parent, title, text, buttons);
}

QMessageBox::StandardButton Messages::information(QString title, QString text,
                             QMessageBox::StandardButtons buttons,
                             QWidget *parent)
{
  return QMessageBox::information(parent, title, text, buttons);
}

QMessageBox::StandardButton Messages::critical(Message message,
                        QMessageBox::StandardButtons buttons,
                        QWidget *parent)
{
  QString title, text;
  getTitleAndText(title, text, message);
  return critical(title, text, buttons, parent);
}

QMessageBox::StandardButton Messages::information(Message message,
                             QMessageBox::StandardButtons buttons,
                             QWidget *parent)
{
  QString title, text;
  getTitleAndText(title, text, message);
  return information(title, text, buttons, parent);
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
      text = tr("Password cannot be empty!");
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
    case FilesDeleted:
      title = "";
      text = tr("Files deleted succesfully");
      break;
    case UnableToDeleteFiles:
      title = "";
      text = tr("Unable to delete files! Try again or delete them manually");
      break;
    case QuestionDeleteUserFiles:
      title = "";
      text = tr("Limit will be disabled and time will be stopped.\n"
                "Are you sure (deleting files cannot be undone)?");
  }
}
