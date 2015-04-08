#include "timeguardservice.h"
#include "QsLog.h"
#include "QsLogDest.h"

int main(int argc, char *argv[])
{
   TimeGuardService timeGuardService(argc, argv);

   QsLogging::Logger &logger = QsLogging::Logger::instance();
   logger.setLoggingLevel(QsLogging::DebugLevel);
   QsLogging::DestinationPtr logFileDestination(QsLogging::DestinationFactory::MakeFileDestination("D:/timeguard.log",
                                                QsLogging::DisableLogRotation));
   logger.addDestination(logFileDestination);

   return timeGuardService.exec();
}
