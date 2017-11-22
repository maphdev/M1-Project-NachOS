#include "syscall.h"

static sem_t a;

void function (void* s) {
  int i;
  char c;
  P(a);
  c = *((char*)s);
  for(i = 0; i < 2; ++i)
    PutChar(c+i);
  V(a);
}

int main () {
  int i;
  char c1 = 'a';
  char c2 = 'A';

  a = SemaphoreCreate(1);

  PutString("Demarrage du main...\n");

  // Tread 1
  ThreadCreate(function, &c1 );

  // Thread 2
  ThreadCreate(function, &c2 );

  PutChar('a');

  //SemaphoreDelete(a);
  return 0;
}
