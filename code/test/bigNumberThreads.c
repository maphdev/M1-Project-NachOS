#include "syscall.h"

void function (void* s) {
  int i;
  char c = *((char*)s);
  for(i=0; i<4; ++i)
    PutChar(c+i);

}

int main () {
  int i;
  char c1 = 'a';

  // Thread
  for(i = 0; i < 12; i++){
      ThreadCreate(function, &c1 );
  }

  return 0;
}
