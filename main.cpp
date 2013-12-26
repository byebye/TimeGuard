#include "timeguard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  TimeGuard timeGuard;
  timeGuard.show();
  
  return app.exec();
}
