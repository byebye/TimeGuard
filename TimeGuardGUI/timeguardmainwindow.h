#ifndef TIMEGUARDMAINWINDOW_H
#define TIMEGUARDMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class TimeGuardMainWindow;
}

class TimeGuardMainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit TimeGuardMainWindow(QWidget *parent = 0);
   ~TimeGuardMainWindow();

private slots:
   void on_connectWithService_clicked();

private:
   Ui::TimeGuardMainWindow *ui;
};

#endif // TIMEGUARDMAINWINDOW_H
