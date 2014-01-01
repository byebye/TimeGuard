#include "timeguard.h"
#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setQuitOnLastWindowClosed(false);

  TimeGuard timeGuard;
  timeGuard.show();

  QObject::connect(&timeGuard, SIGNAL(programQuit()), &app, SLOT(quit()));
  
  return app.exec();
}
