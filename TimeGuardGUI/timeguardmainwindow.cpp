#include "timeguardmainwindow.h"
#include "ui_timeguardmainwindow.h"
#include <QUuid>
#include <QDebug>
#include "windows.h"
#include "wtsapi32.h"

TimeGuardMainWindow::TimeGuardMainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::TimeGuardMainWindow)
{
   ui->setupUi(this);
}

TimeGuardMainWindow::~TimeGuardMainWindow()
{
   delete ui;
}

void TimeGuardMainWindow::on_connectWithService_clicked()
{
   QUuid uuid = QUuid::createUuid();
   qDebug() << "UUID: " << uuid.toString();
   LPTSTR data = NULL;
   DWORD bytesReturned = 0;
   unsigned long sessionId = 0;
   if(WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
                              WTS_CURRENT_SESSION,
                              WTSSessionId,
                              &data,
                              &bytesReturned)) {
      sessionId = *data;
      qDebug() << "Session: " << sessionId;
   }
   QString status = "Session ID: " + QString::number(sessionId)
                    + " UUID: " + uuid.toString();
   ui->communicationStatus->setText(status);
   WTSFreeMemory(data);
}
