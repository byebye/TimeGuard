#ifndef ADMIN_H
#define ADMIN_H

#include <QObject>
#include <QCryptographicHash>
#include <QFile>
#include <QString>

class Admin : public QObject
{
  Q_OBJECT
public:
  explicit Admin(QObject *parent = 0);
  ~Admin();
  bool isPasswordCorrect(QString password);
  void changePassword(QString newPassword);
signals:
  
public slots:

private:
  QCryptographicHash *hasher;
  QFile *passwordFile;
  QString passwordHash;
  
  void readCurrentPassword();
  QString hashPassword(QString password);
};

#endif // ADMIN_H
