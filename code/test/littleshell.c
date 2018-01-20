#include "syscall.h"

#define MAX_STRING_SIZE 256

// fonction parseur de la ligne saisie
void parser (char *line, char *command, char *args) {
  int i, j;

  // récupération de la commande
  for (i = 0; line[i] != ' ' && line[i] != '\0'; ++i)
    command[i] = line[i];

  command[i] = '\0';

  // si pas d'arguments, alors dans directory actuel .
  if (line[i] == '\0') {
    args[0] = '.';
    args[1] = '\0';
    return;
  }

  // récupération des arguments
  for (j = 0, ++i; line[i] != '\0'; ++j, ++i)
    args[j] = line[i];

  args[j] = '\0';
}

// compare la commande saisie dans le shell à une commande du shell
int compare (char *commandUser, char *commandShell) {
  int i;

  for (i = 0; commandUser[i] != '\0'; ++i) {
    if (commandUser[i] != commandShell[i])
      return -1;
  }

  if(commandShell[i] == '\0')
    return 1;

  return -1;
}

// fonction d'usage
void Usage () {
  PutString("Usage :\n     command args\n");
  PutString("     ./program args\n\n");

  PutString("- help : get help about the commands\n");
  PutString("- touch <filename> : create a file named <filename>\n");
  PutString("- rm <filename> : remove the file named <filename>\n");
  PutString("- cd <dirname> : move to the directory <directory>\n");
  PutString("- mkdir <dirname>: create a directory named <dirname>\n");
  PutString("- rmdir <dirname> : delete a directory named <dirname>\n");
  PutString("- ls : list the files and directories of the current direcory\n");
  PutString("- quit : quit the shell\n");
}



int main () {
  int i;
  char line[MAX_STRING_SIZE];
  char command[MAX_STRING_SIZE];
  char args[MAX_STRING_SIZE];
  char buffer[MAX_STRING_SIZE];


  printf("Welcome to the shell.\n");

  while (1) {
    printf("\n");
    PrintCurrentDir();
    printf("$ ");
    GetString(line, MAX_STRING_SIZE);
    parser(line, command, args);

    if (command[0] == '.' && command[1] == '/') {
      for(i = 2; command[i] != '\0'; ++i)
        buffer[i-2] = command[i];
      buffer[i-2] = '\0';
      ForkExec(buffer);
      PutChar('\n');
    } else if ((i = compare(command, "help")) == 1){
        Usage ();
    } else if ((i = compare(command, "touch")) == 1){
        Touch(args);
    } else if ((i = compare(command, "rm")) == 1){
        Rm(args);
    } else if ((i = compare(command, "cd")) == 1){
        Cd(args);
    } else if ((i = compare(command, "mkdir")) == 1){
        Mkdir(args);
    } else if ((i = compare(command, "rmdir")) == 1){
        Rmdir(args);
    } else if ((i = compare(command, "echo")) == 1){
        printf("%s\n",args);
    } else if ((i = compare(command, "ls")) == 1){
        Ls();
    } else if ((i = compare(command, "quit")) == 1){
        PutString("Goodbye !");
        return 0;
    }
  }
}
