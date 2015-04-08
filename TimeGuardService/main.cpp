#include "timeguardservice.h"
#include "QsLog.h"
#include "QsLogDest.h"

int main(int argc, char *argv[])
{
   TimeGuardService timeGuardService(argc, argv);

   QsLogging::Logger &logger = QsLogging::Logger::instance();
   logger.setLoggingLevel(QsLogging::DebugLevel);
   QsLogging::DestinationPtr logFileDestination(QsLogging::DestinationFactory::MakeFileDestination(
                                                   "D:/timeguard.log",
                                                   QsLogging::EnableLogRotation,
                                                   QsLogging::MaxSizeBytes(20480),
                                                   QsLogging::MaxOldLogCount(5))
                                                );
   logger.addDestination(logFileDestination);

   return timeGuardService.exec();
}
