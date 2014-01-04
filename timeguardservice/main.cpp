#include <QCoreApplication>
#include "timeguardservice.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  TimeGuardService service(L"TimeGuardService");
  ServiceBase::run(service);

  return a.exec();
}
