#ifdef CHANGED

#define _POSIX_SOURCE
#include <unistd.h>   //chdir
#undef _POSIX_SOURCE

#include "system.h"
#include <sys/stat.h>   //mkdir...

char* GetCurrentDir () {
  char *cwd;
  if (getcwd(cwd, 256) == NULL){
    perror("getcwd() error");
    return NULL;
  }
  else {
    return cwd;
  }
}

int do_Cd (char *dirname) {
  int ret = chdir(dirname);
  return ret;
}



int do_Mkdir (char *dirname) {
  int ret = mkdir(dirname, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  return ret;
}

int do_Rmdir (char *dirname) {
  int ret = rmdir(dirname);
  return ret;
}

int do_GetCurrentDir(char *currentDir){
  printf("%s\n", GetCurrentDir());
}

#endif
