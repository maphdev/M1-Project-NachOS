#include "syscall.h"
#define THIS "c"
#define THAT "d"

const int N = 10; // Choose it large enough!

void puts(const char *s) {
  const char *p;
  for (p = s; *p != '\0'; p++)
    PutChar(*p);
}

void f(void *arg) {
  const char *s = arg;
  int i;
  for (i = 0; i < N; i++)
    puts(s);
}

int main() {
  ThreadCreate(f, THIS);
  f(THAT);
}
