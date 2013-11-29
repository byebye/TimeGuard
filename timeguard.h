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
  
private:
  Ui::TimeGuard *ui;
};

#endif // TIMEGUARD_H
