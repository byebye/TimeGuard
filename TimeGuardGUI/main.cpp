#include "timeguardmainwindow.h"
#include <QApplication>
#include "QsLog.h"
#include "QsLogDest.h"

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   QsLogging::Logger &logger = QsLogging::Logger::instance();
   logger.setLoggingLevel(QsLogging::DebugLevel);
   QsLogging::DestinationPtr logFileDestination(QsLogging::DestinationFactory::MakeFileDestination("D:/timeguard-gui.log",
                                                QsLogging::DisableLogRotation));
   QsLogging::DestinationPtr debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());
   logger.addDestination(logFileDestination);
   logger.addDestination(debugDestination);

   TimeGuardMainWindow timeGuardMainWindow;
   timeGuardMainWindow.show();

   return app.exec();
}
