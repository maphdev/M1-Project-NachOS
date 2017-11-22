// exception.cc
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
#include "synch.h"      // in order to use a Semaphore
#include "synchconsole.h"     // in order to use SynchConsole
#include "userthread.h"
#endif

//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2.
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions
//      are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);

    switch (which)
      {
	case SyscallException:
          {
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Halt ();
		    break;
		  }
    #ifdef CHANGED
    case SC_PutChar:
      {
        DEBUG ('s', "PutChar\n ");
        mutexWrite->P ();
        synchconsole->SynchPutChar(machine->ReadRegister (4));
        mutexWrite->V ();
        break;
      }
    case SC_PutString:
      {
        DEBUG ('s', "PutString\n ");
        mutexWrite->P ();
        char buffer[MAX_STRING_SIZE];
        int adr = machine->ReadRegister (4);
        while (synchconsole->copyStringFromMachine (adr, buffer, MAX_STRING_SIZE)){
          synchconsole->SynchPutString (buffer);
          adr = adr + strlen (buffer);
        }
        mutexWrite->V ();
        break;
      }
    case SC_GetChar:
      {
        DEBUG ('s', "GetChar\n ");
        mutexRead->P ();
        char c = synchconsole->SynchGetChar ();
        machine->WriteRegister (2, (int)c);
        mutexRead->V ();
        break;
      }
    case SC_GetString:
      {
        DEBUG ('s', "GetString\n ");
        mutexRead->P ();
        int adr = machine->ReadRegister (4);
        int n = machine->ReadRegister (5);
        char buffer[n+1];
        synchconsole->SynchGetString (buffer, n+1);
        synchconsole->copyStringToMachine (adr, buffer, n+1);
        mutexRead->V ();
        break;
      }
    case SC_PutInt:
      {
        DEBUG ('s', "PutInt\n ");
        mutexWrite->P ();
        int n = machine->ReadRegister (4);
        synchconsole->SynchPutInt (n);
        mutexWrite->V ();
        break;
      }
    case SC_GetInt:
      {
        DEBUG ('s', "GetInt\n ");
        mutexRead->P ();
        int addr = machine->ReadRegister (4);
        int n;
        synchconsole->SynchGetInt (&n);
        machine->WriteMem(addr, sizeof(int), n);
        mutexRead->V ();
        break;
      }
    case SC_Exit:
      {
        DEBUG ('s', "Exit\n ");
        //int addr = machine->ReadRegister (4);     this int is the return value
        delete mutexWrite;
        mutexWrite = NULL;
        delete mutexRead;
        mutexRead = NULL;
        interrupt->Halt ();
        break;
      }
    case SC_ThreadCreate:
      {
        DEBUG ('s', "ThreadCreate\n ");
        mutexRead->P ();
        int f = machine->ReadRegister(4);
        int arg = machine->ReadRegister(5);
        int ret = machine->ReadRegister(6);
        int returnValue = do_ThreadCreate(f, arg, ret);
        machine->WriteRegister(2, returnValue);
        mutexRead->V ();
        break;
      }
    case SC_ThreadExit:
      {
        DEBUG ('s', "ThreadExit\n ");
        do_ThreadExit();
        break;
      }
    case SC_SemaphoreCreate:
      {
        DEBUG ('s', "SemaphoreCreate\n ");
        int nbCredit = machine->ReadRegister(4);
        int semUser = do_SemaphoreCreate(nbCredit);
        machine->WriteRegister(2, semUser);
        break;
      }
    case SC_SemaphoreDelete:
      {
        DEBUG ('s', "SemaphoreDelete\n ");
        int id = machine->ReadRegister(4);
        do_SemaphoreDelete(id);
        break;
      }
    case SC_SemaphoreP:
      {
        DEBUG ('s', "SemaphoreP\n ");
        int id = machine->ReadRegister(4);
        do_SemaphoreP(id);
        break;
      }
      case SC_SemaphoreV:
      {
        DEBUG ('s', "SemaphoreV\n ");
        int id = machine->ReadRegister(4);
        do_SemaphoreV(id);
        break;
      }
    #endif
		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!type) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", type, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }

	default:
	  printf ("Unexpected user mode exception %d %d at PC %x\n", which, type, machine->registers[PCReg]);
	  ASSERT (FALSE);
      }
}
