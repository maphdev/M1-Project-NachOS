#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

SynchConsole::SynchConsole(const char *in, const char *out)
{
  readAvail = new Semaphore("read avail", 0);
  writeDone = new Semaphore("write done", 0);
  console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, 0);
}

SynchConsole::~SynchConsole()
{
  delete console;
  delete writeDone;
  delete readAvail;
}

void SynchConsole::SynchPutChar(int c)
{
  console->PutChar (c);
  writeDone->P ();
}

int SynchConsole::SynchGetChar()
{

  char c;

  readAvail->P ();
  c = console->GetChar ();

  return c;
}

void SynchConsole::SynchPutString(const char s[])
{
  int i = 0;
  int length = strlen(s);

  while(i < length && s[i] != '\0'){
    SynchPutChar (s[i]);
    i++;
  }

}

void SynchConsole::SynchGetString(char *s, int n)
{
  int i = 0;
  char c;

  c = SynchGetChar ();
  while(i < n-1 && c != EOF && c != '\n'){
    s[i] = c;
    i++;
    c = SynchGetChar ();
  }
  s[i] = '\0';                                       // final character for security
}

int SynchConsole::copyStringFromMachine (int from, char *to, unsigned size)
{
  unsigned i = 0;
  int val;

  while (i < size-1){                           // size - 1 because '\0' must be written at the end
    if (machine->ReadMem(from, 1, &val)){
      if (val == '\0'){
        break;
      }else{
        to[i] = (char) val;
        from++;
      }
      i++;
    }
  }
  to[i] = '\0';
  return i;
}

void SynchConsole::copyStringToMachine (int to, char *from, unsigned size)
{
  unsigned i = 0;
  int val;

  while (i < size-1){
    if (from[i] == '\0'){
      break;
    } else {
      val = (int) from[i];
      machine->WriteMem(to+i, 1, val);
    }
    i++;
  }
  machine->WriteMem(to+i, 1, '\0');
}

void SynchConsole::SynchPutInt(int n)
{
  char buffer[MAX_STRING_SIZE];
  snprintf (buffer, MAX_STRING_SIZE, "%d", n);
  SynchPutString(buffer);
}

void SynchConsole::SynchGetInt(int *n)
{
  char buffer[11];                //size of an int : 10 + '\0'
  SynchGetString (buffer, 11);
  sscanf(buffer, "%i", n);
}

#endif // CHANGED
