#ifndef ADMINLOGINDIALOG_H
#define ADMINLOGINDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QFile>
#include <QCryptographicHash>

namespace Ui {
  class AdminLoginDialog;
}

class AdminLoginDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit AdminLoginDialog(QWidget *parent = 0);
  ~AdminLoginDialog();
  
private slots:
  void on_buttonBox_clicked(QAbstractButton *button);
  void on_showCharsCheckBox_stateChanged(int state);
signals:
  void passwordAccepted();
private:
  Ui::AdminLoginDialog *ui;
  QCryptographicHash *hasher;
  QFile *passwordFile;
  QString currentPassword;

  bool isPasswordCorrect(QString password);
  void showPasswordAcceptedDialog();
  void showPasswordRejectedDialog();
  void accept();
  void close();
  void readCurrentPassword();
};

#endif // ADMINLOGINDIALOG_H
