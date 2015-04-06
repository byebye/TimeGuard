#include "timeguardmainwindow.h"
#include "ui_timeguardmainwindow.h"
#include <QUuid>
#include <QDebug>
#include <QMessageBox>
#include <QDataStream>
#include "windows.h"
#include "wtsapi32.h"

QString TimeGuardMainWindow::serviceServerName = "timeguardserver";

TimeGuardMainWindow::TimeGuardMainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::TimeGuardMainWindow)
{
   ui->setupUi(this);
   makeInitialConnectionWithService();
}

TimeGuardMainWindow::~TimeGuardMainWindow()
{
   delete ui;
}

void TimeGuardMainWindow::makeInitialConnectionWithService()
{
   socket = new QLocalSocket(this);
   connect(socket, SIGNAL(connected()),
           this, SLOT(initialConnectionEstablished()));
   connect(socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
           this, SLOT(connectionWithServiceError(QLocalSocket::LocalSocketError)));
   this->setEnabled(false);
//   QMessageBox::information(this, "Time Guard", "Connecting with service...");
   socket->connectToServer(serviceServerName);
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
   QDataStream out(socket);
   out.setVersion(QDataStream::Qt_5_4);
   socket->write(status.toUtf8());
}

void TimeGuardMainWindow::initialConnectionEstablished()
{
   this->setEnabled(true);
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
   WTSFreeMemory(data);
   QString status = "Session ID: " + QString::number(sessionId)
                    + " UUID: " + uuid.toString();
   ui->communicationStatus->setText(status);
}

void TimeGuardMainWindow::connectionWithServiceError(QLocalSocket::LocalSocketError socketError)
{
   QMessageBox::information(this, "Time Guard",
                            tr("Unable to connect with service"));
   this->setEnabled(true);
}

