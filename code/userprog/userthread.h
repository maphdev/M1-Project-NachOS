#ifdef CHANGED

#ifndef USERTHREAD_H
#define USERTHREAD_H

extern int do_ThreadCreate(int f, int arg, int ret);
extern void do_ThreadExit();


extern int do_SemaphoreCreate(int nbCredit);
extern void do_SemaphoreDelete(int id);
extern void do_SemaphoreP(int id);
extern void do_SemaphoreV(int id);

#endif // USERTHREAD_H

#endif // CHANGED
