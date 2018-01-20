#ifdef CHANGED

#include "userthread.h"
#include "system.h"
#include "synch.h"
#include "bitmap.h"

//----------------------------------------------------------------------
// Schmurtz
//
//    Structure permettant de passer à la fois l'adresse de la fonction
//    et l'adresse des arguments à StartUserThread.
//----------------------------------------------------------------------

struct Package {
  int f;
  int arg;
  int ret;
};

static int nbThreads = 1;
static Semaphore *mutexNbThreads = new Semaphore("mutex nb threads", 1);

//----------------------------------------------------------------------
// StartUserThread
//
//
//----------------------------------------------------------------------

static void StartUserThread(void *recup){
  // initialise tous les registres à 0
  for (int i = 0; i < NumTotalRegs; i++) {
    machine->WriteRegister (i, 0);
    DEBUG('x', "Reg[%d] = %d\n", i, machine->ReadRegister(i));
  }

  // initial program counter : fonction f
  machine->WriteRegister (PCReg, ((Package*)recup)->f);
  DEBUG('x', "PCReg = %d\n", machine->ReadRegister(PCReg));

  // write arguments in reg 4 or f will not work
  machine->WriteRegister (4, ((Package*)recup)->arg);
  DEBUG('x', "arg = %d\n", machine->ReadRegister(4));

  // where the next instruction is (usually 4 oct after PCReg)
  machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
  DEBUG('x', "NextPCReg = %d\n", machine->ReadRegister(NextPCReg));

  machine->WriteRegister (RetAddrReg, ((Package*)recup)->ret);

  // must be the end of
  int userStack = currentThread->space->AllocateUserStack ();
  machine->WriteRegister (StackReg, userStack);
  DEBUG('x', "StackReg = %d\n", userStack);

  machine->Run();
}

//----------------------------------------------------------------------
// do_ThreadCreate
//
//    Appelée par l'appel système ThreadCreate.
//
//    Créé un thread avec la fonction située à l'addr f et les args situés
//    à l'addr arg.
//
//    Retourne -1 en cas d'erreur de création de thread.
//----------------------------------------------------------------------

int do_ThreadCreate(int f, int arg, int ret){
  mutexNbThreads->P ();
  nbThreads++;
  mutexNbThreads->V ();

  Thread* newThread = new Thread("userThread");

  if (newThread == NULL) {
    return -1;
  }

  Package* schmurtz = new Package;
  schmurtz->f = f;
  schmurtz->arg = arg;
  schmurtz->ret = ret;

  newThread->Start(StartUserThread, schmurtz);
  currentThread->Yield();

  return 0;
}

//----------------------------------------------------------------------
// do_TreadExit
//
//
//----------------------------------------------------------------------

void do_ThreadExit(){
  DEBUG('x', "ThreadExit");
  mutexNbThreads->P ();
  --nbThreads;
  mutexNbThreads->V ();

  if (nbThreads > 0) {
    currentThread->space->DeAllocateUserStack(machine->ReadRegister(StackReg));
    currentThread->Finish();
  } else {
    interrupt->Halt();
  }
  //machine->Run();
}

//----------------------------------------------------------------------
//
// Semaphores pour utilisateurs
//
//----------------------------------------------------------------------

static Semaphore *semTab[256];
static BitMap *bitmap = new BitMap(256);
static Semaphore *mutexBitmap = new Semaphore("mutex Bitmap", 1);

int do_SemaphoreCreate(int nbCredit){
  // on cherche une place grâce à bitmap, renvoit -1 si plus de place
  mutexBitmap->P ();
  int idSem = bitmap->Find();
  // si erreur retourne -1
  if(idSem == -1){
    mutexBitmap->V ();
    return -1;
  }
  // création du sémaphore utilisateur
  Semaphore *semUser = new Semaphore("semUser", nbCredit);
  // remplit le bitmap et le tableau de sémaphores
  semTab[idSem] = semUser;
  // retourne l'indice du sémaphore
  mutexBitmap->V ();

  return idSem;
}

extern void do_SemaphoreDelete(int id){
  mutexBitmap->P ();
  // on supprime le sémaphore
  delete(semTab[id]);
  // on libère la place dans le bitmap
  bitmap->Clear(id);
  mutexBitmap->V ();
  return;
}

void do_SemaphoreP(int id){
  semTab[id]->P ();
}

void do_SemaphoreV(int id){
  semTab[id]->V ();
}

#endif
