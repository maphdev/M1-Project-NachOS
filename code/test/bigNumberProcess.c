#include "syscall.h"

main()
{
  int i;
  for(i = 0; i < 12; i++) {
      ForkExec("./test/bigNumberThreads");
  }
}
