#include "timeguardmainwindow.h"
#include "ui_timeguardmainwindow.h"
#include <QDebug>

TimeGuardMainWindow::TimeGuardMainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::TimeGuardMainWindow)
{
   ui->setupUi(this);
   initializeConnectionWithService();
}

TimeGuardMainWindow::~TimeGuardMainWindow()
{
   delete ui;
}

void TimeGuardMainWindow::on_connectWithService_clicked()
{
   ui->communicationStatus->setText(communicationSocket->getChannelName());
}

void TimeGuardMainWindow::initializeConnectionWithService()
{
   communicationSocket = new ServiceCommunicationSocket();
   communicationSocket->createIndividualCommunicationChannel();
}
