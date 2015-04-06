#ifndef TIMEGUARDMAINWINDOW_H
#define TIMEGUARDMAINWINDOW_H

#include <QMainWindow>
#include "servicecommunicationsocket.h"

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
   ServiceCommunicationSocket *communicationSocket;

   void initializeConnectionWithService();
};

#endif // TIMEGUARDMAINWINDOW_H
