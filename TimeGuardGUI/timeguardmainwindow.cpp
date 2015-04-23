#include "timeguardmainwindow.h"
#include "ui_timeguardmainwindow.h"
#include <QDebug>
#include <QVariantMap>
#include <QMessageBox>
#include "systeminfo.h"

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

void TimeGuardMainWindow::on_dailyLimitSaveButton_clicked()
{
    QVariantMap dailyLimitPackage {
       {"command", "daily_limit"},
       {"username", SystemInfo::getUserName()},
       {"value", ui->dailyLimitEdit->time()}
    };
    if (communicationSocket->sendPackage(dailyLimitPackage))
       QMessageBox::information(this, tr("Daily limit - success"), tr("Daily limit successfully changed"));
    else
      QMessageBox::critical(this, tr("Daily limit - fail"), tr("Daily limit failed to be set - try again"));
}
