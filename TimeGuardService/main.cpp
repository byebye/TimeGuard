#include "timeguardservice.h"

int main(int argc, char *argv[])
{
   TimeGuardService timeGuardService(argc, argv);
   return timeGuardService.exec();
}
