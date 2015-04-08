#include "timeguardmainwindow.h"
#include "ui_timeguardmainwindow.h"
#include <QDebug>

TimeGuardMainWindow::TimeGuardMainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::TimeGuardMainWindow)
{
   ui->setupUi(this);
   communicationSocket = new ServiceCommunicationSocket();
   initializeConnectionWithService();
}

TimeGuardMainWindow::~TimeGuardMainWindow()
{
   delete ui;
   delete communicationSocket;
}

void TimeGuardMainWindow::on_connectWithService_clicked()
{
   initializeConnectionWithService();
}

void TimeGuardMainWindow::initializeConnectionWithService()
{
   QString status = "Connection failed!";
   if (communicationSocket->createIndividualCommunicationChannel())
      status = communicationSocket->getChannelName();
   ui->communicationStatus->setText(status);
}
