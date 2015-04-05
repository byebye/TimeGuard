#include "timeguardmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   TimeGuardMainWindow timeGuardMainWindow;
   timeGuardMainWindow.show();

   return app.exec();
}
