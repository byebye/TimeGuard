#ifndef TIMEGUARDMAINWINDOW_H
#define TIMEGUARDMAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

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
   void initialConnectionEstablished();
   void connectionWithServiceError(QLocalSocket::LocalSocketError socketError);
private:
   static QString serviceServerName;
   Ui::TimeGuardMainWindow *ui;
   QLocalSocket *socket;

   void makeInitialConnectionWithService();
};

#endif // TIMEGUARDMAINWINDOW_H
