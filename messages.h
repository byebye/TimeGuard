#ifndef MESSAGES_H
#define MESSAGES_H

#include <QObject>
#include <QMessageBox>

class Messages : public QObject
{
  Q_OBJECT
public:
  explicit Messages(QObject *parent = 0);
  enum Message { PasswordIncorrect, PasswordEmpty, PasswordIdentical,
                 PasswordNotMatch, PasswordCorrect, PasswordChanged,
                 ErrorReadingUsers,
                 LimitNotSet,
                 UserTimeout,
                 QuestionDeleteUserFiles, FilesDeleted, UnableToDeleteFiles,
                };
  QMessageBox::StandardButton critical(QString title, QString text,
                   QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                   QWidget *parent = NULL);
  QMessageBox::StandardButton information(QString title, QString text,
                   QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                   QWidget *parent = NULL);
  QMessageBox::StandardButton critical(Message message,
                   QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                   QWidget *parent = NULL);
  QMessageBox::StandardButton information(Message message,
                   QMessageBox::StandardButtons buttons = QMessageBox::Ok,
                   QWidget *parent = NULL);
  void getTitleAndText(QString &title, QString &text, Message message);
signals:
  
public slots:
  
};

#endif // MESSAGES_H
