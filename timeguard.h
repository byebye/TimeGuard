#ifndef TIMEGUARD_H
#define TIMEGUARD_H

#include <QMainWindow>

namespace Ui {
  class TimeGuard;
}

class TimeGuard : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit TimeGuard(QWidget *parent = 0);
  ~TimeGuard();
  
private slots:
  void on_logOffButton_clicked();

private:
  Ui::TimeGuard *ui;
  QString getUserName();
  void startTimer();
};

#endif // TIMEGUARD_H
