#ifndef MESSAGES_H
#define MESSAGES_H

#include <QObject>

class Messages : public QObject
{
  Q_OBJECT
public:
  explicit Messages(QObject *parent = 0);
  enum Message { PasswordIncorrect, PasswordEmpty, PasswordIdentical,
                         PasswordNotMatch, PasswordCorrect, PasswordChanged,
                         ErrorReadingUsers,
                         LimitNotSet,
                         UserTimeout };
  void critical(QString title, QString text, QWidget *parent = NULL);
  void information(QString title, QString text, QWidget *parent = NULL);
  void critical(Message message, QWidget *parent = NULL);
  void information(Message message, QWidget *parent = NULL);
  void getTitleAndText(QString &title, QString &text, Message message);
signals:
  
public slots:
  
};

#endif // MESSAGES_H
