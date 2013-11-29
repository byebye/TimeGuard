#include "timeguard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  TimeGuard w;
  w.show();
  
  return a.exec();
}
