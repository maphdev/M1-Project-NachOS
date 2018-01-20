#include "syscall.h"

#define N 100                         /* nombre de places dans le tampon */

static sem_t mutex;
static sem_t vide;
static sem_t plein;
static char buffer[10];
static int i = 0;

void producteur (void* s) {
  char c;
  while(1){
    c = GetChar();
    P (vide);
    P (mutex);
    buffer[i] = c;
    i++;
    V (mutex);
    V (plein);
  }
}

void consommateur (void* s) {
  char c;
  while(1){
    P (plein);
    P (mutex);
    PutChar(buffer[i]);
    i--;
    V (mutex);
    V (vide);
  }
}

int main () {

  mutex = SemaphoreCreate(1);
  vide = SemaphoreCreate(N);
  plein = SemaphoreCreate(0);

  PutString("Demarrage du main...\n");

  // Tread 1
  ThreadCreate(producteur, "" );

  // Thread 2
  ThreadCreate(consommateur, "");

  return 0;
}
