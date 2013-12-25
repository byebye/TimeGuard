#ifndef ADMINLOGINDIALOG_H
#define ADMINLOGINDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "admin.h"
#include "messages.h"

namespace Ui {
  class AdminLoginDialog;
}

class AdminLoginDialog : public QDialog
{
  Q_OBJECT
  
public:
  explicit AdminLoginDialog(QWidget *parent = 0, Messages *messages = 0,
                            Admin *admin = 0);
  ~AdminLoginDialog();
private slots:
  void on_buttonBox_clicked(QAbstractButton *button);
  void on_showCharsCheckBox_stateChanged(int state);
signals:
  void passwordAccepted();
private:
  Ui::AdminLoginDialog *ui;
  Messages *messages;
  Admin *admin;

  void showPasswordAcceptedDialog();
  void showPasswordRejectedDialog();
  void accept();
  void close();
};

#endif // ADMINLOGINDIALOG_H
